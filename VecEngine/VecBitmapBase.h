/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// VecBitmapBase.h: interface for the CVecBitmapBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECBITMAPBASE_H__DEAD8EE8_BDC0_11D3_A49C_00C0CA1261A6__INCLUDED_)
#define AFX_VECBITMAPBASE_H__DEAD8EE8_BDC0_11D3_A49C_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BitmapBaseEx.h"
#include "VecBase.h"
class CNodedMemoryBufferEx;

class CVecBitmapBase  
{
public:
	CVecBitmapBase();
	virtual ~CVecBitmapBase();

   void SetBitmap(CBitmapBaseEx *pBitmap);
   CBitmapBaseEx* GetBitmap(){return &m_bitmap;}
   virtual void DrawBitmap(CDC* pDC, LPRECTANGLE lpRectangle, DWORD dwFlags = DF_NORMAL);
   virtual void DrawBitmap(CDC* pDC, LPRECT lpRect, DWORD dwFlags = DF_NORMAL);

   inline BOOL HasBitmap(){return m_bitmap.IsAllocated();}
   virtual void SaveToBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags = SBF_FULL);
   virtual void LoadFromBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags = SBF_FULL);

   virtual void Copy(CVecBitmapBase* pVec);

   inline void SetTransparent(BOOL b){m_bIsTransparent = b;}
   inline BOOL IsTransparent(){return m_bIsTransparent;}
   inline void SetTransparentColor(COLORREF col)
   {
	   m_crTransparentColor = col;
	   m_mask.DeleteObject();
	   m_masked.DeleteObject();
   }
   inline COLORREF GetTransparentColor(){return m_crTransparentColor;}

protected:
   CBitmapBaseEx m_bitmap;
   BOOL m_bIsTransparent;
   COLORREF m_crTransparentColor;

   CBitmap m_mask;
   CBitmap m_masked;

private:
    virtual PXFORM CalcRotationAndShear(double radRotation,double dShearAng, XFORM& xform);
    virtual PXFORM CalcFlip( XFORM& xform);
    virtual PXFORM CalcReverse( XFORM& xform);
	virtual PXFORM CalcScale(LPRECTANGLE lpRectangle, XFORM& xform);
	virtual PXFORM CalcTranslation(int xTrans,int yTrans, XFORM& xform);
	PBITMAPINFO CreateBitmapInfoStruct(HBITMAP hBmp, unsigned long& size);
	LPBYTE GetRawBitmapData(unsigned long& bmiSize, unsigned long& bmSize);
	void SetRawBitmapData(LPBYTE lpData, unsigned long size);

	WORD CalcColorBits(unsigned short biPlanes, unsigned short biBitCount);
	unsigned long CalcBitmapInfoStructSize(WORD cClrBits);
	unsigned long CalcBitmapImageSize(long biWidth, long biHeight, WORD cClrBits);


   enum VEC_FIELDS // fields ids (file type) - do not cheange values
   {
      VF_Bitmap = 0,
      VF_IsTransparent,
      VF_TransparentColor
   };
};

#endif // !defined(AFX_VECBITMAPBASE_H__DEAD8EE8_BDC0_11D3_A49C_00C0CA1261A6__INCLUDED_)
