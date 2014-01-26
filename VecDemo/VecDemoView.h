// VecDemoView.h : interface of the CVecDemoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECDEMOVIEW_H__44AA35D0_538F_11D2_B46D_444553540000__INCLUDED_)
#define AFX_VECDEMOVIEW_H__44AA35D0_538F_11D2_B46D_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "VecView.h"

#define VEC_UNITS_PER_INCH 100
#define PICTURE_UNITS_PER_SRC_PIXELS 0.5
#define CM_IN_INCH 2.54


enum enVecTypes
{
   VECENT_NONE = -2,
   VECENT_MORE_THEN_ONE = -1,
   VECENT_PIC = 0,
   VECENT_TEXT,
   VECENT_LINE,
   VECENT_SHAPE,
   VECENT_GROUP
};

#define SUBMENU_NONE -1
enum ObjectMenuSubMenus
{
   OM_SUB_PIC = 0,
   OM_SUB_TEXT,
   OM_SUB_LINE,
   OM_SUB_SHAPE,
   OM_SUB_GENERAL,
   OM_SUB_BACKGROUND,
   OM_SUB_PIC_MAPPING,
   OM_SUB_TEXT_MAPPING
};

class CVecDemoDoc;
class CGobalBuffer;

#define CParentClass CVecView

class CVecDemoView : public CParentClass
{
protected: // create from serialization only
	CVecDemoView();
	DECLARE_DYNCREATE(CVecDemoView)

   BOOL m_bShowRullers;

   void SetContainerOffsetBeforeDraw();
	virtual void DrawBackground(CDC* pDC);      // overridden to draw this view
   virtual BOOL OnStartDragDrop(POINT point);

// Attributes
public:
   void CreateArtObjectFromSelection(HGLOBAL &hGlobal);
   void PutArtObjectInPage(HGLOBAL &hGlobal);
   static void RenderArtObjectSnapshot(HGLOBAL hBuffer, CBitmapBaseEx* pBitmap, RECT* rect = NULL);
   void LoadFromBuffer(CGobalBuffer &buff);
   void SaveToBuffer(CGobalBuffer &buff);
   void ClearDocument();
   void RenderDocumentSnapshot(CGobalBuffer &buff, CBitmap& snapshot, int x, int y);
   void RenderCurrentDocumentSnapshot(CBitmap& snapshot, int x, int y);
   inline BOOL IsViewDisabled(){return FALSE;}

   enVecTypes GetVecEntityType(unsigned ui);
   enVecTypes CVecDemoView::GetSelectedType();

   CVecDemoDoc*      GetDocument();

   UINT GetSelectedItems(CPtrList& Items, BOOL bExcludeLockObjects = FALSE);
   UINT GetSelectedItemsCount(BOOL bExcludeLockObjects = FALSE);
   BOOL ExcludeLockObjects(CPtrList& Items);

   void LoadBackground();

private:
   static void DoLoadBackground(CVecDemoDoc* pDoc,CVecRectanglePic* pBackgroundVec);
   BOOL DesideIfToShowShowPoints(CVecBase* pVec);
   void StoreUndoStepItemsZOrder(CPtrList& Items);

	virtual DROPEFFECT OnDragEnterPictures(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragOverPictures(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
   virtual DROPEFFECT OnDropExPictures( COleDataObject* pDataObject, DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point );	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual DROPEFFECT OnDragEnterVectors(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragOverVectors(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
   virtual DROPEFFECT OnDropExVectors( COleDataObject* pDataObject, DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point );	//virtual BOOL PreTranslateMessage(MSG* pMsg);
// Operations
public:
   
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVecDemoView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnDragLeave();
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
   virtual DROPEFFECT OnDropEx( COleDataObject* pDataObject, DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point );	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CVecDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

   COleDropTarget m_DropTarget;
   CRect m_LastDragRect;
   CDRect m_LastDragItemsRect;
   CDRect m_OrgDragItemsRect;
   CPtrList m_DradOverItms;
   CSize m_DragPicSize;
   CVecRectanglePic* m_pBackgroundVec;
   CVecContainer m_BackroundContainer;
   
   BOOL m_bIsDragFromThisView;
   POINT m_ptDragFromPoint;

   BOOL PopupShapesMenu(BOOL bShowPrevuislySelected = TRUE);
   void DoRotate(float ang);
   void DoDelete(CPtrList &Items);
   BOOL CheckIfSelected(enVecTypes exType, BOOL bExcludeLockObjects = FALSE);
   int CreateVecEntity();
   int DestroyVecEntity();
	void OnUpdatePictureCrop(CCmdUI* pCmdUI, UINT uiCrop);
	BOOL OnPictureCrop(UINT uiCrop);
	void OnUpdateTextAlign(CCmdUI* pCmdUI, UINT uiAlign);
	void OnTextAlign(UINT uiAlign);

// Generated message map functions
protected:
	//{{AFX_MSG(CVecDemoView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEditCopy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditDelete();
	afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
	afx_msg void OnEditSelectall();
	afx_msg void OnUpdateEditSelectall(CCmdUI* pCmdUI);
	afx_msg void OnPageFittowindow();
	afx_msg void OnPageResetzoom();
	afx_msg void OnPageZoomin();
	afx_msg void OnPageZoomout();
	afx_msg void OnUpdatePageFittowindow(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePageResetzoom(CCmdUI* pCmdUI);
	afx_msg void OnPaint();
	afx_msg void OnPageFittowidth();
	afx_msg void OnUpdatePageFittowidth(CCmdUI* pCmdUI);
	afx_msg void OnPageFittoheight();
	afx_msg void OnUpdatePageFittoheight(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnOrderBringtofront();
	afx_msg void OnOrderSendtoback();
	afx_msg void OnUpdateOrderSendtoback(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOrderBringtofront(CCmdUI* pCmdUI);
	afx_msg void OnPictureFlipHorizontal();
	afx_msg void OnPictureFlipVertical();
	afx_msg void OnUpdatePictureFlip(CCmdUI* pCmdUI);
	afx_msg void OnPictureRotate180();
	afx_msg void OnUpdatePictureRotate(CCmdUI* pCmdUI);
	afx_msg void OnPictureRotate90ccw();
	afx_msg void OnPictureRotate90cw();
	afx_msg void OnTextFont();
	afx_msg void OnUpdateTextFont(CCmdUI* pCmdUI);
	afx_msg void OnObjectBackcolor();
	afx_msg void OnObjectForecolor();
	afx_msg void OnUpdateObjectForecolor(CCmdUI* pCmdUI);
	afx_msg void OnUpdateObjectBackcolor(CCmdUI* pCmdUI);
	afx_msg void OnLineLine();
	afx_msg void OnUpdateLineLine(CCmdUI* pCmdUI);
	afx_msg void OnTextEdittext();
	afx_msg void OnUpdateTextEdittext(CCmdUI* pCmdUI);
	afx_msg void OnShapeFill();
	afx_msg void OnUpdateShapeFill(CCmdUI* pCmdUI);
	afx_msg void OnRotateFree();
	afx_msg void OnPageBackground();
	afx_msg void OnUpdatePageBackground(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePageZoomin(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePageZoomout(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnPictureSize();
	afx_msg void OnUpdatePictureSize(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnViewGrid();
	afx_msg void OnUpdateViewGrid(CCmdUI* pCmdUI);
	afx_msg void OnViewGridProperties();
	afx_msg void OnUpdateViewGridProperties(CCmdUI* pCmdUI);
	afx_msg void OnViewGuides();
	afx_msg void OnUpdateViewGuides(CCmdUI* pCmdUI);
	afx_msg void OnViewRullers();
	afx_msg void OnUpdateViewRullers(CCmdUI* pCmdUI);
	afx_msg void OnViewSnap2grid();
	afx_msg void OnUpdateViewSnap2grid(CCmdUI* pCmdUI);
	afx_msg void OnViewSnap2guides();
	afx_msg void OnUpdateViewSnap2guides(CCmdUI* pCmdUI);
	afx_msg void OnOrderSendoneback();
	afx_msg void OnUpdateOrderSendoneback(CCmdUI* pCmdUI);
	afx_msg void OnOrderSendonefore();
	afx_msg void OnUpdateOrderSendonefore(CCmdUI* pCmdUI);
	afx_msg void OnGroupGroup();
	afx_msg void OnUpdateGroupGroup(CCmdUI* pCmdUI);
	afx_msg void OnGroupUngroup();
	afx_msg void OnUpdateGroupUngroup(CCmdUI* pCmdUI);
	afx_msg void OnViewLockGuides();
	afx_msg void OnUpdateViewLockGuides(CCmdUI* pCmdUI);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPictureCropEllipse();
	afx_msg void OnUpdatePictureCropEllipse(CCmdUI* pCmdUI);
	afx_msg void OnPictureCropFreehand();
	afx_msg void OnUpdatePictureCropFreehand(CCmdUI* pCmdUI);
	afx_msg void OnPictureCropPolygon();
	afx_msg void OnUpdatePictureCropPolygon(CCmdUI* pCmdUI);
	afx_msg void OnPictureCropRectangle();
	afx_msg void OnUpdatePictureCropRectangle(CCmdUI* pCmdUI);
	afx_msg void OnPictureCropShape();
	afx_msg void OnUpdatePictureCropShape(CCmdUI* pCmdUI);
	afx_msg void OnObjectTextAligncenter();
	afx_msg void OnUpdateObjectTextAligncenter(CCmdUI* pCmdUI);
	afx_msg void OnObjectTextAlignleft();
	afx_msg void OnUpdateObjectTextAlignleft(CCmdUI* pCmdUI);
	afx_msg void OnObjectTextAlignright();
	afx_msg void OnUpdateObjectTextAlignright(CCmdUI* pCmdUI);
	afx_msg void OnObjectAlignLockobject();
	afx_msg void OnUpdateObjectAlignLockobject(CCmdUI* pCmdUI);
	afx_msg void OnObjectAlignUnlock();
	afx_msg void OnUpdateObjectAlignUnlock(CCmdUI* pCmdUI);
	afx_msg void OnPicmappingRemovephoto();
	afx_msg void OnUpdatePicmappingRemovephoto(CCmdUI* pCmdUI);
	afx_msg void OnPicmappingResetmappingarea();
	afx_msg void OnUpdatePicmappingResetmappingarea(CCmdUI* pCmdUI);
	afx_msg void OnShapeShowpoints();
	afx_msg void OnUpdateShapeShowpoints(CCmdUI* pCmdUI);
	afx_msg void OnObjectPictureShadow();
	afx_msg void OnUpdateObjectPictureShadow(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditProperties(CCmdUI* pCmdUI);
	afx_msg void OnObjectPictureTransparency();
	afx_msg void OnUpdateObjectPictureTransparency(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnUpdateTools(CCmdUI* pCmdUI);
	afx_msg void OnToolCheanged(UINT nID);
	afx_msg void OnUpdateObjectAlign(CCmdUI* pCmdUI);
	afx_msg void OnObjectAlign(UINT nID);
	afx_msg LRESULT OnRullerRightClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT
		OnRullerDBLClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnVecEvent(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version 
inline CVecDemoDoc* CVecDemoView::GetDocument()
   { return (CVecDemoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VECDEMOVIEW_H__44AA35D0_538F_11D2_B46D_444553540000__INCLUDED_)
