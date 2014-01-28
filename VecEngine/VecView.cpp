/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// VecView.cpp : implementation file
//

#include "stdafx.h"
#include "VecView.h"
#include "MRectTracker.h"
#include "resource.h"
#include "memBuffEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVecView

#define CHECK_VALID_CONTROLLER_TOOL (m_VecController.GetSelectedTool() < FIRST_VIEW_TOOL || m_VecController.GetSelectedTool() >= LAST_VIEW_TOOL)


IMPLEMENT_DYNCREATE(CVecView, CScrollView)

CVecView::CVecView()
{
   m_uiZoomMode = ZM_FIT2WINDOW;
   m_dZoomFactor = 1;

   m_hZoomInCursor = AfxGetApp()->LoadCursor(IDC_ZOOM_IN);
   ASSERT(m_hZoomInCursor);
   m_hZoomOutCursor = AfxGetApp()->LoadCursor(IDC_ZOOM_OUT);
   ASSERT(m_hZoomOutCursor);
   m_hPanCursor = AfxGetApp()->LoadCursor(IDC_PAN);
   ASSERT(m_hPanCursor);
   m_hPanActCursor = AfxGetApp()->LoadCursor(IDC_PAN_ACT);
   ASSERT(m_hPanActCursor);

	m_bShowHorz=FALSE;
	m_bShowVert=FALSE;
	m_nUnit=RU_INCH;
	m_nSize=18;
   m_dVecUnitsPerInch = 100;

   m_ForeColor = RGB(255,255,255);
   m_BackColor = RGB(0,0,0);
}

CVecView::~CVecView()
{
}


BEGIN_MESSAGE_MAP(CVecView, CScrollView)
	//{{AFX_MSG_MAP(CVecView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
   ON_MESSAGE(WM_RULER_NCLBUTTONDOWN, OnRullerMouseDown)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVecView drawing

void CVecView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

   m_VecController.SetContainer(&m_VecContainer);
   m_VecContainer.SetWnd(this);
   m_VecContainer.SetWorkspaceOb(&m_VecWorkspace);
   m_VecWorkspace.m_pVecContainer = &m_VecContainer;


	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

   m_uiClipboardFormat = RegisterClipboardFormat(VEC_CLIPBOARD_FORMAT);

}

/////////////////////////////////////////////////////////////////////////////
// CVecView diagnostics

#ifdef _DEBUG
void CVecView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CVecView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVecView message handlers
void CVecView::SetZoomMode(UINT uiZoomMode)
{
   m_uiZoomMode = uiZoomMode;

   //call it twice for a correct calculation
   UpdateZoomAndScroll();
   UpdateZoomAndScroll();
   m_VecContainer.Repaint();
   UpdateRulerBars();
}
void CVecView::OnSize(UINT nType, int cx, int cy) 
{
   CScrollView::OnSize(nType, cx, cy);

   //call it twice for a correct calculation
   CDRect ContainerRect = GetRect();
   if(cx>0 && cy>0 && !ContainerRect.IsRectNull())
   {
      UpdateZoomAndScroll();
      UpdateZoomAndScroll();
      UpdateRulerBars();
   }
}

void CVecView::UpdateZoomAndScroll()
{
   CDRect ContainerRect = GetRect();
   CDPoint Scaler;
   CDPoint Offset;

   if(m_uiZoomMode == ZM_FIT2WINDOW || m_uiZoomMode == ZM_STRECH2WINDOW)
   {
      //shuld disalbe the scrollbars in order to get the real size of the client area
      SetScrollSizes(MM_TEXT, CSize(0,0), CSize(0,0), CSize(0,0));
   }

   CRect rect;
   GetClientRect(&rect);
   int cx = rect.Width();
   int cy = rect.Height();

   switch(m_uiZoomMode)
   {
   case ZM_FIT2WINDOW:
      {
         Scaler.x = cx / ContainerRect.Width();
         Scaler.y = cy / ContainerRect.Height();
         if(Scaler.x < Scaler.y)
            Scaler.y = Scaler.x;
         else
            Scaler.x = Scaler.y;

         Offset.x = (cx - ContainerRect.Width() * Scaler.x) / 2;
         Offset.y = (cy - ContainerRect.Height() * Scaler.y) / 2;
      }
      break;
   case ZM_FIT2WIDTH:
      {
         Scaler.x = cx / ContainerRect.Width();
         Scaler.y = cy / ContainerRect.Height();
         Scaler.y = Scaler.x;

         Offset.x = (cx - ContainerRect.Width() * Scaler.x) / 2;
         Offset.y = (cy - ContainerRect.Height() * Scaler.y) / 2;
      }
      break;
   case ZM_FIT2HEIGHT:
      {
         Scaler.x = cx / ContainerRect.Width();
         Scaler.y = cy / ContainerRect.Height();
         Scaler.x = Scaler.y;

         Offset.x = (cx - ContainerRect.Width() * Scaler.x) / 2;
         Offset.y = (cy - ContainerRect.Height() * Scaler.y) / 2;
      }
      break;
   case ZM_STRECH2WINDOW:
      {
         Scaler.x = cx / ContainerRect.Width();
         Scaler.y = cy / ContainerRect.Height();

         Offset.x = 0;
         Offset.y = 0;
      }
      break;
   case ZM_NORMAL:
      {
         Scaler.x = 1;
         Scaler.y = 1;

         Offset.x = (cx - ContainerRect.Width() * Scaler.x) / 2;
         Offset.y = (cy - ContainerRect.Height() * Scaler.y) / 2;
      }
      break;
   case ZM_FACTOR:
      {
         Scaler.x = m_dZoomFactor;
         Scaler.y = m_dZoomFactor;

         Offset.x = (cx - ContainerRect.Width() * Scaler.x) / 2;
         Offset.y = (cy - ContainerRect.Height() * Scaler.y) / 2;
      }
      break;
   }

   //manage scroll bars.
   if(m_uiZoomMode != ZM_FIT2WINDOW && m_uiZoomMode != ZM_STRECH2WINDOW)
   {
      if(Offset.x<0)
         Offset.x = 0;
      if(Offset.y<0)
         Offset.y = 0;

      CSize sizeTotal((int)(ContainerRect.Width() * Scaler.x), (int)(ContainerRect.Height() * Scaler.y));
      CSize sizePage(cx, cy);
      CSize sizeLine(10,10);

      SetScrollSizes(MM_TEXT, sizeTotal, sizePage, sizeLine);

      Offset -= CDPoint(GetScrollPosition());
   }

   if(Scaler.x > Scaler.y)
      m_dZoomFactor = Scaler.x;
   else
      m_dZoomFactor = Scaler.y;

   SetScaler(Scaler);
   SetOffset(Offset);
}

void CVecView::SetZoomFactor(double dZoomFactor)
{
   m_dZoomFactor = dZoomFactor;
   SetZoomMode(ZM_FACTOR);
}

void CVecView::SetContainerOffsetBeforeDraw()
{
   m_VecContainer.SetScaler(m_Scaler);
   m_VecContainer.SetOffset(m_Offset);
}

void CVecView::OnDraw(CDC* pDC)
{
   CRect Rect;
   pDC->GetClipBox(&Rect);
   if(Rect.IsRectEmpty())
      GetClientRect(&Rect);

   //cancel the staff that the scroll view is doing to us
   VERIFY(pDC->SaveDC() != 0);
	pDC->SetMapMode(MM_TEXT);
	pDC->SetViewportOrg(0, 0);
	pDC->SetWindowOrg(0, 0);

   CBitmap		*pOldBitmap;

   CDC offDC;
	// prepare memory dc for offscreen operatrions
	CBitmap offBitmap;
	offDC.CreateCompatibleDC(pDC);
   offBitmap.CreateCompatibleBitmap(pDC, Rect.right, Rect.bottom);
   pOldBitmap = offDC.SelectObject(&offBitmap);
   CRgn region;
   region.CreateRectRgn(Rect.left, Rect.top, Rect.right, Rect.bottom);
   offDC.SelectClipRgn(&region, RGN_AND);


   SetContainerOffsetBeforeDraw();
   m_VecContainer.DoTransform();
   DrawBackground(&offDC);
   m_VecController.DrawGrid(&offDC);
   m_VecContainer.Draw(&offDC);
   m_VecController.DrawGuides(&offDC);

   pDC->BitBlt(Rect.left, Rect.top, Rect.right, Rect.bottom, &offDC, Rect.left, Rect.top, SRCCOPY);

   offDC.SelectObject(pOldBitmap);
	VERIFY(pDC->RestoreDC(-1));

}

void CVecView::DrawBackground(CDC* pDC)
{
   RECT Rect;
   GetClientRect(&Rect);
   //clear the backgroung color
   pDC->FillSolidRect(&Rect, m_BackColor);
   CRect containerRect = GetRectByScreen();
   pDC->FillSolidRect(&containerRect, m_ForeColor);
}

void CVecView::OnLButtonDown(UINT nFlags, CPoint point) 
{
   if(m_VecController.GetSelectedTool() == TOOL_PAN || 
      (m_VecController.GetSelectedTool() == TOOL_ZOOM && ::GetKeyState(VK_CONTROL) & 0x8000))
   {
      TrackPan(point);
   }
   else if(m_VecController.GetSelectedTool() == TOOL_ZOOM)
   {
      CDRect rect(point.x,point.y,point.x,point.y);
      CMRectTracker rt(rect, CMRectTracker::solidLine);
      if(rt.TrackRubberBand(this, point, TRUE) && abs(rt.m_rect.Width())>5 && abs(rt.m_rect.Height())>5)
      {
         rt.m_rect.NormalizeRect();
         CDPoint Scaler;
         CDRect ZoomToRect(rt.m_rect);
         CDRect ClientRect = GetRectByScreen();;
         

         Scaler = ClientRect.Size();
         Scaler /= ZoomToRect.Size();

         double dFactor;
         if(Scaler.x > Scaler.y)
            dFactor = Scaler.y;
         else
            dFactor = Scaler.x;

   
         if((nFlags & MK_SHIFT) != MK_SHIFT)
            dFactor = dFactor;//zoom out
         else
            dFactor = 1/dFactor;//zoom out


         CDPoint pos = rt.m_rect.TopLeft();
         pos -= GetOffset(); 
         pos /= GetScaler(); 
         pos.x *= dFactor;
         pos.y *= dFactor;

         //this was a call to SetZoomFactor(dFactor); but i inserted the code here in order to call 
         //ScrollToPositionWithoutScrollWindow in the middle of the opperation
         {
            m_dZoomFactor = dFactor;

            m_uiZoomMode = ZM_FACTOR;

            //call it twice for a correct calculation
            UpdateZoomAndScroll();

            ScrollToPositionWithoutScrollWindow(pos);

            UpdateZoomAndScroll();
            m_VecContainer.Repaint();
            UpdateRulerBars();
         }
      }
      else
      {
         if((nFlags & MK_SHIFT) != MK_SHIFT)
            SetZoomFactor(GetZoomFactor()*1.3);//zoom out
         else
            SetZoomFactor(GetZoomFactor()/1.3);//zoom out
      }
   }
   else
      if(CHECK_VALID_CONTROLLER_TOOL)
         m_VecController.OnMouseDown(nFlags, point);
	
	CScrollView::OnLButtonDown(nFlags, point);
}

void CVecView::OnLButtonUp(UINT nFlags, CPoint point) 
{
   if(CHECK_VALID_CONTROLLER_TOOL)
      m_VecController.OnMouseUp(nFlags, point);
	
	CScrollView::OnLButtonUp(nFlags, point);
}

void CVecView::OnRButtonDown(UINT nFlags, CPoint point) 
{
   if(CHECK_VALID_CONTROLLER_TOOL)
      m_VecController.OnRightClick(nFlags, point);
	
	CScrollView::OnRButtonDown(nFlags, point);
}

void CVecView::OnMouseMove(UINT nFlags, CPoint point) 
{
   if(CHECK_VALID_CONTROLLER_TOOL)
      m_VecController.OnMouseMove(nFlags, point);
	
	CScrollView::OnMouseMove(nFlags, point);
}


BOOL CVecView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
   CRect rect;
   POINT point;
   ::GetCursorPos(&point);
   ScreenToClient(&point);
   GetClientRect(&rect);

   //check it the cursor is inside the client area
   if(!rect.PtInRect(point))
	   return CScrollView::OnSetCursor(pWnd, nHitTest, message);

   if(m_VecController.GetSelectedTool() == TOOL_ZOOM)
   {
      if(::GetKeyState(VK_CONTROL) & 0x8000)
         ::SetCursor(m_hPanCursor);
      else
      {
         if(::GetKeyState(VK_SHIFT) & 0x8000)
            ::SetCursor(m_hZoomOutCursor);
         else
            ::SetCursor(m_hZoomInCursor);
      }
      return TRUE;
   }
   else if(m_VecController.GetSelectedTool() == TOOL_PAN)
   {
      ::SetCursor(m_hPanCursor);
      return TRUE;
   }
   else
      if(CHECK_VALID_CONTROLLER_TOOL)
         return m_VecController.OnSetCursor(pWnd, nHitTest, message);
   
   return FALSE;
}

BOOL CVecView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return TRUE;//CScrollView::OnEraseBkgnd(pDC);
}


void CVecView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
   CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);

   nPos = GetScrollPosition().y;
   CDPoint Offset = GetOffset();
   Offset.y = -((double)nPos);
   SetOffset(Offset);
   m_VecContainer.Repaint();
   m_ruler.CheckScrollPos();
}

void CVecView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
   CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);

   nPos = GetScrollPosition().x;
   CDPoint Offset = GetOffset();
   Offset.x = -((double)nPos);
   SetOffset(Offset);
   m_VecContainer.Repaint();
   m_ruler.CheckScrollPos();
}

void CVecView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   if(m_VecController.GetSelectedTool() == TOOL_ZOOM)
   {
      CPoint point;
      GetCursorPos(&point);
      ScreenToClient(&point);
      CRect rect;
      GetClientRect(&rect);
      if(rect.PtInRect(point))
      {
         if(::GetKeyState(VK_CONTROL) & 0x8000)
            ::SetCursor(m_hPanCursor);
         else
         {
            if(::GetKeyState(VK_SHIFT) & 0x8000)
               ::SetCursor(m_hZoomOutCursor);
            else
               ::SetCursor(m_hZoomInCursor);
         }
      }
   }
   else
      if(CHECK_VALID_CONTROLLER_TOOL)
         m_VecController.OnKeyDown(nChar, nRepCnt, nFlags);
	
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CVecView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   if(m_VecController.GetSelectedTool() == TOOL_ZOOM)
   {
      CPoint point;
      GetCursorPos(&point);
      ScreenToClient(&point);
      CRect rect;
      GetClientRect(&rect);
      if(rect.PtInRect(point))
      {
         if(::GetKeyState(VK_CONTROL) & 0x8000)
            ::SetCursor(m_hPanCursor);
         else
         {
            if(::GetKeyState(VK_SHIFT) & 0x8000)
               ::SetCursor(m_hZoomOutCursor);
            else
               ::SetCursor(m_hZoomInCursor);
         }
      }
   }
   else if(m_VecController.GetSelectedTool() == TOOL_PAN)
   {
   }
   else
      if(CHECK_VALID_CONTROLLER_TOOL)
         m_VecController.OnKeyUp(nChar, nRepCnt, nFlags);
	
	CScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CVecView::TrackPan(CPoint point)
{
   ::SetCursor(m_hPanActCursor);
	// don't handle if capture already set
	if (::GetCapture() != NULL)
		return;

   AfxLockTempMaps();  // protect maps while looping

	// set capture to the window which received this message
	SetCapture();
	ASSERT(this == CWnd::GetCapture());
	UpdateWindow();

   CPoint OriginalPoint(point);
   CPoint LastPoint(point);
   //CPoint OriginalPos(GetScrollPosition());


   BOOL bNeedHScroll = FALSE;
   BOOL bNeedVScroll = FALSE;
   CPoint TotalSize = GetTotalSize();
   CRect ClientRect;
   GetClientRect(&ClientRect);
   if(ClientRect.Width()<TotalSize.x)
      bNeedHScroll = TRUE;
   if(ClientRect.Height()<TotalSize.y)
      bNeedVScroll = TRUE;


	// get messages until capture lost or cancelled/accepted
	for (;;)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (CWnd::GetCapture() != this)
			break;

		switch (msg.message)
		{
		// handle movement/accept messages
		case WM_MOUSEMOVE:
         {
            CPoint p;
            ::GetCursorPos(&p);
            ScreenToClient(&p);

			   // only redraw and callback if the rect actually changed!
			   if (LastPoint != p)
			   {
               CPoint pos = GetScrollPosition();
               CPoint diff(p-LastPoint);
               if(bNeedHScroll)
                  pos.x-=diff.x;
               if(bNeedVScroll)
                  pos.y-=diff.y;
               ScrollToPosition(pos);
               LastPoint = p;

               pos = GetScrollPosition();
               CDPoint Offset = GetOffset();
               CDPoint OldOffset(Offset);
               if(bNeedHScroll)
                  Offset.x = -pos.x;
               if(bNeedVScroll)
                  Offset.y = -pos.y;

               if(Offset != OldOffset)//repaint only if needed
               {
                  SetOffset(Offset);
                  m_VecContainer.Repaint();
                  m_ruler.CheckScrollPos();
               }
			   }
         }
			break;

		// handle cancel messages
		case WM_KEYUP:
		case WM_KEYDOWN:
			if (msg.wParam != VK_ESCAPE)
				break;
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
			goto ExitLoop;

		// just dispatch rest of the messages
		default:
			DispatchMessage(&msg);
			break;
		}
	}

ExitLoop:
	ReleaseCapture();

	AfxUnlockTempMaps(FALSE);
}

//this function is a copy of the functions : CScrollView::ScrollToDevicePosition and CScrollView::ScrollToPosition
//that was cheanged not to call ScrollWindow function 
void CVecView::ScrollToPositionWithoutScrollWindow(POINT pt)    // logical coordinates
{
	ASSERT(m_nMapMode > 0);     // not allowed for shrink to fit
	if (m_nMapMode != MM_TEXT)
	{
		CWindowDC dc(NULL);
		dc.SetMapMode(m_nMapMode);
		dc.LPtoDP((LPPOINT)&pt);
	}

	// now in device coordinates - limit if out of range
	int xMax = GetScrollLimit(SB_HORZ);
	int yMax = GetScrollLimit(SB_VERT);
	if (pt.x < 0)
		pt.x = 0;
	else if (pt.x > xMax)
		pt.x = xMax;
	if (pt.y < 0)
		pt.y = 0;
	else if (pt.y > yMax)
		pt.y = yMax;

	ASSERT(pt.x >= 0);
	ASSERT(pt.y >= 0);
	SetScrollPos(SB_HORZ, pt.x);
	SetScrollPos(SB_VERT, pt.y);
}


void CVecView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
   if(CHECK_VALID_CONTROLLER_TOOL)
      m_VecController.OnLButtonDblClk(nFlags, point);
	
	CScrollView::OnLButtonDblClk(nFlags, point);
}

int CVecView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	VERIFY(m_ruler.Attach(this,TRUE,TRUE));

	UpdateRulerBars();
	
	return 0;
}

void CVecView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	UpdateRulerBars();
	
}

void CVecView::UpdateRulerBars()
{
	ASSERT(m_ruler.IsAttached());

	m_ruler.SetShowHorzRulerBar(m_bShowHorz);
	m_ruler.SetShowVertRulerBar(m_bShowVert);

   if(!m_bShowHorz && !m_bShowHorz)
      return;

	m_ruler.SetVertRulerBarWidth(m_nSize);
	m_ruler.SetHorzRulerBarHeight(m_nSize);

   int iUnits = 0;
	switch(m_nUnit)
	{
	case RU_INCH:
		{
         iUnits = 100;
			break;
		}
	case RU_CM:
		{
         iUnits = 254;
			break;
		}
	case RU_MM:
		{
         iUnits = 2540;
			break;
		}
	default:
		ASSERT(FALSE);
	}
   double dUnitsX = ((iUnits*m_dVecUnitsPerInch)/100.0);
   double dUnitsY = ((iUnits*m_dVecUnitsPerInch)/100.0);
	m_ruler.CalibrateRuler((UINT)dUnitsX,TRUE,FALSE);
	m_ruler.CalibrateRuler((UINT)dUnitsY,FALSE,FALSE);

   CDPoint Scaler = GetScaler();
   CDPoint Offset = GetOffset();
	m_ruler.ZoomRuler(Scaler.x,TRUE,FALSE);
	m_ruler.ZoomRuler(Scaler.y,FALSE,FALSE);
   m_ruler.OffsetRuler(Offset.x>0 ? Offset.x : 0,TRUE,FALSE);
   m_ruler.OffsetRuler(Offset.y>0 ? Offset.y : 0,FALSE,FALSE);
   m_ruler.CheckScrollPos(FALSE);
	m_ruler.RedrawAttached();
}

LRESULT CVecView::OnRullerMouseDown(WPARAM wParam, LPARAM lParam)
{
   switch(wParam)
   {
   case RULLER_HORIZONTAL:
      m_VecController.OnCreateGuide(TRUE);
      break;
   case RULLER_VERTICAL:
      m_VecController.OnCreateGuide(FALSE);
      break;
   case RULLER_BLANK:
      break;
   }
   return 0;
}



void CVecView::OnKillFocus(CWnd* pNewWnd) 
{
	CScrollView::OnKillFocus(pNewWnd);
	
   m_VecController.KillCropMode();
}


HGLOBAL CVecView::CreateArtObject(CPtrList &Items)
{
   if(Items.GetCount()==0)
   {
      return NULL;
   }
   
   //copy the items to a new container
   CVecContainer VecContainer;
   CPtrList MyItems;
   MyItems.AddTail(&Items);
   while(!MyItems.IsEmpty())
   {
      CVecBase *pVecItem = (CVecBase*)MyItems.RemoveTail();
      CVecBase *pNewItem = CVecBase::CreateNewInstanceByType(pVecItem->GetType());
      pNewItem->Copy(pVecItem);
      pNewItem->SetSelected(FALSE);
      VecContainer.Insert(pNewItem);
   }
   CDRect drect(VecContainer.GetBoundingRect());
   VecContainer.GetItems(MyItems);
   while(!MyItems.IsEmpty())
   {
      CVecBase *pVecItem = (CVecBase*)MyItems.RemoveHead();
      CDRect vecRect = pVecItem->GetRect();
      vecRect.OffsetRect(-drect.TopLeft());
      pVecItem->SetRect(vecRect);
   }
   drect = VecContainer.GetBoundingRect();
   VecContainer.SetRect(drect);

   //dump to a buffer
   HGLOBAL hGlobal;
   CNodedMemoryBufferEx ContainerDumper;
   VecContainer.SaveToBuffer(&ContainerDumper);
   DWORD dwSize = ContainerDumper.CopyData(NULL,0);
   hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwSize);
   ContainerDumper.CopyData(GlobalLock(hGlobal), dwSize);
   GlobalUnlock(hGlobal);
   return hGlobal;
}

CDRect CVecView::ReadArtObject(HGLOBAL hBuffer, CPtrList& Items)
{
   CVecContainer Container;
   unsigned long uMemSize = GlobalSize(hBuffer);
   
   //load the vector entities from the buffer
   CNodedMemoryBufferEx ContainerDumper(GlobalLock(hBuffer), uMemSize);
   Container.LoadFromBuffer(&ContainerDumper);

   CDRect bbox(Container.GetRect());
   // Clean up 
   GlobalUnlock(hBuffer);

   Container.GetItems(Items);
   POSITION pos = Items.GetHeadPosition();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)Items.GetNext(pos);
      pVec->Remove();
   }
   return bbox;
}

COleDataSource* CVecView::BuildOleDataSource(CPtrList &Items)
{
   if(Items.IsEmpty())
      return NULL;

   HGLOBAL hGlobal;
   hGlobal = CreateArtObject(Items);
   ASSERT(hGlobal!=NULL);
   if(hGlobal==NULL)
      return NULL;

   //Build Ole Data Source
   COleDataSource* pSource = new COleDataSource();
   ASSERT(pSource);
   if(!pSource)
   {
      GlobalFree(hGlobal);
      return NULL;
   }

   // Cache the data, and initiate DragDrop
   pSource->CacheGlobalData((WORD)m_uiClipboardFormat, hGlobal);  
   return pSource;
  
}

