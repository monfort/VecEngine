/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// VecWorkspace.h: interface for the CVecWorkspace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECWORKSPACE_H__6318D40B_B6F0_11D3_A497_00C0CA1261A6__INCLUDED_)
#define AFX_VECWORKSPACE_H__6318D40B_B6F0_11D3_A497_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SelectionManager.h"

class CVecContainer;
class CVecBase;

class CVecWorkspace 
{
public:
	CVecWorkspace();
	virtual ~CVecWorkspace();

   CVecContainer* m_pVecContainer;
   CSelectionManager m_SelectionManager;
};

#endif // !defined(AFX_VECWORKSPACE_H__6318D40B_B6F0_11D3_A497_00C0CA1261A6__INCLUDED_)
