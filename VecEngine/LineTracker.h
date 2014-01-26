// LineTracker.h: interface for the CLineTracker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINETRACKER_H__3D37A967_B837_11D3_A497_00C0CA1261A6__INCLUDED_)
#define AFX_LINETRACKER_H__3D37A967_B837_11D3_A497_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "VecTypes.h"
#include "BaseTracker.h"

class CSnapper;

class CLineTracker:  public CBaseTracker
{
public:
	CLineTracker();
	CLineTracker(LPCDPOINT lpPoints, UINT uiCount, int enStyle = styleShowPoints);

	enum StyleFlags
	{
		styleShowPoints = 1,
		styleIgnoreX = 2,
		styleIgnoreY = 4
	};

// Hit-Test codes
	enum TrackerHit
	{
		hitNothing = UINT(-3),
		hitInside = UINT(-2),
		hitLine = UINT(-1),
      hitDot = 0
	};

	
   LPCDPOINT m_lpPoints;
   UINT m_uiCount;
   int m_iWidth;

	int m_nHandleSize;  // size of resize handles (default from WIN.INI)

// Operations
	void DrawDots(CDC* pDC) const;
	virtual BOOL SetCursor(CWnd* pWnd, UINT nHitTest) const;
	BOOL Track(CWnd* pWnd, CPoint point, CSnapper* pSnapper = NULL, CWnd* pWndClipTo = NULL);
	//BOOL TrackRubberBand(CWnd* pWnd, CPoint point);
	virtual UINT HitTest(CPoint point) const;
	virtual UINT HitTestPoints(CPoint point) const;
	virtual UINT HitTestLines(CPoint point) const;

// Overridables
	virtual void DrawTracker(LPCDPOINT lpPoints, CWnd* pWndClipTo,
		CDC* pDC, CWnd* pWnd);
	//virtual void AdjustRect(int nHandle, LPRECT lpRect);
	virtual void OnChangedPoints(const LPCDPOINT LPPointsOld);

	static void DrawDragPolyLine(CDC* pDC, LPPOINT lpPoints, int iWidth,
		LPPOINT lpPointsLast, int iWidthLast, UINT uiCount);
// Implementation
public:
   virtual ~CLineTracker();

protected:
   CSnapper* m_pSnapper;
   int m_enStyle;
   BOOL m_bShowPoints;
   BOOL m_bIgnoreX;
   BOOL m_bIgnoreY;

   static HCURSOR m_hPointTrackerCursor;
   static HCURSOR m_hPointMoveTrackerCursor;

   
   LPPOINT m_PointsLast;
	int m_iWidthLast;
	BOOL m_bErase;          // TRUE if DrawTrackerRect is called for erasing
	BOOL m_bFinalErase;     // TRUE if DragTrackerRect called for final erase

	// implementation helpers
	void GetPointRect(UINT uiHandle, CRect* pHandleRect) const;
	//virtual int GetHandleSize(LPCRECT lpRect = NULL) const;
	BOOL TrackHandle(UINT uiHandle, CWnd* pWnd, CPoint point, CWnd* pWndClipTo);
	void Construct();
};

#endif // !defined(AFX_LINETRACKER_H__3D37A967_B837_11D3_A497_00C0CA1261A6__INCLUDED_)
