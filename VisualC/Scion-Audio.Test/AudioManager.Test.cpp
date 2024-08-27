#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ScionAudioTest
{
	TEST_CLASS(AudioManagerTest)
	{
	public:

		TEST_METHOD(TestInitialize)
		{
			scion::common::CMemoryLeakChecker MemoryLeakChecker;
			BOOL bIsMemDifferent = TRUE;

			HRESULT hr = S_OK;
			scion::engine::sfx::IAudioManager* pAudioManager = NULL;

			do
			{
				HWND hWnd = GetDesktopWindow();
				CWnd* pWnd = CWnd::FromHandle(hWnd);

				MemoryLeakChecker.Begin();

				if (hr = scion::engine::sfx::CreateAudioManager(&pAudioManager); FAILED(hr))
				{
					break;
				}

				if (hr = pAudioManager->Initialize(pWnd); FAILED(hr))
				{
					break;
				}

			} while (SCION_NULL_WHILE_LOOP_CONDITION);

			if (pAudioManager)
			{
				pAudioManager->Quit();
				pAudioManager->Release();
				pAudioManager = NULL;
			}

			bIsMemDifferent = MemoryLeakChecker.End();

			Assert::AreEqual(FALSE, bIsMemDifferent, _T("Memory leak"));
			Assert::IsTrue(SUCCEEDED(hr), _com_error(hr).ErrorMessage());
		}

	};
}
