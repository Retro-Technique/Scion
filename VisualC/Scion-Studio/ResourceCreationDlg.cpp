#include "pch.h"
#include "ResourceCreationDlg.h"
#include "resource.h"

#pragma region Constructors

IMPLEMENT_DYNAMIC(CResourceCreationDlg, CDialogEx)

CResourceCreationDlg::CResourceCreationDlg(CWnd* pParent)
	: CDialogEx(IDD_DIALOG_RESOURCE_CREATION, pParent)
{

}

CResourceCreationDlg::~CResourceCreationDlg()
{

}

#pragma endregion
#pragma region Overridables

void CResourceCreationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_RESOURCE_NAME, m_strName);
	DDX_Text(pDX, IDC_MFCEDITBROWSE_RESOURCEFILE, m_strFileName);
	DDV_MaxChars(pDX, m_strFileName, MAX_PATH);
}

#pragma endregion
#pragma region Messages

BEGIN_MESSAGE_MAP(CResourceCreationDlg, CDialogEx)
	ON_WM_CREATE()
END_MESSAGE_MAP()

int CResourceCreationDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
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