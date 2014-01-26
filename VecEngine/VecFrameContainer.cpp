// VecFrameContainer.cpp: implementation of the CVecFrameContainer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VecFrameContainer.h"
#include "MemBuffEx.h"
#include "UndoManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVecFrameContainer::CVecFrameContainer()
{
   m_uiType = VEC_FRAME_CONTAINER;
   Insert(new CVecContainer);
   Insert(new CVecContainer);
   Insert(new CVecContainer);
   InitFrameContainers();
}

void CVecFrameContainer::InitFrameContainers()
{
   CPtrList Items;
   GetItems(Items);
   ASSERT(Items.GetCount()==3);//shuld be 3 containers
   POSITION pos = Items.GetHeadPosition();
   m_pForeContainer = (CVecContainer*)Items.GetNext(pos);
   m_pHotContainer = (CVecContainer*)Items.GetNext(pos);
   m_pBackContainer = (CVecContainer*)Items.GetNext(pos);

   ASSERT(m_pForeContainer!=NULL);
   ASSERT(m_pHotContainer!=NULL);
   ASSERT(m_pBackContainer!=NULL);
   ASSERT(m_pForeContainer->GetType()==VEC_CONTAINER);
   ASSERT(m_pHotContainer->GetType()==VEC_CONTAINER);
   ASSERT(m_pBackContainer->GetType()==VEC_CONTAINER);

   m_bBlockOpenedChange = FALSE;
   SetOpened(TRUE);
   m_bShowOpenedGourpMark = FALSE;
   m_bBlockSelectChange = FALSE;
   m_bBlockOpenedChange = TRUE;

   m_pHotContainer->m_bBlockOpenedChange = FALSE;
   m_pHotContainer->SetOpened(TRUE);
   m_pHotContainer->m_bShowOpenedGourpMark = FALSE;
   //m_pHotContainer->SetLocked(TRUE);

   m_pHotContainer->m_bBlockSelectChange = FALSE;
   m_pHotContainer->SetSelected(FALSE);
   m_pHotContainer->m_bBlockSelectChange = TRUE;
   m_pHotContainer->m_bBlockOpenedChange = TRUE;

   m_pForeContainer->m_bBlockSelectChange = FALSE;
   m_pForeContainer->SetSelected(FALSE);
   m_pForeContainer->m_bBlockSelectChange = TRUE;
   m_pForeContainer->m_bBlockOpenedChange  = TRUE;

   m_pBackContainer->m_bBlockSelectChange = FALSE;
   m_pBackContainer->SetSelected(FALSE);
   m_pBackContainer->m_bBlockSelectChange = TRUE;
   m_pBackContainer->m_bBlockOpenedChange  = TRUE;

}


CVecFrameContainer::~CVecFrameContainer()
{
}

void CVecFrameContainer::Copy(CVecBase* pVec)
{
   CVecContainer::Copy(pVec);

   m_pBackContainer = NULL;
   m_pForeContainer = NULL;
   m_pHotContainer = NULL;
   CVecFrameContainer* pVecContainer = dynamic_cast<CVecFrameContainer*>(pVec);
   if(pVecContainer!=NULL)
   {
      InitFrameContainers();
   }
}

void CVecFrameContainer::Draw(CDC * pDC, DWORD dwFlags)
{
   if(IsVisible())
   {
      m_pBackContainer->Draw(pDC, dwFlags);
      m_pHotContainer->Draw(pDC, dwFlags);
      m_pForeContainer->Draw(pDC, dwFlags);
   }
}

/*void CVecFrameContainer::DrawPoints(CDC * pDC)
{
   CVecContainer::DrawPoints(pDC);
   CPtrList Items;
   m_pHotContainer->GetItems(Items);
   while(!Items.IsEmpty())
   {
      CVecBase* pVec = (CVecBase*)Items.RemoveHead();
      if(pVec->IsSelected())
         pVec->DrawPoints(pDC);
   }
}*/

/*void CVecFrameContainer::DoTransform()
{
   CVecContainer::DoTransform();
   m_BackContainer.DoTransform();
   m_ForeContainer.DoTransform();
}

UINT CVecFrameContainer::GetSelectedItems(CPtrList &SelItems)
{
   CVecWorkspace* pWorkspace = GetWorkspace();
   if(pWorkspace)
   {
      return pWorkspace->m_SelectionManager.GetSelectedItems(SelItems);
   }
   else
   {
      SelItems.RemoveAll();

      UINT uiCount = 0;
      POSITION pos = m_Children.GetHeadPosition();
      while(pos != NULL)
      {
         CVecBase* pVec = (CVecBase*)m_Children.GetNext(pos);
         if(pVec->IsSelected())
         {
            uiCount++;
            SelItems.AddTail(pVec);
         }
      }
      return uiCount;
   }
}

UINT CVecFrameContainer::GetSelectedItemsCount()
{
   CVecWorkspace* pWorkspace = GetWorkspace();
   if(pWorkspace)
   {
      return pWorkspace->m_SelectionManager.GetSelectedItemsCount();
   }
   else
   {
      UINT uiCount = 0;
      POSITION pos = m_Children.GetHeadPosition();
      while(pos != NULL)
      {
         CVecBase* pVec = (CVecBase*)m_Children.GetNext(pos);
         if(pVec->IsSelected())
         {
            uiCount++;
         }
      }
      return uiCount;
   }
}

UINT CVecFrameContainer::GetItems(CPtrList &Items)
{
   Items.RemoveAll();

   UINT uiCount = 0;
   POSITION pos = m_Children.GetHeadPosition();
   while(pos != NULL)
   {
      CVecBase* pVec = (CVecBase*)m_Children.GetNext(pos);
      Items.AddTail(pVec);
   }
   return uiCount;
}

void CVecFrameContainer::SetSelectedForAll(BOOL b)
{
   POSITION pos = m_Children.GetHeadPosition();
   while(pos != NULL)
   {
      CVecBase* pVec = (CVecBase*)m_Children.GetNext(pos);
      pVec->SetSelected(b);
   }
}*/



CVecBase* CVecFrameContainer::HitTestObj(CDPoint &point)
{
   //hit test topo container
   CPtrList items;
   m_pForeContainer->GetItems(items);
   POSITION pos = items.GetHeadPosition();
   while(pos != NULL)
   {
      CVecBase* pVec = (CVecBase*)items.GetNext(pos);
      if(pVec->HitTest(point) != HT_NONE)
         return this;
   }

   CVecBase* pVec = NULL;
   pVec = HitTestHotItems(point);
   if(pVec!=NULL) return pVec;
   return this;
}

BOOL CVecFrameContainer::OnDBClick(UINT nFlags, CPoint point, CUndoManager* pUndoMan)
{
   CDPoint p(point);
   CVecBase* pVec = HitTestObj(p);
   if(pVec!=NULL && pVec!=this)
   {
      return pVec->OnDBClick(nFlags, point, pUndoMan);
   }
   return FALSE;
}



HitTestResoult CVecFrameContainer::HitTest(CDPoint &p)
{
   CVecBase* pVec = HitTestObj(p);
   if(pVec!=NULL && pVec!=this)
   {
      return pVec->HitTest(p);
   }

   return CVecContainer::HitTest(p);
}

CVecBase* CVecFrameContainer::HitTestHotItems(CDPoint &point)
{
   CPtrList items;
   m_pHotContainer->GetItems(items);
   POSITION pos = items.GetHeadPosition();
   while(pos != NULL)
   {
      CVecBase* pVec = (CVecBase*)items.GetNext(pos);
      if(pVec->HitTest(point) != HT_NONE)
         return pVec;
   }
   return NULL;
}

BOOL CVecFrameContainer::SetCursor(CWnd* pWnd, UINT nHitTest, UINT uiTool)
{
   CPoint point;
   GetCursorPos(&point);
   pWnd->ScreenToClient(&point);

   CDPoint p(point);
   CVecBase* pVec = HitTestObj(p);
   if(pVec!=NULL && pVec!=this)
   {
      if(pVec->SetCursor(pWnd, nHitTest, uiTool))
         return TRUE;
   }

   return CVecContainer::SetCursor(pWnd, nHitTest, uiTool);
}

BOOL CVecFrameContainer::Track(CDPoint point, UINT uiTool, CUndoManager* pUndoMan, CSnapper* pSnapper)
{
   CVecBase* pVec = HitTestObj(point);
   if(pVec!=NULL && pVec!=this)
   {
      int iHit = pVec->HitTest(point);
      if(iHit==HT_ON_MAPPING_POINT || iHit==HT_INSIDE_MAPPING)
         return pVec->Track(point, uiTool, pUndoMan, pSnapper);
   }

   return CVecContainer::Track(point, uiTool, pUndoMan, pSnapper);
}

void CVecFrameContainer::SaveToBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
{
   pMemBuff->WriteNodeHeader(VEC_PARENT);
   CVecContainer::SaveToBuffer(pMemBuff, iFlags);

   pMemBuff->WriteEndOfFields();
}

void CVecFrameContainer::LoadFromBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
{
   m_pBackContainer = NULL;
   m_pForeContainer = NULL;
   m_pHotContainer = NULL;
   while(pMemBuff->ReadNextFieldHeader())
   {
      int id = pMemBuff->GetFieldType();
      switch(id)
      {
      case ID_FIELD_IS_NODE: 
	      switch (pMemBuff->GetNodeType())
	      {
         case VEC_PARENT: 
            CVecContainer::LoadFromBuffer(pMemBuff, iFlags);
            break;
	      default: 
            pMemBuff->SkipField(); 
            break;
	      } 
         break;
      default: 
         pMemBuff->SkipField(); 
         break;
      }
   }
   InitFrameContainers();
}

void CVecFrameContainer::StoreToBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
{
   pMemBuff->WriteNodeHeader(VEC_PARENT);
   CVecContainer::StoreToBuffer(pMemBuff, iFlags);

   pMemBuff->WriteEndOfFields();
}

void CVecFrameContainer::RestoreFromBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
{
   BOOL bDontCreate = ((iFlags & SBF_DONT_CREATE_ITEMS) == SBF_DONT_CREATE_ITEMS);
   if(!bDontCreate)
   {
      m_pBackContainer = NULL;
      m_pForeContainer = NULL;
      m_pHotContainer = NULL;
   }

   while(pMemBuff->ReadNextFieldHeader())
   {
      int id = pMemBuff->GetFieldType();
      switch(id)
      {
      case ID_FIELD_IS_NODE: 
	      switch (pMemBuff->GetNodeType())
	      {
         case VEC_PARENT: 
            CVecContainer::RestoreFromBuffer(pMemBuff, iFlags);
            break;
	      default: 
            pMemBuff->SkipField(); 
            break;
	      } 
         break;
      default: 
         pMemBuff->SkipField(); 
         break;
      }
   }
   InitFrameContainers();
}


/*void CVecFrameContainer::DrawDragImage(CDC* pDC,LPDRECT lpRectNew,LPDRECT lpRectLast,LPDRECT lpRectOrigianl)
{
   m_BackContainer.DrawDragImage(pDC,lpRectNew,lpRectLast,lpRectOrigianl);
   CVecContainer::DrawDragImage(pDC,lpRectNew,lpRectLast,lpRectOrigianl);
   m_ForeContainer.DrawDragImage(pDC,lpRectNew,lpRectLast,lpRectOrigianl);
}

void CVecFrameContainer::DrawRotatedDragImage(CDC* pDC, double dAng, LPPOINT lpCenter, double dAngLast, LPPOINT lpCenterLast)
{
   m_BackContainer.DrawRotatedDragImage(pDC,dAng,lpCenter,dAngLast,lpCenterLast);
   CVecContainer::DrawRotatedDragImage(pDC,dAng,lpCenter,dAngLast,lpCenterLast);
   m_ForeContainer.DrawRotatedDragImage(pDC,dAng,lpCenter,dAngLast,lpCenterLast);
}

void CVecFrameContainer::SetRect(CDRect &rect)
{
   CDRect newRect = rect;
   CDRect oldRect = GetRect();

   CDRect drect(m_BackContainer.GetRect());
   CDPoint TopLeft = rect.TopLeft();
   CDPoint BottomRight = rect.BottomRight();
   ScaleAndOffsetPoint(&TopLeft, &newRect, &oldRect);
   ScaleAndOffsetPoint(&BottomRight, &newRect, &oldRect);
   drect = CDRect(TopLeft.x, TopLeft.y, BottomRight.x, BottomRight.y);
   m_BackContainer.SetRect(drect);

   drect = CDRect(CVecContainer::GetRect());
   TopLeft = rect.TopLeft();
   BottomRight = rect.BottomRight();
   ScaleAndOffsetPoint(&TopLeft, &newRect, &oldRect);
   ScaleAndOffsetPoint(&BottomRight, &newRect, &oldRect);
   drect = CDRect(TopLeft.x, TopLeft.y, BottomRight.x, BottomRight.y);
   CVecContainer::SetRect(drect);

   rect = CDRect(m_ForeContainer.GetRect());
   TopLeft = rect.TopLeft();
   BottomRight = rect.BottomRight();
   ScaleAndOffsetPoint(&TopLeft, &newRect, &oldRect);
   ScaleAndOffsetPoint(&BottomRight, &newRect, &oldRect);
   drect = CDRect(TopLeft.x, TopLeft.y, BottomRight.x, BottomRight.y);
   m_ForeContainer.SetRect(drect);
   
}


void CVecFrameContainer::Rotate(double dAng, LPDPOINT ptCenter)
{
   ASSERT(GetParent()!=NULL); 
   CDPoint center;
   if(ptCenter!=NULL)
      center = *ptCenter;
   else
      center = GetBoundingRect().CenterPoint();

   m_BackContainer.Rotate(dAng, &center);
   CVecContainer::Rotate(dAng, &center);
   m_ForeContainer.Rotate(dAng, &center);
}

void CVecFrameContainer::Flip(BOOL bHorizontal, LPDPOINT ptCenter)
{
   ASSERT(GetParent()!=NULL); 
   CDPoint center;
   if(ptCenter!=NULL)
      center = *ptCenter;
   else
      center = GetBoundingRect().CenterPoint();

   m_BackContainer.Rotate(bHorizontal, &center);
   CVecContainer::Rotate(bHorizontal, &center);
   m_ForeContainer.Rotate(bHorizontal, &center);
}
*/