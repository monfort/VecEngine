// ShadowDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VecDemo.h"
#include "ShadowDlg.h"
#include "VecDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShadowDlg dialog

#define NUM_OF_UNITS 2
enum ShadowUnits
{
   UNITS_CM = 0,
   UNITS_INCH,
};
char* aShadowUnitsStrings[NUM_OF_UNITS] = 
{
   "cm", 
   "Inch", 
};

int CShadowDlg::m_iOldHeightUnits = 1;
int CShadowDlg::m_iOldWidthUnits = 1;

void CShadowDlg::StoreToRegistry()
{
   AfxGetApp()->WriteProfileInt("ShadowDlg", "WidthUnits", m_iOldWidthUnits);
   AfxGetApp()->WriteProfileInt("ShadowDlg", "HeightUnits", m_iOldHeightUnits);
}

void CShadowDlg::ReadFromRegistry()
{
   m_iOldWidthUnits = AfxGetApp()->GetProfileInt("ShadowDlg", "WidthUnits", m_iOldWidthUnits);
   m_iOldHeightUnits = AfxGetApp()->GetProfileInt("ShadowDlg", "HeightUnits", m_iOldHeightUnits);
}

CShadowDlg::CShadowDlg(CVecDemoView* pVecTstView)
	: CDialog(CShadowDlg::IDD, NULL)
{
   m_pVecTstView = pVecTstView;
	//{{AFX_DATA_INIT(CShadowDlg)
	m_Rop2 = FALSE;
	m_bUseShadow = FALSE;
	m_iHeightUnits = m_iOldHeightUnits;
	m_iWidthUnits = m_iOldWidthUnits;
	m_dWidth = 0.0;
	m_dHeight = 0.0;
	//}}AFX_DATA_INIT
}


void CShadowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShadowDlg)
	DDX_Control(pDX, IDC_COLOR_BUTTON, m_ColorPickerButton);
	DDX_Control(pDX, IDC_WIDTH_UNITS, m_cbWidthUnits);
	DDX_Control(pDX, IDC_HEIGHT_UNITS, m_cbHeightUnits);
	DDX_Check(pDX, IDC_ROP2, m_Rop2);
	DDX_Check(pDX, IDC_USE_SHADOW, m_bUseShadow);
	DDX_CBIndex(pDX, IDC_HEIGHT_UNITS, m_iHeightUnits);
	DDX_CBIndex(pDX, IDC_WIDTH_UNITS, m_iWidthUnits);
	DDX_Text(pDX, IDC_WIDTH, m_dWidth);
	DDX_Text(pDX, IDC_HEIGHT, m_dHeight);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShadowDlg, CDialog)
	//{{AFX_MSG_MAP(CShadowDlg)
	ON_BN_CLICKED(IDAPPLY, OnApply)
	ON_CBN_SELCHANGE(IDC_HEIGHT_UNITS, OnSelchangeHeightUnits)
	ON_CBN_SELCHANGE(IDC_WIDTH_UNITS, OnSelchangeWidthUnits)
	ON_BN_CLICKED(IDC_USE_SHADOW, OnUseShadow)
	ON_EN_KILLFOCUS(IDC_WIDTH, OnKillfocusWidth)
	ON_EN_KILLFOCUS(IDC_HEIGHT, OnKillfocusHeight)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_COLOR_BUTTON, OnColorButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShadowDlg message handlers

void CShadowDlg::OnOK() 
{
   if(!UpdateData(TRUE))
      return;
	Apply();
	
	CDialog::OnOK();
}

void CShadowDlg::OnApply() 
{
   if(!UpdateData(TRUE))
      return;
	Apply();
}

BOOL CShadowDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   for(int i = 0; i<NUM_OF_UNITS; i++)
   {
      m_cbWidthUnits.AddString(aShadowUnitsStrings[i]);
      m_cbHeightUnits.AddString(aShadowUnitsStrings[i]);
   }

   CString strColorHex;
   CPtrList SelItems;
   m_pVecTstView->GetSelectedItems(SelItems);
   POSITION pos = SelItems.GetHeadPosition();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);;

      m_bUseShadow = pVec->IsShowShadow();
      m_dRealWidth = pVec->GetShadowOffset().x;
      m_dRealHeight = pVec->GetShadowOffset().y;
      m_Rop2 = (pVec->GetShadowRop2() == R2_MASKPEN);
	  
	  m_clrShadow = pVec->GetShadowColor();
	   strColorHex.Format("Color: #%06X",m_clrShadow);
	   m_ColorPickerButton.SetWindowText(strColorHex);

      UpdateData(FALSE);
      break;//initiate the dialog with the data of the first text item
   }

   OnUseShadow();
   UpdateSize();
   UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CShadowDlg::Apply()
{
   UpdateRealSize();

   CPtrList SelItems;
   m_pVecTstView->GetSelectedItems(SelItems);
   POSITION pos = SelItems.GetHeadPosition();
   m_pVecTstView->m_VecController.m_UndoManager.CreateNewStep();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);;
      m_pVecTstView->m_VecController.m_UndoManager.AddActionToLastStep(pVec, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
      pVec->SetShowShadow(m_bUseShadow);
      CDPoint RealOffset(m_dRealWidth, m_dRealHeight);
      pVec->SetShadowOffset(RealOffset);
      pVec->SetShadowRop2(m_Rop2 ? R2_MASKPEN : R2_COPYPEN);
      pVec->SetShadowColor(m_clrShadow);

   }
   m_pVecTstView->m_VecContainer.Repaint();
   m_pVecTstView->m_VecController.SetModified();
}

void CShadowDlg::OnSelchangeHeightUnits() 
{
   UpdateData(TRUE);

   UpdateSize();

   UpdateData(FALSE);
}

void CShadowDlg::OnSelchangeWidthUnits() 
{
   UpdateData(TRUE);

   UpdateSize();

   UpdateData(FALSE);
}

void CShadowDlg::OnUseShadow() 
{
   UpdateData(TRUE);
	GetDlgItem(IDC_COLOR_BUTTON)->EnableWindow(m_bUseShadow);
	GetDlgItem(IDC_WIDTH_UNITS)->EnableWindow(m_bUseShadow);
	GetDlgItem(IDC_HEIGHT_UNITS)->EnableWindow(m_bUseShadow);
	GetDlgItem(IDC_ROP2)->EnableWindow(m_bUseShadow);
	GetDlgItem(IDC_WIDTH)->EnableWindow(m_bUseShadow);
	GetDlgItem(IDC_HEIGHT)->EnableWindow(m_bUseShadow);
}

void CShadowDlg::OnKillfocusWidth() 
{
   UpdateData(TRUE);

   UpdateRealSize();

   UpdateData(FALSE);
}

void CShadowDlg::OnKillfocusHeight() 
{
   UpdateData(TRUE);

   UpdateRealSize();

   UpdateData(FALSE);
}

void CShadowDlg::UpdateRealSize()
{
   switch(m_iWidthUnits)
   {
   case UNITS_CM:
	   m_dRealWidth = m_dWidth * (VEC_UNITS_PER_INCH / CM_IN_INCH);
      break;
   case UNITS_INCH:
	   m_dRealWidth = m_dWidth * VEC_UNITS_PER_INCH;
      break;
   }
   switch(m_iHeightUnits)
   {
   case UNITS_CM:
	   m_dRealHeight = m_dHeight * (VEC_UNITS_PER_INCH / CM_IN_INCH);
      break;
   case UNITS_INCH:
	   m_dRealHeight = m_dHeight * VEC_UNITS_PER_INCH;
      break;
   }
}

void CShadowDlg::UpdateSize()
{
   switch(m_iWidthUnits)
   {
   case UNITS_CM:
      m_dWidth = m_dRealWidth / (VEC_UNITS_PER_INCH / CM_IN_INCH);
      break;
   case UNITS_INCH:
      m_dWidth = m_dRealWidth / VEC_UNITS_PER_INCH;
      break;
   }
   switch(m_iHeightUnits)
   {
   case UNITS_CM:
      m_dHeight = m_dRealHeight / (VEC_UNITS_PER_INCH / CM_IN_INCH);
      break;
   case UNITS_INCH:
      m_dHeight = m_dRealHeight / VEC_UNITS_PER_INCH;
      break;
   }
}

void CShadowDlg::OnDestroy() 
{
   UpdateData(TRUE);

	CDialog::OnDestroy();
	
	m_iOldHeightUnits = m_iHeightUnits;
	m_iOldWidthUnits = m_iWidthUnits;
}

void CShadowDlg::OnColorButton() 
{
	CColorDialog dlg(m_clrShadow, CC_FULLOPEN);
   
	if(dlg.DoModal()==IDOK)
   {
		m_clrShadow = dlg.GetColor();

	    CString strColorHex;
		strColorHex.Format("Color: #%06X",m_clrShadow);
		m_ColorPickerButton.SetWindowText(strColorHex);
   }	
}
