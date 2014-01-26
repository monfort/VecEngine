// VecLine.h: interface for the CVecLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECLINE_H__9AC864F7_B6BC_11D3_A496_00C0CA1261A6__INCLUDED_)
#define AFX_VECLINE_H__9AC864F7_B6BC_11D3_A496_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VecBase.h"

class CVecLine : public CVecBase
{
public:
	CVecLine();
	virtual ~CVecLine();
   virtual void Draw(CDC * pDC, DWORD dwFlags = DF_NORMAL);
   virtual void DrawShadow(CDC * pDC, DWORD dwFlags = DF_NORMAL);
   virtual void DrawPoints(CDC * pDC);
   BOOL Track(CDPoint point, UINT uiTool, CUndoManager* pUndoMan, CSnapper* pSnapper = NULL);
	virtual BOOL SetCursor(CWnd* pWnd, UINT nHitTest, UINT uiTool);
   virtual HitTestResoult HitTest(CDPoint &p);
   virtual void DrawDragImage(CDC* pDC,LPDRECT lpRectNew,LPDRECT lpRectLast,LPDRECT lpRectOrigianl);
   virtual void DrawRotatedDragImage(CDC* pDC, double dAng, LPPOINT lpCenter, double dAngLast, LPPOINT lpCenterLast);

};

#endif // !defined(AFX_VECLINE_H__9AC864F7_B6BC_11D3_A496_00C0CA1261A6__INCLUDED_)
