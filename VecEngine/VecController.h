// VecController.h: interface for the CVecController class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECCONTROLLER_H__6318D40B_B6F0_11D3_A497_00C0CA1261A6__INCLUDED_)
#define AFX_VECCONTROLLER_H__6318D40B_B6F0_11D3_A497_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CVecContainer;
class CVecBase;
#include "VecTypes.h"
#include "Snapper.h"
#include "UndoManager.h"


typedef struct TagSHAPE
{
   int iVertexCount;
   CDPOINT *aVertex;
}SHAPE, *LPSHAPE;


#define FIRST_CREATORS_TOOLS 100
#define FIRST_CROP_TOOLS 200
enum CONTROLLER_TOOLS
{
   TOOL_SELECT = 0,
   TOOL_ROTATE,
   TOOL_DRAG_DROP,
   LAST_CHEANGE_TOOL,

   TOOL_RECTANGLE = FIRST_CREATORS_TOOLS,
   TOOL_ELLIPSE,
   TOOL_LINE,
   TOOL_POLYLINE,
   TOOL_POLYGON,
   TOOL_FREEHAND,
   TOOL_FREEHAND_CLOSED,
   TOOL_TEXT,
   TOOL_SHAPE,
   LAST_CREATORS_TOOLS,

   TOOL_CROP_RECTANGLE = FIRST_CROP_TOOLS,
   TOOL_CROP_ELLIPSE,
   TOOL_CROP_POLYGON,
   TOOL_CROP_FREEHAND,
   TOOL_CROP_SHAPE,
   LAST_CROP_TOOLS
};

class CVecController : public CSnapper
{
public:
	CVecController();
	virtual ~CVecController();
   inline void SetContainer(CVecContainer* pContainer){m_pContainer = pContainer;}
   inline CVecContainer* GetContainer(){return m_pContainer;}
   virtual void DrawGrid(CDC * pDC);
   virtual void DrawGuides(CDC * pDC);
   virtual void OnCreateGuide(BOOL bHorizontal);

public://events:
   void OnMouseMove(UINT nFlags, CPoint point);
   void OnMouseDown(UINT nFlags, CPoint point);
   void OnMouseUp(UINT nFlags, CPoint point);
   void OnRightClick(UINT nFlags, CPoint point);
   BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
   void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) ;
   void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) ;
   void OnLButtonDblClk(UINT nFlags, CPoint point);
   inline UINT GetSelectedTool(){return m_uiSlectedTool;}
   virtual void SetSelectedTool(UINT uiTool);
   inline LPSHAPE GetSelectedShapeTool(){return m_pSlectedShapeTool;}
   virtual void SetSelectedShapeTool(LPSHAPE lpShape){m_pSlectedShapeTool = lpShape;}
   virtual void SetCropTool(UINT uiTool, CVecBase* pVec);
   virtual void KillCropMode();
   virtual void SetModified(BOOL bModified = TRUE);
   inline BOOL IsModified(){return m_bModified;}

public: //SnapTo callbacks (CSnapper APIs)
   virtual double FixSnapTo(LPCDPOINT lpPoint);
   virtual double FixSnapTo(LPDRECT lpRect);
   virtual double FixSnapTo(LPCDPOINT lpPoints, UINT uiCount);
   virtual int FixSnapTo(LPPOINT lpPoint);
   virtual int FixSnapTo(LPRECT lpRect);
   virtual int FixSnapTo(LPPOINT lpPoints, UINT uiCount);

protected://events
   virtual void OnVecCreated(CVecBase* pVec);
   virtual void OnSelChanged();
   BOOL WaitForMouseUp(CWnd* pWnd);


private:
   virtual void DoCreate(UINT nFlags, CPoint point);
   virtual void DoCrop(UINT nFlags, CPoint point);
   void RestoreCropHighlight();
   CDRectangle CropCopyAndReplaceItem(CVecBase* pVec);
   void LoadCursors();
   CVecContainer* m_pContainer;
   HCURSOR m_hHotRotateCursor;
   HCURSOR m_hHotSetSelectedCursor;
   HCURSOR m_hCreateCursor;
   HCURSOR m_hCropCursor;
   HCURSOR m_hGuideHorizontal;
   HCURSOR m_hGuideVertical;
   UINT m_uiSlectedTool;
   LPSHAPE m_pSlectedShapeTool;
   BOOL m_bModified;
   CVecBase* m_pCropVec;

   int m_iOldLineStyle;
   double m_dOldLineWidth;
   COLORREF m_crOldForeColor;

public://guides and grid and undo
   //in the top container cordinate system. 
   CDPoint m_ptGridSize;
   CPtrList m_GuidesList;//CDPoint objects
   //in the top container cordinate system after transform
   CDPoint m_t_ptGridSize;
   BOOL m_bShowGuides;
   BOOL m_bShowGrid;
   BOOL m_bSnapToGuides;
   BOOL m_bSnapToGrid;
   BOOL m_bLockGuides;
   CUndoManager m_UndoManager;

};

#endif // !defined(AFX_VECCONTROLLER_H__6318D40B_B6F0_11D3_A497_00C0CA1261A6__INCLUDED_)
