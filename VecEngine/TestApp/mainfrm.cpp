// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "TestApp.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
	m_pVecView->SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_pVecView->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
   m_pVecView = new CChildView;
   m_pVecView->Create(NULL, "Hi", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 1234);
   m_pVecView->OnInitialUpdate();

   CDRect drect(0,0,800,600);
   m_pVecView->m_VecContainer.SetRect(drect);
   m_pVecView->SetRect(drect);
   m_pVecView->SetForeColor(RGB(255,255,255));
   m_pVecView->SetBackColor(GetSysColor(COLOR_APPWORKSPACE));
	
	return 0;
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	CRect ClientRect;
   GetClientRect(ClientRect);
   m_pVecView->MoveWindow(ClientRect, true);
}

void CMainFrame::OnDestroy() 
{
	CFrameWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CMainFrame::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	
	// Do not call CFrameWnd::OnPaint() for painting messages
}

void CMainFrame::OnFileNew() 
{
	// TODO: Add your command handler code here
	
}
