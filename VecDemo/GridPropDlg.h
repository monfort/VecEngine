#if !defined(AFX_GRIDPROP_H__F9BACE26_DE2E_4E88_87C0_E6A7A70EF9EA__INCLUDED_)
#define AFX_GRIDPROP_H__F9BACE26_DE2E_4E88_87C0_E6A7A70EF9EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GridProp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGridPropDlg dialog

class CGridPropDlg : public CDialog
{
// Construction
public:
	CGridPropDlg(CWnd* pParent = NULL);   // standard constructor

   static void StoreToRegistry();
   static void ReadFromRegistry();

// Dialog Data
	//{{AFX_DATA(CGridPropDlg)
	enum { IDD = IDD_GRID_PROP };
	CComboBox	m_cbSizeUnits;
	double	m_dSize;
	int		m_iUnits;
	//}}AFX_DATA

	double	m_dRealSize;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridPropDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
   void UpdateRealSize();
   void UpdateSize();
	static int m_iOldUnits;

	// Generated message map functions
	//{{AFX_MSG(CGridPropDlg)
	virtual void OnOK();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeWidthUnits();
	afx_msg void OnChangeWidth();
	afx_msg void OnKillfocusWidth();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDPROP_H__F9BACE26_DE2E_4E88_87C0_E6A7A70EF9EA__INCLUDED_)
