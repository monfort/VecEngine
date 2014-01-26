// VecDemo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "VecDemo.h"

#include "MainFrm.h"
#include "VecDemoDoc.h"
#include "VecDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVecDemoApp

BEGIN_MESSAGE_MAP(CVecDemoApp, CWinApp)
	//{{AFX_MSG_MAP(CVecDemoApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


BOOL SetRegKeyValue(HKEY hBaseKey, LPTSTR pszKey, LPTSTR pszSubkey, LPTSTR pszValue);
/////////////////////////////////////////////////////////////////////////////
// CVecDemoApp construction

CVecDemoApp::CVecDemoApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CVecDemoApp object

CVecDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CVecDemoApp initialization

BOOL CVecDemoApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
    if (!AfxOleInit())
    {
        AfxMessageBox("Ole Initialization Failed");
        return FALSE;
    }   

   HDC hdc=GetDC(NULL);
   ASSERT(hdc!=NULL);
   int bpp = GetDeviceCaps(hdc, BITSPIXEL);
   ReleaseDC(NULL, hdc);
   if(bpp <= 8)
   {
      AfxMessageBox("This application requires more then 256 colors.");
      return FALSE;
   }


#if _MSC_VER<8
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Monfort Software"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CVecDemoDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CVecDemoView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

   // main frame will not show itself on creation
   m_nCmdShow = SW_HIDE;

	// Dispatch commands specified on the command line
   BOOL bFileOpen = FALSE;
   if(cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen)
   {
      cmdInfo.m_nShellCommand = CCommandLineInfo::FileNew;
      bFileOpen = TRUE;
   }
   
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;


   ((CMainFrame*)m_pMainWnd)->LoadWorkspace();

   // show the main frame with the size that we loded from the registery
   m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();
	

   if(bFileOpen)
   {
      POSITION pos = pDocTemplate->GetFirstDocPosition();
      pDocTemplate->GetNextDoc(pos)->OnOpenDocument(cmdInfo.m_strFileName);
   }
   else
   {
	  POSITION pos = pDocTemplate->GetFirstDocPosition();
	  CVecDemoDoc* pAlbDoc = (CVecDemoDoc*)pDocTemplate->GetNextDoc(pos);
	  pos = pAlbDoc->GetFirstViewPosition();
	  CVecDemoView* pVecDemoView = (CVecDemoView*) pAlbDoc->GetNextView(pos);
	  pVecDemoView->ClearDocument();

   }
   return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CStatusMessage 
CStatusMessage::CStatusMessage()
{
}

CStatusMessage::CStatusMessage(LPCSTR lpszMessage)
{
   m_strOldMessage = ((CMainFrame*)AfxGetMainWnd())->GetStatusBarMessage();
   ((CMainFrame*)AfxGetMainWnd())->SetStatusBarMessage(lpszMessage);
}

CStatusMessage::~CStatusMessage()
{
   ((CMainFrame*)AfxGetMainWnd())->SetStatusBarMessage(m_strOldMessage);
}

void CStatusMessage::SetStatusMessage(LPCSTR lpszMessage)
{
   ((CMainFrame*)AfxGetMainWnd())->SetStatusBarMessage(lpszMessage);
}

void CStatusMessage::SetKeepStatusMessage(LPCSTR lpszMessage)
{
    m_strOldMessage = ((CMainFrame*)AfxGetMainWnd())->GetStatusBarMessage();
   ((CMainFrame*)AfxGetMainWnd())->SetStatusBarMessage(lpszMessage);
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CVecDemoApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CVecDemoApp message handlers
// Get path to input folder


CString CVecDemoApp::GetPath()
{
   CString strPath = m_pszHelpFilePath;
   int i = strPath.ReverseFind('\\');
   if(i!=-1)
      return strPath.Left(i + 1);
   else
      return "" ;
}


void CVecDemoApp::ReleaseControl()
{
   MSG msg;

   while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
   {
      TRY
      {
         PumpMessage();
      }
      CATCH_ALL(e)
      {
         TRACE0("Exception in PumpMessage "
                "WinApp::ReleaseControl\n");
      }
      END_CATCH_ALL
   }

   return;
}


#define MAX_STRING_LENGTH _MAX_PATH

BOOL SetRegKeyValue(
       HKEY hBaseKey,
       LPTSTR pszKey,
       LPTSTR pszSubkey,
       LPTSTR pszValue)
{
  BOOL bOk = FALSE;
  LONG ec;
  HKEY hKey;
  TCHAR szKey[MAX_STRING_LENGTH];

  lstrcpy(szKey, pszKey);

  if (NULL != pszSubkey)
  {
    lstrcat(szKey, TEXT("\\"));
    lstrcat(szKey, pszSubkey);
  }

  ec = RegCreateKeyEx(
         hBaseKey,
         szKey,
         0,
         NULL,
         REG_OPTION_NON_VOLATILE,
         KEY_ALL_ACCESS,
         NULL,
         &hKey,
         NULL);

  if (NULL != pszValue && ERROR_SUCCESS == ec)
  {
    ec = RegSetValueEx(
           hKey,
           NULL,
           0,
           REG_SZ,
           (BYTE *)pszValue,
           (lstrlen(pszValue)+1)*sizeof(TCHAR));
    if (ERROR_SUCCESS == ec)
      bOk = TRUE;
    RegCloseKey(hKey);
  }

  return bOk;
}

