#include "pch.h"
#include "AboutDlg.h"
#include "Resource.h"
#include "MainApp.h"

#pragma region Constructors

CAboutDlg::CAboutDlg() noexcept 
	: CDialogEx(IDD_ABOUTBOX)
	, m_strProductVer(_T(""))
{

}

#pragma endregion
#pragma region Overridables

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_PRODUCT_VER, m_strProductVer);
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Ajoutez ici une initialisation supplémentaire
	m_strProductVer = theApp.GetVersion();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}

#pragma endregion
#pragma region Messages

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

#pragma endregion