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
#include "SoundBufferImpl.h"
#include "AudioManagerImpl.h"
#include "WaveBuffer.h"

namespace scion
{
	namespace engine
	{
		namespace sfx
		{
			namespace priv
			{

#pragma region Constructors

				IMPLEMENT_DYNAMIC(CSoundBufferImpl, CObject)

				CSoundBufferImpl::CSoundBufferImpl()
					: m_pSecondaryBuffer(NULL)
					, m_uDataSize(0)
				{
				
				}

				CSoundBufferImpl::~CSoundBufferImpl()
				{
					Unload();
				}

#pragma endregion
#pragma region Operations

				HRESULT CSoundBufferImpl::LoadFromFile(LPCTSTR pszFileName)
				{
					ASSERT_VALID(this);

					if (!AfxIsValidString(pszFileName, MAX_PATH))
					{
						return E_INVALIDARG;
					}

					HRESULT hr = S_OK;

					do
					{
						CWaveBuffer WaveBuffer;

						if (hr = WaveBuffer.LoadFromFile(pszFileName); FAILED(hr))
						{
							break;
						}

						DWORD uDataLen = WaveBuffer.GetDataLen();
						WAVEFORMATEX wfFormat = { 0 };

						if (hr = WaveBuffer.GetFormat(wfFormat); FAILED(hr))
						{
							break;
						}

						DSBUFFERDESC BufferDesc = { 0 };
						BufferDesc.dwSize = sizeof(DSBUFFERDESC);
						BufferDesc.dwFlags = 0ul;
						BufferDesc.dwFlags |= DSBCAPS_STATIC;
						BufferDesc.dwFlags |= DSBCAPS_CTRLVOLUME;
						BufferDesc.dwFlags |= DSBCAPS_GETCURRENTPOSITION2;
						BufferDesc.dwFlags |= DSBCAPS_STICKYFOCUS;
						BufferDesc.dwFlags |= DSBCAPS_GLOBALFOCUS;
						BufferDesc.dwFlags |= DSBCAPS_CTRL3D;
						BufferDesc.dwBufferBytes = uDataLen;
						BufferDesc.lpwfxFormat = &wfFormat;
						BufferDesc.guid3DAlgorithm = DS3DALG_DEFAULT;

						LPDIRECTSOUNDBUFFER pSecondaryBuffer = NULL;
						if (hr = AudioManager.CreateSecondaryBuffer(&BufferDesc, &pSecondaryBuffer); FAILED(hr))
						{
							break;
						}

						if (hr = pSecondaryBuffer->QueryInterface(IID_IDirectSoundBuffer8, reinterpret_cast<void**>(&m_pSecondaryBuffer)); FAILED(hr))
						{
							break;
						}

						pSecondaryBuffer->Release();

						LPBYTE pData = NULL;

						if (hr = m_pSecondaryBuffer->Lock(0ul, uDataLen, reinterpret_cast<void**>(&pData)
							, &uDataLen, NULL, NULL, 0ul); FAILED(hr))
						{
							break;
						}

						uDataLen = WaveBuffer.GetData(pData, uDataLen);

						if (hr = m_pSecondaryBuffer->Unlock(pData, uDataLen, NULL, NULL); FAILED(hr))
						{
							break;
						}

						m_uDataSize = uDataLen;

						WaveBuffer.Unload();

					} while (SCION_NULL_WHILE_LOOP_CONDITION);

					if (FAILED(hr))
					{
						Unload();
					}

					return hr;
				}

				CTimeSpan CSoundBufferImpl::GetDuration() const
				{
					if (!m_pSecondaryBuffer)
					{
						return CTimeSpan();
					}

					WAVEFORMATEX WaveFormat = { 0 };
					if (const HRESULT hr = m_pSecondaryBuffer->GetFormat(&WaveFormat, sizeof(WAVEFORMATEX), NULL); FAILED(hr))
					{
						return CTimeSpan();
					}

					const FLOAT fSeconds = static_cast<FLOAT>(m_uDataSize) / (WaveFormat.nAvgBytesPerSec * WaveFormat.nBlockAlign);
					const INT nSeconds = static_cast<INT>(fSeconds);

					return CTimeSpan(0, 0, 0, nSeconds);
				}

				WORD CSoundBufferImpl::GetChannelCount() const
				{
					if (!m_pSecondaryBuffer)
					{
						return 0u;
					}

					WAVEFORMATEX WaveFormat = { 0 };
					if (const HRESULT hr = m_pSecondaryBuffer->GetFormat(&WaveFormat, sizeof(WAVEFORMATEX), NULL); FAILED(hr))
					{
						return 0u;
					}

					return WaveFormat.nChannels;
				}

				DWORD CSoundBufferImpl::GetSampleRate() const
				{
					if (!m_pSecondaryBuffer)
					{
						return 0ul;
					}

					WAVEFORMATEX WaveFormat = { 0 };
					if (const HRESULT hr = m_pSecondaryBuffer->GetFormat(&WaveFormat, sizeof(WAVEFORMATEX), NULL); FAILED(hr))
					{
						return 0ul;
					}

					return WaveFormat.nSamplesPerSec;
				}

				void CSoundBufferImpl::Unload()
				{
					if (m_pSecondaryBuffer)
					{
						m_pSecondaryBuffer->Release();
						m_pSecondaryBuffer = NULL;
					}

					m_uDataSize = 0;
				}

#pragma endregion
#pragma region Overridables

#ifdef _DEBUG

				void CSoundBufferImpl::AssertValid() const
				{
					CObject::AssertValid();

				}

				void CSoundBufferImpl::Dump(CDumpContext& dc) const
				{
					CObject::Dump(dc);

					dc << _T("Duration: ") << GetDuration() << _T("\n");
					dc << _T("ChannelCount: ") << GetChannelCount() << _T("\n");
					dc << _T("SampleRate: ") << GetSampleRate() << _T("\n");
				}

#endif

#pragma endregion

			}
		}
	}
}