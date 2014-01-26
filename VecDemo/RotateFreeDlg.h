#if !defined(AFX_ROTATEFREEDLG_H__B4767673_A658_11D3_A489_00C0CA1261A6__INCLUDED_)
#define AFX_ROTATEFREEDLG_H__B4767673_A658_11D3_A489_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RotateFreeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRotateFreeDlg dialog
class CVecDemoView;

class CRotateFreeDlg : public CDialog
{
// Construction
public:
	CRotateFreeDlg(CVecDemoView* pVecTstView);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRotateFreeDlg)
	enum { IDD = IDD_ROTATE_FREE };
	CEdit	m_edtAngle;
	CSpinButtonCtrl	m_SpinCtrl;
	double	m_dAngle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRotateFreeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
   CVecDemoView* m_pVecTstView;
   void Apply();

	// Generated message map functions
	//{{AFX_MSG(CRotateFreeDlg)
	afx_msg void OnApply();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEdit2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROTATEFREEDLG_H__B4767673_A658_11D3_A489_00C0CA1261A6__INCLUDED_)
