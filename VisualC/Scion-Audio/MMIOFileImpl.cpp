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
#include "MMIOFileImpl.h"

namespace scion
{
	namespace engine
	{
		namespace sfx
		{
			namespace priv
			{

				static inline HRESULT MMRESULT_HRESULT(MMRESULT mmRes)
				{
					switch (mmRes)
					{
					case MMIOERR_ACCESSDENIED: return E_MMIOERR_ACCESSDENIED;
					case MMIOERR_INVALIDFILE: return E_MMIOERR_INVALIDFILE;
					case MMIOERR_NETWORKERROR: return E_MMIOERR_NETWORKERROR;
					case MMIOERR_PATHNOTFOUND: return E_MMIOERR_PATHNOTFOUND;
					case MMIOERR_SHARINGVIOLATION: return E_MMIOERR_SHARINGVIOLATION;
					case MMIOERR_TOOMANYOPENFILES: return E_MMIOERR_TOOMANYOPENFILES;
					case MMIOERR_CHUNKNOTFOUND: return E_MMIOERR_CHUNKNOTFOUND;
					case MMIOERR_CANNOTSEEK: return E_MMIOERR_CANNOTSEEK;
					case MMIOERR_CANNOTWRITE: return E_MMIOERR_CANNOTWRITE;
					case MMIOERR_CANNOTEXPAND: return E_MMIOERR_CANNOTEXPAND;
					case MMIOERR_CANNOTREAD: return E_MMIOERR_CANNOTREAD;
					case MMIOERR_OUTOFMEMORY: return E_MMIOERR_OUTOFMEMORY;
					case MMIOERR_UNBUFFERED: return E_MMIOERR_UNBUFFERED;
					default: return E_FAIL;
					}
				}

#pragma region Constructors

				IMPLEMENT_DYNAMIC(CMMIOFileImpl, CObject)

				CMMIOFileImpl::CMMIOFileImpl()
					: m_hMMIO(NULL)
					, m_pWaveFormat(NULL)
					, m_ckIn { 0 }
					, m_ckInRIFF { 0 }
					, m_pData(NULL)
				{
				
				}

				CMMIOFileImpl::~CMMIOFileImpl()
				{
					Unload();
				}

#pragma endregion
#pragma region Operations

				HRESULT CMMIOFileImpl::LoadFromFile(LPCTSTR pszFileName)
				{
					ASSERT_VALID(this);

					if (!AfxIsValidString(pszFileName, MAX_PATH))
					{
						return E_INVALIDARG;
					}

					HRESULT hr = S_OK;

					do
					{
						hr = Open(pszFileName);
						if (FAILED(hr))
						{
							break;
						}

						hr = StartRead();
						if (FAILED(hr))
						{
							break;
						}

						m_pData = reinterpret_cast<LPBYTE>(VirtualAlloc(NULL, m_ckIn.cksize, MEM_COMMIT, PAGE_READWRITE));
						if (!m_pData)
						{
							hr = E_OUTOFMEMORY;
							break;
						}

						hr = Read(m_ckIn.cksize, m_pData);

					} while (SCION_NULL_WHILE_LOOP_CONDITION);

					return hr;
				}

				LPVOID CMMIOFileImpl::GetWaveFormat() const
				{
					ASSERT_VALID(this);

					return m_pWaveFormat;
				}

				void CMMIOFileImpl::Unload()
				{
					if (m_pData)
					{
						VirtualFree(m_pData, m_ckIn.cksize, MEM_DECOMMIT);
						m_pData = NULL;
					}

					Close();
				}

#pragma endregion
#pragma region Overridables

#if defined(_DEBUG) || defined(_AFXDLL)

				void CMMIOFileImpl::AssertValid() const
				{
					CObject::AssertValid();

				}

				void CMMIOFileImpl::Dump(CDumpContext& dc) const
				{
					CObject::Dump(dc);
				}

#endif

#pragma endregion
#pragma region Implementations

				HRESULT CMMIOFileImpl::Open(LPCTSTR pszFileName)
				{
					ASSERT_VALID(this);
					ASSERT(AfxIsValidString(pszFileName, MAX_PATH));

					HRESULT hr = S_OK;

					do
					{
						MMIOINFO mmioInfo = { 0 };
						m_hMMIO = mmioOpen(const_cast<LPTSTR>(pszFileName), &mmioInfo, MMIO_ALLOCBUF | MMIO_READWRITE);
						if (!m_hMMIO)
						{
							hr = MMRESULT_HRESULT(mmioInfo.wErrorRet);
							break;
						}

						MMRESULT mmRes = mmioDescend(m_hMMIO, &m_ckInRIFF, NULL, 0);
						if (MMSYSERR_NOERROR != mmRes)
						{
							hr = MMRESULT_HRESULT(mmRes);
							break;
						}

						if ((FOURCC_RIFF != m_ckInRIFF.ckid) || mmioFOURCC('W', 'A', 'V', 'E') != m_ckInRIFF.fccType)
						{
							hr = E_MMIOERR_NOTWAVEFILE;
							break;
						}

						m_ckIn.ckid = mmioFOURCC('f', 'm', 't', ' ');
						mmRes = mmioDescend(m_hMMIO, &m_ckIn, &m_ckInRIFF, MMIO_FINDCHUNK);
						if (MMSYSERR_NOERROR != mmRes)
						{
							hr = MMRESULT_HRESULT(mmRes);
							break;
						}

						if (sizeof(PCMWAVEFORMAT) > m_ckIn.cksize)
						{
							hr = E_MMIOERR_NOTWAVEFILE;
							break;
						}

						PCMWAVEFORMAT pcmWaveFormat;
						mmRes = mmioRead(m_hMMIO, reinterpret_cast<HPSTR>(&pcmWaveFormat), sizeof(PCMWAVEFORMAT));
						if (sizeof(PCMWAVEFORMAT) != mmRes)
						{
							hr = E_MMIOERR_CANNOTREAD;
							break;
						}

						WORD uExtraAlloc = 0;
						if (pcmWaveFormat.wf.wFormatTag != WAVE_FORMAT_PCM)
						{
							mmRes = mmioRead(m_hMMIO, reinterpret_cast<HPSTR>(&uExtraAlloc), sizeof(WORD));
							if (sizeof(WORD) != mmRes)
							{
								hr = E_MMIOERR_CANNOTREAD;
								break;
							}
						}

						m_pWaveFormat = reinterpret_cast<WAVEFORMATEX*>(VirtualAlloc(NULL, uExtraAlloc + sizeof(WAVEFORMATEX), MEM_COMMIT, PAGE_READWRITE));
						if (!m_pWaveFormat)
						{
							hr = E_OUTOFMEMORY;
							break;
						}

						CopyMemory(m_pWaveFormat, &pcmWaveFormat, sizeof(PCMWAVEFORMAT));
						if (0 != uExtraAlloc)
						{
							mmRes = mmioRead(m_hMMIO, reinterpret_cast<HPSTR>(&m_pWaveFormat->cbSize) + sizeof(WORD), uExtraAlloc);
							if (uExtraAlloc != mmRes)
							{
								hr = E_MMIOERR_NOTWAVEFILE;
								break;
							}
						}

						mmRes = mmioAscend(m_hMMIO, &m_ckIn, 0);
						if (MMSYSERR_NOERROR != mmRes)
						{
							hr = MMRESULT_HRESULT(mmRes);
							break;
						}

					} while (SCION_NULL_WHILE_LOOP_CONDITION);

					if (FAILED(hr))
					{
						Close();
					}

					return hr;
				}

				HRESULT CMMIOFileImpl::StartRead()
				{
					ASSERT_VALID(this);

					LONG nRet = mmioSeek(m_hMMIO, m_ckInRIFF.dwDataOffset + sizeof(FOURCC), SEEK_SET);
					if (-1 == nRet)
					{
						return E_MMIOERR_CANNOTSEEK;
					}

					m_ckIn.ckid = mmioFOURCC('d', 'a', 't', 'a');
					MMRESULT mmRes = mmioDescend(m_hMMIO, &m_ckIn, &m_ckInRIFF, MMIO_FINDCHUNK);
					if (MMSYSERR_NOERROR != mmRes)
					{
						return MMRESULT_HRESULT(mmRes);
					}

					return S_OK;
				}

				HRESULT CMMIOFileImpl::Read(DWORD uRead, LPBYTE pData)
				{
					ASSERT_VALID(this);

					MMIOINFO mmioInfoIn = { 0 };
					MMRESULT mmRes = mmioGetInfo(m_hMMIO, &mmioInfoIn, 0);
					if (MMSYSERR_NOERROR != mmRes)
					{
						return E_FAIL;
					}

					UINT uDataIn = uRead;
					if (uDataIn > m_ckIn.cksize)
					{
						uDataIn = m_ckIn.cksize;
					}

					for (UINT i = 0; i < uDataIn; i++)
					{
						if (mmioInfoIn.pchNext == mmioInfoIn.pchEndRead)
						{
							mmRes = mmioAdvance(m_hMMIO, &mmioInfoIn, MMIO_READ);
							if (MMSYSERR_NOERROR != mmRes)
							{
								return MMRESULT_HRESULT(mmRes);
							}

							if (mmioInfoIn.pchNext == mmioInfoIn.pchEndRead)
							{
								return E_MMIOERR_CORRUPTWAVEFILE;
							}
						}

						*((LPBYTE)pData + i) = *((LPBYTE)mmioInfoIn.pchNext);
						(LPBYTE)mmioInfoIn.pchNext++;
					}

					mmRes = mmioSetInfo(m_hMMIO, &mmioInfoIn, 0);
					if (MMSYSERR_NOERROR != mmRes)
					{
						return E_FAIL;
					}

					return S_OK;
				}

				void CMMIOFileImpl::Close()
				{
					ASSERT_VALID(this);

					if (m_pWaveFormat)
					{
						const WORD uExtraAlloc = m_pWaveFormat->cbSize;
						VirtualFree(m_pWaveFormat, uExtraAlloc + sizeof(WAVEFORMATEX), MEM_DECOMMIT);
						m_pWaveFormat = NULL;
					}

					if (m_hMMIO)
					{
						mmioClose(m_hMMIO, 0);
						m_hMMIO = NULL;
					}
				}

#pragma endregion

			}
		}
	}
}