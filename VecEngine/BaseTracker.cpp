/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// BaseTracker.cpp: implementation of the CBaseTracker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseTracker.h"

CBaseTracker::CBaseTracker()
{
   m_bRetainProportions = FALSE;
}

BOOL CBaseTracker::CheckRetainProportions()
{
   BOOL b = (::GetKeyState(VK_SHIFT) & 0x8000)!=0;
   if(m_bRetainProportions)
      b = !b;
   return b;
}

