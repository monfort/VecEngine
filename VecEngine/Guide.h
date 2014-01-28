/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

// Guide.h: interface for the CGuide class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUIDE_H__FFB8D6CD_2975_47C8_B903_95EFCFC429E0__INCLUDED_)
#define AFX_GUIDE_H__FFB8D6CD_2975_47C8_B903_95EFCFC429E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGuide  
{
   friend class CVecController;
public:
	CGuide(BOOL bIsHorizontal, double dPos = 0);
	virtual ~CGuide();

   inline double GetPos(){return m_dPos;}
   inline double GetScreenPos(){return m_t_dPos;}
   inline void GetPos(double d){m_dPos = d;}
   inline void GetScreenPos(double d){m_t_dPos = d;}

private:
   BOOL m_bIsHorizontal;
   double m_dPos;
   double m_t_dPos;
};

#endif // !defined(AFX_GUIDE_H__FFB8D6CD_2975_47C8_B903_95EFCFC429E0__INCLUDED_)
