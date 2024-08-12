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
#include "AudioManagerImpl.h"

namespace scion
{
	namespace engine
	{
		namespace sfx
		{
			namespace priv
			{

#pragma region Constructors

				CAudioManagerImpl CAudioManagerImpl::ms_Instance;

				IMPLEMENT_DYNAMIC(CAudioManagerImpl, CObject)

				CAudioManagerImpl::CAudioManagerImpl()
					: m_pDevice(NULL)
					, m_pPrimaryBuffer(NULL)
				{
				
				}

				CAudioManagerImpl::~CAudioManagerImpl()
				{
					
				}

#pragma endregion
#pragma region Operations

				CAudioManagerImpl& CAudioManagerImpl::GetInstance()
				{
					return ms_Instance;
				}

				HRESULT CAudioManagerImpl::Initialize(CWnd* pWnd)
				{
					ASSERT_POINTER(pWnd, CWnd);
					ASSERT_VALID(pWnd);

					HRESULT hr = S_OK;

					do
					{
						hr = CreateDevice(pWnd);
						if (FAILED(hr))
						{
							break;
						}

						hr = CreatePrimaryBuffer();
						if (FAILED(hr))
						{
							break;
						}

						hr = StartThread();
						if (FAILED(hr))
						{
							break;
						}

					} while (SCION_NULL_WHILE_LOOP_CONDITION);

					if (FAILED(hr))
					{
						Quit();
					}

					return hr;
				}

				HRESULT CAudioManagerImpl::CreateSecondaryBuffer(LPCDSBUFFERDESC pBufferDesc, LPDIRECTSOUNDBUFFER* ppBuffer)
				{
					if (m_pDevice)
					{
						return m_pDevice->CreateSoundBuffer(pBufferDesc, ppBuffer, NULL);
					}

					return E_FAIL;
				}

				void CAudioManagerImpl::Quit()
				{
					StopThread();
					DestroyPrimaryBuffer();
					DestroyDevice();
				}

#pragma endregion
#pragma region Overridables

#if defined(_DEBUG) || defined(_AFXDLL)

				void CAudioManagerImpl::AssertValid() const
				{
					CObject::AssertValid();

				}

				void CAudioManagerImpl::Dump(CDumpContext& dc) const
				{
					CObject::Dump(dc);
				}

#endif

#pragma endregion
#pragma region Implementations

				HRESULT CAudioManagerImpl::CreateDevice(CWnd* pWnd)
				{
					HRESULT hr = S_OK;

					do
					{
						hr = DirectSoundCreate(NULL, &m_pDevice, NULL);
						if (FAILED(hr))
						{
							break;
						}

						hr = m_pDevice->SetCooperativeLevel(pWnd->GetSafeHwnd(), DSSCL_PRIORITY);
						if (FAILED(hr))
						{
							break;
						}

					} while (SCION_NULL_WHILE_LOOP_CONDITION);

					return hr;
				}

				HRESULT CAudioManagerImpl::CreatePrimaryBuffer()
				{
					HRESULT hr = S_OK;

					do
					{
						DSBUFFERDESC DSBufferDesc = { 0 };
						DSBufferDesc.dwSize = sizeof(DSBUFFERDESC);
						DSBufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;

						hr = m_pDevice->CreateSoundBuffer(&DSBufferDesc, &m_pPrimaryBuffer, NULL);
						if (FAILED(hr))
						{
							break;
						}

						WAVEFORMATEX DSWaveFormat = { 0 };
						DSWaveFormat.wFormatTag = WAVE_FORMAT_PCM;
						DSWaveFormat.nChannels = DEFAULT_CHANNEL_COUNT;
						DSWaveFormat.nSamplesPerSec = DEFAULT_FREQUENCY;
						DSWaveFormat.wBitsPerSample = DEFAULT_FORMAT;
						DSWaveFormat.nBlockAlign = (DSWaveFormat.nChannels * DSWaveFormat.wBitsPerSample) / 8; // divis� par 8 pour passer de bits � bytes
						DSWaveFormat.nAvgBytesPerSec = DSWaveFormat.nSamplesPerSec * DSWaveFormat.nBlockAlign;

						hr = m_pPrimaryBuffer->SetFormat(&DSWaveFormat);
						if (FAILED(hr))
						{
							break;
						}

						hr = m_pPrimaryBuffer->Play(0, 0, DSBPLAY_LOOPING);
						if (FAILED(hr))
						{
							break;
						}

					} while (SCION_NULL_WHILE_LOOP_CONDITION);

					return hr;
				}

				HRESULT CAudioManagerImpl::StartThread()
				{
					m_evAudioLoopExit.ResetEvent();

					LPCVOID pThread = AfxBeginThread(AudioThreadProc, this);
					if (!pThread)
					{
						return E_FAIL;
					}

					return S_OK;
				}

				void CAudioManagerImpl::StopThread()
				{
					m_evAudioLoopExit.SetEvent();
				}

				void CAudioManagerImpl::DestroyPrimaryBuffer()
				{
					if (m_pPrimaryBuffer)
					{
						m_pPrimaryBuffer->Release();
						m_pPrimaryBuffer = NULL;
					}
				}

				void CAudioManagerImpl::DestroyDevice()
				{
					if (m_pDevice)
					{
						m_pDevice->Release();
						m_pDevice = NULL;
					}
				}

				void CAudioManagerImpl::OnAudioLoop()
				{
					/*if (m_mapSoundBuffers.IsEmpty())
					{
						return;
					}

					POSITION pos = m_mapSoundBuffers.GetStartPosition();
					while (pos)
					{
						CString strKey;
						CSoundBuffer* pSoundBuffer = NULL;

						m_mapSoundBuffers.GetNextAssoc(pos, strKey, pSoundBuffer);
						if (pSoundBuffer)
						{

						}
					}*/
				}

				UINT CAudioManagerImpl::AudioThreadProc(LPVOID pData)
				{
					/*CAudioManager* pAudioManager = reinterpret_cast<CAudioManager*>(pData);
					BOOL bIsRunning = TRUE;

					while (bIsRunning)
					{
						DWORD uResult = WaitForSingleObject(pAudioManager->GetExitEvent(), 1000);

						switch (uResult)
						{
						case WAIT_TIMEOUT:
							pAudioManager->OnAudioLoop();
							break;
						case WAIT_OBJECT_0:
						default:
							bIsRunning = FALSE;
							break;
						}
					}*/

					return 0;
				}

#pragma endregion

			}
		}
	}
}