// VecEllipse.h: interface for the CVecEllipse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECELLIPSE_H__12AA3CCC_BCEE_11D3_A49C_00C0CA1261A6__INCLUDED_)
#define AFX_VECELLIPSE_H__12AA3CCC_BCEE_11D3_A49C_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VecBase.h"

class CVecEllipse : public CVecBase  
{
public:
	CVecEllipse();
	virtual ~CVecEllipse();
   BOOL Track(CDPoint point, UINT uiTool, CUndoManager* pUndoMan, CSnapper* pSnapper = NULL);
   virtual void DrawPoints(CDC * pDC);
   virtual void Draw(CDC * pDC, DWORD dwFlags = DF_NORMAL);
   virtual void DrawShadow(CDC * pDC, DWORD dwFlags = DF_NORMAL);
	virtual BOOL SetCursor(CWnd* pWnd, UINT nHitTest, UINT uiTool);
   virtual HitTestResoult HitTest(CDPoint &p);
   virtual void DrawDragImage(CDC* pDC,LPDRECT lpRectNew,LPDRECT lpRectLast,LPDRECT lpRectOrigianl);
   virtual void Rotate(double dAng, LPDPOINT PtCenter = NULL);
   virtual void Flip(BOOL bHorizontal, LPDPOINT PtCenter = NULL);
   virtual void DrawRotatedDragImage(CDC* pDC, double dAng, LPPOINT lpCenter, double dAngLast, LPPOINT lpCenterLast);

public://types
   virtual BOOL IsCloseShape(){return TRUE;}
};

#endif // !defined(AFX_VECELLIPSE_H__12AA3CCC_BCEE_11D3_A49C_00C0CA1261A6__INCLUDED_)
