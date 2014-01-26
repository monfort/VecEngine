// RectangleTracker.cpp: implementation of the CRectangleTracker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VecTypes.h"
#include "RectangleTracker.h"
#include "PolygonTracker.h"
#include "Snapper.h"
#include "resource.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define POINT_OVER_LINE_SENSETIVITY 3


struct RECTANGLE_HANDLEINFO
{
   int Xmove;
   int Ymove;
};




// this array describes all 8 handles (clock-wise)
const RECTANGLE_HANDLEINFO aRectangleTrackerPoints[] =
{
	// corner handles (top-left, top-right, bottom-right, bottom-left
	{ 3,1 },//top left [0]
	{ 2,0 },//top right [1]
	{ 1,3 },//bottom left [2]
	{ 0,2 },//bottom right [3]
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HCURSOR CRectangleTracker::m_hPointTrackerCursor = NULL;
HCURSOR CRectangleTracker::m_hPointMoveTrackerCursor = NULL;

CRectangleTracker::CRectangleTracker(CDRectangle PicRect)
{
   m_rect = PicRect;
   m_lpPoints = (LPCDPOINT)m_rect.GetPoints();
   m_uiCount = m_rect.GetNumOfPoints();

   //calc what are the width and the height of the rect polygon
   CDPoint sizes = PicRect.GetSizes();
   if(sizes.y!=0)
      m_dWidthDivHeight = sizes.x / sizes.y;
   else
      m_dWidthDivHeight = 1;

   m_iTrackingPointY = m_iTrackingPointX = 0;
	m_RectLast.SetRectangleEmpty();
	m_bErase = FALSE;
	m_bFinalErase =  FALSE;

   m_nHandleSize = 4;

   if(m_hPointTrackerCursor == NULL)
      m_hPointTrackerCursor = AfxGetApp()->LoadCursor(IDC_MOVE4WAY);
   if(m_hPointMoveTrackerCursor == NULL)
      m_hPointMoveTrackerCursor = AfxGetApp()->LoadCursor(IDC_TRACK4WAY);
}

CRectangleTracker::~CRectangleTracker()
{
}

void CRectangleTracker::GetPointRect(UINT uiPoint, CRect* pHandleRect) const
{
	ASSERT(uiPoint < m_uiCount);

   int size_1 = m_nHandleSize /2;
   int size_2 = m_nHandleSize-size_1;
	CRect rect;
   
   rect.left = (int)m_lpPoints[uiPoint].x - size_1;
   rect.top = (int)m_lpPoints[uiPoint].y - size_1;
   rect.right = (int)m_lpPoints[uiPoint].x + size_2;
   rect.bottom = (int)m_lpPoints[uiPoint].y + size_2;

	*pHandleRect = rect;
}

UINT CRectangleTracker::HitTest(CPoint point) const
{
   UINT uiHit = HitTestPoints(point);
   if(uiHit!=hitNothing)
      return uiHit;

   //hittest on lines
	if(HitTestLines(point)!=hitNothing)
      return (UINT)hitLine;

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

UINT CRectangleTracker::HitTestLines(CPoint point) const
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


UINT CRectangleTracker::HitTestPoints(CPoint point) const
{
   for (UINT ui = 0; ui < m_uiCount; ++ui)
	{
      CRect rect;
		GetPointRect(ui, &rect);
      if(rect.PtInRect(point))
         return ui;
	}
   return (UINT)hitNothing;
}

void CRectangleTracker::DrawDots(CDC* pDC) const
{
	// set initial DC state
	VERIFY(pDC->SaveDC() != 0);
	pDC->SetMapMode(MM_TEXT);
	pDC->SetViewportOrg(0, 0);
	pDC->SetWindowOrg(0, 0);

	CPen* pOldPen = (CPen*)pDC->SelectStockObject(BLACK_PEN);
	CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(WHITE_BRUSH);
	// draw dots
	for (UINT ui = 0; ui < m_uiCount; ++ui)
	{
      CRect rect;
		GetPointRect(ui, &rect);
      pDC->Rectangle(rect);
	}

	// cleanup pDC state
	if (pOldPen != NULL)
		pDC->SelectObject(pOldPen);
	if (pOldBrush != NULL)
		pDC->SelectObject(pOldBrush);
	VERIFY(pDC->RestoreDC(-1));
}

BOOL CRectangleTracker::SetCursor(CWnd* pWnd, UINT nHitTest, BOOL bRotate) const
{
	// trackers should only be in client area
	if (nHitTest != HTCLIENT)
		return FALSE;

	// convert cursor position to client co-ordinates
	CPoint point;
	GetCursorPos(&point);
	pWnd->ScreenToClient(&point);

	// do hittest and normalize hit
	int iHit = HitTest(point);
	if (iHit == hitNothing)
		return FALSE;

   if(iHit == hitLine)
	   ::SetCursor(m_hPointMoveTrackerCursor);
   else if(iHit >= hitDotTopLeft)
	   ::SetCursor(m_hPointTrackerCursor);
   else if(iHit == hitInside)
	   ::SetCursor(m_hPointMoveTrackerCursor);
   else
      ASSERT(FALSE);

	return TRUE;
}


BOOL CRectangleTracker::Track(CWnd* pWnd, CPoint point, CSnapper* pSnapper,
	CWnd* pWndClipTo)
{

   int iHit = HitTest(point);
   BOOL bResizeOrMove = iHit>=hitDotTopLeft ? TRUE : FALSE; 

	// don't handle if capture already set
	if (::GetCapture() != NULL)
		return FALSE;


	m_rectOrigianl = m_rect;

   AfxLockTempMaps();  // protect maps while looping

	ASSERT(!m_bFinalErase);

	// set capture to the window which received this message
	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());
	pWnd->UpdateWindow();
	if (pWndClipTo != NULL)
		pWndClipTo->UpdateWindow();

   CPoint LastPoint(point);

	// get DC for drawing
	CDC* pDrawDC;
	if (pWndClipTo != NULL)
	{
		// clip to arbitrary window by using adjusted Window DC
		pDrawDC = pWndClipTo->GetDCEx(NULL, DCX_CACHE);
	}
	else
	{
		// otherwise, just use normal DC
		pDrawDC = pWnd->GetDC();
	}
	ASSERT_VALID(pDrawDC);

	CDRectangle rectOld;
	BOOL bMoved = FALSE;

	// get messages until capture lost or cancelled/accepted
	for (;;)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (CWnd::GetCapture() != pWnd)
			break;

      if(msg.message == WM_MOUSEMOVE)
			DispatchMessage(&msg);

		switch (msg.message)
		{
		// handle movement/accept messages
		case WM_LBUTTONUP:
		case WM_MOUSEMOVE:
OnCheange:
         {
   			rectOld = m_rect;
            m_rect = m_rectOrigianl;
			   // handle resize cases (and part of move)
            CPoint p;
            ::GetCursorPos(&p);
            pWnd->ScreenToClient(&p);

            p -= LastPoint;
            
			   // handle move case
			   if (!bResizeOrMove)
			   {
               if((::GetKeyState(VK_SHIFT) & 0x8000)!=0)
               {
                  if(ABS(p.x)>ABS(p.y))
                     p.y=0;
                  else
                     p.x=0;
               }
               LPDPOINT lpPoints = m_rect.GetPoints();
               for(int i=0; i<m_rect.GetNumOfPoints();i++)
               {
                  lpPoints[i].x += p.x;
                  lpPoints[i].y += p.y;
               }
               if(pSnapper!=NULL)
                  pSnapper->FixSnapTo((LPCDPOINT)lpPoints,m_rect.GetNumOfPoints());
			   }
            else
            {
               double dAngle1 = m_rect.GetAngle1();
               double dAngle2 = m_rect.GetAngle2();

               CDPoint dp(p);
               CDPoint center(0,0);
               RotatePoints(&dp, 1, -dAngle1, &center);

               if(CheckRetainProportions() && (m_dWidthDivHeight!=0)) //if the shift is pressed then remain proporions
               {
                  if(iHit == 0 || iHit == 2)
                     dp.y=dp.x/m_dWidthDivHeight;
                  else
                     dp.y=-dp.x/m_dWidthDivHeight;
               }

               LPDPOINT lpPoints = m_rect.GetPoints();

               CDPoint px(dp.x*cos(dAngle1),dp.x*sin(dAngle1));
               CDPoint py(dp.y*cos(dAngle2),dp.y*sin(dAngle2));

               int i = iHit;
               CDPoint ppp = lpPoints[i];
               CDPoint pppOrg(ppp);
               ppp.x += px.x+py.x;
               ppp.y += px.y+py.y;
               lpPoints[i] = ppp;

               if(pSnapper!=NULL)
               {
                  pSnapper->FixSnapTo((LPCDPOINT)&lpPoints[i],1);

                  if(ppp!=lpPoints[i])//if the point has changed then readjast the other points
                  {
                     dp = CDPoint(lpPoints[i].x - pppOrg.x, lpPoints[i].y - pppOrg.y);
                     RotatePoints(&dp, 1, -dAngle1, &center);
                     if(CheckRetainProportions() && (m_dWidthDivHeight!=0)) //if the shift is pressed then remain proporions
                     {
                        if(iHit == 0 || iHit == 2)
                           dp.y=dp.x/m_dWidthDivHeight;
                        else
                           dp.y=-dp.x/m_dWidthDivHeight;
                     }
                     px = CDPoint(dp.x*cos(dAngle1),dp.x*sin(dAngle1));
                     py = CDPoint(dp.y*cos(dAngle2),dp.y*sin(dAngle2));

                     i = iHit;
                     ppp = pppOrg;
                     ppp.x += px.x+py.x;
                     ppp.y += px.y+py.y;
                     lpPoints[i] = ppp;
                  }
               }

               i = aRectangleTrackerPoints[iHit].Xmove;
               ppp = lpPoints[i];
               ppp.x += px.x;
               ppp.y += px.y;
               lpPoints[i] = ppp;

               i = aRectangleTrackerPoints[iHit].Ymove;
               ppp = lpPoints[i];
               ppp.x += py.x;
               ppp.y += py.y;
               lpPoints[i] = ppp;

            }


			   // only redraw and callback if the rect actually changed!
			   m_bFinalErase = (msg.message == WM_LBUTTONUP);
			   if (!rectOld.IsEqual(m_rect) || m_bFinalErase)
			   {
				   if (bMoved)
				   {
					   m_bErase = TRUE;
					   DrawTracker(&rectOld, pWndClipTo, pDrawDC, pWnd);
				   }
				   if (msg.message != WM_LBUTTONUP)
					   bMoved = TRUE;
			   }
			   if (m_bFinalErase)
				   goto ExitLoop;

			   if (!rectOld.IsEqual(m_rect))
			   {
				   m_bErase = FALSE;
				   DrawTracker(&m_rect, pWndClipTo, pDrawDC, pWnd);
			   }
         }
			break;

      case WM_KEYUP:
			if (msg.wParam == VK_SHIFT)
            goto OnCheange;
			break;

		// handle cancel messages
		case WM_KEYDOWN:
			if (msg.wParam == VK_SHIFT)
            goto OnCheange;
			if (msg.wParam != VK_ESCAPE)
				break;
		case WM_RBUTTONDOWN:
			if (bMoved)
			{
				m_bErase = m_bFinalErase = TRUE;
				DrawTracker(&m_rect, pWndClipTo, pDrawDC, pWnd);
			}
			m_rect = m_rectOrigianl;
			goto ExitLoop;

		// just dispatch rest of the messages
		default:
			DispatchMessage(&msg);
			break;
		}
	}

ExitLoop:
	if (pWndClipTo != NULL)
		pWndClipTo->ReleaseDC(pDrawDC);
	else
		pWnd->ReleaseDC(pDrawDC);
	ReleaseCapture();

	AfxUnlockTempMaps(FALSE);

	// restore rect in case bMoved is still FALSE
	if (!bMoved)
		m_rect = m_rectOrigianl;
	m_bFinalErase = FALSE;
	m_bErase = FALSE;

	// return TRUE only if rect has changed
	return !m_rectOrigianl.IsEqual(m_rect);
}

void CRectangleTracker::DrawTracker(
	CDRectangle* lpRect, CWnd* pWndClipTo, CDC* pDC, CWnd* pWnd)
{
	// first, normalize the rectangle for drawing
	CRectangle rect = lpRect->GetRectangle();

   LPPOINT lpPointsNew = NULL;
   LPPOINT lpPointsLast = NULL;
	if (!m_bFinalErase)
	{
      lpPointsNew = rect.GetPoints();
	}

   if(!m_RectLast.IsNullRectangle())
      lpPointsLast = m_RectLast.GetPoints();

	// and draw it
	if (m_bFinalErase || !m_bErase)
   {
      CPolygonTracker::DrawDragPolygon(pDC, lpPointsNew, 1, lpPointsLast, 1, CRectangle::GetNumOfPoints());
   }

	// remember last rectangles
	m_RectLast = rect;
}
