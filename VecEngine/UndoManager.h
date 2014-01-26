// UndoManager.h: interface for the CUndoManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNDOMANAGER_H__B9F11906_BB1B_4407_98FC_25D44BD18FB4__INCLUDED_)
#define AFX_UNDOMANAGER_H__B9F11906_BB1B_4407_98FC_25D44BD18FB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CVecBase;
class CVecContainer;

class CUndoManager  
{
public:
	CUndoManager();
	virtual ~CUndoManager();

   void AddToStack(CVecBase* pVec, UINT uiType);
   void AddToStack(CPtrList &objects, UINT uiType);
   void CreateNewStep();
   void AddActionToLastStep(CVecBase* pVec, UINT uiType);
   void PerformUndo(CVecContainer* pContainer);
   void PerformRedo(CVecContainer* pContainer);
   void ClearStack();
   BOOL IsUndoAvailable();
   BOOL IsRedoAvailable();
   inline void SetStepLimit(UINT ui){m_uiStepLimit = ui; AdjastStepsWithLimits();};
   inline UINT GetStepLimit(){return m_uiStepLimit;};
   inline void SetMemLimit(UINT ui){m_uiMemLimit = ui; AdjastStepsWithLimits();};
   inline UINT GetMemLimit(){return m_uiMemLimit;};
   UINT GetMemSize();

private:
   CPtrList m_UndoStack;//of CUndoStep
   CPtrList m_RedoStack;//of CUndoStep
   UINT m_uiStepLimit;
   UINT m_uiMemLimit;
   void AdjastStepsWithLimits();
   void ClearRedoStack();
};

class CUndoStep
{
public:
   CUndoStep();
   ~CUndoStep();
   void AddAction(CVecBase* pVec, UINT uiType);
   void PerformUndo(CVecContainer* pContainer, CUndoStep* pReverceStep);
   UINT GetMemSize();
private:
   CPtrList m_Actions;//of CUndoAction
};

enum UNDO_ACTION_TYPE
{
   UAT_RESTORE_OB_FROM_LIGHT_DUMP = 0,
   UAT_RESTORE_OB_FROM_DUMP,
   UAT_GROUP_ITEMS,
   UAT_UNGROUP_ITEMS,
   UAT_DELETE_ITEM,
   UAT_ADD_ITEM,
   UAT_RESTORE_ITEMS_ORDER,
   UAT_LAST_ITEM
};

class CUndoAction
{
public:
   CUndoAction();
   CUndoAction(CVecBase* pVec, UINT uiType);
   ~CUndoAction();
   void PerformAction(CVecContainer* pContainer, CUndoAction* pReverceAction);
   UINT GetMemSize(){if(m_hData==NULL) return 0; return GlobalSize(m_hData);}

private:
   void StoreAction(CVecBase* pVec, UINT uiType);
   BOOL StoreBeforeOrAfter(UINT uiType);
   UINT GetReverceAction(UINT uiType);
   
   UINT m_uiType;
   UINT m_uiVecID;
   UINT m_uiParentID;
   HGLOBAL m_hData;
   HGLOBAL m_hData2;
   UINT m_uiVar1;

private:
   //UAT_RESTORE_OB_FROM_LIGHT_DUMP
   void StoreLightObDump(CVecBase* pVec);
   void RestoreObFromLightDump(CVecBase* pVec);

   //UAT_RESTORE_OB_FROM_DUMP
   void StoreObDump(CVecBase* pVec);
   void RestoreObFromDump(CVecBase* pVec);

   //UAT_GROUP_ITEMS
   void StoreGroupBeforeUnGroupping(CVecBase* pVec);
   CVecBase* Group(CVecBase* pVec);

   //UAT_UNGROUP_ITEMS
   void StoreGroupForUnGroup(CVecBase* pVec);
   void UnGroup(CVecBase* pVec);

   //UAT_DELETE_ITEM
   void StoreOgIdForDelete(CVecBase* pVec);
   void Delete(CVecBase* pVec);

   //UAT_ADD_ITEM
   void StoreObDumpForAdd(CVecBase* pVec);
   CVecBase* Add(CVecBase* pVec);

   //UAT_RESTORE_ITEMS_ORDER
   void StoreItemsOrder(CVecBase* pVec);
   void RestoreItemsOrder(CVecBase* pVec);
};

#endif // !defined(AFX_UNDOMANAGER_H__B9F11906_BB1B_4407_98FC_25D44BD18FB4__INCLUDED_)
