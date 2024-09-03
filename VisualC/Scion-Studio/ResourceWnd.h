#pragma once

#include "resource.h"

class CResourceWnd : public CListCtrl
{
#pragma region Constructors

	DECLARE_DYNAMIC(CResourceWnd)

public:

	CResourceWnd();
	virtual ~CResourceWnd();

#pragma endregion
#pragma region Operations

public:

	void UpdateItems(scion::engine::CGameDocument* pDocument);

#pragma endregion
#pragma region Attributes

private:

	static constexpr const struct
	{
		UINT	_uStringID;
		INT		_nPercentSize;

	} COLUMNS[] =
	{
		{ IDS_RESOURCE_COL_TYPE, 10 },
		{ IDS_RESOURCE_COL_LABEL, 80 },
		{ IDS_RESOURCE_COL_REF, 10 },
	};
	static constexpr const INT COLUMN_COUNT = ARRAYSIZE(COLUMNS);
	C_ASSERT((COLUMNS[0]._nPercentSize + COLUMNS[1]._nPercentSize + COLUMNS[2]._nPercentSize) == 100);

private:

	CImageList	m_ImageList;

#pragma endregion
#pragma region Messages

protected:

	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

#pragma endregion
};


