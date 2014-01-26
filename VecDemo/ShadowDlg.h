#if !defined(AFX_SHADOWDLG_H__6B23B52B_648E_4351_9F6E_B70284582961__INCLUDED_)
#define AFX_SHADOWDLG_H__6B23B52B_648E_4351_9F6E_B70284582961__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShadowDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShadowDlg dialog
//#include "OXColorPickerButton.h"

class CVecDemoView;

class CShadowDlg : public CDialog
{
// Construction
public:
	CShadowDlg(CVecDemoView* pVecTstView);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CShadowDlg)
	enum { IDD = IDD_SHADOW_DLG };
	CButton	m_ColorPickerButton;
	CComboBox	m_cbWidthUnits;
	CComboBox	m_cbHeightUnits;
	BOOL	m_Rop2;
	BOOL	m_bUseShadow;
	int		m_iHeightUnits;
	int		m_iWidthUnits;
	double	m_dWidth;
	double	m_dHeight;
	COLORREF m_clrShadow;
	//}}AFX_DATA

   static void StoreToRegistry();
   static void ReadFromRegistry();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShadowDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
   CVecDemoView* m_pVecTstView;

   void Apply();
   void UpdateRealSize();
   void UpdateSize();
	double	m_dRealWidth;
	double	m_dRealHeight;

	static int m_iOldHeightUnits;
	static int m_iOldWidthUnits;

   // Generated message map functions
	//{{AFX_MSG(CShadowDlg)
	virtual void OnOK();
	afx_msg void OnApply();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeHeightUnits();
	afx_msg void OnSelchangeWidthUnits();
	afx_msg void OnUseShadow();
	afx_msg void OnKillfocusWidth();
	afx_msg void OnKillfocusHeight();
	afx_msg void OnDestroy();
	afx_msg void OnColorButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHADOWDLG_H__6B23B52B_648E_4351_9F6E_B70284582961__INCLUDED_)
