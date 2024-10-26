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
	DDV_MaxChars(pDX, m_strName, scion::engine::CResourceManager::RESOURCE_NAME_MAX_LEN);
	DDX_Text(pDX, IDC_MFCEDITBROWSE_RESOURCEFILE, m_strFileName);
	DDV_MaxChars(pDX, m_strFileName, MAX_PATH);
}

#pragma endregion
#pragma region Implementations

void CResourceCreationDlg::SetDefaultNameFromFileName(LPCTSTR pszFilePath)
{
	LPCTSTR pszFileName = PathFindFileName(pszFilePath);
	
	m_strName.SetString(pszFileName);

	LPTSTR pszBuffer = m_strName.GetBufferSetLength(MAX_PATH);
	PathRemoveExtension(pszBuffer);
	m_strName.ReleaseBuffer();

	m_strName.MakeUpper();
	m_strName.Replace(_T(' '), _T('_'));

	UpdateData(UPDATE_DATA_BACK_TO_FRONT);
}

#pragma endregion
#pragma region Messages

BEGIN_MESSAGE_MAP(CResourceCreationDlg, CDialogEx)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDOK, &CResourceCreationDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE_RESOURCEFILE, &CResourceCreationDlg::OnEnChangeMfceditbrowseResourcefile)
END_MESSAGE_MAP()

int CResourceCreationDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	BOOL bValue = TRUE;
	if (const HRESULT hr = DwmSetWindowAttribute(GetSafeHwnd(), DWMWA_USE_IMMERSIVE_DARK_MODE, &bValue, sizeof(BOOL)); FAILED(hr))
	{
		return -1;
	}

	return 0;
}

void CResourceCreationDlg::OnEnChangeMfceditbrowseResourcefile()
{
	UpdateData(UPDATE_DATA_FRONT_TO_BACK);

	if (m_strName.IsEmpty())
	{		
		SetDefaultNameFromFileName(m_strFileName.GetString());
	}
}

void CResourceCreationDlg::OnBnClickedOk()
{
	if (m_strName.IsEmpty())
	{
		AfxMessageBox(IDS_RESOURCE_NAME_EMPTY, MB_OK | MB_ICONERROR);
		return;
	}

	if (m_strFileName.IsEmpty())
	{
		AfxMessageBox(IDS_RESOURCE_FILE_EMPTY, MB_OK | MB_ICONERROR);
		return;
	}

	CDialogEx::OnOK();
}

#pragma endregion