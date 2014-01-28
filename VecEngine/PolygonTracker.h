/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// PolygonTracker.h: interface for the CPolygonTracker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POLYGONTRACKER_H__A7DA2C88_BC21_11D3_A49A_00C0CA1261A6__INCLUDED_)
#define AFX_POLYGONTRACKER_H__A7DA2C88_BC21_11D3_A49A_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineTracker.h"

class CPolygonTracker : public CLineTracker
{
public:
	CPolygonTracker();
	CPolygonTracker(LPCDPOINT lpPoints, UINT uiCount, int enStyle = styleShowPoints);


   virtual UINT HitTest(CPoint point) const;
	virtual UINT HitTestLines(CPoint point) const;

// Overridables
	virtual void DrawTracker(LPCDPOINT lpPoints, CWnd* pWndClipTo,
		CDC* pDC, CWnd* pWnd);
	static void DrawDragPolygon(CDC* pDC, LPPOINT lpPoints, int iWidth,
		LPPOINT lpPointsLast, int iWidthLast, UINT uiCount);

};

#endif // !defined(AFX_POLYGONTRACKER_H__A7DA2C88_BC21_11D3_A49A_00C0CA1261A6__INCLUDED_)
