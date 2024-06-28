#include "pch.h"

#ifdef APP_MODE_MDI

#include "framework.h"
#include "MainApp.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma region Constructors

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

CChildFrame::CChildFrame() noexcept
{
	
}

CChildFrame::~CChildFrame()
{

}

#pragma endregion
#pragma region Overridables

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CMDIChildWndEx::PreCreateWindow(cs))
	{
		return FALSE;
	}

	return TRUE;
}

#ifdef _DEBUG

void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}

#endif

#pragma endregion
#pragma region Implementations

BOOL CChildFrame::CreateStatusBar()
{
	// TODO: ajoutez ici votre code de création de la barre d'état

	return TRUE;
}

BOOL CChildFrame::CreateToolBars()
{
	// TODO: ajoutez ici votre code de création des barres d'outils

	return TRUE;
}

#pragma endregion
#pragma region Messages

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_WM_CREATE()
END_MESSAGE_MAP()

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
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

	// TODO:  Ajoutez ici votre code de création spécialisé

	return 0;
}

#pragma endregion

#endif