// VecContainer.h: interface for the CVecContainer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECCONTAINER_H__9AC864F9_B6BC_11D3_A496_00C0CA1261A6__INCLUDED_)
#define AFX_VECCONTAINER_H__9AC864F9_B6BC_11D3_A496_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VecBase.h"
#include "VecWorkspace.h"

#define WM_VEC_EVENT WM_USER+5

enum VEC_EVENTS
{
   VE_MODIFIED,
   VE_MODIFIED_CHANGED,
   VE_SEL_CHANGE,
   VE_GUIDE_CREATED,
   VE_VEC_CREATED,
   VE_START_DRAG_DROP,
   VE_LAST
};

enum ITEMS_FLAGS
{
   IF_LOCAL,
   IF_RECURS,
   IF_RECURS_OPENED
};

class CVecContainer : public CVecBase  
{
   friend class CVecBase;
public:
	CVecContainer();
	CVecContainer(CWnd* pWnd);
	virtual ~CVecContainer();
   virtual void Draw(CDC * pDC, DWORD dwFlags = DF_NORMAL);
   virtual void DrawShadow(CDC * pDC, DWORD dwFlags = DF_NORMAL);
   virtual void DoTransform();
   virtual void DrawPoints(CDC * pDC);
   void Insert(CVecBase* pVecObj);
   void Remove(CVecBase* pVecObj);
   virtual CVecBase* HitTestObj(CDPoint &point);
   inline void SetWnd(CWnd* pWnd){m_pWnd = pWnd;}
   inline CWnd* GetWnd(){return m_pWnd;}
   virtual CVecContainer* GetTopContainer();
   void GetClippingRegion(CRgn& rgn);
   virtual CVecBase* GetObByRunningID(UINT uiID);
   virtual void SetSelected(BOOL b);
   virtual void SetLocked(BOOL b);
   inline BOOL IsOpened(){return m_bOpened;}
   virtual void SetOpened(BOOL b);
   virtual BOOL OnDBClick(UINT nFlags, CPoint point, CUndoManager* pUndoMan);
   virtual BOOL CheckIfToCloseGroups();

   virtual void SaveToBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags = SBF_FULL);
   virtual void LoadFromBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags = SBF_FULL);
   virtual void StoreToBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags = SBF_FULL);
   virtual void RestoreFromBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags = SBF_FULL);

   UINT GetSelectedItems(CPtrList &SelItems, DWORD dwFlags = IF_RECURS_OPENED);
   UINT GetSelectedItemsCount(DWORD dwFlags = IF_RECURS_OPENED);
   UINT GetItems(CPtrList &Items, DWORD dwFlags = IF_LOCAL);
   void SetSelectedForAll(BOOL b, DWORD dwFlags = IF_RECURS_OPENED);
   void SendToBack(CVecBase* pVec);
   void BringToFront(CVecBase* pVec);
   void SendOneBackward(CVecBase* pVec);
   void BringOneForward(CVecBase* pVec);
   void RemoveAll();
   virtual void Copy(CVecBase* pVec);
   void ReplaceItem(CVecBase* pVecTargetPos, CVecBase* pVecItem, BOOL bDeleteOld);
   void InsertBefore(CVecBase* pVecTargetPos, CVecBase* pVecItem);
   void InsertAfter(CVecBase* pVecTargetPos, CVecBase* pVecItem);
   void SetIOtemsOrder(CPtrList &Items);

   UINT Ungroup(CPtrList &Items); //this container is the group and you shuld delete it
   CVecContainer* Group(CPtrList &Items); //retuens the new container (the group)

   virtual HitTestResoult HitTest(CDPoint &p);
   virtual BOOL SetCursor(CWnd* pWnd, UINT nHitTest, UINT uiTool);
   virtual BOOL Track(CDPoint point, UINT uiTool, CUndoManager* pUndoMan, CSnapper* pSnapper = NULL);
   virtual void DrawDragImage(CDC* pDC,LPDRECT lpRectNew,LPDRECT lpRectLast,LPDRECT lpRectOrigianl);
   virtual void DrawRotatedDragImage(CDC* pDC, double dAng, LPPOINT lpCenter, double dAngLast, LPPOINT lpCenterLast);
   virtual void Rotate(double dAng, LPDPOINT PtCenter = NULL);
   virtual void Flip(BOOL bHorizontal, LPDPOINT PtCenter = NULL);
   void SetRect(CDRect &rect);
   virtual CDRect GetBoundingRect();
   virtual CDRect UpdateBoundingRect();
   inline CVecWorkspace* GetWorkspaceOb(){ return m_pVecWorkspace;}
   inline void SetWorkspaceOb(CVecWorkspace* ptr){ m_pVecWorkspace = ptr;}

   static int RemoveItemsThatThirParentIsInTheList(CPtrList& Items);
   static void ArrangeItemsForTrack(CPtrList& Items);

private:
   CWnd* m_pWnd;
   CPtrList m_Children;
   CVecWorkspace *m_pVecWorkspace;
   BOOL m_bOpened;
protected:
   virtual CVecBase* GetOverrideTracker(){if(GetParent()==NULL)return NULL; else return GetParent()->GetOverrideTracker();}

public:
   BOOL m_bShowOpenedGourpMark;
   BOOL m_bBlockSelectChange;
   BOOL m_bBlockOpenedChange;

private:
   enum VEC_FIELDS // fields ids (file type) - do not cheange values
   {
      VF_NextVecObjID=0
   };
};

#endif // !defined(AFX_VECCONTAINER_H__9AC864F9_B6BC_11D3_A496_00C0CA1261A6__INCLUDED_)
