/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// VecEllipsePic.h: interface for the CVecEllipsePic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECELLIPSEPIC_H__DEAD8EE9_BDC0_11D3_A49C_00C0CA1261A6__INCLUDED_)
#define AFX_VECELLIPSEPIC_H__DEAD8EE9_BDC0_11D3_A49C_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VecBitmapBase.h"
#include "VecEllipse.h"

class CVecEllipsePic : 	public CVecEllipse, 	public CVecBitmapBase  
{
public:
	CVecEllipsePic();
	virtual ~CVecEllipsePic();
   virtual void Draw(CDC * pDC, DWORD dwFlags = DF_NORMAL);
   virtual void Copy(CVecBase* pVec);

   virtual BOOL Track(CDPoint point, UINT uiTool, CUndoManager* pUndoMan, CSnapper* pSnapper = NULL);
	virtual BOOL SetCursor(CWnd* pWnd, UINT nHitTest, UINT uiTool);
   virtual void DrawPoints(CDC * pDC);
   virtual HitTestResoult HitTest(CDPoint &p);
   virtual BOOL OnDBClick(UINT nFlags, CPoint point, CUndoManager* pUndoMan);
   virtual void SaveToBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags = SBF_FULL);
   virtual void LoadFromBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags = SBF_FULL);

public://types
   virtual BOOL HasPicture(){return TRUE;}

};

#endif // !defined(AFX_VECELLIPSEPIC_H__DEAD8EE9_BDC0_11D3_A49C_00C0CA1261A6__INCLUDED_)
