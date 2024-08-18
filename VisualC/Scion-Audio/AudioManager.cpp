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
#include "AudioManager.h"
#include "SoundBuffer.h"
#include "Sound.h"
#include "Listener.h"

namespace scion
{
	namespace engine
	{
		namespace sfx
		{

#pragma region Constructors

			HRESULT CreateAudioManager(IAudioManager** ppAudioManager)
			{
				ASSERT_NULL_OR_POINTER(*ppAudioManager, IAudioManager);

				IAudioManager* pAudioManager = new CAudioManager;
				if (!pAudioManager)
				{
					return E_OUTOFMEMORY;
				}

				*ppAudioManager = pAudioManager;

				return S_OK;
			}

			IMPLEMENT_DYNAMIC(CAudioManager, CObject)

			CAudioManager::CAudioManager()
				: m_nRef(1)
				, m_pDevice(NULL)
				, m_pPrimaryBuffer(NULL)
				, m_pListener(NULL)
			{

			}

			CAudioManager::~CAudioManager()
			{
				
			}

#pragma endregion
#pragma region Operations

			HRESULT CAudioManager::CreateSecondaryBuffer(LPCDSBUFFERDESC pBufferDesc, LPDIRECTSOUNDBUFFER* ppBuffer)
			{
				ASSERT_POINTER(pBufferDesc, DSBUFFERDESC);
				ASSERT_NULL_OR_POINTER(*ppBuffer, IDirectSoundBuffer);

				if (!m_pDevice)
				{
					return E_FAIL;
				}

				return m_pDevice->CreateSoundBuffer(pBufferDesc, ppBuffer, NULL);
			}

			HRESULT CAudioManager::DuplicateSecondaryBuffer(LPDIRECTSOUNDBUFFER pOriginalBuffer, LPDIRECTSOUNDBUFFER* ppDuplicateBuffer)
			{
				ASSERT_POINTER(m_pDevice, IDirectSound);
				ASSERT_POINTER(pOriginalBuffer, IDirectSoundBuffer);
				ASSERT_NULL_OR_POINTER(*ppDuplicateBuffer, IDirectSoundBuffer);

				return m_pDevice->DuplicateSoundBuffer(pOriginalBuffer, ppDuplicateBuffer);
			}

			HRESULT CAudioManager::SetListenerPosition(FLOAT x, FLOAT y, FLOAT z)
			{
				ASSERT_POINTER(m_pListener, IDirectSound3DListener8);

				return m_pListener->SetPosition(x, y, z, DS3D_DEFERRED);
			}

			HRESULT CAudioManager::GetListenerPosition(FLOAT& x, FLOAT& y, FLOAT& z)
			{
				ASSERT_POINTER(m_pListener, IDirectSound3DListener8);

				D3DVECTOR vPosition = { 0 };
				if (const HRESULT hr = m_pListener->GetPosition(&vPosition); FAILED(hr))
				{
					return hr;
				}

				x = vPosition.x;
				y = vPosition.y;
				z = vPosition.z;

				return S_OK;
			}

#pragma endregion
#pragma region Overridables

			HRESULT CAudioManager::Initialize(CWnd* pWnd)
			{
				ASSERT_POINTER(pWnd, CWnd);
				ASSERT_VALID(pWnd);

				HRESULT hr = S_OK;

				do
				{
					if (hr = CreateDevice(pWnd); FAILED(hr))
					{
						break;
					}

					if (hr = CreatePrimaryBuffer(); FAILED(hr))
					{
						break;
					}

					if (hr = CreateListener(); FAILED(hr))
					{
						break;
					}

					if (hr = StartThread(); FAILED(hr))
					{
						break;
					}

				} while (SCION_NULL_WHILE_LOOP_CONDITION);

				return hr;
			}

			void CAudioManager::Quit()
			{
				StopThread();
				DestroyListener();
				DestroyPrimaryBuffer();
				DestroyDevice();
			}

			HRESULT CAudioManager::CreateSoundBuffer(ISoundBuffer** ppSoundBuffer)
			{
				ASSERT_POINTER(this, CAudioManager);
				ASSERT_VALID(this);
				ASSERT_NULL_OR_POINTER(*ppSoundBuffer, ISoundBuffer);

				ISoundBuffer* pSoundBuffer = new CSoundBuffer(this);
				if (!pSoundBuffer)
				{
					return E_OUTOFMEMORY;
				}

				*ppSoundBuffer = pSoundBuffer;

				return S_OK;
			}

			HRESULT CAudioManager::CreateSound(ISound** ppSound)
			{
				ASSERT_POINTER(this, CAudioManager);
				ASSERT_VALID(this);
				ASSERT_NULL_OR_POINTER(*ppSound, ISound);

				ISound* pSound = new CSound(this);
				if (!pSound)
				{
					return E_OUTOFMEMORY;
				}

				*ppSound = pSound;

				return S_OK;
			}

			HRESULT CAudioManager::CreateListener(IListener** ppListener)
			{
				ASSERT_POINTER(this, CAudioManager);
				ASSERT_VALID(this);
				ASSERT_NULL_OR_POINTER(*ppListener, IListener);

				IListener* pListener = new CListener(this);
				if (!pListener)
				{
					return E_OUTOFMEMORY;
				}

				*ppListener = pListener;

				return S_OK;
			}

#ifdef _DEBUG

			void CAudioManager::AssertValid() const
			{
				CObject::AssertValid();

			}

			void CAudioManager::Dump(CDumpContext& dc) const
			{
				CObject::Dump(dc);
			}

#endif

			void CAudioManager::AddRef() const
			{
				InterlockedIncrement(&m_nRef);
			}

			BOOL CAudioManager::Release() const
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

			HRESULT CAudioManager::CreateDevice(CWnd* pWnd)
			{
				HRESULT hr = S_OK;

				do
				{
					if (hr = DirectSoundCreate8(NULL, &m_pDevice, NULL); FAILED(hr))
					{
						break;
					}

					if (hr = m_pDevice->SetCooperativeLevel(pWnd->GetSafeHwnd(), DSSCL_PRIORITY); FAILED(hr))
					{
						break;
					}

				} while (SCION_NULL_WHILE_LOOP_CONDITION);

				return hr;
			}

			HRESULT CAudioManager::CreatePrimaryBuffer()
			{
				HRESULT hr = S_OK;

				do
				{
					DSBUFFERDESC BufferDesc = { 0 };
					BufferDesc.dwSize = sizeof(DSBUFFERDESC);
					BufferDesc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;

					if (hr = m_pDevice->CreateSoundBuffer(&BufferDesc, &m_pPrimaryBuffer, NULL); FAILED(hr))
					{
						break;
					}

					WAVEFORMATEX WaveFormat = { 0 };
					WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
					WaveFormat.nChannels = DEFAULT_CHANNEL_COUNT;
					WaveFormat.nSamplesPerSec = DEFAULT_FREQUENCY;
					WaveFormat.wBitsPerSample = DEFAULT_FORMAT;
					WaveFormat.nBlockAlign = (WaveFormat.nChannels * WaveFormat.wBitsPerSample) / 8; // divis� par 8 pour passer de bits � bytes
					WaveFormat.nAvgBytesPerSec = WaveFormat.nSamplesPerSec * WaveFormat.nBlockAlign;

					if (hr = m_pPrimaryBuffer->SetFormat(&WaveFormat); FAILED(hr))
					{
						break;
					}

					if (hr = m_pPrimaryBuffer->Play(0, 0, DSBPLAY_LOOPING); FAILED(hr))
					{
						break;
					}

				} while (SCION_NULL_WHILE_LOOP_CONDITION);

				return hr;
			}

			HRESULT CAudioManager::CreateListener()
			{
				HRESULT hr = S_OK;

				do
				{
					if (hr = m_pPrimaryBuffer->QueryInterface(IID_IDirectSound3DListener8, reinterpret_cast<void**>(&m_pListener)); FAILED(hr))
					{
						break;
					}

					/*DS3DLISTENER ds3dListener = { 0 };
					ds3dListener.dwSize = sizeof(DS3DLISTENER);
					ds3dListener.vPosition.x = 0.0f;
					ds3dListener.vPosition.y = 0.0f;
					ds3dListener.vPosition.z = 0.0f;
					ds3dListener.vVelocity.x = 0.0f;
					ds3dListener.vVelocity.y = 0.0f;
					ds3dListener.vVelocity.z = 0.0f;
					ds3dListener.vOrientFront.x = 0.0f;
					ds3dListener.vOrientFront.y = 0.0f;
					ds3dListener.vOrientFront.z = 1.0f;
					ds3dListener.vOrientTop.x = 0.0f;
					ds3dListener.vOrientTop.y = 1.0f;
					ds3dListener.vOrientTop.z = 0.0f;
					ds3dListener.flDistanceFactor = 1.0f;
					ds3dListener.flRolloffFactor = 0.0f;
					ds3dListener.flDopplerFactor = 0.0f;

					if (hr = m_pListener->SetAllParameters(&ds3dListener, DS3D_DEFERRED); FAILED(hr))
					{
						break;
					}*/

				} while (SCION_NULL_WHILE_LOOP_CONDITION);

				return hr;
			}

			HRESULT CAudioManager::StartThread()
			{
				m_evAudioLoopExit.ResetEvent();

				if (LPCVOID pThread = AfxBeginThread(AudioThreadProc, this); !pThread)
				{
					return E_FAIL;
				}

				return S_OK;
			}

			void CAudioManager::StopThread()
			{
				m_evAudioLoopExit.SetEvent();
			}

			void CAudioManager::DestroyListener()
			{
				if (m_pListener)
				{
					m_pListener->Release();
					m_pListener = NULL;
				}
			}

			void CAudioManager::DestroyPrimaryBuffer()
			{
				if (m_pPrimaryBuffer)
				{
					m_pPrimaryBuffer->Release();
					m_pPrimaryBuffer = NULL;
				}
			}

			void CAudioManager::DestroyDevice()
			{
				if (m_pDevice)
				{
					m_pDevice->Release();
					m_pDevice = NULL;
				}
			}

			void CAudioManager::OnAudioLoop()
			{
				ASSERT_POINTER(m_pListener, IDirectSound3DListener8);

				m_pListener->CommitDeferredSettings();
			}

			UINT CAudioManager::AudioThreadProc(LPVOID pData)
			{
				CAudioManager* pAudioManager = reinterpret_cast<CAudioManager*>(pData);
				BOOL bIsRunning = TRUE;

				while (bIsRunning)
				{
					DWORD uResult = WaitForSingleObject(pAudioManager->GetExitEvent(), DEFAULT_THREAD_LOOP_MS);

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
				}

				return 0;
			}

#pragma endregion

		}
	}
}