#pragma once

class CResourcePane : public CDockablePane
{
#pragma region Constructors

	DECLARE_DYNAMIC(CResourcePane)

public:

	CResourcePane();
	virtual ~CResourcePane();

#pragma endregion
#pragma region Messages

protected:

	DECLARE_MESSAGE_MAP()

#pragma endregion
};


