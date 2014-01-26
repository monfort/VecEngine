// NewPagePropDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VecDemo.h"
#include "NewPagePropDlg.h"
#include "VecDemoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewPagePropDlg dialog

BOOL CNewPagePropDlg::m_bIsOldValuesAvilable = FALSE;
int CNewPagePropDlg::m_iOldLandscapeRadio;
int CNewPagePropDlg::m_iOldPageSize;

CNewPagePropDlg::CNewPagePropDlg()
	:  CDialog(CNewPagePropDlg::IDD)
{
	//{{AFX_DATA_INIT(CNewPagePropDlg)
	m_iLandscapeRadio = 1;
	m_iPageSize = -1;
	//}}AFX_DATA_INIT
    m_bReadOnly = FALSE;

   //initilize dialog with old values
   if(m_bIsOldValuesAvilable)
   {
      m_iLandscapeRadio = m_iOldLandscapeRadio;
      m_iPageSize = m_iOldPageSize;
   }
}

CNewPagePropDlg::CNewPagePropDlg(CString const  &strTitle)
   :  CDialog(CNewPagePropDlg::IDD)
{
   
   //{{AFX_DATA_INIT(CNewPagePropDlg)
	m_iLandscapeRadio = 1;
	m_iPageSize = -1;
	//}}AFX_DATA_INIT
   m_bReadOnly = TRUE;
   SetWindowText(strTitle);


}

void CNewPagePropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewPagePropDlg)
	DDX_Control(pDX, IDC_PREVIEW, m_Preview);
	DDX_Control(pDX, IDC_PAGE_SIZE_COMBO, m_cbPageSize);
	DDX_Radio(pDX, IDC_PORTRAIT_RADIO, m_iLandscapeRadio);
	DDX_CBIndex(pDX, IDC_PAGE_SIZE_COMBO, m_iPageSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewPagePropDlg, CDialog)
	//{{AFX_MSG_MAP(CNewPagePropDlg)
	ON_BN_CLICKED(IDC_LANDSCAPE_RADIO, OnLandscapeRadio)
	ON_CBN_SELCHANGE(IDC_PAGE_SIZE_COMBO, OnSelchangePageSizeCombo)
	ON_BN_CLICKED(IDC_PORTRAIT_RADIO, OnPortraitRadio)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewPagePropDlg message handlers

#define NUM_OF_PAGE_SIZES 3
char* aPageSizesStrings[NUM_OF_PAGE_SIZES] = {"A3", "A4", "A5"};
double aPageSizesInches[NUM_OF_PAGE_SIZES][2] = 
{ //{width, height} of the page in Portrait
   {16.5354, 23.3858},
   {8.2677, 11.6929},
   {4.13385, 5.84645}
};
#define DEFAULT_SELECTED_ITEM 1

BOOL CNewPagePropDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

   for(int i=0; i<NUM_OF_PAGE_SIZES; i++)
      m_cbPageSize.AddString(aPageSizesStrings[i]);
   if(!m_bReadOnly)
      m_cbPageSize.SetCurSel(DEFAULT_SELECTED_ITEM);
   
   UpdatePreviewBitmap();
	
   if(m_bReadOnly)
   {
      m_cbPageSize.EnableWindow(FALSE);
      GetDlgItem(IDC_PORTRAIT_RADIO)->EnableWindow(FALSE);
      GetDlgItem(IDC_LANDSCAPE_RADIO)->EnableWindow(FALSE);
      m_cbPageSize.SetCurSel(m_iPageSize);
   }

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNewPagePropDlg::OnOK() 
{
   UpdateData();

   //get the sizes of the page
   m_dPageWidth = aPageSizesInches[m_iPageSize][0];
   m_dPageHeight = aPageSizesInches[m_iPageSize][1];

   //if the page is landscape then switch
   if(m_iLandscapeRadio == 1)
   {
      double dTemp = m_dPageHeight;
      m_dPageHeight = m_dPageWidth;
      m_dPageWidth = dTemp;
   }
	
   //store to old values
   m_bIsOldValuesAvilable = TRUE;
   m_iOldLandscapeRadio = m_iLandscapeRadio;
   m_iOldPageSize = m_iPageSize;

   CDialog::OnOK();
}

void CNewPagePropDlg::On2sidesCheck() 
{
}

void CNewPagePropDlg::OnLandscapeRadio() 
{
	UpdatePreviewBitmap();
}

void CNewPagePropDlg::OnSelchangePageSizeCombo() 
{
}

void CNewPagePropDlg::OnPortraitRadio() 
{
	UpdatePreviewBitmap();
}

void CNewPagePropDlg::OnRullerRightRadio() 
{
	UpdatePreviewBitmap();
}

void CNewPagePropDlg::OnRullerTopRadio() 
{
	UpdatePreviewBitmap();
}

void CNewPagePropDlg::OnRullerBottomRadio() 
{
	UpdatePreviewBitmap();
}

void CNewPagePropDlg::OnRullerLeftRadio() 
{
	UpdatePreviewBitmap();
}

void CNewPagePropDlg::UpdatePreviewBitmap()
{
   if(!m_bReadOnly)
      UpdateData();

   //detach and delete the old bitmap if there was one
   m_PreviewBitmap.DeleteObject();

   //check wich bitmap is the onw we should show
	if(m_iLandscapeRadio == 1)
   {
	  m_PreviewBitmap.LoadBitmap(IDB_PAGE_LAND);
   }
	else
   {
	  m_PreviewBitmap.LoadBitmap(IDB_PAGE_PORT);
   }

   m_Preview.SetBitmap(m_PreviewBitmap);
}



void CNewPagePropDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
}
