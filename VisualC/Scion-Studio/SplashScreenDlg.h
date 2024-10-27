#pragma once

#include "PictureControlWnd.h"

class CSplashScreenDlg : public CDialogEx
{
#pragma region Constructors

	DECLARE_DYNAMIC(CSplashScreenDlg)

public:

	CSplashScreenDlg(CWnd* pParent = NULL);
	virtual ~CSplashScreenDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SPLASHSCREEN };
#endif

#pragma endregion
#pragma region Attributes

private:

	CPictureControlWnd m_wndLogo;
	CStatic m_ctrlProductVer;
	CStatic m_ctrlCopyright;

#pragma endregion
#pragma region Overridables

protected:

	void DoDataExchange(CDataExchange* pDX) override; 
	BOOL OnInitDialog() override;

#pragma endregion
#pragma region Messages

protected:

	DECLARE_MESSAGE_MAP()

#pragma endregion
};
