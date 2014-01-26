#if !defined(AFX_TEXT_ROUTINES_H__0C75D402_74E1_480D_B5BF_8DF64196FD5B__INCLUDED_)
#define AFX_TEXT_ROUTINES_H__0C75D402_74E1_480D_B5BF_8DF64196FD5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define TEXT_DRAW_BITMAP 1
#define TEXT_DRAW_ANTIALIAS 2
#define TEXT_DRAW_POLYGON 3

void DrawRotatedText(HDC hdc, LPCSTR lpszText, int iNumOfChars, LPPOINT pPoint, double dAngle, int iDrawType = TEXT_DRAW_BITMAP);
int NumOfCharsInWidth(HDC hDC, LPCSTR lpszText, int iNumOfChars, long lWidthToMatch, long &lRealWidth);
long GetLineWidth(HDC hDC, LPCSTR lpszText, int iNumOfChars);
void InitMul256();

#endif // !defined(AFX_TEXT_ROUTINES_H__0C75D402_74E1_480D_B5BF_8DF64196FD5B__INCLUDED_)
