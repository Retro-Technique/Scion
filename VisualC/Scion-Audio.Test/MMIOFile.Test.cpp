#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ScionAudioTest
{
	TEST_CLASS(MMIOFileTest)
	{
	public:

		MMIOFileTest()
		{			
			SetCurrentDirectory(_T("..\\..\\Scion-Audio.Test"));
		}
		
		TEST_METHOD(TestLoadBOURB1M1)
		{			
			BOOL bIsDifferent = TRUE;

			HRESULT hr = LoadWav(FILENAMES[0], bIsDifferent);

			Assert::AreEqual(FALSE, bIsDifferent);
			Assert::IsTrue(SUCCEEDED(hr));
		}

		TEST_METHOD(TestLoadGRPLAFF1)
		{
			BOOL bIsDifferent = TRUE;

			HRESULT hr = LoadWav(FILENAMES[1], bIsDifferent);

			Assert::AreEqual(FALSE, bIsDifferent);
			Assert::IsTrue(SUCCEEDED(hr));
		}

		TEST_METHOD(TestLoadHELLO)
		{
			BOOL bIsDifferent = TRUE;

			HRESULT hr = LoadWav(FILENAMES[2], bIsDifferent);

			Assert::AreEqual(FALSE, bIsDifferent);
			Assert::IsTrue(SUCCEEDED(hr));
		}

		TEST_METHOD(TestLoadSIREN1)
		{
			BOOL bIsDifferent = TRUE;

			HRESULT hr = LoadWav(FILENAMES[3], bIsDifferent);

			Assert::AreEqual(FALSE, bIsDifferent);
			Assert::IsTrue(SUCCEEDED(hr));
		}

		TEST_METHOD(TestLoadEQUITATION)
		{
			BOOL bIsDifferent = TRUE;

			HRESULT hr = LoadWav(FILENAMES[4], bIsDifferent);

			Assert::AreEqual(FALSE, bIsDifferent);
			Assert::IsFalse(SUCCEEDED(hr));
		}

	private:

		static constexpr LPCTSTR FILENAMES[] = { _T("res\\BOURB1M1.WAV"), _T("res\\GRPLAFF1.WAV"), _T("res\\HELLO.WAV"), _T("res\\SIREN1.WAV"), _T("res\\EQUITATION.PNG") };
		static constexpr const INT_PTR FILENAME_COUNT = ARRAYSIZE(FILENAMES);

	private:

		HRESULT LoadWav(LPCTSTR pszFileName, BOOL& bIsMemDifferent)
		{
#ifdef _DEBUG
			_CrtMemState State1;
			_CrtMemState State2;
			_CrtMemState State3;

			_CrtMemCheckpoint(&State1);
#endif

			HRESULT hr = S_OK;

			{
				scion::engine::sfx::CMMIOFile MMIOFile;

				hr = MMIOFile.LoadFromFile(pszFileName);

				MMIOFile.Unload();
			}

#ifdef _DEBUG
			_CrtMemCheckpoint(&State2);

			bIsMemDifferent = _CrtMemDifference(&State3, &State1, &State2);
#endif

			return hr;
		}

	};
}
