/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// VecTextBase.cpp: implementation of the CVecTextBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VecTextBase.h"
#include "MemBuffEx.h"
#include "G_routines.h"
#include "Text_Routines.h"
#include "math.h"
#include "TextEditRect.h"
#include "VecContainer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#pragma warning(disable: 4996) //'strcpy': This function or variable may be unsafe.

CVecTextBase::CVecTextBase()
{
   memset(&m_lfFont, 0, sizeof(LOGFONT));
   m_lfFont.lfHeight = 22;
   strcpy(m_lfFont.lfFaceName, "Arial");
   m_iAlignment = AL_LEFT;
}

CVecTextBase::~CVecTextBase()
{

}

void CVecTextBase::Copy(CVecTextBase* pVec)
{
   m_strText = pVec->m_strText;
   m_iAlignment = pVec->m_iAlignment;
   memcpy(&m_lfFont, &pVec->m_lfFont, sizeof(LOGFONT));
}

void CVecTextBase::SaveToBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
{
   pMemBuff->WriteField(VF_Font, &m_lfFont, sizeof(LOGFONT));
   pMemBuff->WriteStringField(VF_Text, m_strText);

   pMemBuff->WriteIntField(VF_Algment, m_iAlignment);

   pMemBuff->WriteEndOfFields();
}

void CVecTextBase::LoadFromBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
{
   while(pMemBuff->ReadNextFieldHeader())
   {
      int id = pMemBuff->GetFieldType();
      switch(id)
      {
      case VF_Font:
         pMemBuff->Read(&m_lfFont, sizeof(LOGFONT));
         break;
      case VF_Text:
         m_strText = pMemBuff->ReadString();
         break;
      case VF_Algment:
         m_iAlignment = pMemBuff->ReadInt();
         break;
      default: 
         pMemBuff->SkipField(); 
         break;
      }
   }
}

void CVecTextBase::DrawText(CDC* pDC, LPRECT lpRect, DWORD dwFlags)
{
   CRectangle rectangle(*lpRect);
   DrawText(pDC, &rectangle, dwFlags);
}

void CVecTextBase::DrawText(CDC* pDC, LPRECTANGLE lpRectangle, DWORD dwFlags)
{
   CRectangle MappingRectangle(*lpRectangle);

   //calc the angles of the parallelogram
   double dAng = - MappingRectangle.GetAngle1();
   //double dAng2 = GetPointAng(lpTopRight, lpBottomRight)+RAD90D;

   //calc what are the width and the height of the rect polygon
   CPoint sizes = MappingRectangle.GetSizes();
   int iWidth = sizes.x;
   int iHeight = sizes.y;

   TEXTMETRIC textmetric;
   pDC->GetTextMetrics(&textmetric);

   CDPoint Yslop(MappingRectangle.BottomLeft);
   Yslop-=MappingRectangle.TopLeft;
   double dNumOfLines = iHeight/((double)textmetric.tmHeight);
   Yslop /= CDPoint(dNumOfLines,dNumOfLines);

   CDPoint Xslop(cos(dAng), -sin(dAng));

   CString sTextLeft(m_strText);
   CDPoint currentPos = MappingRectangle.TopLeft;

   if(Xslop.x/ABS(Xslop.x) != Yslop.y/ABS(Yslop.y))//if the y and x are opposite signes then
      currentPos += Yslop;

   int i = 0;
   while(i<dNumOfLines)
   {
      if(sTextLeft.GetLength()==0)
         break;
      //word wrapping
      long lLineWidth;
      int iLineWidthInChars = NumOfCharsInWidth(pDC->m_hDC, sTextLeft, sTextLeft.GetLength(), iWidth, lLineWidth);
      CString sThisLine = sTextLeft.Left(iLineWidthInChars);
      if(sThisLine.GetLength()!=sTextLeft.GetLength())
      {
         int iThisLineWidthInChars = sThisLine.ReverseFind(' ')+1;
         if(iThisLineWidthInChars>0)
            sThisLine = sTextLeft.Left(iThisLineWidthInChars);
      }

      //hundle ENTER case
      int iThisLineWidthInChars = sThisLine.Find('\n');
      if(iThisLineWidthInChars!=-1)
         sThisLine = sTextLeft.Left(iThisLineWidthInChars+1);

      sTextLeft = sTextLeft.Right(sTextLeft.GetLength()-sThisLine.GetLength());

      sThisLine.TrimRight();

      //handle special caracters cases
      sThisLine.Replace("\r","");
      sThisLine.Replace("\n","");
      sThisLine.Replace("\t","        ");//8 spaces

      lLineWidth = GetLineWidth(pDC->m_hDC, sThisLine, sThisLine.GetLength());

      //deal with aligments
      int iOffset = 0;
      if(m_iAlignment == AL_CENTER)
         iOffset = iWidth/2 - lLineWidth/2;
      else if(m_iAlignment == AL_RIGHT)
         iOffset = iWidth - lLineWidth;

      //draw
      POINT pos = currentPos;
      pos.x += (long)(iOffset*Xslop.x);
      pos.y += (long)(iOffset*Xslop.y);
      InitMul256();
      DrawRotatedText(pDC->m_hDC, (LPSTR)(LPCSTR)sThisLine, sThisLine.GetLength(), &pos, dAng, (dwFlags & DF_PRINT) != 0 ? TEXT_DRAW_POLYGON : TEXT_DRAW_ANTIALIAS);//TEXT_DRAW_BITMAP

      i++;
      currentPos += Yslop;
   }
}

BOOL CVecTextBase::GotoEditMode()
{
   CTextEditRect dlg;
   CVecBase* pVec = dynamic_cast<CVecBase*>(this);

   CRect Rect = pVec->GetRectByScreen();
   CRectangle PicRect = pVec->GetMappingRectangleByScreen().GetRectangle();
   if(PicRect.IsRectangleEmpty())
      PicRect = CRectangle(Rect);
   CRect vecRect = GetPointsBBox(PicRect.GetPoints(), PicRect.GetNumOfPoints());

   CRect clientRect;
   CWnd* pWnd = pVec->GetTopContainer()->GetWnd();
   pWnd->GetClientRect(&clientRect);
   dlg.m_Rect.IntersectRect(&clientRect, &vecRect);
   pWnd->ClientToScreen(&dlg.m_Rect);
   CDPoint Scaler = pVec->GetTopContainer()->GetScaler();

   dlg.m_strText = m_strText;

   CFont font;
   LOGFONT lf;
   GetFont(&lf);
   lf.lfHeight = (int)(lf.lfHeight * Scaler.x);
   VERIFY(font.CreateFontIndirect(&lf));
   
   dlg.m_pFont = &font;
   if(dlg.DoModal()/* == IDOK*/)
   {
      m_strText = dlg.m_strText;
      return TRUE;
   }
   return FALSE;
}
