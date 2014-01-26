// PolylineCreatorTracker.h: interface for the CPolylineCreatorTracker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POLYLINECREATORTRACKER_H__68B8AF24_C0DA_11D3_A49D_00C0CA1261A6__INCLUDED_)
#define AFX_POLYLINECREATORTRACKER_H__68B8AF24_C0DA_11D3_A49D_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSnapper;
#include "VecTypes.h"
#include "BaseTracker.h"

class CPolylineCreatorTracker : public CBaseTracker
{
public:
	CPolylineCreatorTracker();
	virtual ~CPolylineCreatorTracker();

   BOOL CreatePolyline(CWnd* pWnd, CDPoint point, CSnapper* pSnapper = NULL);
   BOOL CreateFreehand(CWnd* pWnd, CDPoint point, CSnapper* pSnapper = NULL);

   void GetPoints(LPCDPOINT lpPoints);
   UINT GetPointsCount();

protected:
	virtual void DrawTracker(POINT Point, CDC* pDC, CWnd* pWnd);
   LPPOINT m_PointsLast;
   CPtrList m_Points;
	BOOL m_bErase;          // TRUE if DrawTrackerRect is called for erasing
	BOOL m_bFinalErase;     // TRUE if DragTrackerRect called for final erase

};

#endif // !defined(AFX_POLYLINECREATORTRACKER_H__68B8AF24_C0DA_11D3_A49D_00C0CA1261A6__INCLUDED_)
