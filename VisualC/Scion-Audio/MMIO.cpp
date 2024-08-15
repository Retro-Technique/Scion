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

				IMPLEMENT_DYNAMIC(CMMIO, CObject)

					CMMIO::CMMIO()
					: m_hMMIO(NULL)
				{

				}

				CMMIO::~CMMIO()
				{
					Close();
				}

#pragma endregion
#pragma region Operations

				HRESULT CMMIO::OpenFromFile(LPCTSTR pszFileName, DWORD uFlags)
				{
					ASSERT_VALID(this);

					if (!AfxIsValidString(pszFileName, MAX_PATH))
					{
						return E_INVALIDARG;
					}

					CMMIOInfo mmioInfo;
					m_hMMIO = mmioOpen(const_cast<LPTSTR>(pszFileName), &mmioInfo, uFlags);

					return MMRESULTToHRESULT(mmioInfo.wErrorRet);
				}

				HRESULT CMMIO::OpenFromMemory(CMMMemoryIOInfo& mmioInfo)
				{
					if (m_hMMIO = mmioOpen(NULL, &mmioInfo, MMIO_READWRITE); !m_hMMIO)
					{
						return MMRESULTToHRESULT(mmioInfo.wErrorRet);
					}

					return S_OK;
				}

				void CMMIO::Close(UINT uFlags)
				{
					if (m_hMMIO)
					{
						mmioClose(m_hMMIO, uFlags);
						m_hMMIO = NULL;
					}
				}

				HRESULT CMMIO::Ascend(CMMChunk& mmckInfo, UINT uFlags)
				{
					if (!m_hMMIO)
					{
						return E_FAIL;
					}

					if (const MMRESULT mmRes = mmioAscend(m_hMMIO, &mmckInfo, uFlags); MMSYSERR_NOERROR != mmRes)
					{
						return MMRESULTToHRESULT(mmRes);
					}

					return S_OK;
				}

				HRESULT CMMIO::Descend(CMMChunk& mmckInfo, UINT uFlags)
				{
					if (!m_hMMIO)
					{
						return E_FAIL;
					}

					if (const MMRESULT mmRes = mmioDescend(m_hMMIO, &mmckInfo, 0, uFlags); MMSYSERR_NOERROR != mmRes)
					{
						return MMRESULTToHRESULT(mmRes);
					}

					return S_OK;
				}

				HRESULT CMMIO::Descend(CMMChunk& mmckInfo, CMMChunk& mmckParent, UINT uFlags)
				{
					if (!m_hMMIO)
					{
						return E_FAIL;
					}

					if (const MMRESULT mmRes = mmioDescend(m_hMMIO, &mmckInfo, &mmckParent, uFlags); MMSYSERR_NOERROR != mmRes)
					{
						return MMRESULTToHRESULT(mmRes);
					}

					return S_OK;
				}

				LONG CMMIO::Read(HPSTR pData, LONG nLen)
				{
					if (!m_hMMIO)
					{
						return E_FAIL;
					}

					return mmioRead(m_hMMIO, pData, nLen);
				}

				LONG CMMIO::Write(LPCSTR pData, LONG nLen)
				{
					if (!m_hMMIO)
					{
						return E_FAIL;
					}

					return mmioWrite(m_hMMIO, pData, nLen);
				}

				LONG CMMIO::Seek(LONG nOffset, INT nOrigin)
				{
					if (!m_hMMIO)
					{
						return E_FAIL;
					}

					return mmioSeek(m_hMMIO, nOffset, nOrigin);
				}

				LRESULT CMMIO::SendMessage(UINT uMsg, LPARAM lParam1, LPARAM lParam2)
				{
					if (!m_hMMIO)
					{
						return E_FAIL;
					}

					return mmioSendMessage(m_hMMIO, uMsg, lParam1, lParam2);
				}

				HRESULT CMMIO::SetBuffer(LPSTR pBuffer, LONG nBuffer, UINT uFlags)
				{
					if (!m_hMMIO)
					{
						return E_FAIL;
					}

					if (const MMRESULT mmRes = mmioSetBuffer(m_hMMIO, pBuffer, nBuffer, uFlags); MMSYSERR_NOERROR != mmRes)
					{
						return MMRESULTToHRESULT(mmRes);
					}

					return S_OK;
				}

				HRESULT CMMIO::GetInfo(CMMIOInfo& mmioInfo, UINT uFlags)
				{
					if (!m_hMMIO)
					{
						return E_FAIL;
					}

					if (const MMRESULT mmRes = mmioGetInfo(m_hMMIO, &mmioInfo, uFlags); MMSYSERR_NOERROR != mmRes)
					{
						return MMRESULTToHRESULT(mmRes);
					}

					return S_OK;
				}

				HRESULT CMMIO::SetInfo(const CMMIOInfo& mmioInfo, UINT uFlags)
				{
					if (!m_hMMIO)
					{
						return E_FAIL;
					}

					if (const MMRESULT mmRes = mmioSetInfo(m_hMMIO, &mmioInfo, uFlags); MMSYSERR_NOERROR != mmRes)
					{
						return MMRESULTToHRESULT(mmRes);
					}

					return S_OK;
				}

				HRESULT CMMIO::Advance(CMMIOInfo& mmioInfo, UINT uFlags)
				{
					if (!m_hMMIO)
					{
						return E_FAIL;
					}

					if (const MMRESULT mmRes = mmioAdvance(m_hMMIO, &mmioInfo, uFlags); MMSYSERR_NOERROR != mmRes)
					{
						return MMRESULTToHRESULT(mmRes);
					}

					return S_OK;
				}

#pragma endregion
#pragma region Overridables

#ifdef _DEBUG

				void CMMIO::AssertValid() const
				{
					CObject::AssertValid();
				}

				void CMMIO::Dump(CDumpContext& dc) const
				{
					CObject::Dump(dc);
				}

#endif

#pragma endregion
#pragma region Implementations

				HRESULT CMMIO::MMRESULTToHRESULT(MMRESULT mmRes)
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

#pragma endregion

			}
		}
	}
}