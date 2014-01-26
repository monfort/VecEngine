// GobalBuffer.h: interface for the CGobalBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GOBALBUFFER_H__B7623437_AAD9_11D3_9C15_0020182C5402__INCLUDED_)
#define AFX_GOBALBUFFER_H__B7623437_AAD9_11D3_9C15_0020182C5402__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGobalBuffer  
{
public:
	void Free();
   CGobalBuffer();
	virtual ~CGobalBuffer();

   BOOL Alloc(DWORD dwBytes ,UINT uFlags = GHND);
   void* AllocAndLock(DWORD dwBytes ,UINT uFlags = GHND);
   
   BOOL ReAlloc(DWORD dwBytes ,UINT uFlags = GHND);
   void* ReAllocAndLock(DWORD dwBytes ,UINT uFlags = GHND);

   operator HGLOBAL() const { return  m_hGlobal;}
   CGobalBuffer& operator=(HGLOBAL hGlobal);
   BOOL operator!=(const HGLOBAL & hGlobal) const { return  m_hGlobal != hGlobal;}
   BOOL operator==(const HGLOBAL & hGlobal) const { return  m_hGlobal == hGlobal;}


   void* GetBuffer();
   HGLOBAL GetCpoyOfBuffer();


   DWORD GetSize();                            
	UINT GetFlags();
	BOOL Unlock();

protected:
   HGLOBAL m_hGlobal;
};

#endif // !defined(AFX_GOBALBUFFER_H__B7623437_AAD9_11D3_9C15_0020182C5402__INCLUDED_)
