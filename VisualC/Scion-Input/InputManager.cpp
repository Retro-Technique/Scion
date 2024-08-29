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
#pragma region Operations

			HRESULT CInputManager::AcquireJoystick(LPCDIDEVICEINSTANCE pdidInstance)
			{
				LPDIRECTINPUTDEVICE8 pInput = NULL;

				if (const HRESULT hr = m_pDevice->CreateDevice(pdidInstance->guidInstance, &pInput, NULL); FAILED(hr))
				{
					return DIENUM_CONTINUE;
				}

				return DIENUM_STOP;
			}

			HRESULT CInputManager::CreateInput(LPCDIDEVICEINSTANCE pdidInstance)
			{
				LPDIRECTINPUTDEVICE8 pInput = NULL;

				if (const HRESULT hr = m_pDevice->CreateDevice(pdidInstance->guidInstance, &pInput, NULL); FAILED(hr))
				{
					return hr;
				}

				m_listInputs.AddTail(pInput);

				return S_OK;
			}

#pragma endregion
#pragma region Overridables

			HRESULT CInputManager::Initialize(HINSTANCE hInstance, CWnd* pWnd, BOOL bKeyboard, BOOL bMouse)
			{
				ASSERT_POINTER(hInstance, HINSTANCE);
				ASSERT_POINTER(pWnd, CWnd);
				ASSERT_VALID(pWnd);

				HRESULT hr = S_OK;
				const auto pfnOnDevice = [](LPCDIDEVICEINSTANCE pdidInstance, LPVOID pData) -> BOOL
					{
						CInputManager* pInputManager = reinterpret_cast<CInputManager*>(pData);

						if (const HRESULT hr = pInputManager->CreateInput(pdidInstance); FAILED(hr))
						{
							return DIENUM_CONTINUE;
						}

						return DIENUM_STOP;
					};

				do
				{
					if (hr = DirectInput8Create(hInstance,
						DIRECTINPUT_VERSION,
						IID_IDirectInput8,
						reinterpret_cast<void**>(&m_pDevice),
						NULL); FAILED(hr))
					{
						break;
					}

					if (hr = m_pDevice->EnumDevices(DI8DEVCLASS_ALL, pfnOnDevice, this, DIEDFL_ATTACHEDONLY); FAILED(hr))
					{
						break;
					}
					
					/*if (hr = AcquireDefaultInput(pWnd, bKeyboard, bMouse); FAILED(hr))
					{
						break;
					}*/

				} while (SCION_NULL_WHILE_LOOP_CONDITION);

				return hr;
			}

			void CInputManager::Quit()
			{
				for (INT_PTR i = 0; i < m_arrInputs.GetCount(); i++)
				{
					LPDIRECTINPUTDEVICE8 pInput = m_arrInputs.GetAt(i);
					if (pInput)
					{
						pInput->Unacquire();
						pInput->Release();
						pInput = NULL;
					}
				}

				m_arrInputs.RemoveAll();

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
#pragma region Implementations

			HRESULT CInputManager::AcquireDefaultInput(CWnd* pWnd, BOOL bKeyboard, BOOL bMouse)
			{
				struct
				{
					const GUID& _guid;
					const LPCDIDATAFORMAT pFormat;
					const BOOL bActive;

				} Inputs[] =
				{
					{ GUID_SysKeyboard, &c_dfDIKeyboard, bKeyboard },
					{ GUID_SysMouse, &c_dfDIMouse, bMouse }
				};
				static constexpr const INT INPUT_COUNT = ARRAYSIZE(Inputs);

				HRESULT hr = S_OK;

				for (INT i = 0; i < INPUT_COUNT; i++)
				{
					if (!Inputs[i].bActive)
					{
						continue;
					}

					LPDIRECTINPUTDEVICE8 pInput = NULL;

					if (hr = m_pDevice->CreateDevice(Inputs[i]._guid, &pInput, NULL); FAILED(hr))
					{
						break;
					}

					if (hr = pInput->SetDataFormat(Inputs[i].pFormat); FAILED(hr))
					{
						break;
					}

					if (hr = pInput->SetCooperativeLevel(pWnd->GetSafeHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); FAILED(hr))
					{
						break;
					}

					if (hr = pInput->Acquire(); FAILED(hr))
					{
						break;
					}

					m_arrInputs.Add(pInput);
				}

				return hr;
			}

			HRESULT CInputManager::AcquireJoysticks(CWnd* pWnd)
			{
				/*const auto pfnOnDevice = [](LPCDIDEVICEINSTANCE pdidInstance, LPVOID pData) -> BOOL
					{
						CInputManager* pInputManager = reinterpret_cast<CInputManager*>(pData);

						pInputManager->AcquireJoystick(pdidInstance);

						return 
					};

				return m_pDevice->EnumDevices(DI8DEVCLASS_GAMECTRL, pfnOnDevice, NULL, DIEDFL_ATTACHEDONLY);*/
				return E_NOTIMPL;
			}

			HRESULT CInputManager::AcquireInputs(CWnd* pWnd)
			{
				HRESULT hr = S_OK;

				POSITION pos = m_listInputs.GetHeadPosition();
				while (pos)
				{
					LPDIRECTINPUTDEVICE8 pInput = m_listInputs.GetNext(pos);
					if (!pInput)
					{
						continue;
					}

					/*if (hr = pInput->SetDataFormat(Inputs[i].pFormat); FAILED(hr))
					{
						continue;
					}*/

					if (hr = pInput->SetCooperativeLevel(pWnd->GetSafeHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); FAILED(hr))
					{
						continue;
					}

					if (hr = pInput->Acquire(); FAILED(hr))
					{
						continue;
					}
				}

				return hr;
			}

#pragma endregion
		}
	}
}