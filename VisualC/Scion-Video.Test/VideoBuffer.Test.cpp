#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ScionVideoTest
{
	TEST_CLASS(VideoBufferTest)
	{
	public:
		
		VideoBufferTest()
		{
			SetCurrentDirectory(_T("..\\..\\Scion-Video.Test"));
		}

		TEST_METHOD(TestLoadPLANET)
		{
			BOOL bIsDifferent = FALSE;
			FLOAT fFrameRate = 0.f;
			CTimeSpan durVideo;
			
			HRESULT hr = LoadWav(FILENAMES[0], fFrameRate, durVideo, bIsDifferent);

			Assert::AreEqual(FALSE, bIsDifferent, _T("Memory leak"));
			Assert::IsTrue(SUCCEEDED(hr), _com_error(hr).ErrorMessage());
			Assert::AreEqual(30.f, fFrameRate, 0.1f, _T("Invalid framerate"));
			Assert::AreEqual(30l, durVideo.GetSeconds(), _T("Invalid duration"));
		}

		TEST_METHOD(TestLoadDEADCITY)
		{
			BOOL bIsDifferent = FALSE;
			FLOAT fFrameRate = 0.f;
			CTimeSpan durVideo;

			HRESULT hr = LoadWav(FILENAMES[1], fFrameRate, durVideo, bIsDifferent);

			Assert::AreEqual(FALSE, bIsDifferent, _T("Memory leak"));
			Assert::IsTrue(SUCCEEDED(hr), _com_error(hr).ErrorMessage());
			Assert::AreEqual(25.f, fFrameRate, 0.1f, _T("Invalid framerate"));
			Assert::AreEqual(57l, durVideo.GetSeconds(), _T("Invalid duration"));
		}

	private:

		static constexpr LPCTSTR FILENAMES[] = { _T("res\\PLANET.AVI"), _T("res\\DEADCITY.AVI") };
		static constexpr const INT_PTR FILENAME_COUNT = ARRAYSIZE(FILENAMES);

	private:

		HRESULT LoadWav(LPCTSTR pszFileName, FLOAT& fFrameRate, CTimeSpan& durVideo, BOOL& bIsMemDifferent)
		{
#ifdef _DEBUG
			_CrtMemState State1;
			_CrtMemState State2;
			_CrtMemState State3;

			_CrtMemCheckpoint(&State1);
#endif

			HRESULT hr = S_OK;
			scion::engine::vfx::IVideoManager* pVideoManager = NULL;
			scion::engine::vfx::IVideoBuffer* pVideoBuffer = NULL;

			do
			{
				if (hr = scion::engine::vfx::CreateVideoManager(&pVideoManager); FAILED(hr))
				{
					break;
				}

				if (hr = pVideoManager->CreateVideoBuffer(&pVideoBuffer); FAILED(hr))
				{
					break;
				}

				if (hr = pVideoBuffer->OpenFromFile(pszFileName); FAILED(hr))
				{
					break;
				}

				fFrameRate = pVideoBuffer->GetFrameRate();
				durVideo = pVideoBuffer->GetDuration();

			} while (SCION_NULL_WHILE_LOOP_CONDITION);

			if (pVideoBuffer)
			{
				pVideoBuffer->Close();
				pVideoBuffer->Release();
				pVideoBuffer = NULL;
			}

			if (pVideoManager)
			{
				pVideoManager->Quit();
				pVideoManager->Release();
				pVideoManager = NULL;
			}

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
