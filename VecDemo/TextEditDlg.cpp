// TextEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VecDemo.h"
#include "TextEditDlg.h"
#include "VecDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextEditDlg dialog


CTextEditDlg::CTextEditDlg(CVecDemoView* pVecTstView)
	: CDialog(CTextEditDlg::IDD, NULL)
{
   m_pVecTstView = pVecTstView;
	//{{AFX_DATA_INIT(CTextEditDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
   m_bFirstApply = FALSE;
}


void CTextEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextEditDlg)
	DDX_Control(pDX, IDC_EDIT1, m_EditCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTextEditDlg, CDialog)
	//{{AFX_MSG_MAP(CTextEditDlg)
	ON_BN_CLICKED(IDAPPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextEditDlg message handlers

void CTextEditDlg::OnApply() 
{
   if(!UpdateData(TRUE))
      return;
   Apply();
}

void CTextEditDlg::OnOK() 
{
   if(!UpdateData(TRUE))
      return;
   Apply();
	
	CDialog::OnOK();
}

BOOL CTextEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
/*
	m_LayoutManager.Attach(this);
	
	m_LayoutManager.SetConstraint(IDC_EDIT1, OX_LMS_TOP,  OX_LMT_SAME, 8);
	m_LayoutManager.SetConstraint(IDC_EDIT1, OX_LMS_BOTTOM, OX_LMT_OPPOSITE, -8, IDOK);
	m_LayoutManager.SetConstraint(IDC_EDIT1, OX_LMS_LEFT,  OX_LMT_SAME, 8);
	m_LayoutManager.SetConstraint(IDC_EDIT1, OX_LMS_RIGHT, OX_LMT_SAME, -8);

   m_LayoutManager.SetConstraint(IDCANCEL, OX_LMS_BOTTOM, OX_LMT_SAME, -8);
	m_LayoutManager.SetConstraint(IDCANCEL, OX_LMS_RIGHT, OX_LMT_SAME, -8);
	
   m_LayoutManager.SetConstraint(IDAPPLY, OX_LMS_BOTTOM, OX_LMT_SAME, -8);
	m_LayoutManager.SetConstraint(IDAPPLY, OX_LMS_RIGHT, OX_LMT_OPPOSITE, -8, IDCANCEL);

	m_LayoutManager.SetConstraint(IDOK, OX_LMS_BOTTOM, OX_LMT_SAME, -8);
	m_LayoutManager.SetConstraint(IDOK, OX_LMS_RIGHT, OX_LMT_OPPOSITE, -8, IDAPPLY);

   m_LayoutManager.SetMinMax(IDC_EDIT1, CSize(153,40));
	// Draw the layout with the new constraints
	// This is necessary when constraints are implemented and the window must be refreshed
	m_LayoutManager.RedrawLayout();
*/	

   CPtrList SelItems;
   m_pVecTstView->m_VecContainer.GetSelectedItems(SelItems);
   POSITION pos = SelItems.GetHeadPosition();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);;
      if(pVec->HasText())
      {
         CString sText = (dynamic_cast<CVecTextBase*>(pVec))->GetText();
         m_EditCtrl.SetWindowText(sText);
         break;//initiate the dialog with the data of the first text item
      }
   }
   
   
   return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTextEditDlg::Apply() 
{
   CString sText;
   m_EditCtrl.GetWindowText(sText);
   if(m_bFirstApply && m_strText.Compare(sText)==0)
      return;
   m_strText = sText;
   m_bFirstApply = TRUE;

   CPtrList SelItems;
   m_pVecTstView->m_VecContainer.GetSelectedItems(SelItems);
   POSITION pos = SelItems.GetHeadPosition();
   m_pVecTstView->m_VecController.m_UndoManager.CreateNewStep();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);;
      if(pVec->HasText())
      {
         m_pVecTstView->m_VecController.m_UndoManager.AddActionToLastStep(pVec, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
         (dynamic_cast<CVecTextBase*>(pVec))->SetText(sText);
      }
   }
   m_pVecTstView->m_VecContainer.Repaint();
   m_pVecTstView->m_VecController.SetModified();
}
