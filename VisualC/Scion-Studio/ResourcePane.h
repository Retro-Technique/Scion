#pragma once

#include "ResourceWnd.h"
#include "PaneToolBar.h"

class CResourcePane : public CDockablePane
{
#pragma region Constructors

	DECLARE_DYNAMIC(CResourcePane)

public:

	CResourcePane();
	virtual ~CResourcePane();

#pragma endregion
#pragma region Attributes

private:

	CPaneToolBar	m_wndToolBar;
	CResourceWnd	m_wndResourceList;

#pragma endregion
#pragma region Messages

protected:

	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnUpdatePane(WPARAM wParam, LPARAM lParam);
	afx_msg void OnAddResource();

#pragma endregion
public:
	afx_msg void OnRemoveResource();
};


