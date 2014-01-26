// RotateFreeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VecDemo.h"
#include "RotateFreeDlg.h"
#include "VecDemoView.h"
#include "G_Routines.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRotateFreeDlg dialog


CRotateFreeDlg::CRotateFreeDlg(CVecDemoView* pVecTstView)
	: CDialog(CRotateFreeDlg::IDD, NULL)
{
   m_pVecTstView = pVecTstView;
	//{{AFX_DATA_INIT(CRotateFreeDlg)
	m_dAngle = 0.0;
	//}}AFX_DATA_INIT
}


void CRotateFreeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRotateFreeDlg)
	DDX_Control(pDX, IDC_EDIT2, m_edtAngle);
	DDX_Control(pDX, IDC_SPIN1, m_SpinCtrl);
	DDX_Text(pDX, IDC_EDIT2, m_dAngle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRotateFreeDlg, CDialog)
	//{{AFX_MSG_MAP(CRotateFreeDlg)
	ON_BN_CLICKED(IDAPPLY, OnApply)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeEdit2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRotateFreeDlg message handlers

void CRotateFreeDlg::OnApply() 
{
   if(!UpdateData(TRUE))
      return;
	Apply();
}

void CRotateFreeDlg::OnOK() 
{
   if(!UpdateData(TRUE))
      return;
	Apply();
	
	CDialog::OnOK();
}

BOOL CRotateFreeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_SpinCtrl.SetBuddy(&m_edtAngle);
	m_SpinCtrl.SetRange(-360, 360);

   return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRotateFreeDlg::Apply()
{
   if(m_dAngle==0)
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
      pVec->Rotate(DegToRad(m_dAngle), NULL);
   }
   m_pVecTstView->m_VecContainer.Repaint();
   m_pVecTstView->m_VecController.SetModified();
}



void CRotateFreeDlg::OnChangeEdit2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
}
