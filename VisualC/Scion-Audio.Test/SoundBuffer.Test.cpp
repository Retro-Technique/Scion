#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ScionAudioTest
{
	TEST_CLASS(SoundBufferTest)
	{
	private:

		scion::engine::sfx::CAudioManager m_AudioManager;

	public:

		SoundBufferTest()
		{			
			SetCurrentDirectory(_T("..\\..\\Scion-Audio.Test"));

			HWND hWnd = GetDesktopWindow();
			CWnd* pWnd = CWnd::FromHandle(hWnd);

			m_AudioManager.Initialize(pWnd);
		}

		~SoundBufferTest()
		{
			m_AudioManager.Quit();
		}

		TEST_METHOD(TestLoadBOURB1M1)
		{			
			BOOL bIsDifferent = TRUE;
			CTimeSpan durSound;

			HRESULT hr = LoadWav(FILENAMES[0], durSound, bIsDifferent);

			Assert::AreEqual(FALSE, bIsDifferent);
			Assert::IsTrue(SUCCEEDED(hr));
			Assert::AreEqual(17l, durSound.GetSeconds());
		}

		TEST_METHOD(TestLoadGRPLAFF1)
		{
			BOOL bIsDifferent = TRUE;
			CTimeSpan durSound;

			HRESULT hr = LoadWav(FILENAMES[1], durSound, bIsDifferent);

			Assert::AreEqual(FALSE, bIsDifferent);
			Assert::IsTrue(SUCCEEDED(hr));
			Assert::AreEqual(2l, durSound.GetSeconds());
		}

		TEST_METHOD(TestLoadHELLO)
		{
			BOOL bIsDifferent = TRUE;
			CTimeSpan durSound;

			HRESULT hr = LoadWav(FILENAMES[2], durSound, bIsDifferent);

			Assert::AreEqual(FALSE, bIsDifferent);
			Assert::IsTrue(SUCCEEDED(hr));
			Assert::AreEqual(5l, durSound.GetSeconds());
		}

		TEST_METHOD(TestLoadSIREN1)
		{
			BOOL bIsDifferent = TRUE;
			CTimeSpan durSound;

			HRESULT hr = LoadWav(FILENAMES[3], durSound, bIsDifferent);

			Assert::AreEqual(FALSE, bIsDifferent);
			Assert::IsTrue(SUCCEEDED(hr));
			Assert::AreEqual(3l, durSound.GetSeconds());
		}

		TEST_METHOD(TestLoadEQUITATION)
		{
			BOOL bIsDifferent = TRUE;
			CTimeSpan durSound;

			HRESULT hr = LoadWav(FILENAMES[4], durSound, bIsDifferent);

			Assert::AreEqual(FALSE, bIsDifferent);
			Assert::IsFalse(SUCCEEDED(hr));
			Assert::AreEqual(0l, durSound.GetSeconds());
		}

	private:

		static constexpr LPCTSTR FILENAMES[] = { _T("res\\BOURB1M1.WAV"), _T("res\\GRPLAFF1.WAV"), _T("res\\HELLO.WAV"), _T("res\\SIREN1.WAV"), _T("res\\EQUITATION.PNG") };
		static constexpr const INT_PTR FILENAME_COUNT = ARRAYSIZE(FILENAMES);

	private:

		HRESULT LoadWav(LPCTSTR pszFileName, CTimeSpan& durSound, BOOL& bIsMemDifferent)
		{
#ifdef _DEBUG
			_CrtMemState State1;
			_CrtMemState State2;
			_CrtMemState State3;

			_CrtMemCheckpoint(&State1);
#endif

			HRESULT hr = S_OK;
			

			{
				scion::engine::sfx::CSoundBuffer SoundBuffer;

				hr = SoundBuffer.LoadFromFile(pszFileName);

				durSound = SoundBuffer.GetDuration();

				SoundBuffer.Unload();
			}

#ifdef _DEBUG
			_CrtMemCheckpoint(&State2);

			bIsMemDifferent = _CrtMemDifference(&State3, &State1, &State2);
#endif

			return hr;
		}

	};
}
