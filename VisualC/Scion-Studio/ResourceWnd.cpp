#include "pch.h"
#include "ResourceWnd.h"

#pragma region Constructors

IMPLEMENT_DYNAMIC(CResourceWnd, CListCtrl)

CResourceWnd::CResourceWnd()
{

}

CResourceWnd::~CResourceWnd()
{

}

#pragma endregion
#pragma region Operations

void CResourceWnd::UpdateItems(scion::engine::CGameDocument* pDocument)
{
	ASSERT_POINTER(pDocument, scion::engine::CGameDocument);
	ASSERT_VALID(pDocument);

	static const auto ENUM_RESOURCES_PROC = [](LPCTSTR pszName, LONG nRefCount, LPVOID pData)
		{
			static INT s_nIndex = 0;

			CListCtrl* pListCtrl = reinterpret_cast<CListCtrl*>(pData);
			CString strRefCount;
			strRefCount.Format(_T("%d"), nRefCount);

			LVITEM lvItem = { 0 };

			lvItem.iItem = s_nIndex;

			lvItem.mask = LVIF_IMAGE;
			lvItem.iSubItem = 0;
			lvItem.iImage = 0; //TODO Resource Type
			pListCtrl->InsertItem(&lvItem);

			lvItem.mask = LVIF_TEXT;
			lvItem.iSubItem = 1;
			lvItem.pszText = StrDup(pszName);
			pListCtrl->SetItem(&lvItem);

			lvItem.iSubItem = 2;
			lvItem.pszText = StrDup(strRefCount.GetString());
			pListCtrl->SetItem(&lvItem);
			
			pListCtrl->SetItemData(s_nIndex, reinterpret_cast<DWORD_PTR>(pszName));

			s_nIndex++;
		};

	DeleteAllItems();

	pDocument->EnumerateResources(ENUM_RESOURCES_PROC, this);
}

#pragma endregion
#pragma region Messages

BEGIN_MESSAGE_MAP(CResourceWnd, CListCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CResourceWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	SetFont(&GetGlobalData()->fontRegular);

	//m_ImageList.Create(16, 16, ILC_COLOR24 | ILC_MASK, 2, 0);
	//CPngImage PngImage;
	//
	//m_ImageList.Add(&PngImage, RGB(255, 0, 255));

	SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);

	for (INT i = 0; i < COLUMN_COUNT; i++)
	{
		CString strHeader;

		if (strHeader.LoadString(COLUMNS[i]._uStringID))
		{
			LV_COLUMN lvColumn = { 0 };
			lvColumn.mask = LVCF_FMT | LVCF_TEXT;
			lvColumn.pszText = StrDup(strHeader.GetString());
			lvColumn.fmt = LVCFMT_CENTER;

			InsertColumn(i, &lvColumn);
		}
	}

	return 0;
}

void CResourceWnd::OnSize(UINT nType, int cx, int cy)
{
	CListCtrl::OnSize(nType, cx, cy);

	for (INT i = 0; i < COLUMN_COUNT; i++)
	{
		LV_COLUMN lvColumn = { 0 };
		lvColumn.mask = LVCF_WIDTH;

		if (GetColumn(i, &lvColumn))
		{
			lvColumn.cx = MulDiv(cx, COLUMNS[i]._nPercentSize, 100);
			SetColumn(i, &lvColumn);
		}
	}
}

#pragma endregion