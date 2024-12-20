#include "pch.h"
#include "ResourcePane.h"
#include "resource.h"
#include "MainDocument.h"
#include "ResourceCreationDlg.h"
#include "MainView.h"

#pragma region Constructors

IMPLEMENT_DYNAMIC(CResourcePane, CDockablePane)

CResourcePane::CResourcePane()
{

}

CResourcePane::~CResourcePane()
{

}

#pragma endregion
#pragma region Messages

BEGIN_MESSAGE_MAP(CResourcePane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(WM_UPDATE_PANE, &CResourcePane::OnUpdatePane)
	ON_COMMAND(ID_ADD_RESOURCE, &CResourcePane::OnAddResource)
	ON_COMMAND(ID_REMOVE_RESOURCE, &CResourcePane::OnRemoveResource)
END_MESSAGE_MAP()

int CResourcePane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
	
	SetFont(&GetGlobalData()->fontRegular);

	if (!m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, AFX_IDW_TOOLBAR + 1) ||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR_RESOURCE))
	{
		return -1;
	}
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	if (!m_wndResourceList.Create(WS_CHILD | WS_VISIBLE | LVS_SINGLESEL | LVS_REPORT, CRect(0, 0, 0, 0), this, 1))
	{
		return -1;
	}

	return 0;
}

void CResourcePane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (GetSafeHwnd() == NULL || (AfxGetMainWnd() != NULL && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	const INT cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, 0, 0, cx, cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndResourceList.SetWindowPos(NULL, 0, cyTlb, cx, cy - cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
}

LRESULT CResourcePane::OnUpdatePane(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	if (wParam & EPane_ResourceList)
	{
		scion::engine::CGameDocument* pDocument = reinterpret_cast<scion::engine::CGameDocument*>(lParam);

		m_wndResourceList.UpdateItems(pDocument);
	}

	return 0;
}

void CResourcePane::OnAddResource()
{
	CMainDocument* pDocument = AFX_GET_ACTIVE_DOCUMENT(CMainDocument);
	if (!pDocument)
	{
		return;
	}

	scion::engine::CRenderView* pView = AFX_GET_ACTIVE_VIEW(scion::engine::CRenderView);
	ASSERT_POINTER(pView, scion::engine::CRenderView);
	ASSERT_VALID(pView);

	CResourceCreationDlg Dlg;	
	if (const INT_PTR nRet = Dlg.DoModal(); IDOK == nRet)
	{
		LPCTSTR pszName = Dlg.GetName();
		LPCTSTR pszFileName = Dlg.GetFileName();
		
		pDocument->AddResource(pszName, pszFileName);
	}
}

void CResourcePane::OnRemoveResource()
{
	CMainView* pView = AFX_GET_ACTIVE_VIEW(CMainView);
	if (!pView)
	{
		return;
	}
}

#pragma endregion