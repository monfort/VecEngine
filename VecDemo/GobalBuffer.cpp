// GobalBuffer.cpp: implementation of the CGobalBuffer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VecDemo.h"
#include "GobalBuffer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGobalBuffer::CGobalBuffer()
{
   m_hGlobal = NULL;
}

CGobalBuffer::~CGobalBuffer()
{
   if(m_hGlobal)
      Free();
}

BOOL CGobalBuffer::Alloc(DWORD dwBytes ,UINT uFlags /*= GHND*/)
{
    m_hGlobal = GlobalAlloc(uFlags, dwBytes);
    if(m_hGlobal)
       return TRUE;
    return FALSE;
}

void* CGobalBuffer::AllocAndLock(DWORD dwBytes ,UINT uFlags /*= GHND*/)
{
    m_hGlobal = GlobalAlloc(uFlags, dwBytes);
    if(!m_hGlobal)
       return NULL;
    return GlobalLock(m_hGlobal);
}

BOOL CGobalBuffer::Unlock()
{
   ASSERT(m_hGlobal);
   return GlobalUnlock(m_hGlobal);
}

void* CGobalBuffer::GetBuffer()
{
    ASSERT(m_hGlobal);
    return GlobalLock(m_hGlobal);
}

HGLOBAL CGobalBuffer::GetCpoyOfBuffer()
{
    ASSERT(m_hGlobal);

    HGLOBAL hGlobal = GlobalAlloc(GHND, GetSize());
    if(!hGlobal)
       return NULL;

    memcpy(GlobalLock(hGlobal), GetBuffer(), GetSize());

    Unlock();

    ASSERT(hGlobal);

    return hGlobal;
}


DWORD CGobalBuffer::GetSize()
{
   if(m_hGlobal) 
      return GlobalSize(m_hGlobal);
   return 0;
}

BOOL CGobalBuffer::ReAlloc(DWORD dwBytes ,UINT uFlags /*= GHND*/)
{
   ASSERT(m_hGlobal);
   m_hGlobal = GlobalReAlloc(m_hGlobal, uFlags, dwBytes);
   if(m_hGlobal)
      return TRUE;
   return FALSE;
}

void* CGobalBuffer::ReAllocAndLock(DWORD dwBytes ,UINT uFlags /*= GHND*/)
{
    ASSERT(m_hGlobal);
    m_hGlobal = GlobalReAlloc(m_hGlobal, uFlags, dwBytes);
    if(!m_hGlobal)
       return NULL;
 
    return GlobalLock(m_hGlobal);
}

UINT CGobalBuffer::GetFlags()
{
   ASSERT(m_hGlobal);
   return GlobalFlags(m_hGlobal);
}

void CGobalBuffer::Free()
{
   m_hGlobal = GlobalFree(m_hGlobal);
   ASSERT(m_hGlobal == NULL);
}

CGobalBuffer & CGobalBuffer::operator=(HGLOBAL hGlobal)
{
   ASSERT(m_hGlobal == NULL);
   m_hGlobal = hGlobal;
   return *this;
}