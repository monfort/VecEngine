// VecRectangle.cpp: implementation of the CVecRectangle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VecRectangle.h"
#include "VecContainer.h"
#include "UndoManager.h"
#include "PolygonTracker.h"
#include "RectangleTracker.h"

#ifdef TRACK_AS_RECT
   #include "MRectTracker.h"
#else
   #include "SuperTracker.h"
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVecRectangle::CVecRectangle()
{
   m_uiType = VEC_RECTANGLE;

#ifdef TRACK_AS_RECT
   m_bTrackAsShape = TRUE;
#else
   m_bTrackAsShape = FALSE;
#endif


   CDPOINT lpPoints[4];
   lpPoints[0] = CDPoint(0,0);//top left
   lpPoints[1] = CDPoint(100,0);//top right
   lpPoints[2] = CDPoint(100,100);//bottom right
   lpPoints[3] = CDPoint(0,100);//bottom left
   SetPoints(lpPoints, 4);
   m_MappingRectangle = CDRectangle(*(LPDRECTANGLE)lpPoints);
}

CVecRectangle::~CVecRectangle()
{
}

#ifndef TRACK_AS_RECT

BOOL CVecRectangle::Track(CDPoint point, UINT uiTool, CUndoManager* pUndoMan, CSnapper* pSnapper)
{
   CRectangleTracker rt(*((LPDRECTANGLE)m_t_lpPoints));
   if(HasPicture())
      rt.SetTrackRetainProportions();
   if(rt.Track(GetTopContainer()->GetWnd(), point, pSnapper))
   {
      pUndoMan->AddToStack(this, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
      SetPointsByScreen((LPCDPOINT)rt.m_rect.GetPoints(), rt.m_rect.GetNumOfPoints());
      return TRUE;
   }

   return FALSE;
}

#else

void CVecRectangle::Draw(CDC * pDC, DWORD dwFlags)
{
   PreperDC(pDC);
   pDC->Polygon(m_t_lpIntPoints,m_uiNumOfPoints);
   RestoreDC(pDC);
}

void CVecRectangle::DrawPoints(CDC * pDC)
{
   CMRectTracker rt((CRect)m_t_Rect, CMRectTracker::resizeOutside);
   rt.Draw(pDC);
}

BOOL CVecRectangle::Track(CDPoint point, UINT uiTool, CUndoManager* pUndoMan)
{
   CMRectTracker rt((CRect)m_t_Rect, CMRectTracker::solidLine | CMRectTracker::resizeOutside);
   int iHit = rt.HitTest(point);
   if(iHit != CMRectTracker::hitMiddle && iHit != CMRectTracker::hitNothing)
   {//track the bounding rect
      if(rt.Track(GetTopContainer()->GetWnd(), point, TRUE))
      {
         pUndoMan->AddToStack(this, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
         rt.m_rect.NormalizeRect();
         CDRect drect(rt.m_rect);
         drect -= m_t_Offset;
         drect /= m_t_Scaler;
         SetRect(drect);
         return TRUE;
      }
   }
   else
   {//track as polygon
      return CVecPolygon::Track(point, uiTool);
   }

   return FALSE;
}

BOOL CVecRectangle::SetCursor(CWnd* pWnd, UINT nHitTest, UINT uiTool)
{
   CPoint point;
   GetCursorPos(&point);
   pWnd->ScreenToClient(&point);

   CMRectTracker rt((CRect)m_t_Rect, CMRectTracker::solidLine | CMRectTracker::resizeOutside);
   int iHit = rt.HitTest(point);
   if(iHit != CMRectTracker::hitMiddle && iHit != CMRectTracker::hitNothing)
      rt.SetCursor(pWnd, nHitTest);//show cursor of the rect tracker
   else
      CVecPolygon::SetCursor(pWnd, nHitTest);//show the cursor of the polygon tracker
   return TRUE;

}

HitTestResoult CVecRectangle::HitTest(CDPoint &p)
{
   CMRectTracker rt((CRect)m_t_Rect, CMRectTracker::solidLine | CMRectTracker::resizeOutside);
   int iHit = rt.HitTest(p);
   if(iHit != CMRectTracker::hitMiddle && iHit != CMRectTracker::hitNothing && IsSelected())
      return HT_ON_POINT;
   else
   {
      return CVecPolygon::HitTest(p);
   }

   return HT_NONE;
}

#endif

