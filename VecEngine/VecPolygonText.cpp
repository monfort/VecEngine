// VecPolygonText.cpp: implementation of the CVecPolygonText class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VecPolygonText.h"
#include "VecContainer.h"
#include "PicTracker.h"
#include "G_Routines.h"
#include "MemBuffEx.h"
#include "UndoManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVecPolygonText::CVecPolygonText()
{
   m_uiType = VEC_POLYGON_TEXT;

}

CVecPolygonText::~CVecPolygonText()
{

}

void CVecPolygonText::Draw(CDC * pDC, DWORD dwFlags)
{
   CVecPolygon::Draw(pDC, dwFlags);

   if(!m_strText.IsEmpty())
   {
      PreperDC(pDC);

      CFont font;
      CBrush brush;
      LOGFONT lf;
      GetFont(&lf);
      lf.lfHeight = (int)(lf.lfHeight * m_t_Scaler.x);
      VERIFY(font.CreateFontIndirect(&lf));
      VERIFY(brush.CreateSolidBrush(m_ForeColor));
      COLORREF oldTextCol = pDC->SetTextColor(m_ForeColor);

      CFont* OldFont = (CFont*)pDC->SelectObject(&font);
      CBrush* oldBrush = (CBrush*)pDC->SelectObject(&brush);

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

      DrawText(pDC, &PicRect, dwFlags);

      pDC->SelectObject(oldBrush);
      pDC->SelectObject(OldFont);
      pDC->SetTextColor(oldTextCol);

      pDC->RestoreDC(iDCSaved);

      RestoreDC(pDC);
   }

}

void CVecPolygonText::Copy(CVecBase* pVec)
{
   CVecPolygon::Copy(pVec);//call Parent copy routine

   CVecTextBase* pVecTextBase = dynamic_cast<CVecTextBase*>(pVec);
   if(pVecTextBase!=NULL)
   {
      CVecTextBase::Copy(pVecTextBase);
   }
}

BOOL CVecPolygonText::Track(CDPoint point, UINT uiTool, CUndoManager* pUndoMan, CSnapper* pSnapper)
{
   CDRectangle PicRect = m_t_MappingRectangle;
   if(PicRect.IsRectangleEmpty())
      PicRect = CDRectangle(m_t_Rect);

   CPicTracker PicTracker(m_t_Rect, PicRect);

   if(PicTracker.HitTest(point)!=CPicTracker::hitNothing)
   {
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

BOOL CVecPolygonText::SetCursor(CWnd* pWnd, UINT nHitTest, UINT uiTool)
{
   CPicTracker PicTracker(m_t_Rect, m_t_MappingRectangle);

   if(PicTracker.SetCursor(pWnd, nHitTest, uiTool == VEC_TOOL_ROTATE))
      return TRUE;

   return CVecPolygon::SetCursor(pWnd, nHitTest, uiTool);
}

void CVecPolygonText::DrawPoints(CDC * pDC)
{
   CVecPolygon::DrawPoints(pDC);
   
   CPicTracker PicTracker(m_t_Rect, m_t_MappingRectangle);
   PicTracker.DrawDot(pDC);
}

HitTestResoult CVecPolygonText::HitTest(CDPoint &p)
{
   CPicTracker PicTracker(m_t_Rect, m_t_MappingRectangle);
   UINT uiHit = PicTracker.HitTest(p);
   if(uiHit == CPicTracker::hitOnPoint)
      return HT_ON_MAPPING_POINT;
   if(uiHit == CPicTracker::hitInside)
      return HT_INSIDE_MAPPING;

   return CVecPolygon::HitTest(p);
}

BOOL CVecPolygonText::OnDBClick(UINT nFlags, CPoint point, CUndoManager* pUndoMan)
{
   if(!IsLocked())
   {
      CPicTracker PicTracker(m_t_Rect, m_t_MappingRectangle);
      if(PicTracker.HitTest(point) == CPicTracker::hitInside)
      {
         pUndoMan->AddToStack(this, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
         m_MappingRectangle = CDRectangle(m_Rect);
         return TRUE;
      }
   }

   pUndoMan->AddToStack(this, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
   return GotoEditMode();
}


void CVecPolygonText::SaveToBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
{
   pMemBuff->WriteNodeHeader(VEC_PARENT);
   CVecPolygon::SaveToBuffer(pMemBuff, iFlags);

   pMemBuff->WriteNodeHeader(VEC_2ND_TEXT_BASE);
   CVecTextBase::SaveToBuffer(pMemBuff, iFlags);


   pMemBuff->WriteEndOfFields();
}

void CVecPolygonText::LoadFromBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
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
         case VEC_2ND_TEXT_BASE: 
            CVecTextBase::LoadFromBuffer(pMemBuff, iFlags);
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

