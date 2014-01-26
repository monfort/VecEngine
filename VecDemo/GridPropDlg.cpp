// GridProp.cpp : implementation file
//

#include "stdafx.h"
#include "VecDemo.h"
#include "GridPropDlg.h"
#include "VecDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define GRID_MIN_SIZE_IN_INCH 0.05 // Remember to change IDS_GRID_TOO_SMALL

/////////////////////////////////////////////////////////////////////////////
// CGridPropDlg dialog

int CGridPropDlg::m_iOldUnits = 1;

void CGridPropDlg::StoreToRegistry()
{
   AfxGetApp()->WriteProfileInt("GridPropDialog", "SizeUnits", m_iOldUnits);
}

void CGridPropDlg::ReadFromRegistry()
{
   m_iOldUnits = AfxGetApp()->GetProfileInt("GridPropDialog", "SizeUnits", m_iOldUnits);
}

#define NUM_OF_UNITS 2
enum GridUnits
{
   UNITS_CM = 0,
   UNITS_INCH
};
char* aGridUnitsStrings[NUM_OF_UNITS] = 
{
   "cm", 
   "Inch", 
};

CGridPropDlg::CGridPropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGridPropDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGridPropDlg)
	m_dSize = 0.0;
	m_iUnits = m_iOldUnits;
	//}}AFX_DATA_INIT
   m_dRealSize = 0;
}


void CGridPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGridPropDlg)
	DDX_Control(pDX, IDC_WIDTH_UNITS, m_cbSizeUnits);
	DDX_Text(pDX, IDC_WIDTH, m_dSize);
	DDX_CBIndex(pDX, IDC_WIDTH_UNITS, m_iUnits);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGridPropDlg, CDialog)
	//{{AFX_MSG_MAP(CGridPropDlg)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_WIDTH_UNITS, OnSelchangeWidthUnits)
	ON_EN_CHANGE(IDC_WIDTH, OnChangeWidth)
	ON_EN_KILLFOCUS(IDC_WIDTH, OnKillfocusWidth)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridPropDlg message handlers

void CGridPropDlg::OnDestroy() 
{
   UpdateData(TRUE);

   CDialog::OnDestroy();
	
   m_iOldUnits = m_iUnits;
}

BOOL CGridPropDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   for(int i = 0; i<NUM_OF_UNITS; i++)
   {
      m_cbSizeUnits.AddString(aGridUnitsStrings[i]);
   }
	
   UpdateSize();
   UpdateData(FALSE);

   return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGridPropDlg::OnOK() 
{
   if(!UpdateData(TRUE))
      return;
	
   UpdateRealSize();

   if(m_dRealSize<GRID_MIN_SIZE_IN_INCH*VEC_UNITS_PER_INCH)
   {
      AfxMessageBox(IDS_GRID_TOO_SMALL);
      return;
   }

   CDialog::OnOK();
}

void CGridPropDlg::UpdateRealSize()
{
   switch(m_iUnits)
   {
   case UNITS_CM:
	   m_dRealSize = m_dSize * (VEC_UNITS_PER_INCH / CM_IN_INCH);
      break;
   case UNITS_INCH:
	   m_dRealSize = m_dSize * VEC_UNITS_PER_INCH;
      break;
   }
}

void CGridPropDlg::UpdateSize()
{
   switch(m_iUnits)
   {
   case UNITS_CM:
      m_dSize = m_dRealSize / (VEC_UNITS_PER_INCH / CM_IN_INCH);
      break;
   case UNITS_INCH:
      m_dSize = m_dRealSize / VEC_UNITS_PER_INCH;
      break;
   }
}

void CGridPropDlg::OnSelchangeWidthUnits() 
{
   UpdateData(TRUE);

   UpdateSize();

   UpdateData(FALSE);
}

void CGridPropDlg::OnChangeWidth() 
{
}

void CGridPropDlg::OnKillfocusWidth() 
{
   UpdateData(TRUE);

   UpdateRealSize();

   UpdateData(FALSE);
}
