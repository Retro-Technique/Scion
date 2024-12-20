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
#include "TextureFileResource.h"
#include "FontFileResource.h"
#include "SoundFileResource.h"
#include "VideoFileResource.h"

namespace scion
{
	namespace engine
	{

#pragma region Constructors

		IMPLEMENT_SERIAL(CResourceManager, CObject, 1);

		CResourceManager::CResourceManager()
		{
	
		}

		CResourceManager::~CResourceManager()
		{

		}

#pragma endregion
#pragma region Operations

		HRESULT CResourceManager::CreateResource(LPCTSTR pszName, LPCTSTR pszFileName)
		{
			ASSERT(AfxIsValidString(pszName));
			ASSERT(AfxIsValidString(pszFileName, MAX_PATH));
			
			if (IsExists(pszName))
			{
				return E_FAIL;
			}

			LPCTSTR pszExtension = PathFindExtension(pszFileName);
			CRuntimeClass* pRuntimeClass = CFileResource::GetRuntimeClassFromExt(pszExtension);
			if (!pRuntimeClass)
			{
				return E_FAIL; //TODO not found
			}

			CObject* pObject = pRuntimeClass->CreateObject();
			if (!pObject)
			{
				return E_OUTOFMEMORY;
			}

			ASSERT_KINDOF(CFileResource, pObject);
			CFileResource* pFileResource = STATIC_DOWNCAST(CFileResource, pObject);

			pFileResource->SetName(pszName);
			HRESULT hr = pFileResource->LoadFromFile(pszFileName);
			if (SUCCEEDED(hr))
			{
				m_mapResources.SetAt(pszName, pFileResource);
				pFileResource->AddRef();
			}

			pFileResource->Release();

			return hr;
		}

		BOOL CResourceManager::IsExists(LPCTSTR pszName) const
		{
			CObject* pObj = NULL;

			return m_mapResources.Lookup(pszName, pObj);
		}

		void CResourceManager::OnUpdate()
		{
			POSITION pos = m_mapResources.GetStartPosition();
			while (pos)
			{
				CString strName;
				CObject* pObject = NULL;
				m_mapResources.GetNextAssoc(pos, strName, pObject);
				if (pObject)
				{

				}
			}
		}

		void CResourceManager::EnumerateResources(ENUMRESOURCEPROC pfnEnumResource, LPVOID pData)
		{
			ASSERT(pfnEnumResource);

			POSITION pos = m_mapResources.GetStartPosition();
			while (pos)
			{
				CString strName;
				CObject* pObject = NULL;
				m_mapResources.GetNextAssoc(pos, strName, pObject);
				if (pObject)
				{
					const CFileResource* pResource = STATIC_DOWNCAST_CONST(CFileResource, pObject);
					pfnEnumResource(pResource->GetName(), pResource->GetType(), pResource->GetReferenceCount(), pData);
				}
			}
		}

		void CResourceManager::Clear()
		{
			POSITION pos = m_mapResources.GetStartPosition();
			while (pos)
			{
				CString strName;
				CObject* pObject = NULL;
				m_mapResources.GetNextAssoc(pos, strName, pObject);
				if (pObject)
				{
					const CFileResource* pResource = STATIC_DOWNCAST_CONST(CFileResource, pObject);
					pResource->Release();
				}
			}

			m_mapResources.RemoveAll();
		}

#pragma endregion
#pragma region Overridables

		void CResourceManager::Serialize(CArchive& ar)
		{
			CObject::Serialize(ar);
			
			m_mapResources.Serialize(ar);
		}

#ifdef _DEBUG

		void CResourceManager::AssertValid() const
		{
			CObject::AssertValid();

			ASSERT_VALID(&m_mapResources);
		}

		void CResourceManager::Dump(CDumpContext& dc) const
		{
			CObject::Dump(dc);

			AFXDUMP(&m_mapResources);
		}

#endif

#pragma endregion
	}
}