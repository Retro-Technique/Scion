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

IMPLEMENT_DYNCREATE(CMainView, scion::engine::CRenderView)

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

#ifdef _DEBUG
void CMainView::AssertValid() const
{
	scion::engine::CRenderView::AssertValid();
}

void CMainView::Dump(CDumpContext& dc) const
{
	scion::engine::CRenderView::Dump(dc);
}
#endif 

#pragma endregion
#pragma region Messages

BEGIN_MESSAGE_MAP(CMainView, scion::engine::CRenderView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

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

#pragma endregion