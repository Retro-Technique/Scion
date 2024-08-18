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
#include "VideoBuffer.h"
#include "VideoManager.h"

namespace scion
{
	namespace engine
	{
		namespace vfx
		{

#pragma region Constructors

			IMPLEMENT_DYNAMIC(CVideoBuffer, CObject)

			CVideoBuffer::CVideoBuffer(CVideoManager* pVideoManager)
				: m_nRef(1)
				, m_pVideoManager(pVideoManager)
				, m_pAviFile(NULL)
				, m_pAviStream(NULL)
			{
				m_pVideoManager->AddRef();
			}

			CVideoBuffer::~CVideoBuffer()
			{
				Close();

				if (m_pVideoManager)
				{
					m_pVideoManager->Release();
					m_pVideoManager = NULL;
				}
			}

#pragma endregion
#pragma region Operations

			HRESULT CVideoBuffer::OpenFromFile(LPCTSTR pszFileName)
			{
				ASSERT_VALID(this);
				ASSERT(AfxIsValidString(pszFileName, MAX_PATH));

				Close();

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

					/*PGETFRAME pGetFrame = AVIStreamGetFrameOpen(m_pAviStream, NULL);
					if (!pGetFrame)
					{
						hr = E_AVI_CANNOTFINDDECOMPRESSOR;
						break;
					}

					AVIStreamGetFrameClose(pGetFrame);*/

				} while (SCION_NULL_WHILE_LOOP_CONDITION);

				if (FAILED(hr))
				{
					Close();
				}

				return hr;
			}

			FLOAT CVideoBuffer::GetFrameRate() const
			{
				if (!m_pAviFile)
				{
					return 0.f;
				}

				AVISTREAMINFO aviStreamInfo = { 0 };
				HRESULT hr = AVIStreamInfo(m_pAviStream, &aviStreamInfo, sizeof(AVISTREAMINFO));
				if (FAILED(hr))
				{
					return 0.f;
				}

				return static_cast<FLOAT>(aviStreamInfo.dwRate) / aviStreamInfo.dwScale;
			}

			CTimeSpan CVideoBuffer::GetDuration() const
			{
				if (!m_pAviFile)
				{
					return CTimeSpan();
				}

				AVISTREAMINFO aviStreamInfo = { 0 };
				HRESULT hr = AVIStreamInfo(m_pAviStream, &aviStreamInfo, sizeof(AVISTREAMINFO));
				if (FAILED(hr))
				{
					return CTimeSpan();
				}

				const FLOAT fSeconds = static_cast<FLOAT>(aviStreamInfo.dwLength) / GetFrameRate();
				const INT nSeconds = static_cast<INT>(fSeconds);

				return CTimeSpan(0, 0, 0, nSeconds);
			}

			void CVideoBuffer::Close()
			{
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

#ifdef _DEBUG

			void CVideoBuffer::AssertValid() const
			{
				CObject::AssertValid();

				ASSERT_POINTER(m_pVideoManager, CVideoManager);
				ASSERT_VALID(m_pVideoManager);
			}

			void CVideoBuffer::Dump(CDumpContext& dc) const
			{
				CObject::Dump(dc);

				dc << _T("FrameRate: ") << GetFrameRate() << _T("\n");
				dc << _T("Duration: ") << GetDuration() << _T("\n");
			}

#endif

			void CVideoBuffer::AddRef() const
			{
				InterlockedIncrement(&m_nRef);
			}

			BOOL CVideoBuffer::Release() const
			{
				const LONG nRefCount = InterlockedDecrement(&m_nRef);
				if (0l == nRefCount)
				{
					delete this;
					return TRUE;
				}

				return FALSE;
			}

#pragma endregion

		}
	}
}