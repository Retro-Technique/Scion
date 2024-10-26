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
			{

			}

			CAudioManager::~CAudioManager()
			{

			}

#pragma endregion
#pragma region Operations

			HRESULT CAudioManager::CreateSecondaryBuffer(LPCDSBUFFERDESC pBufferDesc, LPDIRECTSOUNDBUFFER* ppBuffer)
			{
				ASSERT_POINTER(m_spDevice.p, IDirectSound);
				ASSERT_POINTER(pBufferDesc, DSBUFFERDESC);
				ASSERT_NULL_OR_POINTER(*ppBuffer, IDirectSoundBuffer);

				return m_spDevice->CreateSoundBuffer(pBufferDesc, ppBuffer, NULL);
			}

			HRESULT CAudioManager::DuplicateSecondaryBuffer(LPDIRECTSOUNDBUFFER pOriginalBuffer, LPDIRECTSOUNDBUFFER* ppDuplicateBuffer)
			{
				ASSERT_POINTER(m_spDevice.p, IDirectSound);
				ASSERT_POINTER(pOriginalBuffer, IDirectSoundBuffer);
				ASSERT_NULL_OR_POINTER(*ppDuplicateBuffer, IDirectSoundBuffer);

				return m_spDevice->DuplicateSoundBuffer(pOriginalBuffer, ppDuplicateBuffer);
			}

			HRESULT CAudioManager::SetListenerPosition(FLOAT x, FLOAT y, FLOAT z)
			{
				ASSERT_POINTER(m_spListener.p, IDirectSound3DListener8);

				return m_spListener->SetPosition(x, y, z, DS3D_DEFERRED);
			}

			HRESULT CAudioManager::GetListenerPosition(FLOAT& x, FLOAT& y, FLOAT& z)
			{
				ASSERT_POINTER(m_spListener.p, IDirectSound3DListener8);

				D3DVECTOR vPosition = { 0 };

				RETURN_IF_FAILED(m_spListener->GetPosition(&vPosition));

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
				LPDIRECTSOUND8 pDevice = NULL;

				RETURN_IF_FAILED(DirectSoundCreate8(NULL, &pDevice, NULL));

				m_spDevice.Attach(pDevice);

				RETURN_IF_FAILED(m_spDevice->SetCooperativeLevel(pWnd->GetSafeHwnd(), DSSCL_PRIORITY));

				return S_OK;
			}

			HRESULT CAudioManager::CreatePrimaryBuffer()
			{
				ASSERT_POINTER(m_spDevice.p, LPDIRECTSOUND8);

				DSBUFFERDESC BufferDesc = { 0 };
				BufferDesc.dwSize = sizeof(DSBUFFERDESC);
				BufferDesc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;

				LPDIRECTSOUNDBUFFER pPrimaryBuffer = NULL;

				RETURN_IF_FAILED(m_spDevice->CreateSoundBuffer(&BufferDesc, &pPrimaryBuffer, NULL));

				m_spPrimaryBuffer.Attach(pPrimaryBuffer);

				WAVEFORMATEX WaveFormat = { 0 };
				WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
				WaveFormat.nChannels = DEFAULT_CHANNEL_COUNT;
				WaveFormat.nSamplesPerSec = DEFAULT_FREQUENCY;
				WaveFormat.wBitsPerSample = DEFAULT_FORMAT;
				WaveFormat.nBlockAlign = (WaveFormat.nChannels * WaveFormat.wBitsPerSample) / 8; // divisé par 8 pour passer de bits à bytes
				WaveFormat.nAvgBytesPerSec = WaveFormat.nSamplesPerSec * WaveFormat.nBlockAlign;

				RETURN_IF_FAILED(m_spPrimaryBuffer->SetFormat(&WaveFormat)); 
				RETURN_IF_FAILED(m_spPrimaryBuffer->Play(0, 0, DSBPLAY_LOOPING));

				return S_OK;
			}

			HRESULT CAudioManager::CreateListener()
			{
				ASSERT_POINTER(m_spPrimaryBuffer.p, LPDIRECTSOUNDBUFFER);

				LPDIRECTSOUND3DLISTENER8 pListener = NULL;

				RETURN_IF_FAILED(m_spPrimaryBuffer->QueryInterface(IID_IDirectSound3DListener8, reinterpret_cast<void**>(&pListener)));

				m_spListener.Attach(pListener);

				return S_OK;
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

			void CAudioManager::OnAudioLoop()
			{
				ASSERT_POINTER(m_spListener.p, IDirectSound3DListener8);

				m_spListener->CommitDeferredSettings();
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