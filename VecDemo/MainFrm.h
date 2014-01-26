// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__A8F04FA9_95A5_11D3_A47C_00C0CA1261A6__INCLUDED_)
#define AFX_MAINFRM_H__A8F04FA9_95A5_11D3_A47C_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DOCK_BARS_REG_KEY _T("DockBars")

#include <afxtempl.h>

#define CMainFrameWindow CFrameWnd

#define WM_SET_MESSAGE  WM_USER + 1000

class CMainFrame : public CMainFrameWindow
{

protected: // create from serialization only
	DECLARE_DYNCREATE(CMainFrame)


// Attributes
public:
	CMainFrame();
	virtual ~CMainFrame();
   void LoadWorkspace();
	void OpenContextMenu(CPoint point, UINT nIDResource, int iSubMenu);
   void SetStatusBarMessage(LPCSTR lpszMessage);
   CString CMainFrame::GetStatusBarMessage();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	void SwitchPage(int iPage);
	void ChangeActiveView(CView *pView);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
   int CreateToolbar(CToolBar* pToolbar, UINT nIDResource, UINT nShowHideId, CString strName, int iRaw = 0, int iCol = 0);

   CStatusBar  m_wndStatusBar;
   CToolBar m_wndToolBar;
   CToolBar m_wndToolBox;

   CToolBar m_wndToolbarPage;
   CToolBar m_wndToolbarView;
   CToolBar m_wndToolbarObject;
   CToolBar m_wndToolbarAlign;

// customize manager
// Generated message map functions
protected:          
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
   afx_msg LRESULT OnStatusBarMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDevModeChange(LPTSTR lpDeviceName);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//}}AFX_MSG
	afx_msg void OnUpdateViewToolbars(CCmdUI* pCmdUI);
	afx_msg void OnViewToolbars(UINT nID);
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__A8F04FA9_95A5_11D3_A47C_00C0CA1261A6__INCLUDED_)
