// BitmapBaseEx.cpp: implementation of the CBitmapBaseEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BitmapBaseEx.h"

#undef abs
#include "math.h"
#include "G_routines.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBitmapBaseEx::CBitmapBaseEx()
{

}

CBitmapBaseEx::~CBitmapBaseEx()
{

}

//////////////////////////////////////////////////////////////////////
// Getters & Setters
//////////////////////////////////////////////////////////////////////

void CBitmapBaseEx::SetBitmap(const CBitmap& bitmap)
{
	m_cbmp.DeleteObject();
	m_cbmp.Attach(bitmap.m_hObject);
}

void CBitmapBaseEx::GetBitmap(CBitmap& bitmap)
{
	bitmap.Attach(m_cbmp.m_hObject);
}

int CBitmapBaseEx::GetWidth()
{
	BITMAP stBitmap = {0};
    GetObject( GetHandle(), sizeof(stBitmap), &stBitmap );
	
	return stBitmap.bmWidth;
}

int CBitmapBaseEx::GetHeight()
{
	BITMAP stBitmap = {0};
    GetObject( GetHandle(), sizeof(stBitmap), &stBitmap );
	
	return stBitmap.bmHeight;
}

int CBitmapBaseEx::IsAllocated()
{
	return m_cbmp.m_hObject != NULL;
}

//////////////////////////////////////////////////////////////////////
// Loader functions
//////////////////////////////////////////////////////////////////////

int CBitmapBaseEx::LoadBitmapFromResource(UINT uiResourceId)
{
	return m_cbmp.LoadBitmap(uiResourceId);
}

int CBitmapBaseEx::LoadBitmapFromFile(TCHAR szFileName[])
{
	HBITMAP hBitmap = (HBITMAP) ::LoadImage(AfxGetInstanceHandle(),
		szFileName, IMAGE_BITMAP, 0, 0, 
    LR_LOADFROMFILE);
	
	if (hBitmap != NULL)
	{
		m_cbmp.Attach(hBitmap);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//////////////////////////////////////////////////////////////////////
// Object manipulation functions
//////////////////////////////////////////////////////////////////////

int CBitmapBaseEx::Create(int uWidth, int uHeight)
{	 
	HDC hdc = GetDC(NULL);
	ASSERT(hdc!=NULL);
	CDC memDC;
	memDC.Attach(::CreateCompatibleDC(hdc));
	memDC.SelectObject( m_cbmp );
	
	return m_cbmp.CreateCompatibleBitmap(&memDC,uWidth,uHeight);
}

int CBitmapBaseEx::CreateMask(CBitmap& pBitmapMask,CBitmap& pBitmapMasked, COLORREF rgbTransparent)
{	 
	HBITMAP hBitmapOld,hBitmapMask,hBitmapMasked;

	hBitmapOld = GetHandle();
	hBitmapMask = CreateBitmapMask(hBitmapOld, rgbTransparent);
	hBitmapMasked = CreateBitmapMasked(hBitmapOld, hBitmapMask);

	pBitmapMask.DeleteObject();
	pBitmapMasked.DeleteObject();

	pBitmapMask.Attach(hBitmapMask);
	pBitmapMasked.Attach(hBitmapMasked);

	return TRUE;
}

int CBitmapBaseEx::Copy(CBitmapBaseEx& pBitmap)
{
	HBITMAP hBitmapOld,hBitmapNew;

	hBitmapOld = pBitmap.GetHandle();
	hBitmapNew = CloneBitmap(hBitmapOld);

	m_cbmp.DeleteObject();

	return m_cbmp.Attach(hBitmapNew);
}

int CBitmapBaseEx::Free()
{
	return m_cbmp.DeleteObject();
}

//////////////////////////////////////////////////////////////////////
// Graphic operation functions
//////////////////////////////////////////////////////////////////////

int CBitmapBaseEx::Draw(HDC hDC, LPRECT prcDst)
{
	int targetWidth = prcDst->right - prcDst->left;
	int targetHeight = prcDst->bottom - prcDst->top;

	// Create a memory DC compatible with the display to hold transformed bitmap
	HDC hdcMem = CreateCompatibleDC(hDC);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, m_cbmp);

	//Set stretching mode to perform block scaling
	SetStretchBltMode(hDC,HALFTONE);

	//perform actual blitting
	StretchBlt(hDC, prcDst->left, prcDst->top,targetWidth, targetHeight, hdcMem, 0,0, GetWidth(),GetHeight(), SRCCOPY); 

	//Rest to avoid brush errors
	SetBrushOrgEx(hDC,0,0,NULL);

	SelectObject(hdcMem, hOldBitmap);
    
	DeleteDC(hdcMem);

	return TRUE;
}


int CBitmapBaseEx::DrawTransparent(HDC hDC, LPRECT prcDst,CBitmap& bmpMask,CBitmap& bmpMasked)
{
	int targetWidth = prcDst->right - prcDst->left;
	int targetHeight = prcDst->bottom - prcDst->top;

	// Create a memory DC compatible with the display to hold transformed bitmap
	HDC hdcMask = CreateCompatibleDC(hDC);
	HDC hdcMasked = CreateCompatibleDC(hDC);

	HBITMAP hBitmapMask = (HBITMAP)SelectObject(hdcMask, bmpMask);
	HBITMAP hBitmapMasked = (HBITMAP)SelectObject(hdcMasked, bmpMasked);
	
	COLORREF clrOldBck = GetBkColor(hDC);
	COLORREF clrOldTxt = GetTextColor(hDC);

	// Set up destination for monochrome blt. 
	SetBkColor(hDC, RGB(255, 255, 255));    // 1s --> 0xFFFFFF
	SetTextColor(hDC, RGB(0, 0, 0));        // 0s --> 0x000000

	// Do the real work.
	StretchBlt(hDC, prcDst->left, prcDst->top,targetWidth, targetHeight, hdcMask, 0,0, GetWidth(),GetHeight(), SRCAND); 

	//Set stretching mode to perform block scaling
	SetStretchBltMode(hDC,HALFTONE);

	//Apply picture	
	StretchBlt(hDC, prcDst->left, prcDst->top,targetWidth, targetHeight, hdcMasked, 0,0, GetWidth(),GetHeight(), SRCPAINT); 

	SetBkColor(hDC, clrOldBck);    
	SetTextColor(hDC, clrOldTxt);        

	//Rest to avoid brush errors
	SetBrushOrgEx(hDC,0,0,NULL);

	SelectObject(hdcMask, hBitmapMask);
	SelectObject(hdcMasked, hBitmapMasked);
    
	DeleteDC(hdcMask);
	DeleteDC(hdcMasked);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// Private functions
//////////////////////////////////////////////////////////////////////

HBITMAP CBitmapBaseEx::CloneBitmap( HBITMAP hSourceBitmap )
{
    // Pointer to access the pixels of bitmap
    HDC hdcSrc = CreateCompatibleDC( NULL );
    HDC hdcDst = CreateCompatibleDC( NULL );
    HBITMAP hNewBitmap;
    BITMAP stBitmap = {0};
    GetObject( hSourceBitmap, sizeof(stBitmap), &stBitmap );
    hNewBitmap = CreateBitmap( stBitmap.bmWidth, stBitmap.bmHeight,
                                stBitmap.bmPlanes,stBitmap.bmBitsPixel,NULL);
    if( hNewBitmap )
    {
        SelectObject( hdcSrc, hSourceBitmap);
        SelectObject(hdcDst, hNewBitmap);
		
        // Blit existing bitmap to newly created bitmap
        BitBlt(hdcDst, 0, 0, stBitmap.bmWidth, stBitmap.bmHeight, hdcSrc, 0, 0, SRCCOPY);
    }

    // Cleanup the created Device contexts
    SelectObject(hdcSrc, 0 );
    SelectObject(hdcDst, 0 );
    DeleteDC(hdcSrc);
    DeleteDC(hdcDst);

    return hNewBitmap;
}

HBITMAP CBitmapBaseEx::CreateBitmapMask( HBITMAP hSourceBitmap, COLORREF rgbTransparent )
{
    // Pointer to access the pixels of bitmap
    HDC hdcSrc = CreateCompatibleDC( NULL );
	HDC hdcMask = CreateCompatibleDC( NULL );

    BITMAP bm = {0};
    GetObject( hSourceBitmap, sizeof(bm), &bm );

	//Creates a monochrome bitmap of the given bitmap size
	HBITMAP	hbmMask = CreateBitmap(bm.bmWidth,bm.bmHeight,1,1,NULL);
    
	if( hbmMask )
    {
        SelectObject( hdcSrc, hSourceBitmap);
        SelectObject(hdcMask, hbmMask);
		
		COLORREF clrOldBck = GetBkColor(hdcSrc);

		//used to create the mask from the source bitmap
		SetBkColor(hdcSrc, rgbTransparent);

        // Blit existing bitmap to newly created bitmap
        BitBlt(hdcMask, 0, 0, bm.bmWidth, bm.bmHeight, hdcSrc, 0, 0, SRCCOPY);

		SetBkColor(hdcSrc, clrOldBck);
    }

    // Cleanup the created Device contexts
    SelectObject(hdcSrc, 0 );
    SelectObject(hdcMask, 0 );
    DeleteDC(hdcSrc);
    DeleteDC(hdcMask);

    return hbmMask;
}

HBITMAP CBitmapBaseEx::CreateBitmapMasked( HBITMAP hSourceBitmap, HBITMAP hMaskBitmap )
{
    // Pointer to access the pixels of bitmap
	HDC hdcMask = CreateCompatibleDC( NULL );
	HDC hdcMasked = CreateCompatibleDC( NULL );

    BITMAP bm = {0};
    GetObject( hSourceBitmap, sizeof(bm), &bm );
    
	//Create a clone of the source bitmap
	HBITMAP	hbmMasked = CloneBitmap(hSourceBitmap);
    
	if( hbmMasked )
    {
        SelectObject(hdcMask, hMaskBitmap);
		SelectObject(hdcMasked, hbmMasked);

		COLORREF clrOldBck = GetBkColor(hdcMasked);
		COLORREF clrOldTxt = GetTextColor(hdcMasked);

		// Set up destination for monochrome blt. These are the default 
		// values and may not need to be changed. They should also be 
		// restored.
		SetBkColor(hdcMasked, RGB(0, 0, 0));    // 0s --> 0x000000
		SetTextColor(hdcMasked, RGB(255, 255, 255));        // 1s --> 0xFFFFFF

		// Do the real work.
		BitBlt(hdcMasked, 0, 0, bm.bmWidth, bm.bmHeight, hdcMask, 0, 0, SRCAND);

		SetBkColor(hdcMasked, clrOldBck);    
		SetTextColor(hdcMasked, clrOldTxt);  
    }

    // Cleanup the created Device contexts
    SelectObject(hdcMask, 0 );
	SelectObject(hdcMasked, 0 );
    DeleteDC(hdcMask);
	DeleteDC(hdcMasked);

    return hbmMasked;
}