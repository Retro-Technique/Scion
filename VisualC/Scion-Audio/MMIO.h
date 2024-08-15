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

#pragma once

#include "MMIOUtils.h"

namespace scion
{
	namespace engine
	{
		namespace sfx
		{
			namespace priv
			{
				
				class CMMIO : public CObject
				{
#pragma region Constructors

					DECLARE_DYNAMIC(CMMIO)

				public:

					CMMIO();
					virtual ~CMMIO();

#pragma endregion
#pragma region Attributes

				private:

					HMMIO m_hMMIO;

#pragma endregion
#pragma region Operations

				public:

					HRESULT OpenFromFile(LPCTSTR pszFileName, DWORD uFlags = MMIO_READ);
					HRESULT OpenFromMemory(CMMMemoryIOInfo& mmioInfo);
					void Close(UINT uFlags = 0u);
					HRESULT Ascend(CMMChunk& mmckInfo, UINT uFlags = 0u);
					HRESULT Descend(CMMChunk& mmckInfo, UINT uFlags = 0u);
					HRESULT Descend(CMMChunk& mmckInfo, CMMChunk& mmckParent, UINT uFlags = 0u);
					LONG Read(HPSTR pData, LONG nLen);
					LONG Write(LPCSTR pData, LONG nLen);
					LONG Seek(LONG nOffset, INT nOrigin);
					LRESULT SendMessage(UINT uMsg, LPARAM lParam1, LPARAM lParam2);
					HRESULT SetBuffer(LPSTR pBuffer, LONG nBuffer, UINT uFlags = 0u);
					HRESULT GetInfo(CMMIOInfo& mmioInfo, UINT uFlags = 0u);
					HRESULT SetInfo(const CMMIOInfo& mmioInfo, UINT uFlags = 0u);
					HRESULT Advance(CMMIOInfo& mmioInfo, UINT uFlags = 0u);

#pragma endregion
#pragma region Overridables

				public:

#ifdef _DEBUG
					void AssertValid() const override;
					void Dump(CDumpContext& dc) const override;
#endif

#pragma endregion
#pragma region Implementations

				private:

					HRESULT MMRESULTToHRESULT(MMRESULT mmRes);

#pragma endregion
				};

			}
		}
	}
}