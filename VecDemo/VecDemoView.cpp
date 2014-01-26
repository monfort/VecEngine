// VecDemoView.cpp : implementation of the CVecDemoView class
//

#include "stdafx.h"
#include <math.h>
#include "VecDemo.h"
#include "MainFrm.h"
#include "VecDemoDoc.h"
#include "VecDemoView.h"
#include "LineTypeDlg.h"
#include "TextEditDlg.h"
#include "ShapeFillDlg.h"
#include "RotateFreeDlg.h"
#include "PageBackgroundDlg.h"
#include "PictureSizeDlg.h"
#include "MemBuffEx.h"
#include "MRectTracker.h"
#include "g_routines.h"
#include "GridPropDlg.h"
#include "shapes.h"
#include "ShadowDlg.h"
#include "PicTransparentDlg.h"
#include "GobalBuffer.h"
#include "shapes.h"

#include "ShapesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define UNDO_DEPTH 1
#define MIN_ZOOM_PRESENT 0.15

#define BORDER_OF_PAGE_GAP_SIZE_IN_INCHES 0.125

#define PAGE_SNAPSHOT_SIZE 100
#define PAGE_SNAPSHOT_GAP_SIZE 3
/////////////////////////////////////////////////////////////////////////////
// CVecDemoView

IMPLEMENT_DYNCREATE(CVecDemoView, CParentClass)

BEGIN_MESSAGE_MAP(CVecDemoView, CParentClass)
	//{{AFX_MSG_MAP(CVecDemoView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_WM_ERASEBKGND()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
	ON_COMMAND(ID_EDIT_SELECTALL, OnEditSelectall)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECTALL, OnUpdateEditSelectall)
	ON_COMMAND(ID_PAGE_FITTOWINDOW, OnPageFittowindow)
	ON_COMMAND(ID_PAGE_RESETZOOM, OnPageResetzoom)
	ON_COMMAND(ID_PAGE_ZOOMIN, OnPageZoomin)
	ON_COMMAND(ID_PAGE_ZOOMOUT, OnPageZoomout)
	ON_UPDATE_COMMAND_UI(ID_PAGE_FITTOWINDOW, OnUpdatePageFittowindow)
	ON_UPDATE_COMMAND_UI(ID_PAGE_RESETZOOM, OnUpdatePageResetzoom)
	ON_WM_PAINT()
	ON_COMMAND(ID_PAGE_FITTOWIDTH, OnPageFittowidth)
	ON_UPDATE_COMMAND_UI(ID_PAGE_FITTOWIDTH, OnUpdatePageFittowidth)
	ON_COMMAND(ID_PAGE_FITTOHEIGHT, OnPageFittoheight)
	ON_UPDATE_COMMAND_UI(ID_PAGE_FITTOHEIGHT, OnUpdatePageFittoheight)
	ON_WM_DESTROY()
	ON_COMMAND(ID_ORDER_BRINGTOFRONT, OnOrderBringtofront)
	ON_COMMAND(ID_ORDER_SENDTOBACK, OnOrderSendtoback)
	ON_UPDATE_COMMAND_UI(ID_ORDER_SENDTOBACK, OnUpdateOrderSendtoback)
	ON_UPDATE_COMMAND_UI(ID_ORDER_BRINGTOFRONT, OnUpdateOrderBringtofront)
	ON_COMMAND(ID_PICTURE_FLIP_HORIZONTAL, OnPictureFlipHorizontal)
	ON_COMMAND(ID_PICTURE_FLIP_VERTICAL, OnPictureFlipVertical)
	ON_UPDATE_COMMAND_UI(ID_PICTURE_FLIP_HORIZONTAL, OnUpdatePictureFlip)
	ON_COMMAND(ID_PICTURE_ROTATE_180, OnPictureRotate180)
	ON_UPDATE_COMMAND_UI(ID_PICTURE_ROTATE_180, OnUpdatePictureRotate)
	ON_COMMAND(ID_PICTURE_ROTATE_90CCW, OnPictureRotate90ccw)
	ON_COMMAND(ID_PICTURE_ROTATE_90CW, OnPictureRotate90cw)
	ON_COMMAND(ID_TEXT_FONT, OnTextFont)
	ON_UPDATE_COMMAND_UI(ID_TEXT_FONT, OnUpdateTextFont)
	ON_COMMAND(ID_OBJECT_BACKCOLOR, OnObjectBackcolor)
	ON_COMMAND(ID_OBJECT_FORECOLOR, OnObjectForecolor)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_FORECOLOR, OnUpdateObjectForecolor)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_BACKCOLOR, OnUpdateObjectBackcolor)
	ON_COMMAND(ID_LINE_LINE, OnLineLine)
	ON_UPDATE_COMMAND_UI(ID_LINE_LINE, OnUpdateLineLine)
	ON_COMMAND(ID_TEXT_EDITTEXT, OnTextEdittext)
	ON_UPDATE_COMMAND_UI(ID_TEXT_EDITTEXT, OnUpdateTextEdittext)
	ON_COMMAND(ID_SHAPE_FILL, OnShapeFill)
	ON_UPDATE_COMMAND_UI(ID_SHAPE_FILL, OnUpdateShapeFill)
	ON_COMMAND(ID_PICTURE_ROTATE_FREE, OnRotateFree)
	ON_COMMAND(ID_PAGE_BACKGROUND, OnPageBackground)
	ON_UPDATE_COMMAND_UI(ID_PAGE_BACKGROUND, OnUpdatePageBackground)
	ON_UPDATE_COMMAND_UI(ID_PAGE_ZOOMIN, OnUpdatePageZoomin)
	ON_UPDATE_COMMAND_UI(ID_PAGE_ZOOMOUT, OnUpdatePageZoomout)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_PICTURE_SIZE, OnPictureSize)
	ON_UPDATE_COMMAND_UI(ID_PICTURE_SIZE, OnUpdatePictureSize)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_VIEW_GRID, OnViewGrid)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GRID, OnUpdateViewGrid)
	ON_COMMAND(ID_VIEW_GRID_PROPERTIES, OnViewGridProperties)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GRID_PROPERTIES, OnUpdateViewGridProperties)
	ON_COMMAND(ID_VIEW_GUIDES, OnViewGuides)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GUIDES, OnUpdateViewGuides)
	ON_COMMAND(ID_VIEW_RULLERS, OnViewRullers)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RULLERS, OnUpdateViewRullers)
	ON_COMMAND(ID_VIEW_SNAP2GRID, OnViewSnap2grid)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SNAP2GRID, OnUpdateViewSnap2grid)
	ON_COMMAND(ID_VIEW_SNAP2GUIDES, OnViewSnap2guides)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SNAP2GUIDES, OnUpdateViewSnap2guides)
	ON_COMMAND(ID_ORDER_SENDONEBACK, OnOrderSendoneback)
	ON_UPDATE_COMMAND_UI(ID_ORDER_SENDONEBACK, OnUpdateOrderSendoneback)
	ON_COMMAND(ID_ORDER_SENDONEFORE, OnOrderSendonefore)
	ON_UPDATE_COMMAND_UI(ID_ORDER_SENDONEFORE, OnUpdateOrderSendonefore)
	ON_COMMAND(ID_GROUP_GROUP, OnGroupGroup)
	ON_UPDATE_COMMAND_UI(ID_GROUP_GROUP, OnUpdateGroupGroup)
	ON_COMMAND(ID_GROUP_UNGROUP, OnGroupUngroup)
	ON_UPDATE_COMMAND_UI(ID_GROUP_UNGROUP, OnUpdateGroupUngroup)
	ON_COMMAND(ID_VIEW_LOCK_GUIDES, OnViewLockGuides)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LOCK_GUIDES, OnUpdateViewLockGuides)
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_PICTURE_CROP_ELLIPSE, OnPictureCropEllipse)
	ON_UPDATE_COMMAND_UI(ID_PICTURE_CROP_ELLIPSE, OnUpdatePictureCropEllipse)
	ON_COMMAND(ID_PICTURE_CROP_FREEHAND, OnPictureCropFreehand)
	ON_UPDATE_COMMAND_UI(ID_PICTURE_CROP_FREEHAND, OnUpdatePictureCropFreehand)
	ON_COMMAND(ID_PICTURE_CROP_POLYGON, OnPictureCropPolygon)
	ON_UPDATE_COMMAND_UI(ID_PICTURE_CROP_POLYGON, OnUpdatePictureCropPolygon)
	ON_COMMAND(ID_PICTURE_CROP_RECTANGLE, OnPictureCropRectangle)
	ON_UPDATE_COMMAND_UI(ID_PICTURE_CROP_RECTANGLE, OnUpdatePictureCropRectangle)
	ON_COMMAND(ID_PICTURE_CROP_SHAPE, OnPictureCropShape)
	ON_UPDATE_COMMAND_UI(ID_PICTURE_CROP_SHAPE, OnUpdatePictureCropShape)
	ON_COMMAND(ID_OBJECT_TEXT_ALIGNCENTER, OnObjectTextAligncenter)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_TEXT_ALIGNCENTER, OnUpdateObjectTextAligncenter)
	ON_COMMAND(ID_OBJECT_TEXT_ALIGNLEFT, OnObjectTextAlignleft)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_TEXT_ALIGNLEFT, OnUpdateObjectTextAlignleft)
	ON_COMMAND(ID_OBJECT_TEXT_ALIGNRIGHT, OnObjectTextAlignright)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_TEXT_ALIGNRIGHT, OnUpdateObjectTextAlignright)
	ON_COMMAND(ID_OBJECT_ALIGN_LOCKOBJECT, OnObjectAlignLockobject)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_ALIGN_LOCKOBJECT, OnUpdateObjectAlignLockobject)
	ON_COMMAND(ID_OBJECT_ALIGN_UNLOCK, OnObjectAlignUnlock)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_ALIGN_UNLOCK, OnUpdateObjectAlignUnlock)
	ON_COMMAND(ID_PICMAPPING_REMOVEPHOTO, OnPicmappingRemovephoto)
	ON_UPDATE_COMMAND_UI(ID_PICMAPPING_REMOVEPHOTO, OnUpdatePicmappingRemovephoto)
	ON_COMMAND(ID_PICMAPPING_RESETMAPPINGAREA, OnPicmappingResetmappingarea)
	ON_UPDATE_COMMAND_UI(ID_PICMAPPING_RESETMAPPINGAREA, OnUpdatePicmappingResetmappingarea)
	ON_COMMAND(ID_SHAPE_SHOWPOINTS, OnShapeShowpoints)
	ON_UPDATE_COMMAND_UI(ID_SHAPE_SHOWPOINTS, OnUpdateShapeShowpoints)
	ON_COMMAND(ID_OBJECT_PICTURE_SHADOW, OnObjectPictureShadow)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_PICTURE_SHADOW, OnUpdateObjectPictureShadow)
	ON_UPDATE_COMMAND_UI(ID_PICTURE_FLIP_VERTICAL, OnUpdatePictureFlip)
	ON_UPDATE_COMMAND_UI(ID_PICTURE_ROTATE_90CCW, OnUpdatePictureRotate)
	ON_UPDATE_COMMAND_UI(ID_PICTURE_ROTATE_90CW, OnUpdatePictureRotate)
	ON_UPDATE_COMMAND_UI(ID_PICTURE_ROTATE_FREE, OnUpdatePictureRotate)
	ON_COMMAND(ID_OBJECT_PICTURE_TRANSPARENCY, OnObjectPictureTransparency)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_PICTURE_TRANSPARENCY, OnUpdateObjectPictureTransparency)
	//}}AFX_MSG_MAP

	// Standard printing commands
	//ON_COMMAND(ID_FILE_PRINT, CParentClass::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_DIRECT, CParentClass::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_PREVIEW, CParentClass::OnFilePrintPreview)
    
	ON_UPDATE_COMMAND_UI_RANGE(ID_TOOL_SELECT, ID_TOOL_PANIMAGE, OnUpdateTools)
	ON_COMMAND_RANGE(ID_TOOL_SELECT, ID_TOOL_PANIMAGE, OnToolCheanged)
    ON_UPDATE_COMMAND_UI_RANGE(ID_TOOL_ROTATE, ID_TOOL_SHAPE, OnUpdateTools)
    ON_COMMAND_RANGE(ID_TOOL_ROTATE, ID_TOOL_SHAPE, OnToolCheanged)
    ON_UPDATE_COMMAND_UI_RANGE(ID_TOOL_DRAG_DROP, ID_TOOL_DRAG_DROP, OnUpdateTools)
	ON_COMMAND_RANGE(ID_TOOL_DRAG_DROP, ID_TOOL_DRAG_DROP, OnToolCheanged)
    ON_MESSAGE(WM_RULER_NCRBUTTONDOWN, OnRullerRightClick)
    ON_MESSAGE(WM_RULER_NCLBUTTONDBLCLK, OnRullerDBLClick)
    ON_MESSAGE(WM_VEC_EVENT, OnVecEvent)
    ON_UPDATE_COMMAND_UI_RANGE(ID_OBJECT_ALIGN_LEFT, ID_OBJECT_ALIGN_DISTRIBUTEVERTICALLY, OnUpdateObjectAlign)
	ON_COMMAND_RANGE(ID_OBJECT_ALIGN_LEFT, ID_OBJECT_ALIGN_DISTRIBUTEVERTICALLY, OnObjectAlign)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// View Disabled Macros

#define UPDATE_CMD_UI_VIEW_DISABLED \
   if(IsViewDisabled())\
   {\
      pCmdUI->Enable(FALSE);\
      return;\
   }

#define RETURN_IF_VIEW_DISABLED \
   if(IsViewDisabled())\
      return;

#define ASSERT_VIEW_DISABLED \
   ASSERT(!IsViewDisabled());

#define CHECK_VIEW_DISABLED \
   IsViewDisabled()

#define SET_MODIFIED_FLAG \
   m_VecController.SetModified(TRUE);

#define KILL_CROP_MODE \
   m_VecController.KillCropMode();

/////////////////////////////////////////////////////////////////////////////
// CVecDemoView construction/destruction

CVecDemoView::CVecDemoView():
	m_pBackgroundVec(NULL),
   m_bShowRullers(FALSE)
{
   m_bIsDragFromThisView = FALSE;
   m_LastDragItemsRect.SetRectEmpty();
   m_OrgDragItemsRect.SetRectEmpty();
   m_dVecUnitsPerInch = VEC_UNITS_PER_INCH;
   CPictureSizeDlg::ReadFromRegistry();
   CGridPropDlg::ReadFromRegistry();
   CShadowDlg::ReadFromRegistry();
   m_LastDragRect.SetRectEmpty();
}

CVecDemoView::~CVecDemoView()
{
   CPictureSizeDlg::StoreToRegistry();
   CGridPropDlg::StoreToRegistry();
   CShadowDlg::StoreToRegistry();
}

/******** Page Management ******************************************************/

void CVecDemoView::LoadFromBuffer(CGobalBuffer &buff)
{
   CStatusMessage status("Loading...");
   CWaitCursor wait;//create a wait cursor

   //destroy the old container
   m_VecContainer.RemoveAll();
   m_VecController.m_UndoManager.ClearStack();

   unsigned long uMemSize = buff.GetSize();

   //load the vector entities from the buffer
   CNodedMemoryBufferEx ContainerDumper(buff.GetBuffer(), uMemSize);
   m_VecContainer.LoadFromBuffer(&ContainerDumper);

   // Clean up 
   buff.Unlock();

   CDRect rcContainer;
 
   //calc the size of the background container
	rcContainer = m_VecContainer.GetRect();

   //resize the background container to the new size of the page
   m_BackroundContainer.SetRect(rcContainer);
   m_pBackgroundVec->SetRect(rcContainer);
   SetRect(rcContainer);

   //load the background
   LoadBackground();

   //show rulers
	m_bShowVert = m_bShowHorz = m_bShowRullers;

   //init and reset the display
   m_VecContainer.SetVisible(TRUE);
   m_BackroundContainer.SetVisible(TRUE);
   unsigned int uiZoomMode = GetZoomMode();
   if(uiZoomMode == 5)//reset zoom but save the zoom mode
      if(GetZoomFactor()>1)
         uiZoomMode = 2;
   SetZoomMode(uiZoomMode);	

   //set the window handle that was saved in the initialization
   m_VecContainer.SetWnd(this);
   //DragAcceptFiles(TRUE);

   m_VecContainer.Repaint();

   m_VecController.SetModified(FALSE);
   //m_bPageSnapshotDirty = FALSE;
}

void CVecDemoView::SaveToBuffer(CGobalBuffer &buff)
{
   KILL_CROP_MODE;

   CStatusMessage status("Saving...");
   CWaitCursor wait;//create a wait cursor

   //if there was an old buffer then delete it
   if(buff != NULL)
   {
      buff.Free();
   }

   //dump the vector entities to the buffer
   HGLOBAL hGlobal;
   
   CNodedMemoryBufferEx ContainerDumper;
   m_VecContainer.SaveToBuffer(&ContainerDumper);
   DWORD dwSize = ContainerDumper.CopyData(NULL,0);
   hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwSize);
   
   ContainerDumper.CopyData(GlobalLock(hGlobal), dwSize);
   GlobalUnlock(hGlobal);

   buff = hGlobal; 

   //UpdateCurrentPageSnapshot();
   m_VecController.SetModified(FALSE);
}

void CVecDemoView::RenderDocumentSnapshot(CGobalBuffer &buff,CBitmap& snapshot, int x, int y)
{

 /////////////////load the vector entities container
   CVecContainer vecContainer;

   unsigned long uMemSize = buff.GetSize();

   //load the vector entities from the buffer
   CNodedMemoryBufferEx ContainerDumper(buff.GetBuffer(), uMemSize);
   vecContainer.LoadFromBuffer(&ContainerDumper);

   // Clean up 
   buff.Unlock();

   ///////////////create and initilize the background container
   CDRect rcContainer(0, 0, GetDocument()->GetWidthInInches() * VEC_UNITS_PER_INCH, GetDocument()->GetHeightInInches() * VEC_UNITS_PER_INCH);

   CVecContainer backroundContainer;
   backroundContainer.SetRect(rcContainer);
   
   //load the background
   CVecRectanglePic BackgroundVec;
   BackgroundVec.SetRect(rcContainer);
   DoLoadBackground(GetDocument(), &BackgroundVec);
   backroundContainer.Insert(&BackgroundVec);

   //set the scaler proportions
   CDPoint Scaler;
   Scaler.x = (x - PAGE_SNAPSHOT_GAP_SIZE) / rcContainer.right;
   Scaler.y = (y - PAGE_SNAPSHOT_GAP_SIZE) / rcContainer.bottom;
   //retain porportions between x and y (scale them both with the same amount)
   if(Scaler.x< Scaler.y)
      Scaler.y = Scaler.x;
   else
      Scaler.x = Scaler.y;
   //center it in the image
   CDPoint Offset;
   Offset.x = (x - (rcContainer.right * Scaler.x)) / 2;
   Offset.y = (y - (rcContainer.bottom * Scaler.y)) / 2;

   backroundContainer.SetScaler(Scaler);
   vecContainer.SetScaler(Scaler);
   
   backroundContainer.SetOffset(Offset);
   
   //add a slight gap
   Offset.x += (BORDER_OF_PAGE_GAP_SIZE_IN_INCHES * VEC_UNITS_PER_INCH) * Scaler.x;
   Offset.y += (BORDER_OF_PAGE_GAP_SIZE_IN_INCHES * VEC_UNITS_PER_INCH) * Scaler.y;
   
   vecContainer.SetOffset(Offset);

   /////////////render the page snapshot
   CDC PrimeryDC;
   PrimeryDC.Attach(::GetDC(NULL));
   CDC dc;
   dc.CreateCompatibleDC(&PrimeryDC);
   
   snapshot.CreateCompatibleBitmap(&PrimeryDC,x,y);
   dc.SelectObject(snapshot);

   //draw
   dc.FillSolidRect(0,0,x,y,GetDocument() ->GetSolidCol());
   backroundContainer.DoTransform();
   backroundContainer.Draw(&dc, DF_DONT_DRAW_SELECTION);
   vecContainer.DoTransform();
   vecContainer.Draw(&dc, DF_DONT_DRAW_SELECTION);

   dc.DeleteDC();
   ::ReleaseDC(NULL, PrimeryDC.Detach());
}

void CVecDemoView::RenderCurrentDocumentSnapshot(CBitmap& snapshot, int x, int y)
{
   CVecContainer &vecContainer = m_VecContainer;
   CVecContainer &backroundContainer = m_BackroundContainer;

   //backup scaler and offset
   CDPoint OldBackgScaler = backroundContainer.GetScaler();
   CDPoint OldBackgOffset = backroundContainer.GetOffset();
   CDPoint OldRealScaler = vecContainer.GetScaler();
   CDPoint OldRealgScaler = vecContainer.GetOffset();


   CDRect rcContainer;
   rcContainer = m_VecContainer.GetRect();
   
   //set the scaler proportions
   CDPoint Scaler;
   Scaler.x = (x - PAGE_SNAPSHOT_GAP_SIZE) / rcContainer.right;
   Scaler.y = (y - PAGE_SNAPSHOT_GAP_SIZE) / rcContainer.bottom;
   //retain porportions between x and y (scale them both with the same amount)
   if(Scaler.x < Scaler.y)
      Scaler.y = Scaler.x;
   else
      Scaler.x = Scaler.y;
   //center it in the image
   CDPoint Offset;
   Offset.x = (x - (rcContainer.right * Scaler.x)) / 2;
   Offset.y = (y - (rcContainer.bottom * Scaler.y)) / 2;

   backroundContainer.SetScaler(Scaler);
   vecContainer.SetScaler(Scaler);

   backroundContainer.SetOffset(Offset);

   //add a slight gap
   Offset.x += (BORDER_OF_PAGE_GAP_SIZE_IN_INCHES * VEC_UNITS_PER_INCH) * Scaler.x;
   Offset.y += (BORDER_OF_PAGE_GAP_SIZE_IN_INCHES * VEC_UNITS_PER_INCH) * Scaler.y;

   vecContainer.SetOffset(Offset);

   /////////////render the page snapshot
   CDC PrimeryDC;
   PrimeryDC.Attach(::GetDC(NULL));
   CDC dc;
   dc.CreateCompatibleDC(&PrimeryDC);
   CBitmap bitmap;
   bitmap.CreateCompatibleBitmap(&PrimeryDC,x,y);
   dc.SelectObject(bitmap);

   //draw
   dc.FillSolidRect(0,0,x,y,GetDocument()->GetSolidCol());
   backroundContainer.DoTransform();
   backroundContainer.Draw(&dc, DF_DONT_DRAW_SELECTION);
   vecContainer.DoTransform();
   vecContainer.Draw(&dc, DF_DONT_DRAW_SELECTION);

   dc.DeleteDC();
   ::ReleaseDC(NULL, PrimeryDC.Detach());

   //restore scaler and offset
   backroundContainer.SetScaler(OldBackgScaler);
   backroundContainer.SetOffset(OldBackgOffset);
   vecContainer.SetScaler(OldRealScaler);
   vecContainer.SetOffset(OldRealgScaler);
   backroundContainer.DoTransform();
   vecContainer.DoTransform();
}

void CVecDemoView::ClearDocument()
{
	CString cleanStr;
	GetDocument()->SetBGImgPath(cleanStr);
	GetDocument()->SetIsBGImg(FALSE);
	GetDocument()->SetSolidCol(RGB(255,255,255));

   //destroy the old container
   m_VecContainer.RemoveAll();
   m_VecController.m_UndoManager.ClearStack();

   CDRect rcContainer;
 
   //calc the size of the background container
   rcContainer = CDRect(0, 0, GetDocument()->GetWidthInInches() * VEC_UNITS_PER_INCH, GetDocument()->GetHeightInInches() * VEC_UNITS_PER_INCH);
   
   //resize the background container to the new size of the page
   m_BackroundContainer.SetRect(rcContainer);
   m_pBackgroundVec->SetRect(rcContainer);
   SetRect(rcContainer);

   CDRect rcContainerMinusGap(0,0, rcContainer.right-2*VEC_UNITS_PER_INCH*BORDER_OF_PAGE_GAP_SIZE_IN_INCHES, rcContainer.bottom-2*VEC_UNITS_PER_INCH*BORDER_OF_PAGE_GAP_SIZE_IN_INCHES);
   m_VecContainer.SetRect(rcContainerMinusGap);

   //load the background
   LoadBackground();

   //show rulers
	m_bShowVert = m_bShowHorz = m_bShowRullers;

   //init and reset the display
   m_VecContainer.SetVisible(TRUE);
   m_BackroundContainer.SetVisible(TRUE);
   unsigned int uiZoomMode = GetZoomMode();
   if(uiZoomMode == 5)//reset zoom but save the zoom mode
      if(GetZoomFactor()>1)
         uiZoomMode = 2;
   SetZoomMode(uiZoomMode);	

   //set the window handle that was saved in the initialization
   m_VecContainer.SetWnd(this);
   //DragAcceptFiles(TRUE);

   static bool firstTimeTest = false;
   if(!firstTimeTest)
   {
	   CVecRectangleText* pVec = new CVecRectangleText;
	   CDRect rect(300,300,500,500);
	   pVec->SetRect(rect);
	   pVec->SetText("try dragging bmp files from explorer into this view.");
	   m_VecContainer.Insert(pVec);
	   firstTimeTest = true;
   }


   m_VecContainer.Repaint();

   m_VecController.SetModified(FALSE);
}

void CVecDemoView::RenderArtObjectSnapshot(HGLOBAL hBuffer, CBitmapBaseEx* pBitmap, RECT* rect)
{
   /////////////////load the vector entities container
   CVecContainer Container;
   CPtrList Items;
   CDRect rcContainer = ReadArtObject(hBuffer,Items);
   Container.SetRect(rcContainer);
   while(!Items.IsEmpty())
   {
      CVecBase* pVec = (CVecBase*)Items.RemoveTail();
      Container.Insert(pVec);
   }

   CRect picRect;
   if(rect!=NULL)
      picRect = *rect;
   else
      picRect = CRect(0,0,pBitmap->GetWidth(),pBitmap->GetHeight());

   //set the scaler proportions
   CDPoint Scaler;
   Scaler.x = picRect.Width() / rcContainer.right;
   Scaler.y = picRect.Height() / rcContainer.bottom;
   //retain porportions between x and y (scale them both with the same amount)
   if(Scaler.x< Scaler.y)
      Scaler.y = Scaler.x;
   else
      Scaler.x = Scaler.y;
   //center it in the image
   CDPoint Offset;

   Offset.x = picRect.left + (picRect.Width() - (rcContainer.right * Scaler.x)) / 2;
   Offset.y = picRect.top + (picRect.Height() - (rcContainer.bottom * Scaler.y)) / 2;

   Container.SetScaler(Scaler);
   Container.SetOffset(Offset);
}

void CVecDemoView::PutArtObjectInPage(HGLOBAL &hGlobal)
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

   CPtrList Items;
   CDRect bbox = ReadArtObject(hGlobal, Items);
   CDPoint offset(m_VecContainer.GetRect().CenterPoint());
   offset-=bbox.CenterPoint();
   m_VecContainer.SetSelectedForAll(FALSE);
   m_VecController.m_UndoManager.CreateNewStep();
   while(!Items.IsEmpty())
   {
      CVecBase* pVec = (CVecBase*)Items.RemoveTail();
      m_VecContainer.Insert(pVec);
      bbox = pVec->GetRect();
      bbox.OffsetRect(offset);
      pVec->SetRect(bbox);
      pVec->SetSelected(TRUE);
      m_VecController.m_UndoManager.AddActionToLastStep(pVec, UAT_DELETE_ITEM);
   }

   SET_MODIFIED_FLAG;
   m_VecContainer.Repaint();
}

void CVecDemoView::CreateArtObjectFromSelection(HGLOBAL &hGlobal)
{
   CPtrList SelItems;
   GetSelectedItems(SelItems);
   hGlobal = CreateArtObject(SelItems);
}

/********************************************************************************/

void CVecDemoView::DrawBackground(CDC* pDC)
{
   CVecView::DrawBackground(pDC);
   m_BackroundContainer.DoTransform();

   //draw page shade
   CRect containerRect = GetRectByScreen();
   containerRect.OffsetRect(5,5);
   pDC->FillSolidRect(&containerRect, RGB(0,0,0));

   m_BackroundContainer.Draw(pDC);

   //draw page frame
   containerRect = GetRectByScreen();
   containerRect.InflateRect(1,1);
   CGdiObject *pOldPen = pDC->SelectStockObject(BLACK_PEN);
   CGdiObject *pOldBrush = pDC->SelectStockObject(NULL_BRUSH);
   pDC->Rectangle(&containerRect);
   pDC->SelectObject(pOldPen);
   pDC->SelectObject(pOldBrush);
}

void CVecDemoView::SetContainerOffsetBeforeDraw()
{
   CDPoint Offset = GetOffset();
   CDPoint Scaler = GetScaler();
   m_BackroundContainer.SetOffset(Offset);
   m_BackroundContainer.SetScaler(Scaler);

   Offset.x += (BORDER_OF_PAGE_GAP_SIZE_IN_INCHES * VEC_UNITS_PER_INCH) * Scaler.x;
   Offset.y += (BORDER_OF_PAGE_GAP_SIZE_IN_INCHES * VEC_UNITS_PER_INCH) * Scaler.y;

   m_VecContainer.SetOffset(Offset);
   m_VecContainer.SetScaler(Scaler);
}


BOOL CVecDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
   cs.style|=(WS_CLIPCHILDREN|WS_CLIPSIBLINGS);
	return CParentClass::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CVecDemoView drawing

void CVecDemoView::OnDraw(CDC* pDC)
{
	CVecDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
   CParentClass::OnDraw(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CVecDemoView printing

BOOL CVecDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
   pInfo->SetMaxPage(1);
	return DoPreparePrinting(pInfo);
}

void CVecDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CVecDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CVecDemoView diagnostics

#ifdef _DEBUG
void CVecDemoView::AssertValid() const
{
	CParentClass::AssertValid();
}

void CVecDemoView::Dump(CDumpContext& dc) const
{
	CParentClass::Dump(dc);
}

CVecDemoDoc* CVecDemoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVecDemoDoc)));
	return (CVecDemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVecDemoView message handlers

void CVecDemoView::OnSize(UINT nType, int cx, int cy) 
{
	CParentClass::OnSize(nType, cx, cy);
}

int CVecDemoView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	m_bShowVert = m_bShowHorz = m_bShowRullers && !IsViewDisabled();
	
   if (CParentClass::OnCreate(lpCreateStruct) == -1)
		return -1;

   // Register the COleDropTarget object with the OLE dll's
   BOOL success = m_DropTarget.Register(this);
   if(!success )
   MessageBox("Ole Register Drop Target Failed");  
   //DragAcceptFiles(FALSE);

   if (CreateVecEntity() == -1)
		return -1;


   return 0;
}

int CVecDemoView::CreateVecEntity()
{
   m_BackroundContainer.SetWnd(this);
   CDRect drect(0,0,800,600);
   m_VecContainer.SetRect(drect);
   SetRect(drect);
   SetForeColor(RGB(255,255,255));
   SetBackColor(GetSysColor(COLOR_APPWORKSPACE));
   
   m_VecController.SetSelectedShapeTool(&ShapeArrowUp);

   //create the background stamp vector entity and initialize it
   m_pBackgroundVec = new CVecRectanglePic;
   m_BackroundContainer.Insert(m_pBackgroundVec);

   //disable the display
   m_VecContainer.SetVisible(FALSE);
   m_BackroundContainer.SetVisible(FALSE);

   return 0;
}


void CVecDemoView::LoadBackground()
{
   ASSERT_VIEW_DISABLED;

   DoLoadBackground(GetDocument(),m_pBackgroundVec);
}

void CVecDemoView::DoLoadBackground(CVecDemoDoc* pDoc, CVecRectanglePic* pBackgroundVec)
{
   
	//int err;
   ASSERT(pBackgroundVec!=NULL);
   CDRect rcContainer = pBackgroundVec->GetRect();//get hte size of hte background 


   if(pDoc ->GetIsBGImg()) //load a bitmap
   {
      CBitmapBaseEx bitmap;
      int err = bitmap.LoadBitmapFromFile((LPSTR)(LPCSTR)pDoc->GetBGImgPath());

      if(err==1)//if load was OK
      {
         pBackgroundVec->SetBitmap(&bitmap);//set the bitmap
         pBackgroundVec->SetLineStyle(PS_NULL);
         //bitmapex.Free();
      }
	  
	  bitmap.Free();
   }
   else// use the solid color
   {
      pBackgroundVec->SetBitmap(NULL);//clear the bitmap
      pBackgroundVec->SetBackColor(pDoc ->GetSolidCol());
      pBackgroundVec->SetForeColor(pDoc ->GetSolidCol());
      pBackgroundVec->SetFillMode(OPAQUE);
      pBackgroundVec->SetLineStyle(PS_NULL);
   }

}


void CVecDemoView::OnInitialUpdate() 
{
	CParentClass::OnInitialUpdate();
   GetDocument()->SetVecTstView(this);
}


BOOL CVecDemoView::OnEraseBkgnd(CDC* /*pDC*/) 
{
	// do not erease the background
	return TRUE;//CParentClass::OnEraseBkgnd(pDC);
}


DROPEFFECT CVecDemoView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
   if(IsViewDisabled())
      return DROPEFFECT_NONE;

   if(pDataObject->IsDataAvailable((CLIPFORMAT)m_uiClipboardFormat))
      return OnDragEnterVectors(pDataObject, dwKeyState, point);

   if(pDataObject->IsDataAvailable((CLIPFORMAT)CF_HDROP))
      return OnDragEnterPictures(pDataObject, dwKeyState, point);

   return DROPEFFECT_NONE;
}

void CVecDemoView::OnDragLeave() 
{
   if(!IsViewDisabled())
   {
      if(!m_DradOverItms.IsEmpty())
      {
         CDC* pDC = GetDC();
         POSITION pos = m_DradOverItms.GetHeadPosition();
         while(pos!=NULL)
         {
            CVecBase* pVec = (CVecBase*)m_DradOverItms.GetNext(pos);
            pVec->DrawDragImage(pDC, NULL, &m_LastDragItemsRect, &m_OrgDragItemsRect);
            if(!m_bIsDragFromThisView)
            {
               pVec->Remove();
               delete pVec;
            }
         }
         if(!m_bIsDragFromThisView)
            m_DradOverItms.RemoveAll();
         ReleaseDC(pDC);
         m_LastDragItemsRect.SetRectEmpty();
         m_OrgDragItemsRect.SetRectEmpty();
      }
      else if(!m_LastDragRect.IsRectEmpty())
      {
         CDC* pDC = GetDC();
         CMRectTracker::DrawDragRect(pDC, &m_LastDragRect, CSize(1,1), NULL, CSize(0,0));
         ReleaseDC(pDC);
         m_LastDragRect.SetRectEmpty();
      }
   }
}

DROPEFFECT CVecDemoView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
   if(IsViewDisabled())
      return DROPEFFECT_NONE;

   CRect ContainerRect = m_VecContainer.GetRectByScreen();
   CRect ClientRect;
   GetClientRect(&ClientRect);
   CRect rectIntersect;
   rectIntersect.IntersectRect(&ContainerRect, &ClientRect);
   if(!rectIntersect.PtInRect(point))//check if point is out of screen
   {
      if(!m_DradOverItms.IsEmpty())
      {
         CDC* pDC = GetDC();
         POSITION pos = m_DradOverItms.GetHeadPosition();
         while(pos!=NULL)
         {
            CVecBase* pVec = (CVecBase*)m_DradOverItms.GetNext(pos);
            pVec->DrawDragImage(pDC, NULL, &m_LastDragItemsRect, &m_OrgDragItemsRect);
         }
         ReleaseDC(pDC);
         m_LastDragItemsRect.SetRectEmpty();
      }
      else if(!m_LastDragRect.IsRectEmpty())
      {
         CDC* pDC = GetDC();
         CMRectTracker::DrawDragRect(pDC, &m_LastDragRect, CSize(1,1), NULL, CSize(0,0));
         ReleaseDC(pDC);
         m_LastDragRect.SetRectEmpty();
      }
      return DROPEFFECT_NONE;
   }

   if(pDataObject->IsDataAvailable((CLIPFORMAT)m_uiClipboardFormat))
      return OnDragOverVectors(pDataObject, dwKeyState, point);

   if(pDataObject->IsDataAvailable((CLIPFORMAT)CF_HDROP))
      return OnDragOverPictures(pDataObject, dwKeyState, point);

   return DROPEFFECT_NONE;
}

DROPEFFECT CVecDemoView::OnDropEx( COleDataObject* pDataObject, DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point )
{
   if(IsViewDisabled())
   	return DROPEFFECT_NONE;

   CRect ContainerRect = m_VecContainer.GetRectByScreen();
   CRect ClientRect;
   GetClientRect(&ClientRect);
   CRect rectIntersect;
   rectIntersect.IntersectRect(&ContainerRect, &ClientRect);
   if(!rectIntersect.PtInRect(point))//check if point is out of screen
      return DROPEFFECT_NONE;

   if(pDataObject->IsDataAvailable((CLIPFORMAT)m_uiClipboardFormat))
      return OnDropExVectors( pDataObject, dropDefault, dropList, point );

   if(pDataObject->IsDataAvailable((CLIPFORMAT)CF_HDROP))
      return OnDropExPictures( pDataObject, dropDefault, dropList, point );

   return DROPEFFECT_NONE;
}

DROPEFFECT CVecDemoView::OnDragEnterPictures(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
   m_DragPicSize = CPoint(0,0);
   m_LastDragRect.SetRectEmpty();

   CStatusMessage status("Loading Image...");
   CWaitCursor wait;//create a wait cursor

   HDROP hDropInfo = (HDROP) pDataObject->GetGlobalData(CF_HDROP); 
   if(hDropInfo == NULL)
      return DROPEFFECT_NONE;
	
   int err;
/*	UINT nFiles = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);
   if(nFiles==1)*/
	{
      UINT iFile = 0;
		CBitmap  pBitMap;
      TCHAR szFileName[_MAX_PATH];
		::DragQueryFile(hDropInfo, iFile, szFileName, _MAX_PATH);


      CBitmapBaseEx bitmap;
      err = bitmap.LoadBitmapFromFile(szFileName);
      if(err==1)
      {
         m_DragPicSize.cx = bitmap.GetWidth();
         m_DragPicSize.cy = bitmap.GetHeight();

         CRect rect;

         CDPoint Scaler = m_VecContainer.GetScaler();

         m_DragPicSize.cx = (LONG)(m_DragPicSize.cx*PICTURE_UNITS_PER_SRC_PIXELS*Scaler.x);
         m_DragPicSize.cy = (LONG)(m_DragPicSize.cy*PICTURE_UNITS_PER_SRC_PIXELS*Scaler.y);

         rect.left = point.x - m_DragPicSize.cx/2;
         rect.top = point.y - m_DragPicSize.cy/2;
         rect.right = point.x + (m_DragPicSize.cx-(m_DragPicSize.cx/2));
         rect.bottom = point.y + (m_DragPicSize.cy-(m_DragPicSize.cy/2));

         CDC* pDC = GetDC();
         CMRectTracker::DrawDragRect(pDC, &rect, CSize(1,1), NULL, CSize(0,0));
         ReleaseDC(pDC);

         m_LastDragRect = rect;

      }
      bitmap.Free();
	}

   GlobalFree(hDropInfo);

   return DROPEFFECT_COPY;//CParentClass::OnDragEnter(pDataObject, dwKeyState, point);
}

DROPEFFECT CVecDemoView::OnDragOverPictures(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{

   CDPoint dpoint(point);
   CPtrList selItems;
   m_VecContainer.GetSelectedItems(selItems);
   CVecBitmapBase* pVecBitmap = NULL;
   while(!selItems.IsEmpty())
   {
      CVecBase* pVec = (CVecBase*)selItems.RemoveHead();
      if(pVec->HasPicture())
      {
         int iHit = pVec->HitTest(dpoint);
         if(iHit==HT_INSIDE_MAPPING)
         {
            pVecBitmap = dynamic_cast<CVecBitmapBase*>(pVec);
            break;
         }
      }

   }

   if(pVecBitmap!=NULL)
   {
      CDC* pDC = GetDC();
      CMRectTracker::DrawDragRect(pDC, &m_LastDragRect, CSize(1,1), NULL, CSize(0,0));
      ReleaseDC(pDC);
      m_LastDragRect.SetRectEmpty();
   }
   else 
   {
      CRect rect;
      rect.left = point.x - m_DragPicSize.cx/2;
      rect.top = point.y - m_DragPicSize.cy/2;
      rect.right = point.x + (m_DragPicSize.cx-(m_DragPicSize.cx/2));
      rect.bottom = point.y + (m_DragPicSize.cy-(m_DragPicSize.cy/2));

      if(rect!=m_LastDragRect)
      {
         CDC* pDC = GetDC();
         CMRectTracker::DrawDragRect(pDC, &rect, CSize(1,1), &m_LastDragRect, CSize(1,1));
         ReleaseDC(pDC);
      }

      m_LastDragRect = rect;
   }
   
	return DROPEFFECT_COPY;//CParentClass::OnDragOver(pDataObject, dwKeyState, point);
}

DROPEFFECT CVecDemoView::OnDropExPictures( COleDataObject* pDataObject, DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point )
{
   HDROP hDropInfo = (HDROP) pDataObject->GetGlobalData(CF_HDROP); 
   if(hDropInfo == NULL)
      return DROPEFFECT_NONE;
	

   CStatusMessage status("Loading Image(s)...");
   CWaitCursor wait;//create a wait cursor

   CVecBitmapBase* pVecBitmap = NULL;
   //if(DragQueryPoint(hDropInfo, &point))
   {
      CDPoint dpoint(point);
      CPtrList selItems;
      m_VecContainer.GetSelectedItems(selItems);
      while(!selItems.IsEmpty())
      {
         CVecBase* pVec = (CVecBase*)selItems.RemoveHead();
         if(pVec->HasPicture())
         {
            int iHit = pVec->HitTest(dpoint);
            if(iHit==HT_INSIDE_MAPPING)
            {
               pVecBitmap = dynamic_cast<CVecBitmapBase*>(pVec);
               break;
            }
         }
      }
   }

   int err;

   BOOL bRepaint = FALSE;
   if(pVecBitmap==NULL)
   {
      m_VecContainer.SetSelectedForAll(FALSE);//select nothing
      bRepaint = TRUE;
   }

   ((CFrameWnd*)AfxGetMainWnd())->SetActiveView(this);      // activate us first !
   m_VecController.m_UndoManager.CreateNewStep();
	UINT nFiles = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);
   CDRect rect;
   rect.SetRectEmpty();
	for (UINT iFile = 0; iFile < nFiles; iFile++)
	{
		CBitmap  pBitMap;
      TCHAR szFileName[_MAX_PATH];
		::DragQueryFile(hDropInfo, iFile, szFileName, _MAX_PATH);


      CBitmapBaseEx bitmap;
      err = bitmap.LoadBitmapFromFile(szFileName);
      if(err!=1)
         continue;

      if(pVecBitmap)
      {
         m_VecController.m_UndoManager.AddActionToLastStep(dynamic_cast<CVecBase*>(pVecBitmap), UAT_RESTORE_OB_FROM_DUMP);
         pVecBitmap->SetBitmap(&bitmap);
         (dynamic_cast<CVecBase*>(pVecBitmap))->SetLineStyle(PS_NULL);
         bitmap.Free();
         bRepaint = TRUE;
         break;
      }
      else
      {
         int x = bitmap.GetWidth();
         int y = bitmap.GetHeight();

         CVecRectanglePic* pDragDropStamp = new CVecRectanglePic();
         pDragDropStamp->SetBitmap(&bitmap);
         pDragDropStamp->SetLineStyle(PS_NULL);
         //pDragDropStamp->SetFillMode(TRANSPARENT);
         bitmap.Free();

         if(rect.IsRectEmpty())
         {
            CDPoint apoint(point);
            m_VecContainer.ScreenToLocal(&apoint);
            rect.left = apoint.x - x*PICTURE_UNITS_PER_SRC_PIXELS/2;
            rect.top = apoint.y - y*PICTURE_UNITS_PER_SRC_PIXELS/2;
            rect.right = apoint.x + x*PICTURE_UNITS_PER_SRC_PIXELS/2;
            rect.bottom = apoint.y + y*PICTURE_UNITS_PER_SRC_PIXELS/2;
         }
         else
         {
            rect.left += VEC_UNITS_PER_INCH/4;
            rect.top += VEC_UNITS_PER_INCH/4;
            rect.right = rect.left + x*PICTURE_UNITS_PER_SRC_PIXELS;
            rect.bottom = rect.top + y*PICTURE_UNITS_PER_SRC_PIXELS;
         }

         pDragDropStamp->SetRect(rect);
         m_VecContainer.Insert(pDragDropStamp);
         pDragDropStamp->SetSelected(TRUE);
         m_VecController.m_UndoManager.AddActionToLastStep(pDragDropStamp, UAT_DELETE_ITEM);

         pDragDropStamp = NULL;
         bRepaint = TRUE;
      }
   }	
   if(bRepaint)
   {
      m_VecContainer.Repaint();
      SET_MODIFIED_FLAG;
      //UpdateCurrentPageSnapshot();
   }

   GlobalFree(hDropInfo);

	return DROPEFFECT_COPY;//OnDropEx( pDataObject, dropDefault, dropList, point );
}

//****************************************************************************************************
//****************************************************************************************************
//****************************************************************************************************
DROPEFFECT CVecDemoView::OnDragEnterVectors(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
   m_DragPicSize = CPoint(0,0);
   m_LastDragRect.SetRectEmpty();

   if(!m_bIsDragFromThisView)
   {
      CStatusMessage status("Loading Objects...");
      CWaitCursor wait;//create a wait cursor

      HGLOBAL hGlobal = pDataObject->GetGlobalData((CLIPFORMAT)m_uiClipboardFormat); 
      if(hGlobal == NULL)
         return DROPEFFECT_NONE;
	   
      m_DradOverItms.RemoveAll();
      m_OrgDragItemsRect = ReadArtObject(hGlobal, m_DradOverItms);
      if(m_DradOverItms.GetCount()>1)
      {
         CVecContainer* pVecContainer = new CVecContainer;
         while(!m_DradOverItms.IsEmpty())
         {
            CVecBase* pVec = (CVecBase*)m_DradOverItms.RemoveTail();
            pVecContainer->Insert(pVec);
         }
         pVecContainer->SetRect(m_OrgDragItemsRect);
         m_DradOverItms.AddHead(pVecContainer);
      }

      POSITION pos = m_DradOverItms.GetTailPosition();
      while(pos!=NULL)
      {
         CVecBase* pVec = (CVecBase*)m_DradOverItms.GetPrev(pos);
         m_VecContainer.Insert(pVec);
         pVec->DoTransform();
      }
      GlobalFree(hGlobal);
   }
   else
   {
      //get the original bounding rect of the items
      m_OrgDragItemsRect.SetRectEmpty();
      POSITION pos = m_DradOverItms.GetHeadPosition();
      while(pos != NULL)
      {
         CVecBase* pVec = (CVecBase*)m_DradOverItms.GetNext(pos);
         if(m_OrgDragItemsRect.IsRectEmpty())
         {
            m_OrgDragItemsRect = pVec->GetRect();
            m_OrgDragItemsRect.NormalizeRect();
         }
         else
         {
            CDRect rect = m_OrgDragItemsRect;
            CDRect VecRect = pVec->GetRect();
            VecRect.NormalizeRect();
            m_OrgDragItemsRect.UnionRect(&rect, &VecRect);
         }
      }
   }
   m_VecContainer.LocalToScreen(&m_OrgDragItemsRect);

   if(m_bIsDragFromThisView)
   {//center items around current point and not the center point
      m_OrgDragItemsRect-=m_OrgDragItemsRect.CenterPoint();
      m_OrgDragItemsRect+=CDPoint(m_ptDragFromPoint);
   }

   CDRect bbox(m_OrgDragItemsRect);
   bbox-=m_OrgDragItemsRect.CenterPoint();
   bbox+=CDPoint(point);

   CDC* pDC = GetDC();
   POSITION pos = m_DradOverItms.GetHeadPosition();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)m_DradOverItms.GetNext(pos);
      pVec->DrawDragImage(pDC, &bbox, NULL, &m_OrgDragItemsRect);
   }
   ReleaseDC(pDC);

   m_LastDragItemsRect = bbox;

   return m_bIsDragFromThisView ? DROPEFFECT_MOVE : DROPEFFECT_COPY;//CParentClass::OnDragEnter(pDataObject, dwKeyState, point);
}

DROPEFFECT CVecDemoView::OnDragOverVectors(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{

   CDRect bbox(m_OrgDragItemsRect);
   bbox-=m_OrgDragItemsRect.CenterPoint();
   bbox+=CDPoint(point);

   CDC* pDC = GetDC();
   POSITION pos = m_DradOverItms.GetHeadPosition();
   while(pos!=NULL && bbox!=m_LastDragItemsRect)
   {
      CVecBase* pVec = (CVecBase*)m_DradOverItms.GetNext(pos);
      pVec->DrawDragImage(pDC, &bbox, &m_LastDragItemsRect, &m_OrgDragItemsRect);
   }
   ReleaseDC(pDC);

   m_LastDragItemsRect = bbox;
   
	return m_bIsDragFromThisView ? DROPEFFECT_MOVE : DROPEFFECT_COPY;//CParentClass::OnDragOver(pDataObject, dwKeyState, point);
}

DROPEFFECT CVecDemoView::OnDropExVectors( COleDataObject* pDataObject, DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point )
{
   HGLOBAL hGlobal = pDataObject->GetGlobalData((CLIPFORMAT)m_uiClipboardFormat); 
   if(hGlobal == NULL)
      return DROPEFFECT_NONE;

/*   if(!m_bIsDragFromThisView)
   {*/
      m_VecContainer.SetSelectedForAll(FALSE);//select nothing

      CDRect bbox(m_OrgDragItemsRect);
      bbox-=m_OrgDragItemsRect.CenterPoint();
      bbox+=CDPoint(point);

      CDC* pDC = GetDC();
      POSITION pos = m_DradOverItms.GetHeadPosition();
      while(pos!=NULL)
      {
         CVecBase* pVec = (CVecBase*)m_DradOverItms.GetNext(pos);
         pVec->DrawDragImage(pDC, NULL, &m_LastDragItemsRect, &m_OrgDragItemsRect);
      }
      ReleaseDC(pDC);

      m_VecController.m_UndoManager.CreateNewStep();
      while(!m_DradOverItms.IsEmpty())
      {
         CVecBase* pVec = (CVecBase*)m_DradOverItms.RemoveHead();
         if(m_bIsDragFromThisView)
            m_VecController.m_UndoManager.AddActionToLastStep(pVec, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
         pVec->SetRectScaleAndOffsetByScreen(&bbox, &m_OrgDragItemsRect);
         pVec->SetSelected(TRUE);
         if(!m_bIsDragFromThisView)
            m_VecController.m_UndoManager.AddActionToLastStep(pVec, UAT_DELETE_ITEM);
      }

      m_LastDragItemsRect.SetRectEmpty();
      m_OrgDragItemsRect.SetRectEmpty();

   GlobalFree(hGlobal);
   m_VecContainer.Repaint();
   SET_MODIFIED_FLAG;
//   UpdateCurrentPageSnapshot();
	return m_bIsDragFromThisView ? DROPEFFECT_NONE : DROPEFFECT_COPY;//OnDropEx( pDataObject, dropDefault, dropList, point );
}

//convert from lead vectory entity type to our general types
enVecTypes CVecDemoView::GetVecEntityType(unsigned int ui)
{
   switch(ui)
   {
   case VEC_RECTANGLE_TEXT:
   case VEC_POLYGON_TEXT:
   case VEC_ELLIPSE_TEXT:
      return VECENT_TEXT;
   case VEC_POLYGON_PIC:
   case VEC_RECTANGLE_PIC:
   case VEC_ELLIPSE_PIC:
      return VECENT_PIC;
   case VEC_LINE:
   case VEC_POLYLINE:
      return VECENT_LINE;
   case VEC_ELLIPSE:
   case VEC_RECTANGLE:
   case VEC_POLYGON:
      return VECENT_SHAPE;
   case VEC_CONTAINER:
   case VEC_FRAME_CONTAINER:
      return VECENT_GROUP;
   default:
      return VECENT_NONE;
   }
}

//get the type of selected item(s)
enVecTypes CVecDemoView::GetSelectedType()
{
   //if we are in zoom_rect or pan_image tools then ignore selection
   if(m_VecController.GetSelectedTool() == TOOL_ZOOM || m_VecController.GetSelectedTool() == TOOL_PAN)
      return VECENT_NONE;

   enVecTypes enType = VECENT_NONE;
   CPtrList SelItems;
   m_VecContainer.GetSelectedItems(SelItems);
   POSITION pos = SelItems.GetHeadPosition();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);;
      enVecTypes enCurType;

      //check what type of object is it
      enCurType = GetVecEntityType(pVec->GetType());
      ASSERT(enCurType != VECENT_NONE);
      ASSERT(enCurType != VECENT_MORE_THEN_ONE);

      //if there was nothing selected then this is the selected
      if(enType == VECENT_NONE)
         enType = enCurType;
      else if(enType != enCurType)//if there was something else then its more then one
      {
         enType = VECENT_MORE_THEN_ONE;
         break;
      }

   }

   return enType;
}

void CVecDemoView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/) 
{
	// TODO: Add your specialized code here and/or call the base class
		
}

BOOL CVecDemoView::OnStartDragDrop(POINT point)
{
   CPtrList SelItems;
   if(GetSelectedItems(SelItems, TRUE)<1)
      return FALSE;

   CVecContainer::ArrangeItemsForTrack(SelItems);

   COleDataSource* pOleDataSrc;
   {//display wait curssor in this {..}
      CWaitCursor wait;
      CStatusMessage status("Preparing for Drag and Drop operation...");
      pOleDataSrc = BuildOleDataSource(SelItems);
   }
   m_ptDragFromPoint = point;
   m_bIsDragFromThisView = TRUE;
   m_DradOverItms.RemoveAll();
   m_DradOverItms.AddHead(&SelItems);
   DROPEFFECT resoult = pOleDataSrc->DoDragDrop(DROPEFFECT_COPY|DROPEFFECT_MOVE | DROPEFFECT_SCROLL);  
   m_bIsDragFromThisView = FALSE;
   pOleDataSrc->Empty();
   delete pOleDataSrc;
   if(resoult==DROPEFFECT_MOVE)
      DoDelete(SelItems);
   return (resoult != DROPEFFECT_NONE);
}

void CVecDemoView::OnContextMenu(CWnd* /*pWnd*/, CPoint point) 
{
   RETURN_IF_VIEW_DISABLED;

   int iMenu = SUBMENU_NONE;
   
   CPtrList SelItems;
   UINT uiCount = GetSelectedItems(SelItems);
   if(uiCount==1)
   {
      POINT p = point;
      ScreenToClient(&p);
      CDPoint dpoint(p);
      CVecBase* pVec = (CVecBase*)SelItems.GetHead();
      if(pVec!=NULL)
      {
         HitTestResoult enHit = pVec->HitTest(dpoint);
         if(enHit == HT_ON_MAPPING_POINT || enHit == HT_INSIDE_MAPPING)
         {
            if(pVec->HasPicture())
               iMenu = OM_SUB_PIC_MAPPING;
            else if(pVec->HasText())
               iMenu = OM_SUB_TEXT_MAPPING;
         }
      }
   }


   if(iMenu == SUBMENU_NONE) 
   {
      switch(GetSelectedType())
      {
      case VECENT_PIC:
         iMenu = OM_SUB_PIC;
         break;
      case VECENT_TEXT:
         iMenu = OM_SUB_TEXT;
         break;
      case VECENT_SHAPE:
         iMenu = OM_SUB_SHAPE;
         break;
      case VECENT_LINE:
         iMenu = OM_SUB_LINE;
         break;
      case VECENT_MORE_THEN_ONE:
      case VECENT_GROUP:
         iMenu = OM_SUB_GENERAL;
         break;
      case VECENT_NONE:
         iMenu = OM_SUB_BACKGROUND;
         break;
      default:
         ASSERT(FALSE);
         break;
      }
   }

   if(point.x == -1 && point.y == -1)//if this context menu was launched form the keyboard
   {
      POINT p = point;
      ClientToScreen(&p);
      point = p;
   }

   ((CMainFrame*)AfxGetMainWnd())->OpenContextMenu(point, IDR_ITEM_MENU, iMenu);
}


BOOL CVecDemoView::CheckIfSelected(enVecTypes exType, BOOL bExcludeLockObjects) 
{
   ASSERT_VIEW_DISABLED;

   CPtrList SelItems;
   m_VecContainer.GetSelectedItems(SelItems);
   POSITION pos = SelItems.GetHeadPosition();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);
      if(bExcludeLockObjects)
         if(pVec->IsLocked())
            continue;
      enVecTypes enCurType = GetVecEntityType(pVec->GetType());
      if(enCurType == exType)
      {
         return TRUE;
      }
   }
   return FALSE;
}


void CVecDemoView::OnEditCut() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

   OnEditCopy();
   OnEditDelete();

   SET_MODIFIED_FLAG;
}

void CVecDemoView::OnEditCopy() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

   CPtrList SelItems;
   GetSelectedItems(SelItems, TRUE);

   CVecContainer::ArrangeItemsForTrack(SelItems);

   if(SelItems.IsEmpty()) 
      return;

   CWaitCursor wait;
   COleDataSource* pSource = BuildOleDataSource(SelItems);
   if (pSource)
   {
        pSource->SetClipboard(); // OLE deletes data source
   }
}

void CVecDemoView::OnEditPaste() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

   COleDataObject dataObject;
   VERIFY(dataObject.AttachClipboard());
   HGLOBAL hGlobal = (HDROP) dataObject.GetGlobalData((WORD)m_uiClipboardFormat); 
   if(hGlobal!=NULL)
   {
      CWaitCursor wait;
      PutArtObjectInPage(hGlobal);
      GlobalFree(hGlobal);
   }

}

void CVecDemoView::OnEditUndo() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

	m_VecController.m_UndoManager.PerformUndo(&m_VecContainer);
   m_VecContainer.Repaint();

   SET_MODIFIED_FLAG;
}

void CVecDemoView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;
	
   pCmdUI->Enable(m_VecController.m_UndoManager.IsUndoAvailable());
}

void CVecDemoView::OnEditRedo() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

	m_VecController.m_UndoManager.PerformRedo(&m_VecContainer);
   m_VecContainer.Repaint();

   SET_MODIFIED_FLAG;
}

void CVecDemoView::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;
	
   pCmdUI->Enable(m_VecController.m_UndoManager.IsRedoAvailable());
}

void CVecDemoView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

   COleDataObject dataObject;
   VERIFY(dataObject.AttachClipboard());
   pCmdUI->Enable(dataObject.IsDataAvailable((WORD)m_uiClipboardFormat));
}

void CVecDemoView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

   pCmdUI->Enable(GetSelectedItemsCount() > 0);
}

void CVecDemoView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

   pCmdUI->Enable(GetSelectedItemsCount(TRUE) > 0);
}

void CVecDemoView::DoDelete(CPtrList &Items)
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

   CVecContainer::ArrangeItemsForTrack(Items);

   StoreUndoStepItemsZOrder(Items);

   while(!Items.IsEmpty())
   {
      CVecBase* pVec = ((CVecBase*)Items.RemoveHead());
      m_VecController.m_UndoManager.AddActionToLastStep(pVec, UAT_ADD_ITEM);
      pVec->Remove();
      delete pVec;
   }
   m_VecContainer.Repaint();

   SET_MODIFIED_FLAG;
}

void CVecDemoView::OnEditDelete() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

   CPtrList SelItems;
   GetSelectedItems(SelItems, TRUE);
   if(SelItems.IsEmpty())
      return;

   DoDelete(SelItems);
}

void CVecDemoView::OnUpdateEditDelete(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

   pCmdUI->Enable(GetSelectedItemsCount(TRUE) > 0);
}

void CVecDemoView::OnEditSelectall() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

   m_VecContainer.SetSelectedForAll(TRUE);
   m_VecContainer.Repaint();

   SET_MODIFIED_FLAG;
}


void CVecDemoView::OnUpdateEditSelectall(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

}

void CVecDemoView::OnToolCheanged(UINT nID) 
{
   ASSERT_VIEW_DISABLED;

   UINT uiTool = TOOL_SELECT;
   switch(nID)
   {
   case ID_TOOL_DRAG_DROP:
         uiTool = TOOL_DRAG_DROP;
         break;
   case ID_TOOL_SELECT:
         uiTool = TOOL_SELECT;
         break;
   case ID_TOOL_ZOOM_RECT:
         uiTool = TOOL_ZOOM;
         break;
   case ID_TOOL_ROTATE:
         uiTool = TOOL_ROTATE;
         break;
   case ID_TOOL_PANIMAGE:
         uiTool = TOOL_PAN;
         break;
   case ID_TOOL_RECT:
         uiTool = TOOL_RECTANGLE;
         break;
   case ID_TOOL_ELLIPSE:
         uiTool = TOOL_ELLIPSE;
         break;
   case ID_TOOL_LINE:
         uiTool = TOOL_LINE;
         break;
   case ID_TOOL_POLYLINE:
         uiTool = TOOL_POLYLINE;
         break;
   case ID_TOOL_POLYGON:
         uiTool = TOOL_POLYGON;
         break;
   case ID_TOOL_FREEHAND:
         uiTool = TOOL_FREEHAND;
         break;
   case ID_TOOL_FREEHAND_CLOSED:
         uiTool = TOOL_FREEHAND_CLOSED;
         break;
   case ID_TOOL_TEXT:
         uiTool = TOOL_TEXT;
         break;
   case ID_TOOL_SHAPE:
	     if(PopupShapesMenu())
			uiTool = TOOL_SHAPE;
         break;
   default:
      ASSERT(FALSE);
   }
   m_VecController.SetSelectedTool(uiTool);
}

void CVecDemoView::OnUpdateTools(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

   UINT uiSelectedTool = m_VecController.GetSelectedTool();
   switch(pCmdUI->m_nID)
   {
   case ID_TOOL_SELECT:
         pCmdUI->SetCheck(uiSelectedTool==TOOL_SELECT);
         break;
   case ID_TOOL_ZOOM_RECT:
         pCmdUI->SetCheck(uiSelectedTool==TOOL_ZOOM);
         break;
   case ID_TOOL_ROTATE:
         pCmdUI->SetCheck(uiSelectedTool==TOOL_ROTATE);
         break;
   case ID_TOOL_PANIMAGE:
         pCmdUI->SetCheck(uiSelectedTool==TOOL_PAN);
         break;
   case ID_TOOL_DRAG_DROP:
         pCmdUI->SetCheck(uiSelectedTool==TOOL_DRAG_DROP);
         break;
   case ID_TOOL_RECT:
         pCmdUI->SetCheck(uiSelectedTool==TOOL_RECTANGLE);
         break;
   case ID_TOOL_ELLIPSE:
         pCmdUI->SetCheck(uiSelectedTool==TOOL_ELLIPSE);
         break;
   case ID_TOOL_LINE:
         pCmdUI->SetCheck(uiSelectedTool==TOOL_LINE);
         break;
   case ID_TOOL_POLYLINE:
         pCmdUI->SetCheck(uiSelectedTool==TOOL_POLYLINE);
         break;
   case ID_TOOL_POLYGON:
         pCmdUI->SetCheck(uiSelectedTool==TOOL_POLYGON);
         break;
   case ID_TOOL_FREEHAND:
         pCmdUI->SetCheck(uiSelectedTool==TOOL_FREEHAND);
         break;
   case ID_TOOL_FREEHAND_CLOSED:
         pCmdUI->SetCheck(uiSelectedTool==TOOL_FREEHAND_CLOSED);
         break;
   case ID_TOOL_TEXT:
         pCmdUI->SetCheck(uiSelectedTool==TOOL_TEXT);
         break;
   case ID_TOOL_SHAPE:
         pCmdUI->SetCheck(uiSelectedTool==TOOL_SHAPE);
         break;
   default:
      ASSERT(FALSE);
   }
}


void CVecDemoView::OnPageFittowindow() 
{
   ASSERT_VIEW_DISABLED;

   SetZoomMode(ZM_FIT2WINDOW);
}

void CVecDemoView::OnPageResetzoom() 
{
   ASSERT_VIEW_DISABLED;
   
   SetZoomMode(ZM_NORMAL);
}

void CVecDemoView::OnPageZoomin() 
{
   ASSERT_VIEW_DISABLED;

   SetZoomFactor((GetZoomFactor()*1.1));	
}

void CVecDemoView::OnPageZoomout() 
{
   ASSERT_VIEW_DISABLED;

   double dZoom = GetZoomFactor();
   if(dZoom > MIN_ZOOM_PRESENT)
      SetZoomFactor((dZoom/1.1));	
}

void CVecDemoView::OnUpdatePageZoomin(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

}

void CVecDemoView::OnUpdatePageZoomout(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

}

void CVecDemoView::OnUpdatePageFittowindow(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

   pCmdUI->SetCheck((GetZoomMode()==ZM_FIT2WINDOW));
}

void CVecDemoView::OnUpdatePageResetzoom(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

   pCmdUI->Enable(!(GetZoomMode()==ZM_NORMAL));
}

void CVecDemoView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CView::OnPaint() for painting messages
   OnDraw(&dc);
}


void CVecDemoView::OnPageFittowidth() 
{
   ASSERT_VIEW_DISABLED;

   SetZoomMode(ZM_FIT2WIDTH);
}

void CVecDemoView::OnUpdatePageFittowidth(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

   pCmdUI->SetCheck((GetZoomMode()==ZM_FIT2WIDTH));
}

void CVecDemoView::OnPageFittoheight() 
{
   ASSERT_VIEW_DISABLED;
   SetZoomMode(ZM_FIT2HEIGHT);
}

void CVecDemoView::OnUpdatePageFittoheight(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;
   pCmdUI->SetCheck((GetZoomMode()==ZM_FIT2HEIGHT));
}


void CVecDemoView::OnDestroy() 
{
	CParentClass::OnDestroy();
	
   //unregister the ole drop target
   m_DropTarget.Revoke();

   delete m_pBackgroundVec;
}

void CVecDemoView::StoreUndoStepItemsZOrder(CPtrList& Items)
{
   CPtrList ParentsList;
   POSITION pos = Items.GetHeadPosition();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)Items.GetNext(pos);
      if(ParentsList.Find(pVec->GetParent())==NULL)
         ParentsList.AddHead(pVec->GetParent());
   }
   ASSERT(ParentsList.GetCount()!=0);

   pos = ParentsList.GetHeadPosition();
   m_VecController.m_UndoManager.CreateNewStep();
   while(pos!=NULL)
   {
      m_VecController.m_UndoManager.AddActionToLastStep((CVecBase*)ParentsList.GetNext(pos), UAT_RESTORE_ITEMS_ORDER);
   }
}

void CVecDemoView::OnOrderBringtofront() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

   CPtrList SelItems;
   GetSelectedItems(SelItems, TRUE);
   StoreUndoStepItemsZOrder(SelItems);
   while(!SelItems.IsEmpty())
      ((CVecBase*)SelItems.RemoveTail())->BringToFront();
   m_VecContainer.Repaint();

   SET_MODIFIED_FLAG;
}

void CVecDemoView::OnOrderSendtoback() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

   CPtrList SelItems;
   GetSelectedItems(SelItems, TRUE);
   StoreUndoStepItemsZOrder(SelItems);
   while(!SelItems.IsEmpty())
      ((CVecBase*)SelItems.RemoveHead())->SendToBack();
   m_VecContainer.Repaint();

   SET_MODIFIED_FLAG;
}

void CVecDemoView::OnUpdateOrderSendtoback(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

   pCmdUI->Enable(GetSelectedItemsCount(TRUE) > 0);
}

void CVecDemoView::OnUpdateOrderBringtofront(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

   pCmdUI->Enable(GetSelectedItemsCount(TRUE) > 0);
}

void CVecDemoView::OnOrderSendoneback() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

   CPtrList SelItems;
   GetSelectedItems(SelItems, TRUE);
   StoreUndoStepItemsZOrder(SelItems);
   CVecBase* pLast = (CVecBase*)SelItems.GetTail();
   CPtrList Items;
   m_VecContainer.GetItems(Items);
   if(Items.GetTail() != pLast)
      pLast = NULL;

   while(!SelItems.IsEmpty())
      ((CVecBase*)SelItems.RemoveTail())->SendOneBackward();

   if(pLast!=NULL)
      pLast->SendToBack();

   m_VecContainer.Repaint();
   SET_MODIFIED_FLAG;
}

void CVecDemoView::OnUpdateOrderSendoneback(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

   pCmdUI->Enable(GetSelectedItemsCount(TRUE) > 0);
}

void CVecDemoView::OnOrderSendonefore() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

   CPtrList SelItems;
   GetSelectedItems(SelItems, TRUE);
   StoreUndoStepItemsZOrder(SelItems);
   CVecBase* pFirst = (CVecBase*)SelItems.GetHead();
   CPtrList Items;
   m_VecContainer.GetItems(Items);
   if(Items.GetHead() != pFirst)
      pFirst = NULL;

   while(!SelItems.IsEmpty())
      ((CVecBase*)SelItems.RemoveHead())->BringOneForward();

   if(pFirst!=NULL)
      pFirst->BringToFront();
   
   m_VecContainer.Repaint();
   SET_MODIFIED_FLAG;
}

void CVecDemoView::OnUpdateOrderSendonefore(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

   pCmdUI->Enable(GetSelectedItemsCount(TRUE) > 0);
}

UINT CVecDemoView::GetSelectedItems(CPtrList& Items, BOOL bExcludeLockObjects)
{
   UINT uiCount = m_VecContainer.GetSelectedItems(Items);
   if(bExcludeLockObjects)
      if(ExcludeLockObjects(Items))
         uiCount = Items.GetCount();
   return uiCount;
}

UINT CVecDemoView::GetSelectedItemsCount(BOOL bExcludeLockObjects)
{
   CPtrList Items;
   UINT uiCount = m_VecContainer.GetSelectedItems(Items);
   if(bExcludeLockObjects)
      if(ExcludeLockObjects(Items))
         uiCount = Items.GetCount();
   return uiCount;
}

BOOL CVecDemoView::ExcludeLockObjects(CPtrList& Items)
{
   POSITION pos = Items.GetHeadPosition();
   BOOL bWasLocked = FALSE;
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)Items.GetNext(pos);
      if(pVec->IsLocked())
      {
         POSITION removePos = Items.Find(pVec);
         Items.RemoveAt(removePos);
         bWasLocked = TRUE;
      }
   }
   return bWasLocked;
}


void CVecDemoView::OnGroupGroup() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

   CPtrList SelItems;
   GetSelectedItems(SelItems, TRUE);
   CVecContainer::ArrangeItemsForTrack(SelItems);

   POSITION pos = SelItems.GetHeadPosition();
   CVecContainer* pParent = NULL;
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*) SelItems.GetNext(pos);
      if(pParent==NULL)
         pParent = pVec->GetParent();
      else
         if(pParent!=pVec->GetParent())
            return;
   }
   if(pParent==NULL)
      return;

   StoreUndoStepItemsZOrder(SelItems);

   CVecContainer* pVecContainer = pParent->Group(SelItems);

   m_VecController.m_UndoManager.AddActionToLastStep(pVecContainer, UAT_UNGROUP_ITEMS);

   pVecContainer->SetSelected(TRUE);
   m_VecContainer.Repaint();

   SET_MODIFIED_FLAG;
}

void CVecDemoView::OnUpdateGroupGroup(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

   if(GetSelectedItemsCount(TRUE) < 1)
   {
      pCmdUI->Enable(FALSE);
      return;
   }

   CPtrList SelItems;
   GetSelectedItems(SelItems, TRUE);
   CVecContainer::ArrangeItemsForTrack(SelItems);

   POSITION pos = SelItems.GetHeadPosition();
   CVecContainer* pParent = NULL;
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*) SelItems.GetNext(pos);
      if(pParent==NULL)
         pParent = pVec->GetParent();
      else
         if(pParent!=pVec->GetParent())
         {
            pCmdUI->Enable(FALSE);
            return;
         }
   }
   if(pParent==NULL)
   {
      pCmdUI->Enable(FALSE);
      return;
   }
   pCmdUI->Enable(TRUE);
}

void CVecDemoView::OnGroupUngroup() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

   CPtrList SelItems;
   GetSelectedItems(SelItems, TRUE);

   POSITION pos = SelItems.GetHeadPosition();
   BOOL bChanged = FALSE;
   m_VecController.m_UndoManager.CreateNewStep();
   while(pos != NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);
      if(pVec->GetType()==VEC_CONTAINER)
      {
         CVecContainer* pVecContainer = (CVecContainer*)pVec;
         CPtrList GroupItems;
   
         m_VecController.m_UndoManager.AddActionToLastStep(pVecContainer, UAT_GROUP_ITEMS);

         pVecContainer->Ungroup(GroupItems);
         while(!GroupItems.IsEmpty())
         {
            CVecBase* pVec = (CVecBase*)GroupItems.RemoveTail();
            pVec->SetSelected(TRUE);
         }
         delete pVecContainer;
         bChanged = TRUE;
      }
   }

   if(bChanged)
   {
      m_VecContainer.Repaint();
      SET_MODIFIED_FLAG;
   }
}

void CVecDemoView::OnUpdateGroupUngroup(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

   CPtrList SelItems;
   m_VecContainer.GetSelectedItems(SelItems);
   POSITION pos = SelItems.GetHeadPosition();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);
      if(pVec->IsLocked())
         continue;
      if(pVec->GetType() == VEC_CONTAINER)
      {
         pCmdUI->Enable(TRUE);
         return;
      }
   }
   pCmdUI->Enable(FALSE);
}


void CVecDemoView::OnPictureFlipHorizontal() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

   CPtrList SelItems;
   GetSelectedItems(SelItems, TRUE);
   CVecContainer::ArrangeItemsForTrack(SelItems);
   POSITION pos = SelItems.GetHeadPosition();
   m_VecController.m_UndoManager.CreateNewStep();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);;
      m_VecController.m_UndoManager.AddActionToLastStep(pVec, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
      pVec->Flip(TRUE, NULL);
   }
   m_VecContainer.Repaint();

   SET_MODIFIED_FLAG;
}

void CVecDemoView::OnPictureFlipVertical() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

   CPtrList SelItems;
   GetSelectedItems(SelItems, TRUE);
   CVecContainer::ArrangeItemsForTrack(SelItems);
   POSITION pos = SelItems.GetHeadPosition();
   m_VecController.m_UndoManager.CreateNewStep();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);;
      m_VecController.m_UndoManager.AddActionToLastStep(pVec, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
      pVec->Flip(FALSE, NULL);
   }
   m_VecContainer.Repaint();

   SET_MODIFIED_FLAG;
}

void CVecDemoView::OnUpdatePictureFlip(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

   pCmdUI->Enable(GetSelectedItemsCount(TRUE) > 0);
}

void CVecDemoView::OnPictureRotate180() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

	DoRotate(180);

   SET_MODIFIED_FLAG;
}

void CVecDemoView::OnPictureRotate90ccw() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

	DoRotate(-90);

   SET_MODIFIED_FLAG;
}

void CVecDemoView::OnPictureRotate90cw() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

	DoRotate(90);

   SET_MODIFIED_FLAG;
}

void CVecDemoView::DoRotate(float ang)
{
   ASSERT_VIEW_DISABLED;

   CPtrList SelItems;
   GetSelectedItems(SelItems, TRUE);
   CVecContainer::ArrangeItemsForTrack(SelItems);

   POSITION pos = SelItems.GetHeadPosition();
   m_VecController.m_UndoManager.CreateNewStep();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);;
      m_VecController.m_UndoManager.AddActionToLastStep(pVec, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
      pVec->Rotate(DegToRad(ang), NULL);
   }
   m_VecContainer.Repaint();
   m_VecController.SetModified();
}

void CVecDemoView::OnRotateFree() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

	CRotateFreeDlg dlg(this);
   if(dlg.DoModal()==IDOK)
   {
   }
}

void CVecDemoView::OnUpdatePictureRotate(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

   pCmdUI->Enable(GetSelectedItemsCount(TRUE) > 0);
}


void CVecDemoView::OnTextFont() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

   CClientDC dc(NULL);
   LOGFONT logfont;
   memset(&logfont, 0, sizeof(LOGFONT));

   CPtrList SelItems;
   m_VecContainer.GetSelectedItems(SelItems);
   POSITION pos = SelItems.GetHeadPosition();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);;
      if(pVec->HasText())
      {
         CVecTextBase* pTextBase = dynamic_cast<CVecTextBase*>(pVec);
         pTextBase->GetFont(&logfont);
         break;//initiate the dialog with the data of the first text item
      }
   }

	CFontDialog dlg(&logfont, CF_SCREENFONTS| CF_TTONLY);
   if(dlg.DoModal()==IDOK)
   {
      CPtrList SelItems;
      m_VecContainer.GetSelectedItems(SelItems);
      POSITION pos = SelItems.GetHeadPosition();
      m_VecController.m_UndoManager.CreateNewStep();
      while(pos!=NULL)
      {
         CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);;
         if(pVec->HasText())
         {
            m_VecController.m_UndoManager.AddActionToLastStep(pVec, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
            dlg.GetCurrentFont(&logfont);
            logfont.lfHeight = -logfont.lfHeight;

            CVecTextBase* pTextBase = dynamic_cast<CVecTextBase*>(pVec);
            pTextBase->SetFont(&logfont);
         }
      }
      m_VecContainer.Repaint();
      SET_MODIFIED_FLAG;
   }
}

void CVecDemoView::OnUpdateTextFont(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

   pCmdUI->Enable(CheckIfSelected(VECENT_TEXT));
}

void CVecDemoView::OnObjectBackcolor() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

   COLORREF col = RGB(0,0,0);
   CPtrList SelItems;
   m_VecContainer.GetSelectedItems(SelItems);
   POSITION pos = SelItems.GetHeadPosition();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);;
      col = pVec->GetBackColor();
      break;//initiate the dialog with the data of the first text item
   }

	CColorDialog dlg(col, CC_FULLOPEN);
   if(dlg.DoModal()==IDOK)
   {
      CPtrList SelItems;
      m_VecContainer.GetSelectedItems(SelItems);
      POSITION pos = SelItems.GetHeadPosition();
      m_VecController.m_UndoManager.CreateNewStep();
      while(pos!=NULL)
      {
         CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);;
         m_VecController.m_UndoManager.AddActionToLastStep(pVec, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
         pVec->SetBackColor(dlg.GetColor());
      }
      m_VecContainer.Repaint();
      SET_MODIFIED_FLAG;
   }
}

void CVecDemoView::OnObjectForecolor() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

   COLORREF col = RGB(0,0,0);
   CPtrList SelItems;
   m_VecContainer.GetSelectedItems(SelItems);
   POSITION pos = SelItems.GetHeadPosition();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);;
      col = pVec->GetForeColor();
      break;//initiate the dialog with the data of the first text item
   }

	CColorDialog dlg(col, CC_FULLOPEN);
   if(dlg.DoModal()==IDOK)
   {
      CPtrList SelItems;
      m_VecContainer.GetSelectedItems(SelItems);
      POSITION pos = SelItems.GetHeadPosition();
      m_VecController.m_UndoManager.CreateNewStep();
      while(pos!=NULL)
      {
         CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);;
         m_VecController.m_UndoManager.AddActionToLastStep(pVec, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
         pVec->SetForeColor(dlg.GetColor());
      }
      m_VecContainer.Repaint();
      SET_MODIFIED_FLAG;
   }
}

void CVecDemoView::OnUpdateObjectForecolor(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

   if(CheckIfSelected(VECENT_TEXT)||
      CheckIfSelected(VECENT_LINE)||
      CheckIfSelected(VECENT_SHAPE))
      pCmdUI->Enable(TRUE);
   else
      pCmdUI->Enable(FALSE);
}

void CVecDemoView::OnUpdateObjectBackcolor(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

   if(CheckIfSelected(VECENT_TEXT)||
      CheckIfSelected(VECENT_LINE)||
      CheckIfSelected(VECENT_SHAPE))
      pCmdUI->Enable(TRUE);
   else
      pCmdUI->Enable(FALSE);
}

void CVecDemoView::OnLineLine() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

	CLineTypeDlg dlg(this);
   if(dlg.DoModal()==IDOK)
   {
   }
}

void CVecDemoView::OnUpdateLineLine(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

   if(CheckIfSelected(VECENT_LINE)||
      CheckIfSelected(VECENT_SHAPE)||
      CheckIfSelected(VECENT_TEXT))
      pCmdUI->Enable(TRUE);
   else
      pCmdUI->Enable(FALSE);
}

void CVecDemoView::OnTextEdittext() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

	CTextEditDlg dlg(this);
   if(dlg.DoModal()==IDOK)
   {
   }
}

void CVecDemoView::OnUpdateTextEdittext(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

   if(CheckIfSelected(VECENT_TEXT))
      pCmdUI->Enable(TRUE);
   else
      pCmdUI->Enable(FALSE);
}

void CVecDemoView::OnShapeFill() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

   BOOL bEnablePattern = FALSE;
   if(CheckIfSelected(VECENT_SHAPE))
      bEnablePattern = TRUE;

	CShapeFillDlg dlg(this, bEnablePattern);
   if(dlg.DoModal()==IDOK)
   {
   }
}

void CVecDemoView::OnUpdateShapeFill(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

   if(CheckIfSelected(VECENT_SHAPE)||
     CheckIfSelected(VECENT_LINE)||
     CheckIfSelected(VECENT_TEXT))
      pCmdUI->Enable(TRUE);
   else
      pCmdUI->Enable(FALSE);
}


void CVecDemoView::OnPageBackground() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

   CPageBackgroundDlg dlg(this);
   if(dlg.DoModal()==IDOK)
   {
   }
//   UpdateCurrentPageSnapshot();
}

void CVecDemoView::OnUpdatePageBackground(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

}

void CVecDemoView::OnPictureSize() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

   CPictureSizeDlg dlg(this);
   if(dlg.DoModal()==IDOK)
   {
   }
}

void CVecDemoView::OnUpdatePictureSize(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

   pCmdUI->Enable(GetSelectedItemsCount(TRUE) > 0);
}

void CVecDemoView::OnLButtonDown(UINT nFlags, CPoint point) 
{
   if(!IsViewDisabled())
   	CParentClass::OnLButtonDown(nFlags, point);
}

void CVecDemoView::OnRButtonDown(UINT nFlags, CPoint point) 
{
   if(!IsViewDisabled())
   	CParentClass::OnRButtonDown(nFlags, point);
}

void CVecDemoView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
   if(!IsViewDisabled())
   	CParentClass::OnLButtonDblClk(nFlags, point);
}

void CVecDemoView::OnViewGrid() 
{
	m_VecController.m_bShowGrid =! m_VecController.m_bShowGrid;
   m_VecContainer.Repaint();
}

void CVecDemoView::OnUpdateViewGrid(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;
	pCmdUI->SetCheck(m_VecController.m_bShowGrid);
}

void CVecDemoView::OnViewGridProperties() 
{
	CGridPropDlg dlg;
   dlg.m_dRealSize = m_VecController.m_ptGridSize.x;
   if(dlg.DoModal())
   {
      m_VecController.m_ptGridSize.y = m_VecController.m_ptGridSize.x = dlg.m_dRealSize;
      m_VecContainer.Repaint();
   }
	
}

void CVecDemoView::OnUpdateViewGridProperties(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;
	pCmdUI->Enable(m_VecController.m_bShowGrid);
}

void CVecDemoView::OnViewGuides() 
{
	m_VecController.m_bShowGuides =! m_VecController.m_bShowGuides;
   m_VecContainer.Repaint();
}

void CVecDemoView::OnUpdateViewGuides(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;
	pCmdUI->SetCheck(m_VecController.m_bShowGuides);
}

void CVecDemoView::OnViewRullers() 
{
   m_bShowRullers =! m_bShowRullers;
	m_bShowVert = m_bShowHorz = m_bShowRullers && !IsViewDisabled();
   UpdateRulerBars();
}

void CVecDemoView::OnUpdateViewRullers(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;
	pCmdUI->SetCheck(m_bShowRullers);
}

void CVecDemoView::OnViewSnap2grid() 
{
	m_VecController.m_bSnapToGrid =! m_VecController.m_bSnapToGrid;
}

void CVecDemoView::OnUpdateViewSnap2grid(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;
	pCmdUI->SetCheck(m_VecController.m_bSnapToGrid);
	pCmdUI->Enable(m_VecController.m_bShowGrid);
}

void CVecDemoView::OnViewSnap2guides() 
{
	m_VecController.m_bSnapToGuides =! m_VecController.m_bSnapToGuides;
}

void CVecDemoView::OnUpdateViewSnap2guides(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;
	pCmdUI->SetCheck(m_VecController.m_bSnapToGuides);
	pCmdUI->Enable(m_VecController.m_bShowGuides);
}


void CVecDemoView::OnViewLockGuides() 
{
	m_VecController.m_bLockGuides = !m_VecController.m_bLockGuides;
}

void CVecDemoView::OnUpdateViewLockGuides(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;
	pCmdUI->SetCheck(m_VecController.m_bLockGuides);
	pCmdUI->Enable(m_VecController.m_bShowGuides);
}

LRESULT CVecDemoView::OnRullerRightClick(WPARAM wParam, LPARAM lParam)
{
  return 0;
}

LRESULT CVecDemoView::OnRullerDBLClick(WPARAM wParam, LPARAM lParam)
{
  return 0;
}

LRESULT CVecDemoView::OnVecEvent(WPARAM wParam, LPARAM lParam)
{
   switch(wParam)
   {
   case VE_MODIFIED:
	   {

		}
	   break;

   case VE_MODIFIED_CHANGED:
      if(m_VecController.IsModified())

            GetDocument()->SetModifiedFlag();
      break;

   case VE_START_DRAG_DROP:
      {
         POINT p = {((int)(short)LOWORD(lParam)),((int)(short)HIWORD(lParam))};//see GET_X_LPARAM and GET_Y_LPARAM
         return OnStartDragDrop(p);
      }
      break;
   }
   return 0l;
}

void CVecDemoView::OnUpdatePictureCrop(CCmdUI* pCmdUI, UINT uiCrop)
{
   UPDATE_CMD_UI_VIEW_DISABLED;

   CPtrList SelItems;
   UINT uiCount = GetSelectedItems(SelItems, TRUE);
   if(uiCount==1)
      pCmdUI->Enable(((CVecBase*)SelItems.GetHead())->HasPicture());
   else
      pCmdUI->Enable(FALSE);

   pCmdUI->SetCheck(m_VecController.GetSelectedTool()==uiCrop);
}

BOOL CVecDemoView::OnPictureCrop(UINT uiCrop)
{
   CPtrList SelItems;
   UINT uiCount = GetSelectedItems(SelItems, TRUE);
   if(uiCount!=1)
      return FALSE;

   CVecBase* pVec = (CVecBase*)SelItems.GetHead();
   if(!pVec->HasPicture())
      return FALSE;
   
   if(m_VecController.GetSelectedTool() == uiCrop)
   {
      m_VecController.KillCropMode();
      return FALSE;
   }
   else
   {
      m_VecController.SetCropTool(uiCrop, pVec);
      return TRUE;
   }
}

void CVecDemoView::OnPictureCropEllipse() 
{
   OnPictureCrop(TOOL_CROP_ELLIPSE);
}

void CVecDemoView::OnUpdatePictureCropEllipse(CCmdUI* pCmdUI) 
{
   OnUpdatePictureCrop(pCmdUI, TOOL_CROP_ELLIPSE);
}

void CVecDemoView::OnPictureCropFreehand() 
{
   OnPictureCrop(TOOL_CROP_FREEHAND);
}

void CVecDemoView::OnUpdatePictureCropFreehand(CCmdUI* pCmdUI) 
{
   OnUpdatePictureCrop(pCmdUI, TOOL_CROP_FREEHAND);
}

void CVecDemoView::OnPictureCropPolygon() 
{
   OnPictureCrop(TOOL_CROP_POLYGON);
}

void CVecDemoView::OnUpdatePictureCropPolygon(CCmdUI* pCmdUI) 
{
   OnUpdatePictureCrop(pCmdUI, TOOL_CROP_POLYGON);
}

void CVecDemoView::OnPictureCropRectangle() 
{
   OnPictureCrop(TOOL_CROP_RECTANGLE);
}

void CVecDemoView::OnUpdatePictureCropRectangle(CCmdUI* pCmdUI) 
{
   OnUpdatePictureCrop(pCmdUI, TOOL_CROP_RECTANGLE);
}

void CVecDemoView::OnPictureCropShape() 
{
	if(PopupShapesMenu(FALSE))
      OnPictureCrop(TOOL_CROP_SHAPE);
}

void CVecDemoView::OnUpdatePictureCropShape(CCmdUI* pCmdUI) 
{
   OnUpdatePictureCrop(pCmdUI, TOOL_CROP_SHAPE);
}

void CVecDemoView::OnUpdateTextAlign(CCmdUI* pCmdUI, UINT uiAlign)
{
   UPDATE_CMD_UI_VIEW_DISABLED;
   
   if(CheckIfSelected(VECENT_TEXT))
      pCmdUI->Enable(TRUE);
   else
      pCmdUI->Enable(FALSE);

   CPtrList SelItems;
   m_VecContainer.GetSelectedItems(SelItems);
   POSITION pos = SelItems.GetHeadPosition();
   BOOL bAllignFound = FALSE;
   BOOL bOtherFound = FALSE;
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);;
      if(!pVec->HasText())
         continue;
      UINT uiAl = (dynamic_cast<CVecTextBase*>(pVec))->GetAligment();
      if(uiAl == uiAlign)
         bAllignFound = TRUE;
      else 
         bOtherFound = TRUE;
   }
   pCmdUI->SetCheck(bAllignFound && !bOtherFound);
}

void CVecDemoView::OnTextAlign(UINT uiAlign)
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

   CPtrList SelItems;
   m_VecContainer.GetSelectedItems(SelItems);
   POSITION pos = SelItems.GetHeadPosition();
   m_VecController.m_UndoManager.CreateNewStep();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);;
      if(!pVec->HasText())
         continue;
      m_VecController.m_UndoManager.AddActionToLastStep(pVec, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
      (dynamic_cast<CVecTextBase*>(pVec))->SetAligment(uiAlign);
   }
   m_VecContainer.Repaint();

   SET_MODIFIED_FLAG;
}

void CVecDemoView::OnObjectTextAligncenter() 
{
   OnTextAlign(AL_CENTER);
}

void CVecDemoView::OnUpdateObjectTextAligncenter(CCmdUI* pCmdUI) 
{
   OnUpdateTextAlign(pCmdUI, AL_CENTER);
}

void CVecDemoView::OnObjectTextAlignleft() 
{
   OnTextAlign(AL_LEFT);
}

void CVecDemoView::OnUpdateObjectTextAlignleft(CCmdUI* pCmdUI) 
{
   OnUpdateTextAlign(pCmdUI, AL_LEFT);
}

void CVecDemoView::OnObjectTextAlignright() 
{
   OnTextAlign(AL_RIGHT);
}

void CVecDemoView::OnUpdateObjectTextAlignright(CCmdUI* pCmdUI) 
{
   OnUpdateTextAlign(pCmdUI, AL_RIGHT);
}

BOOL CVecDemoView::PopupShapesMenu(BOOL bShowPrevuislySelected) 
{
   CShapesDlg shapesDlg;
   
   int index = shapesDlg.DoModal();
   if(index<0)
	   return FALSE;

   LPSHAPE lpShape = shapesDlg.GetSelectedShape(index);

	m_VecController.SetSelectedShapeTool(lpShape);
	
	return TRUE;
}


void CVecDemoView::OnObjectAlignLockobject() 
{
   ASSERT_VIEW_DISABLED;
   CPtrList SelItems;
   m_VecContainer.GetSelectedItems(SelItems);
   POSITION pos = SelItems.GetHeadPosition();
   m_VecController.m_UndoManager.CreateNewStep();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);;
      if(pVec->IsLocked() == FALSE && pVec->GetParent()==&m_VecContainer)
      {
         m_VecController.m_UndoManager.AddActionToLastStep(pVec, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
         if(pVec->GetParent()==&m_VecContainer)
            pVec->SetLocked(TRUE);
      }
   }
   m_VecContainer.Repaint();
   m_VecController.SetModified();
}

void CVecDemoView::OnObjectAlignUnlock() 
{
   ASSERT_VIEW_DISABLED;
   CPtrList SelItems;
   m_VecContainer.GetSelectedItems(SelItems);
   POSITION pos = SelItems.GetHeadPosition();
   m_VecController.m_UndoManager.CreateNewStep();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);;
      if(pVec->IsLocked() == TRUE && pVec->GetParent()==&m_VecContainer)
      {
         m_VecController.m_UndoManager.AddActionToLastStep(pVec, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
         if(pVec->GetParent()==&m_VecContainer)
            pVec->SetLocked(FALSE);
      }
   }
   m_VecContainer.Repaint();
   m_VecController.SetModified();
}

void CVecDemoView::OnUpdateObjectAlignLockobject(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

   CPtrList SelItems;
   m_VecContainer.GetSelectedItems(SelItems);
   POSITION pos = SelItems.GetHeadPosition();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);;
      if(pVec->IsLocked() == FALSE && pVec->GetParent()==&m_VecContainer)
      {
         pCmdUI->Enable(TRUE);
         return;
      }
   }
   pCmdUI->Enable(FALSE);
}

void CVecDemoView::OnUpdateObjectAlignUnlock(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

   CPtrList SelItems;
   m_VecContainer.GetSelectedItems(SelItems);
   POSITION pos = SelItems.GetHeadPosition();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);;
      if(pVec->IsLocked() == TRUE && pVec->GetParent()==&m_VecContainer)
      {
         pCmdUI->Enable(TRUE);
         return;
      }
   }
   pCmdUI->Enable(FALSE);
}

void CVecDemoView::OnUpdateObjectAlign(CCmdUI* pCmdUI)
{
   UPDATE_CMD_UI_VIEW_DISABLED;
   //at least tow objects and at least one that is not locked
   CPtrList AllItems,NotLockedItems;
   GetSelectedItems(AllItems);
   CVecContainer::ArrangeItemsForTrack(AllItems);
   GetSelectedItems(NotLockedItems, TRUE);
   CVecContainer::ArrangeItemsForTrack(NotLockedItems);

   pCmdUI->Enable(AllItems.GetCount()>=2 && NotLockedItems.GetCount()>=1);
}

void CVecDemoView::OnObjectAlign(UINT nID)
{
   ASSERT_VIEW_DISABLED;
   CPtrList SelItems;
   GetSelectedItems(SelItems);
   CVecContainer::ArrangeItemsForTrack(SelItems);

   BOOL bThereAreLockedItems = FALSE;
   if(nID == ID_OBJECT_ALIGN_DISTRIBUTEHORIZONTALLY || nID == ID_OBJECT_ALIGN_DISTRIBUTEVERTICALLY)
      ExcludeLockObjects(SelItems);
   else
   {
      POSITION pos = SelItems.GetHeadPosition();
      while(pos!=NULL)
      {
         CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);
         if(pVec->IsLocked())
         {
            bThereAreLockedItems = TRUE;
            break;
         }
      }
   }

   CDRect AlignRect;
   POSITION pos = SelItems.GetHeadPosition();
   int iCoutnt = 0;
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);
      if(bThereAreLockedItems && !pVec->IsLocked())
         continue;
      CDRect VecRect(pVec->GetRect());
      if(nID == ID_OBJECT_ALIGN_DISTRIBUTEHORIZONTALLY || nID == ID_OBJECT_ALIGN_DISTRIBUTEVERTICALLY)
         VecRect = CDRect(VecRect.CenterPoint(),VecRect.CenterPoint());
      if(iCoutnt==0)
         AlignRect = VecRect;
      else 
         AlignRect.UnionRect(AlignRect, VecRect);
      iCoutnt++;
   }

   //sort items
   if(nID == ID_OBJECT_ALIGN_DISTRIBUTEHORIZONTALLY || nID == ID_OBJECT_ALIGN_DISTRIBUTEVERTICALLY)
   {
      CPtrList SortedItems;
      while(!SelItems.IsEmpty())
      {
         CVecBase* pVecLargest = NULL; 
         POSITION pos = SelItems.GetHeadPosition();
         double dCurVal = 0;
         while(pos!=NULL)
         {
            CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);
            if(pVecLargest==NULL)
            {
               pVecLargest = pVec;
               if(nID == ID_OBJECT_ALIGN_DISTRIBUTEHORIZONTALLY)
                  dCurVal = pVecLargest->GetRect().CenterPoint().x;
               else
                  dCurVal = pVecLargest->GetRect().CenterPoint().y;
            }
            else
            {
               double d;
               if(nID == ID_OBJECT_ALIGN_DISTRIBUTEHORIZONTALLY)
                  d = pVec->GetRect().CenterPoint().x;
               else
                  d = pVec->GetRect().CenterPoint().y;

               if(d > dCurVal)
               {
                  pVecLargest = pVec;
                  if(nID == ID_OBJECT_ALIGN_DISTRIBUTEHORIZONTALLY)
                     dCurVal = pVecLargest->GetRect().CenterPoint().x;
                  else
                     dCurVal = pVecLargest->GetRect().CenterPoint().y;
               }
            }
         }
         POSITION RemovePos = SelItems.Find(pVecLargest);
         SelItems.RemoveAt(RemovePos);
         SortedItems.AddHead(pVecLargest);
      }
      SelItems.AddTail(&SortedItems);
   }

   m_VecController.m_UndoManager.CreateNewStep();
   pos = SelItems.GetHeadPosition();
   int iCur = 0;
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);
      if(pVec->IsLocked())
         continue;
      m_VecController.m_UndoManager.AddActionToLastStep(pVec, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
      CDRect VecRect(pVec->GetRect());
      switch(nID)
      {
      case ID_OBJECT_ALIGN_LEFT:
         VecRect.OffsetRect(CDPoint(AlignRect.left-VecRect.left, 0));
         break;
      case ID_OBJECT_ALIGN_CENTER:
         VecRect.OffsetRect(CDPoint(AlignRect.CenterPoint().x-VecRect.CenterPoint().x, 0));
         break;
      case ID_OBJECT_ALIGN_RIGHT:
         VecRect.OffsetRect(CDPoint(AlignRect.right-VecRect.right, 0));
         break;
      case ID_OBJECT_ALIGN_TOP:
         VecRect.OffsetRect(CDPoint(0, AlignRect.top-VecRect.top));
         break;
      case ID_OBJECT_ALIGN_MIDDLE:
         VecRect.OffsetRect(CDPoint(0, AlignRect.CenterPoint().y-VecRect.CenterPoint().y));
         break;
      case ID_OBJECT_ALIGN_BOTTOM:
         VecRect.OffsetRect(CDPoint(0, AlignRect.bottom-VecRect.bottom));
         break;
      case ID_OBJECT_ALIGN_DISTRIBUTEHORIZONTALLY:
         {
            double dCenter = AlignRect.left + ((AlignRect.Width()/(iCoutnt-1))*iCur);
            VecRect.OffsetRect(CDPoint(dCenter-VecRect.CenterPoint().x, 0));
         }
         break;
      case ID_OBJECT_ALIGN_DISTRIBUTEVERTICALLY:
         {
            double dCenter = AlignRect.top + ((AlignRect.Height()/(iCoutnt-1))*iCur);
            VecRect.OffsetRect(CDPoint(0, dCenter-VecRect.CenterPoint().y));
         }
         break;
      default:
         ASSERT(FALSE);
         return;
      }
      pVec->SetRect(VecRect);
      iCur++;
   }
   m_VecContainer.Repaint();
   SET_MODIFIED_FLAG;
}


void CVecDemoView::OnPicmappingRemovephoto() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

   CPtrList SelItems;
   UINT uiCount = GetSelectedItems(SelItems);
   if(uiCount==1)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetHead();
      if(pVec!=NULL && pVec->HasPicture() && pVec->GetType()!=VEC_RECTANGLE_PIC)
      {
         m_VecController.m_UndoManager.AddToStack(pVec, UAT_RESTORE_OB_FROM_DUMP);
         (dynamic_cast<CVecBitmapBase*>(pVec))->SetBitmap(NULL);
         pVec->SetLineStyle(PS_SOLID);
         m_VecContainer.Repaint();
         SET_MODIFIED_FLAG;
      }
   }
}

void CVecDemoView::OnUpdatePicmappingRemovephoto(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;
   CPtrList SelItems;
   UINT uiCount = GetSelectedItems(SelItems);
   if(uiCount==1)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetHead();
      if(pVec!=NULL && pVec->HasPicture() && pVec->GetType()!=VEC_RECTANGLE_PIC)
      {
         pCmdUI->Enable(TRUE);
         return;
      }
   }
   pCmdUI->Enable(FALSE);
}

void CVecDemoView::OnPicmappingResetmappingarea() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

   CPtrList SelItems;
   UINT uiCount = GetSelectedItems(SelItems);
   if(uiCount==1)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetHead();
      if(pVec!=NULL && ((pVec->HasPicture() && pVec->GetType()!=VEC_RECTANGLE_PIC)||(pVec->HasText() && pVec->GetType()!=VEC_RECTANGLE_TEXT)))
      {
         m_VecController.m_UndoManager.AddToStack(pVec, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
         CDRectangle mappingRect(pVec->GetRect());
         pVec->SetMappingRectangle(mappingRect);
         m_VecContainer.Repaint();
         SET_MODIFIED_FLAG;
      }
   }
}

void CVecDemoView::OnUpdatePicmappingResetmappingarea(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;
   CPtrList SelItems;
   UINT uiCount = GetSelectedItems(SelItems);
   if(uiCount==1)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetHead();
      if(pVec!=NULL && ((pVec->HasPicture() && pVec->GetType()!=VEC_RECTANGLE_PIC)||(pVec->HasText() && pVec->GetType()!=VEC_RECTANGLE_TEXT)))
      {
         pCmdUI->Enable(TRUE);
         return;
      }
   }
   pCmdUI->Enable(FALSE);
}

BOOL CVecDemoView::DesideIfToShowShowPoints(CVecBase* pVec)
{
   UINT uiType = pVec->GetType();
   if(uiType==VEC_RECTANGLE || uiType==VEC_RECTANGLE_PIC || uiType==VEC_RECTANGLE_TEXT)
      return FALSE;
   CVecPolygon* pVecPolygon = dynamic_cast<CVecPolygon*>(pVec);
   if(pVecPolygon!=NULL)
   {
      return TRUE;
   }
   CVecPolyline* pVecPolyline = dynamic_cast<CVecPolyline*>(pVec);
   if(pVecPolyline!=NULL)
   {
      return TRUE;
   }
   return FALSE;
}

void CVecDemoView::OnShapeShowpoints() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

   CPtrList SelItems;
   GetSelectedItems(SelItems, TRUE);
   POSITION pos = SelItems.GetHeadPosition();
   m_VecController.m_UndoManager.CreateNewStep();
   BOOL bOneFound = FALSE;
   BOOL bOneState = FALSE;
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);
      if(!DesideIfToShowShowPoints(pVec))
         continue;
      m_VecController.m_UndoManager.AddActionToLastStep(pVec, UAT_RESTORE_OB_FROM_LIGHT_DUMP);
      CVecPolygon* pVecPolygon = dynamic_cast<CVecPolygon*>(pVec);
      if(pVecPolygon!=NULL)
      {
         if(!bOneFound)
         {
            bOneState = !pVecPolygon->IsTrackAsShape();
            bOneFound = TRUE;
         }
         pVecPolygon->SetTrackAsShape(bOneState);
      }
      CVecPolyline* pVecPolyline = dynamic_cast<CVecPolyline*>(pVec);
      if(pVecPolyline!=NULL)
      {
         if(!bOneFound)
         {
            bOneState = !pVecPolyline->IsTrackAsShape();
            bOneFound = TRUE;
         }
         pVecPolyline->SetTrackAsShape(bOneState);
      }
   }
   m_VecContainer.Repaint();
   SET_MODIFIED_FLAG;
}

void CVecDemoView::OnUpdateShapeShowpoints(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;
   CPtrList SelItems;
   GetSelectedItems(SelItems, TRUE);
   POSITION pos = SelItems.GetHeadPosition();
   while(pos!=NULL)
   {
      CVecBase* pVec = (CVecBase*)SelItems.GetNext(pos);
      if(!DesideIfToShowShowPoints(pVec))
         continue;
      CVecPolygon* pVecPolygon = dynamic_cast<CVecPolygon*>(pVec);
      if(pVecPolygon!=NULL)
      {
         pCmdUI->Enable(TRUE);
         pCmdUI->SetCheck(!pVecPolygon->IsTrackAsShape());
         return;
      }
      CVecPolyline* pVecPolyline = dynamic_cast<CVecPolyline*>(pVec);
      if(pVecPolyline!=NULL)
      {
         pCmdUI->Enable(TRUE);
         pCmdUI->SetCheck(!pVecPolyline->IsTrackAsShape());
         return;
      }
   }
   pCmdUI->Enable(FALSE);
}

void CVecDemoView::OnObjectPictureShadow() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

   CShadowDlg dlg(this);
   if(dlg.DoModal()==IDOK)
   {
   }
}

void CVecDemoView::OnUpdateObjectPictureShadow(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

   pCmdUI->Enable(GetSelectedItemsCount(TRUE) > 0);
}



void CVecDemoView::OnUpdateEditProperties(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable(FALSE);
}

void CVecDemoView::OnObjectPictureTransparency() 
{
   KILL_CROP_MODE;
   ASSERT_VIEW_DISABLED;

   CPicTransparentDlg dlg(this);
   if(dlg.DoModal()==IDOK)
   {
   }

}

void CVecDemoView::OnUpdateObjectPictureTransparency(CCmdUI* pCmdUI) 
{
   UPDATE_CMD_UI_VIEW_DISABLED;

   pCmdUI->Enable(CheckIfSelected(VECENT_PIC));
}
