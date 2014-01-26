// VecLine.cpp: implementation of the CVecLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VecLine.h"
#include "LineTracker.h"
#include "VecContainer.h"
#include "G_Routines.h"
#include "UndoManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define NUM_OF_POINTS 2

CVecLine::CVecLine()
{
   m_uiType = VEC_LINE;

}


CVecLine::~CVecLine()
{

}

void CVecLine::Draw(CDC * pDC, DWORD dwFlags)
{
   PreperDC(pDC);
   POINT pPoints[NUM_OF_POINTS] = {(int)m_t_Rect.left, (int)m_t_Rect.top, (int)m_t_Rect.right, (int)m_t_Rect.bottom};
   pDC->Polyline(pPoints,NUM_OF_POINTS);
   RestoreDC(pDC);
}

void CVecLine::DrawShadow(CDC * pDC, DWORD dwFlags)
{
   PreperDC(pDC, TRUE);
   POINT pPoints[NUM_OF_POINTS] = {(int)m_t_Rect.left, (int)m_t_Rect.top, (int)m_t_Rect.right, (int)m_t_Rect.bottom};
   pDC->Polyline(pPoints,NUM_OF_POINTS);
   RestoreDC(pDC);
}

void CVecLine::DrawPoints(CDC * pDC)
{
   DPOINT pPoints[NUM_OF_POINTS] = {(int)m_t_Rect.left, (int)m_t_Rect.top, (int)m_t_Rect.right, (int)m_t_Rect.bottom};
   CLineTracker rt((LPCDPOINT)pPoints, NUM_OF_POINTS);
   rt.DrawDots(pDC);
}

BOOL CVecLine::Track(CDPoint point, UINT uiTool, CUndoManager* pUndoMan, CSnapper* pSnapper)
{
   DPOINT pPoints[NUM_OF_POINTS] = {(int)m_t_Rect.left, (int)m_t_Rect.top, (int)m_t_Rect.right, (int)m_t_Rect.bottom};
   CLineTracker rt((LPCDPOINT)pPoints, NUM_OF_POINTS);
   if(rt.Track(GetTopContainer()->GetWnd(), point, pSnapper))
   {
      pUndoMan->AddToStack(this, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
      CDRect drect(pPoints[0].x, pPoints[0].y, pPoints[1].x, pPoints[1].y);
      drect -= m_t_Offset;
      drect /= m_t_Scaler;
      SetRect(drect);
      return TRUE;
   }
   return FALSE;
}

BOOL CVecLine::SetCursor(CWnd* pWnd, UINT nHitTest, UINT uiTool)
{
   DPOINT pPoints[NUM_OF_POINTS] = {(int)m_t_Rect.left, (int)m_t_Rect.top, (int)m_t_Rect.right, (int)m_t_Rect.bottom};
   CLineTracker rt((LPCDPOINT)pPoints, NUM_OF_POINTS);
   rt.SetCursor(pWnd, nHitTest);
   return TRUE;

}

HitTestResoult CVecLine::HitTest(CDPoint &p)
{
   DPOINT pPoints[NUM_OF_POINTS] = {(int)m_t_Rect.left, (int)m_t_Rect.top, (int)m_t_Rect.right, (int)m_t_Rect.bottom};
   CLineTracker rt((LPCDPOINT)pPoints, NUM_OF_POINTS);
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

void CVecLine::DrawDragImage(CDC* pDC,LPDRECT lpRectNew, LPDRECT lpRectLast, LPDRECT lpRectOrigianl)
{
   CRect rect = m_t_Rect;
   LPPOINT lpPointsNew = NULL;
   LPPOINT lpPointsLast = NULL;

   if(lpRectNew!=NULL)
   {
      lpPointsNew = new POINT[NUM_OF_POINTS];
      lpPointsNew[0] = rect.TopLeft();
      lpPointsNew[1] = rect.BottomRight();

      ScaleAndOffsetPoint(&lpPointsNew[0], lpRectNew, lpRectOrigianl);
      ScaleAndOffsetPoint(&lpPointsNew[1], lpRectNew, lpRectOrigianl);
   }

   if(lpRectLast!=NULL)
   {
      lpPointsLast = new POINT[NUM_OF_POINTS];
      lpPointsLast[0] = rect.TopLeft();
      lpPointsLast[1] = rect.BottomRight();

      ScaleAndOffsetPoint(&lpPointsLast[0], lpRectLast, lpRectOrigianl);
      ScaleAndOffsetPoint(&lpPointsLast[1], lpRectLast, lpRectOrigianl);
   }

   CLineTracker::DrawDragPolyLine(pDC, lpPointsNew, 1, lpPointsLast, 1, NUM_OF_POINTS);

   if(lpPointsNew != NULL)
      delete []lpPointsNew;
   if(lpPointsLast != NULL)
      delete []lpPointsLast;

}


void CVecLine::DrawRotatedDragImage(CDC* pDC, double dAng, LPPOINT lpCenter, double dAngLast, LPPOINT lpCenterLast)
{
   CRect rect = m_t_Rect;
   LPPOINT lpPointsNew = NULL;
   LPPOINT lpPointsLast = NULL;

   if(lpCenter!=NULL)
   {
      lpPointsNew = new POINT[NUM_OF_POINTS];
      lpPointsNew[0] = rect.TopLeft();
      lpPointsNew[1] = rect.BottomRight();
      RotatePoints(lpPointsNew, NUM_OF_POINTS, dAng, lpCenter);
   }

   if(lpCenterLast!=NULL)
   {
      lpPointsLast = new POINT[NUM_OF_POINTS];
      lpPointsLast[0] = rect.TopLeft();
      lpPointsLast[1] = rect.BottomRight();
      RotatePoints(lpPointsLast, NUM_OF_POINTS, dAngLast, lpCenterLast);
   }

   CLineTracker::DrawDragPolyLine(pDC, lpPointsNew, 1, lpPointsLast, 1, NUM_OF_POINTS);

   if(lpPointsNew != NULL)
      delete []lpPointsNew;
   if(lpPointsLast != NULL)
      delete []lpPointsLast;

}
