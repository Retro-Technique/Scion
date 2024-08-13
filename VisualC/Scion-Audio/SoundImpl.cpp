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
					
				}

#pragma endregion
#pragma region Operations

				HRESULT CSoundImpl::SetBuffer(const CSoundBuffer& SoundBuffer)
				{
					HRESULT hr = S_OK;

					const LPBYTE pBufferPtr = SoundBuffer.GetData();
					const UINT uBufferSize = SoundBuffer.GetSize();
					LPCWAVEFORMATEX pWaveFormat = static_cast<LPCWAVEFORMATEX>(SoundBuffer.GetWaveFormat());

					do
					{
						DSBUFFERDESC DSBufferDesc = { 0 };
						DSBufferDesc.dwSize = sizeof(DSBUFFERDESC);
						DSBufferDesc.dwFlags = 0ul;
						DSBufferDesc.dwFlags |= DSBCAPS_STATIC;
						DSBufferDesc.dwFlags |= DSBCAPS_CTRLDEFAULT | DSBCAPS_GETCURRENTPOSITION2;
						DSBufferDesc.dwFlags |= DSBCAPS_STICKYFOCUS;
						DSBufferDesc.dwFlags |= DSBCAPS_GLOBALFOCUS;
						DSBufferDesc.dwBufferBytes = uBufferSize;
						DSBufferDesc.lpwfxFormat = const_cast<LPWAVEFORMATEX>(pWaveFormat);

						hr = AudioManager.CreateSecondaryBuffer(&DSBufferDesc, &m_pSecondaryBuffer);
						if (FAILED(hr))
						{
							break;
						}

						LPBYTE pDstData = NULL;
						DWORD uLength = 0ul;

						hr = m_pSecondaryBuffer->Lock(0ul, DSBufferDesc.dwBufferBytes
							, reinterpret_cast<void**>(&pDstData), &uLength
							, NULL, NULL
							, 0ul); 
						if (FAILED(hr))
						{
							break;
						}

						//pDstData = pBufferPtr;
						CopyMemory(pDstData, pBufferPtr, uLength);
					
						hr = m_pSecondaryBuffer->Unlock(pDstData, uLength, NULL, NULL);
						if (FAILED(hr))
						{
							break;
						}

					} while (SCION_NULL_WHILE_LOOP_CONDITION);

					return hr;
				}

				const CSoundBuffer* CSoundImpl::GetBuffer() const
				{
					return NULL;
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

					
				}

#endif

#pragma endregion
			}
		}
	}
}