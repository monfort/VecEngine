/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// VecPointsBase.h: interface for the CVecPointsBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECPOINTSBASE_H__68B8AF25_C0DA_11D3_A49D_00C0CA1261A6__INCLUDED_)
#define AFX_VECPOINTSBASE_H__68B8AF25_C0DA_11D3_A49D_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VecBase.h"

class CVecPointsBase : public CVecBase  
{
public:
	CVecPointsBase();
	virtual ~CVecPointsBase();
   inline UINT GetPointsCount(){return m_uiNumOfPoints;}
   void SetPoints(LPCDPOINT lpPoints, UINT uiCount);
   void GetPoints(LPCDPOINT lpPoints);
   void SetPointsByScreen(LPPOINT lpPoints, UINT uiCount);
   void SetPointsByScreen(LPCDPOINT lpPoints, UINT uiCount);
   virtual void DoTransform();
   virtual void SetRect(CDRect &rect);
   virtual void Rotate(double dAng, LPDPOINT PtCenter = NULL);
   virtual void Flip(BOOL bHorizontal, LPDPOINT PtCenter = NULL);
   virtual void Copy(CVecBase* pVec);
   virtual void SaveToBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags = SBF_FULL);
   virtual void LoadFromBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags = SBF_FULL);

public://types
   virtual BOOL HasPoints(){return TRUE;}

protected:
   LPPOINT m_t_lpIntPoints;
   LPCDPOINT m_lpPoints;
   LPCDPOINT m_t_lpPoints;
   UINT m_uiNumOfPoints;

   BOOL m_bSetPointsDontDoRecursiv;
   BOOL m_bSetRectDontDoRecursiv;

private:
   enum VEC_FIELDS // fields ids (file type) - do not cheange values
   {
      VF_Points = 0
   };
};

#endif // !defined(AFX_VECPOINTSBASE_H__68B8AF25_C0DA_11D3_A49D_00C0CA1261A6__INCLUDED_)
