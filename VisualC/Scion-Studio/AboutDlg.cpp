#include "pch.h"
#include "AboutDlg.h"
#include "Resource.h"
#include "MainApp.h"

#pragma region Constructors

CAboutDlg::CAboutDlg() noexcept 
	: CDialogEx(IDD_ABOUTBOX)
{

}

#pragma endregion
#pragma region Overridables

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_PRODUCT_VER, m_strProductVer);
	DDX_Text(pDX, IDC_STATIC_COPYRIGHT, m_strLegalCopyright);
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_strProductVer.Format(_T("%s v%s"), theApp.GetProductName(), theApp.GetProductVersion());

#ifdef _WIN64
	m_strProductVer += _T(" x64");
#else
	m_strProductVer += _T(" x86");
#endif

#ifdef _DEBUG
	m_strProductVer += _T(" DEBUG");
#endif

	m_strLegalCopyright = theApp.GetLegalCopyright();

	UpdateData(UPDATE_DATA_BACK_TO_FRONT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}

#pragma endregion
#pragma region Messages

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_CREATE()
END_MESSAGE_MAP()

int CAboutDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	BOOL bValue = TRUE;
	DwmSetWindowAttribute(GetSafeHwnd(), DWMWA_USE_IMMERSIVE_DARK_MODE, &bValue, sizeof(BOOL));

	return 0;
}

#pragma endregion