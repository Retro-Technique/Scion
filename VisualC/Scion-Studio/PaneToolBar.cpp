#include "pch.h"
#include "PaneToolBar.h"

#pragma region Constructors

IMPLEMENT_DYNAMIC(CPaneToolBar, CMFCToolBar)

CPaneToolBar::CPaneToolBar()
{

}

CPaneToolBar::~CPaneToolBar()
{

}

#pragma endregion
#pragma region Overridables

void CPaneToolBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler) 
{
	UNREFERENCED_PARAMETER(pTarget);

	CMFCToolBar::OnUpdateCmdUI(static_cast<CFrameWnd*>(GetOwner()), bDisableIfNoHndler);
}

BOOL CPaneToolBar::AllowShowOnList() const 
{
	return FALSE;
}

void CPaneToolBar::AdjustLayout() 
{
	CMFCToolBar::AdjustLayout();

	STATIC_DOWNCAST(CDockablePane, GetParent())->AdjustLayout();
}

#pragma endregion
#pragma region Messages

BEGIN_MESSAGE_MAP(CPaneToolBar, CMFCToolBar)
END_MESSAGE_MAP()

#pragma endregion