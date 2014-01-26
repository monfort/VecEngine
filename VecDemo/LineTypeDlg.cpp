// LineTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VecDemo.h"
#include "LineTypeDlg.h"
#include "VecDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLineTypeDlg dialog


CLineTypeDlg::CLineTypeDlg(CVecDemoView* pVecTstView)
	: CDialog(CLineTypeDlg::IDD, NULL)
{
   pVecTstView = pVecTstView;
	//{{AFX_DATA_INIT(CLineTypeDlg)
	m_dLineWidth = 0.0;
	//}}AFX_DATA_INIT
   m_dLastSize = -2;
   m_iLastStyle = -2;
}


void CLineTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLineTypeDlg)
	DDX_Control(pDX, IDC_LINE_STYLE, m_lbLineStyle);
	DDX_Text(pDX, IDC_LINE_WIDTH, m_dLineWidth);
	DDV_MinMaxDouble(pDX, m_dLineWidth, 0., 1000.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLineTypeDlg, CDialog)
	//{{AFX_MSG_MAP(CLineTypeDlg)
	ON_BN_CLICKED(IDAPPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLineTypeDlg message handlers

BOOL CLineTypeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//m_imageList.Create(IDB_LINE_STYLE,48,0,RGB(192,192,192));
	//m_lbLineStyle.SetImageList(&m_imageList);

   AddItemToListBox(_T("Solid"),0);
   AddItemToListBox(_T("Dashed"),1);
   AddItemToListBox(_T("Doted"),2);
   AddItemToListBox(_T("Dash Dot"),3);
   AddItemToListBox(_T("Dash Dot Dot"),4);
   AddItemToListBox(_T("No Line"),5);
	
   CPtrList SelItems;
   m_pVecTstView->m_VecContainer.GetSelectedItems(SelItems);
   POSITION pos = SelItems.GetHeadPosition();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);;
      m_dLineWidth = pVec->GetLineWidth();
      m_lbLineStyle.SetCurSel(pVec->GetLineStyle());
      UpdateData(FALSE);
      break;//initiate the dialog with the data of the first text item
   }

   return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CLineTypeDlg::AddItemToListBox(LPCTSTR lpszText, int nImageIndex)
{
	int nIndex = m_lbLineStyle.AddString(lpszText);
	ASSERT(nIndex != LB_ERR);
//	m_lbLineStyle.SetItemImageIndex(nIndex, nImageIndex);
	return nIndex;
}

void CLineTypeDlg::OnApply() 
{
   if(!UpdateData(TRUE))
      return;
   Apply();
}

void CLineTypeDlg::Apply() 
{
   if(m_dLastSize == m_dLineWidth && m_iLastStyle == m_lbLineStyle.GetCurSel())
      return;

   CPtrList SelItems;
   m_pVecTstView->m_VecContainer.GetSelectedItems(SelItems);
   POSITION pos = SelItems.GetHeadPosition();
   m_pVecTstView->m_VecController.m_UndoManager.CreateNewStep();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);;
      m_pVecTstView->m_VecController.m_UndoManager.AddActionToLastStep(pVec, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
      pVec->SetLineWidth(m_dLineWidth);
      pVec->SetLineStyle(m_lbLineStyle.GetCurSel());
   }
   m_pVecTstView->m_VecContainer.Repaint();
   m_pVecTstView->m_VecController.SetModified();

   m_dLastSize = m_dLineWidth;
   m_iLastStyle = m_lbLineStyle.GetCurSel();

}

void CLineTypeDlg::OnOK() 
{
   if(!UpdateData(TRUE))
      return;
	Apply();
	
	CDialog::OnOK();
}
