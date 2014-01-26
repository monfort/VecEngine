// VecRectanglePic.cpp: implementation of the CVecRectanglePic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "math.h"
#include "G_routines.h"
#include "VecRectanglePic.h"
#include "VecContainer.h"
#include "MemBuffEx.h"
#include "UndoManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVecRectanglePic::CVecRectanglePic()
{
   m_uiType = VEC_RECTANGLE_PIC;

}

CVecRectanglePic::~CVecRectanglePic()
{

}

void CVecRectanglePic::Copy(CVecBase* pVec)
{
   CVecRectangle::Copy(pVec);//call Parent copy routine

   CVecBitmapBase* pVecBitmapBase = dynamic_cast<CVecBitmapBase*>(pVec);
   if(pVecBitmapBase!=NULL)
   {
      CVecBitmapBase::Copy(pVecBitmapBase);
   }
}


void CVecRectanglePic::Draw(CDC * pDC, DWORD dwFlags)
{
   if(!IsTransparent())
      CVecRectangle::Draw(pDC, dwFlags);

   if(HasBitmap())
   {
      CRect Rect = m_t_Rect;
      PreperDC(pDC);

      CRgn rgn;
      rgn.CreatePolygonRgn(m_t_lpIntPoints, m_uiNumOfPoints, ALTERNATE);
      int iDCSaved = pDC->SaveDC();
      pDC->SelectClipRgn(&rgn, RGN_AND);

      CRectangle rectangle(*(LPRECTANGLE)m_t_lpIntPoints);
      DrawBitmap(pDC, &rectangle, dwFlags);

      pDC->RestoreDC(iDCSaved);

/*      CGdiObject* pOldBrush = pDC->SelectStockObject(NULL_BRUSH);
      CGdiObject* pOldPen = pDC->SelectStockObject(WHITE_PEN);
      pDC->Polygon(m_t_lpIntPoints, m_uiNumOfPoints);
      pDC->SelectObject(pOldBrush);
      pDC->SelectObject(pOldPen);*/
      RestoreDC(pDC);
   }

}


void CVecRectanglePic::SaveToBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
{
   pMemBuff->WriteNodeHeader(VEC_PARENT);
   CVecRectangle::SaveToBuffer(pMemBuff, iFlags);

   pMemBuff->WriteNodeHeader(VEC_2ND_BITMAP_BASE);
   CVecBitmapBase::SaveToBuffer(pMemBuff, iFlags);

   pMemBuff->WriteEndOfFields();
}

void CVecRectanglePic::LoadFromBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
{
   while(pMemBuff->ReadNextFieldHeader())
   {
      int id = pMemBuff->GetFieldType();
      switch(id)
      {
      case ID_FIELD_IS_NODE: 
	      switch (pMemBuff->GetNodeType())
	      {
         case VEC_PARENT: 
            CVecRectangle::LoadFromBuffer(pMemBuff, iFlags);
            break;
         case VEC_2ND_BITMAP_BASE: 
            CVecBitmapBase::LoadFromBuffer(pMemBuff, iFlags);
            break;
	      default: 
            pMemBuff->SkipField(); 
            break;
	      } 
         break;
      default: 
         pMemBuff->SkipField(); 
         break;
      }
   }
}
