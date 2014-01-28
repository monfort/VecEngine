/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// VecPolygon.cpp: implementation of the CVecPolygon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VecPolygon.h"
#include "PolygonTracker.h"
#include "VecContainer.h"
#include "G_Routines.h"
#include "SuperTracker.h"
#include "MRectTracker.h"
#include "MemBuffEx.h"
#include "UndoManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define TRACKER_STYLE (m_bTrackAsShape? ~CPolygonTracker::styleShowPoints : CPolygonTracker::styleShowPoints)

CVecPolygon::CVecPolygon()
{
   m_TrackerStyle = CPolygonTracker::styleShowPoints; 
   m_bTrackAsShape = FALSE;
   m_uiType = VEC_POLYGON;
}

CVecPolygon::~CVecPolygon()
{
}

void CVecPolygon::Copy(CVecBase* pVec)
{
   CVecPointsBase::Copy(pVec);//call Parent copy routine

   CVecPolygon* pVecPolygon = dynamic_cast<CVecPolygon*>(pVec);
   if(pVecPolygon!=NULL)
   {
      m_bTrackAsShape = pVecPolygon->m_bTrackAsShape;
      m_TrackerStyle = pVecPolygon->m_TrackerStyle;
   }
}

void CVecPolygon::Draw(CDC * pDC, DWORD dwFlags)
{
   PreperDC(pDC);
   pDC->Polygon(m_t_lpIntPoints,m_uiNumOfPoints);
   RestoreDC(pDC);
}

void CVecPolygon::DrawShadow(CDC * pDC, DWORD dwFlags)
{
   PreperDC(pDC, TRUE);
   pDC->Polygon(m_t_lpIntPoints,m_uiNumOfPoints);
   RestoreDC(pDC);
}

void CVecPolygon::DrawPoints(CDC * pDC)
{
   if(m_bTrackAsShape)
   {
      CMRectTracker rt(m_t_Rect, CMRectTracker::resizeOutside | CMRectTracker::cornersOnly);
      rt.Draw(pDC);
   }
   else
   {
      CPolygonTracker rt(m_t_lpPoints,m_uiNumOfPoints, TRACKER_STYLE);
      rt.DrawDots(pDC);
   }
}

BOOL CVecPolygon::Track(CDPoint point, UINT uiTool, CUndoManager* pUndoMan, CSnapper* pSnapper)
{
   if(m_bTrackAsShape)
   {
      CPtrList list;
      list.AddHead(this);
      CSuperTracker st(list);
      if(HasPicture())
         st.SetTrackRetainProportions();
      if(st.Track(GetTopContainer()->GetWnd(), point, 
         HitTest(point) == HT_ON_POINT ? CSuperTracker::Style_Size: CSuperTracker::Style_Move, pSnapper))
      {
         pUndoMan->AddToStack(this, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
         SetRectScaleAndOffsetByScreen(st.m_rect, st.m_rectOrigianl);
         return TRUE;
      }
   }
   else
   {
      CPolygonTracker rt(m_t_lpPoints, m_uiNumOfPoints, TRACKER_STYLE);
      int iHit = rt.HitTestPoints(point);
      BOOL bTrackResoult;
      if(iHit >= CPolygonTracker::hitDot)
      {//track only the selected point and the points next to it
         ASSERT(m_uiNumOfPoints>=3);
         int aPointLocation[3];
         CDPOINT aPointsData[3];
         if(iHit == 0)
         {  
            aPointLocation[0] = m_uiNumOfPoints-1;
            aPointLocation[1] = 0;
            aPointLocation[2] = 1;
         }
         else if(iHit == (int)m_uiNumOfPoints-1)
         {
            aPointLocation[0] = m_uiNumOfPoints-2;
            aPointLocation[1] = m_uiNumOfPoints-1;
            aPointLocation[2] = 0;
         }
         else
         {
            aPointLocation[0] = iHit-1;
            aPointLocation[1] = iHit;
            aPointLocation[2] = iHit+1;
         }

         for(int i=0; i<3; i++)
            aPointsData[i] = m_t_lpPoints[aPointLocation[i]];

         CLineTracker localTracker(aPointsData, 3);
         bTrackResoult = localTracker.Track(GetTopContainer()->GetWnd(), point, pSnapper);
         if(bTrackResoult)
         {
            for(int i=0; i<3; i++)
               m_t_lpPoints[aPointLocation[i]] = aPointsData[i];
         }
      }
      else
      {
         if(HasPicture())
            rt.SetTrackRetainProportions();
         bTrackResoult = rt.Track(GetTopContainer()->GetWnd(), point, pSnapper);
      }

      if(bTrackResoult)
      {
         pUndoMan->AddToStack(this, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
         LPCDPOINT dpoints = new CDPOINT[m_uiNumOfPoints];
         for(UINT ui = 0; ui < m_uiNumOfPoints; ui++)
         {
            dpoints[ui].x = m_t_lpPoints[ui].x;
            dpoints[ui].y = m_t_lpPoints[ui].y;
            dpoints[ui] -= m_t_Offset;
            dpoints[ui] /= m_t_Scaler;
         }
         SetPoints(dpoints, m_uiNumOfPoints);

         delete []dpoints;

         return TRUE;
      }
   }

   return FALSE;
}

BOOL CVecPolygon::SetCursor(CWnd* pWnd, UINT nHitTest, UINT uiTool)
{
   if(m_bTrackAsShape)
   {
      CPoint point;
      GetCursorPos(&point);
      pWnd->ScreenToClient(&point);

      CMRectTracker rt(m_t_Rect, CMRectTracker::resizeOutside | CMRectTracker::cornersOnly);
      int iHit = rt.HitTest(point);
      if(iHit != CMRectTracker::hitMiddle && iHit != CMRectTracker::hitNothing)
      {
         rt.SetCursor(pWnd, nHitTest);//show cursor of the rect tracker
         return TRUE;
      }
   }

   CPolygonTracker rt(m_t_lpPoints, m_uiNumOfPoints, TRACKER_STYLE);
   rt.SetCursor(pWnd, nHitTest);

   return TRUE;
}

HitTestResoult CVecPolygon::HitTest(CDPoint &p)
{
   if(m_bTrackAsShape)
   {
      CMRectTracker rt(m_t_Rect, CMRectTracker::resizeOutside | CMRectTracker::cornersOnly);
      int iHit = rt.HitTest(p);
      if(iHit != CMRectTracker::hitMiddle && iHit != CMRectTracker::hitNothing && IsSelected())
         return HT_ON_POINT;
   }

   CPolygonTracker rt(m_t_lpPoints, m_uiNumOfPoints, TRACKER_STYLE);
   int iHit = rt.HitTest(p);
   switch(iHit)
   {
   case CPolygonTracker::hitInside:
      return HT_INSIDE;
   case CPolygonTracker::hitLine:
      return HT_ON_LINE;
   case CPolygonTracker::hitNothing:
      return HT_NONE;
   default:
      return HT_ON_POINT;
   }
   //return HT_NONE;
}



void CVecPolygon::DrawDragImage(CDC* pDC,LPDRECT lpRectNew, LPDRECT lpRectLast, LPDRECT lpRectOrigianl)
{
   LPPOINT lpPointsNew = NULL;
   LPPOINT lpPointsLast = NULL;

   if(lpRectNew!=NULL)
   {
      lpPointsNew = new POINT[m_uiNumOfPoints];
      for(UINT ui = 0; ui< m_uiNumOfPoints; ui++)
      {
         CDPoint dpoint = m_t_lpPoints[ui];
         ScaleAndOffsetPoint(&dpoint, lpRectNew, lpRectOrigianl);
         lpPointsNew[ui] = dpoint;
      }
   }

   if(lpRectLast!=NULL)
   {
      lpPointsLast = new POINT[m_uiNumOfPoints];
      for(UINT ui = 0; ui< m_uiNumOfPoints; ui++)
      {
         CDPoint dpoint = m_t_lpPoints[ui];
         ScaleAndOffsetPoint(&dpoint, lpRectLast, lpRectOrigianl);
         lpPointsLast[ui] = dpoint;
      }
   }

   CPolygonTracker::DrawDragPolygon(pDC, lpPointsNew, 1, lpPointsLast, 1, m_uiNumOfPoints);

   if(lpPointsNew != NULL)
      delete []lpPointsNew;
   if(lpPointsLast != NULL)
      delete []lpPointsLast;

}

void CVecPolygon::DrawRotatedDragImage(CDC* pDC, double dAng, LPPOINT lpCenter, double dAngLast, LPPOINT lpCenterLast)
{
   CRect rect = m_t_Rect;
   LPPOINT lpPointsNew = NULL;
   LPPOINT lpPointsLast = NULL;

   if(lpCenter!=NULL)
   {
      lpPointsNew = new POINT[m_uiNumOfPoints];
   	memcpy(lpPointsNew, m_t_lpIntPoints, m_uiNumOfPoints*sizeof(POINT));
      RotatePoints(lpPointsNew, m_uiNumOfPoints, dAng, lpCenter);
   }

   if(lpCenterLast!=NULL)
   {
      lpPointsLast = new POINT[m_uiNumOfPoints];
   	memcpy(lpPointsLast, m_t_lpIntPoints, m_uiNumOfPoints*sizeof(POINT));
      RotatePoints(lpPointsLast, m_uiNumOfPoints, dAngLast, lpCenterLast);
   }

   CPolygonTracker::DrawDragPolygon(pDC, lpPointsNew, 1, lpPointsLast, 1, m_uiNumOfPoints);

   if(lpPointsNew != NULL)
      delete []lpPointsNew;
   if(lpPointsLast != NULL)
      delete []lpPointsLast;

}

void CVecPolygon::SaveToBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
{
   pMemBuff->WriteNodeHeader(VEC_PARENT);
   CVecPointsBase::SaveToBuffer(pMemBuff, iFlags);

   pMemBuff->WriteIntField(VF_TrackerStyle, m_TrackerStyle);
   pMemBuff->WriteBoolField(VF_TrackAsShape, m_bTrackAsShape);

   pMemBuff->WriteEndOfFields();
}

void CVecPolygon::LoadFromBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
{
   while(pMemBuff->ReadNextFieldHeader())
   {
      int id = pMemBuff->GetFieldType();
      switch(id)
      {
      case VF_TrackerStyle:
         m_TrackerStyle = pMemBuff->ReadInt();
         break;
      case VF_TrackAsShape:
         m_bTrackAsShape = pMemBuff->ReadBool();
         break;
      case ID_FIELD_IS_NODE: 
	      switch (pMemBuff->GetNodeType())
	      {
         case VEC_PARENT: 
            CVecPointsBase::LoadFromBuffer(pMemBuff, iFlags);
            break;
	      default: 
            pMemBuff->SkipField(); 
            break;
	      } 
         break;
      default: 
         pMemBuff->SkipField(); 
         break;
      }
   }
}
