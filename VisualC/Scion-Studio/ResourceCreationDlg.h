#pragma once

class CResourceCreationDlg : public CDialogEx
{
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RESOURCE_CREATION };
#endif

#pragma region Constructors

	DECLARE_DYNAMIC(CResourceCreationDlg)

public:

	CResourceCreationDlg(CWnd* pParent = NULL); 
	virtual ~CResourceCreationDlg();

#pragma endregion
#pragma region Attributes

private:

	CString m_strName;
	CString m_strFileName;

public:

	/* ACCESSEURS */
	inline [[nodiscard]] LPCTSTR GetName() const { return m_strName.GetString(); }
	inline [[nodiscard]] LPCTSTR GetFileName() const { return m_strFileName.GetString(); }

#pragma endregion
#pragma region Overridables

protected:

	void DoDataExchange(CDataExchange* pDX) override;

#pragma endregion
#pragma region Implementations

private:

	void SetDefaultNameFromFileName(LPCTSTR pszFilePath);

#pragma endregion
#pragma region Messages

	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEnChangeMfceditbrowseResourcefile();
	afx_msg void OnBnClickedOk();

#pragma endregion
};
