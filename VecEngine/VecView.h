/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

#if !defined(AFX_VECVIEW_H__307BDA26_C1A4_11D3_A49D_00C0CA1261A6__INCLUDED_)
#define AFX_VECVIEW_H__307BDA26_C1A4_11D3_A49D_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VecView.h : header file
//

#define VEC_CLIPBOARD_FORMAT "Monfort Vector Object"

/////////////////////////////////////////////////////////////////////////////
// CVecView view
#include "afxole.h" 

#include "VecContainer.h"
#include "VecLine.h"
#include "VecRectangle.h"
#include "VecPolyline.h"
#include "VecPolygon.h"
#include "VecEllipse.h"
#include "VecRectanglePic.h"
#include "VecEllipsePic.h"
#include "VecPolygonPic.h"
#include "VecRectangleText.h"
#include "VecEllipseText.h"
#include "VecPolygonText.h"
#include "VecFrameContainer.h"

#include "VecController.h"
#include "MRulerOrganizer.h"

enum ZOOM_MODE
{
   ZM_NORMAL = 0,
   ZM_FIT2WINDOW,
   ZM_FIT2WIDTH,
   ZM_FIT2HEIGHT,
   ZM_STRECH2WINDOW,
   ZM_FACTOR
};

enum RULLERS_UNITS
{
   RU_INCH = 0,
   RU_CM,
   RU_MM,
};

#define FIRST_VIEW_TOOL LAST_CHEANGE_TOOL
enum VIEW_TOOLS
{
   TOOL_ZOOM = FIRST_VIEW_TOOL,
   TOOL_PAN,
   LAST_VIEW_TOOL
};

class CVecView : public CScrollView
{
public:
   CVecView();           // protected constructor used by dynamic creation
	virtual ~CVecView();
protected:
	DECLARE_DYNCREATE(CVecView)

   CVecContainer m_VecContainer;
   CVecController m_VecController;
   CVecWorkspace m_VecWorkspace;
   UINT m_uiZoomMode;
   double m_dZoomFactor;

   virtual void UpdateZoomAndScroll();
   virtual void SetContainerOffsetBeforeDraw();
   void TrackPan(CPoint point);


   HCURSOR m_hZoomInCursor;
   HCURSOR m_hZoomOutCursor;
   HCURSOR m_hPanCursor;
   HCURSOR m_hPanActCursor;

   COLORREF m_ForeColor;
   COLORREF m_BackColor;
   CDRect m_Rect;
   CDPoint m_Offset;
   CDPoint m_Scaler;
protected:
   static HGLOBAL CreateArtObject(CPtrList &Items);
   static CDRect ReadArtObject(HGLOBAL hBuffer, CPtrList& Items);
   COleDataSource* BuildOleDataSource(CPtrList &Items);
   UINT m_uiClipboardFormat;

   void UpdateRulerBars();
	CMRulerOrganizer m_ruler;
   double m_dVecUnitsPerInch;

public:
   inline void SetForeColor(COLORREF col){m_ForeColor = col;}
   inline COLORREF GetForeColor(){return m_ForeColor;}
   inline void SetBackColor(COLORREF col){m_BackColor = col;}
   inline COLORREF GetBackColor(){return m_BackColor;}
   virtual void SetRect(CDRect &rect){m_Rect = rect;}
   inline CDRect GetRect(){return m_Rect;}
   inline CDRect GetRectByScreen(){CDRect t_Rect(m_Rect); t_Rect*=GetScaler(); t_Rect+=GetOffset(); return t_Rect;}
   virtual void SetScaler(CDPoint &point){m_Scaler = point;}
   inline CDPoint GetScaler(){return m_Scaler;}
   virtual void SetOffset(CDPoint &point){m_Offset = point;}
   inline CDPoint GetOffset(){return m_Offset;}

   BOOL m_bShowHorz;
	BOOL m_bShowVert;
	int m_nUnit;
	UINT m_nSize;


// Attributes
public:
   inline UINT GetZoomMode(){return m_uiZoomMode;}
   virtual void SetZoomMode(UINT uiZoomMode);
   inline double GetZoomFactor(){return m_dZoomFactor;}
   virtual void SetZoomFactor(double dZoomFactor);

// Operations
public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVecView)
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void DrawBackground(CDC* pDC);      // overridden to draw this view
   void ScrollToPositionWithoutScrollWindow(POINT pt);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
   
	afx_msg LRESULT OnRullerMouseDown(WPARAM wParam, LPARAM lParam);

	// Generated message map functions
	//{{AFX_MSG(CVecView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VECVIEW_H__307BDA26_C1A4_11D3_A49D_00C0CA1261A6__INCLUDED_)
