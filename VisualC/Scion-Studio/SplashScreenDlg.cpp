#include "pch.h"
#include "resource.h"
#include "SplashScreenDlg.h"
#include "MainApp.h"


#pragma region Constructors

IMPLEMENT_DYNAMIC(CSplashScreenDlg, CDialogEx)

CSplashScreenDlg::CSplashScreenDlg(CWnd* pParent)
	: CDialogEx(IDD_SPLASHSCREEN, pParent)
	, m_wndLogo(IDB_BITMAP_LOGO)
{

}

CSplashScreenDlg::~CSplashScreenDlg()
{

}

#pragma endregion
#pragma region Overridables

void CSplashScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LOGO, m_wndLogo);
	DDX_Control(pDX, IDC_STATIC_PRODUCT_VER, m_ctrlProductVer);
	DDX_Control(pDX, IDC_STATIC_COPYRIGHT, m_ctrlCopyright);
}

BOOL CSplashScreenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CenterWindow();

	CString strProductVer;
	strProductVer.Format(_T("%s v%s"), theApp.GetProductName(), theApp.GetProductVersion());

	m_ctrlProductVer.SetFont(&GetGlobalData()->fontBold);
	m_ctrlProductVer.SetWindowText(strProductVer.GetString());
	m_ctrlCopyright.SetFont(&GetGlobalData()->fontBold);
	m_ctrlCopyright.SetWindowText(theApp.GetLegalCopyright());

	UpdateData(UPDATE_DATA_BACK_TO_FRONT);

	return TRUE;
}

#pragma endregion
#pragma region Messages

BEGIN_MESSAGE_MAP(CSplashScreenDlg, CDialogEx)
END_MESSAGE_MAP()

#pragma endregion