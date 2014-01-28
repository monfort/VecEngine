/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// VecTextBase.h: interface for the CVecTextBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECTEXTBASE_H__FB60F665_BE7E_11D3_A49C_00C0CA1261A6__INCLUDED_)
#define AFX_VECTEXTBASE_H__FB60F665_BE7E_11D3_A49C_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VecBase.h"

#define AL_LEFT 0
#define AL_CENTER 1
#define AL_RIGHT 2


class CNodedMemoryBufferEx;

class CVecTextBase  
{
public:
	CVecTextBase();
	virtual ~CVecTextBase();

   virtual void SaveToBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags = SBF_FULL);
   virtual void LoadFromBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags = SBF_FULL);
   virtual void DrawText(CDC* pDC, LPRECTANGLE lpRectangle, DWORD dwFlags = DF_NORMAL);
   virtual void DrawText(CDC* pDC, LPRECT lpRect, DWORD dwFlags = DF_NORMAL);
   virtual void Copy(CVecTextBase* pVec);
   virtual BOOL GotoEditMode();

   void SetText(const CString &strTest){m_strText = strTest;}
   void SetText(LPCSTR lpszTest){m_strText = lpszTest;}
   CString GetText() const {return m_strText;}
   void SetFont(const LOGFONT *lplf){memcpy(&m_lfFont, lplf, sizeof(LOGFONT));}
   void GetFont(LOGFONT *lplf) {memcpy(lplf, &m_lfFont, sizeof(LOGFONT));}
   void SetAligment(int i){m_iAlignment = i;}
   int GetAligment(){return m_iAlignment;}

protected:
   CString m_strText;
   LOGFONT m_lfFont;
   int m_iAlignment;

private:
   enum VEC_FIELDS // fields ids (file type) - do not cheange values
   {
      VF_Text = 0,
      VF_Font,
      VF_Algment
   };
};

#endif // !defined(AFX_VECTEXTBASE_H__FB60F665_BE7E_11D3_A49C_00C0CA1261A6__INCLUDED_)
