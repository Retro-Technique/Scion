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

		IMPLEMENT_SERIAL(CGameEngine, CObject, 1);

		CGameEngine::CGameEngine()
		{
			
		}

		CGameEngine::~CGameEngine()
		{

		}

#pragma endregion
#pragma region Operations

		HRESULT CGameEngine::Initialize(CWnd* pMainWnd, _AFX_D2D_STATE* pD2DState)
		{
			HRESULT hr = S_OK;

			do
			{
				hr = m_GraphicsManager.Initialize(pD2DState);
				if (FAILED(hr))
				{
					break;
				}

				hr = m_AudioManager.Initialize(pMainWnd);
				if (FAILED(hr))
				{
					break;
				}

				hr = m_VideoManager.Initialize();
				if (FAILED(hr))
				{
					break;
				}

			} while (SCION_NULL_WHILE_LOOP_CONDITION);

			return hr;
		}

		void CGameEngine::Quit()
		{
			m_VideoManager.Quit();
			m_AudioManager.Quit();
			m_GraphicsManager.Quit();
		}

		void CGameEngine::InitialUpdate()
		{

		}

		void CGameEngine::Update()
		{
//			const ULONGLONG uElapsedTime = m_GameTime.Restart();

			
		}

		void CGameEngine::Draw()
		{

		}

		void CGameEngine::Unload()
		{
			DestroyManagers();
		}

#pragma endregion
#pragma region Overridables

		void CGameEngine::Serialize(CArchive& ar)
		{
			CObject::Serialize(ar);

			m_listManagers.Serialize(ar);
		}

#ifdef _DEBUG

		void CGameEngine::AssertValid() const
		{
			CObject::AssertValid();

			ASSERT_VALID(&m_listManagers);
		}

		void CGameEngine::Dump(CDumpContext& dc) const
		{
			CObject::Dump(dc);
		}

#endif

#pragma endregion
#pragma region Implementations

		HRESULT CGameEngine::CreateManagers()
		{
			static constexpr LPCTSTR MANAGER_NAMES[] =
			{
				_T("CResourceManager"),
				_T("CSceneManager")
			};
			static constexpr const SIZE_T MANAGER_NAME_COUNT = ARRAYSIZE(MANAGER_NAMES);

			for (UINT i = 0; i < MANAGER_NAME_COUNT; i++)
			{
				CRuntimeClass* pRuntimeClass = CRuntimeClass::FromName(MANAGER_NAMES[i]);
				if (!pRuntimeClass)
				{
					return E_ABORT;
				}

 				CObject* pObject = pRuntimeClass->CreateObject();
				if (!pObject)
				{
					return E_OUTOFMEMORY;
				}

				m_listManagers.AddTail(pObject);
			}

			return S_OK;
		}

		void CGameEngine::DestroyManagers()
		{
			POSITION pos = m_listManagers.GetHeadPosition();
			while (pos)
			{
				CObject* pObject = m_listManagers.GetNext(pos);
				ASSERT_POINTER(pObject, CObject);
				ASSERT_KINDOF(CManager, pObject);
				ASSERT_VALID(pObject);
				
				delete pObject;
			}

			m_listManagers.RemoveAll();
		}

#pragma endregion
	}
}