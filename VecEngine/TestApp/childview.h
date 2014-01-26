// ChildView.h : interface of the CChildView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDVIEW_H__3147A2FD_938F_42DF_B786_175FD25FD7DD__INCLUDED_)
#define AFX_CHILDVIEW_H__3147A2FD_938F_42DF_B786_175FD25FD7DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VecView.h"

/////////////////////////////////////////////////////////////////////////////
// CChildView window

class CChildView : public CVecView
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CChildView)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnTestFreehand();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW_H__3147A2FD_938F_42DF_B786_175FD25FD7DD__INCLUDED_)
