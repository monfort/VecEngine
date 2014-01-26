// PicTracker.h: interface for the CPicTracker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PICTRACKER_H__D92C9486_C3FE_11D3_A49E_00C0CA1261A6__INCLUDED_)
#define AFX_PICTRACKER_H__D92C9486_C3FE_11D3_A49E_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSnapper;
#include "BaseTracker.h"

class CPicTracker:  public CBaseTracker
{
public:
	CPicTracker(CRect ObRect, CDRectangle PicRect);
	virtual ~CPicTracker();

   enum TrackerHit
	{
		hitNothing = UINT(-3),
		hitInside = UINT(-2),
		hitOnPoint = UINT(-1),
	};

   void DrawDot(CDC* pDC) const;
	virtual BOOL SetCursor(CWnd* pWnd, UINT nHitTest, BOOL bRotate) const;
	BOOL Track(CWnd* pWnd, CPoint point, BOOL bRotate, CSnapper* pSnapper = NULL, CWnd* pWndClipTo = NULL);
	virtual UINT HitTest(CPoint point) const;

   CDRect m_ObRect;
   CDRectangle m_rect;

   double m_dWidthDivHeight;
   BOOL m_bErase;          // TRUE if DrawTrackerRect is called for erasing
	BOOL m_bFinalErase;     // TRUE if DragTrackerRect called for final erase
	CRectangle m_RectLast;
   int m_iTrackingPointY;
   int m_iTrackingPointX;

protected:
   CRect GetPointRect()const;
   CRect GetAnchorRect(int x,int y)const;
   CDRectangle m_rectOrigianl;
   void DrawTracker(CDRectangle* lpRect, CWnd* pWndClipTo, CDC* pDC, CWnd* pWnd);

   static HCURSOR m_hPointSizeTrackerCursor;
   static HCURSOR m_hPointMoveTrackerCursor;
   static HCURSOR m_hPointRotateTrackerCursor;
};

#endif // !defined(AFX_PICTRACKER_H__D92C9486_C3FE_11D3_A49E_00C0CA1261A6__INCLUDED_)
