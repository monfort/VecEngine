// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "TestApp.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView,CVecView )
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_TEST_FREEHAND, OnTestFreehand)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
   /*CBitmapBaseEx bitmap;
   int err = bitmap.LoadBitmapFromFile("C:\\temp\\duck-green.bmp");*/
   
	CVecRectanglePic* pVec = new CVecRectanglePic;
   CDRect rect(100,100,200,200);
   pVec->SetRect(rect);
   m_VecContainer.Insert(pVec);
   /*pVec->SetBitmap(&bitmap);
   bitmap.Free();
   pVec->Rotate(RAD90D/2);*/
   
   
   m_VecContainer.Repaint();
   m_VecController.SetModified();
	
	CVecView ::OnLButtonDblClk(nFlags, point);
}

void CChildView::OnTestFreehand() 
{

	m_VecController.SetSelectedTool(TOOL_FREEHAND);

}



void CChildView::OnInitialUpdate() 
{
	CVecView ::OnInitialUpdate();
}
