/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

#if !defined(AFX_TEXTEDITRECT_H__73CADF33_C65B_11D3_A4A0_00C0CA1261A6__INCLUDED_)
#define AFX_TEXTEDITRECT_H__73CADF33_C65B_11D3_A4A0_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextEditRect.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTextEditRect dialog
class CTextEditRect : public CDialog
{
// Construction
public:
	CTextEditRect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTextEditRect)
	enum { IDD = IDD_TEXT_EDIT_RECT };
	CEdit m_EditCtrl;
	CString	m_strText;
	//}}AFX_DATA

   CFont *m_pFont;
   CRect m_Rect;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextEditRect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:


	// Generated message map functions
	//{{AFX_MSG(CTextEditRect)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillfocusEdit1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTEDITRECT_H__73CADF33_C65B_11D3_A4A0_00C0CA1261A6__INCLUDED_)
