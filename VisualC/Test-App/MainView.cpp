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

void CMainView::OnDraw(CDC* /*pDC*/)
{
	CMainDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	{
		return;
	}

	// TODO: ajoutez ici le code de dessin pour les données natives
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