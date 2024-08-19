#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "MainApp.h"
#include "MainFrm.h"
#ifdef APP_MODE_MDI
#include "ChildFrm.h"
#endif
#include "MainDocument.h"
#include "MainView.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma region Constructors

CMainApp::CMainApp() noexcept
	: m_bHiColorIcons(TRUE)
{
	EnableHtmlHelp();

	SetAppID(_T("MDI.AppID.NoVersion"));
}

CMainApp theApp;

#pragma endregion
#pragma region Overridables

BOOL CMainApp::InitInstance()
{
	scion::engine::CGameApp::InitInstance();

	EnableTaskbarInteraction(FALSE);

	// SPLASH SCREEN BEGIN

	SetRegistryKey(_T("Retro Technique"));
	LoadStdProfileSettings(4);

	InitAppInfo();

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL, RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);
	
#ifdef APP_MODE_MDI
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_ApplicationTYPE,
		RUNTIME_CLASS(CMainDocument),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(CMainView));
#else
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMainDocument),
		RUNTIME_CLASS(CMainFrame),
		RUNTIME_CLASS(CMainView));
#endif
	if (!pDocTemplate)
	{
		return FALSE;
	}
	AddDocTemplate(pDocTemplate);

#ifdef APP_MODE_MDI
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
#endif

	if (!InitScionEngine())
	{
		return FALSE;
	}

	// SPLASH SCREEN END

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

#ifdef APP_MODE_MDI
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
	{
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	}
#endif

	if (!ProcessShellCommand(cmdInfo))
	{
		return FALSE;
	}

#ifdef APP_MODE_MDI
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
#else
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
#endif

	return TRUE;
}

int CMainApp::ExitInstance()
{
	QuitScionEngine();

	return scion::engine::CGameApp::ExitInstance();
}

void CMainApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CMainApp::LoadCustomState()
{

}

void CMainApp::SaveCustomState()
{

}

#pragma endregion
#pragma region Implementations

void CMainApp::InitAppInfo()
{
	TCHAR szFullPath[1024];

	GetModuleFileName(NULL, szFullPath, ARRAYSIZE(szFullPath));

	DWORD       uVerHnd = 0;
	const DWORD dwVerInfoSize = GetFileVersionInfoSize(szFullPath, &uVerHnd);
	if (dwVerInfoSize)
	{
		if (uVerHnd != 0)
		{
			return;
		}

		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, dwVerInfoSize);
		if (!hMem)
		{
			return;
		}

		LPSTR pszVffInfo = reinterpret_cast <CHAR*>(GlobalLock(hMem));
		if (!pszVffInfo)
		{
			GlobalFree(hMem);
			return;
		}

		GetFileVersionInfo(szFullPath, uVerHnd, dwVerInfoSize, pszVffInfo);

		struct LANGANDCODEPAGE
		{
			WORD uLanguage;
			WORD uCodePage;

		}*pTranslate = NULL;

		UINT uTranslate = 0;

		BOOL bRet = VerQueryValue(pszVffInfo,
			_T("\\VarFileInfo\\Translation"),
			reinterpret_cast <LPVOID*>(&pTranslate),
			&uTranslate);
		if (!bRet)
		{
			uTranslate = 0;
		}

		struct
		{
			LPCTSTR pszSubBlock;
			LPCTSTR pszBuffer;
			UINT    nBufferLen;
		} Queries[] =
		{
			{ _T("ProductVersion"), NULL, 0 },
			{ _T("ProductName"),    NULL, 0 },
			{ _T("LegalCopyright"), NULL, 0 }
		};
		constexpr const UINT uQueryCount = ARRAYSIZE(Queries);

		for (UINT i = 0; i < uQueryCount; i++)
		{
			for (UINT j = 0; j < (uTranslate / sizeof(LANGANDCODEPAGE)); j++)
			{
				CString strSubBlock;
				strSubBlock.Format(_T("\\StringFileInfo\\%04x%04x\\%s"),
					pTranslate[j].uLanguage,
					pTranslate[j].uCodePage,
					Queries[i].pszSubBlock);

				bRet = VerQueryValue(reinterpret_cast <LPVOID>(pszVffInfo),
					strSubBlock.GetString(),
					(LPVOID*)&Queries[i].pszBuffer,
					&Queries[i].nBufferLen);
				if (bRet)
				{
					break;
				}
			}

			if (!bRet)
			{
				Queries[i].pszBuffer = _T("???");
			}
		}

		m_strProductVersion = Queries[0].pszBuffer;
		m_strProductName = Queries[1].pszBuffer;
		m_strLegalCopyright = Queries[2].pszBuffer;

		GlobalUnlock(hMem);
		GlobalFree(hMem);
	}
}

#pragma endregion
#pragma region Messages

BEGIN_MESSAGE_MAP(CMainApp, scion::engine::CGameApp)
	ON_COMMAND(ID_APP_ABOUT, &CMainApp::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_HELP, &CMainApp::OnHelp)
END_MESSAGE_MAP()

void CMainApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CMainApp::OnHelp()
{
	HtmlHelp(0, HH_DISPLAY_TOPIC);
}

#pragma endregion