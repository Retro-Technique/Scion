#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ScionAudioTest
{
	TEST_CLASS(SoundTest)
	{
	public:

		SoundTest()
		{			
			SetCurrentDirectory(_T("..\\..\\Scion-Audio.Test"));
		}

		TEST_METHOD(TestLoadBOURB1M1)
		{			
			BOOL bIsDifferent = TRUE;
			CTimeSpan durSound;
			BOOL bIsPlayingBefore = TRUE;
			BOOL bIsPlayingAfter = FALSE;

			HRESULT hr = LoadWav(FILENAMES[0], durSound, bIsDifferent, bIsPlayingBefore, bIsPlayingAfter);

			Assert::AreEqual(FALSE, bIsPlayingBefore);
			Assert::AreEqual(TRUE, bIsPlayingAfter);
			Assert::AreEqual(FALSE, bIsDifferent);
			Assert::IsTrue(SUCCEEDED(hr));
			Assert::AreEqual(17l, durSound.GetSeconds());
		}

		TEST_METHOD(TestLoadGRPLAFF1)
		{
			BOOL bIsDifferent = TRUE;
			CTimeSpan durSound;
			BOOL bIsPlayingBefore = TRUE;
			BOOL bIsPlayingAfter = FALSE;

			HRESULT hr = LoadWav(FILENAMES[1], durSound, bIsDifferent, bIsPlayingBefore, bIsPlayingAfter);

			Assert::AreEqual(FALSE, bIsPlayingBefore);
			Assert::AreEqual(TRUE, bIsPlayingAfter);
			Assert::AreEqual(FALSE, bIsDifferent);
			Assert::IsTrue(SUCCEEDED(hr));
			Assert::AreEqual(2l, durSound.GetSeconds());
		}

		TEST_METHOD(TestLoadHELLO)
		{
			BOOL bIsDifferent = TRUE;
			CTimeSpan durSound;
			BOOL bIsPlayingBefore = TRUE;
			BOOL bIsPlayingAfter = FALSE;

			HRESULT hr = LoadWav(FILENAMES[2], durSound, bIsDifferent, bIsPlayingBefore, bIsPlayingAfter);

			Assert::AreEqual(FALSE, bIsPlayingBefore);
			Assert::AreEqual(TRUE, bIsPlayingAfter);
			Assert::AreEqual(FALSE, bIsDifferent);
			Assert::IsTrue(SUCCEEDED(hr));
			Assert::AreEqual(5l, durSound.GetSeconds());
		}

		TEST_METHOD(TestLoadSIREN1)
		{
			BOOL bIsDifferent = TRUE;
			CTimeSpan durSound;
			BOOL bIsPlayingBefore = TRUE;
			BOOL bIsPlayingAfter = FALSE;

			HRESULT hr = LoadWav(FILENAMES[3], durSound, bIsDifferent, bIsPlayingBefore, bIsPlayingAfter);

			Assert::AreEqual(FALSE, bIsPlayingBefore);
			Assert::AreEqual(TRUE, bIsPlayingAfter);
			Assert::AreEqual(FALSE, bIsDifferent);
			Assert::IsTrue(SUCCEEDED(hr));
			Assert::AreEqual(3l, durSound.GetSeconds());
		}

	private:

		static constexpr LPCTSTR FILENAMES[] = { _T("res\\BOURB1M1.WAV"), _T("res\\GRPLAFF1.WAV"), _T("res\\HELLO.WAV"), _T("res\\SIREN1.WAV") };
		static constexpr const INT_PTR FILENAME_COUNT = ARRAYSIZE(FILENAMES);

	private:

		HRESULT LoadWav(LPCTSTR pszFileName, CTimeSpan& durSound, BOOL& bIsMemDifferent, BOOL& bIsPlayingBefore, BOOL& bIsPlayingAfter)
		{
#ifdef _DEBUG
			_CrtMemState State1;
			_CrtMemState State2;
			_CrtMemState State3;

			_CrtMemCheckpoint(&State1);
#endif

			HRESULT hr = S_OK;

			do
			{
				scion::engine::sfx::CAudioManager AudioManager;
				HWND hWnd = GetDesktopWindow();
				CWnd* pWnd = CWnd::FromHandle(hWnd);

				AudioManager.Initialize(pWnd);

				scion::engine::sfx::CSoundBuffer SoundBuffer;
				scion::engine::sfx::CSound Sound;

				hr = SoundBuffer.LoadFromFile(pszFileName);
				if (FAILED(hr))
				{
					break;
				}

				hr = Sound.LoadFromBuffer(SoundBuffer);
				if (FAILED(hr))
				{
					break;
				}

				durSound = SoundBuffer.GetDuration();
				bIsPlayingBefore = Sound.IsPlaying();
				
				hr = Sound.Play(FALSE);
				if (FAILED(hr))
				{
					break;
				}

				bIsPlayingAfter = Sound.IsPlaying();

				Sound.Unload();
				SoundBuffer.Unload();

				AudioManager.Quit();

			} while (SCION_NULL_WHILE_LOOP_CONDITION);

#ifdef _DEBUG
			_CrtMemCheckpoint(&State2);

			bIsMemDifferent = _CrtMemDifference(&State3, &State1, &State2);
#else
			bIsMemDifferent = FALSE;
#endif

			return hr;
		}

	};
}
