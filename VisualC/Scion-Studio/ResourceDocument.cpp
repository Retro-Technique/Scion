#include "pch.h"
#include "ResourceDocument.h"

#pragma region Constructors

IMPLEMENT_DYNCREATE(CResourceDocument, CDocument)

CResourceDocument::CResourceDocument()
{

}

CResourceDocument::~CResourceDocument()
{

}
#pragma endregion
#pragma region Overridables

#ifdef _DEBUG
void CResourceDocument::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CResourceDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif

#ifndef _WIN32_WCE

void CResourceDocument::Serialize(CArchive& ar)
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
#endif

#pragma endregion
#pragma region Messages

BEGIN_MESSAGE_MAP(CResourceDocument, CDocument)
END_MESSAGE_MAP()

#pragma endregion