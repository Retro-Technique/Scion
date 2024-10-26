#include "pch.h"
#include "PictureControlWnd.h"

#pragma region Constructors

IMPLEMENT_DYNAMIC(CPictureControlWnd, CStatic)

CPictureControlWnd::CPictureControlWnd(UINT uBitmapResourceID)
	: m_uBitmapResourceID(uBitmapResourceID)
{

}

CPictureControlWnd::~CPictureControlWnd()
{

}

#pragma endregion
#pragma region Overridables

void CPictureControlWnd::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CBitmap Img;

    const BOOL bSucceeded = Img.LoadBitmap(m_uBitmapResourceID);
    if (!bSucceeded)
    {
        return;
    }

    BITMAP bmp;
    Img.GetBitmap(&bmp);

    CSize szDestination;
    CPoint ptDestination;
    const CSize szBitmap = { bmp.bmWidth, bmp.bmHeight };

    CalcLayout(lpDrawItemStruct, szBitmap, ptDestination, szDestination);

    DrawPicture(lpDrawItemStruct, Img, szBitmap, ptDestination, szDestination);
}

#pragma endregion
#pragma region Implementations

void CPictureControlWnd::CalcLayout(LPDRAWITEMSTRUCT lpDrawItemStruct, const CSize& szBitmap, CPoint& ptDestination, CSize& szDestination)
{
    ASSERT_POINTER(lpDrawItemStruct, sizeof(LPDRAWITEMSTRUCT));
    
    const CSize szControl = { lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom };   

    if (szBitmap.cx * szControl.cy > szControl.cx * szBitmap.cy)
    {
        szDestination.cx = szControl.cx;
        szDestination.cy = MulDiv(szDestination.cx, szBitmap.cy, szBitmap.cx);
    }
    else
    {
        szDestination.cy = szControl.cy;
        szDestination.cx = MulDiv(szDestination.cy, szBitmap.cx, szBitmap.cy);
    }

    ptDestination.y = (szControl.cy - szDestination.cy) / 2;
    ptDestination.x = (szControl.cx - szDestination.cx) / 2;
}

void CPictureControlWnd::DrawPicture(LPDRAWITEMSTRUCT lpDrawItemStruct, const CBitmap& Bitmap, const CSize& szBitmap, const CPoint& ptDestination, const CSize& szDestination)
{
    ASSERT_POINTER(lpDrawItemStruct, sizeof(LPDRAWITEMSTRUCT));

    CDC dcScreen;
    dcScreen.Attach(lpDrawItemStruct->hDC);

    CDC dcMem;
    dcMem.CreateCompatibleDC(&dcScreen);
    HGDIOBJ hOldBitmap = dcMem.SelectObject(Bitmap);

    dcScreen.SetStretchBltMode(HALFTONE);
    dcScreen.StretchBlt(ptDestination.x, ptDestination.y, szDestination.cx, szDestination.cy, &dcMem, 0, 0, szBitmap.cx, szBitmap.cy, SRCCOPY);

    dcMem.SelectObject(hOldBitmap);
    dcScreen.Detach();
}

#pragma endregion
#pragma region Messages

BEGIN_MESSAGE_MAP(CPictureControlWnd, CStatic)
END_MESSAGE_MAP()

#pragma endregion