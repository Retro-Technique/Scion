#pragma once

class CPictureControlWnd : public CStatic
{
#pragma region Constructors

	DECLARE_DYNAMIC(CPictureControlWnd)

public:

	CPictureControlWnd(UINT uBitmapResourceID);
	virtual ~CPictureControlWnd();

#pragma endregion
#pragma region Attributes

private:

	UINT m_uBitmapResourceID;

#pragma endregion
#pragma region Overridables

public:

	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) override;

#pragma endregion
#pragma region Implementations

private:

	void CalcLayout(LPDRAWITEMSTRUCT lpDrawItemStruct, const CSize& szBitmap, CPoint& ptDestination, CSize& szDestination);
	void DrawPicture(LPDRAWITEMSTRUCT lpDrawItemStruct, const CBitmap& Bitmap, const CSize& szBitmap, const CPoint& ptDestination, const CSize& szDestination);

#pragma endregion
#pragma region Messages

protected:

	DECLARE_MESSAGE_MAP()

#pragma endregion
};


