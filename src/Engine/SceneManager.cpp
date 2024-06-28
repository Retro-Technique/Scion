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

#pragma region Constructors

		IMPLEMENT_SERIAL(CSceneManager, CManager, 1);

		CSceneManager::CSceneManager()
		{

		}

		CSceneManager::~CSceneManager()
		{

		}

#pragma endregion
#pragma region Operations

		HRESULT CSceneManager::LoadScene(LPCTSTR pszScene)
		{
			if (!AfxIsValidString(pszScene))
			{
				return E_INVALIDARG;
			}

			// TODO: lever un flag demande de changement de scène
			// TODO: changer la scène plus tard

			/*CObject* pObject = NULL;
			BOOL bFound = m_mapScenes.Lookup(pszScene, pObject);
			if (!bFound)
			{
				return E_ABORT;
			}

			ASSERT_POINTER(pObject, CObject);
			ASSERT_KINDOF(CScene, pObject);

			CScene* pScene = STATIC_DOWNCAST(CScene, pObject);*/

			return S_OK;
		}

#pragma endregion
#pragma region Overridables

		HRESULT CSceneManager::OnSetup()
		{
			return S_OK;
		}

		void CSceneManager::OnLoop()
		{
			
		}

		void CSceneManager::Serialize(CArchive& ar)
		{
			CManager::Serialize(ar);

			if (ar.IsStoring())
			{
				ar << m_strCurrentScene;
			}
			else
			{
				ar >> m_strCurrentScene;
			}

			m_mapScenes.Serialize(ar);
		}

#ifdef _DEBUG

		void CSceneManager::AssertValid() const
		{
			CManager::AssertValid();
		}

		void CSceneManager::Dump(CDumpContext& dc) const
		{
			CManager::Dump(dc);
		}

#endif

#pragma endregion
	}
}