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

public:

	CMainDocument* GetDocument() const;

#pragma endregion
#pragma region Overridables

public:

	BOOL PreCreateWindow(CREATESTRUCT& cs) override;
	void OnDraw(CDC* pDC) override;
	
#ifdef _DEBUG
	void AssertValid() const override;
	void Dump(CDumpContext& dc) const override;
#endif

#pragma endregion
#pragma region Messages

protected:

	DECLARE_MESSAGE_MAP()

	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	
#pragma endregion
};

#ifndef _DEBUG 
inline CMainDocument* CMainView::GetDocument() const
   { return STATIC_DOWNCAST(CMainDocument, m_pDocument); }
#endif

