#pragma once

#ifndef __AFXWIN_H__
	#error "incluez 'pch.h' avant d'inclure ce fichier pour PCH"
#endif

#include "resource.h"   

class CMainApp : public CWinAppEx
{
#pragma region Constructors

public:

	CMainApp() noexcept;

#pragma endregion
#pragma region Attributes

public:

	BOOL  m_bHiColorIcons;

#pragma endregion
#pragma region Operations

public:

	CString GetVersion() const;

#pragma endregion
#pragma region Overridables

public:

	BOOL InitInstance() override;
	int ExitInstance() override;
	void PreLoadState() override;
	void LoadCustomState() override;
	void SaveCustomState() override;

#pragma endregion
#pragma region Messages

public:

	DECLARE_MESSAGE_MAP()

	afx_msg void OnAppAbout();
	afx_msg void OnHelp();

#pragma endregion
};

extern CMainApp theApp;
