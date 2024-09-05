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
#include "FileResource.h"
#include "TextureFileResource.h"
#include "FontFileResource.h"
#include "SoundFileResource.h"
#include "VideoFileResource.h"

namespace scion
{
	namespace engine
	{

#pragma region Constructors

		IMPLEMENT_SERIAL(CFileResource, CObject, 1)

		CFileResource::CFileResource()
			: m_nRef(1)
		{

		}

		CFileResource::~CFileResource()
		{

		}

#pragma endregion
#pragma region Operations

		CRuntimeClass* CFileResource::GetRuntimeClassFromExt(LPCTSTR pszExt)
		{
			ASSERT(AfxIsValidString(pszExt));

			static constexpr const INT_PTR EXTENSION_COUNT = 4;
			static struct
			{
				CRuntimeClass*	_pRuntimeClass;
				LPCTSTR			_aExtensions[EXTENSION_COUNT];

			} RUNTIME_CLASSES[] =
			{
				{ CTextureFileResource::GetThisClass(), { _T("bmp"), _T("png"), _T("jpg"), _T("jpeg") } },
				{ CFontFileResource::GetThisClass(), { _T("ttf"), NULL, NULL, NULL } },
				{ CSoundFileResource::GetThisClass(), { _T("wav"), _T("wave"), NULL, NULL } },
				{ CVideoFileResource::GetThisClass(), { _T("avi"), NULL, NULL, NULL } },
			};
			static constexpr const INT RUNTIME_CLASS_COUNT = ARRAYSIZE(RUNTIME_CLASSES);
			C_ASSERT(RUNTIME_CLASS_COUNT == CResourceManager::EResourceType_COUNT);

			for (INT_PTR i = 0; i < RUNTIME_CLASS_COUNT; i++)
			{
				for (INT_PTR j = 0; j < EXTENSION_COUNT; j++)
				{
					if (StrCmp(pszExt, RUNTIME_CLASSES[i]._aExtensions[j]) == 0)
					{
						return RUNTIME_CLASSES[i]._pRuntimeClass;
					}
				}
			}

			return NULL;
		}

#pragma endregion
#pragma region Overridables

		HRESULT CFileResource::LoadFromFile(LPCTSTR pszFileName)
		{
			return E_NOTIMPL;
		}

		void CFileResource::Unload()
		{

		}

		CResourceManager::EResourceType CFileResource::GetType() const
		{
			return CResourceManager::EResourceType_COUNT;
		}

		void CFileResource::Serialize(CArchive& ar)
		{
			CObject::Serialize(ar);

			if (ar.IsStoring())
			{
				ar << m_strName;
			}
			else
			{
				ar >> m_strName;
			}
		}

#ifdef _DEBUG

		void CFileResource::AssertValid() const
		{
			CObject::AssertValid();

		}

		void CFileResource::Dump(CDumpContext& dc) const
		{
			CObject::Dump(dc);

		}

#endif

		void CFileResource::AddRef() const
		{
			InterlockedIncrement(&m_nRef);
		}

		BOOL CFileResource::Release() const
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