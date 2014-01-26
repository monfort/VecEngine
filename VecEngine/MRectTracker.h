// MRectTracker.h: interface for the CMRectTracker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MRECTTRACKER_H__03491868_B76C_11D3_A497_00C0CA1261A6__INCLUDED_)
#define AFX_MRECTTRACKER_H__03491868_B76C_11D3_A497_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSnapper;
#include "VecTypes.h"
#include "BaseTracker.h"

class CMRectTracker: public CBaseTracker
{
public:
// Constructors
	CMRectTracker();
	CMRectTracker(LPCDRECT lpSrcRect, UINT nStyle);

// Style Flags
	enum StyleFlags
	{
		solidLine = 1, dottedLine = 2, hatchedBorder = 4,
		resizeInside = 8, resizeOutside = 16, hatchInside = 32, cornersOnly = 64
	};

// Hit-Test codes
	enum TrackerHit
	{
		hitNothing = -1,
		hitTopLeft = 0, hitTopRight = 1, hitBottomRight = 2, hitBottomLeft = 3,
		hitTop = 4, hitRight = 5, hitBottom = 6, hitLeft = 7, hitMiddle = 8
	};

// Attributes
	UINT m_nStyle;      // current state
	CDRect m_rect;       // current position (always in pixels)
	CDSize m_sizeMin;    // minimum X and Y size during track operation
	int m_nHandleSize;  // size of resize handles (default from WIN.INI)

// Operations
	void Draw(CDC* pDC) const;
	void GetTrueRect(LPRECT lpTrueRect) const;
	BOOL SetCursor(CWnd* pWnd, UINT nHitTest) const;
	BOOL Track(CWnd* pWnd, CPoint point, BOOL bAllowInvert = FALSE, CSnapper* pSnapper = NULL, CWnd* pWndClipTo = NULL);
	BOOL TrackRubberBand(CWnd* pWnd, CPoint point, BOOL bAllowInvert = TRUE, CSnapper* pSnapper = NULL);
	virtual int HitTest(CPoint point) const;
	int NormalizeHit(int nHandle) const;

// Overridables
   virtual void DoDrawTracker(CDC* pDC, LPCRECT lpRect, SIZE size,	LPCRECT lpRectLast, SIZE sizeLast);
	virtual void DrawTracker(LPCRECT lpRect, CWnd* pWndClipTo, CDC* pDC, CWnd* pWnd);
	virtual void AdjustRect(int nHandle, LPCDRECT lpRect);
	virtual void OnChangedRect(const CRect& rectOld);
	virtual UINT GetHandleMask() const;

	static void DrawDragRect(CDC* pDC, LPCRECT lpRect, SIZE size,
		LPCRECT lpRectLast, SIZE sizeLast,
		CBrush* pBrush = NULL, CBrush* pBrushLast = NULL);
// Implementation
public:
	virtual ~CMRectTracker();

protected:
   CSnapper* m_pSnapper;
   double m_dWidthDivHeight;
	BOOL m_bAllowInvert;    // flag passed to Track or TrackRubberBand
	CRect m_rectLast;
	CSize m_sizeLast;
	BOOL m_bErase;          // TRUE if DrawTrackerRect is called for erasing
	BOOL m_bFinalErase;     // TRUE if DragTrackerRect called for final erase

	// implementation helpers
	int HitTestHandles(CPoint point) const;
	void GetHandleRect(int nHandle, CRect* pHandleRect) const;
	void GetModifyPointers(int nHandle, double**ppx, double**ppy, double* px, double*py);
	virtual int GetHandleSize(LPCRECT lpRect = NULL) const;
	BOOL TrackHandle(int nHandle, CWnd* pWnd, CPoint point, CWnd* pWndClipTo);
	void Construct();
};

#endif // !defined(AFX_MRECTTRACKER_H__03491868_B76C_11D3_A497_00C0CA1261A6__INCLUDED_)
