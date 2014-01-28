/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// VecRectangleText.cpp: implementation of the CVecRectangleText class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VecRectangleText.h"
#include "G_routines.h"
#include "VecContainer.h"
#include "MemBuffEx.h"
#include "UndoManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVecRectangleText::CVecRectangleText()
{
   m_uiType = VEC_RECTANGLE_TEXT;

}

CVecRectangleText::~CVecRectangleText()
{

}

void CVecRectangleText::Copy(CVecBase* pVec)
{
   CVecRectangle::Copy(pVec);//call Parent copy routine

   CVecTextBase* pVecTextBase = dynamic_cast<CVecTextBase*>(pVec);
   if(pVecTextBase!=NULL)
   {
      CVecTextBase::Copy(pVecTextBase);
   }
}


void CVecRectangleText::Draw(CDC * pDC, DWORD dwFlags)
{
   CVecRectangle::Draw(pDC, dwFlags);

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

      CRect Rect = m_t_Rect;
      Rect.NormalizeRect();

      CRgn rgn;
      rgn.CreatePolygonRgn(m_t_lpIntPoints, m_uiNumOfPoints, ALTERNATE);
      int iDCSaved = pDC->SaveDC();
      pDC->SelectClipRgn(&rgn, RGN_AND);

      CFont* OldFont = (CFont*)pDC->SelectObject(&font);
      CBrush* oldBrush = (CBrush*)pDC->SelectObject(&brush);

      CRectangle rectangle(*(LPRECTANGLE)m_t_lpIntPoints);
      DrawText(pDC, &rectangle, dwFlags);
      
      pDC->SelectObject(oldBrush);
      pDC->SelectObject(OldFont);
      pDC->SetTextColor(oldTextCol);
      
      pDC->RestoreDC(iDCSaved);
   
      RestoreDC(pDC);
   }
}


BOOL CVecRectangleText::OnDBClick(UINT nFlags, CPoint point, CUndoManager* pUndoMan)
{
   pUndoMan->AddToStack(this, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
   return GotoEditMode();
}


void CVecRectangleText::SaveToBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
{
   pMemBuff->WriteNodeHeader(VEC_PARENT);
   CVecRectangle::SaveToBuffer(pMemBuff, iFlags);

   pMemBuff->WriteNodeHeader(VEC_2ND_TEXT_BASE);
   CVecTextBase::SaveToBuffer(pMemBuff, iFlags);

   pMemBuff->WriteEndOfFields();
}

void CVecRectangleText::LoadFromBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
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


