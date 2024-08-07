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

				struct TMMIOFile
				{
					HMMIO			_hMMIO;
					WAVEFORMATEX* _pWaveFormat;
					MMCKINFO		_ckIn;
					MMCKINFO		_ckInRIFF;
				};

			}

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

			IMPLEMENT_DYNAMIC(CMMIOFile, CObject)

				CMMIOFile::CMMIOFile()
				: m_pImpl(NULL)
				, m_pData(NULL)
			{
				m_pImpl = new priv::TMMIOFile{ 0 };
			}

			CMMIOFile::~CMMIOFile()
			{
				Unload();
				delete m_pImpl;
			}

#pragma endregion
#pragma region Operations

			HRESULT CMMIOFile::LoadFromFile(LPCTSTR pszFileName)
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

					m_pData = reinterpret_cast<LPBYTE>(VirtualAlloc(NULL, m_pImpl->_ckIn.cksize, MEM_COMMIT, PAGE_READWRITE));
					if (!m_pData)
					{
						hr = E_OUTOFMEMORY;
						break;
					}

					hr = Read(m_pImpl->_ckIn.cksize, m_pData);

				} while (FALSE);

				return hr;
			}

			void CMMIOFile::Unload()
			{
				if (m_pData)
				{
					VirtualFree(m_pData, m_pImpl->_ckIn.cksize, MEM_DECOMMIT);
					m_pData = NULL;
				}

				Close();
			}

#pragma endregion
#pragma region Overridables

#if defined(_DEBUG) || defined(_AFXDLL)

			void CMMIOFile::AssertValid() const
			{
				CObject::AssertValid();

				ASSERT_POINTER(m_pImpl, priv::TMMIOFile);
			}

			void CMMIOFile::Dump(CDumpContext& dc) const
			{
				CObject::Dump(dc);
			}

#endif

#pragma endregion
#pragma region Implementations

			HRESULT CMMIOFile::Open(LPCTSTR pszFileName)
			{
				ASSERT_VALID(this);
				ASSERT(AfxIsValidString(pszFileName, MAX_PATH));

				HRESULT hr = S_OK;

				do
				{
					MMIOINFO mmioInfo = { 0 };
					m_pImpl->_hMMIO = mmioOpen(const_cast<LPTSTR>(pszFileName), &mmioInfo, MMIO_ALLOCBUF | MMIO_READWRITE);
					if (!m_pImpl->_hMMIO)
					{
						hr = MMRESULT_HRESULT(mmioInfo.wErrorRet);
						break;
					}

					MMRESULT mmRes = mmioDescend(m_pImpl->_hMMIO, &m_pImpl->_ckInRIFF, NULL, 0);
					if (MMSYSERR_NOERROR != mmRes)
					{
						hr = MMRESULT_HRESULT(mmRes);
						break;
					}

					if ((FOURCC_RIFF != m_pImpl->_ckInRIFF.ckid) || mmioFOURCC('W', 'A', 'V', 'E') != m_pImpl->_ckInRIFF.fccType)
					{
						hr = E_MMIOERR_NOTWAVEFILE;
						break;
					}

					m_pImpl->_ckIn.ckid = mmioFOURCC('f', 'm', 't', ' ');
					mmRes = mmioDescend(m_pImpl->_hMMIO, &m_pImpl->_ckIn, &m_pImpl->_ckInRIFF, MMIO_FINDCHUNK);
					if (MMSYSERR_NOERROR != mmRes)
					{
						hr = MMRESULT_HRESULT(mmRes);
						break;
					}

					if (sizeof(PCMWAVEFORMAT) > m_pImpl->_ckIn.cksize)
					{
						hr = E_MMIOERR_NOTWAVEFILE;
						break;
					}

					PCMWAVEFORMAT pcmWaveFormat;
					mmRes = mmioRead(m_pImpl->_hMMIO, reinterpret_cast<HPSTR>(&pcmWaveFormat), sizeof(PCMWAVEFORMAT));
					if (sizeof(PCMWAVEFORMAT) != mmRes)
					{
						hr = E_MMIOERR_CANNOTREAD;
						break;
					}

					WORD uExtraAlloc = 0;
					if (pcmWaveFormat.wf.wFormatTag != WAVE_FORMAT_PCM)
					{
						mmRes = mmioRead(m_pImpl->_hMMIO, reinterpret_cast<HPSTR>(&uExtraAlloc), sizeof(WORD));
						if (sizeof(WORD) != mmRes)
						{
							hr = E_MMIOERR_CANNOTREAD;
							break;
						}
					}

					m_pImpl->_pWaveFormat = reinterpret_cast<WAVEFORMATEX*>(VirtualAlloc(NULL, uExtraAlloc + sizeof(WAVEFORMATEX), MEM_COMMIT, PAGE_READWRITE));
					if (!m_pImpl->_pWaveFormat)
					{
						hr = E_OUTOFMEMORY;
						break;
					}

					CopyMemory(m_pImpl->_pWaveFormat, &pcmWaveFormat, sizeof(PCMWAVEFORMAT));
					if (0 != uExtraAlloc)
					{
						mmRes = mmioRead(m_pImpl->_hMMIO, reinterpret_cast<HPSTR>(&m_pImpl->_pWaveFormat->cbSize) + sizeof(WORD), uExtraAlloc);
						if (uExtraAlloc != mmRes)
						{
							hr = E_MMIOERR_NOTWAVEFILE;
							break;
						}
					}

					mmRes = mmioAscend(m_pImpl->_hMMIO, &m_pImpl->_ckIn, 0);
					if (MMSYSERR_NOERROR != mmRes)
					{
						hr = MMRESULT_HRESULT(mmRes);
						break;
					}

				} while (FALSE);

				if (FAILED(hr))
				{
					Close();
				}

				return hr;
			}

			HRESULT CMMIOFile::StartRead()
			{
				ASSERT_VALID(this);

				LONG nRet = mmioSeek(m_pImpl->_hMMIO, m_pImpl->_ckInRIFF.dwDataOffset + sizeof(FOURCC), SEEK_SET);
				if (-1 == nRet)
				{
					return E_MMIOERR_CANNOTSEEK;
				}

				m_pImpl->_ckIn.ckid = mmioFOURCC('d', 'a', 't', 'a');
				MMRESULT mmRes = mmioDescend(m_pImpl->_hMMIO, &m_pImpl->_ckIn, &m_pImpl->_ckInRIFF, MMIO_FINDCHUNK);
				if (MMSYSERR_NOERROR != mmRes)
				{
					return MMRESULT_HRESULT(mmRes);
				}

				return S_OK;
			}

			HRESULT CMMIOFile::Read(DWORD uRead, LPBYTE pData)
			{
				ASSERT_VALID(this);

				MMIOINFO mmioInfoIn = { 0 };
				MMRESULT mmRes = mmioGetInfo(m_pImpl->_hMMIO, &mmioInfoIn, 0);
				if (MMSYSERR_NOERROR != mmRes)
				{
					return E_FAIL;
				}

				UINT uDataIn = uRead;
				if (uDataIn > m_pImpl->_ckIn.cksize)
				{
					uDataIn = m_pImpl->_ckIn.cksize;
				}

				for (UINT i = 0; i < uDataIn; i++)
				{
					if (mmioInfoIn.pchNext == mmioInfoIn.pchEndRead)
					{
						mmRes = mmioAdvance(m_pImpl->_hMMIO, &mmioInfoIn, MMIO_READ);
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

				mmRes = mmioSetInfo(m_pImpl->_hMMIO, &mmioInfoIn, 0);
				if (MMSYSERR_NOERROR != mmRes)
				{
					return E_FAIL;
				}

				return S_OK;
			}

			void CMMIOFile::Close()
			{
				ASSERT_VALID(this);

				if (m_pImpl->_pWaveFormat)
				{
					const WORD uExtraAlloc = m_pImpl->_pWaveFormat->cbSize;
					VirtualFree(m_pImpl->_pWaveFormat, uExtraAlloc + sizeof(WAVEFORMATEX), MEM_DECOMMIT);
					m_pImpl->_pWaveFormat = NULL;
				}

				if (m_pImpl->_hMMIO)
				{
					mmioClose(m_pImpl->_hMMIO, 0);
					m_pImpl->_hMMIO = NULL;
				}
			}

#pragma endregion

		}
	}
}