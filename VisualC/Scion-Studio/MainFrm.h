#pragma once

#include "ResourcePane.h"

class CMainFrame : public CFrameWndEx
{
#pragma region Constructors

#ifdef APP_MODE_MDI
	DECLARE_DYNAMIC(CMainFrame)
#else
	DECLARE_DYNCREATE(CMainFrame)
#endif

public:

	CMainFrame() noexcept;
	virtual ~CMainFrame();

#pragma endregion
#pragma region Attributes

protected:

	CMFCMenuBar     m_wndMenuBar;
	CMFCToolBar     m_wndToolBar;
	CMFCStatusBar	m_wndStatusBar;
	CResourcePane	m_wndResourcePane;

#pragma endregion
#pragma region Operations

public:

	void SendMessageToDescendantsEx(UINT uMessage, WPARAM wParam, LPARAM lParam);

#pragma endregion
#pragma region Overridables

public:

	BOOL PreCreateWindow(CREATESTRUCT& cs) override;
	
#ifdef _DEBUG
	void AssertValid() const override;
	void Dump(CDumpContext& dc) const override;
#endif

#pragma endregion
#pragma region Implementations

private:

#ifdef APP_MODE_MDI
	void EnableTabbedGroups(BOOL bEnable);
#endif
	BOOL CreateMenuBar();
	BOOL CreateToolBars();
	BOOL CreatePanes();
	BOOL CreateStatusBar();

#pragma endregion
#pragma region Messages

protected:

	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnViewFullscreen();
	afx_msg void OnUpdateViewFullscreen(CCmdUI* pCmdUI);
	afx_msg void OnViewResourcesList();
	afx_msg void OnUpdateViewResourcesList(CCmdUI* pCmdUI);
	
#pragma endregion
};


