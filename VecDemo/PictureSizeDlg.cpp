// PictureSizeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VecDemo.h"
#include "PictureSizeDlg.h"
#include "VecDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPictureSizeDlg dialog
BOOL CPictureSizeDlg::m_bOldPorportions = TRUE;
int CPictureSizeDlg::m_iOldHeightUnits = 1;
int CPictureSizeDlg::m_iOldWidthUnits = 1;

void CPictureSizeDlg::StoreToRegistry()
{
   AfxGetApp()->WriteProfileInt("PictureSizeDialog", "WidthUnits", m_iOldWidthUnits);
   AfxGetApp()->WriteProfileInt("PictureSizeDialog", "HeightUnits", m_iOldHeightUnits);
   AfxGetApp()->WriteProfileInt("PictureSizeDialog", "Porportions", m_bOldPorportions);
}

void CPictureSizeDlg::ReadFromRegistry()
{
   m_iOldWidthUnits = AfxGetApp()->GetProfileInt("PictureSizeDialog", "WidthUnits", m_iOldWidthUnits);
   m_iOldHeightUnits = AfxGetApp()->GetProfileInt("PictureSizeDialog", "HeightUnits", m_iOldHeightUnits);
   m_bOldPorportions = AfxGetApp()->GetProfileInt("PictureSizeDialog", "Porportions", m_bOldPorportions);
}


#define NUM_OF_UNITS 3
enum Units
{
   UNITS_CM = 0,
   UNITS_INCH,
   UNITS_PERCENT
};
char* aUnitsStrings[NUM_OF_UNITS] = 
{
   "cm", 
   "Inch", 
   "Percent"
};


CPictureSizeDlg::CPictureSizeDlg(CVecDemoView* pVecTstView)
	: CDialog(CPictureSizeDlg::IDD, NULL)
{
   m_pVecTstView = pVecTstView;
	//{{AFX_DATA_INIT(CPictureSizeDlg)
	m_bPorportions = m_bOldPorportions;
	m_dHeight = 0.0;
	m_dWidth = 0.0;
	m_iHeightUnits = m_iOldHeightUnits;
	m_iWidthUnits = m_iOldWidthUnits;
	//}}AFX_DATA_INIT
   m_dLastHeight = -2;
	m_dLastWidth = -2;
	m_iLastHeightUnits = -2;
	m_iLastWidthUnits = -2;
}


void CPictureSizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPictureSizeDlg)
	DDX_Control(pDX, IDC_WIDTH_UNITS, m_cbWidthUnits);
	DDX_Control(pDX, IDC_HEIGHT_UNITS, m_cbHeightUnits);
	DDX_Check(pDX, IDC_PROPORTIONS, m_bPorportions);
	DDX_Text(pDX, IDC_HEIGHT, m_dHeight);
	DDX_Text(pDX, IDC_WIDTH, m_dWidth);
	DDX_CBIndex(pDX, IDC_HEIGHT_UNITS, m_iHeightUnits);
	DDX_CBIndex(pDX, IDC_WIDTH_UNITS, m_iWidthUnits);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPictureSizeDlg, CDialog)
	//{{AFX_MSG_MAP(CPictureSizeDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDAPPLY, OnApply)
	ON_EN_CHANGE(IDC_HEIGHT, OnChangeHeight)
	ON_EN_CHANGE(IDC_WIDTH, OnChangeWidth)
	ON_BN_CLICKED(IDC_PROPORTIONS, OnProportions)
	ON_CBN_SELCHANGE(IDC_HEIGHT_UNITS, OnSelchangeHeightUnits)
	ON_CBN_SELCHANGE(IDC_WIDTH_UNITS, OnSelchangeWidthUnits)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPictureSizeDlg message handlers

BOOL CPictureSizeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   for(int i = 0; i<NUM_OF_UNITS; i++)
   {
      m_cbWidthUnits.AddString(aUnitsStrings[i]);
      m_cbHeightUnits.AddString(aUnitsStrings[i]);
   }

   
   CPtrList SelItems;
   m_pVecTstView->GetSelectedItems(SelItems, TRUE);
   CVecContainer::ArrangeItemsForTrack(SelItems);
   POSITION pos = SelItems.GetHeadPosition();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);;

      CDRect rect;
      rect = pVec->GetRect();
      m_dPicVecWidth = m_dOriginalPicVecWidth = rect.right - rect.left;
      m_dPicVecHeight = m_dOriginalPicVecHeight = rect.bottom - rect.top;

      UpdateData(FALSE);
      break;//initiate the dialog with the data of the first text item
   }

   FillWidthAndHeight();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPictureSizeDlg::Apply() 
{
   if(m_dLastHeight == m_dHeight && m_dLastWidth == m_dWidth && 
      m_iLastHeightUnits == m_iHeightUnits && m_iLastWidthUnits == m_iWidthUnits)
   return;

   CPtrList SelItems;
   m_pVecTstView->GetSelectedItems(SelItems, TRUE);
   CVecContainer::ArrangeItemsForTrack(SelItems);
   POSITION pos = SelItems.GetHeadPosition();
   m_pVecTstView->m_VecController.m_UndoManager.CreateNewStep();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);;
      m_pVecTstView->m_VecController.m_UndoManager.AddActionToLastStep(pVec, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
      CDRect rect;
      rect = pVec->GetRect();

      switch(m_iWidthUnits)
      {
      case UNITS_CM:
         rect.right = rect.left + m_dWidth * (VEC_UNITS_PER_INCH / CM_IN_INCH);
         break;
      case UNITS_INCH:
         rect.right = rect.left + m_dWidth * VEC_UNITS_PER_INCH ;
         break;
      case UNITS_PERCENT:
         rect.right = rect.left + (rect.right-rect.left) * (m_dWidth / 100);
         break;
      }

      switch(m_iHeightUnits)
      {
      case UNITS_CM:
         rect.bottom = rect.top + m_dHeight * (VEC_UNITS_PER_INCH / CM_IN_INCH);
         break;
      case UNITS_INCH:
         rect.bottom = rect.top + m_dHeight * VEC_UNITS_PER_INCH;
         break;
      case UNITS_PERCENT:
         rect.bottom = rect.top +  (rect.bottom-rect.top) * (m_dHeight / 100);
         break;
      }

      pVec->SetRect(rect);
   }
   m_pVecTstView->m_VecContainer.Repaint();
   m_pVecTstView->m_VecController.SetModified();

   m_dLastHeight = m_dHeight;
	m_dLastWidth = m_dWidth;
	m_iLastHeightUnits = m_iHeightUnits;
	m_iLastWidthUnits = m_iWidthUnits;
}

void CPictureSizeDlg::OnDestroy() 
{
   UpdateData(TRUE);

	CDialog::OnDestroy();
	
	m_bOldPorportions = m_bPorportions;
	m_iOldHeightUnits = m_iHeightUnits;
	m_iOldWidthUnits = m_iWidthUnits;
}

void CPictureSizeDlg::OnApply() 
{
   if(!UpdateData(TRUE))
      return;
	Apply();

   m_dOriginalPicVecWidth = m_dPicVecWidth;
   m_dOriginalPicVecHeight = m_dPicVecHeight;
   FillWidthAndHeight();

}

void CPictureSizeDlg::OnOK() 
{
   if(!UpdateData(TRUE))
      return;
	Apply();
	
	CDialog::OnOK();
}

void CPictureSizeDlg::FillWidthAndHeight()
{
   switch(m_iHeightUnits)
   {
   case UNITS_CM:
	   m_dHeight = m_dPicVecHeight / (VEC_UNITS_PER_INCH / CM_IN_INCH);
      break;
   case UNITS_INCH:
	   m_dHeight = m_dPicVecHeight / VEC_UNITS_PER_INCH;
      break;
   case UNITS_PERCENT:
	   m_dHeight = (m_dPicVecHeight / m_dOriginalPicVecHeight) * 100;
      break;
   }

   switch(m_iWidthUnits)
   {
   case UNITS_CM:
	   m_dWidth = m_dPicVecWidth / (VEC_UNITS_PER_INCH / CM_IN_INCH);
      break;
   case UNITS_INCH:
	   m_dWidth = m_dPicVecWidth / VEC_UNITS_PER_INCH;
      break;
   case UNITS_PERCENT:
	   m_dWidth = (m_dPicVecWidth / m_dOriginalPicVecWidth) * 100;
      break;
   }

   UpdateData(FALSE);
}

void CPictureSizeDlg::OnChangeHeight() 
{
   UpdateData();
   UpdateHeight();
   if(m_bPorportions)
   {
      m_dPicVecWidth = m_dPicVecHeight * (m_dOriginalPicVecWidth / m_dOriginalPicVecHeight);
      FillWidthAndHeight();
   }
}

void CPictureSizeDlg::OnChangeWidth() 
{
   UpdateData();
   UpdateWidth();
   if(m_bPorportions)
   {
      m_dPicVecHeight = m_dPicVecWidth * (m_dOriginalPicVecHeight / m_dOriginalPicVecWidth);
      FillWidthAndHeight();
   }
}

void CPictureSizeDlg::OnProportions() 
{
   UpdateData();
   if(m_bPorportions)
   {
      m_dPicVecHeight = m_dOriginalPicVecHeight;
      m_dPicVecWidth = m_dOriginalPicVecWidth;
   }
	FillWidthAndHeight();
}

void CPictureSizeDlg::OnSelchangeHeightUnits() 
{
   UpdateData();
   FillWidthAndHeight();
}

void CPictureSizeDlg::OnSelchangeWidthUnits() 
{
   UpdateData();
   FillWidthAndHeight();
}


void CPictureSizeDlg::UpdateHeight()
{
   switch(m_iHeightUnits)
   {
   case UNITS_CM:
      m_dPicVecHeight = m_dHeight * (VEC_UNITS_PER_INCH / CM_IN_INCH);
      break;
   case UNITS_INCH:
      m_dPicVecHeight = m_dHeight * VEC_UNITS_PER_INCH;
      break;
   case UNITS_PERCENT:
      m_dPicVecHeight = (m_dOriginalPicVecHeight) * (m_dHeight / 100);
      break;
   }
}

void CPictureSizeDlg::UpdateWidth()
{
   switch(m_iWidthUnits)
   {
   case UNITS_CM:
      m_dPicVecWidth = m_dWidth * (VEC_UNITS_PER_INCH / CM_IN_INCH);
      break;
   case UNITS_INCH:
      m_dPicVecWidth = m_dWidth * VEC_UNITS_PER_INCH;
      break;
   case UNITS_PERCENT:
      m_dPicVecWidth = (m_dOriginalPicVecWidth) * (m_dWidth / 100);
      break;
   }
}

