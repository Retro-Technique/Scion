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

namespace scion
{
	namespace engine
	{
		namespace sfx
		{
			namespace priv
			{

				struct TAudioDevice
				{
					IDirectSound* _pDSDevice;
					IDirectSoundBuffer* _pDSPrimaryBuffer;					
				};

			}

#pragma region Constructors

			IMPLEMENT_DYNCREATE(CAudioManager, CWinThread)

			CAudioManager::CAudioManager()
				: m_pImpl(NULL)
			{
				m_pImpl = new priv::TAudioDevice{ 0 };
			}

			CAudioManager::~CAudioManager()
			{
				delete m_pImpl;
			}

#pragma endregion
#pragma region Operations

			HRESULT CAudioManager::Initialize(HWND hWnd)
			{
				if (!hWnd)
				{
					return E_INVALIDARG;
				}

				HRESULT hr = S_OK;

				do
				{
					hr = CreateDevice(hWnd);
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

				} while (FALSE);

				if (FAILED(hr))
				{
					Quit();
				}

				return hr;
			}

			void CAudioManager::Quit()
			{
				StopThread();
				DestroyPrimaryBuffer();
				DestroyDevice();
			}

#pragma endregion
#pragma region Overridables

#if defined(_DEBUG) || defined(_AFXDLL)

			void CAudioManager::AssertValid() const
			{
				CWinThread::AssertValid();

				ASSERT_POINTER(m_pImpl, priv::TAudioDevice);
			}

			void CAudioManager::Dump(CDumpContext& dc) const
			{
				CWinThread::Dump(dc);
			}

#endif

#pragma endregion
#pragma region Implementations

			HRESULT CAudioManager::CreateDevice(HWND hWnd)
			{
				ASSERT(hWnd);

				HRESULT hr = S_OK;

				do
				{
					hr = DirectSoundCreate(NULL, &m_pImpl->_pDSDevice, NULL);
					if (FAILED(hr))
					{
						break;
					}

					hr = m_pImpl->_pDSDevice->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
					if (FAILED(hr))
					{
						break;
					}

				} while (FALSE);

				return hr;
			}

			HRESULT CAudioManager::CreatePrimaryBuffer()
			{
				HRESULT hr = S_OK;

				do
				{
					DSBUFFERDESC DSBufferDesc = { 0 };
					DSBufferDesc.dwSize = sizeof(DSBUFFERDESC);
					DSBufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;

					hr = m_pImpl->_pDSDevice->CreateSoundBuffer(&DSBufferDesc, &m_pImpl->_pDSPrimaryBuffer, NULL);
					if (FAILED(hr))
					{
						break;
					}

					WAVEFORMATEX DSWaveFormat = { 0 };
					DSWaveFormat.wFormatTag = WAVE_FORMAT_PCM;
					DSWaveFormat.nChannels = DEFAULT_CHANNEL_COUNT;
					DSWaveFormat.nSamplesPerSec = DEFAULT_FREQUENCY;
					DSWaveFormat.wBitsPerSample = DEFAULT_FORMAT;
					DSWaveFormat.nBlockAlign = DSWaveFormat.nChannels * DSWaveFormat.wBitsPerSample;
					DSWaveFormat.nAvgBytesPerSec = DSWaveFormat.nSamplesPerSec * DSWaveFormat.nBlockAlign;

					hr = m_pImpl->_pDSPrimaryBuffer->SetFormat(&DSWaveFormat);
					if (FAILED(hr))
					{
						break;
					}

					hr = m_pImpl->_pDSPrimaryBuffer->Play(0, 0, DSBPLAY_LOOPING);
					if (FAILED(hr))
					{
						break;
					}

				} while (FALSE);
		
				return hr;
			}

			HRESULT CAudioManager::StartThread()
			{
				BOOL bSucceeded = CreateThread();
				if (!bSucceeded)
				{
					return E_FAIL;
				}
				
				return S_OK;
			}

			void CAudioManager::StopThread()
			{
				PostThreadMessage(WM_QUIT, 0, 0);
			}

			void CAudioManager::DestroyPrimaryBuffer()
			{
				if (m_pImpl->_pDSPrimaryBuffer)
				{
					m_pImpl->_pDSPrimaryBuffer->Release();
					m_pImpl->_pDSPrimaryBuffer = NULL;
				}
			}

			void CAudioManager::DestroyDevice()
			{
				if (m_pImpl->_pDSDevice)
				{
					m_pImpl->_pDSDevice->Release();
					m_pImpl->_pDSDevice = NULL;
				}
			}

#pragma endregion
#pragma region Messages

			BEGIN_MESSAGE_MAP(CAudioManager, CWinThread)
				ON_THREAD_MESSAGE(WM_SOUND_HANDLING, OnSoundHandling)
			END_MESSAGE_MAP()

			void CAudioManager::OnSoundHandling(WPARAM wParam, LPARAM lParam)
			{
				UNREFERENCED_PARAMETER(wParam);
				UNREFERENCED_PARAMETER(lParam);
			}

#pragma endregion
		}
	}
}