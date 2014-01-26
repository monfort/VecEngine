// VecDemoDoc.cpp : implementation of the CVecDemoDoc class
//
#include "stdafx.h"
#include "VecDemo.h"
#include "VecDemoDoc.h"
#include "VecDemoView.h" 
#include "NodedArchive.h"
#include "NewPagePropDlg.h" 
#include "GobalBuffer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVecDemoDoc

#define VECTOR_FORMAT "Vector Engine Test 4 - 2000"
IMPLEMENT_DYNCREATE(CVecDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CVecDemoDoc, CDocument)
	//{{AFX_MSG_MAP(CVecDemoDoc)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
    ON_COMMAND(ID_FILE_NEW, OnNewPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVecDemoDoc construction/destruction

CVecDemoDoc::CVecDemoDoc()
{
   ResetProperties();
   m_pVecTstView = NULL; 
   SetModifiedFlag(FALSE);     // start off with unmodified
}

CVecDemoDoc::~CVecDemoDoc()
{

}

/////////////////////////////////////////////////////////////////////////////
// CVecDemoDoc diagnostics

#ifdef _DEBUG
void CVecDemoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVecDemoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVecDemoDoc commands

bool CVecDemoDoc::FileClose() 
{
   if(!SaveModified())
      return false;

   SetTitle("Untitled");
   m_strPathName.Empty();  
   SetModifiedFlag(FALSE);     // start off with unmodified

   return true;
}

void CVecDemoDoc::OnFileClose() 
{
	FileClose();
}
///////////////////////////////////////////////////////////////////////////
// set pointer to the APE

void CVecDemoDoc::SetVecTstView(CVecDemoView *pVecTstView)
{
   ASSERT(pVecTstView);
   m_pVecTstView = pVecTstView;
}

///////////////////////////////////////////////////////////////////////////
// return pointer to the VecTstView view
CVecDemoView* CVecDemoDoc::GetVecTstView()
{
   ASSERT(m_pVecTstView);
   return m_pVecTstView;
}
///////////////////////////////////////////////////////////////////////////////
//
// call when new page is created 

void  CVecDemoDoc::OnNewPage()
{
   //open new page dialog
   CNewPagePropDlg newPagePropDlg;

   if(newPagePropDlg.DoModal() != IDOK)
      return;
   if(!FileClose())
      return; 
   ResetProperties();

   //get page properties
   m_dWidthInInches  = newPagePropDlg.m_dPageWidth;
   m_dHeightInInches = newPagePropDlg.m_dPageHeight;
   m_iPageSize = newPagePropDlg.m_iPageSize;
   
   m_pVecTstView->ClearDocument();
   // start on with unmodified
   SetModifiedFlag(TRUE);    
}

///////////////////////////////////////////////////////////////////
//
// The function called by CSingleDocTemplate::OpenDocumentFile
//

BOOL CVecDemoDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (IsModified())
		TRACE0("Warning: OnOpenDocument replaces an unsaved document.\n");

	CFileException fe;
	CFile* pFile = GetFile(lpszPathName,
		CFile::modeRead|CFile::shareDenyWrite, &fe);
	if (pFile == NULL)
	{
		ReportSaveLoadException(lpszPathName, &fe,
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}
   OnFileClose();
	DeleteContents();
	SetModifiedFlag();  // dirty during de-serialize

	CNodedArchive loadArchive(VECTOR_FORMAT, pFile, CArchive::load | CArchive::bNoFlushOnDelete);
	loadArchive.m_pDocument = this;
	loadArchive.m_bForceFlat = FALSE;
	TRY
	{
		CWaitCursor wait;
		if (pFile->GetLength() != 0)
      {
       
         ResetProperties();
         try{
               LoadFromNodedArchive(loadArchive);     // load me
            }
         catch(CFofmatException)
            {
               ReleaseFile(pFile, TRUE);
		         DeleteContents();   // remove failed contents

               AfxMessageBox("Can't open file!\nunknown format.");
		         return FALSE;

            }
      }
		loadArchive.Close();
		ReleaseFile(pFile, FALSE);
	}
	CATCH_ALL(e)
	{
		ReleaseFile(pFile, TRUE);
		DeleteContents();   // remove failed contents

		try
		{
			ReportSaveLoadException(lpszPathName, e,
				FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		}
      catch(CException* pE)
      {
         pE->Delete();
      }
		return FALSE;
	}
	END_CATCH_ALL

	SetModifiedFlag(FALSE);     // start off with unmodified

	return TRUE;
}

////////////////////////////////////////////////////////////////////////
// On Save Document call by CDocument::DoSave

BOOL CVecDemoDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	CFileException fe;
	CFile* pFile = NULL;
	pFile = GetFile(lpszPathName, CFile::modeCreate |
		CFile::modeReadWrite | CFile::shareExclusive, &fe);

	if (pFile == NULL)
	{
		ReportSaveLoadException(lpszPathName, &fe,
			TRUE, AFX_IDP_INVALID_FILENAME);
		return FALSE;
	}

	CNodedArchive saveArchive(VECTOR_FORMAT, pFile, CArchive::store | CArchive::bNoFlushOnDelete);
	saveArchive.m_pDocument = this;
	saveArchive.m_bForceFlat = FALSE;
	TRY
	{
		CWaitCursor wait;
      //call the VecTstView to save his page

		SaveToNodedArchive(saveArchive);

		saveArchive.Close();

		ReleaseFile(pFile, FALSE);
	}
	CATCH_ALL(e)
	{
		ReleaseFile(pFile, TRUE);

		try
		{
			ReportSaveLoadException(lpszPathName, e,
				TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
		}
		 catch(CException* pE)
      {
         pE->Delete();
      }
		return FALSE;
	}
	END_CATCH_ALL

	SetModifiedFlag(FALSE);     // back to unmodified

	return TRUE;        // success

}

///////////////////////////////////////////////////////////////////////////
void CVecDemoDoc::SaveToNodedArchive(CNodedArchive &ar)
{
   //Save the width of a page  
   ar.WriteFieldHeader(Field_WidthInInches, sizeof(double));
   ar<<m_dWidthInInches;

   //Save the height of a page
   ar.WriteFieldHeader(Field_HeightInInches, sizeof(double));
   ar<<m_dHeightInInches;

   //Save the page size (A3,A4 etc...)
   ar.WriteFieldHeader(Field_PageSize, sizeof(UINT));
   ar<<m_iPageSize;
  
   // save BgImagePath
   ar.WriteFieldHeader(Field_PageBgImagePath, m_sBgImagePath.GetLength()*sizeof(TCHAR));
   ar<<m_sBgImagePath;

   //save m_bBgImage that is the flag background is an image or a solid color
   ar.WriteFieldHeader(Field_BoolBgImage,sizeof(BOOL));
   ar<<m_bBgImage;

   //save COLORREF m_SolidCol;
   ar.WriteFieldHeader(Field_PageSolidCol,sizeof(DWORD));
   ar<<(DWORD)m_SolidCol;

   //save the Vector entities Dump 
	CGobalBuffer buff;
	m_pVecTstView->SaveToBuffer(buff);

   void* pVoid = buff.GetBuffer();

   DWORD size =   buff.GetSize();
   ar.WriteFieldHeader(Field_PageVecDump, size);
   
   ar.Write(pVoid, size*sizeof(BYTE));
   
   buff.Unlock();
   ar.WriteEndOfFields();
}

///////////////////////////////////////////////////////////////////////////
//
void CVecDemoDoc::LoadFromNodedArchive(CNodedArchive &ar)
{
   int id, size;
   BOOL bLoad = FALSE;
   while(ar.ReadNextFieldHeader())
   {
      bLoad = TRUE;
      id = ar.GetFieldType();
      size = ar.GetFieldSize();
      switch(id)
      {
         case Field_WidthInInches : 
            ar >> m_dWidthInInches;
            break;   
         case Field_HeightInInches : 
            ar >> m_dHeightInInches;
            break;   
         case Field_PageSize : 
            ar >> m_iPageSize;
            break;  
         case Field_PageBgImagePath : 
            ar >> m_sBgImagePath;
            break;
         case Field_BoolBgImage : 
            ar >>(BOOL) m_bBgImage;
            break;
         case Field_PageSolidCol : 
            ar >>(DWORD) m_SolidCol;
            break;
         case Field_PageVecDump : 
            {
				CGobalBuffer buff;
               void * pVoid =buff.AllocAndLock(size*sizeof(BYTE));

               if(pVoid == NULL)
                  continue;

               ar.Read(pVoid,  size*sizeof(BYTE));
               buff.Unlock();

			   m_pVecTstView->LoadFromBuffer(buff);

               break;
             }
         default: 
            ar.SkipField();
            break;
       }
   }
   if(!bLoad)
      throw CFofmatException();

}

/////////////////////////////////////////////////////////////////////////////
// menu option

//////////////////////////////////////////////////////////////////////////

void CVecDemoDoc::ResetProperties()
{
	//create default landscape A4 document
   m_iPageSize = 1;
   m_dWidthInInches  =  11.6929;
   m_dHeightInInches =  8.2677;
}

double CVecDemoDoc::GetWidthInInches()
{
   return  m_dWidthInInches;
}

double CVecDemoDoc::GetHeightInInches()
{
   return  m_dHeightInInches;
}





