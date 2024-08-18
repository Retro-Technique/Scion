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
			BOOL bIsMemDifferent = TRUE;

			HRESULT hr = S_OK;
			scion::engine::sfx::IAudioManager* pAudioManager = NULL;

			do
			{
				HWND hWnd = GetDesktopWindow();
				CWnd* pWnd = CWnd::FromHandle(hWnd);

				if (hr = scion::engine::sfx::CreateAudioManager(&pAudioManager))
				{
					break;
				}

				if (hr = pAudioManager->Initialize(pWnd); FAILED(hr))
				{
					break;
				}

				pAudioManager->Quit();

			} while (SCION_NULL_WHILE_LOOP_CONDITION);

			if (pAudioManager)
			{
				pAudioManager->Release();
				pAudioManager = NULL;
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
