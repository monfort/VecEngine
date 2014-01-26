// SekectionManager.cpp: implementation of the SelectionManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SelectionManager.h"
#include "VecContainer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSelectionManager::CSelectionManager()
{
   m_pWorkspace = NULL;
}

CSelectionManager::~CSelectionManager()
{
}

void CSelectionManager::CheckItem(CVecBase* pVec)
{
   POSITION pos = m_SelectedItems.Find(pVec);
 
   if(pVec->IsSelected() && pos==NULL)
      m_SelectedItems.AddTail(pVec);
   else if(!pVec->IsSelected() && pos!=NULL)
      m_SelectedItems.RemoveAt(pos);
}
void CSelectionManager::RemoveItem(CVecBase* pVec)
{
   POSITION pos = m_SelectedItems.Find(pVec);
   if(pos!=NULL)
      m_SelectedItems.RemoveAt(pos);
}

UINT CSelectionManager::GetSelectedItems(CPtrList& SellItems)
{
   SellItems.RemoveAll();
   SellItems.AddHead(&m_SelectedItems);
   return SellItems.GetCount();
}

UINT CSelectionManager::GetSelectedItemsCount()
{
   return m_SelectedItems.GetCount();
}

void CSelectionManager::SetSelectedNone()
{
   POSITION pos = m_SelectedItems.GetHeadPosition();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)m_SelectedItems.GetNext(pos);
      pVec->SetSelected(FALSE);
   }
}
