/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// VecContainer.cpp: implementation of the CVecContainer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VecContainer.h"
#include "MRectTracker.h"
#include "SuperTracker.h"
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
#define ASSERT_CONTAINER_VALID ASSERT(m_pWnd!=NULL);

#define CHECK_GO_IN_GROUP(pContainer , dwFlags) (pContainer!=NULL && ((pContainer->IsOpened() && dwFlags == IF_RECURS_OPENED)|| dwFlags == IF_RECURS))


CVecContainer::CVecContainer()
{
   m_pWnd = NULL;
   m_pVecWorkspace = NULL;
   m_uiType = VEC_CONTAINER;
   m_bOpened = FALSE;

   m_bShowOpenedGourpMark = TRUE;
   m_bBlockSelectChange = FALSE;
   m_bBlockOpenedChange = FALSE;

}

CVecContainer::CVecContainer(CWnd* pWnd)
{
   SetWnd(pWnd);
}

CVecContainer::~CVecContainer()
{
   RemoveAll();
}

void CVecContainer::Copy(CVecBase* pVec)
{
   CVecBase::Copy(pVec);

   CVecContainer* pVecContainer = dynamic_cast<CVecContainer*>(pVec);
   if(pVecContainer!=NULL)
   {
      RemoveAll();
      CPtrList Items;
      pVecContainer->GetItems(Items);
      while(!Items.IsEmpty())
      {
         CVecBase *pVecItem = (CVecBase*)Items.RemoveTail();
         CVecBase *pNewItem = CVecBase::CreateNewInstanceByType(pVecItem->GetType());
         pNewItem->Copy(pVecItem);
         Insert(pNewItem);
      }

      m_bShowOpenedGourpMark = pVecContainer->m_bShowOpenedGourpMark;
      m_bBlockSelectChange = pVecContainer->m_bBlockSelectChange;
      m_bBlockOpenedChange = pVecContainer->m_bBlockOpenedChange;

   }
}

void CVecContainer::Draw(CDC * pDC, DWORD dwFlags)
{
   if(IsVisible())
   {
      CRect rect = m_t_Rect;
      if(IsOpened() && (dwFlags & DF_DONT_DRAW_SELECTION)==0 && m_bShowOpenedGourpMark)
      {
         rect.InflateRect(2,2);
         pDC->DrawEdge(&rect, EDGE_BUMP, BF_TOPLEFT|BF_BOTTOMRIGHT);
         rect = m_t_Rect;
      }
   
      CRgn rgn;
      GetClippingRegion(rgn);
      CRect bbbrect;
      int iDCSaved = pDC->SaveDC();
      pDC->SelectClipRgn(&rgn, RGN_AND);

      POSITION pos = m_Children.GetTailPosition();
      while(pos != NULL)
      {
         CVecBase* pVec = (CVecBase*)m_Children.GetPrev(pos);
         if(pVec->IsVisible())
         {
            CRect boundingRect;
            if(pVec->IsShowShadow())
            {
               boundingRect = pVec->GetRectByScreen();
               boundingRect.OffsetRect(pVec->m_t_ShadowOffset);
               if(pDC->RectVisible(&boundingRect))//draw only objects that within the visible region
               {
                  pVec->DrawShadow(pDC, dwFlags);
               }
            }
            boundingRect = pVec->GetRectByScreen();
            if(pDC->RectVisible(&boundingRect))//draw only objects that within the visible region
            {
/*               if(pVec->IsShowShadow())
               {
                  pVec->DrawShadow(pDC, dwFlags);
               }*/
               pVec->Draw(pDC, dwFlags);
               if(pVec->IsSelected() && (dwFlags & DF_DONT_DRAW_SELECTION)==0)
               {
                  pVec->DrawPoints(pDC);
                  if(pVec->IsLocked())
                     pVec->DrawLockedIcon(pDC);
               }
            }
         }
      }
      pDC->RestoreDC(iDCSaved);
   }
}

void CVecContainer::DrawShadow(CDC * pDC, DWORD dwFlags)
{
   POSITION pos = m_Children.GetTailPosition();
   while(pos != NULL)
   {
      CVecBase* pVec = (CVecBase*)m_Children.GetPrev(pos);
      if(pVec->IsVisible())
      {
         CRect boundingRect;
         boundingRect = pVec->GetRectByScreen();
         boundingRect.OffsetRect(m_t_ShadowOffset);
         if(pDC->RectVisible(&boundingRect))//draw only objects that within the visible region
         {
            int iRop2 = pVec->m_iShadowRop2;
            COLORREF col = pVec->m_ShadowColor;
            CDPoint offset = pVec->m_t_ShadowOffset;

            pVec->m_iShadowRop2 = m_iShadowRop2;
            pVec->m_ShadowColor = m_ShadowColor;
            pVec->m_t_ShadowOffset = m_t_ShadowOffset;

            pVec->DrawShadow(pDC, dwFlags);

            pVec->m_iShadowRop2 = iRop2;
            pVec->m_ShadowColor = col;
            pVec->m_t_ShadowOffset = offset;
         }
      }
   }
}

void CVecContainer::GetClippingRegion(CRgn& rgn)
{
   if(GetParent()==NULL)
   {
      rgn.DeleteObject();
      CRect rect = m_t_Rect;
      rgn.CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
   }
   else
      GetTopContainer()->GetClippingRegion(rgn);
}

void CVecContainer::DoTransform()
{
   if(m_pParentContainer!=NULL)
      UpdateBoundingRect();
   CVecBase::DoTransform();

   POSITION pos = m_Children.GetHeadPosition();
   while(pos != NULL)
   {
      CVecBase* pVec = (CVecBase*)m_Children.GetNext(pos);
      pVec->DoTransform();
   }
}

CVecContainer* CVecContainer::GetTopContainer()
{
   if(m_pParentContainer!=NULL)
      return m_pParentContainer->GetTopContainer();
   return this;
}

CVecBase* CVecContainer::HitTestObj(CDPoint &point)
{
/*   if(!m_t_Rect.PtInRect(point))
      return NULL;*/
   POSITION pos = m_Children.GetHeadPosition();
   while(pos != NULL)
   {
      CVecBase* pVec = (CVecBase*)m_Children.GetNext(pos);
      if(pVec->HitTest(point) != HT_NONE)
      {
         CVecContainer* pContainer = dynamic_cast<CVecContainer*>(pVec);
         if(pContainer!=NULL && pContainer->IsOpened())
            return pContainer->HitTestObj(point);
         else
            return pVec;
      }
   }
   return NULL;
}

UINT CVecContainer::GetSelectedItems(CPtrList &SelItems, DWORD dwFlags)
{
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
      CVecContainer* pContainer = dynamic_cast<CVecContainer*>(pVec);
      if(CHECK_GO_IN_GROUP(pContainer, dwFlags))
         uiCount+=pContainer->GetSelectedItems(SelItems, dwFlags);
   }
   return uiCount;
}

UINT CVecContainer::GetSelectedItemsCount(DWORD dwFlags)
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
      CVecContainer* pContainer = dynamic_cast<CVecContainer*>(pVec);
      if(CHECK_GO_IN_GROUP(pContainer, dwFlags))
         uiCount+=pContainer->GetSelectedItemsCount(dwFlags);
   }
   return uiCount;
}

UINT CVecContainer::GetItems(CPtrList &Items, DWORD dwFlags)
{
   UINT uiCount = 0;
   POSITION pos = m_Children.GetHeadPosition();
   while(pos != NULL)
   {
      CVecBase* pVec = (CVecBase*)m_Children.GetNext(pos);
      Items.AddTail(pVec);
      CVecContainer* pContainer = dynamic_cast<CVecContainer*>(pVec);
      if(CHECK_GO_IN_GROUP(pContainer, dwFlags))
         uiCount+=pContainer->GetItems(Items, dwFlags);
   }
   return uiCount;
}

void CVecContainer::SetSelectedForAll(BOOL b, DWORD dwFlags)
{
   POSITION pos = m_Children.GetHeadPosition();
   while(pos != NULL)
   {
      CVecBase* pVec = (CVecBase*)m_Children.GetNext(pos);
      pVec->SetSelected(b);
      CVecContainer* pContainer = dynamic_cast<CVecContainer*>(pVec);
      if(CHECK_GO_IN_GROUP(pContainer, dwFlags))
         pContainer->SetSelectedForAll(b, dwFlags);
   }
}

void CVecContainer::Insert(CVecBase* pVecObj)
{
   pVecObj->SetParent(this);
   m_Children.AddHead(pVecObj);
}

void CVecContainer::Remove(CVecBase* pVecObj)
{
   pVecObj->SetParent(NULL);
   POSITION pos = m_Children.Find(pVecObj);
   ASSERT(pos!=NULL);
   m_Children.RemoveAt(pos);
}

void CVecContainer::SendToBack(CVecBase* pVec)
{
   POSITION pos = m_Children.Find(pVec);
   ASSERT(pos!=NULL);
   m_Children.RemoveAt(pos);
   m_Children.AddTail(pVec);
}

void CVecContainer::BringToFront(CVecBase* pVec)
{
   POSITION pos = m_Children.Find(pVec);
   ASSERT(pos!=NULL);
   m_Children.RemoveAt(pos);
   m_Children.AddHead(pVec);
}

void CVecContainer::SendOneBackward(CVecBase* pVec)
{
   POSITION pos = m_Children.Find(pVec);
   ASSERT(pos!=NULL);
   POSITION oldpos = pos;
   m_Children.GetNext(pos);
   m_Children.RemoveAt(oldpos);
   m_Children.InsertAfter(pos, pVec);
}

void CVecContainer::BringOneForward(CVecBase* pVec)
{
   POSITION pos = m_Children.Find(pVec);
   ASSERT(pos!=NULL);
   POSITION oldpos = pos;
   m_Children.GetPrev(pos);
   m_Children.RemoveAt(oldpos);
   m_Children.InsertBefore(pos, pVec);
}

void CVecContainer::RemoveAll()
{
   while(!m_Children.IsEmpty())
   {
      CVecBase* pVec = (CVecBase*)m_Children.RemoveHead();
      pVec->SetParent(NULL);
      delete pVec;
   }
}

void CVecContainer::ReplaceItem(CVecBase* pVecTargetPos, CVecBase* pVecItem, BOOL bDeleteOld)
{
   pVecTargetPos->SetParent(NULL);
   POSITION pos = m_Children.Find(pVecTargetPos);
   ASSERT(pos!=NULL);
   m_Children.SetAt(pos, pVecItem);
   pVecItem->SetParent(this);

   if(bDeleteOld)
      delete pVecTargetPos;
}

void CVecContainer::InsertBefore(CVecBase* pVecTargetPos, CVecBase* pVecItem)
{
   POSITION pos = m_Children.Find(pVecTargetPos);
   ASSERT(pos!=NULL);
   m_Children.InsertBefore(pos, pVecItem);
   pVecItem->SetParent(this);
}

void CVecContainer::InsertAfter(CVecBase* pVecTargetPos, CVecBase* pVecItem)
{
   POSITION pos = m_Children.Find(pVecTargetPos);
   ASSERT(pos!=NULL);
   m_Children.InsertAfter(pos, pVecItem);
   pVecItem->SetParent(this);
}

void CVecContainer::SetIOtemsOrder(CPtrList &Items)
{
   POSITION pos = Items.GetTailPosition();
   while(pos != NULL)
   {
      CVecBase* pVec = (CVecBase*)Items.GetPrev(pos);
      POSITION ItemPos = m_Children.Find(pVec);
      ASSERT(ItemPos!=NULL);
      m_Children.RemoveAt(ItemPos);
      m_Children.AddHead(pVec);
   }
}

void CVecContainer::DrawPoints(CDC * pDC)
{
   CMRectTracker rt(m_t_Rect, CMRectTracker::resizeOutside | CMRectTracker::cornersOnly);
   rt.Draw(pDC);
}

HitTestResoult CVecContainer::HitTest(CDPoint &p)
{
   ASSERT(GetParent()!=NULL); 

   CMRectTracker rt(m_t_Rect, CMRectTracker::resizeOutside | CMRectTracker::cornersOnly);
   int iHit = rt.HitTest(p);
   if(iHit != CMRectTracker::hitMiddle && iHit != CMRectTracker::hitNothing && IsSelected())
      return HT_ON_POINT;

   POSITION pos = m_Children.GetHeadPosition();
   while(pos != NULL)
   {
      CVecBase* pVec = (CVecBase*)m_Children.GetNext(pos);
      HitTestResoult hit = pVec->HitTest(p);
      if(hit != HT_NONE)
         return HT_INSIDE;
   }

   return HT_NONE;
}

BOOL CVecContainer::SetCursor(CWnd* pWnd, UINT nHitTest, UINT uiTool)
{
   if(GetParent()==NULL)
      return CVecBase::SetCursor(pWnd, nHitTest, uiTool); 

   CPoint point;
   GetCursorPos(&point);
   pWnd->ScreenToClient(&point);

   CMRectTracker rt(m_t_Rect, CMRectTracker::resizeOutside | CMRectTracker::cornersOnly);
   int iHit = rt.HitTest(point);
   if(iHit != CMRectTracker::hitMiddle && iHit != CMRectTracker::hitNothing)
   {
      rt.SetCursor(pWnd, nHitTest);//show cursor of the rect tracker
      return TRUE;
   }

   CDPoint dpoint(point);
   if(HitTest(dpoint)!=HT_NONE)
   {
      rt.SetCursor(pWnd, nHitTest);//show the move cursor of the rect tracker
      return TRUE;
   }
   return FALSE;
}

BOOL CVecContainer::Track(CDPoint point, UINT uiTool, CUndoManager* pUndoMan, CSnapper* pSnapper)
{
   ASSERT(GetParent()!=NULL); 

   CSuperTracker st(m_Children);
   st.SetTrackRetainProportions();
   if(st.Track(GetTopContainer()->GetWnd(), point, 
      HitTest(point) == HT_ON_POINT ? CSuperTracker::Style_Size: CSuperTracker::Style_Move, pSnapper))
   {
      pUndoMan->AddToStack(this, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
      SetRectScaleAndOffsetByScreen(st.m_rect, st.m_rectOrigianl);
      return TRUE;
   }

   return FALSE;
}

void CVecContainer::DrawDragImage(CDC* pDC,LPDRECT lpRectNew,LPDRECT lpRectLast,LPDRECT lpRectOrigianl)
{
   ASSERT(GetParent()!=NULL); 

   POSITION pos = m_Children.GetHeadPosition();
   while(pos != NULL)
   {
      CVecBase* pVec = (CVecBase*)m_Children.GetNext(pos);
      pVec->DrawDragImage(pDC,lpRectNew,lpRectLast,lpRectOrigianl);
   }
}

void CVecContainer::DrawRotatedDragImage(CDC* pDC, double dAng, LPPOINT lpCenter, double dAngLast, LPPOINT lpCenterLast)
{
   ASSERT(GetParent()!=NULL); 

   POSITION pos = m_Children.GetHeadPosition();
   while(pos != NULL)
   {
      CVecBase* pVec = (CVecBase*)m_Children.GetNext(pos);
      pVec->DrawRotatedDragImage(pDC, dAng, lpCenter, dAngLast, lpCenterLast);
   }
}

void CVecContainer::SetRect(CDRect &rect)
{
   if(GetParent()==NULL)
   {
      CVecBase::SetRect(rect); 
      return;
   }

   CDRect newRect = rect;
   CDRect oldRect = GetRect();
   //oldRect.NormalizeRect();
   //newRect.NormalizeRect();

   CDPoint OffsetOld(oldRect.left, oldRect.top);
   CDPoint OffsetNew(newRect.left, newRect.top);
   CDPoint Scale(newRect.Width() / oldRect.Width(), newRect.Height() / oldRect.Height());

   POSITION pos = m_Children.GetHeadPosition();
   while(pos != NULL)
   {
      CVecBase* pVec = (CVecBase*)m_Children.GetNext(pos);
      CDRect drect = pVec->GetRect();

      drect -= OffsetOld;
      drect *= Scale;
      drect += OffsetNew;
      
      pVec->SetRect(drect);
   }
   CVecBase::SetRect(rect); 
}


void CVecContainer::Rotate(double dAng, LPDPOINT ptCenter)
{
   ASSERT(GetParent()!=NULL); 
   CDPoint center;
   if(ptCenter!=NULL)
      center = *ptCenter;
   else
      center = GetBoundingRect().CenterPoint();

   POSITION pos = m_Children.GetHeadPosition();
   while(pos != NULL)
   {
      CVecBase* pVec = (CVecBase*)m_Children.GetNext(pos);
      pVec->Rotate(dAng, &center);
   }
   CDRect drect(GetBoundingRect());
   CVecBase::SetRect(drect);
}

void CVecContainer::Flip(BOOL bHorizontal, LPDPOINT ptCenter)
{
   ASSERT(GetParent()!=NULL); 
   CDPoint center;
   if(ptCenter!=NULL)
      center = *ptCenter;
   else
      center = GetBoundingRect().CenterPoint();

   POSITION pos = m_Children.GetHeadPosition();
   while(pos != NULL)
   {
      CVecBase* pVec = (CVecBase*)m_Children.GetNext(pos);
      pVec->Flip(bHorizontal, &center);
   }
   CDRect drect(GetBoundingRect());
   CVecBase::SetRect(drect);
}

CDRect CVecContainer::GetBoundingRect()
{
   CDRect boundingRect;
   boundingRect.SetRectEmpty();

   POSITION pos = m_Children.GetHeadPosition();
   while(pos != NULL)
   {
      CVecBase* pVec = (CVecBase*)m_Children.GetNext(pos);
      if(boundingRect.IsRectEmpty())
      {
         boundingRect = pVec->GetRect();
         boundingRect.NormalizeRect();
      }
      else
      {
         CDRect VecRect = pVec->GetRect();
         if(!VecRect.IsRectEmpty())
         {
            CDRect rect = boundingRect;
            VecRect.NormalizeRect();
            boundingRect.UnionRect(&rect, &VecRect);
         }
      }
   }
   return boundingRect;
}

CDRect CVecContainer::UpdateBoundingRect()
{
   POSITION pos = m_Children.GetHeadPosition();
   while(pos != NULL)
   {
      CVecBase* pVec = (CVecBase*)m_Children.GetNext(pos);
      CVecContainer* pContainer = dynamic_cast<CVecContainer*>(pVec);
      if(pContainer!=NULL)
         pContainer->UpdateBoundingRect();
   }

   CDRect rect = GetBoundingRect();
   CVecBase::SetRect(rect);
   return rect;
}

CVecContainer* CVecContainer::Group(CPtrList &Items) 
{
   if(Items.GetCount()<1)
   {
      ASSERT(FALSE);
      return NULL;
   }

   CVecContainer* pVecContainer = new CVecContainer;
   {//find the position of the first item in the list and insert the new continer before it.
      POSITION pos = Items.GetHeadPosition();
      ASSERT(pos!=NULL);
      CVecBase* pVec = (CVecBase*)Items.GetNext(pos);
      ASSERT(pVec!=NULL);
      POSITION posItem = m_Children.Find(pVec);
      m_Children.InsertBefore(posItem, pVecContainer);
   }

   POSITION pos = Items.GetTailPosition();
   while(pos != NULL)
   {
      CVecBase* pVec = (CVecBase*)Items.GetPrev(pos);

      pVec->SetSelected(FALSE);
      Remove(pVec);
      pVecContainer->Insert(pVec);
   }

   CDRect drect(pVecContainer->GetBoundingRect());
   pVecContainer->SetRect(drect);

   pVecContainer->SetParent(this);

   return pVecContainer;
}

UINT CVecContainer::Ungroup(CPtrList &Items) 
{
   Items.RemoveAll();
   if(GetParent()==NULL)
   {
      ASSERT(FALSE);
      return 0;
   }

   POSITION pos = GetParent()->m_Children.Find(this);
   ASSERT(pos!=NULL);

   UINT uiCount = 0;
   CPtrList MyItems;
   GetItems(MyItems);
   while(!MyItems.IsEmpty())
   {
      CVecBase* pVec = (CVecBase*)MyItems.RemoveTail();
      Remove(pVec);
      GetParent()->m_Children.InsertAfter(pos, pVec);
      pVec->SetParent(GetParent());

      Items.AddHead(pVec);
      uiCount++;
   }
   GetParent()->Remove(this);
   return uiCount;
}

void CVecContainer::SaveToBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
{
   pMemBuff->WriteNodeHeader(VEC_PARENT);
   CVecBase::SaveToBuffer(pMemBuff, iFlags);

   if((iFlags & SBF_SAVE_THIS_OB_ONLY) ==0)
   {
      POSITION pos = m_Children.GetTailPosition();
      while(pos != NULL)
      {
         CVecBase* pVec = (CVecBase*)m_Children.GetPrev(pos);
         pMemBuff->WriteNodeHeader(pVec->GetType());
         pVec->SaveToBuffer(pMemBuff, iFlags);
      }
   }
   pMemBuff->WriteEndOfFields();
}

void CVecContainer::LoadFromBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
{
   RemoveAll();
   while(pMemBuff->ReadNextFieldHeader())
   {
      int id = pMemBuff->GetFieldType();
      int iNodeId = 0;
      CVecBase* pVec = NULL;
      switch(id)
      {
      case ID_FIELD_IS_NODE: 
         iNodeId = pMemBuff->GetNodeType();
	      switch (iNodeId)
	      {
         case VEC_PARENT: 
            CVecBase::LoadFromBuffer(pMemBuff, iFlags);
            break;
         case VEC_CONTAINER:
         case VEC_RECTANGLE:
         case VEC_ELLIPSE:
         case VEC_LINE:
         case VEC_POLYLINE:
         case VEC_POLYGON:
         case VEC_POLYGON_PIC:
         case VEC_ELLIPSE_PIC:
         case VEC_RECTANGLE_PIC:
         case VEC_RECTANGLE_TEXT:
         case VEC_POLYGON_TEXT:
         case VEC_ELLIPSE_TEXT:
         case VEC_FRAME_CONTAINER:
            pVec = CVecBase::CreateNewInstanceByType(iNodeId);
            ASSERT(pVec!=NULL);
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

      if(pVec!=NULL)
      {
         pVec->LoadFromBuffer(pMemBuff, iFlags);
         Insert(pVec);
      }
   }
}

void CVecContainer::StoreToBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
{
   pMemBuff->WriteNodeHeader(VEC_PARENT);
   CVecBase::SaveToBuffer(pMemBuff, iFlags);

   if((iFlags & SBF_SAVE_THIS_OB_ONLY) ==0)
   {
      POSITION pos = m_Children.GetTailPosition();
      while(pos != NULL)
      {
         CVecBase* pVec = (CVecBase*)m_Children.GetPrev(pos);

         pMemBuff->WriteDwordField(VF_NextVecObjID, pVec->m_uiRunningID);

         pMemBuff->WriteNodeHeader(pVec->GetType());
         pVec->StoreToBuffer(pMemBuff, iFlags);
      }
   }

   pMemBuff->WriteEndOfFields();
}

void CVecContainer::RestoreFromBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
{
   UINT uiNextVecObjID = 0;
   BOOL bDontCreate = ((iFlags & SBF_DONT_CREATE_ITEMS) == SBF_DONT_CREATE_ITEMS);
   if(!bDontCreate)
      RemoveAll();
   while(pMemBuff->ReadNextFieldHeader())
   {
      int id = pMemBuff->GetFieldType();
      int iNodeId = 0;
      CVecBase* pVec = NULL;
      switch(id)
      {
      case VF_NextVecObjID:
         uiNextVecObjID = pMemBuff->ReadDword();
         break;
      case ID_FIELD_IS_NODE: 
         iNodeId = pMemBuff->GetNodeType();
	      switch (iNodeId)
	      {
         case VEC_PARENT: 
            CVecBase::LoadFromBuffer(pMemBuff, iFlags);
            break;
         case VEC_CONTAINER:
         case VEC_RECTANGLE:
         case VEC_ELLIPSE:
         case VEC_LINE:
         case VEC_POLYLINE:
         case VEC_POLYGON:
         case VEC_POLYGON_PIC:
         case VEC_ELLIPSE_PIC:
         case VEC_RECTANGLE_PIC:
         case VEC_RECTANGLE_TEXT:
         case VEC_POLYGON_TEXT:
         case VEC_ELLIPSE_TEXT:
         case VEC_FRAME_CONTAINER:
            {
               if(bDontCreate)
               {
                  POSITION pos = m_Children.GetTailPosition();
                  BOOL bFound = FALSE;
                  while(pos != NULL)
                  {
                     CVecBase* pLocalVec = (CVecBase*)m_Children.GetPrev(pos);
                     if(pLocalVec->m_uiRunningID == uiNextVecObjID)
                     {
                        ASSERT(((UINT)iNodeId) == pLocalVec->GetType());
                        pLocalVec->RestoreFromBuffer(pMemBuff, iFlags);
                        bFound = TRUE;
                        break;
                     }
                  }
                  ASSERT(bFound);
               }
               else
               {
                  pVec = CVecBase::CreateNewInstanceByType(iNodeId);
                  ASSERT(pVec!=NULL);
                  pVec->RestoreFromBuffer(pMemBuff, iFlags);
                  pVec->SetRunningID(uiNextVecObjID);
               }
            }
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

      if(pVec!=NULL)
      {
         Insert(pVec);
      }
   }
}

CVecBase* CVecContainer::GetObByRunningID(UINT uiID)
{
   if(uiID==m_uiRunningID) 
      return this;

   POSITION pos = m_Children.GetTailPosition();
   while(pos != NULL)
   {
      CVecBase* pVec = (CVecBase*)m_Children.GetPrev(pos);
      CVecBase* pRez = pVec->GetObByRunningID(uiID);
      if(pRez!=NULL)
         return pRez;
   }

   return NULL;
}


void CVecContainer::SetSelected(BOOL b)
{
   if(!m_bBlockSelectChange)
      CVecBase::SetSelected(b);
}

void CVecContainer::SetOpened(BOOL b)
{
   if(m_bBlockOpenedChange)
      return;

   m_bOpened = b;

   if(!b)
      SetSelectedForAll(FALSE);
   else 
      CVecBase::SetSelected(FALSE);

   if(!b)
      UpdateBoundingRect();

   m_bBlockSelectChange = b;
}

BOOL CVecContainer::OnDBClick(UINT nFlags, CPoint point, CUndoManager* pUndoMan)
{
   if(!IsOpened())
   {
      SetOpened(TRUE);
      return TRUE;
   }

   CDPoint p(point);
   CVecBase* pVec = HitTestObj(p);
   if(pVec!=NULL)
   {
      return pVec->OnDBClick(nFlags, point, pUndoMan);
   }

   return CVecContainer::OnDBClick(nFlags, point, pUndoMan);
}

BOOL CVecContainer::CheckIfToCloseGroups()
{
   BOOL bChanged = FALSE;
   POSITION pos = m_Children.GetHeadPosition();
   while(pos != NULL)
   {
      CVecBase* pVec = (CVecBase*)m_Children.GetNext(pos);
      CVecContainer* pContainer = dynamic_cast<CVecContainer*>(pVec);
      if(pContainer!=NULL)
         if(pContainer->CheckIfToCloseGroups())
            bChanged = TRUE;
   }

   if(GetParent()==NULL)
      return bChanged;

   if(!IsOpened())
      return bChanged;

   UINT uiCount = GetSelectedItemsCount();
   if(uiCount>0)
      return bChanged;

   SetOpened(FALSE);
   return TRUE;
}

void CVecContainer::SetLocked(BOOL b)
{
   CVecBase::SetLocked(FALSE);
   POSITION pos = m_Children.GetHeadPosition();
   while(pos != NULL)
   {
      CVecBase* pVec = (CVecBase*)m_Children.GetNext(pos);
      pVec->SetLocked(b);
   }
   CVecBase::SetLocked(b);
}

int CVecContainer::RemoveItemsThatThirParentIsInTheList(CPtrList& Items)
{
   POSITION pos = Items.GetHeadPosition();
   int iItemsRemoved = 0;
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)Items.GetNext(pos);
      CVecBase* pParent = pVec->GetParent();
      while(pParent!=NULL)
      {
         if(Items.Find(pParent)!=NULL)
         {
            POSITION posRemove = Items.Find(pVec);
            Items.RemoveAt(posRemove);
            iItemsRemoved++;
            break;
         }
         pParent = pParent->GetParent();
      }
   }
   return iItemsRemoved;
}

void CVecContainer::ArrangeItemsForTrack(CPtrList& Items)
{
   POSITION pos = Items.GetHeadPosition();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)Items.GetNext(pos);
      if(pVec->GetTracker()!=pVec)
      {
         POSITION posRemove = Items.Find(pVec);
         Items.RemoveAt(posRemove);
         pVec = pVec->GetTracker();
         if(Items.Find(pVec)==NULL)
            Items.AddTail(pVec);
      }
   }
   CVecContainer::RemoveItemsThatThirParentIsInTheList(Items);
}
