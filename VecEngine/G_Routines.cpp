#include "stdafx.h"
#include "G_Routines.h"

 /*************************************************************************

   * FUNCTION:   G_PtInPolygon
   *
   * PURPOSE
   * This routine determines if the point passed is in the polygon. It uses

   * the classical polygon hit-testing algorithm: a horizontal ray starting

   * at the point is extended infinitely rightwards and the number of
  * polygon edges that intersect the ray are counted. If the number is odd,

   * the point is inside the polygon.
   *
   * RETURN VALUE
   * (BOOL) TRUE if the point is inside the polygon, FALSE if not.
 *************************************************************************/ 

  BOOL G_PtInPolygon(POINT *rgpts, WORD wnumpts, POINT ptTest,

                   RECT *prbound)

  {

   RECT   r;
   POINT  *ppt ;
   WORD   i ;
   POINT  pt2 ;
   WORD   wnumintsct = 0 ;

   if(prbound!=NULL)
      r = *prbound;
   else
      G_CalcBoundingRect(rgpts,wnumpts,&r);

   if (!PtInRect(&r,ptTest))
      return FALSE ;

   pt2.y = ptTest.y;
   pt2.x = r.right + 50;

   // Now go through each of the lines in the polygon and see if it
   // intersects
   for (i = 0, ppt = rgpts ; i < wnumpts-1 ; i++, ppt++)
   {
      if (Intersect(ptTest, pt2, *ppt, *(ppt+1)))
         wnumintsct++ ;
   }

   // And the last line
   if (Intersect(ptTest, pt2, *ppt, *rgpts))
      wnumintsct++ ;

   return (wnumintsct&1) ;

   }

/*************************************************************************

   * FUNCTION:   G_PtInPolyRect
   *
   * PURPOSE
   * This routine determines if a point is within the smallest rectangle
   * that encloses a polygon.
   *
   * RETURN VALUE
   * (BOOL) TRUE or FALSE depending on whether the point is in the rect or

   * not.
 *************************************************************************/ 

void G_CalcBoundingRect(POINT *rgpts, WORD wnumpts, RECT *prbound)
{
   int   xmin, xmax, ymin, ymax ;
   POINT *ppt ;
   WORD  i ;

   xmin = ymin = INT_MAX ;
   xmax = ymax = -INT_MAX ;

   for (i=0, ppt = rgpts ; i < wnumpts ; i++, ppt++)
   {
      if (ppt->x < xmin)
         xmin = ppt->x ;
      if (ppt->x > xmax)
         xmax = ppt->x ;
      if (ppt->y < ymin)
         ymin = ppt->y ;
      if (ppt->y > ymax)
         ymax = ppt->y ;
   }
   SetRect(prbound, xmin, ymin, xmax, ymax) ;

}

/*************************************************************************
   * FUNCTION:   Intersect
   *
   * PURPOSE
   * Given two line segments, determine if they intersect.
   *
   * RETURN VALUE
   * TRUE if they intersect, FALSE if not.
 *************************************************************************/ 

  BOOL Intersect(POINT p1, POINT p2, POINT p3, POINT p4)
  {

   return ((( CCW(p1, p2, p3) * CCW(p1, p2, p4)) <= 0)

        && (( CCW(p3, p4, p1) * CCW(p3, p4, p2)  <= 0) )) ;

   }

/*************************************************************************

   * FUNCTION:   CCW (CounterClockWise)
   *
   * PURPOSE
   * Determines, given three points, if when travelling from the first to
   * the second to the third, we travel in a counterclockwise direction.
   *
   * RETURN VALUE
   * (int) 1 if the movement is in a counterclockwise direction, -1 if
   * not.
 *************************************************************************/ 

  int CCW(POINT p0, POINT p1, POINT p2)

   {

   LONG dx1, dx2 ;
   LONG dy1, dy2 ;

   dx1 = p1.x - p0.x ; dx2 = p2.x - p0.x ;
   dy1 = p1.y - p0.y ; dy2 = p2.y - p0.y ;

   /* This is basically a slope comparison: we don't do divisions because

    * of divide by zero possibilities with pure horizontal and pure
    * vertical lines.
    */ 

   return ((dx1 * dy2 < dy1 * dx2) ? 1 : -1) ;

   }


BOOL PtInEllipse(LPCRECT lpRect, POINT point) 

{ 
   CRect rect = *lpRect;

   if (!rect.PtInRect (point)) 
      return FALSE; 

   CPoint ptCenter; 

   ptCenter.x = rect.left + (rect.Width () / 2); 

   ptCenter.y = rect.top + (rect.Height () / 2); 

   int cx = point.x - ptCenter.x; 

   int cy = point.y - ptCenter.y; 

   if ((cx * cx) + (cy * cy) < (rect.Width () / 2) * (rect.Width () / 2)) 
      return TRUE; 

   return FALSE; 

} 

BOOL PtOnLine(CPoint point, CPoint lineP1, CPoint lineP2, int iWidth) 
{
   //get the bounding rect of the line
   CRect rect(lineP1, lineP2);

   //normalize the rect correct 0 width of height cases
   rect.NormalizeRect();
   if(rect.Width() <= 1)
   {
      rect.right+=1; rect.left-=1;
   }
   else if(rect.Height() <= 1)
   {
      rect.bottom+=1; rect.top-=1;
   }
   rect.right+=1; rect.left-=1;
   rect.bottom+=1; rect.top-=1;

   //check if the point is in the bounding recangle of the line
   if(!rect.PtInRect(point))
      return FALSE;

   //calc deltas
   double dx = lineP1.x - lineP2.x;
   double dy = lineP1.y - lineP2.y;
   if(abs((int)dx)>abs((int)dy))//find out wich dimantion is bigger
   {
      double dSlop = (dy) / (dx);
      double pty = (point.x - lineP1.x) * dSlop + lineP1.y;

      if(pty <= point.y + iWidth && pty >= point.y - iWidth)
         return TRUE;
   }
   else
   {
      double dSlop = (dx) / (dy);
      double ptx = (point.y - lineP1.y) * dSlop + lineP1.x;

      if(ptx <= point.x + iWidth && ptx >= point.x - iWidth)
         return TRUE;
   }
   return FALSE;
}

void ScaleAndOffsetPoint(LPDPOINT lpPoint, const LPDRECT lpNewRect, const LPDRECT lpOrgRect)
{
   lpPoint->x -= lpOrgRect->left;
   lpPoint->y -= lpOrgRect->top;

   double dX = lpOrgRect->right - lpOrgRect->left;
   if(dX==0)
      lpPoint->x = 0;
   else
      lpPoint->x *= ((lpNewRect->right - lpNewRect->left) / dX);

   double dY = lpOrgRect->bottom - lpOrgRect->top;
   if(dY==0)
      lpPoint->y = 0;
   else
      lpPoint->y *= ((lpNewRect->bottom - lpNewRect->top) / dY);

   lpPoint->x += lpNewRect->left;
   lpPoint->y += lpNewRect->top;
}

void ScaleAndOffsetPoint(LPPOINT lpPoint, const LPRECT lpNewRect, const LPRECT lpOrgRect)
{
   lpPoint->x -= lpOrgRect->left;
   lpPoint->y -= lpOrgRect->top;

   double dX = lpOrgRect->right - lpOrgRect->left;
   if(dX==0)
      lpPoint->x = 0;
   else
      lpPoint->x = (long)(lpPoint->x * (lpNewRect->right - lpNewRect->left) / dX);

   double dY = lpOrgRect->bottom - lpOrgRect->top;
   if(dY==0)
      lpPoint->y = 0;
   else
      lpPoint->y = (long)(lpPoint->y * (lpNewRect->bottom - lpNewRect->top) / dY);

   lpPoint->x += lpNewRect->left;
   lpPoint->y += lpNewRect->top;
}

void ScaleAndOffsetPoint(LPDPOINT lpPoint, const LPRECT lpNewRect, const LPRECT lpOrgRect)
{
   lpPoint->x -= lpOrgRect->left;
   lpPoint->y -= lpOrgRect->top;

   double dX = lpOrgRect->right - lpOrgRect->left;
   if(dX==0)
      lpPoint->x = 0;
   else
      lpPoint->x = (long)(lpPoint->x * (lpNewRect->right - lpNewRect->left) / dX);

   double dY = lpOrgRect->bottom - lpOrgRect->top;
   if(dY==0)
      lpPoint->y = 0;
   else
      lpPoint->y = (long)(lpPoint->y * (lpNewRect->bottom - lpNewRect->top) / dY);

   lpPoint->x += lpNewRect->left;
   lpPoint->y += lpNewRect->top;
}

void ScaleAndOffsetPoint(LPPOINT lpPoint, const LPDRECT lpNewRect, const LPDRECT lpOrgRect)
{
   double x = lpPoint->x - lpOrgRect->left;
   double y = lpPoint->y - lpOrgRect->top;

   double dX = lpOrgRect->right - lpOrgRect->left;
   if(dX==0)
      x = 0;
   else
      x = (long)(x * (lpNewRect->right - lpNewRect->left) / dX);

   double dY = lpOrgRect->bottom - lpOrgRect->top;
   if(dY==0)
      y = 0;
   else
      y = (long)(y * (lpNewRect->bottom - lpNewRect->top) / dY);

   lpPoint->x = (long)(x + lpNewRect->left);
   lpPoint->y = (long)(y + lpNewRect->top);
}

CDRect GetPointsBBox(LPDPOINT lpPoint, UINT uiCount)
{
   double   xmin, xmax, ymin, ymax ;
   DPOINT *ppt ;
   WORD  i ;

   xmin = ymin = INT_MAX ;
   xmax = ymax = -INT_MAX ;

   for (i=0, ppt = lpPoint ; i < uiCount ; i++, ppt++)
   {
      if (ppt->x < xmin)
         xmin = ppt->x ;
      if (ppt->x > xmax)
         xmax = ppt->x ;
      if (ppt->y < ymin)
         ymin = ppt->y ;
      if (ppt->y > ymax)
         ymax = ppt->y ;
   }
   return CDRect(xmin,ymin,xmax,ymax);
}

CDPoint GetPointsCenter(LPDPOINT lpPoint, UINT uiCount)
{
   return GetPointsBBox(lpPoint, uiCount).CenterPoint();
}

CRect GetPointsBBox(LPPOINT lpPoint, UINT uiCount)
{
   long   xmin, xmax, ymin, ymax ;
   POINT *ppt ;
   WORD  i ;

   xmin = ymin = INT_MAX ;
   xmax = ymax = -INT_MAX ;

   for (i=0, ppt = lpPoint ; i < uiCount ; i++, ppt++)
   {
      if (ppt->x < xmin)
         xmin = ppt->x ;
      if (ppt->x > xmax)
         xmax = ppt->x ;
      if (ppt->y < ymin)
         ymin = ppt->y ;
      if (ppt->y > ymax)
         ymax = ppt->y ;
   }
   return CRect(xmin,ymin,xmax,ymax);
}

CPoint GetPointsCenter(LPPOINT lpPoint, UINT uiCount)
{
   return GetPointsBBox(lpPoint, uiCount).CenterPoint();
}

void RotatePoints(LPDPOINT lpPoint, UINT uiCount, double dAng, LPDPOINT ptCenter)
{
   CDPoint center;
   if(ptCenter!=NULL)
      center = *ptCenter;
   else
      center = GetPointsCenter(lpPoint, uiCount);

   for(UINT ui = 0; ui<uiCount; ui++)
   {
      CDPoint point = lpPoint[ui];
      point-=center;


      double dYeter = pitagoran(point.x, point.y);

      double dPointAng = safeAtan2(point.y, point.x);
      
      point.y = sin(dPointAng+dAng) * dYeter;
      point.x = cos(dPointAng+dAng) * dYeter;

      point+=center;

      lpPoint[ui] = point;
   }
}

void RotatePoints(LPPOINT lpPoint, UINT uiCount, double dAng, LPPOINT ptCenter)
{
   CDPoint center;
   if(ptCenter!=NULL)
      center = *ptCenter;
   else
      center = GetPointsCenter(lpPoint, uiCount);

   for(UINT ui = 0; ui<uiCount; ui++)
   {
      CDPoint point = lpPoint[ui];
      point-=center;


      double dYeter = pitagoran(point.x, point.y);

      double dPointAng = safeAtan2(point.y, point.x);
      
      point.y = sin(dPointAng+dAng) * dYeter;
      point.x = cos(dPointAng+dAng) * dYeter;

      point+=center;

      lpPoint[ui] = point;
   }
}

CDRect GetRotatedBoundingBox(LPCDRECT lpSrcRect, double dAng, LPCDPOINT lpCenter)
{
   CDPoint lpPoints[4];
   lpPoints[0] = CDPoint(lpSrcRect->left, lpSrcRect->top);
   lpPoints[1] = CDPoint(lpSrcRect->right, lpSrcRect->top);
   lpPoints[2] = CDPoint(lpSrcRect->left, lpSrcRect->bottom);
   lpPoints[3] = CDPoint(lpSrcRect->right, lpSrcRect->bottom);

   RotatePoints(lpPoints, 4, dAng, lpCenter);
   return GetPointsBBox(lpPoints, 4);
}

CRect GetRotatedBoundingBox(LPCRECT lpSrcRect, double dAng, LPPOINT lpCenter)
{
   POINT lpPoints[4];
   lpPoints[0] = CPoint(lpSrcRect->left, lpSrcRect->top);
   lpPoints[1] = CPoint(lpSrcRect->right, lpSrcRect->top);
   lpPoints[2] = CPoint(lpSrcRect->left, lpSrcRect->bottom);
   lpPoints[3] = CPoint(lpSrcRect->right, lpSrcRect->bottom);

   RotatePoints(lpPoints, 4, dAng, lpCenter);
   return GetPointsBBox(lpPoints, 4);
}

CDRect GetFlippedBoundingBox(LPCDRECT lpSrcRect, BOOL bHorizontal, LPCDPOINT lpCenter)
{
   CDPoint lpPoints[4];
   lpPoints[0] = CDPoint(lpSrcRect->left, lpSrcRect->top);
   lpPoints[1] = CDPoint(lpSrcRect->right, lpSrcRect->top);
   lpPoints[2] = CDPoint(lpSrcRect->left, lpSrcRect->bottom);
   lpPoints[3] = CDPoint(lpSrcRect->right, lpSrcRect->bottom);

   FlipPoints(lpPoints, 4, bHorizontal, lpCenter);
   return GetPointsBBox(lpPoints, 4);
}

CRect GetFlippedBoundingBox(LPCRECT lpSrcRect, BOOL bHorizontal, LPPOINT lpCenter)
{
   POINT lpPoints[4];
   lpPoints[0] = CPoint(lpSrcRect->left, lpSrcRect->top);
   lpPoints[1] = CPoint(lpSrcRect->right, lpSrcRect->top);
   lpPoints[2] = CPoint(lpSrcRect->left, lpSrcRect->bottom);
   lpPoints[3] = CPoint(lpSrcRect->right, lpSrcRect->bottom);

   FlipPoints(lpPoints, 4, bHorizontal, lpCenter);
   return GetPointsBBox(lpPoints, 4);
}
