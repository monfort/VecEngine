/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// VecPolyline.h: interface for the CVecPolyline class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECPOLYLINE_H__2972DE33_B8FF_11D3_A497_00C0CA1261A6__INCLUDED_)
#define AFX_VECPOLYLINE_H__2972DE33_B8FF_11D3_A497_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VecPointsBase.h"

class CVecPolyline : public CVecPointsBase  
{
public:
	CVecPolyline();
	virtual ~CVecPolyline();
   virtual void Draw(CDC * pDC, DWORD dwFlags = DF_NORMAL);
   virtual void DrawShadow(CDC * pDC, DWORD dwFlags = DF_NORMAL);
   virtual void DrawPoints(CDC * pDC);
   BOOL Track(CDPoint point, UINT uiTool, CUndoManager* pUndoMan, CSnapper* pSnapper = NULL);
	virtual BOOL SetCursor(CWnd* pWnd, UINT nHitTest, UINT uiTool);
   virtual HitTestResoult HitTest(CDPoint &p);
   virtual void DrawDragImage(CDC* pDC,LPDRECT lpRectNew,LPDRECT lpRectLast,LPDRECT lpRectOrigianl);
   virtual void DrawRotatedDragImage(CDC* pDC, double dAng, LPPOINT lpCenter, double dAngLast, LPPOINT lpCenterLast);
   inline BOOL IsTrackAsShape(){return m_bTrackAsShape;}
   virtual void SetTrackAsShape(BOOL b){m_bTrackAsShape = b;}
   virtual void SaveToBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags = SBF_FULL);
   virtual void LoadFromBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags = SBF_FULL);
   virtual void Copy(CVecBase* pVec);

protected:
   BOOL m_bTrackAsShape;
private:
   enum VEC_FIELDS // fields ids (file type) - do not cheange values
   {
      VF_TrackAsShape=0
   };
};

#endif // !defined(AFX_VECPOLYLINE_H__2972DE33_B8FF_11D3_A497_00C0CA1261A6__INCLUDED_)
