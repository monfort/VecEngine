// VecRectanglePic.h: interface for the CVecRectanglePic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECRECTANGLEPIC_H__12AA3CD0_BCEE_11D3_A49C_00C0CA1261A6__INCLUDED_)
#define AFX_VECRECTANGLEPIC_H__12AA3CD0_BCEE_11D3_A49C_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VecRectangle.h"
#include "VecBitmapBase.h"

class CVecRectanglePic : public CVecRectangle , public CVecBitmapBase 
{
public:
	CVecRectanglePic();
	virtual ~CVecRectanglePic();
   virtual void Draw(CDC * pDC, DWORD dwFlags = DF_NORMAL);

   virtual void Copy(CVecBase* pVec);
   virtual void SaveToBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags = SBF_FULL);
   virtual void LoadFromBuffer(CNodedMemoryBufferEx* pMemBuff, int iFlags = SBF_FULL);

public://types
   virtual BOOL HasPicture(){return TRUE;}
};

#endif // !defined(AFX_VECRECTANGLEPIC_H__12AA3CD0_BCEE_11D3_A49C_00C0CA1261A6__INCLUDED_)
