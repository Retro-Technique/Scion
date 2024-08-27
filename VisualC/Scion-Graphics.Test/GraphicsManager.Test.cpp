#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ScionGraphicsTest
{
	TEST_CLASS(GraphicsManagerTest)
	{
	public:

		TEST_METHOD(TestInitialize)
		{
			scion::common::CMemoryLeakChecker MemoryLeakChecker;
			BOOL bIsMemDifferent = TRUE;

			HRESULT hr = S_OK;
			_AFX_D2D_STATE* pD2DState = NULL;
			scion::engine::gfx::IGraphicsManager* pGraphicsManager = NULL;

			do
			{
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

			} while (SCION_NULL_WHILE_LOOP_CONDITION);

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

			Assert::AreEqual(FALSE, bIsMemDifferent, _T("Memory leak"));
			Assert::IsTrue(SUCCEEDED(hr), _com_error(hr).ErrorMessage());
		}

	};
}
