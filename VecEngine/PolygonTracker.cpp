// PolygonTracker.cpp: implementation of the CPolygonTracker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PolygonTracker.h"
#include "G_Routines.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define POINT_OVER_LINE_SENSETIVITY 3


/////////////////////////////////////////////////////////////////////////////
// CPolygonTracker intitialization

CPolygonTracker::CPolygonTracker(LPCDPOINT lpPoints, UINT uiCount, int enStyle):
CLineTracker(lpPoints, uiCount, enStyle)
{

}


/////////////////////////////////////////////////////////////////////////////
// CPolygonTracker operations


UINT CPolygonTracker::HitTest(CPoint point) const
{
   UINT uiHit = CLineTracker::HitTest(point);

   if (uiHit != hitNothing)
		return uiHit;

   LPPOINT lpIntPoints = new POINT[m_uiCount];
   for(UINT ui=0; ui<m_uiCount; ui++)
      lpIntPoints[ui] = m_lpPoints[ui];

   if(G_PtInPolygon(lpIntPoints, (WORD)m_uiCount, point, NULL))
   {
      delete []lpIntPoints;
      return (UINT)hitInside;
   }
   delete []lpIntPoints;

   return (UINT)hitNothing;
}

UINT CPolygonTracker::HitTestLines(CPoint point) const
{
	for (UINT ui = 1; ui < m_uiCount; ++ui)
	{
      if(PtOnLine(point, m_lpPoints[ui-1], m_lpPoints[ui], POINT_OVER_LINE_SENSETIVITY))
         return ui-1;
	}
   if(PtOnLine(point, m_lpPoints[m_uiCount-1], m_lpPoints[0], POINT_OVER_LINE_SENSETIVITY))
      return m_uiCount-1;
   return (UINT)hitNothing;
}

void CPolygonTracker::DrawTracker(
	LPCDPOINT lpPoints, CWnd* pWndClipTo, CDC* pDC, CWnd* pWnd)
{
   LPPOINT lpIntPoints = new POINT[m_uiCount];
   for(UINT ui=0; ui<m_uiCount; ui++)
      lpIntPoints[ui] = lpPoints[ui];

   // draw it
	if (m_bFinalErase || !m_bErase)
      DrawDragPolygon(pDC, m_bFinalErase ? NULL : lpIntPoints, m_iWidth, 
                              m_PointsLast, m_iWidthLast, m_uiCount);

	// remember last points
   if(m_PointsLast==NULL)
      m_PointsLast = new POINT[m_uiCount];
	memcpy(m_PointsLast, lpIntPoints, m_uiCount*sizeof(POINT));
	m_iWidthLast = m_iWidth;

   delete []lpIntPoints;
}


void CPolygonTracker::DrawDragPolygon(CDC* pDC, LPPOINT lpPoints, int iWidth,
		LPPOINT lpPointsLast, int iWidthLast, UINT uiCount)
{
	ASSERT(lpPoints == NULL ||
		AfxIsValidAddress(lpPoints, sizeof(POINT)*uiCount, FALSE));
	ASSERT(lpPointsLast == NULL ||
		AfxIsValidAddress(lpPointsLast, sizeof(POINT)*uiCount, FALSE));

   if (lpPointsLast != NULL)
	{
	   HPEN Pen = CreatePen(PS_SOLID, iWidthLast, RGB(255,255,255));
		HGDIOBJ PenOld = SelectObject(pDC->m_hDC, Pen);
      int iRop2Old = SetROP2(pDC->m_hDC, R2_NOT);
      POINT lastpoint;
      MoveToEx(pDC->m_hDC, lpPointsLast[uiCount-1].x, lpPointsLast[uiCount-1].y, &lastpoint);
      PolylineTo(pDC->m_hDC, lpPointsLast, uiCount);
      MoveToEx(pDC->m_hDC, lastpoint.x, lastpoint.y, NULL);
      SetROP2(pDC->m_hDC, iRop2Old);
		SelectObject(pDC->m_hDC, PenOld);
      DeleteObject(Pen);
	}

   // draw into the update/new region
	if (lpPoints != NULL)
	{
	   HPEN Pen = CreatePen(PS_SOLID, iWidth, RGB(255,255,255));
		HGDIOBJ PenOld = SelectObject(pDC->m_hDC, Pen);
      int iRop2Old = SetROP2(pDC->m_hDC, R2_NOT);
      POINT lastpoint;
      MoveToEx(pDC->m_hDC, lpPoints[uiCount-1].x, lpPoints[uiCount-1].y, &lastpoint);
      PolylineTo(pDC->m_hDC, lpPoints, uiCount);
      MoveToEx(pDC->m_hDC, lastpoint.x, lastpoint.y, NULL);
      SetROP2(pDC->m_hDC, iRop2Old);
		SelectObject(pDC->m_hDC, PenOld);
      DeleteObject(Pen);
   }
}
