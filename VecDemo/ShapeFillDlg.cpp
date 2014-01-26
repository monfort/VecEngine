// ShapeFillDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VecDemo.h"
#include "ShapeFillDlg.h"
#include "VecDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShapeFillDlg dialog


CShapeFillDlg::CShapeFillDlg(CVecDemoView* pVecTstView, BOOL bIsPatternEnabled)
	: CDialog(CShapeFillDlg::IDD, NULL)
{
   m_pVecTstView = pVecTstView;
   m_bIsPatternEnabled = bIsPatternEnabled;
	//{{AFX_DATA_INIT(CShapeFillDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
   

   m_iLastFillMode = -2;
   m_iLastPattern = -2;
}


void CShapeFillDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShapeFillDlg)
	DDX_Control(pDX, IDC_FILL_MODE, m_lbFillMode);
	DDX_Control(pDX, IDC_FILL_PATTERN, m_lbFillPattern);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShapeFillDlg, CDialog)
	//{{AFX_MSG_MAP(CShapeFillDlg)
	ON_BN_CLICKED(IDAPPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShapeFillDlg message handlers

void CShapeFillDlg::OnApply() 
{
   if(!UpdateData(TRUE))
      return;
	Apply();
}

void CShapeFillDlg::OnOK() 
{
   if(!UpdateData(TRUE))
      return;
	Apply();
	
	CDialog::OnOK();
}

BOOL CShapeFillDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   AddItemToModeList(_T("Transparent"),0);
   AddItemToModeList(_T("Translucent"),1);
   AddItemToModeList(_T("Opaque"),2);
	
   if(!m_bIsPatternEnabled)
   {
      m_lbFillPattern.EnableWindow(FALSE);
   }
   else
   {
      AddItemToPatList(_T("Solid"),0);
      AddItemToPatList(_T("Horizontal"),5);
      AddItemToPatList(_T("Vertical"),6);
      AddItemToPatList(_T("Forward diagonal"),4);
      AddItemToPatList(_T("Backward diagonal"),1);
      AddItemToPatList(_T("Cross"),2);
      AddItemToPatList(_T("Diagonal cross"),3);
   }
   
   CPtrList SelItems;
   m_pVecTstView->m_VecContainer.GetSelectedItems(SelItems);
   POSITION pos = SelItems.GetHeadPosition();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);;

      int iFillMode = 0;
      switch(pVec->GetFillMode())
      {
      case TRANSPARENT:
         iFillMode = 0;
         break;
      case OPAQUE:
         iFillMode = 2;
         if(pVec->GetRop2()!=R2_COPYPEN)
            iFillMode = 1;
         break;
      }
      m_lbFillMode.SetCurSel(iFillMode);

      if(m_bIsPatternEnabled)
      {
         if(pVec->IsCloseShape())
         {
            m_lbFillPattern.SetCurSel(pVec->GetFillStyle()+1);

            UpdateData(FALSE);
            break;//initiate the dialog with the data of the first text item
         }
      }
   }

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CShapeFillDlg::AddItemToModeList(LPCTSTR lpszText, int nImageIndex)
{
	int nIndex = m_lbFillMode.AddString(lpszText);
	ASSERT(nIndex != LB_ERR);
	return nIndex;
}

int CShapeFillDlg::AddItemToPatList(LPCTSTR lpszText, int nImageIndex)
{
	int nIndex = m_lbFillPattern.AddString(lpszText);
	ASSERT(nIndex != LB_ERR);
	return nIndex;
}

void CShapeFillDlg::Apply() 
{
   if(m_iLastFillMode == m_lbFillMode.GetCurSel() && m_iLastPattern == m_lbFillPattern.GetCurSel())
      return;

   int iFillMode = 0;
   int iRop2 = 0;
   switch(m_lbFillMode.GetCurSel())
   {
   case 0:
      iFillMode = TRANSPARENT;
      iRop2 = R2_COPYPEN;
      break;
   case 1:
      iFillMode = OPAQUE;
      iRop2 = R2_MASKPEN;
      break;
   case 2:
      iFillMode = OPAQUE;
      iRop2 = R2_COPYPEN;
      break;
   }

   CPtrList SelItems;
   m_pVecTstView->m_VecContainer.GetSelectedItems(SelItems);
   POSITION pos = SelItems.GetHeadPosition();
   m_pVecTstView->m_VecController.m_UndoManager.CreateNewStep();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);;
      m_pVecTstView->m_VecController.m_UndoManager.AddActionToLastStep(pVec, UAT_RESTORE_OB_FROM_LIGHT_DUMP);

      pVec->SetFillMode(iFillMode);
      pVec->SetRop2(iRop2);

      if(m_bIsPatternEnabled)
      {
         if(pVec->IsCloseShape())
         {
            pVec->SetFillStyle(m_lbFillPattern.GetCurSel()-1);
         }
      }
   }
   m_pVecTstView->m_VecContainer.Repaint();
   m_pVecTstView->m_VecController.SetModified();

   m_iLastFillMode = m_lbFillMode.GetCurSel();
   m_iLastPattern = m_lbFillPattern.GetCurSel();

#ifdef CHEANGE_AUTO_DEFAULTS
#endif
}

