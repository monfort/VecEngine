/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// BaseTracker.h: interface for the CBaseTracker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASETRACKER_H__12AA3CCD_BCEE_11D3_A49C_00C0CA1261A6__INCLUDED_)
#define AFX_BASETRACKER_H__12AA3CCD_BCEE_11D3_A49C_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CBaseTracker 
{
public:
   CBaseTracker();
   BOOL CheckRetainProportions();
   inline void SetTrackRetainProportions(BOOL b = TRUE){m_bRetainProportions = b;}
   inline BOOL GetTrackRetainProportions(BOOL b = TRUE){return m_bRetainProportions;}
protected:
   BOOL m_bRetainProportions;
};

#endif // !defined(AFX_BASETRACKER_H__12AA3CCD_BCEE_11D3_A49C_00C0CA1261A6__INCLUDED_)
