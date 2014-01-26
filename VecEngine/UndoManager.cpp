// UndoManager.cpp: implementation of the CUndoManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UndoManager.h"
#include "VecContainer.h"
#include "MemBuffEx.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////UndoManager///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

CUndoManager::CUndoManager()
{
   m_uiStepLimit = 0;
   m_uiMemLimit = 10*1024*1024;
}

CUndoManager::~CUndoManager()
{
   ClearStack();
}

void CUndoManager::AdjastStepsWithLimits()
{
   while(true)
   {
      BOOL bCountOk = FALSE;
      BOOL bMemOk = FALSE;
      if(m_uiStepLimit>0 && m_uiStepLimit < (UINT)m_UndoStack.GetCount())
         bCountOk = FALSE;
      else
         bCountOk = TRUE;

      if(bCountOk)
      {
         if(m_uiMemLimit>0 && m_uiMemLimit < GetMemSize())
            bMemOk = FALSE;
         else
            bMemOk = TRUE;
      }

      if(!bCountOk || !bMemOk)
      {
		 if(m_UndoStack.GetCount()<2)
			 break;//never delete the undo item we are currently working on (tail==head)
         CUndoStep* pStep = (CUndoStep*)m_UndoStack.RemoveTail();
         delete pStep;
      }
      else
         break;
   }
}

UINT CUndoManager::GetMemSize()
{
   UINT uiMemSize = 0;
   POSITION pos = m_UndoStack.GetTailPosition();
   while(pos!=NULL)
   {
      CUndoStep* pStep = (CUndoStep*)m_UndoStack.GetPrev(pos);
      uiMemSize += pStep->GetMemSize();
   }
   return uiMemSize;
}

void CUndoManager::AddToStack(CVecBase* pVec, UINT uiType)
{
   ClearRedoStack();
   CUndoStep* pStep = new CUndoStep();
   pStep->AddAction(pVec, uiType);
   m_UndoStack.AddHead(pStep);

   AdjastStepsWithLimits();
}

void CUndoManager::AddToStack(CPtrList &objects, UINT uiType)
{
   ClearRedoStack();
   CUndoStep* pStep = new CUndoStep();
   POSITION pos = objects.GetHeadPosition();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)objects.GetNext(pos);
      pStep->AddAction(pVec, uiType);
   }
   m_UndoStack.AddHead(pStep);

   AdjastStepsWithLimits();
}

void CUndoManager::CreateNewStep()
{
   ClearRedoStack();
   CUndoStep* pStep = new CUndoStep();
   m_UndoStack.AddHead(pStep);
}

void CUndoManager::AddActionToLastStep(CVecBase* pVec, UINT uiType)
{
   CUndoStep* pStep = (CUndoStep*)m_UndoStack.GetHead();
   pStep->AddAction(pVec, uiType);
   AdjastStepsWithLimits();
}

void CUndoManager::PerformUndo(CVecContainer* pContainer)
{
   if(m_UndoStack.IsEmpty())
      return;
   CUndoStep* pStep = (CUndoStep*)m_UndoStack.RemoveHead();

   CUndoStep* pReverceStep = new CUndoStep;

   pStep->PerformUndo(pContainer, pReverceStep);
   delete pStep;

   m_RedoStack.AddHead(pReverceStep);
}

void CUndoManager::PerformRedo(CVecContainer* pContainer)
{
   if(m_RedoStack.IsEmpty())
      return;
   CUndoStep* pStep = (CUndoStep*)m_RedoStack.RemoveHead();

   CUndoStep* pReverceStep = new CUndoStep;

   pStep->PerformUndo(pContainer, pReverceStep);
   delete pStep;

   m_UndoStack.AddHead(pReverceStep);
}

void CUndoManager::ClearStack()
{
   while(!m_UndoStack.IsEmpty())
   {
      CUndoStep* pStep = (CUndoStep*)m_UndoStack.RemoveHead();
      delete pStep;
   }
   ClearRedoStack();
}

void CUndoManager::ClearRedoStack()
{
   while(!m_RedoStack.IsEmpty())
   {
      CUndoStep* pStep = (CUndoStep*)m_RedoStack.RemoveHead();
      delete pStep;
   }
}

BOOL CUndoManager::IsUndoAvailable()
{
   return !m_UndoStack.IsEmpty();
}

BOOL CUndoManager::IsRedoAvailable()
{
   return !m_RedoStack.IsEmpty();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////UndoStep//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
CUndoStep::CUndoStep()
{
}

CUndoStep::~CUndoStep()
{
   while(!m_Actions.IsEmpty())
   {
      CUndoAction* pAction = (CUndoAction*)m_Actions.RemoveHead();
      delete pAction;
   }
}

void CUndoStep::AddAction(CVecBase* pVec, UINT uiType)
{
   CUndoAction* pAction = new CUndoAction(pVec, uiType);
   m_Actions.AddHead(pAction);
}

void CUndoStep::PerformUndo(CVecContainer* pContainer, CUndoStep* pReverceStep)
{
   POSITION pos = m_Actions.GetHeadPosition();
   while(pos!=NULL)
   {
      CUndoAction* pAction = (CUndoAction*)m_Actions.GetNext(pos);

      CUndoAction* pReverceAction = new CUndoAction();

      pAction->PerformAction(pContainer, pReverceAction);

      pReverceStep->m_Actions.AddHead(pReverceAction);
   }
}

UINT CUndoStep::GetMemSize()
{
   UINT uiMemSize = 0;
   POSITION pos = m_Actions.GetTailPosition();
   while(pos!=NULL)
   {
      CUndoAction* pAction = (CUndoAction*)m_Actions.GetPrev(pos);
      uiMemSize += pAction->GetMemSize();
   }
   return uiMemSize;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////UndoAction////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

CUndoAction::CUndoAction()
{
   m_uiType = UINT(-1);
   m_uiParentID = m_uiVecID = UINT(-1);
   m_hData2 = m_hData = NULL;
}

CUndoAction::CUndoAction(CVecBase* pVec, UINT uiType)
{
   StoreAction(pVec, uiType);
}

void CUndoAction::StoreAction(CVecBase* pVec, UINT uiType)
{
   m_uiType = uiType;
   m_uiVecID = pVec->GetRunningID();
   if(pVec->GetParent()!=NULL)
      m_uiParentID = pVec->GetParent()->GetRunningID();
   m_hData = NULL;

   switch(m_uiType)
   {
   case UAT_RESTORE_OB_FROM_LIGHT_DUMP:
      StoreLightObDump(pVec);
      break;
   case UAT_RESTORE_OB_FROM_DUMP:
      StoreObDump(pVec);
      break;
   case UAT_GROUP_ITEMS:
      StoreGroupBeforeUnGroupping(pVec);
      break;
   case UAT_UNGROUP_ITEMS:
      StoreGroupForUnGroup(pVec);
      break;
   case UAT_DELETE_ITEM:
      StoreOgIdForDelete(pVec);
      break;
   case UAT_ADD_ITEM:
      StoreObDumpForAdd(pVec);
      break;
   case UAT_RESTORE_ITEMS_ORDER:
      StoreItemsOrder(pVec);
      break;
   default:
      ASSERT(FALSE);
      break;
   }
}

UINT CUndoAction::GetReverceAction(UINT uiType)
{
   switch(uiType)//find the reverce action
   {
   case UAT_RESTORE_OB_FROM_LIGHT_DUMP:
   case UAT_RESTORE_OB_FROM_DUMP:
   case UAT_RESTORE_ITEMS_ORDER:
      break;
   case UAT_GROUP_ITEMS:
      uiType = UAT_UNGROUP_ITEMS;
      break;
   case UAT_UNGROUP_ITEMS:
      uiType = UAT_GROUP_ITEMS;
      break;
   case UAT_DELETE_ITEM:
      uiType = UAT_ADD_ITEM;
      break;
   case UAT_ADD_ITEM:
      uiType = UAT_DELETE_ITEM;
      break;
   default:
      ASSERT(FALSE);
      break;
   }

   return uiType;
}

BOOL CUndoAction::StoreBeforeOrAfter(UINT uiType)
{
   switch(uiType)
   {
   case UAT_RESTORE_OB_FROM_LIGHT_DUMP:
   case UAT_RESTORE_OB_FROM_DUMP:
   case UAT_RESTORE_ITEMS_ORDER:
   case UAT_UNGROUP_ITEMS:
   case UAT_DELETE_ITEM:
      return TRUE;
   case UAT_GROUP_ITEMS:
   case UAT_ADD_ITEM:
      return FALSE;
   }
   ASSERT(FALSE);
   return TRUE;
}

CUndoAction::~CUndoAction()
{
   if(m_hData!=NULL)
   {
      GlobalFree(m_hData);
   }
   if(m_hData2!=NULL)
   {
      GlobalFree(m_hData2);
   }
}

void CUndoAction::PerformAction(CVecContainer* pContainer, CUndoAction* pReverceAction)
{
   CVecBase* pVec = pContainer->GetObByRunningID(m_uiVecID);
   if(m_uiParentID!=(UINT(-1)))
   {
      CVecContainer* pParent = (CVecContainer*)pContainer->GetObByRunningID(m_uiParentID);
      if(pParent!=NULL)
         pContainer = pParent;
   }
   if(m_uiType == UAT_ADD_ITEM || m_uiType == UAT_GROUP_ITEMS/* || m_uiType == UAT_RESTORE_ITEMS_ORDER*/)
      pVec = pContainer;
   if(pVec==NULL)
   {
      ASSERT(FALSE);
      return;
   }

   UINT uiReverceAction = GetReverceAction(m_uiType);
   BOOL bStoreBeforeOrAfter = StoreBeforeOrAfter(m_uiType);
   if(bStoreBeforeOrAfter)
      pReverceAction->StoreAction(pVec, uiReverceAction);

   CVecBase* pNewVec = NULL;

   switch(m_uiType)
   {
   case UAT_RESTORE_OB_FROM_LIGHT_DUMP:
      RestoreObFromLightDump(pVec);
      break;
   case UAT_RESTORE_OB_FROM_DUMP:
      RestoreObFromDump(pVec);
      break;
   case UAT_GROUP_ITEMS:
      pNewVec = Group(pContainer);
      break;
   case UAT_UNGROUP_ITEMS:
      UnGroup(pVec);
      break;
   case UAT_DELETE_ITEM:
      Delete(pVec);
      break;
   case UAT_ADD_ITEM:
      pNewVec = Add(pContainer);
      break;
   case UAT_RESTORE_ITEMS_ORDER:
      RestoreItemsOrder(pVec);
      break;
   default:
      ASSERT(FALSE);
      break;
   }

   if(!bStoreBeforeOrAfter)
      pReverceAction->StoreAction(pNewVec == NULL ? pVec : pNewVec, uiReverceAction);
}


//UAT_RESTORE_OB_FROM_LIGHT_DUMP
void CUndoAction::StoreLightObDump(CVecBase* pVec)
{
   CNodedMemoryBufferEx membuff;
   pVec->StoreToBuffer(&membuff, SBF_LIGHT);
   if(m_hData!=NULL)
      GlobalFree(m_hData);
   UINT uiSize = membuff.CopyData(NULL,0);
   m_hData = GlobalAlloc(GMEM_MOVEABLE, uiSize);
   membuff.CopyData(GlobalLock(m_hData), uiSize);
   GlobalUnlock(m_hData);
}
void CUndoAction::RestoreObFromLightDump(CVecBase* pVec)
{
   CNodedMemoryBufferEx membuff(GlobalLock(m_hData), GlobalSize(m_hData));
   pVec->RestoreFromBuffer(&membuff, SBF_DONT_CREATE_ITEMS);
   GlobalUnlock(m_hData);
}

//UAT_RESTORE_OB_FROM_DUMP
void CUndoAction::StoreObDump(CVecBase* pVec)
{
   CNodedMemoryBufferEx membuff;
   pVec->StoreToBuffer(&membuff);
   if(m_hData!=NULL)
      GlobalFree(m_hData);
   UINT uiSize = membuff.CopyData(NULL,0);
   m_hData = GlobalAlloc(GMEM_MOVEABLE, uiSize);
   membuff.CopyData(GlobalLock(m_hData), uiSize);
   GlobalUnlock(m_hData);
}
void CUndoAction::RestoreObFromDump(CVecBase* pVec)
{
   CNodedMemoryBufferEx membuff(GlobalLock(m_hData), GlobalSize(m_hData));
   pVec->RestoreFromBuffer(&membuff, SBF_DONT_CREATE_ITEMS);
   GlobalUnlock(m_hData);
}

//UAT_GROUP_ITEMS
void CUndoAction::StoreGroupBeforeUnGroupping(CVecBase* pVec)
{
   if((pVec->GetType() != VEC_CONTAINER)||//must be a container
      (pVec->GetParent() == NULL))//must be a non top container
   {
      ASSERT(FALSE);
      return;
   }

   //save the group CVecBase Properties
   CNodedMemoryBufferEx membuff;
   pVec->StoreToBuffer(&membuff, SBF_SAVE_THIS_OB_ONLY);
   if(m_hData2!=NULL)
      GlobalFree(m_hData2);
   UINT uiSize = membuff.CopyData(NULL,0);
   m_hData2 = GlobalAlloc(GMEM_MOVEABLE, uiSize);
   membuff.CopyData(GlobalLock(m_hData2), uiSize);
   GlobalUnlock(m_hData2);
   
   CPtrList items;
   ((CVecContainer*)pVec)->GetItems(items);

   UINT uiCount = items.GetCount();
   if(m_hData!=NULL)
      GlobalFree(m_hData);
   m_hData = GlobalAlloc(GMEM_MOVEABLE, uiCount*sizeof(UINT));
   UINT* aIDs = (UINT*)GlobalLock(m_hData);

   POSITION pos = items.GetHeadPosition();
   UINT ui = 0;
   while(pos!=NULL)
   {
      aIDs[ui++] = ((CVecBase*)items.GetNext(pos))->GetRunningID();
   }

   GlobalUnlock(m_hData);

}
CVecBase* CUndoAction::Group(CVecBase* pVec)
{
   if((pVec->GetType() != VEC_CONTAINER))//must be a container
   {
      ASSERT(FALSE);
      return NULL;
   }

   CVecContainer* pContainer = ((CVecContainer*)pVec);
   CPtrList items;
   CPtrList group;
   pContainer->GetItems(items);

   UINT uiCount = GlobalSize(m_hData)/sizeof(UINT);
   UINT* aIDs = (UINT*)GlobalLock(m_hData);
   
   for(UINT ui = 0; ui<uiCount; ui++)
   {
      POSITION pos = items.GetHeadPosition();

      BOOL bFound = FALSE;
      while(pos!=NULL)
      {
         CVecBase* pVecItem = ((CVecBase*)items.GetNext(pos));
         if(pVecItem->GetRunningID() == aIDs[ui])
         {
            group.AddHead(pVecItem);
            bFound = TRUE;
            break;
         }
      }
      ASSERT(bFound);
   }

   GlobalUnlock(m_hData);
   CVecBase* pGroup = pContainer->Group(group);
   pGroup->SetRunningID(m_uiVecID);

   CNodedMemoryBufferEx membuff(GlobalLock(m_hData2), GlobalSize(m_hData2));
   pGroup->RestoreFromBuffer(&membuff, SBF_DONT_CREATE_ITEMS);
   GlobalUnlock(m_hData2);
   
   pGroup->SetSelected(TRUE);
   return pGroup;
}

//UAT_UNGROUP_ITEMS
void CUndoAction::StoreGroupForUnGroup(CVecBase* pVec)
{
   if(m_hData!=NULL)
      GlobalFree(m_hData);
   m_hData = NULL;
}
void CUndoAction::UnGroup(CVecBase* pVec)
{
   if((pVec->GetType() != VEC_CONTAINER)||//must be a container
      (pVec->GetParent() == NULL))//must be a non top container
   {
      ASSERT(FALSE);
      return;
   }

   CVecContainer* pContainer = ((CVecContainer*)pVec);
   CPtrList items;
   pContainer->Ungroup(items);
   while(!items.IsEmpty())
   {
      CVecBase* pVec = (CVecBase*)items.RemoveTail();
      pVec->SetSelected(TRUE);
   }
   delete pContainer;
}

//UAT_DELETE_ITEM
void CUndoAction::StoreOgIdForDelete(CVecBase* pVec)
{
   if(m_hData!=NULL)
      GlobalFree(m_hData);
   m_hData = NULL;
}
void CUndoAction::Delete(CVecBase* pVec)
{
   delete pVec;
}

//UAT_ADD_ITEM
void CUndoAction::StoreObDumpForAdd(CVecBase* pVec)
{
   m_uiVar1 = pVec->GetType();
   CNodedMemoryBufferEx membuff;
   pVec->StoreToBuffer(&membuff);
   if(m_hData!=NULL)
      GlobalFree(m_hData);
   UINT uiSize = membuff.CopyData(NULL,0);
   m_hData = GlobalAlloc(GMEM_MOVEABLE, uiSize);
   membuff.CopyData(GlobalLock(m_hData), uiSize);
   GlobalUnlock(m_hData);
}
CVecBase* CUndoAction::Add(CVecBase* pVec)
{
   if((pVec->GetType() != VEC_CONTAINER))//must be a container
   {
      ASSERT(FALSE);
      return NULL;
   }

   CVecBase* pNewVec = CVecBase::CreateNewInstanceByType(m_uiVar1);
   pNewVec->SetRunningID(m_uiVecID);

   CNodedMemoryBufferEx membuff(GlobalLock(m_hData), GlobalSize(m_hData));
   pNewVec->RestoreFromBuffer(&membuff);
   GlobalUnlock(m_hData);

   ((CVecContainer*)pVec)->Insert(pNewVec);
   return pNewVec;
}

//UAT_RESTORE_ITEMS_ORDER
void CUndoAction::StoreItemsOrder(CVecBase* pVec)
{
   if((pVec->GetType() != VEC_CONTAINER))//must be a container
   {
      ASSERT(FALSE);
      return;
   }

   CPtrList items;
   ((CVecContainer*)pVec)->GetItems(items);

   UINT uiCount = items.GetCount();
   if(m_hData!=NULL)
      GlobalFree(m_hData);
   m_hData = GlobalAlloc(GMEM_MOVEABLE, uiCount*sizeof(UINT));
   UINT* aIDs = (UINT*)GlobalLock(m_hData);

   POSITION pos = items.GetHeadPosition();
   UINT ui = 0;
   while(pos!=NULL)
   {
      aIDs[ui++] = ((CVecBase*)items.GetNext(pos))->GetRunningID();
   }

   GlobalUnlock(m_hData);
}
void CUndoAction::RestoreItemsOrder(CVecBase* pVec)
{
   if((pVec->GetType() != VEC_CONTAINER))//must be a container
   {
      ASSERT(FALSE);
      return;
   }

   CVecContainer* pContainer = ((CVecContainer*)pVec);
   CPtrList items;
   CPtrList ItemsOrder;
   pContainer->GetItems(items);

   UINT uiCount = GlobalSize(m_hData)/sizeof(UINT);
   UINT* aIDs = (UINT*)GlobalLock(m_hData);
   
   for(UINT ui = 0; ui<uiCount; ui++)
   {
      POSITION pos = items.GetHeadPosition();

      BOOL bFound = FALSE;
      while(pos!=NULL)
      {
         CVecBase* pVecItem = ((CVecBase*)items.GetNext(pos));
         if(pVecItem->GetRunningID() == aIDs[ui])
         {
            ItemsOrder.AddTail(pVecItem);
            bFound = TRUE;
            break;
         }
      }
      ASSERT(bFound);
   }

   GlobalUnlock(m_hData);
   pContainer->SetIOtemsOrder(ItemsOrder);
}
