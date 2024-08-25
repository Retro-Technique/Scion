#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ScionGraphicsTest
{
	TEST_CLASS(TextureTest)
	{
	public:

		TextureTest()
		{
			SetCurrentDirectory(_T("..\\..\\Scion-Graphics.Test"));
		}

		TEST_METHOD(TestLoad)
		{
			BOOL bIsMemDifferent = FALSE;
			
			HRESULT hr = LoadTexture(_T("res\\EQUITATION.PNG"), bIsMemDifferent);			

			Assert::AreEqual(FALSE, bIsMemDifferent, _T("Memory leak"));
			Assert::IsTrue(SUCCEEDED(hr), _com_error(hr).ErrorMessage());
		}

	private:

		HRESULT LoadTexture(LPCTSTR pszFileName, BOOL& bIsMemDifferent)
		{
			HRESULT hr = S_OK;
			_AFX_D2D_STATE* pD2DState = NULL;
			CWnd* pWnd = NULL;
			scion::engine::gfx::IGraphicsManager* pGraphicsManager = NULL;
			scion::engine::gfx::IRenderWindow* pRenderWindow = NULL;
			scion::engine::gfx::ITexture* pTexture = NULL;
			scion::common::CMemoryLeakChecker MemoryLeakChecker;

			do
			{
				HWND hWnd = NULL;
				if (hWnd = CreateWindow(_T("STATIC"),
					_T("dummy"),
					WS_VISIBLE,
					0, 0, 100, 100,
					NULL, NULL,
					NULL, NULL); !hWnd)
				{
					break;
				}

				if (pWnd = CWnd::FromHandle(hWnd); !pWnd)
				{
					break;
				}

				pWnd->EnableD2DSupport();

				if (pD2DState = AfxGetD2DState(); !pD2DState)
				{
					break;
				}

				if (const BOOL bSucceeded = pD2DState->InitD2D(); !bSucceeded)
				{
					break;
				}

				MemoryLeakChecker.Begin();

				if (hr = scion::engine::gfx::CreateGraphicsManager(&pGraphicsManager); FAILED(hr))
				{
					break;
				}

				if (hr = pGraphicsManager->Initialize(pD2DState); FAILED(hr))
				{
					break;
				}

				if (const BOOL bSucceeded = pWnd->IsD2DSupportEnabled(); !bSucceeded)
				{
					break;
				}

				if (hr = pGraphicsManager->CreateRenderWindow(&pRenderWindow); FAILED(hr))
				{
					break;
				}

				if (hr = pRenderWindow->CreateFromWindow(pWnd); FAILED(hr))
				{
					break;
				}

				if (hr = pRenderWindow->CreateTexture(&pTexture); FAILED(hr))
				{
					break;
				}

				if (hr = pTexture->LoadFromFile(pszFileName); FAILED(hr))
				{
					break;
				}

			} while (SCION_NULL_WHILE_LOOP_CONDITION);

			if (pTexture)
			{
				pTexture->Unload();
				pTexture->Release();
				pTexture = NULL;
			}

			if (pRenderWindow)
			{
				pRenderWindow->Destroy();
				pRenderWindow->Release();
				pRenderWindow = NULL;
			}

			if (pWnd)
			{
				pWnd->DestroyWindow();
				pWnd = NULL;
			}

			if (pGraphicsManager)
			{
				pGraphicsManager->Quit();
				pGraphicsManager->Release();
				pGraphicsManager = NULL;
			}

			if (pD2DState)
			{
				pD2DState->ReleaseD2DRefs();
				pD2DState = NULL;
			}

			bIsMemDifferent = MemoryLeakChecker.End();

			return S_OK;
		}

	};
}
