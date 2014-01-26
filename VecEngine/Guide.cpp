// Guide.cpp: implementation of the CGuide class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Guide.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuide::CGuide(BOOL bIsHorizontal, double dPos)
{
   m_bIsHorizontal = bIsHorizontal;
   m_dPos = dPos;
   m_t_dPos = 0;
}

CGuide::~CGuide()
{

}
