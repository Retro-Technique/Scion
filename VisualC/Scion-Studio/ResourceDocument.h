#pragma once

class CResourceDocument : public CDocument
{
#pragma region Constructors

	DECLARE_DYNCREATE(CResourceDocument)

public:

	CResourceDocument();
	virtual ~CResourceDocument();

#pragma endregion
#pragma region Overridables

public:

#ifndef _WIN32_WCE
	void Serialize(CArchive& ar) override;  
#endif
#ifdef _DEBUG
	void AssertValid() const override;
#ifndef _WIN32_WCE
	void Dump(CDumpContext& dc) const override;
#endif
#endif

#pragma endregion
#pragma region Messages

protected:

	DECLARE_MESSAGE_MAP()

#pragma endregion
};
