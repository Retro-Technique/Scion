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
#include "SoundImpl.h"
#include "AudioManagerImpl.h"
#include "SoundBufferImpl.h"

namespace scion
{
	namespace engine
	{
		namespace sfx
		{
			namespace priv
			{

#pragma region Constructors

				IMPLEMENT_DYNAMIC(CSoundImpl, CObject)

				CSoundImpl::CSoundImpl()
					: m_pSecondaryBuffer(NULL)
				{

				}

				CSoundImpl::~CSoundImpl()
				{
					Unload();
				}

#pragma endregion
#pragma region Operations

				HRESULT CSoundImpl::LoadFromBuffer(const CSoundBuffer& SoundBuffer)
				{
					HRESULT hr = S_OK;

					LPVOID pBuffer = const_cast<LPVOID>(SoundBuffer.GetBuffer());
					LPDIRECTSOUNDBUFFER pOriginalBuffer = static_cast<LPDIRECTSOUNDBUFFER>(pBuffer);

					do
					{
						hr = AudioManager.DuplicateSecondaryBuffer(pOriginalBuffer, &m_pSecondaryBuffer);
						if (FAILED(hr))
						{
							break;
						}

					} while (SCION_NULL_WHILE_LOOP_CONDITION);

					return hr;
				}

				HRESULT CSoundImpl::Play(BOOL bLooping)
				{
					if (!m_pSecondaryBuffer)
					{
						return E_FAIL;
					}

					const DWORD uFlags = bLooping ? DSBPLAY_LOOPING : 0;

					return m_pSecondaryBuffer->Play(0, 0, uFlags);
				}

				HRESULT CSoundImpl::Stop()
				{
					if (!m_pSecondaryBuffer)
					{
						return E_FAIL;
					}

					return m_pSecondaryBuffer->Stop();
				}

				HRESULT CSoundImpl::SetVolume(LONG nVolume)
				{					
					ASSERT(nVolume >= DSBVOLUME_MIN);
					ASSERT(nVolume <= DSBVOLUME_MAX);

					if (!m_pSecondaryBuffer)
					{
						return E_FAIL;
					}

					return m_pSecondaryBuffer->SetVolume(nVolume);
				}

				HRESULT CSoundImpl::GetVolume(LPLONG pVolume) const
				{
					ASSERT_POINTER(pVolume, LONG);

					if (!m_pSecondaryBuffer)
					{
						return E_FAIL;
					}

					return m_pSecondaryBuffer->GetVolume(pVolume);
				}

				BOOL CSoundImpl::IsPlaying() const
				{
					if (!m_pSecondaryBuffer)
					{
						return FALSE;
					}

					DWORD uStatus = 0;
					HRESULT hr = m_pSecondaryBuffer->GetStatus(&uStatus);
					if (FAILED(hr))
					{
						return FALSE;
					}

					return (uStatus & DSBSTATUS_PLAYING);
				}

				BOOL CSoundImpl::IsLooping() const
				{
					if (!m_pSecondaryBuffer)
					{
						return FALSE;
					}

					DWORD uStatus = 0;
					HRESULT hr = m_pSecondaryBuffer->GetStatus(&uStatus);
					if (FAILED(hr))
					{
						return FALSE;
					}

					return (uStatus & DSBSTATUS_LOOPING);
				}

				void CSoundImpl::Unload()
				{
					if (m_pSecondaryBuffer)
					{
						m_pSecondaryBuffer->Release();
						m_pSecondaryBuffer = NULL;
					}
				}

#pragma endregion
#pragma region Overridables

#if defined(_DEBUG) || defined(_AFXDLL)

				void CSoundImpl::AssertValid() const
				{
					CObject::AssertValid();

				}

				void CSoundImpl::Dump(CDumpContext& dc) const
				{
					CObject::Dump(dc);

					LONG nVolume = 0;
					GetVolume(&nVolume);

					dc << _T("Volume: ") << nVolume << _T("\n");
					dc << _T("Playing? ") << IsPlaying() << _T("\n");
					dc << _T("Looping? ") << IsLooping() << _T("\n");
				}

#endif

#pragma endregion
			}
		}
	}
}