#include "stdafx.h"
#include "VecDemo.h"
#include "MemBuff.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// CMemoryBuffer /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
CMemoryBuffer::CMemoryBuffer(DWORD dwGrow, DWORD dwInitialSize):
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

CMemoryBuffer::CMemoryBuffer(void* lpvData, DWORD dwSize):
m_lpvMemory(lpvData),
m_lpvCurrntPos(lpvData),
m_dwSize(dwSize),
m_dwGrow(0),
m_bIsStoring(FALSE),
m_dwMarkPos(0)
{
}

CMemoryBuffer::~CMemoryBuffer()
{
   if(IsStoring())
      free(m_lpvMemory);
}


void CMemoryBuffer::Realloc(DWORD dwSize)
{
   ASSERT(IsStoring());
   DWORD dwSizeWritten = ((DWORD) m_lpvCurrntPos) - ((DWORD) m_lpvMemory);

   m_dwSize += ((dwSize / m_dwGrow) + 1) * m_dwGrow;

   m_lpvMemory = realloc(m_lpvMemory, m_dwSize);

   m_lpvCurrntPos = (void*) (((DWORD)m_lpvMemory) + dwSizeWritten);
}

void CMemoryBuffer::CheckIfSizeIsEnoughAndFixIt(DWORD dwSize)
{
   ASSERT(IsStoring());
   DWORD dwSizeWritten = ((DWORD) m_lpvCurrntPos) - ((DWORD) m_lpvMemory);
   DWORD dwSpaceLeft = m_dwSize - dwSizeWritten;
   if(dwSize >= dwSpaceLeft)
   {
      Realloc(dwSize - dwSpaceLeft);
   }
}


void CMemoryBuffer::Seek(DWORD dwOff, UINT nFrom)
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

void CMemoryBuffer::Mark()
{
   ASSERT(IsLoading());
   m_dwMarkPos = ((DWORD) m_lpvCurrntPos) - ((DWORD) m_lpvMemory);
}

void CMemoryBuffer::Reset()
{
   ASSERT(IsLoading());
   m_lpvCurrntPos = (void*) (((DWORD) m_lpvMemory) + m_dwMarkPos);
}

DWORD CMemoryBuffer::CopyData(void* lpvData, DWORD dwSize)
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
void CMemoryBuffer::Write(void* lpvData, DWORD dwSize)
{
   ASSERT(IsStoring());
   CheckIfSizeIsEnoughAndFixIt(dwSize);
   memcpy(m_lpvCurrntPos, lpvData, dwSize);
   m_lpvCurrntPos = (void*) (((DWORD)m_lpvCurrntPos) + dwSize);
}

void CMemoryBuffer::WriteDword(DWORD dwVal)
{
   Write(&dwVal, sizeof(DWORD));
}

void CMemoryBuffer::WriteByte(BYTE byVal)
{
   Write(&byVal, sizeof(BYTE));
}

void CMemoryBuffer::WriteWord(WORD wVal)
{
   Write(&wVal, sizeof(WORD));
}

void CMemoryBuffer::WriteLong(LONG lVal)
{
   Write(&lVal, sizeof(LONG));
}

void CMemoryBuffer::WriteInt(INT iVal)
{
   Write(&iVal, sizeof(INT));
}

void CMemoryBuffer::WriteBool(BOOL bVal)
{
   Write(&bVal, sizeof(BOOL));
}

void CMemoryBuffer::WriteString(CString strVal)
{
   WriteString((LPSTR)(LPCSTR)strVal);
}

void CMemoryBuffer::WriteString(LPSTR lpszVal)
{
   Write(lpszVal, (strlen(lpszVal)+1)*sizeof(char));
}

void CMemoryBuffer::WriteFloat(float fVal)
{
   Write(&fVal, sizeof(float));
}

void CMemoryBuffer::WriteDouble(double dVal)
{
   Write(&dVal, sizeof(double));
}

///////////////////////////// Read ////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
void CMemoryBuffer::Read(void* lpvData, DWORD dwSize)
{
   ASSERT(!IsStoring());
   ASSERT(m_dwSize >= ((DWORD)m_lpvCurrntPos) - ((DWORD)m_lpvMemory) + dwSize);
   memcpy(lpvData, m_lpvCurrntPos, dwSize);
   m_lpvCurrntPos = (void*) (((DWORD)m_lpvCurrntPos) + dwSize);
}

DWORD CMemoryBuffer::ReadDword()
{
   DWORD dwVal;
   Read(&dwVal, sizeof(DWORD));
   return dwVal;
}

BYTE CMemoryBuffer::ReadByte()
{
   BYTE byVal;
   Read(&byVal, sizeof(BYTE));
   return byVal;
}

WORD CMemoryBuffer::ReadWord()
{
   WORD wVal;
   Read(&wVal, sizeof(WORD));
   return wVal;
}

LONG CMemoryBuffer::ReadLong()
{
   LONG lVal;
   Read(&lVal, sizeof(LONG));
   return lVal;
}


INT CMemoryBuffer::ReadInt()
{
   INT iVal;
   Read(&iVal, sizeof(INT));
   return iVal;
}

BOOL CMemoryBuffer::ReadBool()
{
   BOOL bVal;
   Read(&bVal, sizeof(BOOL));
   return bVal;
}

CString CMemoryBuffer::ReadString()
{
   int iSize = ReadString(NULL, 0);
   CString strVal;
   ReadString(strVal.GetBuffer(iSize), iSize);
   strVal.ReleaseBuffer();
   return strVal;
}

int CMemoryBuffer::ReadString(LPSTR lpszVal, int iSize)//return 0 if size was Enough or the size if not (size includes the null termination char)
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

float CMemoryBuffer::ReadFloat()
{
   float fVal;
   Read(&fVal, sizeof(float));
   return fVal;
}

double CMemoryBuffer::ReadDouble()
{
   double dVal;
   Read(&dVal, sizeof(double));
   return dVal;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// CNodedMemoryBuffer ////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

CNodedMemoryBuffer::CNodedMemoryBuffer(DWORD dwGrow, DWORD dwInitialSize):
CMemoryBuffer(dwGrow, dwInitialSize)
{
   m_iNodeType = ID_END_OF_FIELDS;
   m_iFieldType = ID_END_OF_FIELDS;
   m_dwFieldSize = 0;
}

CNodedMemoryBuffer::CNodedMemoryBuffer(void* lpvData, DWORD dwSize):
CMemoryBuffer(lpvData, dwSize)
{
   m_iNodeType = ID_END_OF_FIELDS;
   m_iFieldType = ID_END_OF_FIELDS;
   m_dwFieldSize = 0;
}

CNodedMemoryBuffer::~CNodedMemoryBuffer()
{
}

BOOL CNodedMemoryBuffer::ReadNextFieldHeader()
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

void CNodedMemoryBuffer::SkipField()
{
   ASSERT(IsLoading());
   if(m_iFieldType == ID_FIELD_IS_NODE)
      SkipNode();
   else
   {
      Seek(m_dwFieldSize, SEEK_CURRENT);
   }
}
	
void CNodedMemoryBuffer::SkipNode()
{
   while(ReadNextFieldHeader())
   {
      SkipField(); 
   }
}

void CNodedMemoryBuffer::WriteNodeHeader(int iNodeType)
{
   ASSERT(IsStoring());
   int i = ID_FIELD_IS_NODE;
   WriteInt(i);

   WriteInt(iNodeType);
}

void CNodedMemoryBuffer::WriteEndOfFields()
{
   ASSERT(IsStoring());
   int i = ID_END_OF_FIELDS;
   WriteInt(i);
}
	
/////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////// Write APIz ////////////////////////////////////////////////////////////////

void CNodedMemoryBuffer::WriteField(int iFieldId, void* lpvData, DWORD dwSize)
{
   ASSERT(IsStoring());
   WriteInt(iFieldId);
   WriteDword(dwSize);
   Write(lpvData, dwSize);
}

void CNodedMemoryBuffer::WriteDwordField(int iFieldId, DWORD dwVal)
{
   ASSERT(IsStoring());
   WriteInt(iFieldId);
   WriteDword(sizeof(DWORD));
   WriteDword(dwVal);
}

void CNodedMemoryBuffer::WriteByteField(int iFieldId, BYTE byVal)
{
   ASSERT(IsStoring());
   WriteInt(iFieldId);
   WriteDword(sizeof(BYTE));
   WriteByte(byVal);
}

void CNodedMemoryBuffer::WriteWordField(int iFieldId, WORD wVal)
{
   ASSERT(IsStoring());
   WriteInt(iFieldId);
   WriteDword(sizeof(WORD));
   WriteWord(wVal);
}

void CNodedMemoryBuffer::WriteLongField(int iFieldId, LONG lVal)
{
   ASSERT(IsStoring());
   WriteInt(iFieldId);
   WriteDword(sizeof(LONG));
   WriteLong(lVal);
}

void CNodedMemoryBuffer::WriteIntField(int iFieldId, INT iVal)
{
   ASSERT(IsStoring());
   WriteInt(iFieldId);
   WriteDword(sizeof(INT));
   WriteInt(iVal);
}

void CNodedMemoryBuffer::WriteBoolField(int iFieldId, BOOL bVal)
{
   ASSERT(IsStoring());
   WriteInt(iFieldId);
   WriteDword(sizeof(BOOL));
   WriteBool(bVal);
}

void CNodedMemoryBuffer::WriteStringField(int iFieldId, CString strVal)
{
   ASSERT(IsStoring());
   WriteInt(iFieldId);
   WriteDword((strVal.GetLength()+1)*sizeof(char));
   WriteString(strVal);
}

void CNodedMemoryBuffer::WriteStringField(int iFieldId, LPSTR lpszVal)
{
   ASSERT(IsStoring());
   WriteInt(iFieldId);
   WriteDword((strlen(lpszVal)+1)*sizeof(char));
   WriteString(lpszVal);
}

void CNodedMemoryBuffer::WriteFloatField(int iFieldId, float fVal)
{
   ASSERT(IsStoring());
   WriteInt(iFieldId);
   WriteDword(sizeof(float));
   WriteFloat(fVal);
}

void CNodedMemoryBuffer::WriteDoubleField(int iFieldId, double dVal)
{
   ASSERT(IsStoring());
   WriteInt(iFieldId);
   WriteDword(sizeof(double));
   WriteDouble(dVal);
}

