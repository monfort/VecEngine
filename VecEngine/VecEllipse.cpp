/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// VecEllipse.cpp: implementation of the CVecEllipse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VecEllipse.h"
#include "VecContainer.h"
#include "EllipseTracker.h"
#include "G_Routines.h"
#include "UndoManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVecEllipse::CVecEllipse()
{
   m_uiType = VEC_ELLIPSE;

}

CVecEllipse::~CVecEllipse()
{

}

void CVecEllipse::Draw(CDC * pDC, DWORD dwFlags)
{
   PreperDC(pDC);
   RECT rect = m_t_Rect;
   pDC->Ellipse(&rect);
   RestoreDC(pDC);
}

void CVecEllipse::DrawShadow(CDC * pDC, DWORD dwFlags)
{
   PreperDC(pDC, TRUE);
   RECT rect = m_t_Rect;
   pDC->Ellipse(&rect);
   RestoreDC(pDC);
}

void CVecEllipse::DrawPoints(CDC * pDC)
{
   CEllipseTracker rt(m_t_Rect, CEllipseTracker::resizeOutside);
   rt.Draw(pDC);
}

BOOL CVecEllipse::Track(CDPoint point, UINT uiTool, CUndoManager* pUndoMan, CSnapper* pSnapper)
{
   CEllipseTracker rt(m_t_Rect, CEllipseTracker::solidLine | CEllipseTracker::resizeOutside);
   if(HasPicture())
      rt.SetTrackRetainProportions();
   if(rt.Track(GetTopContainer()->GetWnd(), point, TRUE, pSnapper))
   {
      pUndoMan->AddToStack(this, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
      rt.m_rect.NormalizeRect();
      SetRectByScreen(rt.m_rect);
      return TRUE;
   }

   return FALSE;
}

BOOL CVecEllipse::SetCursor(CWnd* pWnd, UINT nHitTest, UINT uiTool)
{
   CPoint point;
   GetCursorPos(&point);
   pWnd->ScreenToClient(&point);

   CEllipseTracker rt(m_t_Rect, CEllipseTracker::solidLine | CEllipseTracker::resizeOutside);
   rt.SetCursor(pWnd, nHitTest);//show cursor of the rect tracker
   return TRUE;

}

HitTestResoult CVecEllipse::HitTest(CDPoint &p)
{
   CEllipseTracker rt(m_t_Rect, CEllipseTracker::solidLine | CEllipseTracker::resizeOutside);
   int iHit = rt.HitTest(p);
   if(iHit != CEllipseTracker::hitMiddle && iHit != CEllipseTracker::hitNothing && IsSelected())
      return HT_ON_POINT;
   if(iHit == CEllipseTracker::hitMiddle)
      if(PtInEllipse((CRect)m_t_Rect, p))
         return HT_INSIDE;

   return HT_NONE;
}

void CVecEllipse::DrawDragImage(CDC* pDC,LPDRECT lpRectNew, LPDRECT lpRectLast, LPDRECT lpRectOrigianl)
{
   CDRect rect = m_t_Rect;
   CRect* lpLocalRectNew = NULL;
   CRect* lpLocalRectLast = NULL;

   if(lpRectNew!=NULL)
   {
      CDPOINT Points[2];
      Points[0] = rect.TopLeft();
      Points[1] = rect.BottomRight();

      ScaleAndOffsetPoint(&Points[0], lpRectNew, lpRectOrigianl);
      ScaleAndOffsetPoint(&Points[1], lpRectNew, lpRectOrigianl);
      lpLocalRectNew = new CRect(Points[0],Points[1]);
      lpLocalRectNew->NormalizeRect();
   }

   if(lpRectLast!=NULL)
   {
      CDPOINT Points[2];
      Points[0] = rect.TopLeft();
      Points[1] = rect.BottomRight();

      ScaleAndOffsetPoint(&Points[0], lpRectLast, lpRectOrigianl);
      ScaleAndOffsetPoint(&Points[1], lpRectLast, lpRectOrigianl);
      lpLocalRectLast = new CRect(Points[0],Points[1]);
      lpLocalRectLast->NormalizeRect();
   }

   CEllipseTracker::DrawDragEllipse(pDC, lpLocalRectNew, 1, lpLocalRectLast, 1);

   if(lpLocalRectNew != NULL)
      delete lpLocalRectNew;
   if(lpLocalRectLast != NULL)
      delete lpLocalRectLast;

}

void CVecEllipse::Rotate(double dAng, LPDPOINT PtCenter)
{
   CDRect rect = GetRect();
   CDPoint Point; 
   Point = rect.CenterPoint();

   CVecBase::Rotate(dAng, PtCenter);
   RotatePoints(&Point, 1, dAng, PtCenter);

   CDRect drect(Point.x - rect.Width()/2, Point.y - rect.Height()/2, Point.x + rect.Width()/2, Point.y + rect.Height()/2);
   m_Rect = drect;
}

void CVecEllipse::Flip(BOOL bHorizontal, LPDPOINT PtCenter)
{
   CDRect rect = GetRect();
   CDPoint Point; 
   Point = rect.CenterPoint();

   CVecBase::Flip(bHorizontal, PtCenter);
   FlipPoints(&Point, 1, bHorizontal, PtCenter);

   CDRect drect(Point.x - rect.Width()/2, Point.y - rect.Height()/2, Point.x + rect.Width()/2, Point.y + rect.Height()/2);
   m_Rect = drect;
}

void CVecEllipse::DrawRotatedDragImage(CDC* pDC, double dAng, LPPOINT lpCenter, double dAngLast, LPPOINT lpCenterLast)
{
   CRect rect = m_t_Rect;
   CRect* lpLocalRectNew = NULL;
   CRect* lpLocalRectLast = NULL;

   if(lpCenter!=NULL)
   {
      POINT Point;
      Point = rect.CenterPoint();
      RotatePoints(&Point, 1, dAng, lpCenter);
      lpLocalRectNew = new CRect(Point.x - rect.Width()/2, Point.y - rect.Height()/2, Point.x + rect.Width()/2, Point.y + rect.Height()/2);
      lpLocalRectNew->NormalizeRect();
   }

   if(lpCenterLast!=NULL)
   {
      POINT Point;
      Point = rect.CenterPoint();
      RotatePoints(&Point, 1, dAngLast, lpCenterLast);
      lpLocalRectLast = new CRect(Point.x - rect.Width()/2, Point.y - rect.Height()/2, Point.x + rect.Width()/2, Point.y + rect.Height()/2);
      lpLocalRectLast->NormalizeRect();
   }

   CEllipseTracker::DrawDragEllipse(pDC, lpLocalRectNew, 1, lpLocalRectLast, 1);

   if(lpLocalRectNew != NULL)
      delete lpLocalRectNew;
   if(lpLocalRectLast != NULL)
      delete lpLocalRectLast;

}
