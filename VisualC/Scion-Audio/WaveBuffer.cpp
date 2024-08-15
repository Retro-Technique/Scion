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
#include "WaveBuffer.h"
#include "MMIO.h"

namespace scion
{
	namespace engine
	{
		namespace sfx
		{
			namespace priv
			{

#pragma region Constructors

				IMPLEMENT_DYNAMIC(CWaveBuffer, CObject)

				CWaveBuffer::CWaveBuffer()
					: m_pData(NULL)
					, m_uDataLen(0)
				{

				}

				CWaveBuffer::~CWaveBuffer()
				{
					Unload();
				}

#pragma endregion
#pragma region Operations

				HRESULT CWaveBuffer::LoadFromFile(LPCTSTR pszFileName)
				{
					ASSERT_VALID(this);

					if (!AfxIsValidString(pszFileName, MAX_PATH))
					{
						return E_INVALIDARG;
					}

					Unload();

					CFileException fileException;
					CFile fileWave;
					if (!fileWave.Open(pszFileName, CFile::modeRead, &fileException))
					{
						return fileException.m_lOsError; //TODO: voir c'est quoi le best
					}

					m_uDataLen = static_cast<DWORD>(fileWave.GetLength());

					HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, m_uDataLen);
					if (!hGlobal)
					{
						return E_OUTOFMEMORY;
					}
					
					LPVOID pBuffer = GlobalLock(hGlobal);
					m_pData = static_cast<LPBYTE>(pBuffer);

					fileWave.Read(m_pData, m_uDataLen);

					return S_OK;
				}

				void CWaveBuffer::Unload()
				{
					if (m_pData)
					{
						GlobalFree(m_pData);
						m_pData = NULL;
						m_uDataLen = 0;
					}
				}

				BOOL CWaveBuffer::IsValid() const
				{
					return m_pData ? TRUE : FALSE;
				}

				BOOL CWaveBuffer::Play(BOOL bAsync, BOOL bLooped) const
				{
					if (!IsValid())
					{
						return FALSE;
					}

					DWORD uFlags = SND_MEMORY | SND_NODEFAULT;
					uFlags |= bAsync ? SND_ASYNC : SND_SYNC;
					uFlags |= bLooped ? (SND_ASYNC | SND_LOOP) : 0;

					return PlaySound(reinterpret_cast<LPCTSTR>(m_pData), NULL, uFlags);
				}

				HRESULT CWaveBuffer::GetFormat(WAVEFORMATEX& wfFormat) const
				{
					if (!IsValid())
					{
						return E_FAIL;
					}

					HRESULT hr = S_OK;
					CMMMemoryIOInfo mmioInfo(reinterpret_cast<HPSTR>(m_pData), m_uDataLen);					
					CMMIO mmio;

					do
					{
						if (hr = mmio.OpenFromMemory(mmioInfo); FAILED(hr))
						{
							break;
						}

						CMMTypeChunk mmckParent('W', 'A', 'V', 'E');
						if (hr = mmio.Descend(mmckParent, MMIO_FINDRIFF); FAILED(hr))
						{
							break;
						}

						CMMIdChunk mmckSubChunk('f', 'm', 't', ' ');
						if (hr = mmio.Descend(mmckSubChunk, mmckParent, MMIO_FINDCHUNK); FAILED(hr))
						{
							break;
						}

						if (hr = mmio.Read(reinterpret_cast<HPSTR>(&wfFormat), sizeof(WAVEFORMATEX)); FAILED(hr))
						{
							break;
						}

						if (hr = mmio.Ascend(mmckSubChunk); FAILED(hr))
						{
							break;
						}

					} while (SCION_NULL_WHILE_LOOP_CONDITION);

					mmio.Close();

					return hr;
				}

				DWORD CWaveBuffer::GetDataLen() const
				{
					if (!IsValid())
					{
						return 0;
					}

					HRESULT hr = S_OK;
					CMMMemoryIOInfo mmioInfo(reinterpret_cast<HPSTR>(m_pData), m_uDataLen);
					CMMIO mmio;

					do
					{
						if (hr = mmio.OpenFromMemory(mmioInfo); FAILED(hr))
						{
							break;
						}

						CMMTypeChunk mmckParent('W', 'A', 'V', 'E');
						if (hr = mmio.Descend(mmckParent, MMIO_FINDRIFF); FAILED(hr))
						{
							break;
						}

						CMMIdChunk mmckSubChunk('d', 'a', 't', 'a');
						if (hr = mmio.Descend(mmckSubChunk, mmckParent, MMIO_FINDCHUNK); SUCCEEDED(hr))
						{
							return mmckSubChunk.cksize;
						}

					} while (SCION_NULL_WHILE_LOOP_CONDITION);

					mmio.Close();

					return 0;
				}

				DWORD CWaveBuffer::GetData(LPBYTE pWaveData, DWORD uMaxToCopy) const
				{
					if (!AfxIsValidAddress(pWaveData, uMaxToCopy))
					{
						return 0;
					}

					if (!IsValid())
					{
						return 0;
					}

					HRESULT hr = S_OK;
					DWORD uReadLen = 0;
					CMMMemoryIOInfo mmioInfo(reinterpret_cast<HPSTR>(m_pData), m_uDataLen);
					CMMIO mmio;

					do
					{
						if (hr = mmio.OpenFromMemory(mmioInfo); FAILED(hr))
						{
							break;
						}

						CMMTypeChunk mmckParent('W', 'A', 'V', 'E');
						if (hr = mmio.Descend(mmckParent, MMIO_FINDRIFF); FAILED(hr))
						{
							break;
						}

						uReadLen = mmio.Read(reinterpret_cast<HPSTR>(pWaveData), uMaxToCopy);

					} while (SCION_NULL_WHILE_LOOP_CONDITION);

					mmio.Close();

					return uReadLen;
				}

#pragma endregion
#pragma region Overridables

#if defined(_DEBUG) || defined(_AFXDLL)

				void CWaveBuffer::AssertValid() const
				{
					CObject::AssertValid();
				}

				void CWaveBuffer::Dump(CDumpContext& dc) const
				{
					CObject::Dump(dc);

					dc << _T("DataLen: ") << m_uDataLen << _T("\n");
				}

#endif

#pragma endregion

			}
		}
	}
}