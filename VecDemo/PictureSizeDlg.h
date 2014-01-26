#if !defined(AFX_PICTURESIZEDLG_H__E0476046_B605_11D3_A495_00C0CA1261A6__INCLUDED_)
#define AFX_PICTURESIZEDLG_H__E0476046_B605_11D3_A495_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PictureSizeDlg.h : header file
//

#include "VecDemoView.h"
/////////////////////////////////////////////////////////////////////////////
// CPictureSizeDlg dialog

class CPictureSizeDlg : public CDialog
{
// Construction
public:
	CPictureSizeDlg(CVecDemoView* pVecTstView);   // standard constructor

   static void StoreToRegistry();
   static void ReadFromRegistry();

// Dialog Data
	//{{AFX_DATA(CPictureSizeDlg)
	enum { IDD = IDD_PICTURE_SIZE };
	CComboBox	m_cbWidthUnits;
	CComboBox	m_cbHeightUnits;
	BOOL	m_bPorportions;
	double	m_dHeight;
	double	m_dWidth;
	int		m_iHeightUnits;
	int		m_iWidthUnits;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPictureSizeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
   CVecDemoView* m_pVecTstView;
   void Apply();
   void UpdateHeight();
   void UpdateWidth();
   void FillWidthAndHeight();

   double m_dOriginalPicVecWidth;
   double m_dOriginalPicVecHeight;
   double m_dPicVecWidth;
   double m_dPicVecHeight;

   static BOOL m_bOldPorportions;
	static int m_iOldHeightUnits;
	static int m_iOldWidthUnits;

   double	m_dLastHeight;
	double	m_dLastWidth;
	int		m_iLastHeightUnits;
	int		m_iLastWidthUnits;

   // Generated message map functions
	//{{AFX_MSG(CPictureSizeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnApply();
	virtual void OnOK();
	afx_msg void OnChangeHeight();
	afx_msg void OnChangeWidth();
	afx_msg void OnProportions();
	afx_msg void OnSelchangeHeightUnits();
	afx_msg void OnSelchangeWidthUnits();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICTURESIZEDLG_H__E0476046_B605_11D3_A495_00C0CA1261A6__INCLUDED_)
