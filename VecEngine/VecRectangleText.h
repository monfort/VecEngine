// VecRectangleText.h: interface for the CVecRectangleText class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECRECTANGLETEXT_H__FB60F666_BE7E_11D3_A49C_00C0CA1261A6__INCLUDED_)
#define AFX_VECRECTANGLETEXT_H__FB60F666_BE7E_11D3_A49C_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VecTextBase.h"
#include "VecRectangle.h"

class CVecRectangleText : 	public CVecRectangle, 	public CVecTextBase  
{
public:
	CVecRectangleText();
	virtual ~CVecRectangleText();
   virtual void Draw(CDC * pDC, DWORD dwFlags = DF_NORMAL);

   virtual void Copy(CVecBase* pVec);
   virtual BOOL OnDBClick(UINT nFlags, CPoint point, CUndoManager* pUndoMan);
   virtual void SaveToBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags = SBF_FULL);
   virtual void LoadFromBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags = SBF_FULL);

public://types
   virtual BOOL HasText(){return TRUE;}
};

#endif // !defined(AFX_VECRECTANGLETEXT_H__FB60F666_BE7E_11D3_A49C_00C0CA1261A6__INCLUDED_)
