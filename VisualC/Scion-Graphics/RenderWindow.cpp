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
#include "RenderWindow.h"
#include "GraphicsManager.h"
#include "Texture.h"

namespace scion
{
	namespace engine
	{
		namespace gfx
		{

#pragma region Constructors

			IMPLEMENT_DYNAMIC(CRenderWindow, CObject)

			CRenderWindow::CRenderWindow(const CGraphicsManager* pGraphicsManager)
				: m_nRef(1)
				, m_pGraphicsManager(pGraphicsManager)
				, m_pD2DDeviceContext(NULL)
			{
				m_pGraphicsManager->AddRef();
			}

			CRenderWindow::~CRenderWindow()
			{
				Destroy();

				if (m_pGraphicsManager)
				{
					m_pGraphicsManager->Release();
					m_pGraphicsManager = NULL;
				}
			}

#pragma endregion
#pragma region Operations

			HRESULT CRenderWindow::CreateBitmapFromWicBitmap(IWICBitmapSource* pBitmapSource, ID2D1Bitmap** ppBitmap) const
			{
				ASSERT_VALID(this);
				ASSERT_POINTER(pBitmapSource, IWICBitmapSource);
				ASSERT_NULL_OR_POINTER(*ppBitmap, ID2D1Bitmap);
				
				return m_pD2DDeviceContext->CreateBitmapFromWicBitmap(pBitmapSource, ppBitmap);
			}

#pragma endregion
#pragma region Overridables

			HRESULT CRenderWindow::CreateFromWindow(CWnd* pWnd)
			{
				if (!pWnd)
				{
					return E_INVALIDARG;
				}

				ASSERT_VALID(pWnd);

				CHwndRenderTarget* pHwndRenderTarget = pWnd->GetRenderTarget();
				if (!pHwndRenderTarget)
				{
					return E_POINTER;
				}

				ASSERT_VALID(pHwndRenderTarget);

				ID2D1HwndRenderTarget* pD2DRenderTarget = pHwndRenderTarget->GetHwndRenderTarget();
				ASSERT_POINTER(pD2DRenderTarget, ID2D1HwndRenderTarget);

				return pD2DRenderTarget->QueryInterface(__uuidof(ID2D1DeviceContext7), reinterpret_cast<void**>(&m_pD2DDeviceContext));
			}

			void CRenderWindow::Destroy()
			{
				if (m_pD2DDeviceContext)
				{
					m_pD2DDeviceContext->Release();
					m_pD2DDeviceContext = NULL;
				}
			}

			HRESULT CRenderWindow::CreateTexture(ITexture** ppTexture) const
			{
				ASSERT_POINTER(this, CRenderWindow);
				ASSERT_VALID(this);
				ASSERT_NULL_OR_POINTER(*ppTexture, ITexture);

				ITexture* pTexture = new CTexture(this);
				if (!pTexture)
				{
					return E_OUTOFMEMORY;
				}

				*ppTexture = pTexture;

				return S_OK;
			}

#ifdef _DEBUG

			void CRenderWindow::AssertValid() const
			{
				CObject::AssertValid();

				ASSERT_POINTER(m_pGraphicsManager, CGraphicsManager);
				ASSERT_VALID(m_pGraphicsManager);
				ASSERT_POINTER(m_pD2DDeviceContext, ID2D1DeviceContext7);
			}

			void CRenderWindow::Dump(CDumpContext& dc) const
			{
				CObject::Dump(dc);
			}

#endif

			void CRenderWindow::AddRef() const
			{
				InterlockedIncrement(&m_nRef);
			}

			BOOL CRenderWindow::Release() const
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