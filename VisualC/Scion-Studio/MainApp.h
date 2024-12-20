#pragma once

#ifndef __AFXWIN_H__
	#error "incluez 'pch.h' avant d'inclure ce fichier pour PCH"
#endif

#include "resource.h"   

class CMainApp : public scion::engine::CGameApp
{
#pragma region Constructors

public:

	CMainApp() noexcept;

#pragma endregion
#pragma region Attributes

public:

	BOOL  m_bHiColorIcons;

private:

	CString m_strProductVersion;
	CString m_strProductName;
	CString m_strLegalCopyright;

public:

	inline [[nodiscard]] LPCTSTR GetProductVersion() const { return m_strProductVersion.GetString(); }
	inline [[nodiscard]] LPCTSTR GetProductName() const { return m_strProductName.GetString(); }
	inline [[nodiscard]] LPCTSTR GetLegalCopyright() const { return m_strLegalCopyright.GetString(); }

#pragma endregion
#pragma region Overridables

public:

	BOOL InitInstance() override;
	int ExitInstance() override;
	void PreLoadState() override;
	void LoadCustomState() override;
	void SaveCustomState() override;

#pragma endregion
#pragma region Implementations

private:

	void InitAppInfo();

#pragma endregion
#pragma region Messages

public:

	DECLARE_MESSAGE_MAP()

	afx_msg void OnAppAbout();
	afx_msg void OnHelp();

#pragma endregion
};

extern CMainApp theApp;
