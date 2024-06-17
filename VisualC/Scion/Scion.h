#pragma once

#ifndef __AFXWIN_H__
	#error "incluez 'pch.h' avant d'inclure ce fichier pour PCH"
#endif

#include "resource.h"		

class CScionApp : public CWinApp
{
#pragma region Constructors

public:

	CScionApp();
	virtual ~CScionApp();

#pragma endregion
#pragma region Overridables

public:

	BOOL InitInstance() override;
	int ExitInstance() override;

#pragma endregion
#pragma region Messages

protected:

	DECLARE_MESSAGE_MAP()

#pragma endregion
};
