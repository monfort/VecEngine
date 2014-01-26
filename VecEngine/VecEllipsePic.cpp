// VecEllipsePic.cpp: implementation of the CVecEllipsePic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VecEllipsePic.h"
#include "VecContainer.h"
#include "PicTracker.h"
#include "G_Routines.h"
#include "MemBuffEx.h"
#include "UndoManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVecEllipsePic::CVecEllipsePic()
{
   m_uiType = VEC_ELLIPSE_PIC;

}

CVecEllipsePic::~CVecEllipsePic()
{

}


void CVecEllipsePic::Draw(CDC * pDC, DWORD dwFlags)
{
   if(!IsTransparent())
      CVecEllipse::Draw(pDC, dwFlags);

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
      rgn.CreateEllipticRgn(Rect.left, Rect.top, Rect.right, Rect.bottom);
      int iDCSaved = pDC->SaveDC();
      pDC->SelectClipRgn(&rgn, RGN_AND);

      DrawBitmap(pDC, &PicRect, dwFlags);

      pDC->RestoreDC(iDCSaved);
   
      RestoreDC(pDC);
   }

}

void CVecEllipsePic::Copy(CVecBase* pVec)
{
   CVecEllipse::Copy(pVec);

   CVecBitmapBase* pVecBitmapBase = dynamic_cast<CVecBitmapBase*>(pVec);
   if(pVecBitmapBase!=NULL)
   {
      CVecBitmapBase::Copy(pVecBitmapBase);
   }
}

BOOL CVecEllipsePic::Track(CDPoint point, UINT uiTool, CUndoManager* pUndoMan, CSnapper* pSnapper)
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

   return CVecEllipse::Track(point, uiTool, pUndoMan, pSnapper);
}

BOOL CVecEllipsePic::SetCursor(CWnd* pWnd, UINT nHitTest, UINT uiTool)
{
   CPicTracker PicTracker(m_t_Rect, m_t_MappingRectangle);

   if(PicTracker.SetCursor(pWnd, nHitTest, uiTool == VEC_TOOL_ROTATE))
      return TRUE;

   return CVecEllipse::SetCursor(pWnd, nHitTest, uiTool);
}

void CVecEllipsePic::DrawPoints(CDC * pDC)
{
   CVecEllipse::DrawPoints(pDC);
   
   CPicTracker PicTracker(m_t_Rect, m_t_MappingRectangle);
   PicTracker.DrawDot(pDC);
}

HitTestResoult CVecEllipsePic::HitTest(CDPoint &p)
{
   CPicTracker PicTracker(m_t_Rect, m_t_MappingRectangle);
   UINT uiHit = PicTracker.HitTest(p);
   if(uiHit == CPicTracker::hitOnPoint)
      return HT_ON_MAPPING_POINT;
   if(uiHit == CPicTracker::hitInside)
      return HT_INSIDE_MAPPING;

   return CVecEllipse::HitTest(p);
}

BOOL CVecEllipsePic::OnDBClick(UINT nFlags, CPoint point, CUndoManager* pUndoMan)
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

void CVecEllipsePic::SaveToBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
{
   pMemBuff->WriteNodeHeader(VEC_PARENT);
   CVecEllipse::SaveToBuffer(pMemBuff, iFlags);

   pMemBuff->WriteNodeHeader(VEC_2ND_BITMAP_BASE);
   CVecBitmapBase::SaveToBuffer(pMemBuff, iFlags);

   pMemBuff->WriteEndOfFields();
}

void CVecEllipsePic::LoadFromBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
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
            CVecEllipse::LoadFromBuffer(pMemBuff, iFlags);
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
