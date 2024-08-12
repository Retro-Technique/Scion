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

	cs.style &= ~(WS_BORDER);
	cs.hMenu = NULL;

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
	return TRUE;
}

BOOL CMainFrame::CreateStatusBar()
{
	return TRUE;
}

BOOL CMainFrame::CreateToolBars()
{
	return TRUE;
}

BOOL CMainFrame::CreatePanes()
{
	static constexpr const DWORD STYLE = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_FLOAT_MULTI;
	const CRect rcDummy(0, 0, 200, 200);

	// TODO: ajoutez ici votre code de création des volets

	return TRUE;
}

#pragma endregion
#pragma region Messages

BEGIN_MESSAGE_MAP(CMainFrame, FrameWnd)
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
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
	EnableDocking(CBRS_ALIGN_ANY);

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
	// TODO: ajoutez ici le code de votre gestionnaire de messages et/ou les paramètres par défaut des appels
	lpMMI->ptMinTrackSize.x = 854;
	lpMMI->ptMinTrackSize.y = 480;

	FrameWnd::OnGetMinMaxInfo(lpMMI);
}

#pragma endregion