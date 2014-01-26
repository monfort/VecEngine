// EllipseTracker.h: interface for the CEllipseTracker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELLIPSETRACKER_H__12AA3CCD_BCEE_11D3_A49C_00C0CA1261A6__INCLUDED_)
#define AFX_ELLIPSETRACKER_H__12AA3CCD_BCEE_11D3_A49C_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MRectTracker.h"

class CEllipseTracker : public CMRectTracker
{
public:
	CEllipseTracker();
	CEllipseTracker(LPCDRECT lpSrcRect, UINT nStyle);
	virtual ~CEllipseTracker();

   virtual int HitTest(CPoint point) const;
   virtual void DoDrawTracker(CDC* pDC, LPCRECT lpRect, SIZE size,	LPCRECT lpRectLast, SIZE sizeLast);

	static void DrawDragEllipse(CDC* pDC, LPCRECT lpRect, int iWidth,
		LPCRECT lpRectLast, int iWidthLast);
};

#endif // !defined(AFX_ELLIPSETRACKER_H__12AA3CCD_BCEE_11D3_A49C_00C0CA1261A6__INCLUDED_)
