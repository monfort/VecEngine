// PageBackgroundDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VecDemo.h"
#include "PageBackgroundDlg.h"
#include "VecDemoView.h"
#include "VecDemodoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageBackgroundDlg dialog
BOOL CPageBackgroundDlg::m_bOldApplyForAll = TRUE;

CPageBackgroundDlg::CPageBackgroundDlg(CVecDemoView* pVecTstView)
	: CDialog(CPageBackgroundDlg::IDD, NULL)
{
   m_pVecTstView = pVecTstView;
   m_bApplyed = FALSE;
	//{{AFX_DATA_INIT(CPageBackgroundDlg)
	m_iBgImageRadio = -1;
	//}}AFX_DATA_INIT
}


void CPageBackgroundDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageBackgroundDlg)
	DDX_Control(pDX, IDC_COLOR_BUTTON, m_ColorPickerButton);
	DDX_Control(pDX, IDC_BGIMG_RADIO, m_BgImageRadio);
	DDX_Control(pDX, IDC_BACK_IMAGE_BROWSE, m_BackImageEdit);
	DDX_Radio(pDX, IDC_BGIMG_RADIO, m_iBgImageRadio);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageBackgroundDlg, CDialog)
	//{{AFX_MSG_MAP(CPageBackgroundDlg)
	ON_BN_CLICKED(IDAPPLY, OnApply)
	ON_BN_CLICKED(IDC_BGIMG_RADIO, OnBgimgRadio)
	ON_BN_CLICKED(IDC_SOLIDCOL_RADIO, OnSolidcolRadio)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_COLOR_BUTTON, OnColorButton)
	ON_BN_CLICKED(IDC_BUTTON_FILEOPEN, OnButtonFileopen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageBackgroundDlg message handlers

void CPageBackgroundDlg::OnOK() 
{
   if(!UpdateData(TRUE))
      return;
	Apply();

   CDialog::OnOK();
}

void CPageBackgroundDlg::OnApply() 
{
   if(!UpdateData(TRUE))
      return;
	Apply();
}

CString CPageBackgroundDlg::m_strLastDir;

BOOL CPageBackgroundDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
   GetCurrentDirectory(_MAX_PATH, m_strCurrentDir.GetBuffer(_MAX_PATH));
   m_strCurrentDir.ReleaseBuffer();

   if(!m_strLastDir.IsEmpty())
      SetCurrentDirectory(m_strLastDir);
   
   m_BackImageEdit.SetWindowText(m_pVecTstView->GetDocument()->GetBGImgPath());

   CString strColorHex;
   strColorHex.Format("Color: #%06X",m_pVecTstView->GetDocument()->GetSolidCol());
   m_ColorPickerButton.SetWindowText(strColorHex);

   m_iBgImageRadio = m_pVecTstView->GetDocument()->GetIsBGImg() ? 0 : 1;

   UpdateData(FALSE);
   EnableControls(m_iBgImageRadio==0);
      
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageBackgroundDlg::Apply()
{
	CString sPath;
	m_BackImageEdit.GetWindowText(sPath);

   m_bApplyed = TRUE;
   m_pVecTstView->GetDocument()->SetBGImgPath(sPath);
   m_pVecTstView->GetDocument()->SetIsBGImg(m_iBgImageRadio==0 ? TRUE : FALSE);

   m_pVecTstView->LoadBackground();
   m_pVecTstView->m_VecContainer.Repaint();
   ((CVecDemoApp*)AfxGetApp())->ReleaseControl();//let the ape to repaint itself
   m_pVecTstView->m_VecController.SetModified();
}

void CPageBackgroundDlg::OnBgimgRadio() 
{
   UpdateData(TRUE);
	EnableControls(TRUE);
}

void CPageBackgroundDlg::OnSolidcolRadio() 
{
   UpdateData(TRUE);
	EnableControls(FALSE);
}

void CPageBackgroundDlg::EnableControls(BOOL bImage)
{
   m_BackImageEdit.EnableWindow(bImage);
}
 

void CPageBackgroundDlg::OnDestroy() 
{
   UpdateData(TRUE);

   CDialog::OnDestroy();
	
   GetCurrentDirectory(_MAX_PATH, m_strLastDir.GetBuffer(_MAX_PATH));
   m_strLastDir.ReleaseBuffer();


   SetCurrentDirectory(m_strCurrentDir);
}

#define PREVIEW_BITMAP_SIZE 80
#define PREVIEW_NO_BITMAP_COLOR RGB(128,128,128)

void CPageBackgroundDlg::OnColorButton() 
{
	//Get OldBackGround color
	COLORREF clrSolid = m_pVecTstView->GetDocument()->GetSolidCol();

	CColorDialog dlg(clrSolid, CC_FULLOPEN);
   
	if(dlg.DoModal()==IDOK)
   {
		clrSolid = dlg.GetColor();

	   CString strColorHex;
	   strColorHex.Format("Color: #%06X",clrSolid);
	   m_ColorPickerButton.SetWindowText(strColorHex);

       m_pVecTstView->GetDocument()->SetSolidCol(clrSolid);
   }	
}

void CPageBackgroundDlg::OnButtonFileopen() 
{
	CString strPath;
	m_BackImageEdit.GetWindowText(strPath);

	//m_strCurrentDir	
	CFileDialog OpenDlg(TRUE,                                       // Open
                      (LPCTSTR)"bmp",                              // File Extention
                       strPath,                            // FileName
                       OFN_OVERWRITEPROMPT,                        // Flags
                      (LPCTSTR)"Image Files (*.bmp)|*.bmp|All Files (*.*)|*.*||");// Filter

   //Show The File Dialog
   if(OpenDlg.DoModal() == IDOK)
   {
      strPath = OpenDlg.GetPathName();//copy the path to the control
	  m_BackImageEdit.SetWindowText(strPath);
	  
   }
}
