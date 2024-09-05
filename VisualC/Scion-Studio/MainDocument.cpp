#include "pch.h"
#include "framework.h"
#include "MainApp.h"
#include "MainDocument.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma region Constructors

IMPLEMENT_DYNCREATE(CMainDocument, scion::engine::CGameDocument)

CMainDocument::CMainDocument() noexcept
{
	// TODO: ajoutez ici le code d'une construction unique

}

CMainDocument::~CMainDocument()
{

}

#pragma endregion
#pragma region Operations

void CMainDocument::InitialUpdateScene()
{
	
}

void CMainDocument::UpdateScene()
{
	
}

void CMainDocument::DrawScene()
{
	
}

#pragma endregion
#pragma region Overridables

BOOL CMainDocument::OnNewDocument()
{
	if (!scion::engine::CGameDocument::OnNewDocument())
	{
		return FALSE;
	}

	CWnd* pWnd = AfxGetMainWnd();
	ASSERT_POINTER(pWnd, CWnd);
	ASSERT_VALID(pWnd);
	ASSERT_KINDOF(CMainFrame, pWnd);
	CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, pWnd);

	pMainFrame->SendMessageToDescendantsEx(WM_UPDATE_PANE, EPane_ResourceList, reinterpret_cast<LPARAM>(this));

	return TRUE;
}

BOOL CMainDocument::OnOpenDocument(LPCTSTR pszPathName)
{
	if (!scion::engine::CGameDocument::OnOpenDocument(pszPathName))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CMainDocument::OnSaveDocument(LPCTSTR pszPathName)
{
	// TODO: ajoutez ici votre code spécialisé et/ou l'appel de la classe de base

	return scion::engine::CGameDocument::OnSaveDocument(pszPathName);
}

void CMainDocument::OnCloseDocument()
{
	scion::engine::CGameDocument::OnCloseDocument();
}

void CMainDocument::Serialize(CArchive& ar)
{
	scion::engine::CGameDocument::Serialize(ar);
}

#ifdef _DEBUG

void CMainDocument::AssertValid() const
{
	scion::engine::CGameDocument::AssertValid();

}

void CMainDocument::Dump(CDumpContext& dc) const
{
	scion::engine::CGameDocument::Dump(dc);

}

#endif 

#pragma endregion
#pragma region Messages

BEGIN_MESSAGE_MAP(CMainDocument, CDocument)
	ON_COMMAND(ID_ADD_RESOURCE, &CMainDocument::OnAddResource)
END_MESSAGE_MAP()

void CMainDocument::OnAddResource()
{
	TRACE(_T("\tOnAddResource()\n"));
}

#pragma endregion