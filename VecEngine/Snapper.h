// Snapper.h: interface for the CSnapper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SNAPPER_H__70B73DF3_C982_11D3_A4A3_00C0CA1261A6__INCLUDED_)
#define AFX_SNAPPER_H__70B73DF3_C982_11D3_A4A3_00C0CA1261A6__INCLUDED_

#include "VecTypes.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSnapper  
{
public:
	CSnapper();
	virtual ~CSnapper();

public: //SnapTo callbacks
   virtual double FixSnapTo(LPCDPOINT lpPoint) = 0;
   virtual double FixSnapTo(LPDRECT lpRect) = 0;
   virtual double FixSnapTo(LPCDPOINT lpPoints, UINT uiCount) = 0;
   virtual int FixSnapTo(LPPOINT lpPoint) = 0;
   virtual int FixSnapTo(LPRECT lpRect) = 0;
   virtual int FixSnapTo(LPPOINT lpPoints, UINT uiCount) = 0;
};

#endif // !defined(AFX_SNAPPER_H__70B73DF3_C982_11D3_A4A3_00C0CA1261A6__INCLUDED_)
