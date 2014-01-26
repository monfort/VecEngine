#if !defined(AFX_NewPagePropDlg_H__6E1DFB63_AD36_11D3_A48F_00C0CA1261A6__INCLUDED_)
#define AFX_NewPagePropDlg_H__6E1DFB63_AD36_11D3_A48F_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewPagePropDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewPagePropDlg dialog

class CNewPagePropDlg : public CDialog
{
// Construction
public:
	BOOL m_bReadOnly;
	CNewPagePropDlg(CString const & strTitle);
	CNewPagePropDlg();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewPagePropDlg)
	enum { IDD = IDD_PAGE_TYPE };

	int		m_iPageSize;
	CStatic	m_Preview;
	CComboBox	m_cbPageSize;
	int		m_iLandscapeRadio;

	//}}AFX_DATA

   double m_dPageWidth, m_dPageHeight;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewPagePropDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
   //old values
	static BOOL m_bIsOldValuesAvilable;
	static int m_iOldLandscapeRadio;
	static int m_iOldRollerSide;
	static BOOL m_bOld2Sides;
	static int m_iOldPageSize;

   
   CBitmap m_PreviewBitmap;
   void UpdatePreviewBitmap();

	// Generated message map functions
	//{{AFX_MSG(CNewPagePropDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void On2sidesCheck();
	afx_msg void OnLandscapeRadio();
	afx_msg void OnSelchangePageSizeCombo();
	afx_msg void OnPortraitRadio();
	afx_msg void OnRullerRightRadio();
	afx_msg void OnRullerTopRadio();
	afx_msg void OnRullerBottomRadio();
	afx_msg void OnRullerLeftRadio();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NewPagePropDlg_H__6E1DFB63_AD36_11D3_A48F_00C0CA1261A6__INCLUDED_)
