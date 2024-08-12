#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ScionGraphicsTest
{
	TEST_CLASS(MatrixStackTest)
	{
	public:
		
		TEST_METHOD(TestAllocate)
		{
			BOOL bIsDifferent = TRUE;

			HRESULT hr = Allocate(8, bIsDifferent);
			
			Assert::AreEqual(FALSE, bIsDifferent);
			Assert::IsTrue(SUCCEEDED(hr));
		}

		TEST_METHOD(TestReallocate)
		{
			BOOL bIsDifferent = TRUE;

			HRESULT hr = Allocate(24, bIsDifferent);

			Assert::AreEqual(FALSE, bIsDifferent);
			Assert::IsTrue(SUCCEEDED(hr));
		}

	private:

		HRESULT Allocate(INT nPushCount, BOOL& bIsMemDifferent)
		{
#ifdef _DEBUG
			_CrtMemState State1;
			_CrtMemState State2;
			_CrtMemState State3;

			_CrtMemCheckpoint(&State1);
#endif

			{
				scion::engine::gfx::CMatrixStack MatrixStack;

				for (INT i = 1; i <= nPushCount; i++)
				{
					const FLOAT fVal = static_cast<FLOAT>(i);
					D2D1::Matrix3x2F* pTop = const_cast<D2D1::Matrix3x2F*>(MatrixStack.GetTop());

					pTop->m11 = fVal;
					pTop->m12 = fVal;
					pTop->m21 = fVal;
					pTop->m22 = fVal;
					pTop->dx = fVal;
					pTop->dy = fVal;

					MatrixStack.Push();
				}

				for (INT i = nPushCount; i >= 1; i--)
				{
					const FLOAT fVal = static_cast<FLOAT>(i);

					MatrixStack.Pop();

					D2D1::Matrix3x2F mMatrix(fVal, fVal, fVal, fVal, fVal, fVal);
					const D2D1::Matrix3x2F* pTop = MatrixStack.GetTop();

					const INT nCompare = memcmp(&mMatrix, pTop, sizeof(D2D1::Matrix3x2F));
					if (0 != nCompare)
					{
						return E_FAIL;
					}
				}
			}

#ifdef _DEBUG
			_CrtMemCheckpoint(&State2);

			bIsMemDifferent = _CrtMemDifference(&State3, &State1, &State2);
#endif

			return S_OK;
		}

	};
}
