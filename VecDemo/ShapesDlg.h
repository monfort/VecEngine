#if !defined(AFX_SHAPESDLG_H__7FF803C2_DF9E_46AD_9D3C_12E6108DE6A2__INCLUDED_)
#define AFX_SHAPESDLG_H__7FF803C2_DF9E_46AD_9D3C_12E6108DE6A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShapesDlg.h : header file
//

// the number of shapes
#define NUM_OF_SHAPES	85

// the number of rows
#define BTN_ROWS  9

#include "Shapes.h"

/////////////////////////////////////////////////////////////////////////////
// CShapesDlg dialog

class CShapesDlg : public CDialog
{
// Construction
public:
	CShapesDlg(CWnd* pParent = NULL);   // standard constructor

	void CShapesDlg::GetImageFromList(CImageList *lstImages, 
                             int nImage, CBitmap* destBitmap);

	int GetButtonIndex(LPARAM lParam);
	
	LPSHAPE GetSelectedShape(int index);

// Dialog Data
	//{{AFX_DATA(CShapesDlg)
	enum { IDD = IDD_SHAPESBOX };
	//}}AFX_DATA

	CButton m_arrBtn[NUM_OF_SHAPES];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShapesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	static DWORD m_defaultDataStyles[];

	CImageList m_ilStyles; //Handles multiple bitmap strip

	// Generated message map functions
	//{{AFX_MSG(CShapesDlg)
		// NOTE: the ClassWizard will add member functions here
		virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHAPESDLG_H__7FF803C2_DF9E_46AD_9D3C_12E6108DE6A2__INCLUDED_)
