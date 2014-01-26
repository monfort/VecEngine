// VecTypes.cpp: implementation of the VecTypes class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VecTypes.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



void CDRect::NormalizeRect()
{
	double nTemp;
	if (left > right)
	{
		nTemp = left;
		left = right;
		right = nTemp;
	}
	if (top > bottom)
	{
		nTemp = top;
		top = bottom;
		bottom = nTemp;
	}
}

void CDRect::InflateRect(LPCDRECT lpRect)
{
	left -= lpRect->left;
	top -= lpRect->top;
	right += lpRect->right;
	bottom += lpRect->bottom;
}

void CDRect::InflateRect(double l, double t, double r, double b)
{
	left -= l;
	top -= t;
	right += r;
	bottom += b;
}

void CDRect::DeflateRect(LPCDRECT lpRect)
{
	left += lpRect->left;
	top += lpRect->top;
	right -= lpRect->right;
	bottom -= lpRect->bottom;
}

void CDRect::DeflateRect(double l, double t, double r, double b)
{
	left += l;
	top += t;
	right -= r;
	bottom -= b;
}

/*CDRect CDRect::MulDiv(double nMultiplier, double nDivisor) const
{
	return CDRect(
		::MulDiv(left, nMultiplier, nDivisor),
		::MulDiv(top, nMultiplier, nDivisor),
		::MulDiv(right, nMultiplier, nDivisor),
		::MulDiv(bottom, nMultiplier, nDivisor));
}
*/

BOOL CDRect::UnionRect(LPCDRECT lpRect1, LPCDRECT lpRect2)
{
   left = lpRect1->left;
   if(left>lpRect2->left)   
      left = lpRect2->left;

   right = lpRect1->right;
   if(right<lpRect2->right)   
      right = lpRect2->right;

   top = lpRect1->top;
   if(top>lpRect2->top)   
      top = lpRect2->top;

   bottom = lpRect1->bottom;
   if(bottom<lpRect2->bottom)   
      bottom = lpRect2->bottom;

   return IsRectNull();
}
