/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// RectangleTracker.h: interface for the CMRectTracker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECTANGLETRACKER_H__EC7A8ECF_DD43_418D_81C6_63292844AB5A__INCLUDED_)
#define AFX_RECTANGLETRACKER_H__EC7A8ECF_DD43_418D_81C6_63292844AB5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSnapper;
#include "BaseTracker.h"

class CRectangleTracker:  public CBaseTracker
{
public:
	CRectangleTracker(CDRectangle PicRect);
	virtual ~CRectangleTracker();

   enum TrackerHit
	{
		hitNothing = UINT(-3),
		hitInside = UINT(-2),
		hitLine = UINT(-1),
      hitDotTopLeft = 0,
      hitDotTopRight = 1,
      hitDotBottomRight = 2,
      hitDotBottomLeft = 3
	};

   void DrawDots(CDC* pDC) const;
	virtual BOOL SetCursor(CWnd* pWnd, UINT nHitTest, BOOL bRotate) const;
	BOOL Track(CWnd* pWnd, CPoint point, CSnapper* pSnapper = NULL, CWnd* pWndClipTo = NULL);

   virtual UINT HitTest(CPoint point) const;
	virtual UINT HitTestPoints(CPoint point) const;
	virtual UINT HitTestLines(CPoint point) const;

   CDRectangle m_rect;

   double m_dWidthDivHeight;
   BOOL m_bErase;          // TRUE if DrawTrackerRect is called for erasing
	BOOL m_bFinalErase;     // TRUE if DragTrackerRect called for final erase
	CRectangle m_RectLast;
   int m_iTrackingPointY;
   int m_iTrackingPointX;

protected:
   LPCDPOINT m_lpPoints;
   UINT m_uiCount;
   UINT m_nHandleSize;

	void GetPointRect(UINT uiHandle, CRect* pHandleRect) const;
   CDRectangle m_rectOrigianl;
   void DrawTracker(CDRectangle* lpRect, CWnd* pWndClipTo, CDC* pDC, CWnd* pWnd);

   static HCURSOR m_hPointTrackerCursor;
   static HCURSOR m_hPointMoveTrackerCursor;
};

#endif // !defined(AFX_RECTANGLETRACKER_H__EC7A8ECF_DD43_418D_81C6_63292844AB5A__INCLUDED_)
