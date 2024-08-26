#include "pch.h"
#include "framework.h"
#include "MainApp.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma region Constructors

#ifdef APP_MODE_MDI
IMPLEMENT_DYNAMIC(CMainFrame, FrameWnd)
#else
IMPLEMENT_DYNCREATE(CMainFrame, FrameWnd)
#endif

CMainFrame::CMainFrame() noexcept
{
	
}

CMainFrame::~CMainFrame()
{

}

#pragma endregion
#pragma region Operations

void CMainFrame::SendMessageToDescendantsEx(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	SendMessageToDescendants(uMessage, wParam, lParam);

	const CObList& listMiniFrames = m_dockManager.GetMiniFrames();

	POSITION pos = listMiniFrames.GetHeadPosition();
	while (pos)
	{
		const CObject* pObject = listMiniFrames.GetNext(pos);
		if (pObject->IsKindOf(RUNTIME_CLASS(CMultiPaneFrameWnd)))
		{
			CMultiPaneFrameWnd* pWnd = DYNAMIC_DOWNCAST(CMultiPaneFrameWnd, pObject);
			pWnd->SendMessageToDescendants(uMessage, wParam, lParam);
		}
	}
}

#pragma endregion
#pragma region Overridables

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!FrameWnd::PreCreateWindow(cs))
	{
		return FALSE;
	}

	return TRUE;
}

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	FrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	FrameWnd::Dump(dc);
}
#endif 

#pragma endregion
#pragma region Implementations

#ifdef APP_MODE_MDI
void CMainFrame::EnableTabbedGroups(BOOL bEnable)
{
	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // autres styles disponibles...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // définir à FALSE pour placer le bouton de fermeture à droite de la zone d'onglet
	mdiTabParams.m_bTabIcons = FALSE;    // définir à TRUE pour activer les icônes de document sous les onglets MDI
	mdiTabParams.m_bAutoColor = TRUE;    // définir à FALSE pour désactiver la coloration automatique des onglets MDI
	mdiTabParams.m_bDocumentMenu = TRUE; // activer le menu de document à l'extrémité droite de la zone d'onglet

	EnableMDITabbedGroups(bEnable, mdiTabParams);
}
#endif

BOOL CMainFrame::CreateMenuBar()
{
	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Impossible de créer la barre de menus\n");
		return FALSE;
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	return TRUE;
}

BOOL CMainFrame::CreateStatusBar()
{
	static constexpr const UINT INDICATORS[] =
	{
		ID_SEPARATOR,
		ID_INDICATOR_CAPS,
		ID_INDICATOR_NUM,
		ID_INDICATOR_SCRL,
	};
	static constexpr const UINT INDICATOR_COUNT = ARRAYSIZE(INDICATORS);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Impossible de créer la barre d'état\n");
		return FALSE;
	}

	m_wndStatusBar.SetIndicators(INDICATORS, INDICATOR_COUNT);

	return TRUE;
}

BOOL CMainFrame::CreateToolBars()
{
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("Échec de la création de la barre d'outils\n");
		return FALSE;
	}

	m_wndToolBar.SetPermament();
	m_wndToolBar.EnableTextLabels();

	// TODO: ajoutez ici votre code de création des barres d'outils

	return TRUE;
}

BOOL CMainFrame::CreatePanes()
{
	static constexpr const DWORD STYLE = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_FLOAT_MULTI;
	const CRect rcDummy(0, 0, 200, 200);

	// TODO: ajoutez ici votre code de création des volets
	if (!m_wndResourcePane.Create(I18N(IDS_VIEW_RESOURCES_LIST_LABEL), this, rcDummy, TRUE, ID_VIEW_RESOURCES_LIST, STYLE | CBRS_LEFT))
	{
		return FALSE;
	}

	return TRUE;
}

#pragma endregion
#pragma region Messages

BEGIN_MESSAGE_MAP(CMainFrame, FrameWnd)
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(ID_VIEW_RESOURCES_LIST, &CMainFrame::OnViewResourcesList)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESOURCES_LIST, &CMainFrame::OnUpdateViewResourcesList)
END_MESSAGE_MAP()

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (FrameWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

#ifdef APP_MODE_MDI
	EnableTabbedGroups(FALSE);
#endif

	if (!CreateMenuBar())
	{
		return -1;
	}

	if (!CreateToolBars())
	{
		return -1;
	}

	if (!CreateStatusBar())
	{
		return -1;
	}

	if (!CreatePanes())
	{
		return -1;
	}

	/* m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY); */
	/* m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY); */
	m_wndResourcePane.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);
	DockPane(&m_wndResourcePane);

	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
	CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);

	BOOL bValue = TRUE;
	DwmSetWindowAttribute(this->GetSafeHwnd(), DWMWA_USE_IMMERSIVE_DARK_MODE, &bValue, sizeof(BOOL));
	
	CDockingManager::SetDockingMode(DT_SMART);

	return 0;
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 854;
	lpMMI->ptMinTrackSize.y = 480;

	FrameWnd::OnGetMinMaxInfo(lpMMI);
}

void CMainFrame::OnViewResourcesList()
{
	ShowPane(&m_wndResourcePane, !(m_wndResourcePane.IsVisible()), FALSE, TRUE);

	RecalcLayout();
}

void CMainFrame::OnUpdateViewResourcesList(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndResourcePane.IsVisible());
}

#pragma endregion