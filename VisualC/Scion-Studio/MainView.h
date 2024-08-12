#pragma once

class CMainView : public CView
{
#pragma region Constructors

	DECLARE_DYNCREATE(CMainView)

protected:

	CMainView() noexcept;
	virtual ~CMainView();

#pragma endregion
#pragma region Attributes

private:

	static constexpr const UINT_PTR TIMER_REFRESH_ID = 666;
	static constexpr const UINT		TIMER_REFRESH_FRAME_RATE = 25;
	static constexpr const UINT		TIMER_REFRESH_ELAPSE = 1000 / TIMER_REFRESH_FRAME_RATE;

public:

	CMainDocument* GetDocument() const;

#pragma endregion
#pragma region Overridables

public:

	BOOL PreCreateWindow(CREATESTRUCT& cs) override;
	void OnInitialUpdate() override;
	void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/) override;
	void OnDraw(CDC* pDC) override;
	
#ifdef _DEBUG
	void AssertValid() const override;
	void Dump(CDumpContext& dc) const override;
#endif

#pragma endregion
#pragma region Messages

protected:

	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnAfxDraw2d(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAfxRecreated2dresources(WPARAM wParam, LPARAM lParam);

#pragma endregion
};

#ifndef _DEBUG 
inline CMainDocument* CMainView::GetDocument() const
   { return STATIC_DOWNCAST(CMainDocument, m_pDocument); }
#endif

