/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// PicTracker.cpp: implementation of the CPicTracker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VecTypes.h"
#include "PicTracker.h"
#include "PolygonTracker.h"
#include "Snapper.h"
#include "resource.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define PIC_TRACK_HALF_WIDTH 8
#define PIC_ANCHOR_HALF_WIDTH 2

HCURSOR CPicTracker::m_hPointSizeTrackerCursor = NULL;
HCURSOR CPicTracker::m_hPointMoveTrackerCursor = NULL;
HCURSOR CPicTracker::m_hPointRotateTrackerCursor = NULL;

CPicTracker::CPicTracker(CRect ObRect, CDRectangle PicRect)
{
   m_ObRect = ObRect;
   m_rect = PicRect;

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

   if(m_hPointSizeTrackerCursor == NULL)
      m_hPointSizeTrackerCursor = AfxGetApp()->LoadCursor(IDC_PIC_RESIZE);
   if(m_hPointMoveTrackerCursor == NULL)
      m_hPointMoveTrackerCursor = AfxGetApp()->LoadCursor(IDC_PIC_MOVE);
   if(m_hPointRotateTrackerCursor == NULL)
      m_hPointRotateTrackerCursor = AfxGetApp()->LoadCursor(IDC_PIC_ROTATE);
   
}

CPicTracker::~CPicTracker()
{
}

CRect CPicTracker::GetPointRect() const
{
   CRect rect = m_ObRect;
   rect = CRect(CPoint(rect.CenterPoint()-CPoint(PIC_TRACK_HALF_WIDTH,PIC_TRACK_HALF_WIDTH)), 
                CPoint(rect.CenterPoint()+CPoint(PIC_TRACK_HALF_WIDTH,PIC_TRACK_HALF_WIDTH)));
   return rect;
}
CRect CPicTracker::GetAnchorRect(int x,int y) const
{
   return CRect(x-PIC_ANCHOR_HALF_WIDTH,y-PIC_ANCHOR_HALF_WIDTH,
                x+PIC_ANCHOR_HALF_WIDTH,y+PIC_ANCHOR_HALF_WIDTH);
}

void CPicTracker::DrawDot(CDC* pDC) const
{
	// set initial DC state
	VERIFY(pDC->SaveDC() != 0);
	pDC->SetMapMode(MM_TEXT);
	pDC->SetViewportOrg(0, 0);
	pDC->SetWindowOrg(0, 0);

   CRect rect = GetPointRect();

   CPen* pOldPen = (CPen*)pDC->SelectStockObject(BLACK_PEN);
	CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(WHITE_BRUSH);

   pDC->Rectangle(rect);

   CRect AnchoreRect;
   AnchoreRect = GetAnchorRect(rect.right,rect.top);
   pDC->Rectangle(AnchoreRect);
   AnchoreRect = GetAnchorRect(rect.left,rect.top);
   pDC->Rectangle(AnchoreRect);
   AnchoreRect = GetAnchorRect(rect.right,rect.bottom);
   pDC->Rectangle(AnchoreRect);
   AnchoreRect = GetAnchorRect(rect.left,rect.bottom);
   pDC->Rectangle(AnchoreRect);

   // cleanup pDC state
	if (pOldPen != NULL)
		pDC->SelectObject(pOldPen);
	if (pOldBrush != NULL)
		pDC->SelectObject(pOldBrush);
	VERIFY(pDC->RestoreDC(-1));
}

BOOL CPicTracker::SetCursor(CWnd* pWnd, UINT nHitTest, BOOL bRotate) const
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

   if(iHit == hitOnPoint)
   {
      if(bRotate)
	      ::SetCursor(m_hPointRotateTrackerCursor);
      else
	      ::SetCursor(m_hPointSizeTrackerCursor);
   }
   else if(iHit == hitInside)
   {
	   ::SetCursor(m_hPointMoveTrackerCursor);
   }
   else
      ASSERT(FALSE);

	return TRUE;
}

UINT CPicTracker::HitTest(CPoint point) const
{
   CRect rect = GetPointRect();

   CRect AnchoreRect;
   AnchoreRect = GetAnchorRect(rect.right,rect.top);
   if(AnchoreRect.PtInRect(point))
      return (UINT)hitOnPoint;
   AnchoreRect = GetAnchorRect(rect.left,rect.top);
   if(AnchoreRect.PtInRect(point))
      return (UINT)hitOnPoint;
   AnchoreRect = GetAnchorRect(rect.right,rect.bottom);
   if(AnchoreRect.PtInRect(point))
      return (UINT)hitOnPoint;
   AnchoreRect = GetAnchorRect(rect.left,rect.bottom);
   if(AnchoreRect.PtInRect(point))
      return (UINT)hitOnPoint;

   if(rect.PtInRect(point))
      return (UINT)hitInside;

   return (UINT)hitNothing;
}

BOOL CPicTracker::Track(CWnd* pWnd, CPoint point, BOOL bRotate, CSnapper* pSnapper,
	CWnd* pWndClipTo)
{

   BOOL bResizeOrMove = HitTest(point)==hitOnPoint ? TRUE : FALSE; 

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
            //LastPoint = point;
            
			   // handle move case
            if(bRotate)
            {
               LPDPOINT lpPoints = m_rect.GetPoints();
               CPoint pp = p+LastPoint;
               double dAngle = GetPointAng(&pp, &LastPoint);
               double dist = sqrt((double)(p.x*p.x + p.y*p.y));
               if(dist<30)
               {
                  dAngle /= 30;
                  dAngle *= dist;
               }
               if(::GetKeyState(VK_SHIFT) & 0x8000) //if the shift is pressed then snap to the close 45 degrees
               {
                  dAngle = ((int)((dAngle/(RAD90D/2))+0.499))*(RAD90D/2);
               }

               RotatePoints((LPCDPOINT)lpPoints, m_rect.GetNumOfPoints(), dAngle, NULL);
            }
			   else if (!bResizeOrMove)
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
               double dAngle2 = m_rect.GetAngle2()-RAD90D;

               CDPoint center(0,0);
               CDPoint dp(p);
               RotatePoints(&dp, 1, -dAngle1, &center);

               if(CheckRetainProportions() && (m_dWidthDivHeight!=0)) //if the shift is pressed then remain proporions
               {
                  dp.y=dp.x/m_dWidthDivHeight;
               }

               LPDPOINT lpPoints = m_rect.GetPoints();

               center = GetPointsCenter((LPCDPOINT)lpPoints, m_rect.GetNumOfPoints());
               RotatePoints((LPCDPOINT)lpPoints, m_rect.GetNumOfPoints(), -dAngle1, &center);
			   int i;
               for(i=0; i<m_rect.GetNumOfPoints(); i++)
               {
                  CDPoint ppp = lpPoints[i];
                  ppp-=center;
                  if(ppp.x>0)
                     ppp.x += dp.x;
                  else
                     ppp.x -= dp.x;

                  ppp+=center;
                  lpPoints[i] = ppp;
               }
               RotatePoints((LPCDPOINT)lpPoints, m_rect.GetNumOfPoints(), dAngle1-dAngle2, &center);
               for(i=0; i<m_rect.GetNumOfPoints(); i++)
               {
                  CDPoint ppp = lpPoints[i];
                  ppp-=center;

                  if(ppp.y>0)
                     ppp.y += dp.y;
                  else
                     ppp.y -= dp.y;
                  ppp+=center;
                  lpPoints[i] = ppp;
               }
               RotatePoints((LPCDPOINT)lpPoints, m_rect.GetNumOfPoints(), dAngle2, &center);

               if(pSnapper!=NULL)
                  pSnapper->FixSnapTo((LPCDPOINT)lpPoints,m_rect.GetNumOfPoints());
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

void CPicTracker::DrawTracker(
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

