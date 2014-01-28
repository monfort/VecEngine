/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// EllipseTracker.cpp: implementation of the CEllipseTracker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EllipseTracker.h"
#include "VecTypes.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEllipseTracker::CEllipseTracker(LPCDRECT lpSrcRect, UINT nStyle):
CMRectTracker(lpSrcRect, nStyle)
{
}

CEllipseTracker::~CEllipseTracker()
{

}

int CEllipseTracker::HitTest(CPoint point) const
{
	TrackerHit hitResult = hitNothing;

	CRect rectTrue;
	GetTrueRect(&rectTrue);
	ASSERT(rectTrue.left <= rectTrue.right);
	ASSERT(rectTrue.top <= rectTrue.bottom);
	if (rectTrue.PtInRect(point))
	{
		if ((m_nStyle & (resizeInside|resizeOutside)) != 0)
			hitResult = (TrackerHit)HitTestHandles(point);
		else
			hitResult = hitMiddle;
	}
	return hitResult;
}

void CEllipseTracker::DoDrawTracker(CDC* pDC, LPCRECT lpRect, SIZE size,	LPCRECT lpRectLast, SIZE sizeLast)
{
   DrawDragEllipse(pDC, lpRect, size.cx, lpRectLast, sizeLast.cx);
}


void CEllipseTracker::DrawDragEllipse(CDC* pDC, LPCRECT lpRect, int iWidth,
	LPCRECT lpRectLast, int iWidthLast)
{
	ASSERT(lpRect == NULL || AfxIsValidAddress(lpRect, sizeof(RECT), FALSE));
	ASSERT(lpRectLast == NULL || AfxIsValidAddress(lpRectLast, sizeof(RECT), FALSE));


   CGdiObject* pOldBrush = pDC->SelectStockObject(NULL_BRUSH);
   if (lpRectLast != NULL && iWidthLast>0)
	{
	   CPen Pen(PS_SOLID, iWidthLast, RGB(255,255,255));
		/*CPen* pPenOld = */(CPen*)pDC->SelectObject(Pen);
      int iRop2Old = pDC->SetROP2(R2_NOT);
      pDC->Ellipse(lpRectLast);
      pDC->SetROP2(iRop2Old);
		//pDC->SelectObject(pPenOld);
	}

   // draw into the update/new region
	if (lpRect != NULL && iWidth>0)
	{
	   CPen Pen(PS_SOLID, iWidth, RGB(255,255,255));
	   /*CPen* pPenOld = */(CPen*)pDC->SelectObject(Pen);
      int iRop2Old = pDC->SetROP2(R2_NOT);
      pDC->Ellipse(lpRect);
      pDC->SetROP2(iRop2Old);
   	//pDC->SelectObject(pPenOld);
   }
   pDC->SelectObject(pOldBrush);
}

