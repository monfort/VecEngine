#if !defined(AFX_LINETYPEDLG_H__D12E1058_A621_11D3_A489_00C0CA1261A6__INCLUDED_)
#define AFX_LINETYPEDLG_H__D12E1058_A621_11D3_A489_00C0CA1261A6__INCLUDED_

//#include "OXListBoxEx.h"


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LineTypeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLineTypeDlg dialog

class CVecDemoView;

class CLineTypeDlg : public CDialog
{
// Construction
public:
	CLineTypeDlg(CVecDemoView* pVecTstView);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLineTypeDlg)
	enum { IDD = IDD_LINE_TYPE };
	CListBox m_lbLineStyle;
	double	m_dLineWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLineTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
   CImageList m_imageList;
   CVecDemoView* m_pVecTstView;
   int AddItemToListBox(LPCTSTR lpszText, int nImageIndex);
   void Apply();

   double m_dLastSize;
   int m_iLastStyle;

	// Generated message map functions
	//{{AFX_MSG(CLineTypeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnApply();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINETYPEDLG_H__D12E1058_A621_11D3_A489_00C0CA1261A6__INCLUDED_)
