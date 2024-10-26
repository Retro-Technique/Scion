#pragma once

#include "ResourceDocument.h"

class CMainDocument : public scion::engine::CGameDocument
{
#pragma region Constructors

	DECLARE_DYNCREATE(CMainDocument)

protected:

	CMainDocument() noexcept;
	virtual ~CMainDocument();

#pragma endregion
#pragma region Attributes

private:

	CResourceDocument m_ResourceDocument;

#pragma endregion
#pragma region Operations

public:

	HRESULT AddResource(LPCTSTR pszName, LPCTSTR pszFileName);

#pragma endregion
#pragma region Overridables

public:

	BOOL OnNewDocument() override;
	BOOL OnOpenDocument(LPCTSTR pszPathName) override;
	BOOL OnSaveDocument(LPCTSTR pszPathName) override;
	void OnCloseDocument() override;
	void Serialize(CArchive& ar) override;
	
#ifdef _DEBUG
	void AssertValid() const override;
	void Dump(CDumpContext& dc) const override;
#endif

#pragma endregion
#pragma region Implementations

private:

	void UpdateAllPanes(EPane ePane);

#pragma endregion
#pragma region Messages

protected:

	DECLARE_MESSAGE_MAP()

#pragma endregion
};
