/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// VecFrameContainer.h: interface for the CVecFrameContainer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECFRAMECONTAINER_H__9AC864F9_B6BC_11D3_A496_00C0CA1261A6__INCLUDED_)
#define AFX_VECFRAMECONTAINER_H__9AC864F9_B6BC_11D3_A496_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VecContainer.h"

class CVecFrameContainer : public CVecContainer  
{
public:
	CVecFrameContainer();
	virtual ~CVecFrameContainer();
   void InitFrameContainers();
   virtual void Copy(CVecBase* pVec);
   virtual void Draw(CDC * pDC, DWORD dwFlags = DF_NORMAL);
   //virtual void DoTransform();
   //virtual void DrawPoints(CDC * pDC);
   virtual CVecBase* HitTestObj(CDPoint &point);
   virtual CVecBase* HitTestHotItems(CDPoint &point);
   virtual HitTestResoult HitTest(CDPoint &p);
   virtual BOOL SetCursor(CWnd* pWnd, UINT nHitTest, UINT uiTool);
   virtual BOOL Track(CDPoint point, UINT uiTool, CUndoManager* pUndoMan, CSnapper* pSnapper = NULL);
   virtual BOOL OnDBClick(UINT nFlags, CPoint point, CUndoManager* pUndoMan);

   virtual void SaveToBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags = SBF_FULL);
   virtual void LoadFromBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags = SBF_FULL);
   virtual void StoreToBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags = SBF_FULL);
   virtual void RestoreFromBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags = SBF_FULL);

/*   virtual void DrawDragImage(CDC* pDC,LPDRECT lpRectNew,LPDRECT lpRectLast,LPDRECT lpRectOrigianl);
   virtual void DrawRotatedDragImage(CDC* pDC, double dAng, LPPOINT lpCenter, double dAngLast, LPPOINT lpCenterLast);
   virtual void Rotate(double dAng, LPDPOINT PtCenter = NULL);
   virtual void Flip(BOOL bHorizontal, LPDPOINT PtCenter = NULL);
   void SetRect(CDRect &rect);*/

/*   UINT GetSelectedItems(CPtrList &SelItems);
   UINT GetSelectedItemsCount();
   UINT GetItems(CPtrList &Items);
   void SetSelectedForAll(BOOL b);*/

   CVecContainer* GetBackContainer(){return m_pBackContainer;}
   CVecContainer* GetForeContainer(){return m_pForeContainer;}
   CVecContainer* GetHotContainer(){return m_pHotContainer;}

protected:
   virtual CVecBase* GetOverrideTracker(){return this;}

private:
   CVecContainer* m_pBackContainer;
   CVecContainer* m_pForeContainer;
   CVecContainer* m_pHotContainer;

};

#endif // !defined(AFX_VECFRAMECONTAINER_H__9AC864F9_B6BC_11D3_A496_00C0CA1261A6__INCLUDED_)
