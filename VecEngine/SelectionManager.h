/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// SekectionManager.h: interface for the CSelectionManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEKECTIONMANAGER_H__6318D40B_B6F0_11D3_A497_00C0CA1261A6__INCLUDED_)
#define AFX_SEKECTIONMANAGER_H__6318D40B_B6F0_11D3_A497_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CVecContainer;
class CVecBase;
class CVecWorkspace;

class CSelectionManager 
{
   friend class CVecWorkspace;
public:
	CSelectionManager();
	virtual ~CSelectionManager();
   void CheckItem(CVecBase* pVec);
   void RemoveItem(CVecBase* pVec);
   UINT GetSelectedItems(CPtrList& SellItems);
   UINT GetSelectedItemsCount();
   void SetSelectedNone();
private:
   CVecWorkspace* m_pWorkspace;
   CPtrList m_SelectedItems;

};

#endif // !defined(AFX_SEKECTIONMANAGER_H__6318D40B_B6F0_11D3_A497_00C0CA1261A6__INCLUDED_)
