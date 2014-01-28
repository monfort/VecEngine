/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

#include "stdafx.h"
// TextEditRect.cpp : implementation file
//
#include "resource.h"

#include "TextEditRect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextEditRect dialog

CTextEditRect::CTextEditRect(CWnd* pParent /*=NULL*/)
	: CDialog(CTextEditRect::IDD, pParent)
{
   m_pFont = NULL;
	//{{AFX_DATA_INIT(CTextEditRect)
	m_strText = _T("");
	//}}AFX_DATA_INIT
}


void CTextEditRect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextEditRect)
	DDX_Control(pDX, IDC_EDIT_TEXT_CTRL, m_EditCtrl);
	DDX_Text(pDX, IDC_EDIT_TEXT_CTRL, m_strText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTextEditRect, CDialog)
	//{{AFX_MSG_MAP(CTextEditRect)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_EN_KILLFOCUS(IDC_EDIT_TEXT_CTRL, OnKillfocusEdit1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextEditRect message handlers


BOOL CTextEditRect::OnInitDialog() 
{
   //enable the parent window so the use will be albe to click on it and then we 
   //will get the WM_KILL_FOCUS message ans close the dialog
   CWnd* pParent = GetParent();
   if(pParent!=NULL)
      pParent->EnableWindow(TRUE);
   //AfxGetApp()->EnableModeless(FALSE);

 	CDialog::OnInitDialog();

   if(m_pFont!=NULL)
      m_EditCtrl.SetFont(m_pFont);

   MoveWindow(&m_Rect);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CTextEditRect::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
   if(!!IsWindow(m_EditCtrl.m_hWnd))
   {
      CRect rect;
      GetClientRect(&rect);
      m_EditCtrl.MoveWindow(&rect);
   }
}



void CTextEditRect::OnDestroy() 
{
	CDialog::OnDestroy();
	
}


void CTextEditRect::OnLButtonDown(UINT nFlags, CPoint point) 
{
   ClientToScreen(&point);
   if(!m_Rect.PtInRect(point))
      OnOK();
}


void CTextEditRect::OnRButtonDown(UINT nFlags, CPoint point) 
{
   ClientToScreen(&point);
   if(!m_Rect.PtInRect(point))
      OnOK();
}


void CTextEditRect::OnKillfocusEdit1() 
{
   //close the dialog only if the application is currently active (someone move the focus inside the application)
   if(AfxGetMainWnd() == GetActiveWindow())
   	OnOK();
}


