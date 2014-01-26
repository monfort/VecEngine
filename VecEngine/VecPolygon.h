// VecPolygon.h: interface for the CVecPolygon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECPOLYGON_H__A7DA2C87_BC21_11D3_A49A_00C0CA1261A6__INCLUDED_)
#define AFX_VECPOLYGON_H__A7DA2C87_BC21_11D3_A49A_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VecPointsBase.h"

class CVecPolygon : public CVecPointsBase  
{
public:
	CVecPolygon();
	virtual ~CVecPolygon();
   virtual void Draw(CDC * pDC, DWORD dwFlags = DF_NORMAL);
   virtual void DrawShadow(CDC * pDC, DWORD dwFlags = DF_NORMAL);
   virtual void DrawPoints(CDC * pDC);
   BOOL Track(CDPoint point, UINT uiTool, CUndoManager* pUndoMan, CSnapper* pSnapper = NULL);
	virtual BOOL SetCursor(CWnd* pWnd, UINT nHitTest, UINT uiTool);
   virtual HitTestResoult HitTest(CDPoint &p);
   virtual void DrawDragImage(CDC* pDC,LPDRECT lpRectNew,LPDRECT lpRectLast,LPDRECT lpRectOrigianl);
   virtual void DrawRotatedDragImage(CDC* pDC, double dAng, LPPOINT lpCenter, double dAngLast, LPPOINT lpCenterLast);
   virtual void Copy(CVecBase* pVec);
   inline BOOL IsTrackAsShape(){return m_bTrackAsShape;}
   virtual void SetTrackAsShape(BOOL b){m_bTrackAsShape = b;}
   virtual void SaveToBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags = SBF_FULL);
   virtual void LoadFromBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags = SBF_FULL);

public://types
   virtual BOOL IsCloseShape(){return TRUE;}

protected:
   int m_TrackerStyle;
   BOOL m_bTrackAsShape;

private:
   enum VEC_FIELDS // fields ids (file type) - do not cheange values
   {
      VF_TrackerStyle=0,
      VF_TrackAsShape,
   };
};

#endif // !defined(AFX_VECPOLYGON_H__A7DA2C87_BC21_11D3_A49A_00C0CA1261A6__INCLUDED_)
