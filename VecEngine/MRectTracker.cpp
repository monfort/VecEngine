/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// MRectTracker.cpp: implementation of the CMRectTracker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MRectTracker.h"
#include "Snapper.h"
#include "G_Routines.h"
#include "resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define CX_BORDER 1
#define CY_BORDER 1

// various GDI objects we need to draw
#define NUM_OF_CURSORS 10
static HCURSOR aRectTrackerCursors[NUM_OF_CURSORS] = { 0, };
static HBRUSH _afxHatchBrush = 0;
static HPEN _afxBlackDottedPen = 0;
static int _afxHandleSize = 0;

#define NUM_OF_HUNDLES ((m_nStyle & cornersOnly)!=0 ? 4 : 8)

/*void AFX_CDECL AfxTrackerTerm()
{
	AfxDeleteObject((HGDIOBJ*)&_afxHatchBrush);
	AfxDeleteObject((HGDIOBJ*)&_afxBlackDottedPen);
}
char _afxTrackerTerm = (char)atexit(&AfxTrackerTerm);*/

// the struct below is used to determine the qualities of a particular handle
struct AFX_HANDLEINFO
{
	size_t nOffsetX;    // offset within RECT for X coordinate
	size_t nOffsetY;    // offset within RECT for Y coordinate
	int nCenterX;       // adjust X by Width()/2 * this number
	int nCenterY;       // adjust Y by Height()/2 * this number
	int nHandleX;       // adjust X by handle size * this number
	int nHandleY;       // adjust Y by handle size * this number
	int nInvertX;       // handle converts to this when X inverted
	int nInvertY;       // handle converts to this when Y inverted
};




// this array describes all 8 handles (clock-wise)
AFX_STATIC_DATA const AFX_HANDLEINFO aRectTrackerPoints[] =
{
	// corner handles (top-left, top-right, bottom-right, bottom-left
	{ offsetof(DRECT, left), offsetof(DRECT, top),        0, 0,  0,  0, 1, 3 },
	{ offsetof(DRECT, right), offsetof(DRECT, top),       0, 0, -1,  0, 0, 2 },
	{ offsetof(DRECT, right), offsetof(DRECT, bottom),    0, 0, -1, -1, 3, 1 },
	{ offsetof(DRECT, left), offsetof(DRECT, bottom),     0, 0,  0, -1, 2, 0 },

	// side handles (top, right, bottom, left)
	{ offsetof(DRECT, left), offsetof(DRECT, top),        1, 0,  0,  0, 4, 6 },
	{ offsetof(DRECT, right), offsetof(DRECT, top),       0, 1, -1,  0, 7, 5 },
	{ offsetof(DRECT, left), offsetof(DRECT, bottom),     1, 0,  0, -1, 6, 4 },
	{ offsetof(DRECT, left), offsetof(DRECT, top),        0, 1,  0,  0, 5, 7 }
};

// the struct below gives us information on the layout of a RECT struct and
//  the relationship between its members
struct AFX_RECTINFO
{
	size_t nOffsetAcross;   // offset of opposite point (ie. left->right)
	int nSignAcross;        // sign relative to that point (ie. add/subtract)
};

// this array is indexed by the offset of the RECT member / sizeof(int)
#define NUM_OF_EDGES 4
static const AFX_RECTINFO aRectTrackerEdges[NUM_OF_EDGES] =
{
	{ offsetof(DRECT, right), +1 },
	{ offsetof(DRECT, bottom), +1 },
	{ offsetof(DRECT, left), -1 },
	{ offsetof(DRECT, top), -1 },
};

/////////////////////////////////////////////////////////////////////////////
// CMRectTracker intitialization

CMRectTracker::CMRectTracker(LPCDRECT lpSrcRect, UINT nStyle)
{
	ASSERT(AfxIsValidAddress(lpSrcRect, sizeof(RECT), FALSE));

	Construct();
	m_rect.CopyRect(lpSrcRect);
	m_nStyle = nStyle;
   m_pSnapper = NULL;
}

void CMRectTracker::Construct()
{
	// do one-time initialization if necessary
	//AfxLockGlobals(CRIT_RECTTRACKER);
	static BOOL bInitialized = FALSE;
	if (!bInitialized)
	{
		// sanity checks for assumptions we make in the code
		ASSERT(sizeof(((DRECT*)NULL)->left) == sizeof(double));
		ASSERT(offsetof(DRECT, top) > offsetof(DRECT, left));
		ASSERT(offsetof(DRECT, right) > offsetof(DRECT, top));
		ASSERT(offsetof(DRECT, bottom) > offsetof(DRECT, right));

		if (_afxHatchBrush == NULL)
		{
			// create the hatch pattern + bitmap
			WORD hatchPattern[8];
			WORD wPattern = 0x1111;
			for (int i = 0; i < 4; i++)
			{
				hatchPattern[i] = wPattern;
				hatchPattern[i+4] = wPattern;
				wPattern <<= 1;
			}
			HBITMAP hatchBitmap = CreateBitmap(8, 8, 1, 1, &hatchPattern);
			if (hatchBitmap == NULL)
			{
//				AfxUnlockGlobals(CRIT_RECTTRACKER);
				AfxThrowResourceException();
			}

			// create black hatched brush
			_afxHatchBrush = CreatePatternBrush(hatchBitmap);
			DeleteObject(hatchBitmap);
			if (_afxHatchBrush == NULL)
			{
//				AfxUnlockGlobals(CRIT_RECTTRACKER);
				AfxThrowResourceException();
			}
		}

		if (_afxBlackDottedPen == NULL)
		{
			// create black dotted pen
			_afxBlackDottedPen = CreatePen(PS_DOT, 0, RGB(0, 0, 0));
			if (_afxBlackDottedPen == NULL)
			{
				//AfxUnlockGlobals(CRIT_RECTTRACKER);
				AfxThrowResourceException();
			}
		}


		// initialize the cursor array
		aRectTrackerCursors[0] = AfxGetApp()->LoadCursor(IDC_TRACKNWSE);
		aRectTrackerCursors[1] = AfxGetApp()->LoadCursor(IDC_TRACKNESW);
		aRectTrackerCursors[2] = aRectTrackerCursors[0];
		aRectTrackerCursors[3] = aRectTrackerCursors[1];
		aRectTrackerCursors[4] = AfxGetApp()->LoadCursor(IDC_TRACKNS);
		aRectTrackerCursors[5] = AfxGetApp()->LoadCursor(IDC_TRACKWE);
		aRectTrackerCursors[6] = aRectTrackerCursors[4];
		aRectTrackerCursors[7] = aRectTrackerCursors[5];
		aRectTrackerCursors[8] = AfxGetApp()->LoadCursor(IDC_TRACK4WAY);
		aRectTrackerCursors[9] = AfxGetApp()->LoadCursor(IDC_MOVE4WAY);

      _afxHandleSize = 4;
		bInitialized = TRUE;
	}
	//AfxUnlockGlobals(CRIT_RECTTRACKER);

	m_nStyle = 0;
	m_nHandleSize = _afxHandleSize;
	m_sizeMin.cy = m_sizeMin.cx = 0;//m_nHandleSize*2;

	m_rectLast.SetRectEmpty();
	m_sizeLast.cx = m_sizeLast.cy = 0;
	m_bErase = FALSE;
	m_bFinalErase =  FALSE;
}

CMRectTracker::~CMRectTracker()
{
}

/////////////////////////////////////////////////////////////////////////////
// CMRectTracker operations

void CMRectTracker::Draw(CDC* pDC) const
{
	// set initial DC state
	VERIFY(pDC->SaveDC() != 0);
	pDC->SetMapMode(MM_TEXT);
	pDC->SetViewportOrg(0, 0);
	pDC->SetWindowOrg(0, 0);

	// get normalized rectangle
	CRect rect = m_rect;
	rect.NormalizeRect();

	CPen* pOldPen = NULL;
	CBrush* pOldBrush = NULL;
	CGdiObject* pTemp;
	int nOldROP;

	// draw lines
	if ((m_nStyle & (dottedLine|solidLine)) != 0)
	{
		if (m_nStyle & dottedLine)
			pOldPen = pDC->SelectObject(CPen::FromHandle(_afxBlackDottedPen));
		else
			pOldPen = (CPen*)pDC->SelectStockObject(BLACK_PEN);
		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		nOldROP = pDC->SetROP2(R2_COPYPEN);
		rect.InflateRect(+1, +1);   // borders are one pixel outside
		pDC->Rectangle(rect.left, rect.top, rect.right, rect.bottom);
		pDC->SetROP2(nOldROP);
	}

	// if hatchBrush is going to be used, need to unrealize it
	if ((m_nStyle & (hatchInside|hatchedBorder)) != 0)
		UnrealizeObject(_afxHatchBrush);

	// hatch inside
	if ((m_nStyle & hatchInside) != 0)
	{
		pTemp = pDC->SelectStockObject(NULL_PEN);
		if (pOldPen == NULL)
			pOldPen = (CPen*)pTemp;
		pTemp = pDC->SelectObject(CBrush::FromHandle(_afxHatchBrush));
		if (pOldBrush == NULL)
			pOldBrush = (CBrush*)pTemp;
		pDC->SetBkMode(TRANSPARENT);
		nOldROP = pDC->SetROP2(R2_MASKNOTPEN);
		pDC->Rectangle(rect.left+1, rect.top+1, rect.right, rect.bottom);
		pDC->SetROP2(nOldROP);
	}

	// draw hatched border
	if ((m_nStyle & hatchedBorder) != 0)
	{
		pTemp = pDC->SelectObject(CBrush::FromHandle(_afxHatchBrush));
		if (pOldBrush == NULL)
			pOldBrush = (CBrush*)pTemp;
		pDC->SetBkMode(OPAQUE);
		CRect rectTrue;
		GetTrueRect(&rectTrue);
		pDC->PatBlt(rectTrue.left, rectTrue.top, rectTrue.Width(),
			rect.top-rectTrue.top, 0x000F0001 /* Pn */);
		pDC->PatBlt(rectTrue.left, rect.bottom,
			rectTrue.Width(), rectTrue.bottom-rect.bottom, 0x000F0001 /* Pn */);
		pDC->PatBlt(rectTrue.left, rect.top, rect.left-rectTrue.left,
			rect.Height(), 0x000F0001 /* Pn */);
		pDC->PatBlt(rect.right, rect.top, rectTrue.right-rect.right,
			rect.Height(), 0x000F0001 /* Pn */);
	}

	// draw resize handles
	if ((m_nStyle & (resizeInside|resizeOutside)) != 0)
   {
	   CPen* pOldPen = (CPen*)pDC->SelectStockObject(BLACK_PEN);
	   CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(WHITE_BRUSH);

		UINT mask = GetHandleMask();
		for (int i = 0; i < NUM_OF_HUNDLES; ++i)
		{
			if (mask & (1<<i))
			{
				GetHandleRect((TrackerHit)i, &rect);
				//pDC->FillSolidRect(rect, RGB(0, 0, 0));
            pDC->Rectangle(rect);
			}
		}
	   if (pOldPen != NULL)
		   pDC->SelectObject(pOldPen);
	   if (pOldBrush != NULL)
		   pDC->SelectObject(pOldBrush);
   }

	// cleanup pDC state
	if (pOldPen != NULL)
		pDC->SelectObject(pOldPen);
	if (pOldBrush != NULL)
		pDC->SelectObject(pOldBrush);
	VERIFY(pDC->RestoreDC(-1));
}

BOOL CMRectTracker::SetCursor(CWnd* pWnd, UINT nHitTest) const
{
	// trackers should only be in client area
	if (nHitTest != HTCLIENT)
		return FALSE;

	// convert cursor position to client co-ordinates
	CPoint point;
	GetCursorPos(&point);
	pWnd->ScreenToClient(&point);

	// do hittest and normalize hit
	int nHandle = HitTestHandles(point);
	if (nHandle < 0)
		return FALSE;

	// need to normalize the hittest such that we get proper cursors
	nHandle = NormalizeHit(nHandle);

	// handle special case of hitting area between handles
	//  (logically the same -- handled as a move -- but different cursor)
	if (nHandle == hitMiddle && !m_rect.PtInRect(CDPoint(point)))
	{
		// only for trackers with hatchedBorder (ie. in-place resizing)
		if (m_nStyle & hatchedBorder)
			nHandle = (TrackerHit)9;
	}

	ASSERT(nHandle < NUM_OF_CURSORS);
	::SetCursor(aRectTrackerCursors[nHandle]);
	return TRUE;
}

int CMRectTracker::HitTest(CPoint point) const
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

int CMRectTracker::NormalizeHit(int nHandle) const
{
	ASSERT((nHandle < NUM_OF_HUNDLES || nHandle==8) && nHandle >= -1);
	if (nHandle == hitMiddle || nHandle == hitNothing)
		return nHandle;
	const AFX_HANDLEINFO* pHandleInfo = &aRectTrackerPoints[nHandle];
	if (m_rect.Width() < 0)
	{
		nHandle = (TrackerHit)pHandleInfo->nInvertX;
		pHandleInfo = &aRectTrackerPoints[nHandle];
	}
	if (m_rect.Height() < 0)
		nHandle = (TrackerHit)pHandleInfo->nInvertY;
	return nHandle;
}

BOOL CMRectTracker::Track(CWnd* pWnd, CPoint point, BOOL bAllowInvert, CSnapper* pSnapper, CWnd* pWndClipTo)
{
   m_pSnapper = pSnapper;
	// perform hit testing on the handles
	int nHandle = HitTestHandles(point);
	if (nHandle < 0)
	{
		// didn't hit a handle, so just return FALSE
		return FALSE;
	}

   if(m_rect.Height()!=0)
      m_dWidthDivHeight = m_rect.Width() / ((double)m_rect.Height());
   else
      m_dWidthDivHeight = 1;

   // otherwise, call helper function to do the tracking
	m_bAllowInvert = bAllowInvert;
	return TrackHandle(nHandle, pWnd, point, pWndClipTo);
}

BOOL CMRectTracker::TrackRubberBand(CWnd* pWnd, CPoint point, BOOL bAllowInvert, CSnapper* pSnapper)
{
	// simply call helper function to track from bottom right handle
   m_pSnapper = pSnapper;
	m_bAllowInvert = bAllowInvert;
	m_rect.SetRect(point.x, point.y, point.x, point.y);
   m_dWidthDivHeight = 1;
	return TrackHandle(hitBottomRight, pWnd, point, NULL);
}


void CMRectTracker::DoDrawTracker(CDC* pDC, LPCRECT lpRect, SIZE size,	LPCRECT lpRectLast, SIZE sizeLast)
{
   DrawDragRect(pDC, lpRect, size, lpRectLast, sizeLast);
}

void CMRectTracker::DrawTracker(
	LPCRECT lpRect, CWnd* pWndClipTo, CDC* pDC, CWnd* pWnd)
{
	// first, normalize the rectangle for drawing
	CRect rect = *lpRect;
	rect.NormalizeRect();

	// convert to client coordinates
	if (pWndClipTo != NULL)
	{
		pWnd->ClientToScreen(&rect);
		pWndClipTo->ScreenToClient(&rect);
	}

	CSize size(0, 0);
	if (!m_bFinalErase)
	{
		// otherwise, size depends on the style
		if (m_nStyle & hatchedBorder)
		{
			size.cx = size.cy = max(1, GetHandleSize(rect)-1);
			rect.InflateRect(size);
		}
		else
		{
			size.cx = CX_BORDER;
			size.cy = CY_BORDER;
		}
	}

	// and draw it
	if (m_bFinalErase || !m_bErase)
		DoDrawTracker(pDC, rect, size, m_rectLast, m_sizeLast);
		//pDC->DrawDragRect(rect, size, m_rectLast, m_sizeLast);

	// remember last rectangles
	m_rectLast = rect;
	m_sizeLast = size;
}

void CMRectTracker::AdjustRect(int nHandle, LPCDRECT)
{
	if (nHandle == hitMiddle)
		return;

	// convert the handle into locations within m_rect
	double *px, *py;
	GetModifyPointers(nHandle, &px, &py, NULL, NULL);

	// enforce minimum width
	double nNewWidth = m_rect.Width();
	double nAbsWidth = m_bAllowInvert ? ABS(nNewWidth) : nNewWidth;
	if (px != NULL && nAbsWidth < m_sizeMin.cx)
	{
		nNewWidth = nAbsWidth != 0 ? nNewWidth / nAbsWidth : 1;
		ASSERT((double*)px - (double*)&m_rect < NUM_OF_EDGES);
		const AFX_RECTINFO* pRectInfo = &aRectTrackerEdges[(double*)px - (double*)&m_rect];
		*px = *(double*)((BYTE*)&m_rect + pRectInfo->nOffsetAcross) +
			nNewWidth * m_sizeMin.cx * -pRectInfo->nSignAcross;
	}

	// enforce minimum height
	double nNewHeight = m_rect.Height();
	double nAbsHeight = m_bAllowInvert ? ABS(nNewHeight) : nNewHeight;
	if (py != NULL && nAbsHeight < m_sizeMin.cy)
	{
		nNewHeight = nAbsHeight != 0 ? nNewHeight / nAbsHeight : 1;
		ASSERT((double*)py - (double*)&m_rect < NUM_OF_EDGES);
		const AFX_RECTINFO* pRectInfo = &aRectTrackerEdges[(double*)py - (double*)&m_rect];
		*py = *(double*)((BYTE*)&m_rect + pRectInfo->nOffsetAcross) +
			nNewHeight * m_sizeMin.cy * -pRectInfo->nSignAcross;
	}

   if(CheckRetainProportions() && px!=NULL && py!=NULL && m_dWidthDivHeight!=0) //if the shift is pressed then remain proporions
   {
	   double nNewWidth = m_rect.Width();
	   double nAbsWidth = m_bAllowInvert ? ABS(nNewWidth) : nNewWidth;
	   double nNewHeight = m_rect.Height();
	   double nAbsHeight = m_bAllowInvert ? ABS(nNewHeight) : nNewHeight;

      nNewHeight = nAbsHeight != 0 ? nNewHeight / nAbsHeight : 1;
		ASSERT((double*)py - (double*)&m_rect < NUM_OF_EDGES);
		const AFX_RECTINFO* pRectInfo = &aRectTrackerEdges[(double*)py - (double*)&m_rect];
		*py = (double)( *(double*)((BYTE*)&m_rect + pRectInfo->nOffsetAcross) +
			nNewHeight * (nAbsWidth/m_dWidthDivHeight) * -pRectInfo->nSignAcross);
   }
}

void CMRectTracker::GetTrueRect(LPRECT lpTrueRect) const
{
	ASSERT(AfxIsValidAddress(lpTrueRect, sizeof(RECT)));

	CRect rect = m_rect;
	rect.NormalizeRect();
	int nInflateBy = 0;
	if ((m_nStyle & (resizeOutside|hatchedBorder)) != 0)
		nInflateBy += GetHandleSize() - 1;
	if ((m_nStyle & (solidLine|dottedLine)) != 0)
		++nInflateBy;
	rect.InflateRect(nInflateBy, nInflateBy);
	*lpTrueRect = rect;
}

void CMRectTracker::OnChangedRect(const CRect& /*rectOld*/)
{
	// no default implementation, useful for derived classes
}

/////////////////////////////////////////////////////////////////////////////
// CMRectTracker implementation helpers

void CMRectTracker::GetHandleRect(int nHandle, CRect* pHandleRect) const
{
	ASSERT(nHandle < NUM_OF_HUNDLES);

	// get normalized rectangle of the tracker
	CDRect rectT = m_rect;
	rectT.NormalizeRect();
	if ((m_nStyle & (solidLine|dottedLine)) != 0)
		rectT.InflateRect(+1, +1);

	// since the rectangle itself was normalized, we also have to invert the
	//  resize handles.
	nHandle = NormalizeHit(nHandle);

	// handle case of resize handles outside the tracker
	int size = GetHandleSize();
	if (m_nStyle & resizeOutside)
		rectT.InflateRect(size-1, size-1);

	// calculate position of the resize handle
	double nWidth = rectT.Width();
	double nHeight = rectT.Height();
	CDRect rect;
	const AFX_HANDLEINFO* pHandleInfo = &aRectTrackerPoints[nHandle];
	rect.left = (long)*(double*)((BYTE*)&rectT + pHandleInfo->nOffsetX);
	rect.top = (long)*(double*)((BYTE*)&rectT + pHandleInfo->nOffsetY);
	rect.left += size * pHandleInfo->nHandleX;
	rect.top += size * pHandleInfo->nHandleY;
	rect.left += (long)(pHandleInfo->nCenterX * (nWidth - size) / 2);
	rect.top += (long)(pHandleInfo->nCenterY * (nHeight - size) / 2);
	rect.right = rect.left + size;
	rect.bottom = rect.top + size;

	*pHandleRect = rect;
}

int CMRectTracker::GetHandleSize(LPCRECT lpRect) const
{
   CRect rect = m_rect;
	if (lpRect == NULL)
		lpRect = &rect;

	int size = m_nHandleSize;
	if (!(m_nStyle & resizeOutside))
	{
		// make sure size is small enough for the size of the rect
		int sizeMax = min(abs(lpRect->right - lpRect->left),
			abs(lpRect->bottom - lpRect->top));
		if (size * 2 > sizeMax)
			size = sizeMax / 2;
	}
	return size;
}

int CMRectTracker::HitTestHandles(CPoint point) const
{
	CRect rect;
	UINT mask = GetHandleMask();

	// see if hit anywhere inside the tracker
	GetTrueRect(&rect);
	if (!rect.PtInRect(point))
		return hitNothing;  // totally missed

	// see if we hit a handle
	for (int i = 0; i < NUM_OF_HUNDLES; ++i)
	{
		if (mask & (1<<i))
		{
			GetHandleRect((TrackerHit)i, &rect);
			if (rect.PtInRect(point))
				return (TrackerHit)i;
		}
	}

	// last of all, check for non-hit outside of object, between resize handles
	if ((m_nStyle & hatchedBorder) == 0)
	{
		CRect rect = m_rect;
		rect.NormalizeRect();
		if ((m_nStyle & dottedLine|solidLine) != 0)
			rect.InflateRect(+1, +1);
		if (!rect.PtInRect(point))
			return hitNothing;  // must have been between resize handles
	}
	return hitMiddle;   // no handle hit, but hit object (or object border)
}

BOOL CMRectTracker::TrackHandle(int nHandle, CWnd* pWnd, CPoint point, 
	CWnd* pWndClipTo)
{
   ASSERT(nHandle >= 0);
	ASSERT(nHandle <= NUM_OF_HUNDLES || nHandle==8);   // handle 8 is inside the rect

	// don't handle if capture already set
	if (::GetCapture() != NULL)
		return FALSE;

	AfxLockTempMaps();  // protect maps while looping

	ASSERT(!m_bFinalErase);

	// save original width & height in pixels
	double nWidth = m_rect.Width();
	double nHeight = m_rect.Height();

	// set capture to the window which received this message
	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());
	pWnd->UpdateWindow();
	if (pWndClipTo != NULL)
		pWndClipTo->UpdateWindow();
	CDRect rectSave = m_rect;

	// find out what x/y coords we are supposed to modify
	double *px, *py;
	double xDiff, yDiff;
	GetModifyPointers(nHandle, &px, &py, &xDiff, &yDiff);
	xDiff = point.x - xDiff;
	yDiff = point.y - yDiff;

	// get DC for drawing
	CDC* pDrawDC;
	if (pWndClipTo != NULL)
	{
		// clip to arbitrary window by using adjusted Window DC
		pDrawDC = pWndClipTo->GetDCEx(NULL, DCX_CACHE);
	}
	else
	{
		// otherwise, just use normal DC
		pDrawDC = pWnd->GetDC();
	}
	ASSERT_VALID(pDrawDC);

	CDRect rectOld;
	BOOL bMoved = FALSE;

	// get messages until capture lost or cancelled/accepted
	for (;;)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (CWnd::GetCapture() != pWnd)
			break;

      if(msg.message == WM_MOUSEMOVE)
			DispatchMessage(&msg);

		switch (msg.message)
		{
		// handle movement/accept messages
		case WM_LBUTTONUP:
		case WM_MOUSEMOVE:
OnCheange:
			rectOld = m_rect;
			// handle resize cases (and part of move)
         POINT p;
         ::GetCursorPos(&p);
         pWnd->ScreenToClient(&p);


			if (px != NULL)
				*px = (int)(short)p.x - xDiff;
			if (py != NULL)
				*py = (int)(short)p.y - yDiff;

			// handle move case
			if (nHandle == hitMiddle)
			{
            if((::GetKeyState(VK_SHIFT) & 0x8000)!=0)
            {
               CDPoint ppp = m_rect.TopLeft() - rectSave.TopLeft();
               if(ABS(ppp.x)>ABS(ppp.y))
                  ppp.y=0;
               else
                  ppp.x=0;
               m_rect.left = rectSave.left + ppp.x;
               m_rect.top = rectSave.top + ppp.y;
            }
				m_rect.right = m_rect.left + nWidth;
				m_rect.bottom = m_rect.top + nHeight;
            if(m_pSnapper!=NULL)
               m_pSnapper->FixSnapTo(&m_rect);
			}
         else
         {
            if(m_pSnapper!=NULL)
            {
               CDPoint ppp(0,0);
			      if (px != NULL)
                  ppp.x = *px;
			      if (py != NULL)
                  ppp.y = *py;
               m_pSnapper->FixSnapTo(&ppp);
			      if (px != NULL)
                  *px = ppp.x;
      			if (py != NULL)
                  *py = ppp.y;
            }
         }

			// allow caller to adjust the rectangle if necessary
			AdjustRect(nHandle, &m_rect);

			// only redraw and callback if the rect actually changed!
			m_bFinalErase = (msg.message == WM_LBUTTONUP);
			if (!rectOld.EqualRect(&m_rect) || m_bFinalErase)
			{
				if (bMoved)
				{
					m_bErase = TRUE;
               CRect rect(rectOld);
					DrawTracker(rect, pWndClipTo, pDrawDC, pWnd);
				}
				OnChangedRect(rectOld);
				if (msg.message != WM_LBUTTONUP)
					bMoved = TRUE;
			}
			if (m_bFinalErase)
				goto ExitLoop;

			if (!rectOld.EqualRect(&m_rect))
			{
				m_bErase = FALSE;
            CRect rect(m_rect);
				DrawTracker(&rect, pWndClipTo, pDrawDC, pWnd);
			}
			break;

		case WM_KEYUP:
			if (msg.wParam == VK_SHIFT)
            goto OnCheange;
			break;

		// handle cancel messages
		case WM_KEYDOWN:
			if (msg.wParam == VK_SHIFT)
            goto OnCheange;
			if (msg.wParam != VK_ESCAPE)
				break;
		case WM_RBUTTONDOWN:
			if (bMoved)
			{
				m_bErase = m_bFinalErase = TRUE;
            CRect rect(m_rect);
				DrawTracker(&rect, pWndClipTo, pDrawDC, pWnd);
			}
			m_rect = rectSave;
			goto ExitLoop;

		// just dispatch rest of the messages
		default:
			DispatchMessage(&msg);
			break;
		}
	}

ExitLoop:
	if (pWndClipTo != NULL)
		pWndClipTo->ReleaseDC(pDrawDC);
	else
		pWnd->ReleaseDC(pDrawDC);
	ReleaseCapture();

	AfxUnlockTempMaps(FALSE);

	// restore rect in case bMoved is still FALSE
	if (!bMoved)
		m_rect = rectSave;
	m_bFinalErase = FALSE;
	m_bErase = FALSE;

	// return TRUE only if rect has changed
	return !rectSave.EqualRect(&m_rect);
}

void CMRectTracker::GetModifyPointers(
	int nHandle, double** ppx, double** ppy, double* px, double* py)
{
	ASSERT(nHandle >= 0);
	ASSERT(nHandle <= NUM_OF_HUNDLES || nHandle==8);

	if (nHandle == hitMiddle)
		nHandle = hitTopLeft;   // same as hitting top-left

	*ppx = NULL;
	*ppy = NULL;

	// fill in the part of the rect that this handle modifies
	//  (Note: handles that map to themselves along a given axis when that
	//   axis is inverted don't modify the value on that axis)

	const AFX_HANDLEINFO* pHandleInfo = &aRectTrackerPoints[nHandle];
	if (pHandleInfo->nInvertX != nHandle)
	{
		*ppx = (double*)((BYTE*)&m_rect + pHandleInfo->nOffsetX);
		if (px != NULL)
			*px = **ppx;
	}
	else
	{
		// middle handle on X axis
		if (px != NULL)
			*px = m_rect.left + ABS(m_rect.Width()) / 2;
	}
	if (pHandleInfo->nInvertY != nHandle)
	{
		*ppy = (double*)((BYTE*)&m_rect + pHandleInfo->nOffsetY);
		if (py != NULL)
			*py = **ppy;
	}
	else
	{
		// middle handle on Y axis
		if (py != NULL)
			*py = m_rect.top + ABS(m_rect.Height()) / 2;
	}
}

UINT CMRectTracker::GetHandleMask() const
{
	UINT mask = 0x0F;   // always have 4 corner handles
	int size = m_nHandleSize*3;
	if (ABS(m_rect.Width()) - size > 4)
		mask |= 0x50;
	if (ABS(m_rect.Height()) - size > 4)
		mask |= 0xA0;
	return mask;
}


void CMRectTracker::DrawDragRect(CDC* pDC, LPCRECT lpRect, SIZE size,
	LPCRECT lpRectLast, SIZE sizeLast, CBrush* pBrush, CBrush* pBrushLast)
{
	ASSERT(AfxIsValidAddress(lpRect, sizeof(RECT), FALSE));
	ASSERT(lpRectLast == NULL ||
		AfxIsValidAddress(lpRectLast, sizeof(RECT), FALSE));

	// first, determine the update region and select it
	CRgn rgnNew;
	CRgn rgnOutside, rgnInside;
	rgnOutside.CreateRectRgnIndirect(lpRect);
	CRect rect = *lpRect;
	rect.InflateRect(-size.cx, -size.cy);
	rect.IntersectRect(rect, lpRect);
	rgnInside.CreateRectRgnIndirect(rect);
	rgnNew.CreateRectRgn(0, 0, 0, 0);
	rgnNew.CombineRgn(&rgnOutside, &rgnInside, RGN_XOR);

	CBrush* pBrushOld = NULL;
   BOOL bBrushCreated = FALSE;
	if (pBrush == NULL)
   {
      //pBrush = CDC::GetHalftoneBrush();
      bBrushCreated = TRUE;
      pBrush = new CBrush();
		pBrush->CreateSolidBrush(RGB(255,255,255));
   }
	if (pBrushLast == NULL)
		pBrushLast = pBrush;

	CRgn rgnLast, rgnUpdate;
	if (lpRectLast != NULL)
	{
		// find difference between new region and old region
		rgnLast.CreateRectRgn(0, 0, 0, 0);
		rgnOutside.SetRectRgn(lpRectLast);
		rect = *lpRectLast;
		rect.InflateRect(-sizeLast.cx, -sizeLast.cy);
		rect.IntersectRect(rect, lpRectLast);
		rgnInside.SetRectRgn(rect);
		rgnLast.CombineRgn(&rgnOutside, &rgnInside, RGN_XOR);

		// only diff them if brushes are the same
		if (pBrush->m_hObject == pBrushLast->m_hObject)
		{
			rgnUpdate.CreateRectRgn(0, 0, 0, 0);
			rgnUpdate.CombineRgn(&rgnLast, &rgnNew, RGN_XOR);
		}
	}
	if (pBrush->m_hObject != pBrushLast->m_hObject && lpRectLast != NULL)
	{
		// brushes are different -- erase old region first
		pDC->SelectClipRgn(&rgnLast);
		pDC->GetClipBox(&rect);
		pBrushOld = pDC->SelectObject(pBrushLast);
		pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATINVERT);
      pDC->SelectObject(pBrushOld);
		pBrushOld = NULL;
	}

	// draw into the update/new region
	pDC->SelectClipRgn(rgnUpdate.m_hObject != NULL ? &rgnUpdate : &rgnNew);
	pDC->GetClipBox(&rect);
	pBrushOld = pDC->SelectObject(pBrush);
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATINVERT);

	// cleanup DC
	if (pBrushOld != NULL)
		pDC->SelectObject(pBrushOld);
	pDC->SelectClipRgn(NULL);
   if(bBrushCreated)
      delete pBrush;
}
