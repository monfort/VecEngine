// NodedArchive.h: interface for the CNodedArchive class.
//
//////////////////////////////////////////////////////////////////////
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
#if !defined(AFX_NODEDARCHIVE_H__DAD16B0D_94EE_11D3_A47C_00C0CA1261A6__INCLUDED_)
#define AFX_NODEDARCHIVE_H__DAD16B0D_94EE_11D3_A47C_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define ID_FIELD_IS_NODE -1	//Signals that the field is a node
#define ID_END_OF_FIELDS -2	//Signals there are no more fields

class CNodedArchive: public CArchive
{
public:
	CNodedArchive(const CString& strHeader, CFile* pFile, UINT nMode, int nBufSize = 4096, void* lpBuf = NULL );
	virtual ~CNodedArchive();

   BOOL ReadNextFieldHeader();//	Returns true if there is a field and false if there are no more fields
   int GetFieldType(){return m_iFieldType;}
   int GetFieldSize(){return m_iFieldSize;}
   int GetNodeType(){return m_iNodeType;}
   void SkipField();	
   void WriteFieldHeader(int iFieldId, int iSize);	
   void WriteNodeHeader(int iNodeType);
   void WriteEndOfFields();	

private:
   void SkipNode();
   int m_iFieldType;
   int m_iFieldSize;
   int m_iNodeType;
   BOOL m_bNodedFormat;
};

#endif // !defined(AFX_NODEDARCHIVE_H__DAD16B0D_94EE_11D3_A47C_00C0CA1261A6__INCLUDED_)
