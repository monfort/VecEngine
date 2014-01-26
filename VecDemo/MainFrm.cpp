// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "VecDemo.h"
#include "MainFrm.h"
#include "VecDemoDoc.h"
#include "VecDemoView.h"
#include "afxpriv.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CMainFrameWindow)

BEGIN_MESSAGE_MAP(CMainFrame, CMainFrameWindow)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()       
	ON_WM_CLOSE()
	ON_WM_CONTEXTMENU()
    ON_MESSAGE(WM_SET_MESSAGE, OnStatusBarMessage)
	ON_WM_DEVMODECHANGE()
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
	// standard on/off control bar handlers for View menu. 
    ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_TOOLBARS_STANDARD, ID_VIEW_TOOLBARS_TOOLBOX, OnUpdateViewToolbars)
    ON_COMMAND_RANGE(ID_VIEW_TOOLBARS_STANDARD, ID_VIEW_TOOLBARS_TOOLBOX, OnViewToolbars)
END_MESSAGE_MAP()

#define TOOLBAR_SIZE_X 16
#define TOOLBAR_SIZE_Y 16

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMainFrameWindow::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

   EnableDocking(CBRS_ALIGN_ANY);

   if(CreateToolbar(&m_wndToolBar,        IDR_MAINFRAME,       ID_VIEW_TOOLBARS_STANDARD, _T("Standard"), 0,0) != 0)
      return -1;
   if(CreateToolbar(&m_wndToolbarPage,    IDR_TOOLBAR_PAGE,    ID_VIEW_TOOLBARS_PAGE    , _T("Page"), 0,1) != 0)
      return -1;
   if(CreateToolbar(&m_wndToolbarObject,  IDR_TOOLBAR_OBJECT,  ID_VIEW_TOOLBARS_OBJECT  , _T("Object"), 2,0) != 0)
      return -1;
   if(CreateToolbar(&m_wndToolbarAlign,  IDR_TOOLBAR_ALIGN,  ID_VIEW_TOOLBARS_ALIGN  , _T("Align"), 3,0) != 0)
      return -1;
   
   //hide some of the toolbars
   //m_wndToolbarObject.ShowWindow(SW_HIDE);
   //m_wndToolbarAlign.ShowWindow(SW_HIDE);

   //create the toolbox
   if (!m_wndToolBox.Create(this,
		WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC,
		ID_VIEW_TOOLBARS_TOOLBOX) || !m_wndToolBox.LoadToolBar(IDR_TOOLBOX))
	{
		TRACE0("Failed to create toolbox\n");
		return -1;      // fail to create
	}
	m_wndToolBox.SetWindowText(_T("Toolbox"));
	m_wndToolBox.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBox, AFX_IDW_DOCKBAR_RIGHT);

   AfxGetApp()->m_pMainWnd = this;
   return 0;
}

int CMainFrame::CreateToolbar(CToolBar* pToolbar, UINT nIDResource, UINT nShowHideId, CString strName, int iRaw, int iCol)
{
   if (!pToolbar->Create(this,
		WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC,
		nShowHideId) || !pToolbar->LoadToolBar(nIDResource))
	{
		TRACE1("Failed to create %s toolbar\n",strName);
		return -1;      // fail to create
	}

   pToolbar->SetWindowText(strName);
	pToolbar->EnableDocking(CBRS_ALIGN_ANY);

   RECT rect;
   rect.top = iRaw * TOOLBAR_SIZE_Y; 
   rect.left = iCol * TOOLBAR_SIZE_X;
   rect.bottom = rect.top + TOOLBAR_SIZE_Y; 
   rect.right = rect.left + TOOLBAR_SIZE_X;

   ClientToScreen(&rect);

   DockControlBar(pToolbar, AFX_IDW_DOCKBAR_TOP, &rect);
   return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMainFrameWindow::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMainFrameWindow::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMainFrameWindow::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnClose() 
{
	CMainFrameWindow::OnClose(); 
}

void CMainFrame::LoadWorkspace() 
{
	AfxGetApp()->m_nCmdShow = SW_SHOWMAXIMIZED; //bypass loading

   RecalcLayout();
}


void CMainFrame::OpenContextMenu(CPoint point, UINT nIDResource, int iSubMenu)
{
	CMenu menu;
	VERIFY(menu.LoadMenu(nIDResource));

   CMenu* pMenu = &menu;
   pMenu = menu.GetSubMenu(iSubMenu);

	pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,
		point.x,point.y, this);
}


void CMainFrame::OnViewToolbars(UINT nID) 
{
	CControlBar* pBar = GetControlBar(nID);
	if (pBar != NULL)
	{
		ShowControlBar(pBar, (pBar->GetStyle() & WS_VISIBLE) == 0, FALSE);
	}
}

void CMainFrame::OnUpdateViewToolbars(CCmdUI* pCmdUI) 
{
	CControlBar* pBar=GetControlBar(pCmdUI->m_nID);
	if(pBar!=NULL)
	{
		pCmdUI->SetCheck((pBar->GetStyle() & WS_VISIBLE) != 0);
		return;
	}
}


void CMainFrame::SetStatusBarMessage(LPCSTR lpszMessage)
{
   m_wndStatusBar.SetWindowText(lpszMessage);
}

LRESULT CMainFrame::OnStatusBarMessage(WPARAM wParam, LPARAM /*lParam*/)
{
  LPCSTR lpszMessage =  (LPCSTR)wParam;
  m_wndStatusBar.SetWindowText(lpszMessage);
  return 0;
} 

CString CMainFrame::GetStatusBarMessage()
{
   CString strMessage;
   if(::IsWindow(m_wndStatusBar.m_hWnd))
   {
      m_wndStatusBar.GetWindowText(strMessage);
   }
   return strMessage;
}

void CMainFrame::ChangeActiveView(CView *pView)
{
   //***********************************************************
   // BE AWARE !!!!! the function doesn't call OnActivateView(...)
   //
   // IF You want to do it in the MFC way call SetActiveView 
   // This func is whan the view is not Child of the CMainFrame
   // NoamNoam
   //*******************************************************
   
	ASSERT_KINDOF(CView, pView);
   if(IsChild(pView))
      SetActiveView(pView);
   else
      m_pViewActive = pView;
}

void CMainFrame::OnDevModeChange(LPTSTR lpDeviceName) 
{
	CMainFrameWindow::OnDevModeChange(lpDeviceName);
	
	// TODO: Add your message handler code here
	
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CMainFrameWindow::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
	
}

