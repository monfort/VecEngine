// SuperTracker.h: interface for the CSuperTracker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUPERTRACKER_H__C36129A3_B9D0_11D3_A497_00C0CA1261A6__INCLUDED_)
#define AFX_SUPERTRACKER_H__C36129A3_B9D0_11D3_A497_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VecTypes.h"
#include "BaseTracker.h"
class CSnapper;

class CSuperTracker : public CBaseTracker
{
public:
	enum StyleFlags
	{
		Style_Move = 1, Style_Size = 2,
	};

   CSuperTracker(CPtrList &VecList);
	virtual ~CSuperTracker();
	BOOL Track(CWnd* pWnd, CPoint point, StyleFlags iStyle, CSnapper* pSnapper = NULL, CWnd* pWndClipTo = NULL);

   CDRect m_rectOrigianl;
   CDRect m_rect;

private:
   virtual void DrawTrackers(LPDRECT lpRect, CWnd* pWndClipTo,
		CDC* pDC, CWnd* pWnd);

   double m_dWidthDivHeight;
   BOOL m_bErase;          // TRUE if DrawTrackerRect is called for erasing
	BOOL m_bFinalErase;     // TRUE if DragTrackerRect called for final erase
	CDRect *m_PrectLast;
   int m_iTrackingPointY;
   int m_iTrackingPointX;
   CPtrList &m_VecList;
};

#endif // !defined(AFX_SUPERTRACKER_H__C36129A3_B9D0_11D3_A497_00C0CA1261A6__INCLUDED_)
