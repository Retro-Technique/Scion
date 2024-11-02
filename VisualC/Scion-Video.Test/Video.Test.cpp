#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ScionVideoTest
{
	TEST_CLASS(VideoTest)
	{
	public:
		
		VideoTest()
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
			scion::engine::vfx::IVideo* pVideo = NULL;

			do
			{
				MemoryLeakChecker.Begin();

				if (hr = scion::engine::vfx::CreateVideoManager(&pVideoManager); FAILED(hr))
				{
					break;
				}

				if (hr = pVideoManager->Initialize(); FAILED(hr))
				{
					break;
				}

				if (hr = pVideoManager->CreateVideoBuffer(&pVideoBuffer); FAILED(hr))
				{
					break;
				}

				if (hr = pVideoManager->CreateVideo(&pVideo); FAILED(hr))
				{
					break;
				}

				if (hr = pVideoBuffer->OpenFromFile(pszFileName); FAILED(hr))
				{
					break;
				}

				if (hr = pVideo->LoadFromBuffer(pVideoBuffer); FAILED(hr))
				{
					break;
				}

				pVideo->SetStreamCallback([](const LPBYTE pData, LPVOID) -> HRESULT
					{
						static LONG iFrame = 0;
						Assert::IsNotNull(pData);

						BITMAPINFOHEADER bih;
						MoveMemory(&bih.biSize, pData, sizeof(BITMAPINFOHEADER));

						Assert::IsTrue(bih.biSizeImage >= 1);

						BYTE* Bits = new BYTE[bih.biSizeImage];

						BYTE memBitmapInfo[40];
						MoveMemory(memBitmapInfo, &bih, sizeof(bih));

						BITMAPFILEHEADER bfh;
						bfh.bfType = 19778;    //BM header
						bfh.bfSize = 55 + bih.biSizeImage;
						bfh.bfReserved1 = 0;
						bfh.bfReserved2 = 0;
						bfh.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);

#if 0
						CString FileName;
						FileName.Format(_T("C:\\Users\\olivi\\AppData\\Local\\Temp\\Test\\Frame-%05d.bmp"), iFrame);
						iFrame++;

						CFileException ex;
						CFile File;
						BOOL bOpened = File.Open(FileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary, &ex);
						if (bOpened)
						{
							File.Write(&bfh, sizeof(bfh));
							File.Write(&memBitmapInfo, sizeof(memBitmapInfo));
							File.Write(Bits, bih.biSizeImage);

							File.Close();
						}
#endif

						delete[] Bits;

						return S_OK;
					});

				pVideo->Play();

				fFrameRate = pVideoBuffer->GetFrameRate();
				durVideo = pVideoBuffer->GetDuration();
				nFrameCount = pVideoBuffer->GetFrameCount();

				Sleep(durVideo.GetSeconds() * 1100);

			} while (SCION_NULL_WHILE_LOOP_CONDITION);

			if (pVideo)
			{
				pVideo->Stop();
				pVideo->Unload();
				pVideo->Release();
				pVideo = NULL;
			}
			
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

			bIsMemDifferent = MemoryLeakChecker.End();

			return hr;
		}

	};
}
