// SuperRotateTracker.h: interface for the CSuperRotateTracker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUPERROTATETRACKER_H__A7DA2C86_BC21_11D3_A49A_00C0CA1261A6__INCLUDED_)
#define AFX_SUPERROTATETRACKER_H__A7DA2C86_BC21_11D3_A49A_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "BaseTracker.h"

class CSuperRotateTracker : public CBaseTracker
{
public:
   CSuperRotateTracker(CPtrList &VecList);
	virtual ~CSuperRotateTracker();
	BOOL Track(CWnd* pWnd, CPoint point, CWnd* pWndClipTo = NULL);

   double m_dAng;
   CPoint m_center;

private:
   virtual void DrawTrackers(double dAng, CWnd* pWndClipTo, CDC* pDC, CWnd* pWnd);

   CRect m_rect;
   CPoint *m_pCenterLast;
   BOOL m_bErase;          // TRUE if DrawTrackerRect is called for erasing
	BOOL m_bFinalErase;     // TRUE if DragTrackerRect called for final erase
	double m_dAngLast;
   CPtrList &m_VecList;
};

#endif // !defined(AFX_SUPERROTATETRACKER_H__A7DA2C86_BC21_11D3_A49A_00C0CA1261A6__INCLUDED_)
