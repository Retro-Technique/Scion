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
#include "Font.h"
#include "GraphicsManager.h"

namespace scion
{
	namespace engine
	{
		namespace gfx
		{

#pragma region Constructors

			IMPLEMENT_DYNAMIC(CFont, CObject)

			CFont::CFont(const CGraphicsManager* pGraphicsManager)
				: m_nRef(1)
				, m_pGraphicsManager(pGraphicsManager)
			{
				m_pGraphicsManager->AddRef();
			}

			CFont::~CFont()
			{
				Unload();

				if (m_pGraphicsManager)
				{
					m_pGraphicsManager->Release();
					m_pGraphicsManager = NULL;
				}
			}

#pragma endregion
#pragma region Overridables

			HRESULT CFont::LoadFromFile(LPCTSTR pszFileName)
			{
				ASSERT_VALID(this);

				if (!AfxIsValidString(pszFileName, MAX_PATH))
				{
					return E_INVALIDARG;
				}
				
				HRESULT hr = S_OK;
				IDWriteFontFile* pDWFontFile = NULL;
				IDWriteFontSetBuilder2* pDWFontSetBuilder = NULL;
				IDWriteFontSet* pDWFontSet = NULL;

				do
				{
					if (hr = m_pGraphicsManager->DWCreateFontFileReference(pszFileName, &pDWFontFile); FAILED(hr))
					{
						break;
					}

					if (hr = m_pGraphicsManager->DWCreateFontSetBuilder(&pDWFontSetBuilder); FAILED(hr))
					{
						break;
					}

					if (hr = pDWFontSetBuilder->AddFontFile(pDWFontFile); FAILED(hr))
					{
						break;
					}

					if (hr = pDWFontSetBuilder->CreateFontSet(&pDWFontSet); FAILED(hr))
					{
						break;
					}

					/*pDWriteFactory->CreateFontCollectionFromFontSet(fontSet, &fontCollection);*/

				} while (SCION_NULL_WHILE_LOOP_CONDITION);

				if (pDWFontSet)
				{
					pDWFontSet->Release();
					pDWFontSet = NULL;
				}

				if (pDWFontSetBuilder)
				{
					pDWFontSetBuilder->Release();
					pDWFontSetBuilder = NULL;
				}

				if (pDWFontFile)
				{
					pDWFontFile->Release();
					pDWFontFile = NULL;
				}

				if (FAILED(hr))
				{
					Unload();
				}

				return hr;
			}

			void CFont::Unload()
			{
				
			}

#ifdef _DEBUG

			void CFont::AssertValid() const
			{
				CObject::AssertValid();

				ASSERT_POINTER(m_pGraphicsManager, CGraphicsManager);
				ASSERT_VALID(m_pGraphicsManager);
			}

			void CFont::Dump(CDumpContext& dc) const
			{
				CObject::Dump(dc);
			}

#endif

			void CFont::AddRef() const
			{
				InterlockedIncrement(&m_nRef);
			}

			BOOL CFont::Release() const
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

		}
	}
}