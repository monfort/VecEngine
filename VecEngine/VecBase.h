/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// VecBase.h: interface for the CVecBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECBASE_H__9AC864F8_B6BC_11D3_A496_00C0CA1261A6__INCLUDED_)
#define AFX_VECBASE_H__9AC864F8_B6BC_11D3_A496_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable: 4100) //disable the "unreferenced formal parameter" warning

#include "VecTypes.h"

class CVecWorkspace;
class CVecContainer;
class CVecFrameContainer;
class CSnapper;
class CUndoManager;
class CNodedMemoryBufferEx;

enum SAVE_TO_BUFFER_FLAGS
{
   SBF_FULL = 0,
   SBF_LIGHT = 1,
   SBF_DONT_CREATE_ITEMS = 2,
   SBF_SAVE_THIS_OB_ONLY = 4
};

enum DRAW_FLAGS
{
   DF_NORMAL = 0,
   DF_DONT_DRAW_SELECTION = 1,
   DF_PRINT = 2
};

enum HitTestResoult
{
   HT_NONE = -1,
   HT_INSIDE = 0,
   HT_ON_LINE = 2,
   HT_ON_POINT = 3,
   HT_ON_MAPPING_POINT = 4,
   HT_INSIDE_MAPPING = 5
};

enum VEC_TOOLS
{
   VEC_TOOL_MOVE_SIZE,
   VEC_TOOL_ROTATE,
};

#define VEC_SECOND_PARENT_START 100
enum VEC_TYPES // nodes ids (file type) - do not cheange values
{
   VEC_PARENT = 0,
   VEC_CONTAINER,
   VEC_RECTANGLE,
   VEC_ELLIPSE,
   VEC_LINE,
   VEC_POLYLINE,
   VEC_POLYGON,
   VEC_POLYGON_PIC,
   VEC_ELLIPSE_PIC,
   VEC_RECTANGLE_PIC,
   VEC_RECTANGLE_TEXT,
   VEC_POLYGON_TEXT,
   VEC_ELLIPSE_TEXT,
   VEC_FRAME_CONTAINER,
   VEC_LAST_ITEM,

   VEC_2ND_BITMAP_BASE = VEC_SECOND_PARENT_START,
   VEC_2ND_TEXT_BASE,
   VEC_2ND_LAST_ITEM
};


/*=====================================how to use staff
//---------  int m_iRop2;
#define R2_BLACK            1   //  0       
#define R2_NOTMERGEPEN      2   // DPon     
#define R2_MASKNOTPEN       3   // DPna     
#define R2_NOTCOPYPEN       4   // PN       
#define R2_MASKPENNOT       5   // PDna     
#define R2_NOT              6   // Dn       
#define R2_XORPEN           7   // DPx      
#define R2_NOTMASKPEN       8   // DPan     
#define R2_MASKPEN          9   // DPa      
#define R2_NOTXORPEN        10  // DPxn     
#define R2_NOP              11  // D        
#define R2_MERGENOTPEN      12  // DPno     
#define R2_COPYPEN          13  // P        
#define R2_MERGEPENNOT      14  // PDno     
#define R2_MERGEPEN         15  // DPo      
#define R2_WHITE            16  //  1       

//---------  int m_iLineStyle;
#define PS_SOLID            0
#define PS_DASH             1       // -------  
#define PS_DOT              2       // .......  
#define PS_DASHDOT          3       // _._._._  
#define PS_DASHDOTDOT       4       // _.._.._  
#define PS_NULL             5


//---------  int m_iFillMode;
#define TRANSPARENT         1
#define OPAQUE              2

//---------  int m_iFillStyle;
#define HS_HORIZONTAL       0       // ----- 
#define HS_VERTICAL         1       // ||||| 
#define HS_FDIAGONAL        2       // \\\\\ 
#define HS_BDIAGONAL        3       // ///// 
#define HS_CROSS            4       // +++++ 
#define HS_DIAGCROSS        5       // xxxxx 
*/
#define HS_SOLID            -1

class CVecBase  
{
   friend class CVecContainer;
   friend class CVecFrameContainer;
public:
	CVecBase();
	CVecBase(CVecContainer* pParentVec);
	virtual ~CVecBase();
   virtual void Draw(CDC * pDC, DWORD dwFlags = DF_NORMAL) = 0;
   virtual void DrawShadow(CDC * pDC, DWORD dwFlags = DF_NORMAL) = 0;
   virtual void DrawPoints(CDC * pDC) = 0;
   virtual void DrawLockedIcon(CDC* pDC);
   virtual void DoTransform();
   virtual void InitMembers();
   inline CVecContainer* GetParent(){return m_pParentContainer;}
   virtual void Repaint();
   virtual CVecContainer* GetTopContainer();
   CVecWorkspace* GetWorkspace();
   virtual HitTestResoult HitTest(CDPoint &p) = 0;
   virtual BOOL Track(CDPoint point, UINT uiTool, CUndoManager* pUndoMan, CSnapper* pSnapper = NULL) = 0;
   virtual CVecBase* GetTracker();
   virtual void DrawDragImage(CDC* pDC,LPDRECT lpRectNew,LPDRECT lpRectLast,LPDRECT lpRectOrigianl) = 0;
   virtual void DrawRotatedDragImage(CDC* pDC, double dAng, LPPOINT lpCenter, double dAngLast, LPPOINT lpCenterLast) = 0;
	virtual BOOL SetCursor(CWnd* pWnd, UINT nHitTest, UINT uiTool);
   virtual BOOL OnDBClick(UINT nFlags, CPoint point, CUndoManager* pUndoMan){return FALSE;}
   virtual BOOL IsInsideRect(CDRect &rect);
   virtual void LocalToScreen(LPDPOINT lpPoint);
   virtual void ScreenToLocal(LPDPOINT lpPoint);
   virtual void LocalToScreen(LPDRECT lpRect);
   virtual void ScreenToLocal(LPDRECT lpRect);
   virtual void SaveToBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags = SBF_FULL);
   virtual void LoadFromBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags = SBF_FULL);
   virtual void StoreToBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags = SBF_FULL){SaveToBuffer(pMemBuff, iFlags);}
   virtual void RestoreFromBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags = SBF_FULL){LoadFromBuffer(pMemBuff, iFlags);}
   static CVecBase* CreateNewInstanceByType(UINT uiType);

public://set and get properties
   virtual void SetRect(CDRect &rect);
   inline CDRect GetRect(){return m_Rect;}
   inline CDRect GetRectByScreen(){return m_t_Rect;}
   virtual void SetRectByScreen(CDRect &rect);
   virtual void SetRectScaleAndOffset(LPRECT lpRectNew, LPRECT lpRectOrigianl);
   virtual void SetRectScaleAndOffset(LPDRECT lpRectNew, LPDRECT lpRectOrigianl);
   virtual void SetRectScaleAndOffsetByScreen(LPRECT lpRectNew, LPRECT lpRectOrigianl);
   virtual void SetRectScaleAndOffsetByScreen(LPDRECT lpRectNew, LPDRECT lpRectOrigianl);

   virtual void SetScaler(CDPoint &point){m_Scaler = point;}
   inline CDPoint GetScaler(){return m_Scaler;}
   virtual void SetOffset(CDPoint &point){m_Offset = point;}
   inline CDPoint GetOffset(){return m_Offset;}
   inline void SetForeColor(COLORREF col){m_ForeColor = col;}
   inline COLORREF GetForeColor(){return m_ForeColor;}
   inline void SetBackColor(COLORREF col){m_BackColor = col;}
   inline COLORREF GetPatternColor(){return m_PatternColor;}
   inline void SetPatternColor(COLORREF col){m_PatternColor = col;}
   inline COLORREF GetBackColor(){return m_BackColor;}
   virtual void SetSelected(BOOL b);
   inline BOOL IsSelected(){return m_bSelected;}
   virtual void SetLocked(BOOL b);
   inline BOOL IsLocked(){return m_bLocked;}
   inline void SetVisible(BOOL b){m_bVisible = b;}
   inline BOOL IsVisible(){return m_bVisible;}

   virtual void SetMappingRectangle(CDRectangle &rect){m_MappingRectangle = rect;}
   inline CDRectangle GetMappingRectangle(){return m_MappingRectangle;}
   inline CDRectangle GetMappingRectangleByScreen(){return m_t_MappingRectangle;}
   virtual void SetMappingRectangleByScreen(CDRectangle &rect){ CDRectangle drect(rect); drect -= m_t_Offset; drect /= m_t_Scaler; SetMappingRectangle(drect);}

public://draw properties
   inline void SetRop2(int iRop2){m_iRop2 = iRop2;}
   inline int GetRop2() const {return m_iRop2;}
   inline void SetLineWidth(double dLineWidth){m_dLineWidth = dLineWidth;}
   inline double GetLineWidth() const {return m_dLineWidth;}
   inline void SetLineStyle(int iLineStyle){m_iLineStyle = iLineStyle;}
   inline int GetLineStyle() const {return m_iLineStyle;}
   inline void SetFillMode(int IFillMode){m_iFillMode = IFillMode;}
   inline int GetFillMode() const {return m_iFillMode;}
   inline void SetFillStyle(int iFillStyle){m_iFillStyle = iFillStyle;}
   inline int GetFillStyle() const {return m_iFillStyle;}
public://shadow properties
   inline void SetShadowRop2(int iRop2){m_iShadowRop2 = iRop2;}
   inline int GetShadowRop2() const {return m_iShadowRop2;}
   inline void SetShowShadow(BOOL b){m_bShowShadow = b;}
   inline BOOL IsShowShadow(){return m_bShowShadow;}
   inline COLORREF GetShadowColor(){return m_ShadowColor;}
   inline void SetShadowColor(COLORREF col){m_ShadowColor = col;}
   inline void SetShadowOffset(CDPoint &point){m_ShadowOffset = point;}
   inline CDPoint GetShadowOffset(){return m_ShadowOffset;}


public://types
   virtual UINT GetType(){return m_uiType;}
   virtual BOOL IsCloseShape(){return FALSE;}
   virtual BOOL HasPoints(){return FALSE;}
   virtual BOOL HasText(){return FALSE;}
   virtual BOOL HasPicture(){return FALSE;}

public://actions
   virtual void Rotate(double dAng, LPDPOINT PtCenter = NULL);
   virtual void Flip(BOOL bHorizontal, LPDPOINT PtCenter = NULL);
   void SendToBack();
   void BringToFront();
   void SendOneBackward();
   void BringOneForward();
   void Remove();
   virtual void Copy(CVecBase* pVec);
   


protected:
   CVecContainer* m_pParentContainer;
   static HCURSOR m_hPointCursor;

public:
   inline UINT GetRunningID(){return m_uiRunningID;}
   inline void SetRunningID(UINT uiID){m_uiRunningID = uiID;}
   virtual CVecBase* GetObByRunningID(UINT uiID){if(uiID==m_uiRunningID) return this; return NULL;}

protected://properties
   UINT m_uiRunningID;
   UINT m_uiType;
   COLORREF m_ForeColor;
   COLORREF m_BackColor;
   COLORREF m_PatternColor;
   COLORREF m_ShadowColor;
   BOOL m_bSelected;
   BOOL m_bLocked;
   BOOL m_bVisible;
   BOOL m_bShowShadow;

protected://draw properties   
   int m_iRop2;
   int m_iLineStyle;
   int m_iFillMode;
   int m_iFillStyle;
   int m_iShadowRop2;


   void CreateBrush(CBrush &brush, BOOL bShoadow = FALSE);
   void CreatePen(CPen &pen, BOOL bShoadow = FALSE);
   void PreperDC(CDC* pDC, BOOL bShoadow = FALSE);
   void RestoreDC(CDC* pDC);
private:
   CBrush* m_pOldBrush;
   CPen* m_pOldPen;
   int m_iOldRop2;
   int m_iOldBkMode;
   COLORREF m_OldBkColor;
   CBrush m_Brush;
   CPen m_Pen;
   CPoint m_OldViewPortOrg;

protected://vec properties
   double m_dLineWidth;
   CDRect m_Rect;
   CDPoint m_Scaler;
   CDPoint m_Offset;
   CDRectangle m_MappingRectangle;
   CDPoint m_ShadowOffset;


protected://vec properties after transform
   int m_t_iLineWidth;
   CDRect m_t_Rect;
   CDPoint m_t_Scaler;
   CDPoint m_t_Offset;
   CDRectangle m_t_MappingRectangle;
   CDPoint m_t_ShadowOffset;

private:
   void SetParent(CVecContainer* pParentVec){m_pParentContainer = pParentVec;}

private:
   enum VEC_FIELDS // fields ids (file type) - do not cheange values
   {
      VF_ForeColor=0,
      VF_BackColor,
      VF_PatternColor,
      VF_Selected,
      VF_Visible,
      VF_Rop2,
      VF_LineStyle,
      VF_FillMode,
      VF_FillStyle,
      VF_LineWidth,
      VF_Rect,
      VF_Scaler,
      VF_Offset,
      VF_MappingRectangle,
      VF_Locked,
      VF_ShadowOffset,
      VF_ShowShadow,
      VF_ShadowColor,
      VF_ShadowRop2
   };
};


#endif // !defined(AFX_VECBASE_H__9AC864F8_B6BC_11D3_A496_00C0CA1261A6__INCLUDED_)
