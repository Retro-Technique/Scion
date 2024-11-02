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
			LONG nFrameCount = 0;
			CTimeSpan durVideo;
			
			HRESULT hr = LoadVideo(FILENAMES[0], fFrameRate, durVideo, nFrameCount, bIsDifferent);

			Assert::AreEqual(FALSE, bIsDifferent, _T("Memory leak"));
			Assert::IsTrue(SUCCEEDED(hr), _com_error(hr).ErrorMessage());
			Assert::AreEqual(30.f, fFrameRate, 0.1f, _T("Invalid framerate"));
			Assert::AreEqual(30l, durVideo.GetSeconds(), _T("Invalid duration"));
			Assert::AreEqual(901l, nFrameCount, _T("Invalid frame count"));
		}

		TEST_METHOD(TestLoadDEADCITY)
		{
			BOOL bIsDifferent = FALSE;
			FLOAT fFrameRate = 0.f;
			LONG nFrameCount = 0;
			CTimeSpan durVideo;

			HRESULT hr = LoadVideo(FILENAMES[1], fFrameRate, durVideo, nFrameCount, bIsDifferent);

			Assert::AreEqual(FALSE, bIsDifferent, _T("Memory leak"));
			Assert::IsTrue(SUCCEEDED(hr), _com_error(hr).ErrorMessage());
			Assert::AreEqual(25.f, fFrameRate, 0.1f, _T("Invalid framerate"));
			Assert::AreEqual(57l, durVideo.GetSeconds(), _T("Invalid duration"));
			Assert::AreEqual(1427l, nFrameCount, _T("Invalid frame count"));
		}

	private:

		static constexpr LPCTSTR FILENAMES[] = { _T("res\\PLANET.AVI"), _T("res\\DEADCITY.AVI") };
		static constexpr const INT_PTR FILENAME_COUNT = ARRAYSIZE(FILENAMES);

	private:

		HRESULT LoadVideo(LPCTSTR pszFileName, FLOAT& fFrameRate, CTimeSpan& durVideo, LONG& nFrameCount, BOOL& bIsMemDifferent)
		{
			HRESULT hr = S_OK;
			scion::common::CMemoryLeakChecker MemoryLeakChecker;
			scion::engine::vfx::IVideoManager* pVideoManager = NULL;
			scion::engine::vfx::IVideoBuffer* pVideoBuffer = NULL;

			do
			{
				Logger::WriteMessage("[LoadVideoBuffer] MemoryLeakChecker Begin...");

				MemoryLeakChecker.Begin();

				Logger::WriteMessage("[LoadVideoBuffer] CreateVideoManager...");

				if (hr = scion::engine::vfx::CreateVideoManager(&pVideoManager); FAILED(hr))
				{
					break;
				}

				Logger::WriteMessage("[LoadVideoBuffer] Initialize...");

				if (hr = pVideoManager->Initialize(); FAILED(hr))
				{
					break;
				}

				Logger::WriteMessage("[LoadVideoBuffer] CreateVideoBuffer...");

				if (hr = pVideoManager->CreateVideoBuffer(&pVideoBuffer); FAILED(hr))
				{
					break;
				}

				Logger::WriteMessage("[LoadVideoBuffer] OpenFromFile...");

				if (hr = pVideoBuffer->OpenFromFile(pszFileName); FAILED(hr))
				{
					break;
				}

				fFrameRate = pVideoBuffer->GetFrameRate();
				durVideo = pVideoBuffer->GetDuration();
				nFrameCount = pVideoBuffer->GetFrameCount();

				Logger::WriteMessage("[LoadVideoBuffer] Done");

			} while (SCION_NULL_WHILE_LOOP_CONDITION);

			if (pVideoBuffer)
			{
				pVideoBuffer->Close();
				pVideoBuffer->Release();
				pVideoBuffer = NULL;
			}

			Logger::WriteMessage("[LoadVideoBuffer] VideoBuffer closed");

			if (pVideoManager)
			{
				pVideoManager->Quit();
				pVideoManager->Release();
				pVideoManager = NULL;
			}

			Logger::WriteMessage("[LoadVideoBuffer] VideoManager released");

			bIsMemDifferent = MemoryLeakChecker.End();

			return hr;
		}

	};
}
