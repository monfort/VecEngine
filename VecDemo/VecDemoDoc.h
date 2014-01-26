// VecDemoDoc.h : interface of the CVecDemoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PASDOC_H__A8F04FAB_95A5_11D3_A47C_00C0CA1261A6__INCLUDED_)
#define AFX_PASDOC_H__A8F04FAB_95A5_11D3_A47C_00C0CA1261A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNodedArchive;
class CVecDemoView;
class CBitmap;
class GobalBuffer;
class CVecDemoDoc : public CDocument
{

protected: // create from serialization only
	CVecDemoDoc();
	DECLARE_DYNCREATE(CVecDemoDoc)
   class CFofmatException{};
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVecDemoDoc)
	public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	double GetHeightInInches();
	double GetWidthInInches();
	void SaveToNodedArchive(CNodedArchive &ar);
	void LoadFromNodedArchive(CNodedArchive &ar);
	CVecDemoView* GetVecTstView();
	void SetVecTstView(CVecDemoView* pVecTstView);
	virtual ~CVecDemoDoc();
  
	//****************** background properties
   inline CString GetBGImgPath() {return m_sBgImagePath;}
   inline void SetBGImgPath(CString const &sBgImagePath) {m_sBgImagePath = sBgImagePath;}


   inline BOOL GetIsBGImg() {return m_bBgImage;}//is the background is an image or a solid color
   inline void SetIsBGImg(BOOL bBgImage) {m_bBgImage = bBgImage;}//is the background is an image or a solid color
   inline COLORREF GetSolidCol() {return m_SolidCol;}
   inline void SetSolidCol(COLORREF SolidCol) {m_SolidCol = SolidCol;}

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
   enum FieldIDs{
     Field_WidthInInches = 0,
     Field_HeightInInches,
     Field_PageSize,

      Field_PageBgImagePath,
      Field_BoolBgImage,
      Field_PageSolidCol,
      Field_PageVecDump,
   };

   CVecDemoView* m_pVecTstView;

   double m_dWidthInInches;
   double m_dHeightInInches;
   UINT m_iPageSize;
   
   BOOL m_bBgImage;//is the background is an image or a solid color
   CString m_sBgImagePath;
   COLORREF m_SolidCol;	

	bool FileClose();

// Generated message map functions
protected:
	void ResetProperties();
	//{{AFX_MSG(CVecDemoDoc)
	afx_msg void OnFileClose();
   afx_msg void OnNewPage();
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASDOC_H__A8F04FAB_95A5_11D3_A47C_00C0CA1261A6__INCLUDED_)
