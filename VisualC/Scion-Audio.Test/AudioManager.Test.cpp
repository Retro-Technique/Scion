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
#ifdef _DEBUG
			_CrtMemState State1;
			_CrtMemState State2;
			_CrtMemState State3;

			_CrtMemCheckpoint(&State1);
#endif
			HRESULT hr = S_OK;
			BOOL bIsMemDifferent = TRUE;

			scion::engine::sfx::IAudioManager* pAudioManager = NULL;

			hr = scion::engine::sfx::CreateAudioManager(&pAudioManager);
			if (SUCCEEDED(hr))
			{
				HWND hWnd = GetDesktopWindow();
				CWnd* pWnd = CWnd::FromHandle(hWnd);

				hr = pAudioManager->Initialize(pWnd);

				pAudioManager->Quit();
				pAudioManager->Release();

				pWnd->Detach();
			}

#ifdef _DEBUG
			_CrtMemCheckpoint(&State2);

			bIsMemDifferent = _CrtMemDifference(&State3, &State1, &State2);
#else
			bIsMemDifferent = FALSE;
#endif

			Assert::AreEqual(FALSE, bIsMemDifferent, _T("Memory leak"));
			Assert::IsTrue(SUCCEEDED(hr), _com_error(hr).ErrorMessage());
		}

	};
}
