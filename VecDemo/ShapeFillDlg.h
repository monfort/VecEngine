#if !defined(AFX_SHAPEFILLDLG_H__D12E105A_A621_11D3_A489_00C0CA1261A6__INCLUDED_)
#define AFX_SHAPEFILLDLG_H__D12E105A_A621_11D3_A489_00C0CA1261A6__INCLUDED_

//#include "OXListBoxEx.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShapeFillDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShapeFillDlg dialog
class CVecDemoView;

class CShapeFillDlg : public CDialog
{
// Construction
public:
	CShapeFillDlg(CVecDemoView* pVecTstView, BOOL bIsPatternEnabled = TRUE);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CShapeFillDlg)
	enum { IDD = IDD_SHAPE_FILL };
	CListBox	m_lbFillMode;
	CListBox	m_lbFillPattern;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShapeFillDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
   BOOL m_bIsPatternEnabled;
   CVecDemoView* m_pVecTstView;
   int AddItemToModeList(LPCTSTR lpszText, int nImageIndex);
   int AddItemToPatList(LPCTSTR lpszText, int nImageIndex);
   void Apply();

   int m_iLastFillMode;
   int m_iLastPattern;

	// Generated message map functions
	//{{AFX_MSG(CShapeFillDlg)
	afx_msg void OnApply();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHAPEFILLDLG_H__D12E105A_A621_11D3_A489_00C0CA1261A6__INCLUDED_)
