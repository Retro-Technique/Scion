#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ScionAudioTest
{
	TEST_CLASS(SoundBufferTest)
	{
	public:

		SoundBufferTest()
		{
			SetCurrentDirectory(_T("..\\..\\Scion-Audio.Test"));
		}

		TEST_METHOD(TestLoadBOURB1M1)
		{
			BOOL bIsDifferent = FALSE;
			CTimeSpan durSound;

			HRESULT hr = LoadWav(FILENAMES[0], durSound, bIsDifferent);

			Assert::AreEqual(FALSE, bIsDifferent, _T("Memory leak"));
			Assert::IsTrue(SUCCEEDED(hr), _com_error(hr).ErrorMessage());
			Assert::AreEqual(17l, durSound.GetSeconds(), _T("Invalid duration"));
		}

		TEST_METHOD(TestLoadGRPLAFF1)
		{
			BOOL bIsDifferent = FALSE;
			CTimeSpan durSound;

			HRESULT hr = LoadWav(FILENAMES[1], durSound, bIsDifferent);

			Assert::AreEqual(FALSE, bIsDifferent, _T("Memory leak"));
			Assert::IsTrue(SUCCEEDED(hr), _com_error(hr).ErrorMessage());
			Assert::AreEqual(2l, durSound.GetSeconds(), _T("Invalid duration"));
		}

		TEST_METHOD(TestLoadHELLO)
		{
			BOOL bIsDifferent = FALSE;
			CTimeSpan durSound;

			HRESULT hr = LoadWav(FILENAMES[2], durSound, bIsDifferent);

			Assert::AreEqual(FALSE, bIsDifferent, _T("Memory leak"));
			Assert::IsTrue(SUCCEEDED(hr), _com_error(hr).ErrorMessage());
			Assert::AreEqual(5l, durSound.GetSeconds(), _T("Invalid duration"));
		}

		TEST_METHOD(TestLoadSIREN1)
		{
			BOOL bIsDifferent = FALSE;
			CTimeSpan durSound;

			HRESULT hr = LoadWav(FILENAMES[3], durSound, bIsDifferent);

			Assert::AreEqual(FALSE, bIsDifferent, _T("Memory leak"));
			Assert::IsTrue(SUCCEEDED(hr), _com_error(hr).ErrorMessage());
			Assert::AreEqual(3l, durSound.GetSeconds(), _T("Invalid duration"));
		}

		TEST_METHOD(TestLoadEQUITATION)
		{
			BOOL bIsDifferent = FALSE;
			CTimeSpan durSound;

			HRESULT hr = LoadWav(FILENAMES[4], durSound, bIsDifferent);

			Assert::AreEqual(FALSE, bIsDifferent, _T("Memory leak"));
			Assert::IsFalse(SUCCEEDED(hr), _com_error(hr).ErrorMessage());
			Assert::AreEqual(0l, durSound.GetSeconds(), _T("Invalid duration"));
		}

	private:

		static constexpr LPCTSTR FILENAMES[] = { _T("res\\BOURB1M1.WAV"), _T("res\\GRPLAFF1.WAV"), _T("res\\HELLO.WAV"), _T("res\\SIREN1.WAV"), _T("res\\EQUITATION.PNG") };
		static constexpr const INT_PTR FILENAME_COUNT = ARRAYSIZE(FILENAMES);

	private:

		HRESULT LoadWav(LPCTSTR pszFileName, CTimeSpan& durSound, BOOL& bIsMemDifferent)
		{
			HRESULT hr = S_OK;
			scion::common::CMemoryLeakChecker MemoryLeakChecker;
			scion::engine::sfx::IAudioManager* pAudioManager = NULL;
			scion::engine::sfx::ISoundBuffer* pSoundBuffer = NULL;

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

				if (hr = pAudioManager->CreateSoundBuffer(&pSoundBuffer); FAILED(hr))
				{
					break;
				}

				if (hr = pSoundBuffer->LoadFromFile(pszFileName); FAILED(hr))
				{
					break;
				}

				durSound = pSoundBuffer->GetDuration();

			} while (SCION_NULL_WHILE_LOOP_CONDITION);

			if (pSoundBuffer)
			{
				pSoundBuffer->Unload();
				pSoundBuffer->Release();
				pSoundBuffer = NULL;
			}

			if (pAudioManager)
			{
				pAudioManager->Quit();
				pAudioManager->Release();
				pAudioManager = NULL;
			}

			bIsMemDifferent = MemoryLeakChecker.End();

			return hr;
		}

	};
}
