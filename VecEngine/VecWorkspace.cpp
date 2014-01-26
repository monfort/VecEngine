// VecWorkspace.cpp: implementation of the CVecWorkspace class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VecWorkspace.h"
#include "VecContainer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVecWorkspace::CVecWorkspace()
{
   m_SelectionManager.m_pWorkspace = this;
   m_pVecContainer = NULL;
}

CVecWorkspace::~CVecWorkspace()
{
}
