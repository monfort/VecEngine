/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// ==========================================================================
//				Class Implementation : CMRulerOrganizer
// ==========================================================================

// Copyright © Dundas Software Ltd. 1997-1999, All Rights Reserved                      

// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MRulerOrganizer.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif


double CMRulerBar::PIXEL_PAD=14.0;

CMRulerBar::CMRulerBar() :
	m_rectInvert(0,0,0,0), 
	m_rectOldInvert(0,0,0,0)
{
	m_bHorizontal=TRUE;
	m_dPos=0;
   m_dOffset=0;
	m_dZoom=1;
	m_nCalibrate=100;
	m_bOldIsHilited=FALSE;
	m_dDisplayDpi=100.0;
	m_bUseNonDefaultDpi=FALSE;
}

CMRulerBar::~CMRulerBar()
{
}

BOOL CMRulerBar::Init(BOOL bHorizontal /*=TRUE*/, UINT nCalibrate /*=100*/)
{
	m_nCalibrate=nCalibrate;
	m_bHorizontal=bHorizontal;
	
	// for 1, 2, ...
	// font for horizontal bar
    VERIFY(m_font.CreateFont(-12,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,ANSI_CHARSET,
		0,0,0,0,_T("Times New Roman")));
	// font for vertical bar
    VERIFY(m_fontVert.CreateFont(-12,0,900,0,FW_NORMAL,FALSE,FALSE,FALSE,ANSI_CHARSET,
		0,0,0,0,_T("Times New Roman")));

	return TRUE;
}

BEGIN_MESSAGE_MAP(CMRulerBar, CWnd)
	//{{AFX_MSG_MAP(CMRulerBar)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMRulerBar implementation

BOOL CMRulerBar::Create(CWnd* pParentWnd, BOOL bHorizontal, CRect rect, 
						 DWORD dwStyle/*=WS_CHILD|WS_VISIBLE*/, 
						 UINT nCalibrate/*=100*/, 
						 UINT nID/*=IDC_STATIC*/)
{
	ASSERT(pParentWnd!=NULL);

	WNDCLASS wndClass;
	wndClass.style=CS_DBLCLKS; 
    wndClass.lpfnWndProc=AfxWndProc; 
    wndClass.cbClsExtra=0; 
    wndClass.cbWndExtra=0; 
    wndClass.hInstance=AfxGetInstanceHandle(); 
    wndClass.hIcon=::LoadCursor(NULL,IDC_ARROW); 
    wndClass.hCursor=0; 
    wndClass.hbrBackground=(HBRUSH)(COLOR_WINDOW+1); 
    wndClass.lpszMenuName=NULL; 
	wndClass.lpszClassName=_T("RulerBarWnd");
	
	if(!AfxRegisterClass(&wndClass))
	{
		return FALSE;
	}

	if(!CWnd::Create(wndClass.lpszClassName,NULL,dwStyle,rect,pParentWnd,nID,NULL))
	{
		return FALSE;
	}

	return Init(bHorizontal,nCalibrate);
}

	
void CMRulerBar::DrawTick(CDC* pDC, CRect& rect, double dOffset, BOOL bMidTick)
{
	int nTickX, nTickY;
	int nTickWidth, nTickHeight;

	if(dOffset < 0)
		return;

	if(m_bHorizontal)
	{
		if(bMidTick)
			nTickHeight=rect.Height()/2;
		else
			nTickHeight=rect.Height()/4;

		nTickWidth=0;

		nTickX=(int)floor(rect.left+dOffset);
		nTickY=rect.top+(rect.Height()-nTickHeight+1)/2;
	}
	else
	{
		if(bMidTick)
			nTickWidth=rect.Width()/2;
		else
			nTickWidth=rect.Width()/4;

		nTickHeight=0;
		nTickX=rect.left+(rect.Width()-nTickWidth+1)/2;
		nTickY=(int)floor(rect.top+dOffset);
	}

	CPoint pt(nTickX, nTickY);
	CPoint ptTo(nTickX+nTickWidth, nTickY+nTickHeight);
	if(rect.PtInRect(pt) && rect.PtInRect(ptTo))
	{
		pDC->MoveTo(nTickX, nTickY);
		pDC->LineTo(nTickX+nTickWidth, nTickY+nTickHeight);
	}
}

void CMRulerBar::DrawScale(CDC* pDC, CRect rect)
{
	CBrush bkgndBrush(::GetSysColor(COLOR_WINDOW));
	pDC->FillRect(rect,&bkgndBrush);
	pDC->DrawEdge(rect,EDGE_SUNKEN,BF_RECT);
	rect.DeflateRect(2,2);

	// Clip all output to the rectangle
	pDC->IntersectClipRect(rect);

    CFont* oldFont=NULL;
	if(m_bHorizontal)
		pDC->SelectObject(&m_font);
	else
		pDC->SelectObject(&m_fontVert);
	COLORREF crOldText=pDC->SetTextColor(::GetSysColor(COLOR_BTNTEXT));
	CPen pen(PS_SOLID,1,::GetSysColor(COLOR_BTNTEXT));
	CPen* oldPen=pDC->SelectObject(&pen);
    int oldMode=pDC->SetBkMode(TRANSPARENT);

	double dSpacer=m_dZoom*(0.1*100);
	int nTicksPerUnit=(100!=m_nCalibrate) ? 4 : 10;
	double dDPI=(m_bUseNonDefaultDpi) ? m_dDisplayDpi : 100.0;
	double dPixelsPerUnit=m_dZoom*(100.0*100)/m_nCalibrate*dDPI/100.0;
	double dTickSpace=dPixelsPerUnit/nTicksPerUnit;
	double dOrg=nTicksPerUnit-fmod(m_dPos-m_dOffset, dSpacer);
	int nSpan=(m_bHorizontal) ? rect.Width() : rect.Height();

 	double dSpanEnd=nSpan+2*dSpacer+dPixelsPerUnit*2;
	int nUnitNumber=(int)((m_dPos-m_dOffset)/dPixelsPerUnit);

	int nXErr=0;
	int nYErr=0;
   nUnitNumber--;

	for(double d=dOrg; d<dSpanEnd; d+=dPixelsPerUnit, nUnitNumber++)
	{
		double dPos=-fmod(m_dPos-m_dOffset,dPixelsPerUnit)-dOrg-dPixelsPerUnit;

		// If the space between ticks is greater than PIXEL_PAD/2.0, 
		// draw all of the ticks
		if(dTickSpace>PIXEL_PAD/2.0)
		{
			for(int nIndex=1; nIndex<nTicksPerUnit; nIndex++)
			{	
  				double dOffset=d+dPos+0.5+dTickSpace*nIndex;
				DrawTick(pDC,rect,dOffset,nIndex==nTicksPerUnit/2);
			}
		}
		// Just draw the mid tick
		else if(dPixelsPerUnit>=PIXEL_PAD*2)
		{
			double dOffset=d+dPos+0.5+dTickSpace*nTicksPerUnit/2;
			DrawTick(pDC,rect,dOffset,TRUE);
		}

		if(dPixelsPerUnit<PIXEL_PAD*2 && nUnitNumber%5)
			continue;

		// draw the unit number, starting at 1
		if(/*nUnitNumber>=0*/true)
		{
			//char nCh=(char)(48+(nUnitNumber%10)); // 47 is '0'
			// Keep things as ASCII, since Unicode numeric characters 
			// may be non-consective
			CString sCh/*(nCh)*/; 
         sCh.Format(_T("%d"),nUnitNumber);

			if(dPixelsPerUnit<PIXEL_PAD*2)
			{
				int nHundreds=nUnitNumber%100;
				sCh.Format(_T("%d"),nHundreds);
			}

			CSize sNum=pDC->GetOutputTextExtent(sCh);
			int nXPos;
			int nYPos;

			if(m_bHorizontal)
			{
				nXPos=(int)floor(rect.left+d+dPos+0.5-(sNum.cx/2.0));
				nYPos=rect.top+(rect.Height()-sNum.cy)/2;

				if(nXPos <= nXErr)
					continue;
				else
					nXErr=sNum.cx+nXPos;
			}
			else
			{
				nXPos=rect.left+(rect.Width()-sNum.cy)/2; 
				nYPos=(int)floor(rect.top+d+dPos+0.5+(sNum.cx/2.0));

				if(nYPos <= nYErr)
					continue;
				else
					nYErr=sNum.cy+nYPos;
			}

			pDC->TextOut(nXPos, nYPos, sCh);
		}

	}

	// Get rid of the clipping region
	pDC->SelectClipRgn(NULL,RGN_COPY);

    pDC->SetBkMode(oldMode);
	pDC->SelectObject(oldPen);
	pDC->SetTextColor(crOldText);
	pDC->SelectObject(oldFont);
}

void CMRulerBar::DrawRuler(CDC* pDC, CRect rect, CRect rectClip)
{
	ASSERT(pDC!=NULL);
	if(rect.IsRectEmpty() || rectClip.IsRectEmpty())
		return;

	pDC->IntersectClipRect(rectClip);
	DrawScale(pDC,rect);
	pDC->SelectClipRgn(NULL,RGN_COPY);

	if(m_bOldIsHilited)
	{
		CRect rectIntersect;
		rectIntersect.IntersectRect(m_rectOldInvert,rectClip);
		ReverseHilite(pDC,rectIntersect);
	}
}

void CMRulerBar::ReverseHilite(CDC* pDC, CRect rect)
{
	ASSERT(pDC!=NULL);
	pDC->PatBlt(rect.left,rect.top,rect.Width(),rect.Height(),PATINVERT);
}

void CMRulerBar::Hilite(CDC* pDC, CRect rect, int nStartPos, int nWidth/*=1*/)
{
	ASSERT(pDC!=NULL);

	BOOL bRemoveHilite=FALSE;
	m_rectInvert=rect;
	m_rectInvert.DeflateRect(2,2);

	if(m_bHorizontal)
	{
		m_rectInvert.left+=nStartPos;
		bRemoveHilite=((m_rectInvert.left<=2) || (m_rectInvert.left>=m_rectInvert.right));
		m_rectInvert.right=m_rectInvert.left+nWidth;
		if(m_rectInvert.right>rect.right-2)
			m_rectInvert.right=rect.right-2;
	}
	else
	{
		m_rectInvert.top+=nStartPos;
		bRemoveHilite=((m_rectInvert.top<=2) || (m_rectInvert.top>=m_rectInvert.bottom));
		m_rectInvert.bottom=m_rectInvert.top+nWidth;
		if(m_rectInvert.bottom>rect.bottom-2)
			m_rectInvert.bottom=rect.bottom-2;
	}

	m_rectInvert.NormalizeRect(); // for nWidth < 0

	if(bRemoveHilite)
	{
		if(m_bOldIsHilited)
		{
			CRect rectOldInvert=m_rectOldInvert;
			rectOldInvert.IntersectRect(rectOldInvert,rect);
			ReverseHilite(pDC,rectOldInvert);
		}
		m_rectOldInvert.SetRectEmpty();
		m_rectInvert.SetRectEmpty();
		m_bOldIsHilited=FALSE;
	}
	else
	{
		if(m_bOldIsHilited)
		{
			CRect rectIntersect;
			rectIntersect.IntersectRect(m_rectOldInvert,m_rectInvert);

			CRect rectOldInvert;
			rectOldInvert.SubtractRect(m_rectOldInvert,rectIntersect);
			rectOldInvert.IntersectRect(rectOldInvert,rect);
			ReverseHilite(pDC,rectOldInvert);
			
			CRect rectInvert;
			rectInvert.SubtractRect(m_rectInvert,rectIntersect);
			ReverseHilite(pDC,rectInvert);
		}
		else
			ReverseHilite(pDC,m_rectInvert);
		m_bOldIsHilited=TRUE;
		m_rectOldInvert=m_rectInvert;
	}
}


/////////////////////////////////////////////////////////////////////////////
// CMRulerBar message handlers

void CMRulerBar::Scroll(double dPos, BOOL bRedraw/*=TRUE*/)
{
	m_dPos=m_dZoom*dPos;
	if(bRedraw)
		Invalidate();
}

void CMRulerBar::Offset(double dPos, BOOL bRedraw/*=TRUE*/)
{
	m_dOffset=/*m_dZoom**/dPos;
	if(bRedraw)
		Invalidate();
}

void CMRulerBar::Zoom(double dZoom, BOOL bRedraw/*=TRUE*/)
{
	m_dZoom=dZoom;
	if(bRedraw)
		Invalidate();
}

void CMRulerBar::SetDpi(double dDpi, BOOL bRedraw/*=TRUE*/)
{
	m_dDisplayDpi=dDpi;
	m_bUseNonDefaultDpi=TRUE;
	if(bRedraw)
		Invalidate();
}

void CMRulerBar::UseDefaultDpi(BOOL bUseDefaultDpi, BOOL bRedraw/*=TRUE*/)
{
	m_bUseNonDefaultDpi=!bUseDefaultDpi;
	if(bRedraw)
		Invalidate();
}

void CMRulerBar::SetCalibrate(UINT nCalibrate, BOOL bRedraw/*=TRUE*/)
{
	m_nCalibrate=nCalibrate;
	if(bRedraw)
		Invalidate();
}

void CMRulerBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CRect rectClient;
	GetClientRect(rectClient);
	CRect rectClip;
	dc.GetClipBox(&rectClip); // invalidated region

	DrawRuler(&dc,rectClient,rectClip);
}


////////////////////////////////////////////////////////////////////////////////////


CMRulerOrganizer::CMRulerOrganizer()
	:
	m_bShowHorzRulerBar(FALSE),
	m_bShowVertRulerBar(FALSE),
	m_rectHorzRulerBar(0,0,0,0),
	m_rectVertRulerBar(0,0,0,0),
	m_rectBlank(0,0,0,0),
	m_nHorzRulerBarHeight(MRB_HORZHEIGHT),
	m_nVertRulerBarWidth(MRB_VERTWIDTH)
	//m_nTimerCheckScrollPos(NULL)
{
}


CMRulerOrganizer::~CMRulerOrganizer()
{
}

BOOL CMRulerOrganizer::Attach(CWnd* pWnd, BOOL bHorzRuler/*=TRUE*/, 
							   BOOL bVertRuler/*=TRUE*/)
{
	ASSERT(pWnd!=NULL);

	if(!::IsWindow(pWnd->GetSafeHwnd()))
	{
		TRACE(_T("CMRulerOrganizer::Attach: failed, invalid window handle has been specified\n"));
		return FALSE;
	}

	if(IsAttached())
	{
		TRACE(_T("CMRulerOrganizer::Attach: failed, object already has attached window. Call Detach() before!\n"));
		return FALSE;
	}

	HookWindow(pWnd);

	// create ruler bars
	ASSERT(!::IsWindow(m_horzRulerBar.GetSafeHwnd()) && 
		!::IsWindow(m_vertRulerBar.GetSafeHwnd()));
	CRect rect(0,0,0,0);
	if(!m_horzRulerBar.Create(pWnd,TRUE,rect,WS_CHILD) || 
		!m_vertRulerBar.Create(pWnd,FALSE,rect,WS_CHILD))
	{
		TRACE(_T("CMRulerOrganizer::Attach: failed to create the ruler bars\n"));
		Detach();
		return FALSE;
	}

	// create timer for checking scroll position
	/*m_nTimerCheckScrollPos=pWnd->SetTimer(IDT_MRBO_CHECK_SCROLLPOS,
		MRBO_CHECK_SCROLLPOS_DELAY,NULL);
	if(m_nTimerCheckScrollPos==NULL)
	{
		TRACE(_T("CMRulerOrganizer::Attach: failed to create timer\n"));
		Detach();
		return FALSE;
	}*/

	m_bShowHorzRulerBar=bHorzRuler;
	m_bShowVertRulerBar=bVertRuler;

	CalcLayout(TRUE);

	return TRUE;
}


void CMRulerOrganizer::Detach()
{
	// destroy ruler bars
	if(::IsWindow(m_horzRulerBar.GetSafeHwnd()))
		m_horzRulerBar.DestroyWindow();
	if(::IsWindow(m_vertRulerBar.GetSafeHwnd()))
		m_vertRulerBar.DestroyWindow();

	if(::IsWindow(m_pWndHooked->GetSafeHwnd()))
	{
		/*if(m_nTimerCheckScrollPos!=NULL)
			m_pWndHooked->KillTimer(m_nTimerCheckScrollPos);*/

		RedrawAttached(TRUE);
	}

	UnhookWindow();

	m_bShowHorzRulerBar=FALSE;
	m_bShowVertRulerBar=FALSE;

	//m_nTimerCheckScrollPos=NULL;
}


void CMRulerOrganizer::SetHorzRulerBarHeight(int nHorzRulerBarHeight)
{
	m_nHorzRulerBarHeight=nHorzRulerBarHeight;
	CalcLayout(TRUE);
}


void CMRulerOrganizer::SetVertRulerBarWidth(int nVertRulerBarWidth)
{
	m_nVertRulerBarWidth=nVertRulerBarWidth;
	CalcLayout(TRUE);
}


LRESULT CMRulerOrganizer::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#if defined(_AFXDLL) && defined(_WINDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#endif

	LRESULT result=0;

	switch(uMsg)
	{
	case WM_NCCALCSIZE:
		{
			result=CMHookWnd::WindowProc(uMsg,wParam,lParam);

			LPNCCALCSIZE_PARAMS lpncsp=(LPNCCALCSIZE_PARAMS)lParam;
			// add an offset
			if(GetShowVertRulerBar())
			{
				if(lpncsp->rgrc[0].right-lpncsp->rgrc[0].left>
					GetVertRulerBarWidth()+MRBO_BORDER)
				{
					lpncsp->rgrc[0].left+=GetVertRulerBarWidth()+MRBO_BORDER;
				}
				else
				{
					lpncsp->rgrc[0].left=lpncsp->rgrc[0].right;
				}
			}

			if(GetShowHorzRulerBar())
			{
				if(lpncsp->rgrc[0].bottom-lpncsp->rgrc[0].top>
					GetHorzRulerBarHeight()+MRBO_BORDER)
				{
					lpncsp->rgrc[0].top+=GetHorzRulerBarHeight()+MRBO_BORDER;
				}
				else
				{
					lpncsp->rgrc[0].top=lpncsp->rgrc[0].bottom;
				}
			}
			break;
		}

	case WM_NCPAINT:
		{
			result=CMHookWnd::WindowProc(uMsg,wParam,lParam);

			CWindowDC dc(m_pWndHooked);

			if(!m_rectBlank.IsRectEmpty())
				dc.FillSolidRect(m_rectBlank,::GetSysColor(COLOR_BTNFACE));

			if(GetShowHorzRulerBar())
			{
				m_horzRulerBar.DrawRuler(&dc,m_rectHorzRulerBar,m_rectHorzRulerBar);

				dc.MoveTo(m_rectHorzRulerBar.left+1,m_rectHorzRulerBar.bottom+1);
				dc.LineTo(m_rectHorzRulerBar.right+1,m_rectHorzRulerBar.bottom+1);
				CPen pen(PS_SOLID,1,::GetSysColor(COLOR_3DSHADOW));
				CPen* pOldPen=dc.SelectObject(&pen);
				dc.MoveTo(m_rectHorzRulerBar.left,m_rectHorzRulerBar.bottom);
				dc.LineTo(m_rectHorzRulerBar.right+1,m_rectHorzRulerBar.bottom);
				if(pOldPen!=NULL)
					dc.SelectObject(pOldPen);
			}
			if(GetShowVertRulerBar())
			{
				m_vertRulerBar.DrawRuler(&dc,m_rectVertRulerBar,m_rectVertRulerBar);

				dc.MoveTo(m_rectVertRulerBar.right+1,m_rectVertRulerBar.top+1);
				dc.LineTo(m_rectVertRulerBar.right+1,m_rectVertRulerBar.bottom+1);
				CPen pen(PS_SOLID,1,::GetSysColor(COLOR_3DSHADOW));
				CPen* pOldPen=dc.SelectObject(&pen);
				dc.MoveTo(m_rectVertRulerBar.right,m_rectVertRulerBar.top);
				dc.LineTo(m_rectVertRulerBar.right,m_rectVertRulerBar.bottom+1);
				if(pOldPen!=NULL)
					dc.SelectObject(pOldPen);
			}

			break;
		}

	case WM_SIZE:
		{
			result=CMHookWnd::WindowProc(uMsg,wParam,lParam);
			if(wParam==SIZE_MAXIMIZED || wParam==SIZE_RESTORED)
			{
				CalcLayout();
				/*Oran CheckScrollPos();
				RedrawAttached();*/
			}
			break;
		}

	case WM_HSCROLL:
		{
			result=CMHookWnd::WindowProc(uMsg,wParam,lParam);
			//Oran CheckScrollPos();
			break;
		}

	case WM_VSCROLL:
		{
			result=CMHookWnd::WindowProc(uMsg,wParam,lParam);
			//Oran CheckScrollPos();
			break;
		}

	case WM_MOUSEMOVE:
		{
			result=CMHookWnd::WindowProc(uMsg,wParam,lParam);

			if(GetShowHorzRulerBar())
			{
				HiliteRuler(LOWORD(lParam),TRUE,1);
			}

			if(GetShowVertRulerBar())
			{
				HiliteRuler(HIWORD(lParam),FALSE,1);
			}

			break;
		}

	case WM_TIMER:
		{
			/*if(wParam==(UINT)m_nTimerCheckScrollPos)
			{
				CheckScrollPos();
				return 0;
			}
			result=CMHookWnd::WindowProc(uMsg,wParam,lParam);*/
			break;
		}
   case WM_NCMOUSEMOVE:    
   case WM_NCLBUTTONDOWN:  
   case WM_NCLBUTTONUP:          
   case WM_NCLBUTTONDBLCLK:      
   case WM_NCRBUTTONDOWN:        
   case WM_NCRBUTTONUP:          
   case WM_NCRBUTTONDBLCLK:      
   case WM_NCMBUTTONDOWN:        
   case WM_NCMBUTTONUP:     
   case WM_NCMBUTTONDBLCLK:
      {
         POINT point = {LOWORD(lParam), HIWORD(lParam)};
         CRect WndRect;
         m_pWndHooked->GetWindowRect(&WndRect);
         point.x-=WndRect.left;
         point.y-=WndRect.top;

         int iWhere = -1;
         if(GetShowHorzRulerBar() && m_rectHorzRulerBar.PtInRect(point))
            iWhere = RULLER_HORIZONTAL;
         if(GetShowVertRulerBar() && m_rectVertRulerBar.PtInRect(point)) 
            iWhere = RULLER_VERTICAL;
         if(GetShowHorzRulerBar() && GetShowVertRulerBar() && m_rectBlank.PtInRect(point))
            iWhere = RULLER_BLANK;

         if(iWhere!=-1)
            m_pWndHooked->SendMessage(uMsg+WM_RULER_EVENTS, iWhere, lParam);
         else
			   result=CMHookWnd::WindowProc(uMsg,wParam,lParam);
			break;
		}
   case WM_NCHITTEST:
		{
			result=CMHookWnd::WindowProc(uMsg,wParam,lParam);
         POINT point = {LOWORD(lParam), HIWORD(lParam)};
         CRect WndRect;
         m_pWndHooked->GetWindowRect(&WndRect);
         point.x-=WndRect.left;
         point.y-=WndRect.top;
         if((GetShowHorzRulerBar() && m_rectHorzRulerBar.PtInRect(point))|| 
            (GetShowVertRulerBar() && m_rectVertRulerBar.PtInRect(point))|| 
            (GetShowHorzRulerBar() && GetShowVertRulerBar() && m_rectBlank.PtInRect(point)))
            result = HTBORDER;
			break;
		}
      
	default:
		{
			result=CMHookWnd::WindowProc(uMsg,wParam,lParam);
			break;
		}
	}

	return result;
}


void CMRulerOrganizer::CalcLayout(BOOL bRepositionBars/*=FALSE*/)
{
	m_rectHorzRulerBar.SetRectEmpty();
	m_rectVertRulerBar.SetRectEmpty();
	m_rectBlank.SetRectEmpty();

	if(!IsAttached())
		return;

	ASSERT(::IsWindow(m_pWndHooked->GetSafeHwnd()));

	CRect rectWindow;
	m_pWndHooked->GetWindowRect(rectWindow);
	m_pWndHooked->ScreenToClient(&rectWindow);
	CRect rect;
	m_pWndHooked->GetClientRect(rect);
	rect-=rectWindow.TopLeft();

	DWORD dwStyle=m_pWndHooked->GetStyle();
	if((dwStyle&WS_VSCROLL)==WS_VSCROLL)
		rect.right+=::GetSystemMetrics(SM_CXVSCROLL);
	if((dwStyle&WS_HSCROLL)==WS_HSCROLL)
		rect.bottom+=::GetSystemMetrics(SM_CYHSCROLL);
	if(GetShowVertRulerBar())
		rect.left-=GetVertRulerBarWidth()+MRBO_BORDER;
	if(GetShowHorzRulerBar())
		rect.top-=GetHorzRulerBarHeight()+MRBO_BORDER;


	if(GetShowHorzRulerBar() && GetShowVertRulerBar())
	{
		m_rectBlank=rect;
		m_rectBlank.right=m_rectBlank.left+GetVertRulerBarWidth();
		m_rectBlank.bottom=m_rectBlank.top+GetHorzRulerBarHeight();
	}

	if(GetShowHorzRulerBar())
	{
		m_rectHorzRulerBar.left=m_rectBlank.right;
		m_rectHorzRulerBar.right=rect.right;
		m_rectHorzRulerBar.top=rect.top;
		m_rectHorzRulerBar.bottom=m_rectHorzRulerBar.top+GetHorzRulerBarHeight();
	}

	if(GetShowVertRulerBar())
	{
		m_rectVertRulerBar.top=m_rectBlank.bottom;
		m_rectVertRulerBar.bottom=rect.bottom;
		m_rectVertRulerBar.left=rect.left;
		m_rectVertRulerBar.right=m_rectVertRulerBar.left+GetVertRulerBarWidth();
	}

	if(bRepositionBars)
		RedrawAttached(TRUE);
}


void CMRulerOrganizer::CheckScrollPos(BOOL bDoRedraw)
{
	BOOL bRedraw=FALSE;
	if(GetShowHorzRulerBar())
	{
		SCROLLINFO scrollInfo={ sizeof(SCROLLINFO) };
		if(m_pWndHooked->GetScrollInfo(SB_HORZ,&scrollInfo))
		{
			double dNewScrollPos=scrollInfo.nPos;

			if(dNewScrollPos!=0 && scrollInfo.nPage!=0)
			{
				CRect rectClient;
				m_pWndHooked->GetClientRect(rectClient);

				dNewScrollPos*=(rectClient.Width()/scrollInfo.nPage);
			}

			double dOldScrollPos = m_horzRulerBar.GetCurrentPos();
			dNewScrollPos=dNewScrollPos/m_horzRulerBar.GetZoomLevel();
			m_horzRulerBar.Scroll(dNewScrollPos,FALSE);
			if(dOldScrollPos!=m_horzRulerBar.GetCurrentPos())
			{
				bRedraw=TRUE;
			}
		}
	}

	if(GetShowVertRulerBar())
	{
		SCROLLINFO scrollInfo={ sizeof(SCROLLINFO) };
		if(m_pWndHooked->GetScrollInfo(SB_VERT,&scrollInfo))
		{
			double dNewScrollPos = scrollInfo.nPos;

			if(dNewScrollPos!=0 && scrollInfo.nPage!=0)
			{
				CRect rectClient;
				m_pWndHooked->GetClientRect(rectClient);

				dNewScrollPos*=(rectClient.Height()/scrollInfo.nPage);
			}

			double dOldScrollPos=m_vertRulerBar.GetCurrentPos();
			dNewScrollPos = dNewScrollPos/m_vertRulerBar.GetZoomLevel();
			m_vertRulerBar.Scroll(dNewScrollPos,FALSE);
			if(dOldScrollPos!=m_vertRulerBar.GetCurrentPos())
			{
				bRedraw=TRUE;
			}
		}
	}

	if(bRedraw&&bDoRedraw)
	{
		RedrawAttached();
		//TRACE(_T("RedrawAttached\n"));
	}
}	


