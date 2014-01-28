/*
    * VecEngine - A Win32 vector drawings editing library.
    * Distributed under the LGPL License, see accompanying LICENSE
    * (C) Copyright 2014 Monfort Software Engineering Ltd. http://monfort.co.il
*/ 

#if !defined(MEM_BUFF_EX_H)
#define MEM_BUFF_EX_H

#include "VecTypes.h"

#define SEEK_BEGIN 1
#define SEEK_CURRENT 2

class CMemoryBufferEx : public CObject
{
private:
   void* m_lpvMemory;
   void* m_lpvCurrntPos;
   DWORD m_dwSize;
   DWORD m_dwGrow;
   BOOL m_bIsStoring;
   DWORD m_dwMarkPos;

private:
   void Realloc(DWORD dwSize);
   void CheckIfSizeIsEnoughAndFixIt(DWORD dwSize);

public:
   CMemoryBufferEx(DWORD dwGrow = 1024, DWORD dwInitialSize = DWORD(-1));//constactor for storing
   CMemoryBufferEx(void* dwpvData, DWORD dwSize);//constactor for loading
   ~CMemoryBufferEx();

   inline BOOL IsStoring(){return m_bIsStoring;}
   inline BOOL IsLoading(){return !m_bIsStoring;}
   DWORD CopyData(void* lpvData, DWORD dwSize);

   void Seek(DWORD dwOff, UINT nFrom);
   void Mark();
   void Reset();

//Write APIs
   void Write(void* lpvData, DWORD dwSize);
   void WriteDword(DWORD dwVal);
   void WriteByte(BYTE byVal);
   void WriteWord(WORD wVal);
   void WriteLong(LONG lVal);
   void WriteInt(INT iVal);
   void WriteBool(BOOL bVal);
   void WriteString(CString strVal);
   void WriteString(LPSTR lpszVal);
   void WriteFloat(float fVal);
   void WriteDouble(double dVal);
   void WritePoint(POINT &Val);
   void WriteRect(RECT &Val);
   void WriteDPoint(DPOINT &Val);
   void WriteDRect(DRECT &Val);

//Read APIs
   void Read(void* lpvData, DWORD dwSize);
   DWORD ReadDword();
   BYTE ReadByte();
   WORD ReadWord();
   LONG ReadLong();
   INT ReadInt();
   BOOL ReadBool();
   CString ReadString();
   int ReadString(LPSTR lpszVal, int iSize);//return 0 if size was Enough or the size if not (size includes the null termination char)
   float ReadFloat();
   double ReadDouble();
   void ReadPoint(POINT* pVal);
   void ReadRect(RECT* pVal);
   void ReadDPoint(DPOINT* pVal);
   void ReadDRect(DRECT* pVal);
};

/*
Overview:
This class derives CArchive and helps the programmer to write files with nodes and fields in a chunked file.
The file contains fields, a node is a type of field that contains more fields.
Before every field we save an id of the field and the size of the field.
At the end of every node we save an id indicates that there are no more field in this node.
When the reader does not know a field id he can skip the field. This class can skip it because we save the size of each field and if it's a node we can also skip it by skipping all its fields until the end of the node is found.
All field Ids should be positive (negative values are reserved to the management of the nodes)
Node ids should be defined in one place for each file format.
Field ids can be defined separately for every node.
*/

#define ID_FIELD_IS_NODE -1	//Signals that the field is a node
#define ID_END_OF_FIELDS -2	//Signals there are no more fields

class CNodedMemoryBufferEx : private CMemoryBufferEx
{
public:
   CNodedMemoryBufferEx(DWORD dwGrow = 1024, DWORD dwInitialSize = DWORD(-1));//constartor for storing
   CNodedMemoryBufferEx(void* lpvData, DWORD dwSize);//Constractor for loading
   ~CNodedMemoryBufferEx();
   BOOL ReadNextFieldHeader();//	Returns true if there is a field and false if there are no more fields
   int GetFieldType(){return m_iFieldType;}
   DWORD GetFieldSize(){return m_dwFieldSize;}
   int GetNodeType(){return m_iNodeType;}
   void SkipField();	
   void WriteNodeHeader(int iNodeType);
   void WriteEndOfFields();	

   using CMemoryBufferEx::IsStoring;
   using CMemoryBufferEx::IsLoading;
   using CMemoryBufferEx::CopyData;

//Write APIs
   void WriteField(int iFieldId, void* lpvData, DWORD dwSize);
   void WriteDwordField(int iFieldId, DWORD dwVal);
   void WriteByteField(int iFieldId, BYTE byVal);
   void WriteWordField(int iFieldId, WORD wVal);
   void WriteLongField(int iFieldId, LONG lVal);
   void WriteIntField(int iFieldId, INT iVal);
   void WriteBoolField(int iFieldId, BOOL bVal);
   void WriteStringField(int iFieldId, CString strVal);
   void WriteStringField(int iFieldId, LPSTR lpszVal);
   void WriteFloatField(int iFieldId, float fVal);
   void WriteDoubleField(int iFieldId, double dVal);
   void WritePointField(int iFieldId, POINT &Val);
   void WriteRectField(int iFieldId, RECT &Val);
   void WriteDPointField(int iFieldId, DPOINT &Val);
   void WriteDRectField(int iFieldId, DRECT &Val);

//Read APIs
   using CMemoryBufferEx::Read;
   using CMemoryBufferEx::ReadDword;
   using CMemoryBufferEx::ReadByte;
   using CMemoryBufferEx::ReadWord;
   using CMemoryBufferEx::ReadLong;
   using CMemoryBufferEx::ReadInt;
   using CMemoryBufferEx::ReadBool;
   using CMemoryBufferEx::ReadString;
   using CMemoryBufferEx::ReadFloat;
   using CMemoryBufferEx::ReadDouble;
   using CMemoryBufferEx::ReadPoint;
   using CMemoryBufferEx::ReadRect;
   using CMemoryBufferEx::ReadDPoint;
   using CMemoryBufferEx::ReadDRect;

private:
   void SkipNode();
   int m_iFieldType;
   DWORD m_dwFieldSize;
   int m_iNodeType;
};

#endif //MEM_BUFF_EX_H
