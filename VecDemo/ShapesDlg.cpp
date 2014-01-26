// ShapesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VecDemo.h"
#include "ShapesDlg.h"
#include "Shapes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DWORD CShapesDlg::m_defaultDataStyles[] = 
{
    { (DWORD)&ShapeArrowUp },
    { (DWORD)&ShapeBell },
    { (DWORD)&ShapeBird },
    { (DWORD)&ShapeBlast },
    { (DWORD)&ShapeBomb },
    { (DWORD)&ShapeBook },
    { (DWORD)&ShapeCandle },
    { (DWORD)&ShapeCat },
    { (DWORD)&ShapeClub },
    { (DWORD)&ShapeDiamon },
    { (DWORD)&ShapeDiamond2 },
    { (DWORD)&ShapeDog },
    { (DWORD)&ShapeDrop },
    { (DWORD)&ShapeEye },
    { (DWORD)&ShapeFinger },
    { (DWORD)&ShapeFish },
    { (DWORD)&ShapeFlag },
    { (DWORD)&ShapeFlower },
    { (DWORD)&ShapeHamsha },
    { (DWORD)&ShapeHeart },
    { (DWORD)&ShapeHouse },
    { (DWORD)&ShapeLightbulb },
    { (DWORD)&ShapeLightning },
    { (DWORD)&ShapeLips },
    { (DWORD)&ShapeMoon },
    { (DWORD)&ShapePencil },
    { (DWORD)&ShapePeper },
    { (DWORD)&ShapePhone },
    { (DWORD)&ShapeProfile },
    { (DWORD)&ShapePulm },
    { (DWORD)&ShapeSandClock },
    { (DWORD)&ShapeCards },
    { (DWORD)&ShapeSqwarl },
    { (DWORD)&ShapeStar15 },
    { (DWORD)&ShapeStar4 },
    { (DWORD)&ShapeStar5 },
    { (DWORD)&ShapeStar6 },
    { (DWORD)&ShapeSun },
    { (DWORD)&ShapeSunglasses },
    { (DWORD)&ShapeTalkBubble },
    { (DWORD)&ShapeTextBox },
    { (DWORD)&ShapeThumbUp },
    { (DWORD)&ShapeTrack },
    { (DWORD)&ShapeTree },
    { (DWORD)&ShapeTrofy },
    { (DWORD)&ShapeVideoCam },
    { (DWORD)&ShapeWineGlass },


    { (DWORD)&ShapeTwoheadarrow},
    { (DWORD)&ShapeTshirt},
    { (DWORD)&ShapeTurtle},
    { (DWORD)&ShapeThinkingcloud},
    { (DWORD)&ShapeThinkingcloud1},
    { (DWORD)&ShapeTalkbobble3},
    { (DWORD)&ShapeTalkbobble2},
    { (DWORD)&ShapeTalkbobble1},
    { (DWORD)&ShapeSpaceship},
    { (DWORD)&ShapeSorprise},
    { (DWORD)&ShapeSnail},
    { (DWORD)&ShapeShoe},
    { (DWORD)&ShapeShell},
    { (DWORD)&ShapeReebon},
    { (DWORD)&ShapeRabithead},
    { (DWORD)&Shapequestionmark},
    { (DWORD)&Shapeplus},
    { (DWORD)&ShapePicturframe},
    { (DWORD)&ShapePaper},
/*    { (DWORD)&ShapeNo9},
    { (DWORD)&ShapeNo8},
    { (DWORD)&ShapeNo7},
    { (DWORD)&ShapeNo6},
    { (DWORD)&ShapeNo5},
    { (DWORD)&ShapeNo4},
    { (DWORD)&ShapeNo3},
    { (DWORD)&ShapeNo2},
    { (DWORD)&ShapeNo1},*/
    { (DWORD)&ShapeMedal},
    { (DWORD)&ShapeMashroom},
    { (DWORD)&ShapeKey},
    { (DWORD)&ShapeIcecream},
    { (DWORD)&ShapeHeart2},
    { (DWORD)&ShapeGuitar},
    { (DWORD)&ShapeGhost},
    { (DWORD)&ShapeFootprint},
    { (DWORD)&ShapeFire},
    { (DWORD)&ShapeDolar},
    { (DWORD)&ShapeCow},
    { (DWORD)&ShapeCoopidon},
    { (DWORD)&ShapeCandy},
    { (DWORD)&ShapeBearhead},
    { (DWORD)&ShapeBalloon},
    { (DWORD)&ShapeApple},
    { (DWORD)&ShapeAlarmclock},
    { (DWORD)&ShapeAirplane},
    { (DWORD)&ShapeAgas}
};


/////////////////////////////////////////////////////////////////////////////
// CShapesDlg dialog


CShapesDlg::CShapesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShapesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShapesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

}

void CShapesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShapesDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShapesDlg, CDialog)
	//{{AFX_MSG_MAP(CShapesDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShapesDlg message handlers


BOOL CShapesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CSize btnSize(20,20);

	//Load Image List
	m_ilStyles.Create(IDB_SHAPES_BITMAP, btnSize.cx, btnSize.cy, 0xdeadbeef);

	CBitmap bmp;
	
	CPoint pos(10,10);
	CRect btnRect;
	
	int iNumColumns = 10;
	
	int iRow,iCol;

	for (int i=0; i< NUM_OF_SHAPES; i++)
	{
		GetImageFromList(&m_ilStyles,i,&bmp);	

		iCol= i%iNumColumns;
		iRow= i/iNumColumns;

		btnRect.left = pos.x + (iCol)*(btnSize.cx + 7); 
		btnRect.right = btnRect.left + btnSize.cx + 7;

		btnRect.top =  pos.y + (iRow)*(btnSize.cy + 7);
		btnRect.bottom = btnRect.top + btnSize.cy + 7;

		// Create a bitmap button.
		m_arrBtn[i].Create(_T(""), WS_CHILD|WS_VISIBLE|BS_BITMAP, 
			btnRect, this, 1);

		m_arrBtn[i].SetBitmap(bmp);

		bmp.Detach();
	}

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// public Methods


void CShapesDlg::GetImageFromList(CImageList *lstImages, 
                             int nImage, CBitmap* destBitmap)
{    
     CImageList tmpList;
     tmpList.Create(lstImages);
     
    tmpList.Copy( 0, nImage, ILCF_SWAP );
    
    IMAGEINFO lastImage;
    tmpList.GetImageInfo(0,&lastImage);
    
    CDC dcMem; dcMem.CreateCompatibleDC (GetWindowDC()); 
    
    CRect rect (lastImage.rcImage);
    
    destBitmap->CreateCompatibleBitmap (this->GetWindowDC(), 
                                      rect.Width (), rect.Height ());
    
    CBitmap* pBmpOld = dcMem.SelectObject (destBitmap);
    
    tmpList.DrawIndirect (&dcMem, 0, CPoint (0, 0), 
           CSize (rect.Width (), rect.Height ()), CPoint (0, 0));
    
    dcMem.SelectObject (pBmpOld);
}

BOOL CShapesDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	CDialog::OnCommand(wParam, lParam);

	EndDialog(GetButtonIndex(lParam));
	
	return TRUE;
}

int CShapesDlg::GetButtonIndex(LPARAM lParam)
{
	for(int i=0; i<NUM_OF_SHAPES; i++)
	{
		if(((HWND)lParam)==m_arrBtn[i].m_hWnd)
			return i;		
	}

	return -1;
}

LPSHAPE CShapesDlg::GetSelectedShape(int index)
{
	return (LPSHAPE)m_defaultDataStyles[index];
}

