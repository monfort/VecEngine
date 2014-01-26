// VecPolygonPic.cpp: implementation of the CVecPolygonPic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VecPolygonPic.h"
#include "VecContainer.h"
#include "PicTracker.h"
#include "G_Routines.h"
#include "MemBuffEx.h"
#include "UndoManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVecPolygonPic::CVecPolygonPic()
{
   m_uiType = VEC_POLYGON_PIC;

}

CVecPolygonPic::~CVecPolygonPic()
{

}

void CVecPolygonPic::Draw(CDC * pDC, DWORD dwFlags)
{
   if(!IsTransparent())
      CVecPolygon::Draw(pDC, dwFlags);

   if(HasBitmap())
   {
      PreperDC(pDC);

      CRect Rect = m_t_Rect;
      CRectangle PicRect;
      if(!m_t_MappingRectangle.IsRectangleEmpty())
         PicRect = m_t_MappingRectangle.GetRectangle();
      else
         PicRect = CRectangle(Rect);

      CRgn rgn;
      rgn.CreatePolygonRgn(m_t_lpIntPoints, m_uiNumOfPoints, ALTERNATE);//WINDING
      int iDCSaved = pDC->SaveDC();
      pDC->SelectClipRgn(&rgn, RGN_AND);

      DrawBitmap(pDC, &PicRect, dwFlags);

      pDC->RestoreDC(iDCSaved);

      RestoreDC(pDC);
   }

}

void CVecPolygonPic::Copy(CVecBase* pVec)
{
   CVecPolygon::Copy(pVec);//call Parent copy routine

   CVecBitmapBase* pVecBitmapBase = dynamic_cast<CVecBitmapBase*>(pVec);
   if(pVecBitmapBase!=NULL)
   {
      CVecBitmapBase::Copy(pVecBitmapBase);
   }
}

BOOL CVecPolygonPic::Track(CDPoint point, UINT uiTool, CUndoManager* pUndoMan, CSnapper* pSnapper)
{
   CDRectangle PicRect = m_t_MappingRectangle;
   if(PicRect.IsRectangleEmpty())
      PicRect = CDRectangle(m_t_Rect);

   CPicTracker PicTracker(m_t_Rect, PicRect);

   if(PicTracker.HitTest(point)!=CPicTracker::hitNothing)
   {
      PicTracker.SetTrackRetainProportions();
      if(PicTracker.Track(GetTopContainer()->GetWnd(), point, uiTool == VEC_TOOL_ROTATE))
      {
         pUndoMan->AddToStack(this, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
         CDRectangle PicRect = PicTracker.m_rect;
         PicRect -= m_t_Offset;
         PicRect /= m_t_Scaler;
         m_MappingRectangle = PicRect;
         return TRUE;
      }
      return FALSE;
   }

   return CVecPolygon::Track(point, uiTool, pUndoMan, pSnapper);
}

BOOL CVecPolygonPic::SetCursor(CWnd* pWnd, UINT nHitTest, UINT uiTool)
{
   CPicTracker PicTracker(m_t_Rect, m_t_MappingRectangle);

   if(PicTracker.SetCursor(pWnd, nHitTest, uiTool == VEC_TOOL_ROTATE))
      return TRUE;

   return CVecPolygon::SetCursor(pWnd, nHitTest, uiTool);
}

void CVecPolygonPic::DrawPoints(CDC * pDC)
{
   CVecPolygon::DrawPoints(pDC);
   
   CPicTracker PicTracker(m_t_Rect, m_t_MappingRectangle);
   PicTracker.DrawDot(pDC);
}

HitTestResoult CVecPolygonPic::HitTest(CDPoint &p)
{
   CPicTracker PicTracker(m_t_Rect, m_t_MappingRectangle);
   UINT uiHit = PicTracker.HitTest(p);
   if(uiHit == CPicTracker::hitOnPoint)
      return HT_ON_MAPPING_POINT;
   if(uiHit == CPicTracker::hitInside)
      return HT_INSIDE_MAPPING;

   return CVecPolygon::HitTest(p);
}

BOOL CVecPolygonPic::OnDBClick(UINT nFlags, CPoint point, CUndoManager* pUndoMan)
{
   if(IsLocked())
      return FALSE;
   CPicTracker PicTracker(m_t_Rect, m_t_MappingRectangle);
   if(PicTracker.HitTest(point) == CPicTracker::hitInside)
   {
      pUndoMan->AddToStack(this, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
      m_MappingRectangle = CDRectangle(m_Rect);
      return TRUE;
   }

   return FALSE;
}


void CVecPolygonPic::SaveToBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
{
   pMemBuff->WriteNodeHeader(VEC_PARENT);
   CVecPolygon::SaveToBuffer(pMemBuff, iFlags);

   pMemBuff->WriteNodeHeader(VEC_2ND_BITMAP_BASE);
   CVecBitmapBase::SaveToBuffer(pMemBuff, iFlags);


   pMemBuff->WriteEndOfFields();
}

void CVecPolygonPic::LoadFromBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
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
            CVecPolygon::LoadFromBuffer(pMemBuff, iFlags);
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

