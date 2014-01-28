/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// VecRectangle.h: interface for the CVecRectangle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECRECTANGLE_H__6318D409_B6F0_11D3_A497_00C0CA1261A6__INCLUDED_)
#define AFX_VECRECTANGLE_H__6318D409_B6F0_11D3_A497_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VecPolygon.h"

//#define TRACK_AS_RECT

class CVecRectangle : public CVecPolygon  
{
public:
	CVecRectangle();
	virtual ~CVecRectangle();
   BOOL Track(CDPoint point, UINT uiTool, CUndoManager* pUndoMan, CSnapper* pSnapper = NULL);

#ifdef TRACK_AS_RECT
   virtual void Draw(CDC * pDC, DWORD dwFlags = DF_NORMAL);
   virtual void DrawPoints(CDC * pDC);
	virtual BOOL SetCursor(CWnd* pWnd, UINT nHitTest, UINT uiTool);
   virtual HitTestResoult HitTest(CDPoint &p);
#endif

};

#endif // !defined(AFX_VECRECTANGLE_H__6318D409_B6F0_11D3_A497_00C0CA1261A6__INCLUDED_)
