// VecController.cpp: implementation of the CVecController class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VecController.h"
#include "VecContainer.h"
#include "LineTracker.h"
#include "MRectTracker.h"
#include "EllipseTracker.h"
#include "SuperTracker.h"
#include "SuperRotateTracker.h"
#include "PolylineCreatorTracker.h"
#include "Guide.h"
#include "resource.h"

#include "VecLine.h"
#include "VecRectangle.h"
#include "VecPolyline.h"
#include "VecPolygon.h"
#include "VecEllipse.h"
#include "VecRectanglePic.h"
#include "VecEllipsePic.h"
#include "VecPolygonPic.h"
#include "VecRectangleText.h"
#include "VecEllipseText.h"
#include "VecPolygonText.h"
#include "VecFrameContainer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define CONTROLLER_VALID (m_pContainer!=NULL)
#define ASSERT_CONTROLLER_VALID ASSERT(m_pContainer!=NULL);
#define RETURN_CONTROLLER_VALID if(m_pContainer==NULL) return;
#define ASSERT_RETURN_CONTROLLER_VALID ASSERT_CONTROLLER_VALID RETURN_CONTROLLER_VALID

CVecController::CVecController()
{
   m_pContainer = NULL;
   m_pCropVec = NULL;
   LoadCursors();
   SetSelectedTool(TOOL_SELECT);
   SetSelectedShapeTool(NULL);

   //guides and grid
   m_ptGridSize = CDPoint(50,50);
   m_t_ptGridSize = CDPoint(0,0);
   m_bShowGuides = TRUE;
   m_bShowGrid = FALSE;
   m_bSnapToGuides = TRUE;
   m_bSnapToGrid = TRUE;
   m_bLockGuides = FALSE;
   m_bModified = FALSE;
}


void CVecController::LoadCursors()
{
   m_hCreateCursor = AfxGetApp()->LoadCursor(IDC_CREATE_CURSOR);
   ASSERT(m_hCreateCursor);
   m_hCropCursor = AfxGetApp()->LoadCursor(IDC_CROP_CURSOR);
   ASSERT(m_hCropCursor);
  
   m_hHotRotateCursor = AfxGetApp()->LoadCursor(IDC_ROTATE);
   ASSERT(m_hHotRotateCursor!=NULL);
   m_hHotSetSelectedCursor = AfxGetApp()->LoadCursor(IDC_SET_SELECTED);
   ASSERT(m_hHotSetSelectedCursor!=NULL);

   m_hGuideHorizontal = AfxGetApp()->LoadCursor(IDC_GUIDE_HORZ);
   ASSERT(m_hGuideHorizontal!=NULL);
   m_hGuideVertical = AfxGetApp()->LoadCursor(IDC_GUIDE_VERT);
   ASSERT(m_hGuideVertical!=NULL);
}


CVecController::~CVecController()
{
   while(!m_GuidesList.IsEmpty())
   {
      CGuide* pGuide = (CGuide*)m_GuidesList.RemoveHead();
      delete pGuide;
   }
}

BOOL CVecController::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
   ASSERT_CONTROLLER_VALID;
   if(!CONTROLLER_VALID)
      return FALSE;

	CPoint point;
	GetCursorPos(&point);
	pWnd->ScreenToClient(&point);

   CRect ContainerRect = m_pContainer->GetRectByScreen();

   CRect ClientRect;
   m_pContainer->GetWnd()->GetClientRect(&ClientRect);

   CRect rect;
   rect.IntersectRect(&ContainerRect, &ClientRect);

   //increase the rect to all directions
   rect.bottom++; rect.right++; rect.top--; rect.left--;
   //if the point is outside of the rect then move the cursor and exit
   if(!rect.PtInRect(point))
   {
      m_pContainer->SetCursor(pWnd, nHitTest, VEC_TOOL_MOVE_SIZE);
      return TRUE;
   }

   //check guides
   if(m_bShowGuides && !m_bLockGuides)
   {
      POSITION pos = m_GuidesList.GetHeadPosition();
      while(pos!=NULL)
      {
         CGuide* lpGuide = (CGuide*)m_GuidesList.GetNext(pos);
         if(lpGuide->m_bIsHorizontal)
         {
            if(point.y <= ((long)lpGuide->m_t_dPos)+1 && point.y >= ((long)lpGuide->m_t_dPos)-1)
            {
               SetCursor(m_hGuideHorizontal);
               return TRUE;
            }
         }
         else
         {
            if(point.x <= ((long)lpGuide->m_t_dPos)+1 && point.x >= ((long)lpGuide->m_t_dPos)-1)
            {
               SetCursor(m_hGuideVertical);
               return TRUE;
            }
         }
      }
   }

   UINT uiSelTool = GetSelectedTool();

   if(uiSelTool>=FIRST_CREATORS_TOOLS && uiSelTool<LAST_CREATORS_TOOLS)
   {
      SetCursor(m_hCreateCursor);
      return TRUE;
   }

   if(uiSelTool>=FIRST_CROP_TOOLS && uiSelTool<LAST_CROP_TOOLS)
   {
      SetCursor(m_hCropCursor);
      return TRUE;
   }

   if(::GetKeyState(VK_CONTROL) & 0x8000)
   {
      switch(uiSelTool)
      {
      case TOOL_ROTATE:
         uiSelTool = TOOL_SELECT;
         break;
      case TOOL_DRAG_DROP:
      case TOOL_SELECT:
         uiSelTool = TOOL_ROTATE;
         break;
      default:
         ASSERT(FALSE);
        break;
      }
   }

   CDPoint dp(point);
   CVecBase* pVec = m_pContainer->HitTestObj(dp);

   if(pVec!=NULL)
   {
      if(pVec->IsSelected())
      {
         if(uiSelTool==TOOL_ROTATE)
         {
            int iHit = pVec->HitTest(dp);
            switch(iHit)
            {
            case HT_ON_POINT:
               SetCursor(m_hHotRotateCursor);
               return TRUE;
               break;
            }

         }
         pVec->SetCursor(pWnd, nHitTest, uiSelTool==TOOL_ROTATE ? VEC_TOOL_ROTATE: VEC_TOOL_MOVE_SIZE);
      }
      else
         SetCursor(m_hHotSetSelectedCursor);
   }
   else 
   {
      m_pContainer->SetCursor(pWnd, nHitTest, uiSelTool==TOOL_ROTATE ? VEC_TOOL_ROTATE: VEC_TOOL_MOVE_SIZE);
   }

   return TRUE;
}

void CVecController::OnMouseMove(UINT nFlags, CPoint point)
{
   ASSERT_RETURN_CONTROLLER_VALID;
}

void CVecController::OnMouseDown(UINT nFlags, CPoint point)
{
   ASSERT_RETURN_CONTROLLER_VALID;

   CRect ContainerRect = m_pContainer->GetRectByScreen();

   CRect ClientRect;
   m_pContainer->GetWnd()->GetClientRect(&ClientRect);

   CRect rect;
   rect.IntersectRect(&ContainerRect, &ClientRect);

   //increase the rect to all directions
   rect.bottom++; rect.right++; rect.top--; rect.left--;

   //if the point is outside of the rect then move the cursor and exit
   if(!rect.PtInRect(point))
   {
      m_pContainer->GetWnd()->ClientToScreen(&rect);
      ClipCursor(rect);
      ClipCursor(NULL);
      return;
   }

   //check guides
   if(m_bShowGuides && !m_bLockGuides)
   {
      POSITION pos = m_GuidesList.GetHeadPosition();
      while(pos!=NULL)
      {
         CGuide* lpGuide = (CGuide*)m_GuidesList.GetNext(pos);
         if(lpGuide->m_bIsHorizontal)
         {
            if(point.y <= ((long)lpGuide->m_t_dPos)+1 && point.y >= ((long)lpGuide->m_t_dPos)-1)
            {
               DPOINT points[2] = {rect.left, lpGuide->m_t_dPos, rect.right, lpGuide->m_t_dPos};
               CLineTracker tr((LPCDPOINT)points, 2, CLineTracker::styleIgnoreX);
               if(tr.Track(m_pContainer->GetWnd(), point))
               {
                  rect.bottom--; rect.right--; rect.top++; rect.left++;
                  if(points[0].y > rect.top && points[0].y < rect.bottom)
                  {
                     CDPoint Scaler = m_pContainer->GetScaler();
                     CDPoint Offset = m_pContainer->GetOffset();
                     lpGuide->m_dPos = (points[0].y - Offset.y)/Scaler.y;
                  }
                  else
                  {
                     POSITION pos = m_GuidesList.Find(lpGuide);
                     m_GuidesList.RemoveAt(pos);
                     delete lpGuide;
                  }
                  m_pContainer->Repaint();
               }
               return;
            }
         }
         else
         {
            if(point.x <= ((long)lpGuide->m_t_dPos)+1 && point.x >= ((long)lpGuide->m_t_dPos)-1)
            {
               DPOINT points[2] = {lpGuide->m_t_dPos, rect.top, lpGuide->m_t_dPos, rect.bottom};
               CLineTracker tr((LPCDPOINT)points, 2, CLineTracker::styleIgnoreY);
               if(tr.Track(m_pContainer->GetWnd(), point))
               {
                  rect.bottom--; rect.right--; rect.top++; rect.left++;
                  if(points[0].x > rect.left && points[0].x < rect.right)
                  {
                     CDPoint Scaler = m_pContainer->GetScaler();
                     CDPoint Offset = m_pContainer->GetOffset();
                     lpGuide->m_dPos = (points[0].x - Offset.x)/Scaler.x;
                  }
                  else
                  {
                     POSITION pos = m_GuidesList.Find(lpGuide);
                     m_GuidesList.RemoveAt(pos);
                     delete lpGuide;
                  }
                  m_pContainer->Repaint();
               }
               return;
            }
         }
      }
   }

   m_pContainer->GetWnd()->ClientToScreen(&rect);
   ClipCursor(rect);

   if(GetSelectedTool()>=FIRST_CREATORS_TOOLS && GetSelectedTool()<LAST_CREATORS_TOOLS)
   {
      DoCreate(nFlags, point);
      ClipCursor(NULL);
      return;
   }

   if(GetSelectedTool()>=FIRST_CROP_TOOLS && GetSelectedTool()<LAST_CROP_TOOLS)
   {
      DoCrop(nFlags, point);
      ClipCursor(NULL);
      return;
   }

   UINT uiSelTool = GetSelectedTool();

   if((nFlags & MK_CONTROL) == MK_CONTROL)
   {
      switch(uiSelTool)
      {
      case TOOL_ROTATE:
         uiSelTool = TOOL_SELECT;
         break;
      case TOOL_SELECT:
      case TOOL_DRAG_DROP:
         uiSelTool = TOOL_ROTATE;
         break;
      default:
         ASSERT(FALSE);
         break;
      }
   }

   CDPoint dp(point);

   CVecBase* pVec = m_pContainer->HitTestObj(dp);
   if(pVec!=NULL)
   {
      int iHit = pVec->HitTest(dp);
      switch(iHit)
      {
      case HT_ON_LINE:
      case HT_INSIDE:
      case HT_ON_POINT:
      case HT_ON_MAPPING_POINT:
      case HT_INSIDE_MAPPING:
         {
            BOOL bTrackResoult = FALSE;
            BOOL bRepaint = FALSE;

            if(pVec->IsSelected() || ( iHit == HT_ON_LINE || iHit == HT_INSIDE))
            {
               BOOL bWasSelected = FALSE;
               if(!pVec->IsSelected())
               {
                  if((nFlags & MK_SHIFT) != MK_SHIFT)//if the shift is not pressed
                  {
                     m_pContainer->SetSelectedForAll(FALSE);
                  }
                  pVec->SetSelected(TRUE);
                  OnSelChanged();
                  bRepaint = TRUE;
               }
               else
                  bWasSelected = TRUE;
                  
               CPtrList SelItems;
               UINT uiCount = m_pContainer->GetSelectedItems(SelItems);

               //remove locked items from the list
               POSITION pos = SelItems.GetHeadPosition();
               while(pos!=NULL)
               {
                  CVecBase* pVecRemove = (CVecBase*)SelItems.GetNext(pos);
                  if(pVecRemove->IsLocked())
                  {
                     POSITION posRemove = SelItems.Find(pVecRemove);
                     SelItems.RemoveAt(posRemove);
                     uiCount--;
                  }
               }

               if(uiCount<=1 && !(uiSelTool==TOOL_ROTATE && iHit==HT_ON_POINT))//if the control is not pressed
               {//track one item
                  if(!pVec->IsLocked())
                  {
                     if((iHit == HT_INSIDE || iHit == HT_ON_LINE) && uiSelTool == TOOL_DRAG_DROP)
                     {
                        ClipCursor(NULL);
                        if(m_pContainer->GetWnd()->SendMessage(WM_VEC_EVENT, VE_START_DRAG_DROP, MAKELPARAM(point.x,point.y)))
                           return;
                        bRepaint = TRUE;
                        bTrackResoult = FALSE;
                     }
                     else
                     {
                        bTrackResoult = pVec->GetTracker()->Track(point, uiSelTool==TOOL_ROTATE ? VEC_TOOL_ROTATE : VEC_TOOL_MOVE_SIZE , &m_UndoManager, this);
                        if(bTrackResoult)
                        {
                           bRepaint = TRUE;
                        }
                     }
                  }
               }
               else
               {//track all selected items
                  if((iHit == HT_ON_MAPPING_POINT || iHit == HT_INSIDE_MAPPING) && bWasSelected)
                  {
                     if(!pVec->IsLocked())
                     {
                        bTrackResoult = pVec->GetTracker()->Track(point, uiSelTool==TOOL_ROTATE ? VEC_TOOL_ROTATE: VEC_TOOL_MOVE_SIZE ,
                           &m_UndoManager, this);
                        if(bTrackResoult)
                        {
                           bRepaint = TRUE;
                        }
                     }
                  }
                  else if(iHit == HT_ON_POINT && uiSelTool==TOOL_ROTATE)
                  {//track rotate
                     CVecContainer::ArrangeItemsForTrack(SelItems);
                     CSuperRotateTracker st(SelItems);
                     bTrackResoult = st.Track(m_pContainer->GetWnd(), point);
                     if(bTrackResoult)
                     {
                        m_UndoManager.AddToStack(SelItems, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
                        CDPoint dcenter = CDPoint(st.m_center);
                        m_pContainer->ScreenToLocal(&dcenter);
                        while(!SelItems.IsEmpty())
                        {
                           CVecBase* pVec = ((CVecBase*)SelItems.RemoveHead());
                           pVec->Rotate(st.m_dAng, &dcenter);
                        }
                        bRepaint = TRUE;
                     }
                  }
                  else
                  {//track all selected items with move or size
                     CVecContainer::ArrangeItemsForTrack(SelItems);
                     CSuperTracker st(SelItems);
                     CSuperTracker::StyleFlags enStyle = CSuperTracker::Style_Move;
                     if(iHit == HT_ON_POINT)
                     {
                        enStyle = CSuperTracker::Style_Size;
                     }
                     if(enStyle == CSuperTracker::Style_Move && uiSelTool == TOOL_DRAG_DROP)
                     {
                        ClipCursor(NULL);
                        if(m_pContainer->GetWnd()->SendMessage(WM_VEC_EVENT, VE_START_DRAG_DROP, MAKELPARAM(point.x,point.y)))
                           return;
                        bRepaint = TRUE;
                        bTrackResoult = FALSE;
                     }
                     else
                     {
                        st.SetTrackRetainProportions();
                        bTrackResoult = st.Track(m_pContainer->GetWnd(), point, enStyle, this);
                        if(bTrackResoult)
                        {
                           m_UndoManager.AddToStack(SelItems, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
                           while(!SelItems.IsEmpty())
                           {
                              CVecBase* pVec = ((CVecBase*)SelItems.RemoveHead());
                              pVec->SetRectScaleAndOffsetByScreen(st.m_rect, st.m_rectOrigianl);
                           }
                           bRepaint = TRUE;
                        }
                     }
                  }
               }

               if(!bTrackResoult && bWasSelected)
               {
                  if((nFlags & MK_SHIFT) == MK_SHIFT)//if the shift is not pressed
                  {
                     pVec->SetSelected(FALSE);
                     OnSelChanged();
                     bRepaint = TRUE;
                  }
               }
            }
            else
            {
               if((nFlags & MK_SHIFT) != MK_SHIFT)//if the shift is not pressed
               {
                  m_pContainer->SetSelectedForAll(FALSE);
               }
               pVec->SetSelected(!pVec->IsSelected());
               OnSelChanged();
               bRepaint = TRUE;
            }
            if(bRepaint)
            {
               m_pContainer->CheckIfToCloseGroups();
               m_pContainer->Repaint();
            }
            if(bTrackResoult)
            {
               SetModified(TRUE);
            }
         }
         break;
      }
   }
   else
   {
      //remove selection
      if((nFlags & MK_SHIFT) != MK_SHIFT)//if the control is not pressed
      {
         m_pContainer->SetSelectedForAll(FALSE);
         OnSelChanged();
         //SetModified(TRUE);
         m_pContainer->Repaint();
      }

      CRect rect(point.x,point.y,point.x,point.y);
      CRectTracker rt(rect, CMRectTracker::solidLine);
      if(rt.TrackRubberBand(m_pContainer->GetWnd(), point, TRUE))
      {
         rt.m_rect.NormalizeRect();
         CDRect drect(rt.m_rect);

         CPtrList Items;
         m_pContainer->GetItems(Items, IF_RECURS_OPENED);
         while(!Items.IsEmpty())
         {
            CVecBase* pVec = ((CVecBase*)Items.RemoveHead());
            if(pVec->IsInsideRect(drect))
               pVec->SetSelected(TRUE);
         }
         OnSelChanged();
         m_pContainer->CheckIfToCloseGroups();
         //SetModified(TRUE);
         m_pContainer->Repaint();
      }
      else
         if(m_pContainer->CheckIfToCloseGroups())
            m_pContainer->Repaint();
   }

   ClipCursor(NULL);
}


void CVecController::OnRightClick(UINT nFlags, CPoint point)
{
   ASSERT_RETURN_CONTROLLER_VALID;

   if(m_uiSlectedTool>=FIRST_CROP_TOOLS && m_uiSlectedTool<LAST_CROP_TOOLS)
      return;

   CRect ContainerRect = m_pContainer->GetRectByScreen();

   CRect ClientRect;
   m_pContainer->GetWnd()->GetClientRect(&ClientRect);

   CRect rect;
   rect.IntersectRect(&ContainerRect, &ClientRect);

   //increase the rect to all directions
   rect.bottom++; rect.right++; rect.top--; rect.left--;

   //if the point is outside of the rect then move the cursor and exit
   if(!rect.PtInRect(point))
   {
      m_pContainer->GetWnd()->ClientToScreen(&rect);
      ClipCursor(rect);
      ClipCursor(NULL);
      return;
   }

   CDPoint dp(point);
   CVecBase* pVec = m_pContainer->HitTestObj(dp);
   if(pVec!=NULL)
   {
      //remove selection
      if(!pVec->IsSelected())
         if((nFlags & MK_SHIFT) != MK_SHIFT)
            m_pContainer->SetSelectedForAll(FALSE);

      pVec->SetSelected(TRUE);
      OnSelChanged();
      m_pContainer->CheckIfToCloseGroups();
      //SetModified(TRUE);
      pVec->Repaint();

   }
   else
   {
      //remove selection
      if((nFlags & MK_SHIFT) != MK_SHIFT)
      {
         m_pContainer->SetSelectedForAll(FALSE);
         OnSelChanged();
         //SetModified(TRUE);
         m_pContainer->Repaint();
      }
   }
}

void CVecController::OnMouseUp(UINT nFlags, CPoint point)
{
   ASSERT_RETURN_CONTROLLER_VALID;
}

void CVecController::OnVecCreated(CVecBase* pVec)
{
   ASSERT_RETURN_CONTROLLER_VALID;
   SetModified(TRUE);
   m_UndoManager.AddToStack(pVec, UAT_DELETE_ITEM);
   SetSelectedTool(TOOL_SELECT);
   pVec->SetSelected(TRUE);
   m_pContainer->Repaint();
   m_pContainer->GetWnd()->SendMessage(WM_VEC_EVENT, VE_VEC_CREATED, (LPARAM)pVec);
}

void CVecController::SetModified(BOOL bModified)
{
   ASSERT_RETURN_CONTROLLER_VALID;
   BOOL bChanged = m_bModified!=bModified; 

   if(bModified)
      m_pContainer->GetWnd()->SendMessage(WM_VEC_EVENT, VE_MODIFIED);

   m_bModified = bModified; 

   if(bChanged) 
      m_pContainer->GetWnd()->SendMessage(WM_VEC_EVENT, VE_MODIFIED_CHANGED);
}

void CVecController::OnSelChanged()
{
   ASSERT_RETURN_CONTROLLER_VALID;
   m_pContainer->GetWnd()->SendMessage(WM_VEC_EVENT, VE_SEL_CHANGE);
}

void CVecController::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   ASSERT_RETURN_CONTROLLER_VALID;
   CPoint point;
   CRect rect;
   GetCursorPos(&point);
   m_pContainer->GetWnd()->ScreenToClient(&point);
   m_pContainer->GetWnd()->GetClientRect(&rect);
   if(rect.PtInRect(point))
      OnSetCursor(m_pContainer->GetWnd(), HTCLIENT, NULL);
}

void CVecController::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   ASSERT_RETURN_CONTROLLER_VALID;
   CPoint point;
   CRect rect;
   GetCursorPos(&point);
   m_pContainer->GetWnd()->ScreenToClient(&point);
   m_pContainer->GetWnd()->GetClientRect(&rect);
   if(rect.PtInRect(point))
      OnSetCursor(m_pContainer->GetWnd(), HTCLIENT, NULL);
}

void CVecController::OnLButtonDblClk(UINT nFlags, CPoint point)
{
   ASSERT_RETURN_CONTROLLER_VALID;
   if(m_uiSlectedTool>=FIRST_CROP_TOOLS && m_uiSlectedTool<LAST_CROP_TOOLS)
      return;
   if(!m_pContainer->GetRectByScreen().PtInRect(CDPoint(point)))
      return;

   CDPoint dp(point);
   CVecBase* pVec = m_pContainer->HitTestObj(dp);

   if(m_uiSlectedTool == TOOL_TEXT)
   {
      if(pVec!=NULL && pVec->IsCloseShape() && !pVec->HasPicture() && !pVec->HasText())
      {
         CVecBase* pNewVec = NULL;
         switch(pVec->GetType())
         {
         case VEC_RECTANGLE:
            pNewVec = new CVecRectangleText;
            break;
         case VEC_ELLIPSE:
            pNewVec = new CVecEllipseText;
            break;
         case VEC_POLYGON:
            pNewVec = new CVecPolygonText;
            break;
         }
         if(pNewVec!=NULL)
         {
            pNewVec->Copy(pVec);
            m_UndoManager.CreateNewStep();
            m_UndoManager.AddActionToLastStep(pVec->GetParent(), UAT_RESTORE_ITEMS_ORDER);
            m_UndoManager.AddActionToLastStep(pVec, UAT_ADD_ITEM);
            pVec->GetParent()->ReplaceItem(pVec, pNewVec, TRUE);
            m_UndoManager.AddActionToLastStep(pNewVec, UAT_DELETE_ITEM);
            m_UndoManager.AddActionToLastStep(pNewVec->GetParent(), UAT_RESTORE_ITEMS_ORDER);
            pVec = NULL;
            CVecTextBase* pNewText = dynamic_cast<CVecTextBase*>(pNewVec);
            pNewVec->DoTransform();
            SetSelectedTool(TOOL_SELECT);
            pNewVec->SetSelected(TRUE);
            pNewText->GotoEditMode();
            SetModified(TRUE);
            m_pContainer->Repaint();
            return;
         }
      }
   }

   if(m_uiSlectedTool>=FIRST_CREATORS_TOOLS && m_uiSlectedTool<LAST_CREATORS_TOOLS)
      return;

   if(pVec!=NULL)
   {
      WaitForMouseUp(m_pContainer->GetWnd());
      if(pVec->OnDBClick(nFlags, point, &m_UndoManager))
      {
         SetModified(TRUE);
         pVec->Repaint();
      }

   }
}

BOOL CVecController::WaitForMouseUp(CWnd* pWnd)
{
	// don't handle if capture already set
	if (::GetCapture() != NULL)
		return FALSE;


   AfxLockTempMaps();  // protect maps while looping


	// set capture to the window which received this message
	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());
	pWnd->UpdateWindow();

	// get messages until capture lost or cancelled/accepted
	for (;;)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (CWnd::GetCapture() != pWnd)
			break;

		switch (msg.message)
		{
		// handle movement/accept messages
		case WM_LBUTTONUP:
         goto ExitLoop;
         break;
		default:
			DispatchMessage(&msg);
			break;
		}
	}
ExitLoop:

	ReleaseCapture();

	AfxUnlockTempMaps(FALSE);

   return TRUE;
}

void CVecController::DrawGrid(CDC * pDC)
{
   if(m_bShowGrid)
   {
      CRgn rgn;
      m_pContainer->GetClippingRegion(rgn);
      int iDCSaved = pDC->SaveDC();
      pDC->SelectClipRgn(&rgn, RGN_AND);
      pDC->SetROP2(R2_NOT);
   
      m_t_ptGridSize = m_ptGridSize;
      m_t_ptGridSize *= m_pContainer->GetScaler();
      CDRect ScreenRect = m_pContainer->GetRectByScreen();

      //Draw the grid
      if(m_t_ptGridSize.x>0 && m_t_ptGridSize.y>0)
      {
         CDPoint curPoint;
         curPoint.x = ScreenRect.left;
         while(curPoint.x < ScreenRect.right)
         {
            curPoint.y = ScreenRect.top;
            while(curPoint.y < ScreenRect.bottom)
            {
               pDC->SetPixelV(curPoint,RGB(0,0,0));

               curPoint.y += m_t_ptGridSize.y;
            }
            curPoint.x += m_t_ptGridSize.x;
         }
      }

      pDC->RestoreDC(iDCSaved);
   }
}

void CVecController::DrawGuides(CDC * pDC)
{
   if(m_bShowGuides)
   {
      POINT pPoints[2];
      CRgn rgn;
      int iDCSaved = pDC->SaveDC();
      m_pContainer->GetClippingRegion(rgn);
      pDC->SelectClipRgn(&rgn, RGN_AND);
   
      CPen pen(PS_SOLID, 1,RGB(0,255,255));
      CPen* oldPen = pDC->SelectObject(&pen);

      CDRect ScreenRect = m_pContainer->GetRectByScreen();

      CDPoint Scaler = m_pContainer->GetScaler();
      CDPoint Offset = m_pContainer->GetOffset();


      POSITION pos = m_GuidesList.GetHeadPosition();
      while(pos!=NULL)
      {
         CGuide* pCurGuide = (CGuide*)m_GuidesList.GetNext(pos);


         if(pCurGuide->m_bIsHorizontal)
         {
            pCurGuide->m_t_dPos = Offset.y + pCurGuide->m_dPos * Scaler.y;
         }
         else
         {
            pCurGuide->m_t_dPos = Offset.x + pCurGuide->m_dPos * Scaler.x;
         }

         if(pCurGuide->m_bIsHorizontal)
         {
            pPoints[1].y = pPoints[0].y = (long)pCurGuide->m_t_dPos;
            pPoints[0].x = (long)ScreenRect.left;
            pPoints[1].x = (long)ScreenRect.right;
            pDC->Polyline(pPoints,2);
         }
         else 
         {
            pPoints[1].x = pPoints[0].x = (long)pCurGuide->m_t_dPos;
            pPoints[0].y = (long)ScreenRect.top;
            pPoints[1].y = (long)ScreenRect.bottom;
            pDC->Polyline(pPoints,2);
         }

      }

      pDC->SelectObject(&oldPen);
      pDC->RestoreDC(iDCSaved);

   }
}

#define NO_SNAP_PRIORITY_LIMIT 10
#define SNAP_PRIORITY_NONE -1


double CVecController::FixSnapTo(LPCDPOINT lpPoint)
{
   if(::GetKeyState(VK_MENU) & 0x8000)
      return 0;

   CDRect ScreenRect = m_pContainer->GetRectByScreen();
   CDPoint CurSnapPoint = *lpPoint;
   CDPoint Point = *lpPoint;
   double dCurPriority = INT_MAX;

   //check grid
   if(m_bSnapToGrid && m_bShowGrid)
   {
      if(m_t_ptGridSize.x>0 && m_t_ptGridSize.y>0)
      {
         CDPoint curPoint;
         curPoint.x = ScreenRect.left;
         while(curPoint.x < ScreenRect.right)
         {
            curPoint.y = ScreenRect.top;
            while(curPoint.y < ScreenRect.bottom)
            {
               double dDist = ((abs(Point.x - curPoint.x) + abs(Point.y - curPoint.y))/2);//sould be pitagoras but this is faster.
               if(dDist<dCurPriority)
               {
                  dCurPriority = dDist;
                  CurSnapPoint = curPoint;
               }

               curPoint.y += m_t_ptGridSize.y;
            }
            curPoint.x += m_t_ptGridSize.x;
         }
      }
   }

   if(dCurPriority > NO_SNAP_PRIORITY_LIMIT)
      dCurPriority = NO_SNAP_PRIORITY_LIMIT;

   //check guides
   if(m_bSnapToGuides && m_bShowGuides)
   {
      double dCurXPriority = dCurPriority;
      double dCurYPriority = dCurPriority;

      POSITION pos = m_GuidesList.GetHeadPosition();
      while(pos!=NULL)
      {
         CGuide* lpGuide = (CGuide*)m_GuidesList.GetNext(pos);
         if(lpGuide->m_bIsHorizontal)
         {
            double dDist = abs(Point.y - lpGuide->m_t_dPos);
            if(dDist<dCurXPriority)
            {
               dCurXPriority = dDist;
               //CurSnapPoint.x = Point.x;
               CurSnapPoint.y = lpGuide->m_t_dPos;
            }
         }
         else 
         {
            double dDist = abs(Point.x - lpGuide->m_t_dPos);
            if(dDist<dCurYPriority)
            {
               dCurYPriority = dDist;
               //CurSnapPoint.y = Point.y;
               CurSnapPoint.x = lpGuide->m_t_dPos;
            }
         }
      }

      if(dCurXPriority > NO_SNAP_PRIORITY_LIMIT)
         dCurXPriority = INT_MAX;
      if(dCurYPriority > NO_SNAP_PRIORITY_LIMIT)
         dCurYPriority = INT_MAX;

      //iCurPriority = iCurXPriority < iCurYPriority ? iCurXPriority : iCurYPriority; //the smaller of the two
      dCurPriority = (dCurXPriority + dCurYPriority)/2;
   }


   if(dCurPriority > NO_SNAP_PRIORITY_LIMIT)
   {
      dCurPriority = SNAP_PRIORITY_NONE;
      *lpPoint = Point;
   }
   else
   {
      *lpPoint = CurSnapPoint;
   }

   return dCurPriority;
}

double CVecController::FixSnapTo(LPDRECT lpRect)
{
   double dPriority = INT_MAX;
   double dPrior;
   CDPoint ptOffset(0,0);
   CDPoint Point;

   //check top left
   Point.y = lpRect->top;
   Point.x = lpRect->left;
   dPrior = FixSnapTo(&Point);
   if(dPrior < dPriority && dPrior >=0)
   {
      dPriority = dPrior;
      ptOffset.y = Point.y - lpRect->top;
      ptOffset.x = Point.x - lpRect->left;
   }

   //check top right
   Point.y = lpRect->top;
   Point.x = lpRect->right;
   dPrior = FixSnapTo(&Point);
   if(dPrior < dPriority && dPrior >=0)
   {
      dPriority = dPrior;
      ptOffset.y = Point.y - lpRect->top;
      ptOffset.x = Point.x - lpRect->right;
   }

   //check bottom left
   Point.y = lpRect->bottom;
   Point.x = lpRect->left;
   dPrior = FixSnapTo(&Point);
   if(dPrior < dPriority && dPrior >=0)
   {
      dPriority = dPrior;
      ptOffset.y = Point.y - lpRect->bottom;
      ptOffset.x = Point.x - lpRect->left;
   }

   //check bottom right
   Point.y = lpRect->bottom;
   Point.x = lpRect->right;
   dPrior = FixSnapTo(&Point);
   if(dPrior < dPriority && dPrior >=0)
   {
      dPriority = dPrior;
      ptOffset.y = Point.y - lpRect->bottom;
      ptOffset.x = Point.x - lpRect->right;
   }

   if(dPriority>NO_SNAP_PRIORITY_LIMIT)
      dPriority = SNAP_PRIORITY_NONE;

   if(dPriority>SNAP_PRIORITY_NONE)
   {
      //OffsetRect(lpRect, ptOffset.x , ptOffset.y);
      lpRect->bottom+=ptOffset.y;
      lpRect->top+=ptOffset.y;
      lpRect->left+=ptOffset.x;
      lpRect->right+=ptOffset.x;
   }

   return dPriority;
}

double CVecController::FixSnapTo(LPCDPOINT lpPoints, UINT uiCount)
{
   double dPriority = INT_MAX;
   double dPrior;
   CDPoint ptOffset(0,0);
   CDPoint Point;

   for(UINT ui = 0; ui < uiCount; ui++)
   {
      Point = lpPoints[ui];
      dPrior = FixSnapTo(&Point);
      if(dPrior < dPriority && dPrior >=0)
      {
         dPriority = dPrior;
         ptOffset.x = Point.x - lpPoints[ui].x;
         ptOffset.y = Point.y - lpPoints[ui].y;
      }
   }

   if(dPriority>NO_SNAP_PRIORITY_LIMIT)
      dPriority = SNAP_PRIORITY_NONE;

   if(dPriority>SNAP_PRIORITY_NONE)
   {
      for(UINT ui = 0; ui < uiCount; ui++)
      {
         lpPoints[ui].x += ptOffset.x;
         lpPoints[ui].y += ptOffset.y;
      }
   }

   return dPriority;
}

int CVecController::FixSnapTo(LPPOINT lpPoint)
{
   CDPoint dpoint(*lpPoint);
   double d = FixSnapTo(&dpoint);
   *lpPoint = dpoint;
   return (int)d;
}
int CVecController::FixSnapTo(LPRECT lpRect)
{
   CDRect drect(*lpRect);
   double d = FixSnapTo(&drect);
   *lpRect= drect;
   return (int)d;
}
int CVecController::FixSnapTo(LPPOINT lpPoints, UINT uiCount)
{
   LPCDPOINT lpDPoints = new CDPOINT[uiCount];
   UINT ui;
   for(ui = 0; ui<uiCount; ui++)
      lpDPoints[ui] = lpPoints[ui];
   double d = FixSnapTo(lpDPoints, uiCount);
   for(ui = 0; ui<uiCount; ui++)
      lpPoints[ui] = lpDPoints[ui];
   delete lpDPoints;
   return (int)d;
}

void CVecController::OnCreateGuide(BOOL bHorizontal)
{
   if(!m_bShowGuides)
      return;

	CPoint point;
	GetCursorPos(&point);
	m_pContainer->GetWnd()->ScreenToClient(&point);

   CRect ContainerRect = m_pContainer->GetRectByScreen();

   CRect ClientRect;
   m_pContainer->GetWnd()->GetClientRect(&ClientRect);

   CRect rect;
   rect.IntersectRect(&ContainerRect, &ClientRect);

   //increase the rect to all directions
   rect.bottom++; rect.right++; rect.top--; rect.left--;

   HCURSOR oldCursor = GetCursor();
   if(bHorizontal)
   {
      SetCursor(m_hGuideHorizontal);
      DPOINT points[2] = {rect.left, point.y, rect.right, point.y};
      CLineTracker tr((LPCDPOINT)points, 2, CLineTracker::styleIgnoreX);
      if(tr.Track(m_pContainer->GetWnd(), point))
      {
         rect.bottom--; rect.right--; rect.top++; rect.left++;
         if(points[0].y > rect.top && points[0].y < rect.bottom)
         {
            CGuide* lpGuide = new CGuide(TRUE);
            CDPoint Scaler = m_pContainer->GetScaler();
            CDPoint Offset = m_pContainer->GetOffset();
            lpGuide->m_dPos = (points[0].y - Offset.y)/Scaler.y;
            m_GuidesList.AddHead(lpGuide);
            m_pContainer->GetWnd()->SendMessage(WM_VEC_EVENT, VE_GUIDE_CREATED);
            m_pContainer->Repaint();
         }
      }
   }
   else
   {
      SetCursor(m_hGuideVertical);
      DPOINT points[2] = {point.x, rect.top, point.x, rect.bottom};
      CLineTracker tr((LPCDPOINT)points, 2, CLineTracker::styleIgnoreY);
      if(tr.Track(m_pContainer->GetWnd(), point))
      {
         rect.bottom--; rect.right--; rect.top++; rect.left++;
         if(points[0].x > rect.left && points[0].x < rect.right)
         {
            CGuide* lpGuide = new CGuide(FALSE);
            CDPoint Scaler = m_pContainer->GetScaler();
            CDPoint Offset = m_pContainer->GetOffset();
            lpGuide->m_dPos = (points[0].x - Offset.x)/Scaler.x;
            m_GuidesList.AddHead(lpGuide);
            m_pContainer->GetWnd()->SendMessage(WM_VEC_EVENT, VE_GUIDE_CREATED);
            m_pContainer->Repaint();
         }
      }
   }
   SetCursor(oldCursor);
}

void CVecController::DoCreate(UINT nFlags, CPoint point)
{
   CDPoint dpoint(point);
   FixSnapTo(&dpoint);
   m_pContainer->SetSelectedForAll(FALSE);
   switch(GetSelectedTool())
   {
   case TOOL_RECTANGLE:
      {
         CDRect rect((int)dpoint.x,(int)dpoint.y,(int)dpoint.x,(int)dpoint.y);
         CMRectTracker rt(rect, CMRectTracker::solidLine);
         if(rt.TrackRubberBand(m_pContainer->GetWnd(), dpoint, TRUE, this) && rt.m_rect.Width()!=0 && rt.m_rect.Height()!=0)
         {
            rt.m_rect.NormalizeRect();
            CVecRectangle* pVec = new CVecRectangle;
            m_pContainer->Insert(pVec);
            pVec->DoTransform();
            pVec->SetRectByScreen(rt.m_rect);
            OnVecCreated(pVec);
         }
         break;
      }
   case TOOL_ELLIPSE:
      {
         CDRect rect((int)dpoint.x,(int)dpoint.y,(int)dpoint.x,(int)dpoint.y);
         CEllipseTracker rt(rect, CMRectTracker::solidLine);
         if(rt.TrackRubberBand(m_pContainer->GetWnd(), dpoint, TRUE, this) && rt.m_rect.Width()!=0 && rt.m_rect.Height()!=0)
         {
            rt.m_rect.NormalizeRect();
            CVecEllipse* pVec = new CVecEllipse;
            m_pContainer->Insert(pVec);
            pVec->DoTransform();
            pVec->SetRectByScreen(rt.m_rect);
            OnVecCreated(pVec);
         }
         break;
      }
   case TOOL_LINE:
      {
         DPOINT points[2] = {dpoint.x, dpoint.y, dpoint.x, dpoint.y};
         CLineTracker tr((LPCDPOINT)points, 2);
         if(tr.Track(m_pContainer->GetWnd(), dpoint, this))
         {
            CVecLine* pVec = new CVecLine;
            m_pContainer->Insert(pVec);
            pVec->DoTransform();
            CDRect drect(points[0].x,points[0].y,points[1].x,points[1].y);
            pVec->SetRectByScreen(drect);
            OnVecCreated(pVec);
         }
         break;
      }
   case TOOL_POLYLINE:
      {
         CPolylineCreatorTracker tr;
         if(tr.CreatePolyline(m_pContainer->GetWnd(), dpoint, this))
         {
            CVecPolyline* pVec = new CVecPolyline;
            m_pContainer->Insert(pVec);
            pVec->DoTransform();
            UINT uiCount = tr.GetPointsCount();
            LPCDPOINT lpPoints = new CDPOINT[uiCount];
            tr.GetPoints(lpPoints);
            pVec->SetPointsByScreen(lpPoints, uiCount);
            delete []lpPoints;
            OnVecCreated(pVec);
         }
         break;
      }
   case TOOL_FREEHAND:
      {
         CPolylineCreatorTracker tr;
         if(tr.CreateFreehand(m_pContainer->GetWnd(), point))
         {
            CVecPolyline* pVec = new CVecPolyline;
            m_pContainer->Insert(pVec);
            pVec->DoTransform();
            UINT uiCount = tr.GetPointsCount();
            LPCDPOINT lpPoints = new CDPOINT[uiCount];
            tr.GetPoints(lpPoints);
            pVec->SetPointsByScreen(lpPoints, uiCount);
            delete []lpPoints;
            OnVecCreated(pVec);
         }
         break;
      }
   case TOOL_FREEHAND_CLOSED:
      {
         CPolylineCreatorTracker tr;
         if(tr.CreateFreehand(m_pContainer->GetWnd(), point) && tr.GetPointsCount()>2)
         {
            CVecPolygon* pVec = new CVecPolygon;
            m_pContainer->Insert(pVec);
            pVec->DoTransform();
            UINT uiCount = tr.GetPointsCount();
            LPCDPOINT lpPoints = new CDPOINT[uiCount];
            tr.GetPoints(lpPoints);
            pVec->SetPointsByScreen(lpPoints, uiCount);
            delete []lpPoints;
            OnVecCreated(pVec);
         }
         break;
      }
   case TOOL_POLYGON:
      {
         CPolylineCreatorTracker tr;
         if(tr.CreatePolyline(m_pContainer->GetWnd(), dpoint, this) && tr.GetPointsCount()>2)
         {
            CVecPolygon* pVec = new CVecPolygon;
            m_pContainer->Insert(pVec);
            pVec->DoTransform();
            UINT uiCount = tr.GetPointsCount();
            LPCDPOINT lpPoints = new CDPOINT[uiCount];
            tr.GetPoints(lpPoints);
            pVec->SetPointsByScreen(lpPoints, uiCount);
            delete []lpPoints;
            OnVecCreated(pVec);
         }
         break;
      }
   case TOOL_TEXT:
      {
         CDRect rect((int)dpoint.x,(int)dpoint.y,(int)dpoint.x,(int)dpoint.y);
         CMRectTracker rt(rect, CMRectTracker::solidLine);
         if(rt.TrackRubberBand(m_pContainer->GetWnd(), dpoint, TRUE, this) && rt.m_rect.Width()!=0 && rt.m_rect.Height()!=0)
         {
            rt.m_rect.NormalizeRect();
            CVecRectangleText* pVec = new CVecRectangleText;
            m_pContainer->Insert(pVec);
            pVec->DoTransform();
            pVec->SetRectByScreen(rt.m_rect);
            pVec->DoTransform();
            SetSelectedTool(TOOL_SELECT);
            OnVecCreated(pVec);
            ClipCursor(NULL);
            pVec->GotoEditMode();
            pVec->Repaint();
         }
         break;
      }
   case TOOL_SHAPE:
      {
         if(m_pSlectedShapeTool==NULL)
            return;
         CVecPolygon* pVec = new CVecPolygon;
         pVec->SetTrackAsShape(TRUE);

         pVec->SetPoints(m_pSlectedShapeTool->aVertex, m_pSlectedShapeTool->iVertexCount);
         CDRect rect(dpoint.x-1,dpoint.y-1,dpoint.x,dpoint.y);
         pVec->SetRect(rect);
         pVec->DoTransform();

         CPtrList objList;
         objList.AddHead(pVec);
         CSuperTracker rt(objList);
         if(rt.Track(m_pContainer->GetWnd(), dpoint, CSuperTracker::Style_Size, this) && rt.m_rect.Width()!=0 && rt.m_rect.Height()!=0)
         {
            m_pContainer->Insert(pVec);
            pVec->DoTransform();
            pVec->SetRectByScreen(rt.m_rect);
            OnVecCreated(pVec);
         }
         else
         {
            delete pVec;
         }
         break;
      }
   default:
      ASSERT(FALSE);
   }
}

CDRectangle CVecController::CropCopyAndReplaceItem(CVecBase* pVec)
{
   RestoreCropHighlight();
   pVec->Copy(m_pCropVec);

   CDRectangle bitmaprect;
   bitmaprect = pVec->GetMappingRectangle();
   if(bitmaprect.IsRectangleEmpty())
      bitmaprect = m_pCropVec->GetRect();

   m_UndoManager.CreateNewStep();
   m_UndoManager.AddActionToLastStep(m_pCropVec->GetParent(), UAT_RESTORE_ITEMS_ORDER);
   m_UndoManager.AddActionToLastStep(m_pCropVec, UAT_ADD_ITEM);
   m_pCropVec->GetParent()->ReplaceItem(m_pCropVec, pVec, TRUE);
   m_UndoManager.AddActionToLastStep(pVec, UAT_DELETE_ITEM);
   m_UndoManager.AddActionToLastStep(pVec->GetParent(), UAT_RESTORE_ITEMS_ORDER);
   m_pCropVec = NULL;
   return bitmaprect;
}

void CVecController::DoCrop(UINT nFlags, CPoint point)
{
   ASSERT(m_pCropVec!=NULL);
   if(m_pCropVec==NULL)
      return;
#ifdef _DEBUG
   CPtrList items;
   m_pContainer->GetItems(items, IF_RECURS);
   ASSERT(items.Find(m_pCropVec)!=NULL);
#endif

   CDPoint dpoint(point);
   FixSnapTo(&dpoint);
   BOOL bCropped = FALSE;
   switch(GetSelectedTool())
   {
   case TOOL_CROP_RECTANGLE:
      {
         CDRect rect((int)dpoint.x,(int)dpoint.y,(int)dpoint.x,(int)dpoint.y);
         CMRectTracker rt(rect, CMRectTracker::solidLine);
         if(rt.TrackRubberBand(m_pContainer->GetWnd(), dpoint, TRUE, this) && rt.m_rect.Width()!=0 && rt.m_rect.Height()!=0)
         {
            rt.m_rect.NormalizeRect();
            CVecPolygonPic* pVec = new CVecPolygonPic;

            CDRectangle bitmaprect = CropCopyAndReplaceItem(pVec);

            CDPOINT lpPoints[4];
            lpPoints[0] = CDPoint(0,0);
            lpPoints[1] = CDPoint(0,100);
            lpPoints[2] = CDPoint(100,100);
            lpPoints[3] = CDPoint(100,0);
            pVec->SetPoints(lpPoints, 4);
            pVec->SetTrackAsShape(TRUE);

            pVec->DoTransform();
            pVec->SetRectByScreen(rt.m_rect);

            pVec->SetMappingRectangle(bitmaprect);
            bCropped = TRUE;
         }
         break;
      }
   case TOOL_CROP_ELLIPSE:
      {
         CDRect rect((int)dpoint.x,(int)dpoint.y,(int)dpoint.x,(int)dpoint.y);
         CEllipseTracker rt(rect, CMRectTracker::solidLine);
         if(rt.TrackRubberBand(m_pContainer->GetWnd(), dpoint, TRUE, this) && rt.m_rect.Width()!=0 && rt.m_rect.Height()!=0)
         {
            rt.m_rect.NormalizeRect();
            CVecEllipsePic* pVec = new CVecEllipsePic;

            CDRectangle bitmaprect = CropCopyAndReplaceItem(pVec);

            pVec->DoTransform();
            pVec->SetRectByScreen(rt.m_rect);

            pVec->SetMappingRectangle(bitmaprect);
            bCropped = TRUE;
         }
         break;
      }
   case TOOL_CROP_FREEHAND:
      {
         CPolylineCreatorTracker tr;
         if(tr.CreateFreehand(m_pContainer->GetWnd(), point) && tr.GetPointsCount()>2)
         {
            CVecPolygonPic* pVec = new CVecPolygonPic;

            CDRectangle bitmaprect = CropCopyAndReplaceItem(pVec);

            pVec->DoTransform();
            UINT uiCount = tr.GetPointsCount();
            LPCDPOINT lpPoints = new CDPOINT[uiCount];
            tr.GetPoints(lpPoints);
            pVec->SetPointsByScreen(lpPoints, uiCount);
            delete []lpPoints;

            pVec->SetMappingRectangle(bitmaprect);
            bCropped = TRUE;
         }
         break;
      }
   case TOOL_CROP_POLYGON:
      {
         CPolylineCreatorTracker tr;
         if(tr.CreatePolyline(m_pContainer->GetWnd(), dpoint, this) && tr.GetPointsCount()>2)
         {
            CVecPolygonPic* pVec = new CVecPolygonPic;

            CDRectangle bitmaprect = CropCopyAndReplaceItem(pVec);

            pVec->DoTransform();
            UINT uiCount = tr.GetPointsCount();
            LPCDPOINT lpPoints = new CDPOINT[uiCount];
            tr.GetPoints(lpPoints);
            pVec->SetPointsByScreen(lpPoints, uiCount);
            delete []lpPoints;

            pVec->SetMappingRectangle(bitmaprect);
            bCropped = TRUE;
         }
         break;
      }
   case TOOL_CROP_SHAPE:
      {
         if(m_pSlectedShapeTool==NULL)
            return;
         CVecPolygonPic* pVec = new CVecPolygonPic;
         pVec->SetTrackAsShape(TRUE);
         pVec->SetPoints(m_pSlectedShapeTool->aVertex, m_pSlectedShapeTool->iVertexCount);

         CDRect rect(dpoint.x-1,dpoint.y-1,dpoint.x,dpoint.y);
         pVec->SetRect(rect);
         pVec->DoTransform();

         CPtrList objList;
         objList.AddHead(pVec);
         CSuperTracker rt(objList);
         if(rt.Track(m_pContainer->GetWnd(), dpoint, CSuperTracker::Style_Size, this) && rt.m_rect.Width()!=0 && rt.m_rect.Height()!=0)
         {
            UINT uiCount = pVec->GetPointsCount();
            LPCDPOINT lpPoints = new CDPOINT[uiCount];
            pVec->GetPoints(lpPoints);

            CDRectangle bitmaprect = CropCopyAndReplaceItem(pVec);

            pVec->SetPoints(lpPoints,uiCount);
            delete []lpPoints;
            pVec->SetTrackAsShape(TRUE);

            pVec->DoTransform();
            pVec->SetRectByScreen(rt.m_rect);

            pVec->SetMappingRectangle(bitmaprect);
            bCropped = TRUE;
         }
         else
         {
            delete pVec;
         }
         break;
      }
   default:
      ASSERT(FALSE);
   }

   if(bCropped)
   {
      SetSelectedTool(TOOL_SELECT);
      SetModified(TRUE);
      m_pContainer->Repaint();
   }
}

void CVecController::SetSelectedTool(UINT uiTool)
{
   m_uiSlectedTool = uiTool;
   KillCropMode();
}

void CVecController::KillCropMode()
{
   if(m_uiSlectedTool>=FIRST_CROP_TOOLS && m_uiSlectedTool<LAST_CROP_TOOLS)
      m_uiSlectedTool = TOOL_SELECT;

   if(m_pCropVec!=NULL)
   {
      CPtrList items;
      m_pContainer->GetItems(items, IF_RECURS);
      if(items.Find(m_pCropVec)==NULL)
      {
         ASSERT(FALSE);
         return;
      }

      RestoreCropHighlight();
      m_pContainer->Repaint();
      m_pCropVec = NULL;
   }
}

void CVecController::RestoreCropHighlight()
{
   m_pCropVec->SetLineStyle(m_iOldLineStyle);
   m_pCropVec->SetLineWidth(m_dOldLineWidth);
   m_pCropVec->SetForeColor(m_crOldForeColor);
}

#define CROP_HILIGHT_WIDTH 8

void CVecController::SetCropTool(UINT uiTool, CVecBase* pVec)
{
   KillCropMode();
   m_uiSlectedTool = uiTool;
   m_pCropVec = pVec;

   ASSERT(m_pCropVec->HasPicture());
   if(!m_pCropVec->HasPicture())
      return;

   m_iOldLineStyle = m_pCropVec->GetLineStyle();
   m_dOldLineWidth = m_pCropVec->GetLineWidth();
   m_crOldForeColor = m_pCropVec->GetForeColor();
   m_pCropVec->SetLineStyle(PS_SOLID);
   m_pCropVec->SetLineWidth(CROP_HILIGHT_WIDTH);
   m_pCropVec->SetForeColor(RGB(255,0,0));
   m_pCropVec->Repaint();
}


