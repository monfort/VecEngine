/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// DragMan.h: interface for the CDragManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAGMAN_H__70B73DF3_C982_11D3_A4A3_00C0CA1261A6__INCLUDED_)
#define AFX_DRAGMAN_H__70B73DF3_C982_11D3_A4A3_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum VEC_DRAG_ACTION
{
   VDA_CANCEL,
   VDA_CONTINUE
};

class CDragManager  
{
public:
	CDragManager();
	virtual ~CDragManager();

public: //SnapTo callbacks
   virtual int CheckDragAction() = 0;
};

#endif // !defined(AFX_DRAGMAN_H__70B73DF3_C982_11D3_A4A3_00C0CA1261A6__INCLUDED_)
