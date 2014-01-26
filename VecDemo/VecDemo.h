// VecDemo.h : main header file for the VecDemo application
//

#if !defined(AFX_PAS_H__A8F04FA5_95A5_11D3_A47C_00C0CA1261A6__INCLUDED_)
#define AFX_PAS_H__A8F04FA5_95A5_11D3_A47C_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CVecDemoApp:
// See VecDemo.cpp for the implementation of this class
//

class CVecDemoApp : public CWinApp
{
public:
	CString GetPath();
	CVecDemoApp();
   void ReleaseControl();
   
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVecDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CVecDemoApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:

};

class CStatusMessage
{
   CString m_strOldMessage;
public:
   CStatusMessage();
   CStatusMessage(LPCSTR lpszMessage);
   void SetStatusMessage(LPCSTR lpszMessage);
   void SetKeepStatusMessage(LPCSTR lpszMessage);
   ~CStatusMessage();
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAS_H__A8F04FA5_95A5_11D3_A47C_00C0CA1261A6__INCLUDED_)
