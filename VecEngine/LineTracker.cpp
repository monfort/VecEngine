// LineTracker.cpp: implementation of the CLineTracker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LineTracker.h"
#include "G_Routines.h"
#include "Snapper.h"
#include "resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define POINT_OVER_LINE_SENSETIVITY 3

HCURSOR CLineTracker::m_hPointTrackerCursor;
HCURSOR CLineTracker::m_hPointMoveTrackerCursor;

// various GDI objects we need to draw

// the struct below gives us information on the layout of a RECT struct and
//  the relationship between its members
struct AFX_RECTINFO
{
	size_t nOffsetAcross;   // offset of opposite point (ie. left->right)
	int nSignAcross;        // sign relative to that point (ie. add/subtract)
};

// this array is indexed by the offset of the RECT member / sizeof(int)
#define NUM_OF_EDGES 4
static const AFX_RECTINFO aRectTrackerEdges[NUM_OF_EDGES] =
{
	{ offsetof(RECT, right), +1 },
	{ offsetof(RECT, bottom), +1 },
	{ offsetof(RECT, left), -1 },
	{ offsetof(RECT, top), -1 },
};

/////////////////////////////////////////////////////////////////////////////
// CLineTracker intitialization

CLineTracker::CLineTracker(LPCDPOINT lpPoints, UINT uiCount, int enStyle)
{
	ASSERT(AfxIsValidAddress(lpPoints, sizeof(POINT)*uiCount, FALSE));

	Construct();

   m_enStyle = enStyle;
   m_lpPoints = lpPoints;
   m_uiCount = uiCount;
   m_pSnapper = NULL;

   m_bShowPoints = (((int)m_enStyle & styleShowPoints) == (int)styleShowPoints);
   m_bIgnoreX = (((int)m_enStyle & styleIgnoreX) == ((int)styleIgnoreX));
   m_bIgnoreY = (((int)m_enStyle & styleIgnoreY) == (int)styleIgnoreY);

}

void CLineTracker::Construct()
{
	// do one-time initialization if necessary
	//AfxLockGlobals(CRIT_RECTTRACKER);
	static BOOL bInitialized = FALSE;
	if (!bInitialized)
	{
		// sanity checks for assumptions we make in the code
		ASSERT(sizeof(((RECT*)NULL)->left) == sizeof(int));
		ASSERT(offsetof(RECT, top) > offsetof(RECT, left));
		ASSERT(offsetof(RECT, right) > offsetof(RECT, top));
		ASSERT(offsetof(RECT, bottom) > offsetof(RECT, right));


		// initialize the cursors
      m_hPointTrackerCursor = AfxGetApp()->LoadCursor(IDC_MOVE4WAY);
      m_hPointMoveTrackerCursor = AfxGetApp()->LoadCursor(IDC_TRACK4WAY);

		bInitialized = TRUE;
	}
	//AfxUnlockGlobals(CRIT_RECTTRACKER);

	m_nHandleSize = 4;

	m_iWidthLast = m_iWidth = 1;
   m_PointsLast = NULL;
	m_bErase = FALSE;
	m_bFinalErase =  FALSE;
}

CLineTracker::~CLineTracker()
{
   if(m_PointsLast!=NULL)
      delete []m_PointsLast;
}

/////////////////////////////////////////////////////////////////////////////
// CLineTracker operations

void CLineTracker::DrawDots(CDC* pDC) const
{
   if(!m_bShowPoints)
      return;

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

BOOL CLineTracker::SetCursor(CWnd* pWnd, UINT nHitTest) const
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
   else if(iHit >= hitDot)
	   ::SetCursor(m_hPointTrackerCursor);
   else if(iHit == hitInside)
	   ::SetCursor(m_hPointMoveTrackerCursor);
   else
      ASSERT(FALSE);

	return TRUE;
}

UINT CLineTracker::HitTest(CPoint point) const
{
   if(m_bShowPoints)
   {
      UINT uiHit = HitTestPoints(point);
      if(uiHit!=hitNothing)
         return uiHit;
   }

   //hittest on lines
	if(HitTestLines(point)!=hitNothing)
      return (UINT)hitLine;

   return (UINT)hitNothing;
}

UINT CLineTracker::HitTestLines(CPoint point) const
{
	for (UINT ui = 1; ui < m_uiCount; ++ui)
	{
      if(PtOnLine(point, m_lpPoints[ui-1], m_lpPoints[ui], POINT_OVER_LINE_SENSETIVITY))
         return ui-1;
	}
   return (UINT)hitNothing;
}


UINT CLineTracker::HitTestPoints(CPoint point) const
{
   if(!m_bShowPoints)
      return (UINT)hitNothing;

   for (UINT ui = 0; ui < m_uiCount; ++ui)
	{
      CRect rect;
		GetPointRect(ui, &rect);
      if(rect.PtInRect(point))
         return ui;
	}
   return (UINT)hitNothing;
}


BOOL CLineTracker::Track(CWnd* pWnd, CPoint point, CSnapper* pSnapper, CWnd* pWndClipTo)
{
   m_pSnapper = pSnapper;
	// perform hit testing on the handles
	UINT uiHandle = HitTest(point);
	if (uiHandle == hitNothing)
	{
		// didn't hit a handle, so just return FALSE
		return FALSE;
	}

	// otherwise, call helper function to do the tracking
	return TrackHandle(uiHandle , pWnd, point, pWndClipTo);
}

void CLineTracker::DrawTracker(
	LPCDPOINT lpPoints, CWnd* pWndClipTo, CDC* pDC, CWnd* pWnd)
{

   LPPOINT lpIntPoints = new POINT[m_uiCount];
   for(UINT ui=0; ui<m_uiCount; ui++)
      lpIntPoints[ui] = lpPoints[ui];

   // draw it
	if (m_bFinalErase || !m_bErase)
      DrawDragPolyLine(pDC, m_bFinalErase ? NULL : lpIntPoints, m_iWidth, 
                              m_PointsLast, m_iWidthLast, m_uiCount);

	// remember last points
   if(m_PointsLast==NULL)
      m_PointsLast = new POINT[m_uiCount];
	memcpy(m_PointsLast, lpIntPoints, m_uiCount*sizeof(POINT));
	m_iWidthLast = m_iWidth;

   delete []lpIntPoints;
}


void CLineTracker::OnChangedPoints(const LPCDPOINT LPPointsOld)
{
	// no default implementation, useful for derived classes
}

/////////////////////////////////////////////////////////////////////////////
// CLineTracker implementation helpers

void CLineTracker::GetPointRect(UINT uiPoint, CRect* pHandleRect) const
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


BOOL CLineTracker::TrackHandle(UINT uiHandle , CWnd* pWnd, CPoint point,
	CWnd* pWndClipTo)
{
	// don't handle if capture already set
	if (::GetCapture() != NULL)
		return FALSE;

	AfxLockTempMaps();  // protect maps while looping

	ASSERT(!m_bFinalErase);

	// set capture to the window which received this message
	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());
	pWnd->UpdateWindow();
	if (pWndClipTo != NULL)
		pWndClipTo->UpdateWindow();
	LPCDPOINT pointsSave = new CDPOINT[m_uiCount];
	memcpy(pointsSave , m_lpPoints, m_uiCount*sizeof(CDPOINT));

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

	LPCDPOINT pointsOld = new CDPOINT[m_uiCount];
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
         {
	         memcpy(pointsOld , m_lpPoints, m_uiCount*sizeof(CDPOINT));
	         memcpy(m_lpPoints , pointsSave, m_uiCount*sizeof(CDPOINT));

            POINT p;
            ::GetCursorPos(&p);
            pWnd->ScreenToClient(&p);

            int xxx = p.x - LastPoint.x;
            int yyy = p.y - LastPoint.y;

            if(m_bIgnoreX)
               xxx = 0;
            if(m_bIgnoreY)
               yyy = 0;
            
            if((::GetKeyState(VK_SHIFT) & 0x8000)!=0)
            {
               if(ABS(xxx)>ABS(yyy))
                  yyy=0;
               else
                  xxx=0;
            }
			   // handle move case
			   if (uiHandle < 0 || uiHandle >= m_uiCount)
			   {
               for(UINT ui = 0; ui<m_uiCount; ui++)
               {
                  m_lpPoints[ui].x += xxx;
                  m_lpPoints[ui].y += yyy;
               }
               if(m_pSnapper!=NULL)
                  m_pSnapper->FixSnapTo(m_lpPoints, m_uiCount);
			   }
            else
            {
               m_lpPoints[uiHandle].x += xxx;
               m_lpPoints[uiHandle].y += yyy;
               if(m_pSnapper!=NULL)
                  m_pSnapper->FixSnapTo(&m_lpPoints[uiHandle]);
            }

			   // allow caller to adjust the rectangle if necessary
			   //AdjustRect(nHandle, &m_rect);

			   // only redraw and callback if the rect actually changed!
			   m_bFinalErase = (msg.message == WM_LBUTTONUP);
			   if (memcmp(pointsOld , m_lpPoints, m_uiCount*sizeof(CDPOINT))!=0 || m_bFinalErase)
			   {
				   if (bMoved)
				   {
					   m_bErase = TRUE;
					   DrawTracker(pointsOld, pWndClipTo, pDrawDC, pWnd);
				   }
				   OnChangedPoints(pointsOld);
				   if (msg.message != WM_LBUTTONUP)
					   bMoved = TRUE;
			   }
			   if (m_bFinalErase)
				   goto ExitLoop;

			   if (memcmp(pointsOld , m_lpPoints, m_uiCount*sizeof(CDPOINT))!=0)
			   {
				   m_bErase = FALSE;
				   DrawTracker(m_lpPoints, pWndClipTo, pDrawDC, pWnd);
			   }
         }
			break;

		// handle cancel messages
		case WM_KEYDOWN:
			if (msg.wParam != VK_ESCAPE)
				break;
		case WM_RBUTTONDOWN:
			if (bMoved)
			{
				m_bErase = m_bFinalErase = TRUE;
				DrawTracker(m_lpPoints, pWndClipTo, pDrawDC, pWnd);
			}
      	memcpy(m_lpPoints, pointsSave, m_uiCount*sizeof(CDPOINT));
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
      memcpy(m_lpPoints, pointsSave, m_uiCount*sizeof(CDPOINT));
	m_bFinalErase = FALSE;
	m_bErase = FALSE;


   BOOL bCheanged = (memcmp(pointsSave , m_lpPoints, m_uiCount*sizeof(CDPOINT))!=0);

   if(pointsSave!=NULL)
      delete []pointsSave;
   if(pointsOld!=NULL)
      delete []pointsOld;

   // return TRUE only if rect has changed
	return bCheanged;
}


void CLineTracker::DrawDragPolyLine(CDC* pDC, LPPOINT lpPoints, int iWidth,
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
      Polyline(pDC->m_hDC, lpPointsLast, uiCount);
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
      Polyline(pDC->m_hDC, lpPoints, uiCount);
      SetROP2(pDC->m_hDC, iRop2Old);
		SelectObject(pDC->m_hDC, PenOld);
      DeleteObject(Pen);
   }
}
