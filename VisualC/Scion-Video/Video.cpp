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
#include "Video.h"
#include "VideoManager.h"
#include "VideoBuffer.h"

namespace scion
{
	namespace engine
	{
		namespace vfx
		{

#pragma region Constructors

			IMPLEMENT_DYNAMIC(CVideo, CObject)

			CVideo::CVideo(CVideoManager* pVideoManager)
				: m_nRef(1)
				, m_pVideoManager(pVideoManager)
				, m_pVideoBuffer(NULL)
				, m_bIsPlaying(FALSE)
				, m_pfnStreamProc(NULL)
				, m_pData(NULL)
			{
				m_pVideoManager->AddRef();
			}

			CVideo::~CVideo()
			{
				Unload();

				if (m_pVideoManager)
				{
					m_pVideoManager->Release();
					m_pVideoManager = NULL;
				}
			}

#pragma endregion
#pragma region Operations

			HRESULT CVideo::LoadFromBuffer(const IVideoBuffer* pVideoBuffer)
			{
				ASSERT_POINTER(pVideoBuffer, IVideoBuffer);
				
				m_pVideoBuffer = static_cast<const CVideoBuffer*>(pVideoBuffer);
				m_pVideoBuffer->AddRef();

				return S_OK;
			}

			void CVideo::Unload()
			{
				Stop();

				if (m_pVideoBuffer)
				{
					m_pVideoBuffer->Release();
					m_pVideoBuffer = NULL;
				}
			}

			void CVideo::SetStreamCallback(STREAMPROC pfnStreamProc, LPVOID pData)
			{
				ASSERT_POINTER(pfnStreamProc, STREAMPROC);

				m_pfnStreamProc = pfnStreamProc;
				m_pData = pData;
			}

			HRESULT CVideo::Play()
			{
				m_evVideoLoopExit.ResetEvent();

				if (LPCVOID pThread = AfxBeginThread(VideoThreadProc, this); !pThread)
				{
					return E_FAIL;
				}

				m_bIsPlaying = TRUE;

				return S_OK;
			}

			void CVideo::Stop()
			{
				m_evVideoLoopExit.SetEvent();

				m_bIsPlaying = FALSE;
			}

			BOOL CVideo::IsPlaying() const
			{
				return m_bIsPlaying;
			}

#pragma endregion
#pragma region Overridables

#ifdef _DEBUG

			void CVideo::AssertValid() const
			{
				CObject::AssertValid();

				ASSERT_POINTER(m_pVideoManager, CVideoManager);
				ASSERT_VALID(m_pVideoManager);
				ASSERT_POINTER(m_pVideoBuffer, CVideoBuffer);
				ASSERT_VALID(m_pVideoBuffer);
			}

			void CVideo::Dump(CDumpContext& dc) const
			{
				CObject::Dump(dc);

				dc << _T("IsPlaying? ") << IsPlaying() << _T("\n");
			}

#endif

			void CVideo::AddRef() const
			{
				InterlockedIncrement(&m_nRef);
			}

			BOOL CVideo::Release() const
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
#pragma region Implementations

			UINT CVideo::VideoThreadProc(LPVOID pData)
			{
				ASSERT(pData);

				CVideo* pVideo = reinterpret_cast<CVideo*>(pData);

				BOOL bIsRunning = TRUE;
				const LONG nFirstFrameIndex = pVideo->m_pVideoBuffer->GetFirstFrameIndex();
				const LONG nFrameCount = pVideo->m_pVideoBuffer->GetFrameCount();
				const FLOAT fFrameRateMs = pVideo->m_pVideoBuffer->GetFrameRate();
				const DWORD uFrameRateMs = static_cast<const DWORD>(fFrameRateMs);

				for (LONG i = nFirstFrameIndex; i < nFrameCount; i++)
				{
					const LONG nIndex = i - nFirstFrameIndex;
					const DWORD uResult = WaitForSingleObject(pVideo->GetExitEvent(), uFrameRateMs);
					switch (uResult)
					{
					case WAIT_TIMEOUT:
					{
						const LPBYTE pBuffer = pVideo->m_pVideoBuffer->GetFrameBuffer(nIndex);
						if (pVideo->m_pfnStreamProc)
						{
							pVideo->m_pfnStreamProc(pBuffer, pVideo->m_pData);
						}
					}
						break;
					case WAIT_OBJECT_0:
					default:
						bIsRunning = FALSE;
						break;
					}

					if (!bIsRunning)
					{
						break;
					}
				}

				pVideo->m_bIsPlaying = FALSE;

				return 0;
			}

#pragma endregion

		}
	}
}