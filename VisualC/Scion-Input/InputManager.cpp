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
#include "InputManager.h"

namespace scion
{
	namespace engine
	{
		namespace ifx
		{

#pragma region Constructors

			HRESULT CreateInputManager(IInputManager** ppInputManager)
			{
				ASSERT_NULL_OR_POINTER(*ppInputManager, IInputManager);

				IInputManager* pInputManager = new CInputManager;
				if (!pInputManager)
				{
					return E_OUTOFMEMORY;
				}

				*ppInputManager = pInputManager;

				return S_OK;
			}

			IMPLEMENT_DYNAMIC(CInputManager, CObject)

			CInputManager::CInputManager()
				: m_nRef(1)
				, m_pDevice(NULL)
			{

			}

			CInputManager::~CInputManager()
			{

			}

#pragma endregion
#pragma region Overridables

			HRESULT CInputManager::Initialize(HINSTANCE hInstance)
			{
				ASSERT_POINTER(hInstance, HINSTANCE);

				HRESULT hr = DirectInput8Create(hInstance,
					DIRECTINPUT_VERSION,
					IID_IDirectInput8,
					reinterpret_cast<void**>(&m_pDevice),
					NULL);

				return hr;
			}

			void CInputManager::Quit()
			{
				if (m_pDevice)
				{
					m_pDevice->Release();
					m_pDevice = NULL;
				}
			}

#ifdef _DEBUG

			void CInputManager::AssertValid() const
			{
				CObject::AssertValid();

			}

			void CInputManager::Dump(CDumpContext& dc) const
			{
				CObject::Dump(dc);
			}

#endif

			void CInputManager::AddRef() const
			{
				InterlockedIncrement(&m_nRef);
			}

			BOOL CInputManager::Release() const
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