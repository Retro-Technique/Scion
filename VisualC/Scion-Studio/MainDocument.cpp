#include "pch.h"
#include "framework.h"
#include "MainApp.h"
#include "MainDocument.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma region Constructors

IMPLEMENT_DYNCREATE(CMainDocument, CDocument)

CMainDocument::CMainDocument() noexcept
{
	// TODO: ajoutez ici le code d'une construction unique

}

CMainDocument::~CMainDocument()
{

}

#pragma endregion
#pragma region Overridables

BOOL CMainDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
	{
		return FALSE;
	}

	// TODO: ajoutez ici le code de réinitialisation
	// (les documents SDI réutiliseront ce document)

	return TRUE;
}

BOOL CMainDocument::OnOpenDocument(LPCTSTR pszPathName)
{
	if (!CDocument::OnOpenDocument(pszPathName))
	{
		return FALSE;
	}

	// TODO:  Ajoutez ici votre code de création spécialisé

	return TRUE;
}

BOOL CMainDocument::OnSaveDocument(LPCTSTR pszPathName)
{
	// TODO: ajoutez ici votre code spécialisé et/ou l'appel de la classe de base

	return CDocument::OnSaveDocument(pszPathName);
}

void CMainDocument::OnCloseDocument()
{
	// TODO: ajoutez ici votre code spécialisé et/ou l'appel de la classe de base

	CDocument::OnCloseDocument();
}

void CMainDocument::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ajoutez ici le code de stockage
	}
	else
	{
		// TODO: ajoutez ici le code de chargement
	}
}

#ifdef _DEBUG

void CMainDocument::AssertValid() const
{
	CDocument::AssertValid();
}

void CMainDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}

#endif 

#pragma endregion
#pragma region Messages

BEGIN_MESSAGE_MAP(CMainDocument, CDocument)
END_MESSAGE_MAP()

#pragma endregion