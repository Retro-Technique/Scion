#include "pch.h"
//#ifndef SHARED_HANDLERS
//#include "MainApp.h"
//#endif
//#include "MainDocument.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace scion
{
	namespace engine
	{

#pragma region Constructors

		IMPLEMENT_DYNCREATE(CRenderView, CView)

		CRenderView::CRenderView() noexcept
			: m_pRenderWindow(NULL)
		{

		}

		CRenderView::~CRenderView()
		{

		}

#pragma endregion
#pragma region Attributes

//#ifdef _DEBUG
//		CMainDocument* CRenderView::GetDocument() const
//		{
//			ASSERT_KINDOF(CMainDocument, m_pDocument);
//			return STATIC_DOWNCAST(CMainDocument, m_pDocument);
//		}
//#endif

#pragma endregion
#pragma region Overridables

		BOOL CRenderView::PreCreateWindow(CREATESTRUCT& cs)
		{
			// TODO: changez ici la classe ou les styles Window en modifiant
			//  CREATESTRUCT cs

			return CView::PreCreateWindow(cs);
		}

		void CRenderView::OnInitialUpdate()
		{
			CView::OnInitialUpdate();

			/*CMainDocument* pDoc = GetDocument();
			ASSERT_POINTER(pDoc, CMainDocument);
			ASSERT_VALID(pDoc);

			pDoc->InitialUpdateScene();*/
		}

		void CRenderView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
		{
			/*CMainDocument* pDoc = GetDocument();
			ASSERT_POINTER(pDoc, CMainDocument);
			ASSERT_VALID(pDoc);

			CHwndRenderTarget* pRenderTarget = GetRenderTarget();
			ASSERT_POINTER(pRenderTarget, CHwndRenderTarget);
			ASSERT_VALID(pRenderTarget);

			pDoc->UpdateScene();*/

			Invalidate();
		}

		void CRenderView::OnDraw(CDC* pDC)
		{
			UNUSED_ALWAYS(pDC);
		}

#ifdef _DEBUG
		void CRenderView::AssertValid() const
		{
			CView::AssertValid();
		}

		void CRenderView::Dump(CDumpContext& dc) const
		{
			CView::Dump(dc);
		}
#endif 

#pragma endregion
#pragma region Messages

		BEGIN_MESSAGE_MAP(CRenderView, CView)
			ON_WM_TIMER()
			ON_WM_CREATE()
			ON_WM_DESTROY()
			ON_REGISTERED_MESSAGE(AFX_WM_DRAW2D, &CRenderView::OnAfxDraw2d)
			ON_REGISTERED_MESSAGE(AFX_WM_RECREATED2DRESOURCES, &CRenderView::OnAfxRecreated2dresources)
		END_MESSAGE_MAP()

		int CRenderView::OnCreate(LPCREATESTRUCT lpCreateStruct)
		{
			if (CView::OnCreate(lpCreateStruct) == -1)
			{
				return -1;
			}

			CWinApp* pWinApp = AfxGetApp();
			ASSERT_POINTER(pWinApp, CWinApp);
			ASSERT_VALID(pWinApp);
			ASSERT_KINDOF(CGameApp, pWinApp);

			CGameApp* pGameApp = STATIC_DOWNCAST(CGameApp, pWinApp);
		
			EnableD2DSupport();

			if (!IsD2DSupportEnabled())
			{
				TRACE(_T("D2D support hasn't been enabled\n"));
				return -1;
			}

			HRESULT hr = S_OK;
			const gfx::IGraphicsManager* pGraphicsManager = pGameApp->GetGraphicsManager();
			
			if (hr = pGraphicsManager->CreateRenderWindow(&m_pRenderWindow); FAILED(hr))
			{
				return -1;
			}

			if (hr = m_pRenderWindow->CreateFromWindow(this); FAILED(hr))
			{
				return -1;
			}

			SetTimer(TIMER_REFRESH_ID, TIMER_REFRESH_ELAPSE, NULL);

			return 0;
		}

		void CRenderView::OnDestroy()
		{
			CView::OnDestroy();

			if (m_pRenderWindow)
			{
				m_pRenderWindow->Destroy();
				m_pRenderWindow->Release();
				m_pRenderWindow = NULL;
			}
		}

		void CRenderView::OnTimer(UINT_PTR nIDEvent)
		{
			/*if (TIMER_REFRESH_ID == nIDEvent)
			{
				CMainDocument* pDoc = GetDocument();
				pDoc->UpdateAllViews(NULL);
			}*/

			CView::OnTimer(nIDEvent);
		}

		afx_msg LRESULT CRenderView::OnAfxDraw2d(WPARAM wParam, LPARAM lParam)
		{
			UNREFERENCED_PARAMETER(wParam);

			/*CHwndRenderTarget* pRenderTarget = (CHwndRenderTarget*)lParam;
			ASSERT_POINTER(pRenderTarget, CHwndRenderTarget);
			ASSERT_VALID(pRenderTarget);

			CMainDocument* pDoc = GetDocument();
			ASSERT_POINTER(pDoc, CMainDocument);
			ASSERT_VALID(pDoc);

			ID2D1HwndRenderTarget* pD2DRenderTarget = pRenderTarget->GetHwndRenderTarget();

			pDoc->DrawScene();*/

			return 0;
		}

		afx_msg LRESULT CRenderView::OnAfxRecreated2dresources(WPARAM wParam, LPARAM lParam)
		{
			TRACE(_T("Render target has been lost and re-created\n"));

			return 0;
		}

#pragma endregion

	}
}