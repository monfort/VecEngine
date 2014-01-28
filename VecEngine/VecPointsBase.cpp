/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// VecPointsBase.cpp: implementation of the CVecPointsBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VecPointsBase.h"
#include "G_Routines.h"
#include "MemBuffEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVecPointsBase::CVecPointsBase()
{
   m_lpPoints = NULL;
   m_t_lpPoints = NULL;
   m_t_lpIntPoints = NULL;
   m_uiNumOfPoints = 0;
   m_bSetPointsDontDoRecursiv = FALSE;
   m_bSetRectDontDoRecursiv = FALSE;
}

CVecPointsBase::~CVecPointsBase()
{
   if(m_lpPoints!=NULL)
      delete []m_lpPoints;
   if(m_t_lpPoints!=NULL)
      delete []m_t_lpPoints;
   if(m_t_lpIntPoints!=NULL)
      delete []m_t_lpIntPoints;

}

void CVecPointsBase::Copy(CVecBase* pVec)
{
   CVecBase::Copy(pVec);//call Parent copy routine

   CVecPointsBase* pVecPointsBase = dynamic_cast<CVecPointsBase*>(pVec);
   if(pVecPointsBase!=NULL)
   {
      if(m_lpPoints!=NULL)
         delete []m_lpPoints;
      if(m_t_lpPoints!=NULL)
         delete []m_t_lpPoints;
      if(m_t_lpIntPoints!=NULL)
         delete []m_t_lpIntPoints;

      m_uiNumOfPoints = pVecPointsBase->m_uiNumOfPoints;

      if(pVecPointsBase->m_lpPoints!=NULL)
      {
         m_lpPoints = new CDPOINT[m_uiNumOfPoints];
   	   memcpy(m_lpPoints, pVecPointsBase->m_lpPoints, m_uiNumOfPoints*sizeof(CDPOINT));
      }

      if(pVecPointsBase->m_t_lpPoints!=NULL)
      {
         m_t_lpPoints = new CDPOINT[m_uiNumOfPoints];
   	   memcpy(m_t_lpPoints, pVecPointsBase->m_t_lpPoints, m_uiNumOfPoints*sizeof(CDPOINT));
      }

      if(pVecPointsBase->m_t_lpIntPoints!=NULL)
      {
         m_t_lpIntPoints = new POINT[m_uiNumOfPoints];
   	   memcpy(m_t_lpIntPoints, pVecPointsBase->m_t_lpIntPoints, m_uiNumOfPoints*sizeof(POINT));
      }
   }

}

void CVecPointsBase::DoTransform()
{
   CVecBase::DoTransform();

	memcpy(m_t_lpPoints, m_lpPoints, m_uiNumOfPoints*sizeof(CDPOINT));
   UINT ui;
   for(ui = 0; ui < m_uiNumOfPoints; ui++)
   {
      m_t_lpPoints[ui] *= m_t_Scaler;
      m_t_lpPoints[ui] += m_t_Offset;
   }

   for(ui = 0; ui < m_uiNumOfPoints; ui++)
      m_t_lpIntPoints[ui] = m_t_lpPoints[ui];

}

void CVecPointsBase::SetPointsByScreen(LPPOINT lpPoints, UINT uiCount)
{
	ASSERT(AfxIsValidAddress(lpPoints, sizeof(POINT)*uiCount, FALSE));
   LPCDPOINT lpDPoints = new CDPOINT[uiCount];

   for(UINT ui = 0; ui < uiCount; ui++)
   {
      lpDPoints[ui] = lpPoints[ui];
      lpDPoints[ui] -= m_t_Offset;
      lpDPoints[ui] /= m_t_Scaler;
   }

   SetPoints(lpDPoints, uiCount);

   delete []lpDPoints;
}

void CVecPointsBase::SetPointsByScreen(LPCDPOINT lpPoints, UINT uiCount)
{
	ASSERT(AfxIsValidAddress(lpPoints, sizeof(CDPOINT)*uiCount, FALSE));
   LPCDPOINT lpDPoints = new CDPOINT[uiCount];

   for(UINT ui = 0; ui < uiCount; ui++)
   {
      lpDPoints[ui] = lpPoints[ui];
      lpDPoints[ui] -= m_t_Offset;
      lpDPoints[ui] /= m_t_Scaler;
   }

   SetPoints(lpDPoints, uiCount);

   delete []lpDPoints;
}

void CVecPointsBase::SetPoints(LPCDPOINT lpPoints, UINT uiCount)
{
   if(m_bSetPointsDontDoRecursiv)
      return;

	ASSERT(AfxIsValidAddress(lpPoints, sizeof(CDPOINT)*uiCount, FALSE));

   if(uiCount != m_uiNumOfPoints)
   {
      if(m_lpPoints!=NULL)
         delete []m_lpPoints;
      if(m_t_lpPoints!=NULL)
         delete []m_t_lpPoints;
      if(m_t_lpIntPoints!=NULL)
         delete []m_t_lpIntPoints;

      m_uiNumOfPoints = uiCount;
      m_lpPoints = new CDPOINT[m_uiNumOfPoints];
      m_t_lpPoints = new CDPOINT[m_uiNumOfPoints];
      m_t_lpIntPoints = new POINT[m_uiNumOfPoints];
   }

   memcpy(m_lpPoints, lpPoints, m_uiNumOfPoints*sizeof(CDPOINT));

   //calc the bounding rect
   CDRect rect = GetPointsBBox(m_lpPoints, m_uiNumOfPoints);

   m_bSetPointsDontDoRecursiv = TRUE;
   CVecBase::SetRect(rect);
   m_bSetPointsDontDoRecursiv = FALSE;
}

void CVecPointsBase::GetPoints(LPCDPOINT lpPoints)
{
	ASSERT(AfxIsValidAddress(lpPoints, sizeof(CDPOINT)*m_uiNumOfPoints, FALSE));
	memcpy(lpPoints, m_lpPoints, m_uiNumOfPoints*sizeof(CDPOINT));
}


void CVecPointsBase::SetRect(CDRect &rect)
{
   if(m_bSetRectDontDoRecursiv)
      return;


   CDRect newRect = rect;
   CDRect oldRect = GetRect();
   //oldRect.NormalizeRect();
   //newRect.NormalizeRect();

   CDPoint OffsetOld(oldRect.left, oldRect.top);
   CDPoint OffsetNew(newRect.left, newRect.top);
   CDPoint Scale(newRect.Width() / oldRect.Width(), newRect.Height() / oldRect.Height());

   LPCDPOINT dpoints = new CDPOINT[m_uiNumOfPoints];
	memcpy(dpoints, m_lpPoints, m_uiNumOfPoints*sizeof(CDPOINT));
   for(UINT ui = 0; ui < m_uiNumOfPoints; ui++)
   {
      dpoints[ui] -= OffsetOld;
      dpoints[ui] *= Scale;
      dpoints[ui] += OffsetNew;
   }

	memcpy(m_lpPoints, dpoints, m_uiNumOfPoints*sizeof(CDPOINT));

   CDRect drect = GetPointsBBox(m_lpPoints, m_uiNumOfPoints);
   CVecBase::SetRect(drect);

   delete []dpoints;
}


void CVecPointsBase::Rotate(double dAng, LPDPOINT ptCenter)
{
   m_bSetRectDontDoRecursiv = TRUE;
   CVecBase::Rotate(dAng, ptCenter);
   m_bSetRectDontDoRecursiv = FALSE;

   RotatePoints(m_lpPoints, m_uiNumOfPoints, dAng, ptCenter);
   m_Rect = GetPointsBBox(m_lpPoints, m_uiNumOfPoints);
}

void CVecPointsBase::Flip(BOOL bHorizontal, LPDPOINT PtCenter)
{
   CVecBase::Flip(bHorizontal, PtCenter);
   FlipPoints(m_lpPoints, m_uiNumOfPoints, bHorizontal, PtCenter);
   CDRect drect = GetPointsBBox(m_lpPoints, m_uiNumOfPoints);
   m_Rect = GetPointsBBox(m_lpPoints, m_uiNumOfPoints);
}

void CVecPointsBase::SaveToBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
{
   pMemBuff->WriteNodeHeader(VEC_PARENT);
   CVecBase::SaveToBuffer(pMemBuff, iFlags);

   pMemBuff->WriteField(VF_Points, m_lpPoints, m_uiNumOfPoints*sizeof(CDPoint));

   pMemBuff->WriteEndOfFields();
}

void CVecPointsBase::LoadFromBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags)
{
   while(pMemBuff->ReadNextFieldHeader())
   {
      int id = pMemBuff->GetFieldType();
      int size = pMemBuff->GetFieldSize();
      switch(id)
      {
      case VF_Points:
         {
            UINT uiCount = size/sizeof(CDPoint);
            LPCDPOINT lpPoints = new CDPOINT[uiCount];
            pMemBuff->Read(lpPoints, uiCount*sizeof(CDPoint));
            SetPoints(lpPoints, uiCount);
            delete []lpPoints;
         }
         break;
      case ID_FIELD_IS_NODE: 
	      switch (pMemBuff->GetNodeType())
	      {
         case VEC_PARENT: 
            CVecBase::LoadFromBuffer(pMemBuff, iFlags);
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
