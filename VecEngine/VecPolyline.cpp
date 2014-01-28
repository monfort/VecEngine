/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// VecPolyline.cpp: implementation of the CVecPolyline class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VecPolyline.h"
#include "LineTracker.h"
#include "VecContainer.h"
#include "G_Routines.h"
#include "UndoManager.h"
#include "MRectTracker.h"
#include "SuperTracker.h"
#include "MemBuffEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define TRACKER_STYLE (m_bTrackAsShape? ~CLineTracker::styleShowPoints : CLineTracker::styleShowPoints)

CVecPolyline::CVecPolyline()
{
   m_uiType = VEC_POLYLINE;
   m_bTrackAsShape = FALSE;
}

CVecPolyline::~CVecPolyline()
{
}

void CVecPolyline::Copy(CVecBase* pVec)
{
   CVecPointsBase::Copy(pVec);//call Parent copy routine

   CVecPolyline* pVecPolyline = dynamic_cast<CVecPolyline*>(pVec);
   if(pVecPolyline!=NULL)
   {
      m_bTrackAsShape = pVecPolyline->m_bTrackAsShape;
   }
}

void CVecPolyline::Draw(CDC * pDC, DWORD dwFlags)
{
   PreperDC(pDC);
   pDC->Polyline(m_t_lpIntPoints,m_uiNumOfPoints);
   RestoreDC(pDC);
}

void CVecPolyline::DrawShadow(CDC * pDC, DWORD dwFlags)
{
   PreperDC(pDC, TRUE);
   pDC->Polyline(m_t_lpIntPoints,m_uiNumOfPoints);
   RestoreDC(pDC);
}

void CVecPolyline::DrawPoints(CDC * pDC)
{
   if(m_bTrackAsShape)
   {
      CMRectTracker rt(m_t_Rect, CMRectTracker::resizeOutside | CMRectTracker::cornersOnly);
      rt.Draw(pDC);
   }
   else
   {
      CLineTracker rt(m_t_lpPoints,m_uiNumOfPoints, TRACKER_STYLE);
      rt.DrawDots(pDC);
   }
}

BOOL CVecPolyline::Track(CDPoint point, UINT uiTool, CUndoManager* pUndoMan, CSnapper* pSnapper)
{
   if(m_bTrackAsShape)
   {
      CPtrList list;
      list.AddHead(this);
      CSuperTracker st(list);
      if(HasPicture())
         st.SetTrackRetainProportions();
      if(st.Track(GetTopContainer()->GetWnd(), point, 
         HitTest(point) == HT_ON_POINT ? CSuperTracker::Style_Size: CSuperTracker::Style_Move, pSnapper))
      {
         pUndoMan->AddToStack(this, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
         SetRectScaleAndOffsetByScreen(st.m_rect, st.m_rectOrigianl);
         return TRUE;
      }
   }
   else
   {
      CLineTracker rt(m_t_lpPoints, m_uiNumOfPoints, TRACKER_STYLE);
      int iHit = rt.HitTestPoints(point);
      BOOL bTrackResoult;
      if(iHit >= CLineTracker::hitDot)
      {//track only the selected point and the points next to it
         int iFirstDot = iHit-1;
         int iNumOfDots = 3;
         if(iHit == 0)
         {  
            iFirstDot = 0;
            iNumOfDots = 2;
         }
         else if(iHit == (int)m_uiNumOfPoints-1)
         {
            iNumOfDots = 2;
         }

         CLineTracker localTracker(&m_t_lpPoints[iFirstDot], iNumOfDots);
         bTrackResoult = localTracker.Track(GetTopContainer()->GetWnd(), point, pSnapper);
      }
      else
         bTrackResoult = rt.Track(GetTopContainer()->GetWnd(), point, pSnapper);

      if(bTrackResoult)
      {
         pUndoMan->AddToStack(this, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
         LPCDPOINT dpoints = new CDPOINT[m_uiNumOfPoints];
         for(UINT ui = 0; ui < m_uiNumOfPoints; ui++)
         {
            dpoints[ui].x = m_t_lpPoints[ui].x;
            dpoints[ui].y = m_t_lpPoints[ui].y;
            dpoints[ui] -= m_t_Offset;
            dpoints[ui] /= m_t_Scaler;
         }
         SetPoints(dpoints, m_uiNumOfPoints);

         delete []dpoints;

         return TRUE;
      }
   }

   return FALSE;
}

BOOL CVecPolyline::SetCursor(CWnd* pWnd, UINT nHitTest, UINT uiTool)
{
   if(m_bTrackAsShape)
   {
      CPoint point;
      GetCursorPos(&point);
      pWnd->ScreenToClient(&point);

      CMRectTracker rt(m_t_Rect, CMRectTracker::resizeOutside | CMRectTracker::cornersOnly);
      int iHit = rt.HitTest(point);
      if(iHit != CMRectTracker::hitMiddle && iHit != CMRectTracker::hitNothing)
      {
         rt.SetCursor(pWnd, nHitTest);//show cursor of the rect tracker
         return TRUE;
      }
   }

   CLineTracker rt(m_t_lpPoints, m_uiNumOfPoints, TRACKER_STYLE);
   rt.SetCursor(pWnd, nHitTest);

   return TRUE;
}

HitTestResoult CVecPolyline::HitTest(CDPoint &p)
{
   if(m_bTrackAsShape)
   {
      CMRectTracker rt(m_t_Rect, CMRectTracker::resizeOutside | CMRectTracker::cornersOnly);
      int iHit = rt.HitTest(p);
      if(iHit != CMRectTracker::hitMiddle && iHit != CMRectTracker::hitNothing && IsSelected())
         return HT_ON_POINT;
   }

   CLineTracker rt(m_t_lpPoints, m_uiNumOfPoints, TRACKER_STYLE);
   int iHit = rt.HitTest(p);
   switch(iHit)
   {
   case CLineTracker::hitLine:
      return HT_ON_LINE;
   case CLineTracker::hitNothing:
      return HT_NONE;
   default:
      return HT_ON_POINT;
   }
   
   //return HT_NONE;
}




void CVecPolyline::DrawDragImage(CDC* pDC,LPDRECT lpRectNew, LPDRECT lpRectLast, LPDRECT lpRectOrigianl)
{
   CRect rect = m_t_Rect;
   LPPOINT lpPointsNew = NULL;
   LPPOINT lpPointsLast = NULL;

   if(lpRectNew!=NULL)
   {
      lpPointsNew = new POINT[m_uiNumOfPoints];
      for(UINT ui = 0; ui< m_uiNumOfPoints; ui++)
      {
         CDPoint dpoint = m_t_lpPoints[ui];
         ScaleAndOffsetPoint(&dpoint, lpRectNew, lpRectOrigianl);
         lpPointsNew[ui] = dpoint;
      }
   }

   if(lpRectLast!=NULL)
   {
      lpPointsLast = new POINT[m_uiNumOfPoints];
      for(UINT ui = 0; ui< m_uiNumOfPoints; ui++)
      {
         CDPoint dpoint = m_t_lpPoints[ui];
         ScaleAndOffsetPoint(&dpoint, lpRectLast, lpRectOrigianl);
         lpPointsLast[ui] = dpoint;
      }
   }

   CLineTracker::DrawDragPolyLine(pDC, lpPointsNew, 1, lpPointsLast, 1, m_uiNumOfPoints);

   if(lpPointsNew != NULL)
      delete []lpPointsNew;
   if(lpPointsLast != NULL)
      delete []lpPointsLast;

}


void CVecPolyline::DrawRotatedDragImage(CDC* pDC, double dAng, LPPOINT lpCenter, double dAngLast, LPPOINT lpCenterLast)
{
   CRect rect = m_t_Rect;
   LPPOINT lpPointsNew = NULL;
   LPPOINT lpPointsLast = NULL;

   if(lpCenter!=NULL)
   {
      lpPointsNew = new POINT[m_uiNumOfPoints];
   	memcpy(lpPointsNew, m_t_lpIntPoints, m_uiNumOfPoints*sizeof(POINT));
      RotatePoints(lpPointsNew, m_uiNumOfPoints, dAng, lpCenter);
   }

   if(lpCenterLast!=NULL)
   {
      lpPointsLast = new POINT[m_uiNumOfPoints];
   	memcpy(lpPointsLast, m_t_lpIntPoints, m_uiNumOfPoints*sizeof(POINT));
      RotatePoints(lpPointsLast, m_uiNumOfPoints, dAngLast, lpCenterLast);
   }

   CLineTracker::DrawDragPolyLine(pDC, lpPointsNew, 1, lpPointsLast, 1, m_uiNumOfPoints);

   if(lpPointsNew != NULL)
      delete []lpPointsNew;
   if(lpPointsLast != NULL)
      delete []lpPointsLast;

}

void CVecPolyline::SaveToBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
{
   pMemBuff->WriteNodeHeader(VEC_PARENT);
   CVecPointsBase::SaveToBuffer(pMemBuff, iFlags);

   pMemBuff->WriteBoolField(VF_TrackAsShape, m_bTrackAsShape);

   pMemBuff->WriteEndOfFields();
}

void CVecPolyline::LoadFromBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
{
   while(pMemBuff->ReadNextFieldHeader())
   {
      int id = pMemBuff->GetFieldType();
      switch(id)
      {
      case VF_TrackAsShape:
         m_bTrackAsShape = pMemBuff->ReadBool();
         break;
      case ID_FIELD_IS_NODE: 
	      switch (pMemBuff->GetNodeType())
	      {
         case VEC_PARENT: 
            CVecPointsBase::LoadFromBuffer(pMemBuff, iFlags);
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
