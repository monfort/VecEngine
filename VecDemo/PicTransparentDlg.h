#if !defined(AFX_PICTRANSPARENTDLG_H__C59795A9_4793_4050_A90A_BEA918F862F9__INCLUDED_)
#define AFX_PICTRANSPARENTDLG_H__C59795A9_4793_4050_A90A_BEA918F862F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PicTransparentDlg.h : header file
//

//#include "OXColorPickerButton.h"
/////////////////////////////////////////////////////////////////////////////
// CPicTransparentDlg dialog
class CVecDemoView;

class CPicTransparentDlg : public CDialog
{
// Construction
public:
	CPicTransparentDlg(CVecDemoView* pVecTstView);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPicTransparentDlg)
	enum { IDD = IDD_PIC_TRANSPARENT_DLG };
	CButton	m_ColorPickerButton;
	BOOL	m_bIsTransparent;
	COLORREF m_clrTransparent;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPicTransparentDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
   CVecDemoView* m_pVecTstView;
   void Apply();

	// Generated message map functions
	//{{AFX_MSG(CPicTransparentDlg)
	virtual void OnOK();
	afx_msg void OnIsTransparent();
	afx_msg void OnApply();
	virtual BOOL OnInitDialog();
	afx_msg void OnColorButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICTRANSPARENTDLG_H__C59795A9_4793_4050_A90A_BEA918F862F9__INCLUDED_)
