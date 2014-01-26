// SuperRotateTracker.cpp: implementation of the CSuperRotateTracker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SuperRotateTracker.h"
#include "VecBase.h"
#include "G_Routines.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSuperRotateTracker::CSuperRotateTracker(CPtrList &VecList):
m_VecList(VecList)
{
   //calc the bounding rectangle of all vec odjects
   UINT uiCount = 0;
   m_rect.SetRectEmpty();
   POSITION pos = m_VecList.GetHeadPosition();
   while(pos != NULL)
   {
      CVecBase* pVec = (CVecBase*)m_VecList.GetNext(pos);
      uiCount++;

      CRect vecRect = pVec->GetRectByScreen();
      vecRect.NormalizeRect();

      if(m_rect.IsRectEmpty())
         m_rect = vecRect;
      else
      {
         if(!vecRect.IsRectEmpty())
            m_rect.UnionRect(m_rect, vecRect);
      }
   }
   ASSERT(uiCount!=0);

   m_dAngLast = 0;
   m_bErase = FALSE;
	m_bFinalErase =  FALSE;
   m_pCenterLast = NULL;
}

CSuperRotateTracker::~CSuperRotateTracker()
{
   if(m_pCenterLast!=NULL)
      delete m_pCenterLast;

}

BOOL CSuperRotateTracker::Track(CWnd* pWnd, CPoint point, CWnd* pWndClipTo)
{
	// don't handle if capture already set
	if (::GetCapture() != NULL)
		return FALSE;

   m_dAng = 0;
   m_center = m_rect.CenterPoint();

   AfxLockTempMaps();  // protect maps while looping

	ASSERT(!m_bFinalErase);

	// set capture to the window which received this message
	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());
	pWnd->UpdateWindow();
	if (pWndClipTo != NULL)
		pWndClipTo->UpdateWindow();

   CPoint LastPoint(point);
   double dAngOffset = GetPointAng(&LastPoint, &m_center);

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

	BOOL bMoved = FALSE;
   double dAngOld;

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
            dAngOld = m_dAng;
			   // handle resize cases (and part of move)
            CPoint p;
            ::GetCursorPos(&p);
            pWnd->ScreenToClient(&p);

            m_dAng = GetPointAng(&p, &m_center) - dAngOffset;

            if(::GetKeyState(VK_SHIFT) & 0x8000) //if the shift is pressed then snap to the close 45 degrees
            {
               m_dAng = ((int)((m_dAng/(RAD90D/2))+0.499))*(RAD90D/2);
            }

			   // only redraw and callback if the rect actually changed!
			   m_bFinalErase = (msg.message == WM_LBUTTONUP);
			   if (m_dAng!=dAngOld || m_bFinalErase)
			   {
				   if (bMoved)
				   {
					   m_bErase = TRUE;
					   DrawTrackers(dAngOld, pWndClipTo, pDrawDC, pWnd);
				   }
				   if (msg.message != WM_LBUTTONUP)
					   bMoved = TRUE;
			   }
			   if (m_bFinalErase)
				   goto ExitLoop;

			   if (m_dAng!=dAngOld)
			   {
				   m_bErase = FALSE;
				   DrawTrackers(m_dAng, pWndClipTo, pDrawDC, pWnd);
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
				DrawTrackers(m_dAng, pWndClipTo, pDrawDC, pWnd);
			}
			m_dAng = 0;
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
		m_dAng = 0;
	m_bFinalErase = FALSE;
	m_bErase = FALSE;

	// return TRUE only if rect has changed
	return m_dAng!=0;
}

void CSuperRotateTracker::DrawTrackers(double dAng, CWnd* pWndClipTo, CDC* pDC, CWnd* pWnd)
{
   LPPOINT lpCenterNew = NULL;
	if (!m_bFinalErase)
	{
      lpCenterNew = &m_center;
	}

	// and draw it
	if (m_bFinalErase || !m_bErase)
   {
      POSITION pos = m_VecList.GetHeadPosition();
      while(pos != NULL)
      {
         CVecBase* pVec = (CVecBase*)m_VecList.GetNext(pos);
   		pVec->DrawRotatedDragImage(pDC, dAng, lpCenterNew, m_dAngLast, m_pCenterLast);
      }

   }

	// remember last angle
   if(m_pCenterLast==NULL)
      m_pCenterLast = new CPoint;
   *m_pCenterLast = m_center;
	m_dAngLast = dAng;
}

