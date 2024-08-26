#pragma once

class CPaneToolBar : public CMFCToolBar
{
#pragma region Constructors

	DECLARE_DYNAMIC(CPaneToolBar)

public:

	CPaneToolBar();
	virtual ~CPaneToolBar();

#pragma endregion
#pragma region Overridables

public:

	void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler) override;
	BOOL AllowShowOnList() const override;
	void AdjustLayout() override;

#pragma endregion
#pragma region Messages

protected:

	DECLARE_MESSAGE_MAP()

#pragma endregion
};