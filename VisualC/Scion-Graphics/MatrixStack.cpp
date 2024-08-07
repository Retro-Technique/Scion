/**
 *
 * Scion
 *
 * CEA CNRS INRIA LOGICIEL LIBRE
 *
 * Copyright(c) 2014-2024 Retro Technique
 *
 * This software is a computer program whose purpose is to provide
 * a 2D game engine.
 *
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use,
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 *
 */

#include "pch.h"
#include "MatrixStack.h"

namespace scion
{
	namespace gfx
	{

#pragma region Constructors

		IMPLEMENT_DYNAMIC(CMatrixStack, CObject);

		CMatrixStack::CMatrixStack()
			: m_nSackSize(0)
			, m_nCurrent(0)
			, m_pStack(NULL)
		{
			Allocate(START_SIZE);
			LoadIdentity();
		}

		CMatrixStack::~CMatrixStack()
		{
			if (m_pStack)
			{
				_aligned_free(m_pStack);
				m_pStack = NULL;
			}
		}

#pragma endregion
#pragma region Operations

		const D2D1::Matrix3x2F CMatrixStack::Top() const
		{
			return m_pStack[m_nCurrent];
		}

		const D2D1::Matrix3x2F* CMatrixStack::GetTop() const
		{
			return &m_pStack[m_nCurrent];
		}

		INT_PTR CMatrixStack::GetSize() const
		{
			return m_nCurrent + 1;
		}

		void CMatrixStack::Pop()
		{
			if (m_nCurrent > 0)
			{
				m_nCurrent--;
			}
		}

		void CMatrixStack::Push()
		{
			m_nCurrent++;

			if (m_nCurrent >= m_nSackSize)
			{
				Allocate(m_nSackSize * 2);
			}

			m_pStack[m_nCurrent] = m_pStack[m_nCurrent - 1];
		}

		void CMatrixStack::LoadIdentity()
		{
			m_pStack[m_nCurrent] = D2D1::Matrix3x2F::Identity();
		}

		void CMatrixStack::LoadMatrix(const D2D1::Matrix3x2F& mMatrix)
		{
			m_pStack[m_nCurrent] = mMatrix;
		}

		void CMatrixStack::MultiplyMatrix(const D2D1::Matrix3x2F& mMatrix)
		{
			m_pStack[m_nCurrent] = m_pStack[m_nCurrent] * mMatrix;
		}

		void CMatrixStack::Rotate(FLOAT fAngle, const D2D1_POINT_2F& ptOrigin)
		{
			const D2D1::Matrix3x2F mMatrix = D2D1::Matrix3x2F::Rotation(fAngle, ptOrigin);
			MultiplyMatrix(mMatrix);
		}

		void CMatrixStack::Scale(FLOAT fScale, const D2D1_POINT_2F& ptOrigin)
		{
			const D2D1_SIZE_F vScale = D2D1::SizeF(fScale, fScale);
			const D2D1::Matrix3x2F mMatrix = D2D1::Matrix3x2F::Scale(vScale, ptOrigin);
			MultiplyMatrix(mMatrix);
		}

		void CMatrixStack::Translate(const D2D1_SIZE_F& szTranslation)
		{
			const D2D1::Matrix3x2F mMatrix = D2D1::Matrix3x2F::Translation(szTranslation);
			MultiplyMatrix(mMatrix);
		}

#pragma endregion
#pragma region Overridables

#if defined(_DEBUG) || defined(_AFXDLL)

		void CMatrixStack::AssertValid() const
		{
			CObject::AssertValid();

			ASSERT(m_nSackSize >= START_SIZE);
			ASSERT((m_nSackSize & (m_nSackSize - 1)) == 0); // vérifier qu'il s'agit bien d'une puissance de 2
			ASSERT(m_nCurrent >= 0);
			ASSERT_POINTER(m_pStack, D2D1::Matrix3x2F);
		}

		void CMatrixStack::Dump(CDumpContext& dc) const
		{
			CObject::Dump(dc);
			
			dc << _T("Stack Size: ") << m_nSackSize << _T("\n");
			dc << _T("Current : ") << m_nCurrent << _T("\n");
		}

#endif

#pragma endregion
#pragma region Implementations

		void CMatrixStack::Allocate(INT_PTR nNewSize)
		{
			ASSERT(nNewSize > 0);

			LPVOID pBuffer = _aligned_malloc(nNewSize + sizeof(D2D1::Matrix3x2F), START_SIZE);
			if (!pBuffer)
			{
				return;
			}

			if (m_pStack)
			{
				ASSERT(nNewSize >= m_nSackSize);
				CopyMemory(pBuffer, m_pStack, sizeof(D2D1::Matrix3x2F) * m_nSackSize);
			}

			m_pStack = reinterpret_cast<D2D1::Matrix3x2F*>(pBuffer);
			m_nSackSize = nNewSize;
		}

#pragma endregion

	}
}