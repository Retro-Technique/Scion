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

	inline LPCTSTR GetName() const { return m_strName.GetString(); }
	inline LPCTSTR GetFileName() const { return m_strFileName.GetString(); }

#pragma endregion
#pragma region Overridables

protected:

	void DoDataExchange(CDataExchange* pDX) override;

#pragma endregion
#pragma region Messages

	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

#pragma endregion
};
