// NodedArchive.cpp: implementation of the CNodedArchive class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NodedArchive.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define ID_NODED_ARCHIVE 22222
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNodedArchive::CNodedArchive(const CString& strHeader, CFile* pFile, UINT nMode, int nBufSize, void* lpBuf): 
CArchive(pFile, nMode, nBufSize, lpBuf)
{
   m_iNodeType = ID_END_OF_FIELDS;
   m_iFieldType = ID_END_OF_FIELDS;
   m_iFieldSize = 0;
   m_bNodedFormat = FALSE;  
   if(IsStoring())
   {
      int id = ID_NODED_ARCHIVE;
      *this << id;
      *this << strHeader;
   }
   else if(IsLoading())
   {
      if(pFile->GetLength() > sizeof(int))
      {
         int id;
         *this >> id;
         if(id != ID_NODED_ARCHIVE)
         {
            m_bNodedFormat = FALSE;
            return;
         }
         CString str;
         *this >> str;
         if(str != strHeader)
         {
            m_bNodedFormat = FALSE;
            return;
         }
         m_bNodedFormat = TRUE;
      }
   }
}

CNodedArchive::~CNodedArchive()
{

}

BOOL CNodedArchive::ReadNextFieldHeader()
{
   ASSERT(IsLoading());
   if(!m_bNodedFormat)
      return FALSE;

   m_iNodeType = ID_END_OF_FIELDS;
   m_iFieldSize = 0;

   *this>>m_iFieldType;

   switch(m_iFieldType)
   {
   case ID_END_OF_FIELDS:
      return FALSE;

   case ID_FIELD_IS_NODE:
      *this>>m_iNodeType;
      return TRUE;

   default:
      *this>>m_iFieldSize;
      return TRUE;
   }
}

void CNodedArchive::SkipField()
{
   ASSERT(IsLoading());
   if(m_iFieldType == ID_FIELD_IS_NODE)
      SkipNode();
   else
   {
      Flush();
      GetFile()->Seek(m_iFieldSize, CFile::current);
   }
}

void CNodedArchive::SkipNode()
{
   while(ReadNextFieldHeader())
   {
      SkipField(); 
   }
}

void CNodedArchive::WriteFieldHeader(int iFieldId, int iSize)
{
   ASSERT(IsStoring());
   *this<<iFieldId;
   *this<<iSize;
}

void CNodedArchive::WriteNodeHeader(int iNodeType)
{
   ASSERT(IsStoring());
   int i = ID_FIELD_IS_NODE;
   *this<<i;

   *this<<iNodeType;
}

void CNodedArchive::WriteEndOfFields()
{
   ASSERT(IsStoring());
   int i = ID_END_OF_FIELDS;
   *this<<i;
}

