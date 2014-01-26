// SuperTracker.cpp: implementation of the CSuperTracker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SuperTracker.h"
#include "VecBase.h"
#include "Snapper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSuperTracker::CSuperTracker(CPtrList &VecList):
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

      CDRect vecRect = pVec->GetRectByScreen();
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

   if(m_rect.Height()!=0)
      m_dWidthDivHeight = m_rect.Width() / ((double)m_rect.Height());
   else
      m_dWidthDivHeight = 1;

   m_iTrackingPointY = m_iTrackingPointX = 0;
	m_PrectLast = NULL;
	m_bErase = FALSE;
	m_bFinalErase =  FALSE;
}

CSuperTracker::~CSuperTracker()
{
   if(m_PrectLast!=NULL)
      delete m_PrectLast;
}

BOOL CSuperTracker::Track(CWnd* pWnd, CPoint point, StyleFlags iStyle, CSnapper* pSnapper,
	CWnd* pWndClipTo)
{
	// don't handle if capture already set
	if (::GetCapture() != NULL)
		return FALSE;

   if(iStyle == Style_Size)
   {
      //find out what is the farest corner of the rect from the point
      CDPoint TempPoint(point.x-m_rect.left, point.y-m_rect.top);
      if(m_rect.Width() / 2 > TempPoint.x)
         m_iTrackingPointX = -1;
      else
         m_iTrackingPointX = 1;

      if(m_rect.Height() / 2 > TempPoint.y)
         m_iTrackingPointY = -1;
      else
         m_iTrackingPointY = 1;

/*      //trunc the rect to be between the farest corner in it and this point
      if(m_iTrackingPointY>0)
         m_rect.bottom = point.y;
      else
         m_rect.top = point.y;
      if(m_iTrackingPointX>0)
         m_rect.right = point.x;
      else
         m_rect.left = point.x;*/
   }

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

	CDRect rectOld;
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
            POINT p;
            ::GetCursorPos(&p);
            pWnd->ScreenToClient(&p);

            int xxx = p.x - LastPoint.x;
            int yyy = p.y - LastPoint.y;
            //LastPoint = point;
            
			   // handle move case
			   if (iStyle == Style_Move)
			   {
               if((::GetKeyState(VK_SHIFT) & 0x8000)!=0)
               {
                  if(ABS(xxx)>ABS(yyy))
                     yyy=0;
                  else
                     xxx=0;
               }
               m_rect.bottom += yyy;
               m_rect.top += yyy;
               m_rect.left += xxx;
               m_rect.right += xxx;

               if(pSnapper!=NULL)
                  pSnapper->FixSnapTo(&m_rect);
			   }
            else
            {
               CDPoint thispoint;
               if(m_iTrackingPointY>0)
                  thispoint.y = m_rect.bottom + yyy;
               else
                  thispoint.y = m_rect.top + yyy;
               if(m_iTrackingPointX>0)
                  thispoint.x = m_rect.right + xxx;
               else
                  thispoint.x = m_rect.left + xxx;

               if(pSnapper!=NULL)
                  pSnapper->FixSnapTo(&thispoint);

               if(m_iTrackingPointY>0)
                  m_rect.bottom = thispoint.y;
               else
                  m_rect.top = thispoint.y;
               if(m_iTrackingPointX>0)
                  m_rect.right = thispoint.x;
               else
                  m_rect.left = thispoint.x;
            }

            if(CheckRetainProportions() && m_dWidthDivHeight!=0) //if the shift is pressed then remain proporions
            {
               double dHeight = m_rect.Height();
               double dSign = 1;
               if(dHeight!=0)
                  dSign = dHeight / ABS(dHeight);
               dHeight = ABS(m_rect.Width()/m_dWidthDivHeight)*dSign;
               if(m_iTrackingPointY>0)
                  m_rect.bottom = (m_rect.top + dHeight);
               else
                  m_rect.top = (m_rect.bottom - dHeight);
            }

			   // only redraw and callback if the rect actually changed!
			   m_bFinalErase = (msg.message == WM_LBUTTONUP);
			   if (!rectOld.EqualRect(&m_rect) || m_bFinalErase)
			   {
				   if (bMoved)
				   {
					   m_bErase = TRUE;
					   DrawTrackers(&rectOld, pWndClipTo, pDrawDC, pWnd);
				   }
				   if (msg.message != WM_LBUTTONUP)
					   bMoved = TRUE;
			   }
			   if (m_bFinalErase)
				   goto ExitLoop;

			   if (!rectOld.EqualRect(&m_rect))
			   {
				   m_bErase = FALSE;
				   DrawTrackers(&m_rect, pWndClipTo, pDrawDC, pWnd);
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
				DrawTrackers(&m_rect, pWndClipTo, pDrawDC, pWnd);
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
	return !m_rectOrigianl.EqualRect(&m_rect);
}

void CSuperTracker::DrawTrackers(
	LPDRECT lpRect, CWnd* pWndClipTo, CDC* pDC, CWnd* pWnd)
{
	// first, normalize the rectangle for drawing
	CDRect rect = *lpRect;
	//rect.NormalizeRect();

   LPDRECT lpRectNew = NULL;
	if (!m_bFinalErase)
	{
      lpRectNew = &rect;
	}

	// and draw it
	if (m_bFinalErase || !m_bErase)
   {
      POSITION pos = m_VecList.GetHeadPosition();
      while(pos != NULL)
      {
         CVecBase* pVec = (CVecBase*)m_VecList.GetNext(pos);
   		pVec->DrawDragImage(pDC, lpRectNew, m_PrectLast, &m_rectOrigianl);
      }

   }

	// remember last rectangles
   if(m_PrectLast==NULL)
      m_PrectLast = new CDRect;
	*m_PrectLast = rect;
}

