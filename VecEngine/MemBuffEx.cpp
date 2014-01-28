/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

#include "stdafx.h"
#include "MemBuffEx.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// CMemoryBufferEx /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
CMemoryBufferEx::CMemoryBufferEx(DWORD dwGrow, DWORD dwInitialSize):
m_lpvMemory(NULL),
m_lpvCurrntPos(NULL),
m_dwSize(0),
m_dwGrow(dwGrow),
m_bIsStoring(TRUE),
m_dwMarkPos(0)
{
   if(dwInitialSize == DWORD(-1))
      dwInitialSize = m_dwGrow;

   m_dwSize = dwInitialSize;
   m_lpvMemory = realloc(m_lpvMemory, m_dwSize);
   m_lpvCurrntPos = m_lpvMemory;
}

CMemoryBufferEx::CMemoryBufferEx(void* lpvData, DWORD dwSize):
m_lpvMemory(lpvData),
m_lpvCurrntPos(lpvData),
m_dwSize(dwSize),
m_dwGrow(0),
m_bIsStoring(FALSE),
m_dwMarkPos(0)
{
}

CMemoryBufferEx::~CMemoryBufferEx()
{
   if(IsStoring())
      free(m_lpvMemory);
}


void CMemoryBufferEx::Realloc(DWORD dwSize)
{
   ASSERT(IsStoring());
   DWORD dwSizeWritten = ((DWORD) m_lpvCurrntPos) - ((DWORD) m_lpvMemory);

   m_dwSize += ((dwSize / m_dwGrow) + 1) * m_dwGrow;

   m_lpvMemory = realloc(m_lpvMemory, m_dwSize);

   m_lpvCurrntPos = (void*) (((DWORD)m_lpvMemory) + dwSizeWritten);
}

void CMemoryBufferEx::CheckIfSizeIsEnoughAndFixIt(DWORD dwSize)
{
   ASSERT(IsStoring());
   DWORD dwSizeWritten = ((DWORD) m_lpvCurrntPos) - ((DWORD) m_lpvMemory);
   DWORD dwSpaceLeft = m_dwSize - dwSizeWritten;
   if(dwSize >= dwSpaceLeft)
   {
      Realloc(dwSize - dwSpaceLeft);
   }
}


void CMemoryBufferEx::Seek(DWORD dwOff, UINT nFrom)
{
   ASSERT(IsLoading());
   switch(nFrom)
   {
   case SEEK_BEGIN:
      m_lpvCurrntPos = (void*) (((DWORD) m_lpvMemory) + dwOff);
      break;
   case SEEK_CURRENT:
      m_lpvCurrntPos = (void*) (((DWORD) m_lpvCurrntPos) + dwOff);
      break;
   default:
      ASSERT(FALSE);
      break;
   }

}

void CMemoryBufferEx::Mark()
{
   ASSERT(IsLoading());
   m_dwMarkPos = ((DWORD) m_lpvCurrntPos) - ((DWORD) m_lpvMemory);
}

void CMemoryBufferEx::Reset()
{
   ASSERT(IsLoading());
   m_lpvCurrntPos = (void*) (((DWORD) m_lpvMemory) + m_dwMarkPos);
}

DWORD CMemoryBufferEx::CopyData(void* lpvData, DWORD dwSize)
{
   ASSERT(IsStoring());
   DWORD dwSizeWritten = ((DWORD) m_lpvCurrntPos) - ((DWORD) m_lpvMemory);
   if(lpvData == NULL || dwSizeWritten > dwSize)
   {
      return dwSizeWritten;
   }

   memcpy(lpvData, m_lpvMemory, dwSizeWritten);
   return 0;
}
///////////////////////////// Write ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
void CMemoryBufferEx::Write(void* lpvData, DWORD dwSize)
{
   ASSERT(IsStoring());
   CheckIfSizeIsEnoughAndFixIt(dwSize);
   memcpy(m_lpvCurrntPos, lpvData, dwSize);
   m_lpvCurrntPos = (void*) (((DWORD)m_lpvCurrntPos) + dwSize);
}

void CMemoryBufferEx::WriteDword(DWORD dwVal)
{
   Write(&dwVal, sizeof(DWORD));
}

void CMemoryBufferEx::WriteByte(BYTE byVal)
{
   Write(&byVal, sizeof(BYTE));
}

void CMemoryBufferEx::WriteWord(WORD wVal)
{
   Write(&wVal, sizeof(WORD));
}

void CMemoryBufferEx::WriteLong(LONG lVal)
{
   Write(&lVal, sizeof(LONG));
}

void CMemoryBufferEx::WriteInt(INT iVal)
{
   Write(&iVal, sizeof(INT));
}

void CMemoryBufferEx::WriteBool(BOOL bVal)
{
   Write(&bVal, sizeof(BOOL));
}

void CMemoryBufferEx::WriteString(CString strVal)
{
   WriteString((LPSTR)(LPCSTR)strVal);
}

void CMemoryBufferEx::WriteString(LPSTR lpszVal)
{
   Write(lpszVal, (strlen(lpszVal)+1)*sizeof(char));
}

void CMemoryBufferEx::WriteFloat(float fVal)
{
   Write(&fVal, sizeof(float));
}

void CMemoryBufferEx::WriteDouble(double dVal)
{
   Write(&dVal, sizeof(double));
}

void CMemoryBufferEx::WritePoint(POINT &Val)
{
   Write(&Val, sizeof(POINT));
}

void CMemoryBufferEx::WriteRect(RECT &Val)
{
   Write(&Val, sizeof(RECT));
}

void CMemoryBufferEx::WriteDPoint(DPOINT &Val)
{
   Write(&Val, sizeof(DPOINT));
}

void CMemoryBufferEx::WriteDRect(DRECT &Val)
{
   Write(&Val, sizeof(DRECT));
}
///////////////////////////// Read ////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
void CMemoryBufferEx::Read(void* lpvData, DWORD dwSize)
{
   ASSERT(!IsStoring());
   ASSERT(m_dwSize >= ((DWORD)m_lpvCurrntPos) - ((DWORD)m_lpvMemory) + dwSize);
   memcpy(lpvData, m_lpvCurrntPos, dwSize);
   m_lpvCurrntPos = (void*) (((DWORD)m_lpvCurrntPos) + dwSize);
}

DWORD CMemoryBufferEx::ReadDword()
{
   DWORD dwVal;
   Read(&dwVal, sizeof(DWORD));
   return dwVal;
}

BYTE CMemoryBufferEx::ReadByte()
{
   BYTE byVal;
   Read(&byVal, sizeof(BYTE));
   return byVal;
}

WORD CMemoryBufferEx::ReadWord()
{
   WORD wVal;
   Read(&wVal, sizeof(WORD));
   return wVal;
}

LONG CMemoryBufferEx::ReadLong()
{
   LONG lVal;
   Read(&lVal, sizeof(LONG));
   return lVal;
}


INT CMemoryBufferEx::ReadInt()
{
   INT iVal;
   Read(&iVal, sizeof(INT));
   return iVal;
}

BOOL CMemoryBufferEx::ReadBool()
{
   BOOL bVal;
   Read(&bVal, sizeof(BOOL));
   return bVal;
}

CString CMemoryBufferEx::ReadString()
{
   int iSize = ReadString(NULL, 0);
   CString strVal;
   ReadString(strVal.GetBuffer(iSize), iSize);
   strVal.ReleaseBuffer();
   return strVal;
}

int CMemoryBufferEx::ReadString(LPSTR lpszVal, int iSize)//return 0 if size was Enough or the size if not (size includes the null termination char)
{
   ASSERT(!IsStoring());
   int iRealSize;
   for(iRealSize = 1; ; iRealSize++)
   {
      ASSERT(m_dwSize >= ((DWORD)m_lpvCurrntPos) - ((DWORD)m_lpvMemory) + iRealSize - 1);
      if(*((char*)((DWORD)m_lpvCurrntPos) + iRealSize - 1) == '\0')
         break;
   }
   if(lpszVal != NULL && iRealSize <= iSize)
   {
      Read(lpszVal, iRealSize);
      return 0;
   }
   else
      return iRealSize;
}

float CMemoryBufferEx::ReadFloat()
{
   float fVal;
   Read(&fVal, sizeof(float));
   return fVal;
}

double CMemoryBufferEx::ReadDouble()
{
   double dVal;
   Read(&dVal, sizeof(double));
   return dVal;
}

void CMemoryBufferEx::ReadPoint(POINT* pVal)
{
   Read(pVal, sizeof(POINT));
}

void CMemoryBufferEx::ReadRect(RECT* pVal)
{
   Read(pVal, sizeof(RECT));
}

void CMemoryBufferEx::ReadDPoint(DPOINT* pVal)
{
   Read(pVal, sizeof(DPOINT));
}

void CMemoryBufferEx::ReadDRect(DRECT* pVal)
{
   Read(pVal, sizeof(DRECT));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// CNodedMemoryBufferEx ////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

CNodedMemoryBufferEx::CNodedMemoryBufferEx(DWORD dwGrow, DWORD dwInitialSize):
CMemoryBufferEx(dwGrow, dwInitialSize)
{
   m_iNodeType = ID_END_OF_FIELDS;
   m_iFieldType = ID_END_OF_FIELDS;
   m_dwFieldSize = 0;
}

CNodedMemoryBufferEx::CNodedMemoryBufferEx(void* lpvData, DWORD dwSize):
CMemoryBufferEx(lpvData, dwSize)
{
   m_iNodeType = ID_END_OF_FIELDS;
   m_iFieldType = ID_END_OF_FIELDS;
   m_dwFieldSize = 0;
}

CNodedMemoryBufferEx::~CNodedMemoryBufferEx()
{
}

BOOL CNodedMemoryBufferEx::ReadNextFieldHeader()
{
   ASSERT(IsLoading());
   m_iNodeType = ID_END_OF_FIELDS;
   m_dwFieldSize = 0;

   m_iFieldType = ReadInt();

   switch(m_iFieldType)
   {
   case ID_END_OF_FIELDS:
      return FALSE;

   case ID_FIELD_IS_NODE:
      m_iNodeType = ReadInt();
      return TRUE;

   default:
      m_dwFieldSize = ReadDword();
      return TRUE;
   }
}

void CNodedMemoryBufferEx::SkipField()
{
   ASSERT(IsLoading());
   if(m_iFieldType == ID_FIELD_IS_NODE)
      SkipNode();
   else
   {
      Seek(m_dwFieldSize, SEEK_CURRENT);
   }
}
	
void CNodedMemoryBufferEx::SkipNode()
{
   while(ReadNextFieldHeader())
   {
      SkipField(); 
   }
}

void CNodedMemoryBufferEx::WriteNodeHeader(int iNodeType)
{
   ASSERT(IsStoring());
   int i = ID_FIELD_IS_NODE;
   WriteInt(i);

   WriteInt(iNodeType);
}

void CNodedMemoryBufferEx::WriteEndOfFields()
{
   ASSERT(IsStoring());
   int i = ID_END_OF_FIELDS;
   WriteInt(i);
}
	
/////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////// Write APIz ////////////////////////////////////////////////////////////////

void CNodedMemoryBufferEx::WriteField(int iFieldId, void* lpvData, DWORD dwSize)
{
   ASSERT(IsStoring());
   WriteInt(iFieldId);
   WriteDword(dwSize);
   Write(lpvData, dwSize);
}

void CNodedMemoryBufferEx::WriteDwordField(int iFieldId, DWORD dwVal)
{
   ASSERT(IsStoring());
   WriteInt(iFieldId);
   WriteDword(sizeof(DWORD));
   WriteDword(dwVal);
}

void CNodedMemoryBufferEx::WriteByteField(int iFieldId, BYTE byVal)
{
   ASSERT(IsStoring());
   WriteInt(iFieldId);
   WriteDword(sizeof(BYTE));
   WriteByte(byVal);
}

void CNodedMemoryBufferEx::WriteWordField(int iFieldId, WORD wVal)
{
   ASSERT(IsStoring());
   WriteInt(iFieldId);
   WriteDword(sizeof(WORD));
   WriteWord(wVal);
}

void CNodedMemoryBufferEx::WriteLongField(int iFieldId, LONG lVal)
{
   ASSERT(IsStoring());
   WriteInt(iFieldId);
   WriteDword(sizeof(LONG));
   WriteLong(lVal);
}

void CNodedMemoryBufferEx::WriteIntField(int iFieldId, INT iVal)
{
   ASSERT(IsStoring());
   WriteInt(iFieldId);
   WriteDword(sizeof(INT));
   WriteInt(iVal);
}

void CNodedMemoryBufferEx::WriteBoolField(int iFieldId, BOOL bVal)
{
   ASSERT(IsStoring());
   WriteInt(iFieldId);
   WriteDword(sizeof(BOOL));
   WriteBool(bVal);
}

void CNodedMemoryBufferEx::WriteStringField(int iFieldId, CString strVal)
{
   ASSERT(IsStoring());
   WriteInt(iFieldId);
   WriteDword((strVal.GetLength()+1)*sizeof(char));
   WriteString(strVal);
}

void CNodedMemoryBufferEx::WriteStringField(int iFieldId, LPSTR lpszVal)
{
   ASSERT(IsStoring());
   WriteInt(iFieldId);
   WriteDword((strlen(lpszVal)+1)*sizeof(char));
   WriteString(lpszVal);
}

void CNodedMemoryBufferEx::WriteFloatField(int iFieldId, float fVal)
{
   ASSERT(IsStoring());
   WriteInt(iFieldId);
   WriteDword(sizeof(float));
   WriteFloat(fVal);
}

void CNodedMemoryBufferEx::WriteDoubleField(int iFieldId, double dVal)
{
   ASSERT(IsStoring());
   WriteInt(iFieldId);
   WriteDword(sizeof(double));
   WriteDouble(dVal);
}

void CNodedMemoryBufferEx::WritePointField(int iFieldId, POINT &Val)
{
   ASSERT(IsStoring());
   WriteInt(iFieldId);
   WriteDword(sizeof(double));
   WritePoint(Val);
}

void CNodedMemoryBufferEx::WriteRectField(int iFieldId, RECT &Val)
{
   ASSERT(IsStoring());
   WriteInt(iFieldId);
   WriteDword(sizeof(double));
   WriteRect(Val);
}

void CNodedMemoryBufferEx::WriteDPointField(int iFieldId, DPOINT &Val)
{
   ASSERT(IsStoring());
   WriteInt(iFieldId);
   WriteDword(sizeof(double));
   WriteDPoint(Val);
}

void CNodedMemoryBufferEx::WriteDRectField(int iFieldId, DRECT &Val)
{
   ASSERT(IsStoring());
   WriteInt(iFieldId);
   WriteDword(sizeof(double));
   WriteDRect(Val);
}

