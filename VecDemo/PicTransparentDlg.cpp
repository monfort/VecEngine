// PicTransparentDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VecDemo.h"
#include "PicTransparentDlg.h"
#include "VecDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPicTransparentDlg dialog


CPicTransparentDlg::CPicTransparentDlg(CVecDemoView* pVecTstView)
	: CDialog(CPicTransparentDlg::IDD, NULL)
{
   m_pVecTstView = pVecTstView;
	//{{AFX_DATA_INIT(CPicTransparentDlg)
	m_bIsTransparent = FALSE;
	//}}AFX_DATA_INIT
}


void CPicTransparentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPicTransparentDlg)
	DDX_Control(pDX, IDC_COLOR_BUTTON, m_ColorPickerButton);
	DDX_Check(pDX, IDC_IS_TRANSPARENT, m_bIsTransparent);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPicTransparentDlg, CDialog)
	//{{AFX_MSG_MAP(CPicTransparentDlg)
	ON_BN_CLICKED(IDC_IS_TRANSPARENT, OnIsTransparent)
	ON_BN_CLICKED(IDAPPLY, OnApply)
	ON_BN_CLICKED(IDC_COLOR_BUTTON, OnColorButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPicTransparentDlg message handlers

void CPicTransparentDlg::OnOK() 
{
   if(!UpdateData(TRUE))
      return;
	Apply();
	
	CDialog::OnOK();
}

void CPicTransparentDlg::OnIsTransparent() 
{
   if(!UpdateData(TRUE))
      return;
}

void CPicTransparentDlg::OnApply() 
{
   if(!UpdateData(TRUE))
      return;
	Apply();
}

BOOL CPicTransparentDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strColorHex;
   CPtrList SelItems;
   m_pVecTstView->GetSelectedItems(SelItems);
   POSITION pos = SelItems.GetHeadPosition();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);
      CVecBitmapBase* pBitmapBase = dynamic_cast<CVecBitmapBase*>(pVec);
      if(pBitmapBase!=NULL)
      {
         m_bIsTransparent = pBitmapBase->IsTransparent();

		  m_clrTransparent = pVec->GetShadowColor();
		  strColorHex.Format("Color: #%06X",m_clrTransparent);
		  m_ColorPickerButton.SetWindowText(strColorHex);

         break;
      }
   }

   UpdateData(FALSE);
   OnIsTransparent();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPicTransparentDlg::Apply()
{
   CPtrList SelItems;
   m_pVecTstView->GetSelectedItems(SelItems);
   POSITION pos = SelItems.GetHeadPosition();
   m_pVecTstView->m_VecController.m_UndoManager.CreateNewStep();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);
      CVecBitmapBase* pBitmapBase = dynamic_cast<CVecBitmapBase*>(pVec);
      if(pBitmapBase!=NULL)
      {
         m_pVecTstView->m_VecController.m_UndoManager.AddActionToLastStep(pVec, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
         pBitmapBase->SetTransparent(m_bIsTransparent);

		pVec->SetShadowColor(m_clrTransparent);


      }
   }
   m_pVecTstView->m_VecContainer.Repaint();
   m_pVecTstView->m_VecController.SetModified();
}

void CPicTransparentDlg::OnColorButton() 
{
	CColorDialog dlg(m_clrTransparent, CC_FULLOPEN);
   
	if(dlg.DoModal()==IDOK)
   {
		m_clrTransparent = dlg.GetColor();

	    CString strColorHex;
		strColorHex.Format("Color: #%06X",m_clrTransparent);
		m_ColorPickerButton.SetWindowText(strColorHex);
   }	
}
