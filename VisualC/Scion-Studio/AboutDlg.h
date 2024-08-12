#pragma once

class CAboutDlg : public CDialogEx
{
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

#pragma region Constructors

public:

	CAboutDlg() noexcept;

#pragma endregion
#pragma region Attributes

private:

	CString m_strProductVer;
	CString m_strLegalCopyright;

#pragma endregion
#pragma region Overridables

protected:

	void DoDataExchange(CDataExchange* pDX) override;   
	BOOL OnInitDialog() override;

#pragma endregion
#pragma region Messages

protected:

	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

#pragma endregion
};