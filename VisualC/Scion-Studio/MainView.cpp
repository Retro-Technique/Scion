#include "pch.h"
#include "framework.h"
#ifndef SHARED_HANDLERS
#include "MainApp.h"
#endif
#include "MainDocument.h"
#include "MainView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma region Constructors

IMPLEMENT_DYNCREATE(CMainView, CView)

CMainView::CMainView() noexcept
{

}

CMainView::~CMainView()
{

}

#pragma endregion
#pragma region Attributes

#ifdef _DEBUG
CMainDocument* CMainView::GetDocument() const
{
	ASSERT_KINDOF(CMainDocument, m_pDocument);
	return STATIC_DOWNCAST(CMainDocument, m_pDocument);
}
#endif

#pragma endregion
#pragma region Overridables

BOOL CMainView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: changez ici la classe ou les styles Window en modifiant
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CMainView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CMainDocument* pDoc = GetDocument();
	ASSERT_POINTER(pDoc, CMainDocument);
	ASSERT_VALID(pDoc);

	pDoc->InitialUpdateScene();
}

void CMainView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	CMainDocument* pDoc = GetDocument();
	ASSERT_POINTER(pDoc, CMainDocument);
	ASSERT_VALID(pDoc);

	CHwndRenderTarget* pRenderTarget = GetRenderTarget();
	ASSERT_POINTER(pRenderTarget, CHwndRenderTarget);
	ASSERT_VALID(pRenderTarget);
	
	pDoc->UpdateScene();

	Invalidate();
}

void CMainView::OnDraw(CDC* pDC)
{
	UNUSED_ALWAYS(pDC);
}

#ifdef _DEBUG
void CMainView::AssertValid() const
{
	CView::AssertValid();
}

void CMainView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif 

#pragma endregion
#pragma region Messages

BEGIN_MESSAGE_MAP(CMainView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_REGISTERED_MESSAGE(AFX_WM_DRAW2D, &CMainView::OnAfxDraw2d)
	ON_REGISTERED_MESSAGE(AFX_WM_RECREATED2DRESOURCES, &CMainView::OnAfxRecreated2dresources)
END_MESSAGE_MAP()

int CMainView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	EnableD2DSupport();

	if (!IsD2DSupportEnabled())
	{
		TRACE(_T("D2D support hasn't been enabled\n"));
		return -1;
	}

	SetTimer(TIMER_REFRESH_ID, TIMER_REFRESH_ELAPSE, NULL);

	return 0;
}

void CMainView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMainView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void CMainView::OnTimer(UINT_PTR nIDEvent)
{
	if (TIMER_REFRESH_ID == nIDEvent)
	{
		CMainDocument* pDoc = GetDocument();
		pDoc->UpdateAllViews(NULL);
	}

	CView::OnTimer(nIDEvent);
}

afx_msg LRESULT CMainView::OnAfxDraw2d(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	CHwndRenderTarget* pRenderTarget = (CHwndRenderTarget*)lParam;
	ASSERT_POINTER(pRenderTarget, CHwndRenderTarget);
	ASSERT_VALID(pRenderTarget);

	CMainDocument* pDoc = GetDocument();
	ASSERT_POINTER(pDoc, CMainDocument);
	ASSERT_VALID(pDoc);

	ID2D1HwndRenderTarget* pD2DRenderTarget = pRenderTarget->GetHwndRenderTarget();

	pDoc->DrawScene();

	return 0;
}

#pragma endregion

afx_msg LRESULT CMainView::OnAfxRecreated2dresources(WPARAM wParam, LPARAM lParam)
{
	TRACE(_T("Render target has been lost and re-created\n"));

	return 0;
}
