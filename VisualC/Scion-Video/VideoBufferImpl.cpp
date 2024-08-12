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
			namespace priv
			{

#pragma region Constructors

				IMPLEMENT_DYNAMIC(CVideoBufferImpl, CObject)

				CVideoBufferImpl::CVideoBufferImpl()
					: m_pAviFile(NULL)
					, m_pAviStream(NULL)
					, m_aviStreamInfo { 0 }
				{

				}

				CVideoBufferImpl::~CVideoBufferImpl()
				{
					Unload();
				}

#pragma endregion
#pragma region Operations

				HRESULT CVideoBufferImpl::LoadFromFile(LPCTSTR pszFileName)
				{
					ASSERT(AfxIsValidString(pszFileName, MAX_PATH));

					Unload();

					HRESULT hr = S_OK;

					do
					{
						hr = AVIFileOpen(&m_pAviFile, pszFileName, OF_READ, NULL);
						if (FAILED(hr))
						{
							break;
						}

						hr = AVIFileGetStream(m_pAviFile, &m_pAviStream, streamtypeVIDEO, 0);
						if (FAILED(hr))
						{
							break;
						}

						hr = AVIStreamInfo(m_pAviStream, &m_aviStreamInfo, sizeof(AVISTREAMINFO));
						if (FAILED(hr))
						{
							break;
						}

						PGETFRAME pGetFrame = AVIStreamGetFrameOpen(m_pAviStream, NULL);
						if (!pGetFrame)
						{
							hr = E_FAIL;
							break;
						}

						AVIStreamGetFrameClose(pGetFrame);

					} while (FALSE);

					if (FAILED(hr))
					{
						Unload();
					}

					return hr;
				}

				void CVideoBufferImpl::Unload()
				{
					ZeroMemory(&m_aviStreamInfo, sizeof(AVISTREAMINFO));

					if (m_pAviStream)
					{
						AVIStreamRelease(m_pAviStream);
						m_pAviStream = NULL;
					}

					if (m_pAviFile)
					{
						AVIFileRelease(m_pAviFile);
						m_pAviFile = NULL;
					}
				}

#pragma endregion
#pragma region Overridables

#if defined(_DEBUG) || defined(_AFXDLL)

				void CVideoBufferImpl::AssertValid() const
				{
					CObject::AssertValid();
				}

				void CVideoBufferImpl::Dump(CDumpContext& dc) const
				{
					CObject::Dump(dc);
				}

#endif

#pragma endregion
			}
		}
	}
}