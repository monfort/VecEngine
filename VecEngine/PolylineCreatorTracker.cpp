// PolylineCreatorTracker.cpp: implementation of the CPolylineCreatorTracker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PolylineCreatorTracker.h"
#include "LineTracker.h"
#include "Snapper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPolylineCreatorTracker::CPolylineCreatorTracker()
{
   m_PointsLast = NULL;
}

CPolylineCreatorTracker::~CPolylineCreatorTracker()
{
   while(!m_Points.IsEmpty())
   {
      CDPoint* pPoint = (CDPoint*)m_Points.RemoveHead();
      delete pPoint;
   }
   if(m_PointsLast!=NULL)
      delete []m_PointsLast;
}

void CPolylineCreatorTracker::GetPoints(LPCDPOINT lpPoints)
{
   UINT uiCount = m_Points.GetCount();
	ASSERT(AfxIsValidAddress(lpPoints, sizeof(CDPOINT)*uiCount, FALSE));

   POSITION pos = m_Points.GetHeadPosition();
   UINT ui = 0;
   while(pos != NULL)
   {
      ASSERT(ui < uiCount);
      CDPoint* pPoint = (CDPoint*)m_Points.GetNext(pos);
      memcpy(&lpPoints[ui++], pPoint, sizeof(CDPOINT));
   }
   ASSERT(ui == uiCount);
}

UINT CPolylineCreatorTracker::GetPointsCount()
{
   return m_Points.GetCount();
}

void CPolylineCreatorTracker::DrawTracker(POINT Point, CDC* pDC, CWnd* pWnd)
{
   POINT lpPoints[2];
   LPCDPOINT lpBasePoint = (LPCDPOINT)m_Points.GetTail();
   lpPoints[0] = *lpBasePoint;
   lpPoints[1] = Point;

   // draw it
	if (m_bFinalErase || !m_bErase)
      CLineTracker::DrawDragPolyLine(pDC, m_bFinalErase ? NULL : lpPoints, 1, 
                              m_PointsLast, 1, 2);

	// remember last points
   if(m_PointsLast==NULL)
      m_PointsLast = new POINT[2];
	memcpy(m_PointsLast, lpPoints, 2*sizeof(POINT));
}

BOOL CPolylineCreatorTracker::CreatePolyline(CWnd* pWnd, CDPoint point, CSnapper* pSnapper)
{
	// don't handle if capture already set
	if (::GetCapture() != NULL)
		return FALSE;

	AfxLockTempMaps();  // protect maps while looping

	m_bErase = FALSE;
	m_bFinalErase =  FALSE;

   	// set capture to the window which received this message
	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());
	pWnd->UpdateWindow();

   m_Points.AddTail(new CDPOINT(point));
   CPoint LastPoint(point);

	// get DC for drawing
	CDC* pDrawDC;
		// otherwise, just use normal DC
	pDrawDC = pWnd->GetDC();
	ASSERT_VALID(pDrawDC);

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
		case WM_MOUSEMOVE:
         {
			   // handle resize cases (and part of move)
            CPoint point;
            ::GetCursorPos(&point);
            pWnd->ScreenToClient(&point);

            if((::GetKeyState(VK_SHIFT) & 0x8000)!=0)
            {
               CDPoint *pLastPoint = (CDPoint*)m_Points.GetTail();
               if(pLastPoint!=NULL)
               {
                  CDPOINT ppp = CDPoint(point)-(*pLastPoint);
                  if(ABS(ppp.x)>ABS(ppp.y))
                     ppp.y=0;
                  else
                     ppp.x=0;
                  point = *pLastPoint+ppp;
               }
            }

            if(pSnapper!=NULL)
               pSnapper->FixSnapTo(&point);

			   if (LastPoint != point)
			   {
				   m_bErase = FALSE;
               DrawTracker(point, pDrawDC, pWnd);
               bMoved = TRUE;
			   }

            LastPoint = point;
         }
			break;

      case WM_LBUTTONDOWN:
         {
			   // handle resize cases (and part of move)
            CPoint point;
            ::GetCursorPos(&point);
            pWnd->ScreenToClient(&point);

            CDPoint dpoint(point);
            if((::GetKeyState(VK_SHIFT) & 0x8000)!=0)
            {
               CDPoint *pLastPoint = (CDPoint*)m_Points.GetTail();
               if(pLastPoint!=NULL)
               {
                  CDPOINT ppp = dpoint-(*pLastPoint);
                  if(ABS(ppp.x)>ABS(ppp.y))
                     ppp.y=0;
                  else
                     ppp.x=0;
                  dpoint = *pLastPoint+ppp;
               }
            }

            if(pSnapper!=NULL)
               pSnapper->FixSnapTo(&dpoint);
            point = dpoint;

			   if (LastPoint != dpoint)
			   {
				   m_bErase = FALSE;
               DrawTracker(point, pDrawDC, pWnd);
			   }

            m_Points.AddTail(new CDPoint(dpoint));

            //the tracker will not erease the last line
            if(m_PointsLast!=NULL)
               delete []m_PointsLast;
            bMoved = FALSE;

            m_PointsLast = NULL;

            LastPoint = point;
         }
         break;
		// handle cancel messages
		case WM_KEYDOWN:
			if (msg.wParam != VK_ESCAPE)
				break;
      case WM_LBUTTONDBLCLK:
      case WM_RBUTTONDOWN:
         {
			   if (bMoved)
			   {
				   m_bErase = m_bFinalErase = TRUE;
               DrawTracker(point, pDrawDC, pWnd);
			   }
            UINT uiCount = GetPointsCount();
            if(uiCount>1)//erace all the line
            {
               LPPOINT lpPoints = new POINT[uiCount];
               LPCDPOINT lpDPoints = new CDPOINT[uiCount];
               GetPoints(lpDPoints);
               for(UINT ui=0; ui<uiCount; ui++)
                  lpPoints[ui] = lpDPoints[ui];
               CLineTracker::DrawDragPolyLine(pDrawDC, lpPoints, 1, NULL, 1, uiCount);
               delete []lpPoints;
               delete []lpDPoints;
            }
         }
			goto ExitLoop;

		// just dispatch rest of the messages
		default:
			DispatchMessage(&msg);
			break;
		}
	}

ExitLoop:
	pWnd->ReleaseDC(pDrawDC);
	ReleaseCapture();

	AfxUnlockTempMaps(FALSE);

	// restore rect in case bMoved is still FALSE
	m_bFinalErase = FALSE;
	m_bErase = FALSE;

   // return TRUE only if rect has changed
	return GetPointsCount()>1;
}

BOOL CPolylineCreatorTracker::CreateFreehand(CWnd* pWnd, CDPoint point, CSnapper* pSnapper)
{
	// don't handle if capture already set
	if (::GetCapture() != NULL)
		return FALSE;

	AfxLockTempMaps();  // protect maps while looping

	m_bErase = FALSE;
	m_bFinalErase =  FALSE;

   	// set capture to the window which received this message
	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());
	pWnd->UpdateWindow();

   m_Points.AddTail(new CDPOINT(point));
   CPoint LastPoint(point);

	// get DC for drawing
	CDC* pDrawDC;
		// otherwise, just use normal DC
	pDrawDC = pWnd->GetDC();
	ASSERT_VALID(pDrawDC);

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
			   // handle resize cases (and part of move)
            CPoint point;
            ::GetCursorPos(&point);
            pWnd->ScreenToClient(&point);

            CDPoint dpoint(point);
            if(pSnapper!=NULL)
               pSnapper->FixSnapTo(&dpoint);

            if (LastPoint != point)
			   {
               point = dpoint;
				   m_bErase = FALSE;
               POINT lpPoints[2];
               lpPoints[0] = LastPoint;
               lpPoints[1] = point;
               CLineTracker::DrawDragPolyLine(pDrawDC, lpPoints, 1, NULL, 1, 2);

               m_Points.AddTail(new CDPoint(dpoint));


               LastPoint = point;
            }

            if(msg.message == WM_LBUTTONUP)
               goto EndLoop;
         }
         break;
		// handle cancel messages
		case WM_KEYDOWN:
			if (msg.wParam != VK_ESCAPE)
				break;
      case WM_RBUTTONDOWN:
EndLoop:
         {
			   if (bMoved)
			   {
				   m_bErase = m_bFinalErase = TRUE;
               DrawTracker(point, pDrawDC, pWnd);
			   }
            UINT uiCount = GetPointsCount();
            if(uiCount>1)//erace all the line
            {
               LPPOINT lpPoints = new POINT[uiCount];
               LPCDPOINT lpDPoints = new CDPOINT[uiCount];
               GetPoints(lpDPoints);
               for(UINT ui=0; ui<uiCount; ui++)
                  lpPoints[ui] = lpDPoints[ui];
               CLineTracker::DrawDragPolyLine(pDrawDC, lpPoints, 1, NULL, 1, uiCount);
               delete []lpPoints;
               delete []lpDPoints;
            }
         }
			goto ExitLoop;

		// just dispatch rest of the messages
		default:
			DispatchMessage(&msg);
			break;
		}
	}

ExitLoop:
	pWnd->ReleaseDC(pDrawDC);
	ReleaseCapture();

	AfxUnlockTempMaps(FALSE);

	// restore rect in case bMoved is still FALSE
	m_bFinalErase = FALSE;
	m_bErase = FALSE;

   // return TRUE only if rect has changed
	return GetPointsCount()>1;
}
