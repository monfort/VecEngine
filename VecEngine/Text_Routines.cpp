//////////////////////////////////////////////////////////////////////
//Oran: Microsoft you are the greatest !!!! Spank you very much


#include "stdafx.h"
#include "math.h"
#include "Text_Routines.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


/****************************************************************************
 *  FUNCTION   : FixedFromDouble
 *  RETURNS    : FIXED value representing the given double.
 ****************************************************************************/
FIXED FixedFromDouble(double d)
{
    long l;

    l = (long) (d * 65536L); 
    return *(FIXED *)&l;
}

double DoubleFromFixed(FIXED f)
{
    double d;

    long l = *(long *)&f;
    d = ((double)l / 65536L);
    return d;
}

/****************************************************************************
 *  FUNCTION   : IntFromFixed
 *  RETURNS    : int value approximating the FIXED value.
 ****************************************************************************/ 
int IntFromFixed(FIXED f)
{
    if (f.fract >= 0x8000)
    return(f.value + 1);
    else
    return(f.value);
}

/****************************************************************************
 *  FUNCTION   : fxDiv2
 *  RETURNS    : (val1 + val2)/2 for FIXED values
 ****************************************************************************/ 
FIXED fxDiv2(FIXED fxVal1, FIXED fxVal2)
{
    long l;

    l = (*((long far *)&(fxVal1)) + *((long far *)&(fxVal2)))/2;
    return(*(FIXED *)&l);
}

/****************************************************************************
 *  FUNCTION   : MakeRotationMat
 *  PURPOSE    : Fill in a rotation matrix based on the given angle.
 *  RETURNS    : none.
 ****************************************************************************/
void MakeRotationMat(LPMAT2 lpMat, double dAngle)
{
    double c = cos(dAngle);
    double s = sin(dAngle);
    lpMat->eM11 = FixedFromDouble(c);
    lpMat->eM12 = FixedFromDouble(s);
    lpMat->eM21 = FixedFromDouble(-s);
    lpMat->eM22 = FixedFromDouble(c);
}

/****************************************************************************
 *  FUNCTION   : ShearMat
 *  PURPOSE    : Fill in a 0.25 horizontal shear matrix.
 *  RETURNS    : none.
 ****************************************************************************/
void ShearMat(LPMAT2 lpMat)
{
    lpMat->eM11 = FixedFromDouble(1);
    lpMat->eM12 = FixedFromDouble(0);
    lpMat->eM21 = FixedFromDouble(0.25);
    lpMat->eM22 = FixedFromDouble(1);
}
/****************************************************************************
 *  FUNCTION   : IdentityMat
 *  PURPOSE    : Fill in matrix to be the identity matrix.
 *  RETURNS    : none.
 ****************************************************************************/
void IdentityMat(LPMAT2 lpMat)
{
    lpMat->eM11 = FixedFromDouble(1);
    lpMat->eM12 = FixedFromDouble(0);
    lpMat->eM21 = FixedFromDouble(0);
    lpMat->eM22 = FixedFromDouble(1);
}

void NotMem(void* lpMem, DWORD dwSize)
{
   __asm
   {
      mov ecx,dwSize;
      mov ebx,lpMem;
jmp_loop:
      mov al,[ebx];
      not al;
      mov [ebx],al;
      inc ebx;
      loop jmp_loop;
   }
}

/****************************************************************************
 *
 *  FUNCTION   : BitmapFromT2Bitmap
 *
 *  PURPOSE    : Create a Windows bitmap from the GGO_BITMAP format.
 *               Two methods are provided:
 *                 1) create a DWORD-aligned bitmap by setting WidthBytes.
 *                 2) WORD-align the bits and create conventional bitmap.
 *
 *  RETURNS    : Handle to a Windows bitmap object
 *
 ****************************************************************************/
HBITMAP BitmapFromT2Bitmap(void FAR *lpBits, WORD width, WORD height)
{
// Create a DWORD-aligned Windows bitmap.  By building up a BITMAP structure
// with the bmWidthBytes field corresponding to DWORD-alignment and using
// CreateBitmapIndirect, a bitmap that is DWORD-aligned can be read by 
// the device driver.
    BITMAP bm;

    bm.bmType = 0;
    bm.bmWidth = width;
    bm.bmHeight = height;
    bm.bmWidthBytes = ((width + 31) >> 5) << 2;//((width + 31) >> 5) << 2;
    bm.bmPlanes = 1;
    bm.bmBitsPixel = 1;
    bm.bmBits = lpBits;
    NotMem(lpBits, bm.bmWidthBytes*height);
    return (CreateBitmapIndirect(&bm));
}

HBITMAP BitmapFromT8Bitmap(void FAR *lpBits, WORD width, WORD height)
{
// Create a DWORD-aligned Windows bitmap.  By building up a BITMAP structure
// with the bmWidthBytes field corresponding to DWORD-alignment and using
// CreateBitmapIndirect, a bitmap that is DWORD-aligned can be read by 
// the device driver.
    BITMAP bm;

    bm.bmType = 0;
    bm.bmWidth = width;
    bm.bmHeight = height;
    bm.bmWidthBytes = (((width<<3) + 31) >> 5) << 2;//((width + 31) >> 5) << 2;
    bm.bmPlanes = 1;
    bm.bmBitsPixel = 8;
    bm.bmBits = lpBits;
    return (CreateBitmapIndirect(&bm));
}

HBITMAP CreateBitmapFromT8bits(HDC hDC,void FAR *lpBits, WORD width, WORD height)
{

   char *bibuf = new char[sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD)];
   BITMAPINFO	*pBitmapInfo = (BITMAPINFO *)bibuf;
   BITMAPINFOHEADER *bih = &pBitmapInfo->bmiHeader;

   // create an internal 16bit color DIB
   bih->biSize = sizeof(BITMAPINFOHEADER);
   bih->biWidth = width;
   bih->biHeight = -height;
   bih->biPlanes = 1;
   bih->biBitCount = 8;
   bih->biCompression = BI_RGB;
   bih->biSizeImage = 0;
   bih->biXPelsPerMeter = 0;
   bih->biYPelsPerMeter = 0;
   bih->biClrUsed = 0;
   bih->biClrImportant = 0;

   for(int i = 0; i<=64; i++)
   {
      RGBQUAD cr;
      int iTemp = i<<2;
      if(iTemp>255)
         iTemp = 255;
      cr.rgbRed = (BYTE)(255-iTemp);
      cr.rgbBlue = (BYTE)(0);
      cr.rgbGreen = (BYTE)(255-iTemp);
      pBitmapInfo->bmiColors[i] = cr;
   }
/*   RGBQUAD cr;
   cr.rgbBlue = 255;
   cr.rgbGreen = 255;
   cr.rgbRed = 255;
   pBitmapInfo->bmiColors[0] = cr;*/

   HBITMAP hbm = CreateDIBitmap(hDC, bih, CBM_INIT, lpBits, pBitmapInfo, DIB_RGB_COLORS);

   delete bibuf;
   return hbm;
}

/****************************************************************************
 *  FUNCTION   : MakeBezierFromLine
 *
 *  PURPOSE    : Converts a line define by two points to a four point Bezier
 *               spline representation of the line in pPts.
 *
 *
 *  RETURNS    : number of Bezier points placed into the pPts POINT array.
 ****************************************************************************/ 
UINT MakeBezierFromLine( POINT *pPts, POINT startpt, POINT endpt )
{
    UINT cTotal = 0;

    // starting point of Bezier
    pPts[cTotal] = startpt;
    cTotal++;

    // 1rst Control, pt == endpoint makes Bezier a line
    pPts[cTotal].x = endpt.x;
    pPts[cTotal].y = endpt.y;
    cTotal++;

    // 2nd Control, pt == startpoint makes Bezier a line
    pPts[cTotal].x = startpt.x;
    pPts[cTotal].y = startpt.y;
    cTotal++;

    // ending point of Bezier
    pPts[cTotal] = endpt;
    cTotal++;
    
    return cTotal;
}

/****************************************************************************
 *  FUNCTION   : MakeBezierFromQBSpline
 *
 *  PURPOSE    : Converts a quadratic spline in pSline to a four point Bezier
 *               spline in pPts.
 *
 *
 *  RETURNS    : number of Bezier points placed into the pPts POINT array.
 ****************************************************************************/ 
UINT MakeBezierFromQBSpline( POINT *pPts, POINTFX *pSpline )
{
    POINT   P0,         // Quadratic on curve start point
            P1,         // Quadratic control point
            P2;         // Quadratic on curve end point
    UINT    cTotal = 0;

    // Convert the Quadratic points to integer
    P0.x = IntFromFixed( pSpline[0].x );
    P0.y = IntFromFixed( pSpline[0].y );
    P1.x = IntFromFixed( pSpline[1].x );
    P1.y = IntFromFixed( pSpline[1].y );
    P2.x = IntFromFixed( pSpline[2].x );
    P2.y = IntFromFixed( pSpline[2].y );

    // conversion of a quadratic to a cubic

    // Cubic P0 is the on curve start point
    pPts[cTotal] = P0;
    cTotal++;
    
    // Cubic P1 in terms of Quadratic P0 and P1
    pPts[cTotal].x = P0.x + 2*(P1.x - P0.x)/3;
    pPts[cTotal].y = P0.y + 2*(P1.y - P0.y)/3;
    cTotal++;

    // Cubic P2 in terms of Qudartic P1 and P2
    pPts[cTotal].x = P1.x + 1*(P2.x - P1.x)/3;
    pPts[cTotal].y = P1.y + 1*(P2.y - P1.y)/3;
    cTotal++;

    // Cubic P3 is the on curve end point
    pPts[cTotal] = P2;
    cTotal++;

    return cTotal;
}


/****************************************************************************
 *  FUNCTION   : AppendPolyLineToBezier
 *
 *  PURPOSE    : Converts line segments into their Bezier point 
 *               representation and appends them to a list of Bezier points. 
 *
 *               WARNING - The array must have at least one valid
 *               start point prior to the address of the element passed.
 *
 *  RETURNS    : number of Bezier points added to the POINT array.
 ****************************************************************************/ 
UINT AppendPolyLineToBezier( LPPOINT pt, POINTFX start, LPTTPOLYCURVE lpCurve )
{
    int     i;
    UINT    cTotal = 0;
    POINT   endpt;
    POINT   startpt;
    POINT   bezier[4];

    endpt.x = IntFromFixed(start.x);
    endpt.y = IntFromFixed(start.y);

    for (i = 0; i < lpCurve->cpfx; i++)
    {
        // define the line segment
        startpt = endpt;
        endpt.x = IntFromFixed(lpCurve->apfx[i].x);
        endpt.y = IntFromFixed(lpCurve->apfx[i].y);

        // convert a line to a bezier representation
        MakeBezierFromLine( bezier, startpt, endpt );

        // append the Bezier to the existing ones
                                    // Point 0 is Point 3 of previous.
        pt[cTotal++] = bezier[1];   // Point 1
        pt[cTotal++] = bezier[2];   // Point 2
        pt[cTotal++] = bezier[3];   // Point 3

    }

    return cTotal;
}


/****************************************************************************
 *  FUNCTION   : AppendQuadBSplineToBezier
 *
 *  PURPOSE    : Converts Quadratic spline segments into their Bezier point 
 *               representation and appends them to a list of Bezier points. 
 *
 *               WARNING - The array must have at least one valid
 *               start point prior to the address of the element passed.
 *
 *  RETURNS    : number of Bezier points added to the POINT array.
 ****************************************************************************/ 
UINT AppendQuadBSplineToBezier( LPPOINT pt, POINTFX start, LPTTPOLYCURVE lpCurve )
{
    WORD                i;
    UINT                cTotal = 0;
    POINTFX             spline[3];  // a Quadratic is defined by 3 points
    POINT               bezier[4];  // a Cubic by 4

    // The initial A point is on the curve.
    spline[0] = start;

    for (i = 0; i < lpCurve->cpfx;)
    {
        // The B point.
        spline[1] = lpCurve->apfx[i++];

        // Calculate the C point.
        if (i == (lpCurve->cpfx - 1))
        {
            // The last C point is described explicitly
            // i.e. it is on the curve.
            spline[2] = lpCurve->apfx[i++];
        }     
        else
        {
            // C is midpoint between B and next B point
            // because that is the on curve point of 
            // a Quadratic B-Spline.
            spline[2].x = fxDiv2(
                lpCurve->apfx[i-1].x,
                lpCurve->apfx[i].x
                );
            spline[2].y = fxDiv2(
                lpCurve->apfx[i-1].y,
                lpCurve->apfx[i].y
                );
        }

        // convert the Q Spline to a Bezier
        MakeBezierFromQBSpline( bezier, spline );
        
        // append the Bezier to the existing ones
                                    // Point 0 is Point 3 of previous.
        pt[cTotal++] = bezier[1];   // Point 1
        pt[cTotal++] = bezier[2];   // Point 2
        pt[cTotal++] = bezier[3];   // Point 3

        // New A point for next slice of spline is the 
        // on curve C point of this B-Spline
        spline[0] = spline[2];
    }

    return cTotal;
}

/****************************************************************************
 *  FUNCTION   : CloseContour
 *
 *  PURPOSE    : Adds a bezier line to close the circuit defined in pt.
 *
 *
 *  RETURNS    : number of points aded to the pt POINT array.
 ****************************************************************************/ 
UINT CloseContour( LPPOINT pt, UINT cTotal )
{
    POINT               endpt, 
                        startpt;    // definition of a line
    POINT               bezier[4];

    // connect the first and last points by a line segment
    startpt = pt[cTotal-1];
    endpt = pt[0];

    // convert a line to a bezier representation
    MakeBezierFromLine( bezier, startpt, endpt );

    // append the Bezier to the existing ones
                                // Point 0 is Point 3 of previous.
    pt[cTotal++] = bezier[1];   // Point 1
    pt[cTotal++] = bezier[2];   // Point 2
    pt[cTotal++] = bezier[3];   // Point 3

    return 3;
}

/****************************************************************************
 *  FUNCTION   : DrawT2Outline
 *
 *  PURPOSE    : Decode the GGO_NATIVE outline, create a sequence of Beziers
 *               for each contour, draw with PolyBezier.  Color and relative 
 *               positioning provided by caller. The coordinates of hDC are
 *               assumed to have MM_TEXT orientation.
 *
 *               The outline data is not scaled. To draw a glyph unhinted
 *               the caller should create the font at its EMSquare size
 *               and retrieve the outline data. Then setup a mapping mode
 *               prior to calling this function.
 *
 *  RETURNS    : none.
 ****************************************************************************/ 
void DrawT2Outline(HDC hDC, LPTTPOLYGONHEADER lpHeader, DWORD size) 
{
    WORD                i;
    UINT                cTotal = 0; // Total points in a contour.
    LPTTPOLYGONHEADER   lpStart;    // the start of the buffer
    LPTTPOLYCURVE       lpCurve;    // the current curve of a contour
    LPPOINT             pt;         // the bezier buffer
    POINTFX             ptStart;    // The starting point of a curve
    DWORD               dwMaxPts = size/sizeof(POINTFX); // max possible pts.
    DWORD               dwBuffSize;

    dwBuffSize = dwMaxPts *     // Maximum possible # of contour points.
                 sizeof(POINT) * // sizeof buffer element
                 3;             // Worst case multiplier of one additional point
                                // of line expanding to three points of a bezier

    lpStart = lpHeader;
    pt = (LPPOINT)malloc( dwBuffSize );


    BeginPath(hDC);
   
    // Loop until we have processed the entire buffer of contours.
    // The buffer may contain one or more contours that begin with
    // a TTPOLYGONHEADER. We have them all when we the end of the buffer.
    while ((DWORD)lpHeader < (DWORD)(((LPSTR)lpStart) + size) && pt != NULL)
    {
        if (lpHeader->dwType == TT_POLYGON_TYPE)
        // Draw each coutour, currently this is the only valid
        // type of contour.
        {
            // Convert the starting point. It is an on curve point.
            // All other points are continuous from the "last" 
            // point of the contour. Thus the start point the next
            // bezier is always pt[cTotal-1] - the last point of the 
            // previous bezier. See PolyBezier.
            cTotal = 1;
            pt[0].x = IntFromFixed(lpHeader->pfxStart.x);
            pt[0].y = IntFromFixed(lpHeader->pfxStart.y);

            // Get to first curve of contour - 
            // it starts at the next byte beyond header
            lpCurve = (LPTTPOLYCURVE) (lpHeader + 1);

            // Walk this contour and process each curve( or line ) segment 
            // and add it to the Beziers
            while ((DWORD)lpCurve < (DWORD)(((LPSTR)lpHeader) + lpHeader->cb))
            {
                //**********************************************
                // Format assumption:
                //   The bytes immediately preceding a POLYCURVE
                //   structure contain a valid POINTFX.
                // 
                //   If this is first curve, this points to the 
                //      pfxStart of the POLYGONHEADER.
                //   Otherwise, this points to the last point of
                //      the previous POLYCURVE.
                // 
                //   In either case, this is representative of the
                //      previous curve's last point.
                //**********************************************

                ptStart = *(LPPOINTFX)((LPSTR)lpCurve - sizeof(POINTFX));
                if (lpCurve->wType == TT_PRIM_LINE)
                {
                    // convert the line segments to Bezier segments
                    cTotal += AppendPolyLineToBezier( &pt[cTotal], ptStart, lpCurve );
                    i = lpCurve->cpfx;
                }
                else if (lpCurve->wType == TT_PRIM_QSPLINE)
                {
                    // Decode each Quadratic B-Spline segment, convert to bezier,
                    // and append to the Bezier segments
                    cTotal += AppendQuadBSplineToBezier( &pt[cTotal], ptStart, lpCurve );
                    i = lpCurve->cpfx;
                }
                else
                {
                    // Oops! A POLYCURVE format we don't understand.
                    ; // error, error, error
                    ASSERT(FALSE);
                    break;
                }

            // Move on to next curve in the contour.
            lpCurve = (LPTTPOLYCURVE)&(lpCurve->apfx[i]);
            }

            // Add points to close the contour.
            // All contours are implied closed by TrueType definition.
            // Depending on the specific font and glyph being used, these
            // may not always be needed.
            if ( pt[cTotal-1].x != pt[0].x || pt[cTotal-1].y != pt[0].y )
            {
                cTotal += CloseContour( pt, cTotal );
            }

            // flip coordinates to get glyph right side up (Windows coordinates)
            // TT native coordiantes are zero originate at lower-left.
            // Windows MM_TEXT are zero originate at upper-left.
            for (i = 0; i < cTotal; i++)
                pt[i].y = 0 - pt[i].y;

            // Draw the contour
            PolyBezier( hDC, pt, cTotal );
        }
        else
        {
            // Bad, bail, must have a bogus buffer.
            ASSERT(FALSE);
            break; // error, error, error
        }

        // Move on to next Contour.
        // Its header starts immediate after this contour
        lpHeader = (LPTTPOLYGONHEADER)(((LPSTR)lpHeader) + lpHeader->cb);
    }

   CloseFigure(hDC);
   EndPath(hDC); 
   FillPath(hDC);

   free( pt );
} 

void BlandIt(DWORD* lpScreen, long width, long height, char* lpMask, COLORREF col)
{
   int iWidthInBytes = (((width<<3) + 31) >> 5) << 2;//DWORD alingment
   int LineStart = iWidthInBytes*height;
//   BYTE tr = GetRValue(col);
//   BYTE tg = GetGValue(col);
//   BYTE tb = GetBValue(col);
//   long lDest = 0;
///   for(long y=0; y<height; y++)
//   {
//      LineStart -= iWidthInBytes;
//      for(long x=0; x<width; x++)
//      {
//         COLORREF destCol = lpScreen[lDest];
//         BYTE opac = lpMask[LineStart+x];
//         int temp = ((opac)<<2);
//         if(temp>255)
//            temp = 255;
//         opac = (BYTE)temp;
//         BYTE opac2 = (BYTE)(255-opac);
//         if(opac>0)
//         {
//            int dr = GetRValue(destCol);
//            int dg = GetGValue(destCol);
//            int db = GetBValue(destCol);
//            dr=(dr&opac2)+(opac&tr);
//            dg=(dg&opac2)+(opac&tg);
//            db=(db&opac2)+(opac&tb);
//            lpScreen[lDest] = RGB(dr,dg,db);
//         }
//         lDest++;
//      }
//   }
   __asm
   {
      pusha;

      //make the RGB in col to be BGR
      mov eax,col;
      mov bl,al;
      shr eax,8;
      mov bh,ah;
      mov ah,bl;
      shl eax,8
      mov al,bh;
      mov col,eax;

      mov ecx,height;
      xor esi,esi;
loop_y:
         mov edx,LineStart;
         sub edx,iWidthInBytes;
         mov LineStart,edx;
         push ecx;
         xor ebx,ebx;
loop_x:
            xor edx,edx;
            mov edi,lpMask;
            add edi,LineStart;
            add edi,ebx;//(ebx = x)
            mov dl,[edi];
            shl dx,2;
            cmp dx,256;
            jnz no_dec;
            dec dx;
no_dec:
            cmp dl,0;
            jz no_pixel;

            mov dh,dl;
            shl edx,8;
            mov dl,dh;
            mov ecx,edx;
            not ecx;

            mov edi,esi;//(esi = lDest)
            push ebx;
            shl edi,2;
            add edi,lpScreen;//edi = *lpScreen[lDest]
            mov ebx,[edi];//ebx = destCol

            and ebx,ecx;
            mov ecx,col;
            and ecx,edx;
            add ebx,ecx;

            mov [edi],ebx;

            pop ebx;
no_pixel:

         inc esi;
         inc ebx;
         cmp ebx,width; 
         jnz loop_x;

      pop ecx;
      dec ecx;
      jnz loop_y;
      popa;
   }
}


/*void BlandIt(DWORD* lpScreen, long width, long height, char* lpMask, COLORREF col)
{
   int iWidthInBytes = (((width<<3) + 31) >> 5) << 2;//DWORD alingment
   int LineStart = iWidthInBytes*height;
   int tr = GetRValue(col);
   int tg = GetGValue(col);
   int tb = GetBValue(col);

//   long lDest = 0;
//   for(long y=0; y<height; y++)
//   {
//      LineStart -= iWidthInBytes;
//      for(long x=0; x<width; x++)
//      {
//         COLORREF destCol = lpScreen[lDest];
//         BYTE opac = lpMask[LineStart+x];
//         int temp = ((opac)<<2);
//         if(temp>255)
//            temp = 255;
//         opac = (BYTE)temp;
//         BYTE opac2 = (BYTE)(255-opac);
//         int dr = GetRValue(destCol);
//         int dg = GetGValue(destCol);
//         int db = GetBValue(destCol);
//         dr=(dr*opac2)>>8;
//         dg=(dg*opac2)>>8;
//         db=(db*opac2)>>8;
//         dr+=(tr*opac)>>8;
//         dg+=(tg*opac)>>8;
//         db+=(tb*opac)>>8;
//         lpScreen[lDest] = RGB(dr,dg,db);
//         lDest++;
//      }
//   }
   __asm
   {
      pusha;
      mov ecx,height;
      xor esi,esi;
loop_y:
         mov edx,LineStart;
         sub edx,iWidthInBytes;
         mov LineStart,edx;
         push ecx;
         xor ebx,ebx;
loop_x:
            xor edx,edx;
            mov edi,lpMask;
            add edi,LineStart;
            add edi,ebx;//(ebx = x)
            mov dl,[edi];
            shl dx,2;
            cmp dx,256;
            jnz no_dec;
            dec dx;//dl = opec
no_dec:
            mov dh,255;
            sub dh,dl;//dh = opac2

            mov edi,esi;//(esi = lDest)
            push ebx;
            push esi;
            shl edi,2;
            add edi,lpScreen;//edi = *lpScreen[lDest]
            mov ebx,[edi];//ebx = destCol

            xor eax,eax;
            mov al,bl;
            mul dh;
            shr eax,8;
            mov ecx,eax
            mov eax,tr;
            mul dl;
            shr eax,8;
            add ecx,eax;//cl = final dr

            xor eax,eax;//can be removed
            mov al,bh;
            mul dh;
            shr eax,8;
            mov esi,eax
            mov eax,tg;
            mul dl;
            shr eax,8;
            add esi,eax;//esi = final dg

            shr ebx,16;
            xor eax,eax;//can be removed
            mov al,bl;
            mul dh;
            shr eax,8;
            mov ebx,eax
            mov eax,tb;
            mul dl;
            shr eax,8;
            add ebx,eax;//bl = final db

            shl ebx,16;
            mov bx,si;
            shl bx,8;
            mov bl,cl;
            mov [edi],ebx;

            pop esi;
            pop ebx;

         inc esi;
         inc ebx;
         cmp ebx,width; 
         jnz loop_x;

      pop ecx;
      dec ecx;
      jnz loop_y;
      popa;
   }
}*/

/*char g_mul256[256*256];
BOOL bInit = FALSE;*/

void InitMul256()
{
/*  if(bInit)
     return;
  bInit = TRUE;

  for (int i=0;i<256;i++)
    for (int d=0;d<256;d++)
      g_mul256[(i<<8)+d]=(char)((i*d)>>8);*/
}

/*void BlandIt(DWORD* lpScreen, long width, long height, char* lpMask, COLORREF col)
{
   char* mul256 = g_mul256;
   int iWidthInBytes = (((width<<3) + 31) >> 5) << 2;//DWORD alingment
   int LineStart = iWidthInBytes*height;
   int tr = GetRValue(col);
   int tg = GetGValue(col);
   int tb = GetBValue(col);

//   long lDest = 0;
//   for(long y=0; y<height; y++)
//   {
//      LineStart -= iWidthInBytes;
//      for(long x=0; x<width; x++)
//      {
//         COLORREF destCol = lpScreen[lDest];
//         BYTE opac = lpMask[LineStart+x];
//         int temp = ((opac)<<2);
//         if(temp>255)
//            temp = 255;
//         opac = (BYTE)temp;
//         BYTE opac2 = (BYTE)(255-opac);
//         int dr = GetRValue(destCol);
//         int dg = GetGValue(destCol);
//         int db = GetBValue(destCol);
//         dr=mul256[(dr<<8)+opac2];
//         dg=mul256[(dg<<8)+opac2];
//         db=mul256[(db<<8)+opac2];
//         dr+=mul256[(tr<<8)+opac];
//         dg+=mul256[(tg<<8)+opac];
//         db+=mul256[(tb<<8)+opac];
//         lpScreen[lDest] = RGB(dr,dg,db);
//         lDest++;
//      }
//   }
   __asm
   {
      pusha;
      mov ecx,height;
      xor esi,esi;
loop_y:
         mov edx,LineStart;
         sub edx,iWidthInBytes;
         mov LineStart,edx;
         push ecx;
         xor ebx,ebx;
loop_x:
            xor edx,edx;
            mov edi,lpMask;
            add edi,LineStart;
            add edi,ebx;//(ebx = x)
            mov dl,[edi];
            shl dx,2;
            cmp dx,256;
            jnz no_dec;
            dec dx;//dl = opec
no_dec:
            mov dh,255;
            sub dh,dl;//dh = opac2

            mov edi,esi;//(esi = lDest)
            push ebx;
            push esi;
            shl edi,2;
            add edi,lpScreen;//edi = *lpScreen[lDest]
            mov eax,[edi];//ebx = destCol
            push edi;

            xor di,di;
            mov di,dx;
            and di,0xff00;
            shl dx,8;

            mov ecx,mul256;

            xor ebx,ebx;
            mov bl,al;
            add bx,di;
            add ebx,ecx;
            mov al,[ebx];
            xor ebx,ebx;
            mov bx,dx;
            add ebx,ecx;
            add ebx,tr;
            add al,[ebx];

            xor ebx,ebx;
            mov bl,ah;
            add bx,di;
            add ebx,ecx;
            mov ah,[ebx];
            xor ebx,ebx;
            mov bx,dx;
            add ebx,ecx;
            add ebx,tg;
            add ah,[ebx];

            push ax;
            shr eax,8;

            xor ebx,ebx;
            mov bl,ah;
            add bx,di;
            add ebx,ecx;
            mov ah,[ebx];
            xor ebx,ebx;
            mov bx,dx;
            add ebx,ecx;
            add ebx,tb;
            add ah,[ebx];

            shl eax,8;
            pop ax;

            pop edi;
            mov [edi],eax;

            pop esi;
            pop ebx;

         inc esi;
         inc ebx;
         cmp ebx,width; 
         jnz loop_x;

      pop ecx;
      dec ecx;
      jnz loop_y;
      popa;
   }
}*/


void BlandBlt(HDC hDC, long destX, long destY, long width, long height, char* lpBits)//must be DWORD alligned
{
   //CREATE THE BITMAP HEADER
   // fill up some DIB structures
	char *bibuf = new char[sizeof(BITMAPINFOHEADER)+12];
	BITMAPINFO	*pBitmapInfo = (BITMAPINFO *)bibuf;
	BITMAPINFOHEADER *bih = &pBitmapInfo->bmiHeader;
	DWORD *pPixelsBuffer = new DWORD[width*height];

	// create an internal 16bit color DIB
	bih->biSize = sizeof(BITMAPINFOHEADER);
 	bih->biWidth = width;
	bih->biHeight = height;
	bih->biPlanes = 1;
	bih->biBitCount = 32;
	bih->biCompression = BI_RGB;
	bih->biSizeImage = 0;
	bih->biXPelsPerMeter = 0;
	bih->biYPelsPerMeter = 0;
	bih->biClrUsed = 0;
	bih->biClrImportant = 0;

	// 32bit color
	((DWORD*)pBitmapInfo->bmiColors)[0] = 0xFF;
	((DWORD*)pBitmapInfo->bmiColors)[1] = 0x00FF;
	((DWORD*)pBitmapInfo->bmiColors)[2] = 0x0000FF;

   //get the text color
   COLORREF textCol = GetTextColor(hDC);

   //get the pixels form the DC
   HDC memDC = CreateCompatibleDC(hDC);
   HBITMAP memBmp = CreateCompatibleBitmap(hDC, width, height);
   SelectObject(memDC, memBmp);
   BitBlt(memDC, 0, 0, width, height, hDC, destX, destY, SRCCOPY);
   GetDIBits(memDC, memBmp, 0, height, pPixelsBuffer, pBitmapInfo, DIB_RGB_COLORS);
   DeleteObject(memBmp);
   DeleteDC(memDC);

   BlandIt(pPixelsBuffer, width, height, lpBits, textCol);

   //BILT THE BITMAP
	StretchDIBits(hDC, destX, destY, width, height,  
						0, 0, width, height, 
						pPixelsBuffer, pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);


   //DELETE THE MEM
	delete pPixelsBuffer;
   delete bibuf;
}

/****************************************************************************
 *  FUNCTION   : OutputGlyph
 *
 *  PURPOSE    : Output a character glyph at a given position.
 *
 *               Depending on user choices, the glyph is either retrieved
 *               as a bitmap or an outline or is output with TextOut.
 *
 *  RETURNS    : TRUE if succesful otherwize FALSE.
 ****************************************************************************/
BOOL OutputGlyph(HDC hDC, UINT letter, LONG x, LONG y, LPMAT2 lpmat, int ascent, int iDrawType)
{
    GLYPHMETRICS gm;
    DWORD size;
    HANDLE hBits;
    LPSTR lpBits;
    POINT oldOrg;

    // allocate space for the bitmap/outline
    int iflag;
    if(iDrawType == TEXT_DRAW_POLYGON) 
       iflag = GGO_NATIVE;
    else if(iDrawType == TEXT_DRAW_BITMAP) 
       iflag = GGO_BITMAP;
    else//if(iDrawType == TEXT_DRAW_ANTIALIAS) 
       iflag = GGO_GRAY8_BITMAP;

    size = GetGlyphOutline(hDC, letter, iflag, &gm, 0, NULL, lpmat);
    if(size == GDI_ERROR)
    {
       ASSERT(FALSE);//with TEXT_DRAW_PRINTER use TTF fonts only
       return FALSE;
    }

    if(size==0)
       return TRUE;

    if(iDrawType==TEXT_DRAW_POLYGON)//exit if out of screen
    {
      RECT rect = {x, y, x+gm.gmBlackBoxX, y+gm.gmBlackBoxY};   
      if(!RectVisible(hDC,&rect))
         return TRUE;
    }

    hBits = GlobalAlloc(GHND, size);
    lpBits = (char*)GlobalLock(hBits);


    if ((GetGlyphOutline(hDC, letter, iflag, &gm, size, lpBits, lpmat)) != size)
    {
   	 //MessageBox(hMyWnd, "Won't get it", "foo", MB_OK);
       ASSERT(FALSE);
  	    return FALSE;
    }

    if(iDrawType!=TEXT_DRAW_POLYGON)
    {
       x = x + (gm.gmptGlyphOrigin.x);
       y = y - (gm.gmptGlyphOrigin.y);
    }

    if(iDrawType!=TEXT_DRAW_POLYGON)//exit if out of screen
    {
      RECT rect = {x, y, x+gm.gmBlackBoxX, y+gm.gmBlackBoxY};   
      if(!RectVisible(hDC,&rect))
      {
         GlobalUnlock(hBits);
         GlobalFree(hBits);
         return TRUE;
      }
    }

    if (iDrawType == TEXT_DRAW_BITMAP)
    {
       // Get the glyph in Windows bitmap format and blt to the screen.
       // Alignment is done in the blting.
      HDC hdcMem = CreateCompatibleDC(hDC);
      ASSERT(hdcMem!=NULL);
      HBITMAP hbm = BitmapFromT2Bitmap(lpBits, (SHORT)gm.gmBlackBoxX, (SHORT)gm.gmBlackBoxY);
      ASSERT(hbm!=NULL);
      HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbm);

      COLORREF oldBKCol = SetBkColor(hDC, RGB(255, 255, 255));
      BitBlt(hDC, x, y, gm.gmBlackBoxX, gm.gmBlackBoxY, hdcMem, 0, 0, SRCINVERT);
      COLORREF oldForeCol = SetTextColor(hDC, RGB(0, 0, 0));
      BitBlt(hDC, x, y, gm.gmBlackBoxX, gm.gmBlackBoxY, hdcMem, 0, 0, SRCAND);
      SetTextColor(hDC, oldForeCol);
      BitBlt(hDC, x, y, gm.gmBlackBoxX, gm.gmBlackBoxY, hdcMem, 0, 0, SRCINVERT);
      SetBkColor(hDC, oldBKCol);

      hbm = (HBITMAP)SelectObject(hdcMem, hbmOld);
      DeleteObject(hbm);
      DeleteDC(hdcMem);
    }
    else if(iDrawType == TEXT_DRAW_ANTIALIAS)
    {
/*      HBITMAP hbm = CreateBitmapFromT8bits(hDC, lpBits, (SHORT)gm.gmBlackBoxX, (SHORT)gm.gmBlackBoxY);
      HDC hdcMem = CreateCompatibleDC(hDC);
      ASSERT(hdcMem!=NULL);
      ASSERT(hbm!=NULL);
      HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbm);
      
      BitBlt(hDC, x, y, gm.gmBlackBoxX, gm.gmBlackBoxY, hdcMem, 0, 0, SRCAND);
      BitBlt(hDC, x, y, gm.gmBlackBoxX, gm.gmBlackBoxY, hdcMem, 0, 0, PATINVERT);

      hbm = (HBITMAP)SelectObject(hdcMem, hbmOld);

      DeleteObject(hbm);*/

      if(lpBits!=NULL)
         BlandBlt(hDC, x, y, gm.gmBlackBoxX, gm.gmBlackBoxY, lpBits);
    }
    else 
    {
       // Glyph is in outline format.  Set up viewport origin to align the
       // glyph and draw it.  Actual decoding and drawing is done in 
       // DrawT2Outline.
	   GetViewportOrgEx(hDC, &oldOrg);

	   // Align based on cell origin.
	   OffsetViewportOrgEx(hDC, x , y , NULL);
	   DrawT2Outline(hDC, (LPTTPOLYGONHEADER)lpBits, size);

	   SetViewportOrgEx(hDC, oldOrg.x, oldOrg.y, NULL);
    }

   GlobalUnlock(hBits);
   GlobalFree(hBits);
   return TRUE;
}


void DrawRotatedText(HDC hdc, LPCSTR lpszText, int iNumOfChars, LPPOINT pPoint, double dAngle, int iDrawType)
{
   GLYPHMETRICS gm;
   TEXTMETRIC tm;
   MAT2 rotmat;
   MAT2 identmat;
   IdentityMat(&identmat);

   double dXslop = cos(dAngle);
   double dYslop = -sin(dAngle);

   // make the 2x2 matrix
   if (dAngle == 0)
     IdentityMat(&rotmat);
   else 
     MakeRotationMat(&rotmat, dAngle);

   GetTextMetrics(hdc, &tm);

   long lWidth = 0;
   long xOrg = pPoint->x;
   long yOrg = pPoint->y;
   xOrg += (long)(sin(dAngle)*tm.tmAscent);
   yOrg += (long)(cos(dAngle)*tm.tmAscent);
   long x = xOrg;
   long y = yOrg;

   for (int i = 0; i < iNumOfChars; i++)
   {
      if(!OutputGlyph(hdc, (UINT)lpszText[i], x, y, &rotmat, tm.tmAscent, iDrawType))
         return;
      GetGlyphOutline(hdc, (UINT)lpszText[i], GGO_METRICS, &gm, 0, NULL, &identmat);
      lWidth += gm.gmCellIncX;
      x = (long)(xOrg + lWidth*dXslop);
      y = (long)(yOrg + lWidth*dYslop);
   }
}

int NumOfCharsInWidth(HDC hDC, LPCSTR lpszText, int iNumOfChars, long lWidthToMatch, long &lRealWidth)
{
   MAT2 mat;
   GLYPHMETRICS gm;
   memset(&gm, 0, sizeof(GLYPHMETRICS));
   IdentityMat(&mat);
   lRealWidth = 0;
   int i;
   for (i = 0; i < iNumOfChars && lRealWidth<=lWidthToMatch; i++)
   {
      GetGlyphOutline(hDC, (UINT)lpszText[i], GGO_METRICS, &gm, 0, NULL, &mat);
      lRealWidth+=gm.gmCellIncX;
   }
   if(lRealWidth>lWidthToMatch)
   {
      lRealWidth-=gm.gmCellIncX;
      i--;
   }
   return i;
}

long GetLineWidth(HDC hDC, LPCSTR lpszText, int iNumOfChars)
{
   MAT2 mat;
   GLYPHMETRICS gm;
   IdentityMat(&mat);
   long lWidth = 0;
   for (int i = 0; i < iNumOfChars; i++)
   {
      GetGlyphOutline(hDC, (UINT)lpszText[i], GGO_METRICS, &gm, 0, NULL, &mat);
      lWidth+=gm.gmCellIncX;
   }
   return lWidth;
}