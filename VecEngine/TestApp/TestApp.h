// TestApp.h : main header file for the TestApp application
//

#if !defined(AFX_TestApp_H__B37B9A13_1B80_4F96_992C_3F89249CAF9D__INCLUDED_)
#define AFX_TestApp_H__B37B9A13_1B80_4F96_992C_3F89249CAF9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestAppApp:
// See TestApp.cpp for the implementation of this class
//

class CTestAppApp : public CWinApp
{
public:
	CTestAppApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestAppApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CTestAppApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TestApp_H__B37B9A13_1B80_4F96_992C_3F89249CAF9D__INCLUDED_)
