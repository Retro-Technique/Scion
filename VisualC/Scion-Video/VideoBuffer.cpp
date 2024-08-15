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
#include "VideoBufferImpl.h"

namespace scion
{
	namespace engine
	{
		namespace vfx
		{

#pragma region Constructors

			IMPLEMENT_DYNAMIC(CVideoBuffer, CObject)

			CVideoBuffer::CVideoBuffer()
				: m_pImpl(new priv::CVideoBufferImpl)
			{

			}

			CVideoBuffer::~CVideoBuffer()
			{
				delete m_pImpl;
			}

#pragma endregion
#pragma region Operations

			HRESULT CVideoBuffer::LoadFromFile(LPCTSTR pszFileName)
			{
				return m_pImpl->LoadFromFile(pszFileName);
			}

			BOOL CVideoBuffer::IsLoaded() const
			{
				return m_pImpl->IsLoaded();
			}

			FLOAT CVideoBuffer::GetFrameRate() const
			{
				return m_pImpl->GetFrameRate();
			}

			CTimeSpan CVideoBuffer::GetDuration() const
			{
				return m_pImpl->GetDuration();
			}

			void CVideoBuffer::Unload()
			{
				m_pImpl->Unload();
			}
		
#pragma endregion
#pragma region Overridables

#ifdef _DEBUG

			void CVideoBuffer::AssertValid() const
			{
				CObject::AssertValid();

				ASSERT_VALID(m_pImpl);
			}

			void CVideoBuffer::Dump(CDumpContext& dc) const
			{
				CObject::Dump(dc);

				AFXDUMP(m_pImpl);
			}

#endif

#pragma endregion
		}
	}
}