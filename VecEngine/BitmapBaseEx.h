// BitmapBaseEx.h: interface for the CBitmapBaseEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BITMAPBASEEX_H__0534C431_BDBD_11D3_A49C_00C0CA1261A6__INCLUDED_)
#define AFX_BITMAPBASEEX_H__0534C431_BDBD_11D3_A49C_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define abs(x)                      (((x)<0)  ?-(x):(x))

class CBitmapBaseEx 
{
public:
	CBitmapBaseEx();
	virtual ~CBitmapBaseEx();

	HBITMAP GetHandle() const {return (HBITMAP)m_cbmp.m_hObject;};

	void SetBitmap(const CBitmap& bitmap);
    void GetBitmap(CBitmap& bitmap);
	int GetWidth();
	int GetHeight();
	int IsAllocated();

    int LoadBitmapFromResource(UINT uiResourceId);
	int LoadBitmapFromFile(TCHAR szFileName[]);

	int Create(int uWidth=100,int uHeight=100);
	int CreateMask(CBitmap& pBitmapMask, CBitmap& pBitmapMasked, COLORREF rgbTransparent);
	int Copy(CBitmapBaseEx& pBitmap);
	int Free();
	
	int Draw(HDC hDC, LPRECT prcDst);
	int DrawTransparent(HDC hDC, LPRECT prcDst,CBitmap& bmpMask,CBitmap& bmpMasked);

private:
	HBITMAP CloneBitmap( HBITMAP hSourceBitmap);
	HBITMAP CreateBitmapMask( HBITMAP hSourceBitmap, COLORREF rgbTransparent );
	HBITMAP CreateBitmapMasked( HBITMAP hSourceBitmap, HBITMAP hMaskBitmap );

protected:
	CBitmap m_cbmp;
};

#endif // !defined(AFX_BITMAPBASEEX_H__0534C431_BDBD_11D3_A49C_00C0CA1261A6__INCLUDED_)
