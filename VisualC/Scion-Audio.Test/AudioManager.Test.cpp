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
			BOOL bIsMemDifferent = FALSE;

			{
				scion::engine::sfx::CAudioManager AudioManager;

				HWND hWnd = GetDesktopWindow();
				CWnd* pWnd = CWnd::FromHandle(hWnd);

				hr = AudioManager.Initialize(pWnd);

				AudioManager.Quit();

				pWnd->Detach();
			}

#ifdef _DEBUG
			_CrtMemCheckpoint(&State2);

			bIsMemDifferent = _CrtMemDifference(&State3, &State1, &State2);
#endif

			Assert::AreEqual(FALSE, bIsMemDifferent);
			Assert::IsTrue(SUCCEEDED(hr));
		}

	};
}