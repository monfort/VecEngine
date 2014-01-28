/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// VecEllipseText.cpp: implementation of the CVecEllipseText class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VecEllipseText.h"
#include "VecContainer.h"
#include "PicTracker.h"
#include "G_Routines.h"
#include "MemBuffEx.h"
#include "UndoManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVecEllipseText::CVecEllipseText()
{
   m_uiType = VEC_ELLIPSE_TEXT;

}

CVecEllipseText::~CVecEllipseText()
{

}


void CVecEllipseText::Draw(CDC * pDC, DWORD dwFlags)
{
   CVecEllipse::Draw(pDC, dwFlags);

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
      rgn.CreateEllipticRgn(Rect.left, Rect.top, Rect.right, Rect.bottom);
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

void CVecEllipseText::Copy(CVecBase* pVec)
{
   CVecEllipse::Copy(pVec);

   CVecTextBase* pVecTextBase = dynamic_cast<CVecTextBase*>(pVec);
   if(pVecTextBase!=NULL)
   {
      CVecTextBase::Copy(pVecTextBase);
   }
}

BOOL CVecEllipseText::Track(CDPoint point, UINT uiTool, CUndoManager* pUndoMan, CSnapper* pSnapper)
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

   return CVecEllipse::Track(point, uiTool, pUndoMan, pSnapper);
}

BOOL CVecEllipseText::SetCursor(CWnd* pWnd, UINT nHitTest, UINT uiTool)
{
   CPicTracker PicTracker(m_t_Rect, m_t_MappingRectangle);

   if(PicTracker.SetCursor(pWnd, nHitTest, uiTool == VEC_TOOL_ROTATE))
      return TRUE;

   return CVecEllipse::SetCursor(pWnd, nHitTest, uiTool);
}

void CVecEllipseText::DrawPoints(CDC * pDC)
{
   CVecEllipse::DrawPoints(pDC);
   
   CPicTracker PicTracker(m_t_Rect, m_t_MappingRectangle);
   PicTracker.DrawDot(pDC);
}

HitTestResoult CVecEllipseText::HitTest(CDPoint &p)
{
   CPicTracker PicTracker(m_t_Rect, m_t_MappingRectangle);
   UINT uiHit = PicTracker.HitTest(p);
   if(uiHit == CPicTracker::hitOnPoint)
      return HT_ON_MAPPING_POINT;
   if(uiHit == CPicTracker::hitInside)
      return HT_INSIDE_MAPPING;

   return CVecEllipse::HitTest(p);
}

BOOL CVecEllipseText::OnDBClick(UINT nFlags, CPoint point, CUndoManager* pUndoMan)
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

void CVecEllipseText::SaveToBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
{
   pMemBuff->WriteNodeHeader(VEC_PARENT);
   CVecEllipse::SaveToBuffer(pMemBuff, iFlags);

   pMemBuff->WriteNodeHeader(VEC_2ND_TEXT_BASE);
   CVecTextBase::SaveToBuffer(pMemBuff, iFlags);

   pMemBuff->WriteEndOfFields();
}

void CVecEllipseText::LoadFromBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
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
