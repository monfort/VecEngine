#include "VecTypes.h"

#undef abs
#include "math.h"

#ifndef G_ROUTINES_INCLUDED
#define G_ROUTINES_INCLUDED

BOOL G_PtInPolygon(POINT *rgpts, WORD wnumpts, POINT ptTest, RECT *prbound);
void G_CalcBoundingRect(POINT *rgpts, WORD wnumpts, RECT *prbound) ;
BOOL Intersect(POINT p1, POINT p2, POINT p3, POINT p4) ;
int  CCW(POINT p0, POINT p1, POINT p2) ;

BOOL PtInEllipse(LPCRECT lpRect, POINT point);

BOOL PtOnLine(CPoint point, CPoint lineP1, CPoint lineP2, int iWidth);

void ScaleAndOffsetPoint(CDPoint* lpPoint, const LPDRECT lpNewRect, const LPDRECT lpOrgRect);
void ScaleAndOffsetPoint(LPPOINT lpPoint, const LPRECT lpNewRect, const LPRECT lpOrgRect);
void ScaleAndOffsetPoint(CDPoint* lpPoint, const LPRECT lpNewRect, const LPRECT lpOrgRect);
void ScaleAndOffsetPoint(LPPOINT lpPoint, const LPDRECT lpNewRect, const LPDRECT lpOrgRect);

void RotatePoints(CDPoint* lpPoint, UINT uiCount, double dAng, CDPoint* ptCenter);
void RotatePoints(LPPOINT lpPoint, UINT uiCount, double dAng, LPPOINT ptCenter);

CDRect GetPointsBBox(CDPoint* lpPoint, UINT uiCount);
CRect GetPointsBBox(LPPOINT lpPoint, UINT uiCount);
CDPoint GetPointsCenter(CDPoint* lpPoint, UINT uiCount);
CPoint GetPointsCenter(LPPOINT lpPoint, UINT uiCount);

CDRect GetRotatedBoundingBox(LPCDRECT lpSrcRect, double dAng, LPCDPOINT lpCenter);
CRect GetRotatedBoundingBox(LPCRECT lpSrcRect, double dAng, LPPOINT lpCenter);
CDRect GetFlippedBoundingBox(LPCDRECT lpSrcRect, BOOL bHorizontal, LPCDPOINT lpCenter);
CRect GetFlippedBoundingBox(LPCRECT lpSrcRect, BOOL bHorizontal, LPPOINT lpCenter);

const double PI = 3.1415926535897932384626433832795;
double pitagoran(const double a, const double b);


inline double RadToDeg(const double ang) {return ang*(180/PI);}
inline double DegToRad(const double ang) {return (ang*PI)/180;}

template<class t> t ABS(t d) { return (d>0 ? d : -d);}

//90 degrees in radians
const double RAD90D = (PI/2);

//------------------------------------------------------------------------------
inline double safeAtan2(const double y, const double x)
{
   if(x==0)
   {
      if(y>0)
         return RAD90D;
      else
         return -RAD90D;
   }
   else
      return atan2(y, x);
}
//------------------------------------------------------------------------------
template <class CPointType>
double GetPointAng(const CPointType* pPoint, const CPointType* ptCenter)
{
	CDPoint point(pPoint->x, pPoint->y);
	point.x -= ptCenter->x;
	point.y -= ptCenter->y;

	return safeAtan2(point.y, point.x);
}
//------------------------------------------------------------------------------
inline double pitagoran(const double a, const double b)
{
   return sqrt(a*a+b*b);
}
//------------------------------------------------------------------------------
template<class CPointType>
void FlipPoints(CPointType* lpPoint, UINT uiCount, BOOL bHorizontal, const CPointType* ptCenter)
{
   CPointType center;
   if(ptCenter!=NULL)
      center = *ptCenter;
   else
      center = GetPointsCenter(lpPoint, uiCount);

   if(bHorizontal)
   {
      for(UINT ui = 0; ui<uiCount; ui++)
         lpPoint[ui].x = center.x - (lpPoint[ui].x - center.x);
   }
   else
   {
      for(UINT ui = 0; ui<uiCount; ui++)
         lpPoint[ui].y = center.y - (lpPoint[ui].y - center.y);
   }
}
//------------------------------------------------------------------------------
#endif G_ROUTINES_INCLUDED

