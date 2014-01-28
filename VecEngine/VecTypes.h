/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// VecTypes.h: interface for the VecTypes class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECTYPES_H__9AC864FD_B6BC_11D3_A496_00C0CA1261A6__INCLUDED_)
#define AFX_VECTYPES_H__9AC864FD_B6BC_11D3_A496_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDSize;
class CDPoint;
class CDRect;

typedef struct tagDSIZE
{
    double cx;
    double cy;
} DSIZE, *PDSIZE, *LPDSIZE;

typedef struct tagDRECT
{
    double    left;
    double    top;
    double    right;
    double    bottom;
} DRECT, *PDRECT, NEAR *NPDRECT, FAR *LPDRECT;

typedef const DRECT FAR* LPCDRECT;

// This is a separate structure rather than being part of CDPoint so that
// it can be used to create arrays with initil values...
struct DPOINT
{
    double x;
    double y;
};

// For backward compatablity...
#define LPDPOINT CDPoint*

class CDPoint : 
	public DPOINT
{
public:
// Constructors

	// create an uninitialized point
	CDPoint();
	// create from two integers
	CDPoint(double initX, double initY);
	// create from a size
	CDPoint(DSIZE initSize);
	// create fromF a dword: x = LOWORD(dw) y = HIWORD(dw)
	CDPoint(DWORD dwPoint);

// Operations

// translate the point
	void Offset(double xOffset, double yOffset);
	void Offset(DPOINT point);
	void Offset(DSIZE size);

	BOOL operator==(DPOINT point) const;
	BOOL operator!=(DPOINT point) const;
	void operator+=(DSIZE size);
	void operator-=(DSIZE size);
	void operator+=(DPOINT point);

// Operators returning CDPoint values
	CDPoint operator+(DSIZE size) const;
	CDPoint operator-(DSIZE size) const;
	CDPoint operator-() const;
	CDPoint operator+(DPOINT point) const;

// Operators returning CDSize values
	CDSize operator-(DPOINT point) const;

// Operators returning CDRect values
	CDRect operator+(const DRECT* lpRect) const;
	CDRect operator-(const DRECT* lpRect) const;

   ////////////////////
   inline void operator*=(const CDPoint p) {x*=p.x; y*=p.y;}
   inline void operator-=(const CDPoint p) {x-=p.x; y-=p.y;}
   inline void operator/=(const CDPoint p) {x/=p.x; y/=p.y;}
   CDPoint(POINT p){x=p.x; y=p.y;}
   inline operator CPoint() const {return CPoint((int)x, (int)y);}

} ;

#define LPCDPOINT CDPoint*
#define CDPOINT CDPoint

class CDSize : public tagDSIZE
{
public:

// Constructors
	// construct an uninitialized size
	CDSize();
	// create from two integers
	CDSize(double initCX, double initCY);
	// create from another size
	CDSize(DSIZE initSize);
	// create from a point
	CDSize(DPOINT initPt);
	// create from a DWORD: cx = LOWORD(dw) cy = HIWORD(dw)
	CDSize(DWORD dwSize);

// Operations
	BOOL operator==(DSIZE size) const;
	BOOL operator!=(DSIZE size) const;
	void operator+=(DSIZE size);
	void operator-=(DSIZE size);

// Operators returning CDSize values
	CDSize operator+(DSIZE size) const;
	CDSize operator-(DSIZE size) const;
	CDSize operator-() const;

// Operators returning CDPoint values
	CDPoint operator+(DPOINT point) const;
	CDPoint operator-(DPOINT point) const;

// Operators returning CDRect values
	CDRect operator+(const DRECT* lpRect) const;
	CDRect operator-(const DRECT* lpRect) const;
};

/////////////////////////////////////////////////////////////////////////////
// CDRect - A 2-D rectangle, similar to Windows DRECT structure.

typedef const DRECT* LPCDRECT;    // pointer to read/only DRECT

class CDRect : public tagDRECT
{
public:

// Constructors

	// uninitialized rectangle
	CDRect();
	// from left, top, right, and bottom
	CDRect(double l, double t, double r, double b);
	// copy constructor
	CDRect(const DRECT& srCDRect);
	// from a pointer to another rect
	CDRect(LPCDRECT lpSrCDRect);
	// from a point and size
	CDRect(DPOINT point, DSIZE size);
	// from two points
	CDRect(DPOINT topLeft, DPOINT bottomRight);

// Attributes (in addition to DRECT members)

	// retrieves the width
	double Width() const;
	// returns the height
	double Height() const;
	// returns the size
	CDSize Size() const;
	// reference to the top-left point
	CDPoint& TopLeft();
	// reference to the bottom-right point
	CDPoint& BottomRight();
	// const reference to the top-left point
	const CDPoint& TopLeft() const;
	// const reference to the bottom-right point
	const CDPoint& BottomRight() const;
	// the geometric center point of the rectangle
	CDPoint CenterPoint() const;
	// swap the left and right
	void SwapLeftRight();
	static void SwapLeftRight(LPDRECT lpRect);

	// convert between CDRect and LPDRECT/LPCDRECT (no need for &)
	operator LPDRECT();
	operator LPCDRECT() const;

	// returns TRUE if rectangle has no area
	BOOL IsRectEmpty() const;
	// returns TRUE if rectangle is at (0,0) and has no area
	BOOL IsRectNull() const;
	// returns TRUE if point is within rectangle
	BOOL PtInRect(DPOINT point) const;

// Operations

	// set rectangle from left, top, right, and bottom
	static void SetRect(LPDRECT lpRect, double x1, double y1, double x2, double y2);
	void SetRect(double x1, double y1, double x2, double y2);
	void SetRect(DPOINT topLeft, DPOINT bottomRight);
	// empty the rectangle
	void SetRectEmpty();
	// copy from another rectangle
	void CopyRect(LPCDRECT lpSrCDRect);
	// TRUE if exactly the same as another rectangle
	BOOL EqualRect(LPCDRECT lpRect) const;
   static BOOL EqualRectStatic(const LPCDRECT lpRect1, const LPCDRECT lpRect2);

	// inflate rectangle's width and height without
	// moving its top or left
	static void InflateRect(LPDRECT lpRect, double x, double y);
	void InflateRect(double x, double y);
	void InflateRect(DSIZE size);
	void InflateRect(LPCDRECT lpRect);
	void InflateRect(double l, double t, double r, double b);
	// deflate the rectangle's width and height without
	// moving its top or left
   static void DeflateRect(LPDRECT lpRect, double x, double y);
	void DeflateRect(double x, double y);
	void DeflateRect(DSIZE size);
	void DeflateRect(LPCDRECT lpRect);
	void DeflateRect(double l, double t, double r, double b);

	// translate the rectangle by moving its top and left
	void OffsetRect(double x, double y);
   static CDRect OffsetRect(LPDRECT pr, double x, double y);
   static BOOL CopyRect(LPDRECT lprcDst, CONST DRECT *lprcSrc);

	void OffsetRect(DSIZE size);
	void OffsetRect(DPOINT point);
	void NormalizeRect();

	// set this rectangle to intersection of two others
	BOOL IntersectRect(LPCDRECT lpRect1, LPCDRECT lpRect2);

	// set this rectangle to bounding union of two others
	BOOL UnionRect(LPCDRECT lpRect1, LPCDRECT lpRect2);

	// set this rectangle to minimum of two others
	BOOL SubtractRect(LPCDRECT lpRectSrc1, LPCDRECT lpRectSrc2);

// Additional Operations
	void operator=(const DRECT& srCDRect);
	BOOL operator==(const DRECT& rect) const;
	BOOL operator!=(const DRECT& rect) const;
	void operator+=(DSIZE size);
	void operator+=(LPCDRECT lpRect);
	void operator-=(DPOINT point);
	void operator-=(DSIZE size);
	void operator-=(LPCDRECT lpRect);
	void operator&=(const DRECT& rect);
	void operator|=(const DRECT& rect);

// Operators returning CDRect values
	CDRect operator+(DPOINT point) const;
	CDRect operator-(DPOINT point) const;
	CDRect operator+(LPCDRECT lpRect) const;
	CDRect operator+(DSIZE size) const;
	CDRect operator-(DSIZE size) const;
	CDRect operator-(LPCDRECT lpRect) const;
	CDRect operator&(const DRECT& rect2) const;
	CDRect operator|(const DRECT& rect2) const;
	CDRect MulDiv(double nMultiplier, double nDivisor) const;

///////////////
   inline void operator*=(const CDPoint p) {left*=p.x; right*=p.x; top*=p.y; bottom*=p.y;}
   inline void operator/=(const CDPoint p) {left/=p.x; right/=p.x; top/=p.y; bottom/=p.y;}
   CDRect(RECT r){left=r.left; right=r.right; top=r.top; bottom=r.bottom;}
   inline operator CRect() const {return CRect((int)left, (int)top, (int)right, (int)bottom);}
   inline void operator+=(const CDPoint p) {left+=p.x; right+=p.x; top+=p.y; bottom+=p.y;}
};

// CDSize
inline CDSize::CDSize()
	{ /* random filled */ }
inline CDSize::CDSize(double initCX, double initCY)
	{ cx = initCX; cy = initCY; }
inline CDSize::CDSize(DSIZE initSize)
	{ *(DSIZE*)this = initSize; }
inline CDSize::CDSize(DPOINT initPt)
	{ *(DPOINT*)this = initPt; }
inline CDSize::CDSize(DWORD dwSize)
	{
		cx = (short)LOWORD(dwSize);
		cy = (short)HIWORD(dwSize);
	}
inline BOOL CDSize::operator==(DSIZE size) const
	{ return (cx == size.cx && cy == size.cy); }
inline BOOL CDSize::operator!=(DSIZE size) const
	{ return (cx != size.cx || cy != size.cy); }
inline void CDSize::operator+=(DSIZE size)
	{ cx += size.cx; cy += size.cy; }
inline void CDSize::operator-=(DSIZE size)
	{ cx -= size.cx; cy -= size.cy; }
inline CDSize CDSize::operator+(DSIZE size) const
	{ return CDSize(cx + size.cx, cy + size.cy); }
inline CDSize CDSize::operator-(DSIZE size) const
	{ return CDSize(cx - size.cx, cy - size.cy); }
inline CDSize CDSize::operator-() const
	{ return CDSize(-cx, -cy); }
inline CDPoint CDSize::operator+(DPOINT point) const
	{ return CDPoint(cx + point.x, cy + point.y); }
inline CDPoint CDSize::operator-(DPOINT point) const
	{ return CDPoint(cx - point.x, cy - point.y); }
inline CDRect CDSize::operator+(const DRECT* lpRect) const
	{ return CDRect(lpRect) + *this; }
inline CDRect CDSize::operator-(const DRECT* lpRect) const
	{ return CDRect(lpRect) - *this; }


// CDPoint
inline CDPoint::CDPoint()
	{ /* random filled */ }
inline CDPoint::CDPoint(double initX, double initY)
	{ x = initX; y = initY; }
inline CDPoint::CDPoint(DSIZE initSize)
	{ *(DSIZE*)this = initSize; }
inline CDPoint::CDPoint(DWORD dwPoint)
	{
		x = (short)LOWORD(dwPoint);
		y = (short)HIWORD(dwPoint);
	}
inline void CDPoint::Offset(double xOffset, double yOffset)
	{ x += xOffset; y += yOffset; }
inline void CDPoint::Offset(DPOINT point)
	{ x += point.x; y += point.y; }
inline void CDPoint::Offset(DSIZE size)
	{ x += size.cx; y += size.cy; }
inline BOOL CDPoint::operator==(DPOINT point) const
	{ return (x == point.x && y == point.y); }
inline BOOL CDPoint::operator!=(DPOINT point) const
	{ return (x != point.x || y != point.y); }
inline void CDPoint::operator+=(DSIZE size)
	{ x += size.cx; y += size.cy; }
inline void CDPoint::operator-=(DSIZE size)
	{ x -= size.cx; y -= size.cy; }
inline void CDPoint::operator+=(DPOINT point)
	{ x += point.x; y += point.y; }
inline CDPoint CDPoint::operator+(DSIZE size) const
	{ return CDPoint(x + size.cx, y + size.cy); }
inline CDPoint CDPoint::operator-(DSIZE size) const
	{ return CDPoint(x - size.cx, y - size.cy); }
inline CDPoint CDPoint::operator-() const
	{ return CDPoint(-x, -y); }
inline CDPoint CDPoint::operator+(DPOINT point) const
	{ return CDPoint(x + point.x, y + point.y); }
inline CDSize CDPoint::operator-(DPOINT point) const
	{ return CDSize(x - point.x, y - point.y); }
inline CDRect CDPoint::operator+(const DRECT* lpRect) const
	{ return CDRect(lpRect) + *this; }
inline CDRect CDPoint::operator-(const DRECT* lpRect) const
	{ return CDRect(lpRect) - *this; }

// CDRect
inline BOOL CDRect::CopyRect(LPDRECT lprcDst, CONST DRECT *lprcSrc)
   {lprcDst->bottom=lprcSrc->bottom; lprcDst->top=lprcSrc->top; lprcDst->left=lprcSrc->left; lprcDst->right=lprcSrc->right; return TRUE;}
inline CDRect::CDRect()
	{ /* random filled */ }
inline CDRect::CDRect(double l, double t, double r, double b)
	{ left = l; top = t; right = r; bottom = b; }
inline CDRect::CDRect(const DRECT& srCDRect)
	{ CopyRect(this, &srCDRect); }
inline CDRect::CDRect(LPCDRECT lpSrCDRect)
	{ CopyRect(this, lpSrCDRect); }
inline CDRect::CDRect(DPOINT point, DSIZE size)
	{ right = (left = point.x) + size.cx; bottom = (top = point.y) + size.cy; }
inline CDRect::CDRect(DPOINT topLeft, DPOINT bottomRight)
	{ left = topLeft.x; top = topLeft.y;
		right = bottomRight.x; bottom = bottomRight.y; }
inline double CDRect::Width() const
	{ return right - left; }
inline double CDRect::Height() const
	{ return bottom - top; }
inline CDSize CDRect::Size() const
	{ return CDSize(right - left, bottom - top); }
inline CDPoint& CDRect::TopLeft()
	{ return *((CDPoint*)this); }
inline CDPoint& CDRect::BottomRight()
	{ return *((CDPoint*)this+1); }
inline const CDPoint& CDRect::TopLeft() const
	{ return *((CDPoint*)this); }
inline const CDPoint& CDRect::BottomRight() const
	{ return *((CDPoint*)this+1); }
inline CDPoint CDRect::CenterPoint() const
	{ return CDPoint((left+right)/2, (top+bottom)/2); }
inline void CDRect::SwapLeftRight()
	{ SwapLeftRight(LPDRECT(this)); }
inline void CDRect::SwapLeftRight(LPDRECT lpRect)
	{ double temp = lpRect->left; lpRect->left = lpRect->right; lpRect->right = temp; }
inline CDRect::operator LPDRECT()
	{ return this; }
inline CDRect::operator LPCDRECT() const
	{ return this; }
inline BOOL CDRect::IsRectEmpty() const
	{ return (Width() == 0 && Height() == 0); }
inline BOOL CDRect::IsRectNull() const
	{ return (left == 0 && right == 0 && top == 0 && bottom == 0); }
inline BOOL CDRect::PtInRect(DPOINT point) const
	{ return (left<=point.x && right>=point.x && top<=point.y && bottom>=point.y);}
inline void CDRect::SetRect(LPDRECT lpRect, double x1, double y1, double x2, double y2)
{lpRect->left = x1; lpRect->top = y1; lpRect->right = x2; lpRect->bottom = y2;}

inline void CDRect::SetRect(double x1, double y1, double x2, double y2)
	{ SetRect(this, x1, y1, x2, y2); }
inline void CDRect::SetRect(DPOINT topLeft, DPOINT bottomRight)
	{ SetRect(this, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y); }
inline void CDRect::SetRectEmpty()
	{ left = 0; right = 0; top = 0; bottom = 0; }
inline void CDRect::CopyRect(LPCDRECT lpSrCDRect)
	{ CopyRect(this, lpSrCDRect); }
inline BOOL CDRect::EqualRectStatic(const LPCDRECT lpRect1, const LPCDRECT lpRect2)
   {return (lpRect1->top == lpRect2->top && lpRect1->right == lpRect2->right && lpRect1->left == lpRect2->left && lpRect1->bottom == lpRect2->bottom);}
inline BOOL CDRect::EqualRect(LPCDRECT lpRect) const
	{ return EqualRectStatic(this, lpRect); }
inline void CDRect::InflateRect(LPDRECT lpRect, double x, double y)
{lpRect->top-=y; lpRect->bottom+=y; lpRect->left-=x; lpRect->right+=x;}
inline void CDRect::InflateRect(double x, double y)
	{ InflateRect(this, x, y); }
inline void CDRect::InflateRect(DSIZE size)
	{ InflateRect(this, size.cx, size.cy); }
inline void CDRect::DeflateRect(LPDRECT lpRect, double x, double y)
{lpRect->top+=y; lpRect->bottom-=y; lpRect->left+=x; lpRect->right-=x;}
inline void CDRect::DeflateRect(double x, double y)
	{ InflateRect(this, -x, -y); }
inline void CDRect::DeflateRect(DSIZE size)
	{ InflateRect(this, -size.cx, -size.cy); }
inline void CDRect::OffsetRect(double x, double y)
	{ right+=x; left+=x; top+=y; bottom+=y;}
inline CDRect CDRect::OffsetRect(LPDRECT pr, double x, double y)
	{ return CDRect(pr->right+x, pr->top+y, pr->left+x, pr->bottom+y);}
inline void CDRect::OffsetRect(DPOINT point)
	{ OffsetRect(point.x, point.y); }
inline void CDRect::OffsetRect(DSIZE size)
	{ OffsetRect(size.cx, size.cy); }
/*inline BOOL CDRect::IntersectRect(LPCDRect lpRect1, LPCDRect lpRect2)
	{ return ::IntersectRect(this, lpRect1, lpRect2);}*/
/*inline void CDRect::operator=(const DRECT& srCDRect)
	{ ::CopyRect(this, &srCDRect); }*/
inline BOOL CDRect::operator==(const DRECT& rect) const
	{ return EqualRectStatic(this, &rect); }
inline BOOL CDRect::operator!=(const DRECT& rect) const
	{ return !EqualRectStatic(this, &rect); }
inline void CDRect::operator+=(DSIZE size)
	{ OffsetRect(size.cx, size.cy); }
/*inline void CDRect::operator+=(LPCDRect lpRect)
	{ InflateRect(lpRect); }*/
inline void CDRect::operator-=(DPOINT point)
	{ OffsetRect(-point.x, -point.y); }
inline void CDRect::operator-=(DSIZE size)
	{ OffsetRect(-size.cx, -size.cy); }
/*inline void CDRect::operator-=(LPCDRect lpRect)
	{ DeflateRect(lpRect); }
inline void CDRect::operator&=(const DRECT& rect)
	{ ::IntersectRect(this, this, &rect); }
inline void CDRect::operator|=(const DRECT& rect)
	{ ::UnionRect(this, this, &rect); }*/
inline CDRect CDRect::operator+(DPOINT pt) const
	{ 
   CDRect rect(*this); 
   OffsetRect(&rect, pt.x, pt.y); 
   return rect; 
}
inline CDRect CDRect::operator-(DPOINT pt) const
	{ CDRect rect(*this); OffsetRect(&rect, -pt.x, -pt.y); return rect; }
inline CDRect CDRect::operator+(DSIZE size) const
	{ CDRect rect(*this); OffsetRect(&rect, size.cx, size.cy); return rect; }
inline CDRect CDRect::operator-(DSIZE size) const
	{ CDRect rect(*this); OffsetRect(&rect, -size.cx, -size.cy); return rect; }
/*inline CDRect CDRect::operator+(LPCDRect lpRect) const
	{ CDRect rect(this); rect.InflateRect(lpRect); return rect; }
inline CDRect CDRect::operator-(LPCDRect lpRect) const
	{ CDRect rect(this); rect.DeflateRect(lpRect); return rect; }
inline CDRect CDRect::operator&(const DRECT& rect2) const
	{ CDRect rect; ::IntersectRect(&rect, this, &rect2);
		return rect; }
inline CDRect CDRect::operator|(const DRECT& rect2) const
	{ CDRect rect; ::UnionRect(&rect, this, &rect2);
		return rect; }
inline BOOL CDRect::SubtractRect(LPCDRect lpRectSrc1, LPCDRect lpRectSrc2)
	{ return ::SubtractRect(this, lpRectSrc1, lpRectSrc2); }*/

typedef struct tagDRECTANGLE
{
    DPOINT    TopLeft;
    DPOINT    TopRight;
    DPOINT    BottomRight;
    DPOINT    BottomLeft;
} DRECTANGLE, *PDRECTANGLE, NEAR *NPDRECTANGLE, FAR *LPDRECTANGLE;

typedef const DRECTANGLE FAR* LPCDRECTANGLE;

typedef struct tagRECTANGLE
{
    POINT    TopLeft;
    POINT    TopRight;
    POINT    BottomRight;
    POINT    BottomLeft;
} RECTANGLE, *PRECTANGLE, NEAR *NPRECTANGLE, FAR *LPRECTANGLE;

typedef const DRECTANGLE FAR* LPCDRECTANGLE;

#define DRECTANGLE_POINTS(d) ((LPDPOINT)d)
#define DRECTANGLE_NUM_OF_POINTS 4

#define RECTANGLE_POINTS(d) ((LPPOINT)d)
#define RECTANGLE_NUM_OF_POINTS 4

template <class TBASE, class TRECT,class TCRect, class TCPoint, class TOb>
class TRectangle :
	public TBASE
{
public:
	TRectangle();
	// copy constructor
	TRectangle(const TBASE& srRectangle);

	TRectangle(const TRECT& srRect);
	// from a pointer to another rect
	TRectangle(TCRect* lpSrCDRect);
	// from two points
	TRectangle(TCPoint ptTopLeft, TCPoint ptTopRight, TCPoint ptBottomRight, TCPoint ptBottomLeft);
   
   BOOL IsNullRectangle();
   BOOL IsRectangleEmpty();
   void SetRectangleEmpty();

   inline TCPoint* GetPoints(){return ((TCPoint*)this);}
   inline static int GetNumOfPoints(){return DRECTANGLE_NUM_OF_POINTS;}

	void operator+=(TCPoint point);
	void operator-=(TCPoint point);
   void operator*=(const TCPoint point);
   void operator/=(const TCPoint point);

	RECTANGLE GetRectangle();
	DRECTANGLE GetDRectangle();

   BOOL IsEqual(const TBASE& srRectangle);
   TCPoint GetSizes();
   double GetAngle1();
   double GetAngle2();
};

#include "G_Routines.h"

template <class TBASE, class TRECT,class TCRect, class TCPoint, class TOb>
inline TRectangle<TBASE, TRECT,TCRect, TCPoint, TOb>::TRectangle()
	{ /* random filled */ }
//------------------------------------------------------------------------------
template <class TBASE, class TRECT,class TCRect, class TCPoint, class TOb>
inline TRectangle<TBASE, TRECT,TCRect, TCPoint, TOb>::
TRectangle(const TBASE& srCDRectangle)
{  
   TopLeft = srCDRectangle.TopLeft;
   TopRight = srCDRectangle.TopRight;
   BottomLeft = srCDRectangle.BottomLeft;
   BottomRight = srCDRectangle.BottomRight;
}
//------------------------------------------------------------------------------
template <class TBASE, class TRECT,class TCRect, class TCPoint, class TOb>
inline TRectangle<TBASE, TRECT,TCRect, TCPoint, TOb>::TRectangle(const TRECT& srCDRect)
	{  
   TopLeft.x = srCDRect.left;
   TopLeft.y = srCDRect.top;
   BottomLeft.x = srCDRect.left;
   BottomLeft.y = srCDRect.bottom;
   TopRight.x = srCDRect.right;
   TopRight.y = srCDRect.top;
   BottomRight.x = srCDRect.right;
   BottomRight.y = srCDRect.bottom;
   }
template <class TBASE, class TRECT,class TCRect, class TCPoint, class TOb>
inline TRectangle<TBASE, TRECT,TCRect, TCPoint, TOb>::TRectangle(TCRect* lpSrCDRect)
	{ 
   TopLeft.x = lpSrCDRect->left;
   TopLeft.y = lpSrCDRect->top;
   BottomLeft.x = lpSrCDRect->left;
   BottomLeft.y = lpSrCDRect->bottom;
   TopRight.x = lpSrCDRect->right;
   TopRight.y = lpSrCDRect->top;
   BottomRight.x = lpSrCDRect->right;
   BottomRight.y = lpSrCDRect->bottom;
   }
//------------------------------------------------------------------------------
template <class TBASE, class TRECT,class TCRect, class TCPoint, class TOb>
inline TRectangle<TBASE, TRECT,TCRect, TCPoint, TOb>::
TRectangle(TCPoint ptTopLeft, TCPoint ptTopRight, TCPoint ptBottomRight, TCPoint ptBottomLeft)
	{ 
   TopLeft = ptTopLeft;
   BottomLeft = ptBottomLeft;
   TopRight = ptTopRight;
   BottomRight = ptBottomRight;
   }

template <class TBASE, class TRECT,class TCRect, class TCPoint, class TOb>
inline void TRectangle<TBASE, TRECT,TCRect, TCPoint, TOb>::operator+=(TCPoint point)
{
   TopLeft.x += point.x;
   TopLeft.y += point.y;
   BottomLeft.x += point.x;
   BottomLeft.y += point.y;
   TopRight.x += point.x;
   TopRight.y += point.y;
   BottomRight.x += point.x;
   BottomRight.y += point.y;
}

template <class TBASE, class TRECT,class TCRect, class TCPoint, class TOb>
inline void TRectangle<TBASE, TRECT,TCRect, TCPoint, TOb>::operator-=(TCPoint point)
{
   TopLeft.x -= point.x;
   TopLeft.y -= point.y;
   BottomLeft.x -= point.x;
   BottomLeft.y -= point.y;
   TopRight.x -= point.x;
   TopRight.y -= point.y;
   BottomRight.x -= point.x;
   BottomRight.y -= point.y;
}
template <class TBASE, class TRECT,class TCRect, class TCPoint, class TOb>
inline void TRectangle<TBASE, TRECT,TCRect, TCPoint, TOb>::operator*=(const TCPoint point) 
{
   TopLeft.x *= point.x;
   TopLeft.y *= point.y;
   BottomLeft.x *= point.x;
   BottomLeft.y *= point.y;
   TopRight.x *= point.x;
   TopRight.y *= point.y;
   BottomRight.x *= point.x;
   BottomRight.y *= point.y;
}
//------------------------------------------------------------------------------
template <class TBASE, class TRECT,class TCRect, class TCPoint, class TOb>
inline void TRectangle<TBASE, TRECT,TCRect, TCPoint, TOb>::
operator/=(const TCPoint point) 
{
   TopLeft.x /= point.x;
   TopLeft.y /= point.y;
   BottomLeft.x /= point.x;
   BottomLeft.y /= point.y;
   TopRight.x /= point.x;
   TopRight.y /= point.y;
   BottomRight.x /= point.x;
   BottomRight.y /= point.y;
}
//------------------------------------------------------------------------------
template <class TBASE, class TRECT,class TCRect, class TCPoint, class TOb>
inline BOOL TRectangle<TBASE, TRECT,TCRect, TCPoint, TOb>::IsRectangleEmpty() 
{
   return (
   TopLeft.x == BottomLeft.x && BottomLeft.x == TopRight.x && TopRight.x == BottomRight.x &&
   TopLeft.y == BottomLeft.y && BottomLeft.y == TopRight.y && TopRight.y == BottomRight.y);
}
template <class TBASE, class TRECT,class TCRect, class TCPoint, class TOb>
inline BOOL TRectangle<TBASE, TRECT,TCRect, TCPoint, TOb>::IsNullRectangle() 
{
   return (
   TopLeft.x ==0 &&
   TopLeft.y ==0 &&
   BottomLeft.x ==0 &&
   BottomLeft.y ==0 &&
   TopRight.x ==0 &&
   TopRight.y ==0 &&
   BottomRight.x ==0 &&
   BottomRight.y == 0);
}
template <class TBASE, class TRECT,class TCRect, class TCPoint, class TOb>
inline void TRectangle<TBASE, TRECT,TCRect, TCPoint, TOb>::SetRectangleEmpty() 
{
   TopLeft.x =0;
   TopLeft.y =0;
   BottomLeft.x =0;
   BottomLeft.y =0;
   TopRight.x =0;
   TopRight.y =0;
   BottomRight.x =0;
   BottomRight.y =0;
}
template <class TBASE, class TRECT,class TCRect, class TCPoint, class TOb>
inline DRECTANGLE TRectangle<TBASE, TRECT,TCRect, TCPoint, TOb>::GetDRectangle()
{
   DRECTANGLE rect;
   rect.TopLeft.x = TopLeft.x;
   rect.TopRight.x = TopRight.x;
   rect.BottomLeft.x = BottomLeft.x;
   rect.BottomRight.x = BottomRight.x;
   rect.TopLeft.y = TopLeft.y;
   rect.TopRight.y = TopRight.y;
   rect.BottomLeft.y = BottomLeft.y;
   rect.BottomRight.y = BottomRight.y;
   return drect;
}
template <class TBASE, class TRECT,class TCRect, class TCPoint, class TOb>
inline RECTANGLE TRectangle<TBASE, TRECT,TCRect, TCPoint, TOb>::GetRectangle()
{
   RECTANGLE rect;
   rect.TopLeft.x = (long)TopLeft.x;
   rect.TopRight.x = (long)TopRight.x;
   rect.BottomLeft.x = (long)BottomLeft.x;
   rect.BottomRight.x = (long)BottomRight.x;
   rect.TopLeft.y = (long)TopLeft.y;
   rect.TopRight.y = (long)TopRight.y;
   rect.BottomLeft.y = (long)BottomLeft.y;
   rect.BottomRight.y = (long)BottomRight.y;
   return rect;
}
template <class TBASE, class TRECT,class TCRect, class TCPoint, class TOb>
inline BOOL TRectangle<TBASE, TRECT,TCRect, TCPoint, TOb>::IsEqual(const TBASE& srCDRectangle)
	{  
   return 
   (TopLeft.x == srCDRectangle.TopLeft.x &&
   TopRight.x == srCDRectangle.TopRight.x &&
   BottomLeft.x == srCDRectangle.BottomLeft.x &&
   BottomRight.x == srCDRectangle.BottomRight.x &&
   TopLeft.y == srCDRectangle.TopLeft.y &&
   TopRight.y == srCDRectangle.TopRight.y &&
   BottomLeft.y == srCDRectangle.BottomLeft.y &&
   BottomRight.y == srCDRectangle.BottomRight.y);
   }
template <class TBASE, class TRECT,class TCRect, class TCPoint, class TOb>
inline TCPoint TRectangle<TBASE, TRECT,TCRect, TCPoint, TOb>::GetSizes()
{
   TCPoint sizes;
   sizes.x = (TOb)pitagoran((double)TopRight.x - TopLeft.x, 
      (double)TopRight.y - TopLeft.y);
   sizes.y = (TOb)pitagoran((double)BottomLeft.x - TopLeft.x, 
      (double)BottomLeft.y - TopLeft.y);
   return sizes;
}
template <class TBASE, class TRECT,class TCRect, class TCPoint, class TOb>
inline double TRectangle<TBASE, TRECT,TCRect, TCPoint, TOb>::GetAngle1()
{
   return GetPointAng(&TopRight, &TopLeft);
}
template <class TBASE, class TRECT,class TCRect, class TCPoint, class TOb>
inline double TRectangle<TBASE, TRECT,TCRect, TCPoint, TOb>::GetAngle2()
{
   return GetPointAng(&BottomLeft, &TopLeft);
}


typedef TRectangle<tagRECTANGLE, RECT, CRect, CPoint, long> CRectangle;
typedef TRectangle<tagDRECTANGLE, DRECT, CDRect, CDPoint, double> CDRectangle;

/*class CRectangle : public TRectangle<tagRECTANGLE, RECT, CRect, POINT, CPoint, int>
{
};

class CDRectangle : public TRectangle<tagDRECTANGLE, DRECT, CDRect, DPOINT, CDPoint, double>
{
};*/

#endif // !defined(AFX_VECTYPES_H__9AC864FD_B6BC_11D3_A496_00C0CA1261A6__INCLUDED_)
