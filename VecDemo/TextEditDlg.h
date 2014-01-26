#if !defined(AFX_TEXTEDITDLG_H__D12E1059_A621_11D3_A489_00C0CA1261A6__INCLUDED_)
#define AFX_TEXTEDITDLG_H__D12E1059_A621_11D3_A489_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextEditDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextEditDlg dialog

class CVecDemoView;

class CTextEditDlg : public CDialog
{
// Construction
public:
	CTextEditDlg(CVecDemoView* pVecTstView);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTextEditDlg)
	enum { IDD = IDD_EDIT_TEXT };
	CEdit	m_EditCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
   CVecDemoView* m_pVecTstView;
   void Apply();

   CString m_strText;
   BOOL m_bFirstApply;

	// Generated message map functions
	//{{AFX_MSG(CTextEditDlg)
	afx_msg void OnApply();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTEDITDLG_H__D12E1059_A621_11D3_A489_00C0CA1261A6__INCLUDED_)
