#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ScionVideoTest
{
	TEST_CLASS(VideoBufferTest)
	{
	private:

		scion::engine::vfx::CVideoManager m_VideoManager;

	public:
		
		VideoBufferTest()
		{
			SetCurrentDirectory(_T("..\\..\\Scion-Video.Test"));

			m_VideoManager.Initialize();
		}

		~VideoBufferTest()
		{
			m_VideoManager.Quit();
		}

		TEST_METHOD(TestLoad)
		{
#ifdef _DEBUG
			_CrtMemState State1;
			_CrtMemState State2;
			_CrtMemState State3;

			_CrtMemCheckpoint(&State1);
#endif

			BOOL bIsMemDifferent = FALSE;
			FLOAT fFrameRate = 0.f;
			CTimeSpan durVideo;
			HRESULT hr = S_OK;

			{
				scion::engine::vfx::CVideoBuffer VideoBuffer;

				hr = VideoBuffer.LoadFromFile(_T("res\\PLANET.AVI"));

				fFrameRate = VideoBuffer.GetFrameRate();
				durVideo = VideoBuffer.GetDuration();

				VideoBuffer.Unload();
			}

#ifdef _DEBUG
			_CrtMemCheckpoint(&State2);

			bIsMemDifferent = _CrtMemDifference(&State3, &State1, &State2);
#endif

			Assert::AreEqual(FALSE, bIsMemDifferent);
			Assert::IsTrue(SUCCEEDED(hr));
			Assert::AreEqual(30.f, fFrameRate, 0.1f);
			Assert::AreEqual(30l, durVideo.GetSeconds());
		}

	};
}
