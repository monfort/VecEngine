#if !defined(AFX_PAGEBACKGROUNDDLG_H__F2955E53_A6EA_11D3_A48A_00C0CA1261A6__INCLUDED_)
#define AFX_PAGEBACKGROUNDDLG_H__F2955E53_A6EA_11D3_A48A_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageBackgroundDlg.h : header file
//

class CVecDemoView;
/////////////////////////////////////////////////////////////////////////////
// CPageBackgroundDlg dialog

class CBitmapBaseEx;
class CPageBackgroundDlg : public CDialog
{
// Construction
public:
	CPageBackgroundDlg(CVecDemoView* pVecTstView);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CPageBackgroundDlg)
	enum { IDD = IDD_PAGE_BACKGROUND };
	CButton	m_ColorPickerButton;
	CButton	m_BgImageRadio;
	CEdit m_BackImageEdit;
	int		m_iBgImageRadio;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageBackgroundDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
   BOOL m_bApplyed;
   CString m_strCurrentDir;
   static CString m_strLastDir;
   CVecDemoView* m_pVecTstView;
   void EnableControls(BOOL bImage);
   void Apply();

   static BOOL m_bOldApplyForAll;

	// Generated message map functions
	//{{AFX_MSG(CPageBackgroundDlg)
	virtual void OnOK();
	afx_msg void OnApply();
	virtual BOOL OnInitDialog();
	afx_msg void OnBgimgRadio();
	afx_msg void OnSolidcolRadio();
	afx_msg void OnDestroy();
	afx_msg void OnColorButton();
	afx_msg void OnButtonFileopen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEBACKGROUNDDLG_H__F2955E53_A6EA_11D3_A48A_00C0CA1261A6__INCLUDED_)
