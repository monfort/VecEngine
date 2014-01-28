/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// VecBase.cpp: implementation of the CVecBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VecBase.h"
#include "VecContainer.h"
#include "G_Routines.h"
#include "MemBuffEx.h"
#include "resource.h"
#include "VecWorkspace.h"


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


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static UINT g_uiRunningID = 0;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HCURSOR CVecBase::m_hPointCursor = NULL;

CVecBase::CVecBase()
{
   m_uiType = VEC_PARENT;
   m_uiRunningID = g_uiRunningID++;
   m_pParentContainer = NULL;
   InitMembers();
   if(m_hPointCursor==NULL)
   {
      m_hPointCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
      ASSERT(m_hPointCursor!=NULL);
   }
   m_pOldBrush = NULL;
   m_pOldPen = NULL;
   m_iOldRop2 = -1;
   m_iOldBkMode = -1;
   m_OldBkColor = DWORD(-1);
   m_OldViewPortOrg = CPoint(0,0);
}

void CVecBase::InitMembers()
{
   m_bLocked = FALSE;
   m_bVisible = TRUE;
   m_bSelected = FALSE;
   m_Rect.right = m_Rect.top = m_Rect.left = m_Rect.bottom = 0;
   m_Scaler.x = m_Scaler.y = 1;
   m_Offset.x = m_Offset.y = 0;
   m_ForeColor = RGB(255,0,0);
   m_BackColor = RGB(255,255,255);
   m_PatternColor = RGB(0,0,255);
   m_t_Scaler.x = m_t_Scaler.y = 1;
   m_t_Offset.x = m_t_Offset.y = 0;
   m_t_Rect.right = m_t_Rect.top = m_t_Rect.left = m_t_Rect.bottom = 0;
   m_t_ShadowOffset = m_ShadowOffset = CDPoint(0,0);
   m_bShowShadow = FALSE;
   m_ShadowColor = RGB(0,0,0);
   m_iShadowRop2 = R2_MASKPEN;

   m_dLineWidth = 1;
   m_t_iLineWidth = 1;
   m_iRop2 = R2_COPYPEN;
   m_iLineStyle = PS_SOLID;
   m_iFillMode = OPAQUE;
   m_iFillStyle = HS_SOLID;

   m_MappingRectangle.SetRectangleEmpty();
   m_t_MappingRectangle.SetRectangleEmpty();
}

CVecBase::~CVecBase()
{
   if(GetParent()!=NULL)
      Remove();
}

void CVecBase::Copy(CVecBase* pVec)
{
   m_bLocked = pVec->m_bLocked;
   m_bSelected = pVec->m_bSelected;
   m_Rect = pVec->m_Rect;
   m_Scaler = pVec->m_Scaler;
   m_ForeColor = pVec->m_ForeColor;
   m_BackColor = pVec->m_BackColor;
   m_PatternColor = pVec->m_PatternColor;
   m_t_Scaler = pVec->m_t_Scaler;
   m_t_Offset = pVec->m_t_Offset;
   m_t_Rect = pVec->m_t_Rect;

   m_dLineWidth = pVec->m_dLineWidth;
   m_t_iLineWidth = pVec->m_t_iLineWidth;
   m_iRop2 = pVec->m_iRop2;
   m_iLineStyle = pVec->m_iLineStyle;
   m_iFillMode = pVec->m_iFillMode;
   m_iFillStyle = pVec->m_iFillStyle;

   m_MappingRectangle = pVec->m_MappingRectangle;
   m_t_MappingRectangle = pVec->m_t_MappingRectangle;

   m_t_ShadowOffset = pVec->m_t_ShadowOffset;
   m_ShadowOffset = pVec->m_ShadowOffset;
   m_bShowShadow = pVec->m_bShowShadow;
   m_ShadowColor = pVec->m_ShadowColor;
   m_iShadowRop2 = pVec->m_iShadowRop2;
}

void CVecBase::SetRect(CDRect &rect)
{
   CDRect oldRect(m_Rect);
   m_Rect = rect;

   for(int i=0; i<m_MappingRectangle.GetNumOfPoints(); i++)
   {
      ScaleAndOffsetPoint(&m_MappingRectangle.GetPoints()[i], &m_Rect, &oldRect);
   }
}

void CVecBase::Rotate(double dAng, LPDPOINT PtCenter)
{
   CDPoint center;
   if(PtCenter!=NULL)
      center = *PtCenter;
   else
      center = GetRect().CenterPoint();

   RotatePoints(m_MappingRectangle.GetPoints(),m_MappingRectangle.GetNumOfPoints(),dAng, &center);

   CDRect rect = GetRect();
   CDPOINT lpPointsNew[2];
   lpPointsNew[0] = rect.TopLeft();
   lpPointsNew[1] = rect.BottomRight();

   RotatePoints(lpPointsNew, 2, dAng, &center);

   m_Rect = CDRect(lpPointsNew[0].x, lpPointsNew[0].y, lpPointsNew[1].x, lpPointsNew[1].y);
}
void CVecBase::Flip(BOOL bHorizontal, LPDPOINT PtCenter)
{
   CDPoint center;
   if(PtCenter!=NULL)
      center = *PtCenter;
   else
      center = GetRect().CenterPoint();

   FlipPoints(m_MappingRectangle.GetPoints(),m_MappingRectangle.GetNumOfPoints(),bHorizontal, &center);

   CDRect rect = GetRect();
   CDPOINT lpPointsNew[2];
   lpPointsNew[0] = rect.TopLeft();
   lpPointsNew[1] = rect.BottomRight();

   FlipPoints(lpPointsNew, 2, bHorizontal, &center);

   m_Rect = CDRect(lpPointsNew[0].x, lpPointsNew[0].y, lpPointsNew[1].x, lpPointsNew[1].y);
}

void CVecBase::DoTransform()
{
   m_t_Offset = m_Offset;
   m_t_Scaler = m_Scaler;
   m_t_Rect = m_Rect;

   CVecContainer* pParent = GetParent();

   if(pParent!=NULL)
   {
      m_t_Scaler *= pParent->m_t_Scaler;
      m_t_Offset += pParent->m_t_Offset;
   }

   m_t_Rect *= m_t_Scaler;
   m_t_Rect += m_t_Offset;

   m_t_iLineWidth = (int)(m_dLineWidth * m_t_Scaler.x);
   if(m_t_iLineWidth<1)
      m_t_iLineWidth = 1;

   m_t_MappingRectangle = m_MappingRectangle;
   
   m_t_MappingRectangle *= m_t_Scaler;
   m_t_MappingRectangle += m_t_Offset;

   m_t_ShadowOffset = m_ShadowOffset;
   m_t_ShadowOffset *= m_t_Scaler;
}


CVecContainer* CVecBase::GetTopContainer()
{
   if(m_pParentContainer!=NULL)
      return m_pParentContainer->GetTopContainer();
   return NULL;
}

CVecWorkspace* CVecBase::GetWorkspace()
{
   CVecContainer* pContainer = GetTopContainer();
   if(pContainer!=NULL)
      return pContainer->GetWorkspaceOb();
   else
      return NULL;
}

void CVecBase::SetSelected(BOOL b)
{
   m_bSelected = b;
}

void CVecBase::Repaint()
{
   CWnd* pWnd = GetTopContainer()->GetWnd();
   pWnd->Invalidate();
}

BOOL CVecBase::SetCursor(CWnd* pWnd, UINT nHitTest, UINT uiTool) 
{
   ::SetCursor(m_hPointCursor);
   return TRUE;
}

void CVecBase::DrawLockedIcon(CDC* pDC)
{
   CBitmapBaseEx bitmap;
   CBitmap bitmapMask;
   CBitmap bitmapMasked;

   bitmap.LoadBitmapFromResource(IDB_LOCKED_OBJ_ICON);
   
   bitmap.CreateMask(bitmapMask,bitmapMasked,RGB(255,0,0));

   bitmap.SetBitmap(bitmapMask);
   
   bitmap.DrawTransparent(pDC->m_hDC,CRect((int)m_t_Rect.left,(int)m_t_Rect.top,(int)m_t_Rect.left+bitmap.GetWidth(),(int)m_t_Rect.top+bitmap.GetHeight()),bitmapMask,bitmapMasked);
}

BOOL CVecBase::IsInsideRect(CDRect &rect)
{
   CDPoint p1(m_t_Rect.left, m_t_Rect.top);
   CDPoint p2(m_t_Rect.right, m_t_Rect.bottom);
   if(rect.PtInRect(p1) && rect.PtInRect(p2))
      return TRUE;

   return FALSE;
}

void CVecBase::SendToBack()
{
   CVecContainer* pParent = GetParent();
   if(pParent!=NULL)
      pParent->SendToBack(this);
}

void CVecBase::BringToFront()
{
   CVecContainer* pParent = GetParent();
   if(pParent!=NULL)
      pParent->BringToFront(this);
}

void CVecBase::SendOneBackward()
{
   CVecContainer* pParent = GetParent();
   if(pParent!=NULL)
      pParent->SendOneBackward(this);
}

void CVecBase::BringOneForward()
{
   CVecContainer* pParent = GetParent();
   if(pParent!=NULL)
      pParent->BringOneForward(this);
}

void CVecBase::Remove()
{
   CVecContainer* pParent = GetParent();
   if(pParent!=NULL)
      pParent->Remove(this);
}

void CVecBase::SetRectByScreen(CDRect &rect)
{
   CDRect drect(rect);
   drect -= m_t_Offset;
   drect /= m_t_Scaler;
   SetRect(drect);
}

void CVecBase::SetRectScaleAndOffset(LPRECT lpRectNew, LPRECT lpRectOrigianl)
{
   CDRect rect(GetRect());
   CDPoint TopLeft = rect.TopLeft();
   CDPoint BottomRight = rect.BottomRight();

   ScaleAndOffsetPoint(&TopLeft, lpRectNew, lpRectOrigianl);
   ScaleAndOffsetPoint(&BottomRight, lpRectNew, lpRectOrigianl);
   CDRect drect(TopLeft.x, TopLeft.y, BottomRight.x, BottomRight.y);
   SetRect(drect);
}
void CVecBase::SetRectScaleAndOffsetByScreen(LPRECT lpRectNew, LPRECT lpRectOrigianl)
{
   CDRect rect(GetRectByScreen());
   CDPoint TopLeft = rect.TopLeft();
   CDPoint BottomRight = rect.BottomRight();

   ScaleAndOffsetPoint(&TopLeft, lpRectNew, lpRectOrigianl);
   ScaleAndOffsetPoint(&BottomRight, lpRectNew, lpRectOrigianl);
   CDRect drect(TopLeft.x, TopLeft.y, BottomRight.x, BottomRight.y);
   SetRectByScreen(drect);
}
void CVecBase::SetRectScaleAndOffset(LPDRECT lpRectNew, LPDRECT lpRectOrigianl)
{
   CDRect rect(GetRect());
   CDPoint TopLeft = rect.TopLeft();
   CDPoint BottomRight = rect.BottomRight();

   ScaleAndOffsetPoint(&TopLeft, lpRectNew, lpRectOrigianl);
   ScaleAndOffsetPoint(&BottomRight, lpRectNew, lpRectOrigianl);
   CDRect drect(TopLeft.x, TopLeft.y, BottomRight.x, BottomRight.y);
   SetRect(drect);
}
void CVecBase::SetRectScaleAndOffsetByScreen(LPDRECT lpRectNew, LPDRECT lpRectOrigianl)
{
   CDRect rect(GetRectByScreen());
   CDPoint TopLeft = rect.TopLeft();
   CDPoint BottomRight = rect.BottomRight();

   ScaleAndOffsetPoint(&TopLeft, lpRectNew, lpRectOrigianl);
   ScaleAndOffsetPoint(&BottomRight, lpRectNew, lpRectOrigianl);
   CDRect drect(TopLeft.x, TopLeft.y, BottomRight.x, BottomRight.y);
   SetRectByScreen(drect);
}

void CVecBase::LocalToScreen(LPDPOINT lpPoint)
{
   CDPoint dpoint = *lpPoint;
   dpoint *= m_t_Scaler;
   dpoint += m_t_Offset;
   *lpPoint = dpoint;
}
void CVecBase::ScreenToLocal(LPDPOINT lpPoint)
{
   CDPoint dpoint = *lpPoint;
   dpoint -= m_t_Offset;
   dpoint /= m_t_Scaler;
   *lpPoint = dpoint;
}
void CVecBase::LocalToScreen(LPDRECT lpRect)
{
   CDRect drect = *lpRect;
   drect *= m_t_Scaler;
   drect += m_t_Offset;
   *lpRect = drect;
}
void CVecBase::ScreenToLocal(LPDRECT lpRect)
{
   CDRect drect = *lpRect;
   drect -= m_t_Offset;
   drect /= m_t_Scaler;
   *lpRect = drect;
}

void CVecBase::CreateBrush(CBrush &brush, BOOL bShoadow)
{
   LOGBRUSH logbrush;
   logbrush.lbHatch = m_iFillStyle;
   logbrush.lbStyle = m_iFillStyle == HS_SOLID ? BS_SOLID : BS_HATCHED;
   logbrush.lbColor = m_iFillStyle == HS_SOLID ? m_BackColor : m_PatternColor;
   if(bShoadow)
      logbrush.lbColor = m_ShadowColor;
   brush.CreateBrushIndirect(&logbrush);
}
void CVecBase::CreatePen(CPen &pen, BOOL bShoadow)
{
   pen.CreatePen(m_iLineStyle, m_t_iLineWidth, bShoadow ? m_ShadowColor : m_ForeColor);
}
void CVecBase::PreperDC(CDC* pDC, BOOL bShoadow)
{
   ASSERT(m_iOldRop2 == -1);
   ASSERT(m_pOldBrush == NULL);
   ASSERT(m_pOldPen == NULL);
   ASSERT(m_iOldBkMode == -1);
   ASSERT(m_OldBkColor == DWORD(-1));

   if(m_iFillMode == TRANSPARENT && m_iFillStyle == HS_SOLID)
   {
      m_pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
   }
   else
   {
      CreateBrush(m_Brush, bShoadow);
      m_pOldBrush = (CBrush*)pDC->SelectObject(&m_Brush);
   }

   CreatePen(m_Pen, bShoadow);
   m_pOldPen = (CPen*)pDC->SelectObject(&m_Pen);

   m_iOldRop2 = pDC->SetROP2(bShoadow ? m_iShadowRop2 : m_iRop2);
   m_iOldBkMode = pDC->SetBkMode(bShoadow ? m_ShadowColor : m_iFillMode);
   m_OldBkColor = pDC->SetBkColor(bShoadow ? m_ShadowColor : m_BackColor);

	m_OldViewPortOrg = pDC->GetViewportOrg();
   if(bShoadow)
   {
      CPoint p(m_t_ShadowOffset);
      p+=m_OldViewPortOrg;
	   pDC->SetViewportOrg(p);
   }

}

void CVecBase::RestoreDC(CDC* pDC)
{
   pDC->SetROP2(m_iOldRop2);
   pDC->SelectObject(m_pOldBrush);
   pDC->SelectObject(m_pOldPen);
   pDC->SetBkMode(m_iOldBkMode);
   pDC->SetBkColor(m_OldBkColor);

   m_Brush.DeleteObject();
   m_Pen.DeleteObject();

	pDC->SetViewportOrg(m_OldViewPortOrg);

#ifdef _DEBUG
   m_pOldBrush = NULL;
   m_pOldPen = NULL;
   m_iOldRop2 = -1;
   m_iOldBkMode = -1;
   m_OldBkColor = DWORD(-1);
#endif
}


void CVecBase::SaveToBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
{
   pMemBuff->WriteDwordField(VF_ForeColor, m_ForeColor);
   pMemBuff->WriteDwordField(VF_BackColor, m_BackColor);
   pMemBuff->WriteDwordField(VF_PatternColor, m_PatternColor);
   //pMemBuff->WriteBoolField(VF_Selected, m_bSelected);
   pMemBuff->WriteBoolField(VF_Visible, m_bVisible);
   pMemBuff->WriteIntField(VF_Rop2, m_iRop2);
   pMemBuff->WriteIntField(VF_LineStyle, m_iLineStyle);
   pMemBuff->WriteIntField(VF_FillMode, m_iFillMode);
   pMemBuff->WriteIntField(VF_FillStyle, m_iFillStyle);
   pMemBuff->WriteDoubleField(VF_LineWidth, m_dLineWidth);
   pMemBuff->WriteDRectField(VF_Rect, m_Rect);
   pMemBuff->WriteDPointField(VF_Scaler, m_Scaler);
   pMemBuff->WriteDPointField(VF_Offset, m_Offset);
   pMemBuff->WriteField(VF_MappingRectangle, &m_MappingRectangle, sizeof(DRECTANGLE));
   pMemBuff->WriteBoolField(VF_Locked, m_bLocked);
   pMemBuff->WriteDPointField(VF_ShadowOffset, m_ShadowOffset);
   pMemBuff->WriteBoolField(VF_ShowShadow, m_bShowShadow);
   pMemBuff->WriteDwordField(VF_ShadowColor, m_ShadowColor);
   pMemBuff->WriteIntField(VF_ShadowRop2, m_iShadowRop2);

   pMemBuff->WriteEndOfFields();
}

void CVecBase::LoadFromBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
{
   while(pMemBuff->ReadNextFieldHeader())
   {
      int id = pMemBuff->GetFieldType();
      switch(id)
      {
      case VF_ShadowOffset:
         pMemBuff->ReadDPoint(&m_ShadowOffset);
         break;
      case VF_ShowShadow:
         m_bShowShadow = pMemBuff->ReadBool();
         break;
      case VF_ShadowColor:
         m_ShadowColor = pMemBuff->ReadDword();
         break;
      case VF_ShadowRop2:
         m_iShadowRop2 = pMemBuff->ReadInt();
         break;
      case VF_ForeColor:
         m_ForeColor = pMemBuff->ReadDword();
         break;
      case VF_BackColor:
         m_BackColor = pMemBuff->ReadDword();
         break;
      case VF_PatternColor:
         m_PatternColor = pMemBuff->ReadDword();
         break;
      /*case VF_Selected:
         m_bSelected = pMemBuff->ReadBool();
         break;*/
      case VF_Locked:
         m_bLocked = pMemBuff->ReadBool();
         break;
      case VF_Visible:
         m_bVisible = pMemBuff->ReadBool();
         break;
      case VF_Rop2:
         m_iRop2 = pMemBuff->ReadInt();
         break;
      case VF_LineStyle:
         m_iLineStyle = pMemBuff->ReadInt();
         break;
      case VF_FillMode:
         m_iFillMode = pMemBuff->ReadInt();
         break;
      case VF_FillStyle:
         m_iFillStyle = pMemBuff->ReadInt();
         break;
      case VF_LineWidth:
         m_dLineWidth = pMemBuff->ReadDouble();
         break;
      case VF_Rect:
         pMemBuff->ReadDRect(&m_Rect);
         break;
      case VF_Scaler:
         pMemBuff->ReadDPoint(&m_Scaler);
         break;
      case VF_Offset:
         pMemBuff->ReadDPoint(&m_Offset);
         break;
      case VF_MappingRectangle:
         pMemBuff->Read(&m_MappingRectangle, sizeof(DRECTANGLE));
         break;
      default: 
         pMemBuff->SkipField(); 
         break;
      }
   }
}


CVecBase* CVecBase::CreateNewInstanceByType(UINT uiType)
{
   CVecBase* pVec = NULL;
   switch(uiType)
   {
   case VEC_CONTAINER:
      pVec = new CVecContainer;
      break;
   case VEC_RECTANGLE:
      pVec = new CVecRectangle;
      break;
   case VEC_ELLIPSE:
      pVec = new CVecEllipse;
      break;
   case VEC_LINE:
      pVec = new CVecLine;
      break;
   case VEC_POLYLINE:
      pVec = new CVecPolyline;
      break;
   case VEC_POLYGON:
      pVec = new CVecPolygon;
      break;
   case VEC_POLYGON_PIC:
      pVec = new CVecPolygonPic;
      break;
   case VEC_ELLIPSE_PIC:
      pVec = new CVecEllipsePic;
      break;
   case VEC_RECTANGLE_PIC:
      pVec = new CVecRectanglePic;
      break;
   case VEC_RECTANGLE_TEXT:
      pVec = new CVecRectangleText;
      break;
   case VEC_POLYGON_TEXT:
      pVec = new CVecPolygonText;
      break;
   case VEC_ELLIPSE_TEXT:
      pVec = new CVecEllipseText;
      break;
   case VEC_FRAME_CONTAINER:
      pVec = new CVecFrameContainer;
      break;
	default: 
      ASSERT(FALSE); 
      break;
	} 
   return pVec;
}

void CVecBase::SetLocked(BOOL b)
{
   if(GetParent()!=NULL)
      if(GetParent()->IsLocked())
         return;
   m_bLocked = b;
}

CVecBase* CVecBase::GetTracker()
{
   if(GetParent()!=NULL)
   {
      CVecBase* pTracker = GetParent()->GetOverrideTracker(); 
      if(pTracker!=NULL) 
         return pTracker; 
   }
   return this;
}
