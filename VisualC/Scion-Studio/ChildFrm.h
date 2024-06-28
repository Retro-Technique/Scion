#pragma once

#ifdef APP_MODE_MDI

class CChildFrame : public CMDIChildWndEx
{
#pragma region Constructors

	DECLARE_DYNCREATE(CChildFrame)

public:

	CChildFrame() noexcept;
	virtual ~CChildFrame();

#pragma endregion
#pragma region Attributes

private:

	CSplitterWnd m_wndSplitter;

#pragma endregion
#pragma region Overridables

public:

	BOOL PreCreateWindow(CREATESTRUCT& cs) override;

#ifdef _DEBUG
	void AssertValid() const override;
	void Dump(CDumpContext& dc) const override;
#endif

#pragma endregion
#pragma region Implementations

private:

	BOOL CreateToolBars();
	BOOL CreateStatusBar();

#pragma endregion
#pragma region Messages

protected:

	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

#pragma endregion
	
};

#endif