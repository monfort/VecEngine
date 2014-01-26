// VecBitmapBase.cpp: implementation of the CVecBitmapBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VecBitmapBase.h"
#include "MemBuffEx.h"
#include "G_routines.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVecBitmapBase::CVecBitmapBase()
{
   m_bIsTransparent = FALSE;
   m_crTransparentColor = RGB(0,0,0);

}

CVecBitmapBase::~CVecBitmapBase()
{

}

void CVecBitmapBase::SetBitmap(CBitmapBaseEx *pBitmap)
{
   if(pBitmap!=NULL)
   {
      m_bitmap.Copy(*pBitmap);
	}
   else
   {
      m_bitmap.Free();
   }

   m_mask.DeleteObject();
   m_masked.DeleteObject();
}

void CVecBitmapBase::Copy(CVecBitmapBase* pVec)
{
   m_bitmap.Copy(pVec->m_bitmap);
   m_bIsTransparent = pVec->m_bIsTransparent;
   m_crTransparentColor = pVec->m_crTransparentColor;
}

void CVecBitmapBase::SaveToBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
{
   if((iFlags & SBF_LIGHT) != SBF_LIGHT)
   {
		unsigned long bmiSize; //Info size
		unsigned long bmSize; //actual bits size

		LPBYTE lpData = GetRawBitmapData(bmiSize,bmSize);
		
		pMemBuff->WriteField(VF_Bitmap, lpData, bmiSize+bmSize);

		delete []lpData;
   }

   pMemBuff->WriteBoolField(VF_IsTransparent, m_bIsTransparent);
   pMemBuff->WriteDwordField(VF_TransparentColor, m_crTransparentColor);

   pMemBuff->WriteEndOfFields();
}

void CVecBitmapBase::LoadFromBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
{
   while(pMemBuff->ReadNextFieldHeader())
   {
      int id = pMemBuff->GetFieldType();
      int size = pMemBuff->GetFieldSize();
      switch(id)
      {
      case VF_IsTransparent:
         m_bIsTransparent = pMemBuff->ReadBool();
         break;
      case VF_TransparentColor:
         m_crTransparentColor = pMemBuff->ReadDword();
         break;
      case VF_Bitmap:
         {
			 BYTE *lpData = new BYTE[size];
			 
			 pMemBuff->Read(lpData, size);
			 
			 SetRawBitmapData(lpData, size);
			
			 delete []lpData;

			 //Clear the mask and masked bitmaps
			 m_mask.DeleteObject();
			 m_masked.DeleteObject();			 
         }
         break;
      default: 
         pMemBuff->SkipField(); 
         break;
      }
   }
}

void CVecBitmapBase::DrawBitmap(CDC* pDC, LPRECT lpRect, DWORD dwFlags)
{
   CRectangle rectangle(*lpRect);
   DrawBitmap(pDC, &rectangle, dwFlags);
}

void CVecBitmapBase::DrawBitmap(CDC* pDC, LPRECTANGLE lpRectangle, DWORD dwFlags)
{
   if(HasBitmap())
   {
		//Check if we need to create a new mask
		if (IsTransparent() && m_mask.m_hObject == NULL )
		{
			m_bitmap.CreateMask(m_mask, m_masked, GetTransparentColor());
		}

		//Get bounding box of rectangular shape
		CRect Rect = GetPointsBBox(RECTANGLE_POINTS(lpRectangle), RECTANGLE_NUM_OF_POINTS);

		//Calculate center of bounding box
		int rectXCnt = Rect.CenterPoint().x;
		int rectYCnt = Rect.CenterPoint().y;

		CRectangle MappingRectangle(*lpRectangle);
      
		//calc the angles of the transformation
		double dRotAng = -(MappingRectangle.GetAngle1());
		double dShearAng = MappingRectangle.GetAngle2() + dRotAng -PI/2;

		//Contains the world transform matrix
		XFORM xform;

		//needed prior to changing world transform
		SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);

		//Resets the world transform to entity matrix
		ModifyWorldTransform(pDC->m_hDC,NULL,MWT_IDENTITY);

		if(dRotAng !=0 || dShearAng !=0)// the bitmap is rotated or sheered 
		{
			//Translate minus center of rectangle
			ModifyWorldTransform(pDC->m_hDC, CalcTranslation(-rectXCnt,-rectYCnt, xform), MWT_RIGHTMULTIPLY);		

			//Scale the matrix to compensate for rotation and Shearing
			ModifyWorldTransform(pDC->m_hDC, CalcScale(lpRectangle, xform), MWT_RIGHTMULTIPLY);		

			//rotate and shear
			ModifyWorldTransform(pDC->m_hDC, CalcRotationAndShear(dRotAng,dShearAng, xform), MWT_RIGHTMULTIPLY);	

			//Translate plus center of rectangle
			ModifyWorldTransform(pDC->m_hDC, CalcTranslation(rectXCnt,rectYCnt, xform), MWT_RIGHTMULTIPLY);		
		}

		//Check if bitmap has transparency
        if (IsTransparent())
		{
			m_bitmap.DrawTransparent(pDC->m_hDC, &Rect, m_mask, m_masked);
		}
		else
		{
			m_bitmap.Draw(pDC->m_hDC, &Rect);
		}

		//return graphic mode to default
		SetGraphicsMode( pDC->m_hDC, GM_COMPATIBLE );
	}
}

PXFORM CVecBitmapBase::CalcRotationAndShear(double radRotation,double radShearFactor, XFORM& xform)
{
	float cosine = (float)cos(radRotation);
	float sine = (float)sin(radRotation);
	float tang = (float)tan(radShearFactor);

	xform.eM11 = cosine;
	xform.eM12 = -sine;
	xform.eM21 = sine - tang * cosine;//sine;
	xform.eM22 = cosine + tang * sine;//cosine;
	xform.eDx = (float)0.0;
	xform.eDy = (float)0.0;

	return &xform;
}

PXFORM CVecBitmapBase::CalcFlip(XFORM& xform)
{
	xform.eM11 = (float) -1.0; 
	xform.eM12 = (float) 0.0; 
    xform.eM21 = (float) 0.0; 
    xform.eM22 = (float) 1.0; 
    xform.eDx  = (float) 0.0; 
    xform.eDy  = (float) 0.0;

	return &xform;
}

PXFORM CVecBitmapBase::CalcReverse(XFORM& xform)
{
	xform.eM11 = (float) 1.0; 
	xform.eM12 = (float) 0.0; 
    xform.eM21 = (float) 0.0; 
    xform.eM22 = (float) -1.0; 
    xform.eDx  = (float) 0.0; 
    xform.eDy  = (float) 0.0;

	return &xform;
}

PXFORM CVecBitmapBase::CalcScale(LPRECTANGLE lpRectangle, XFORM& xform)
{
	double ax = (double)(lpRectangle->TopRight.x- lpRectangle->TopLeft.x);
	double bx = (double)(lpRectangle->TopRight.y- lpRectangle->TopLeft.y);
	double ay = (double)(lpRectangle->BottomLeft.x- lpRectangle->TopLeft.x);
	double by = (double)(lpRectangle->BottomLeft.y- lpRectangle->TopLeft.y);

	float cx = (float)pitagoran(ax,bx);
	float cy = (float)pitagoran(ay,by);

	CRect Rect = GetPointsBBox(RECTANGLE_POINTS(lpRectangle), RECTANGLE_NUM_OF_POINTS);

	float dx = (float)(Rect.right - Rect.left);
	float dy = (float)(Rect.bottom - Rect.top);

	float fHorizontalFactor = cx/dx;
	float fVerticalFactor = cy/dy;

	xform.eM11 = fHorizontalFactor;
	xform.eM12 = (float)0.0;
	xform.eM21 = (float)0.0;
	xform.eM22 = fVerticalFactor;
	xform.eDx = (float)0.0;
	xform.eDy = (float)0.0;

	return &xform;
}

PXFORM CVecBitmapBase::CalcTranslation(int xTrans,int yTrans, XFORM& xform)
{
	xform.eM11 = (float)1.0;
	xform.eM12 = (float)0.0; 
	xform.eM21 = (float)0.0;
	xform.eM22 = (float)1.0;
	xform.eDx = (float) xTrans;
	xform.eDy = (float) yTrans;

	return &xform;
}

PBITMAPINFO CVecBitmapBase::CreateBitmapInfoStruct(HBITMAP hBmp, unsigned long& size )
{ 
    BITMAP bmp; 
    PBITMAPINFO pbmi; 
    WORD    cClrBits; 
	BYTE *bpmiBuff;

    // Retrieve the bitmap color format, width, and height. 
    GetObject(hBmp, sizeof(BITMAP), &bmp); 
        
	//Calculate size of bitmap info structure
	cClrBits = CalcColorBits(bmp.bmPlanes,bmp.bmBitsPixel);
	size = CalcBitmapInfoStructSize(cClrBits);

    // Allocate memory for the BITMAPINFO structure. (This structure 
    // contains a BITMAPINFOHEADER structure and an array of RGBQUAD 
    // data structures.) 

	 bpmiBuff = new BYTE[size];
	 pbmi = (PBITMAPINFO)bpmiBuff;

    // Initialize the fields in the BITMAPINFO structure. 
    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
    pbmi->bmiHeader.biWidth = bmp.bmWidth; 
    pbmi->bmiHeader.biHeight = bmp.bmHeight; 
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes; 
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel; 
    if (cClrBits < 24) 
        pbmi->bmiHeader.biClrUsed = (1<<cClrBits); 

    // If the bitmap is not compressed, set the BI_RGB flag. 
    pbmi->bmiHeader.biCompression = BI_RGB; 

    // Compute the number of bytes in the array of color 
    // indices and store the result in biSizeImage. 
    // For Windows NT, the width must be DWORD aligned unless 
    // the bitmap is RLE compressed. This example shows this. 
    // For Windows 95/98/Me, the width must be WORD aligned unless the 
    // bitmap is RLE compressed.
    pbmi->bmiHeader.biSizeImage = CalcBitmapImageSize(pbmi->bmiHeader.biWidth,pbmi->bmiHeader.biHeight,cClrBits);

    // Set biClrImportant to 0, indicating that all of the 
    // device colors are important. 
     pbmi->bmiHeader.biClrImportant = 0; 

	 return pbmi; 
 }

WORD CVecBitmapBase::CalcColorBits(unsigned short biPlanes, unsigned short biBitCount)
{
// Convert the color format to a count of bits. 
    WORD cClrBits = (WORD)(biPlanes * biBitCount); 
    
	if (cClrBits == 1) 
        cClrBits = 1; 
    else if (cClrBits <= 4) 
        cClrBits = 4; 
    else if (cClrBits <= 8) 
        cClrBits = 8; 
    else if (cClrBits <= 16) 
        cClrBits = 16; 
    else if (cClrBits <= 24) 
        cClrBits = 24; 
    else cClrBits = 32; 

	return cClrBits;
}

unsigned long CVecBitmapBase::CalcBitmapInfoStructSize(WORD cClrBits)
{
     if (cClrBits != 24) 
         return sizeof(BITMAPINFOHEADER) + 
                    sizeof(RGBQUAD) * (1<< cClrBits); 
     // There is no RGBQUAD array for the 24-bit-per-pixel format. 
     else 
         return sizeof(BITMAPINFOHEADER);
}

unsigned long CVecBitmapBase::CalcBitmapImageSize(long biWidth, long biHeight, WORD cClrBits)
{
	return ((biWidth * cClrBits +31) & ~31) /8 * biHeight; 
}

LPBYTE CVecBitmapBase::GetRawBitmapData(unsigned long& bmiSize, unsigned long& bmSize)
{
		LPBYTE lpData = NULL;

		if (m_bitmap.GetHandle() == NULL)
		{
			bmiSize = 0;
			bmSize = 0;
		}
		else
		{
			PBITMAPINFO bmi = CreateBitmapInfoStruct(m_bitmap.GetHandle(), bmiSize);

			bmSize = bmi->bmiHeader.biSizeImage;

			lpData = new BYTE[bmiSize + bmSize];

			memcpy(lpData, bmi, bmiSize);

			HDC hdc = CreateCompatibleDC( NULL );
		
			GetDIBits(hdc, m_bitmap.GetHandle(), 0, (WORD) bmi->bmiHeader.biHeight, lpData + bmiSize, bmi, 
			DIB_RGB_COLORS);

			delete []bmi;

			//Clear the memory device
			SelectObject(hdc, 0 );
			DeleteDC(hdc);
		}

		return lpData;
}

void CVecBitmapBase::SetRawBitmapData(LPBYTE lpData, unsigned long size)
{
		if(lpData != NULL)
		{
			PBITMAPINFO lpbmi = (PBITMAPINFO)lpData;
			
			WORD cClrBits = CalcColorBits(lpbmi->bmiHeader.biPlanes,lpbmi->bmiHeader.biBitCount);
			unsigned long bmiSize = CalcBitmapInfoStructSize(cClrBits);

			BYTE *lpvBits = lpData + bmiSize;

 			int nWidth = lpbmi->bmiHeader.biWidth;
			int nHeight = lpbmi->bmiHeader.biHeight;
			UINT cPlanes = lpbmi->bmiHeader.biPlanes;
			UINT cBitsPerPixel = lpbmi->bmiHeader.biBitCount; 

			HDC memDC = CreateCompatibleDC ( NULL );
			HBITMAP memBM =::CreateBitmap(nWidth, nHeight,cPlanes,cBitsPerPixel, NULL);
			SelectObject(memDC, memBM);		

			int retVal;
			retVal = ::SetDIBits(
						memDC,
						memBM,
						0,
						nHeight,
						lpvBits,
						lpbmi,
						DIB_RGB_COLORS
						);			

			CBitmap cbmp;
			cbmp.Attach(memBM);
			m_bitmap.SetBitmap(cbmp);
			cbmp.Detach(); //important
							
			//Clear the memory device
			SelectObject(memDC, 0);
			DeleteDC(memDC);
		}
}