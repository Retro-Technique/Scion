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
#include "Renderer.h"

namespace scion
{
	namespace gfx
	{

		IRenderer* IRenderer::Create(CHwndRenderTarget* pRenderTarget)
		{
			return new CRenderer(pRenderTarget);
		}

		void IRenderer::Free(IRenderer* piRenderer)
		{
			CRenderer* pRenderer = reinterpret_cast<CRenderer*>(piRenderer);
			delete pRenderer;
		}

#pragma region Constructors

		IMPLEMENT_DYNAMIC(CRenderer, CObject);

		CRenderer::CRenderer(CHwndRenderTarget* pHwndRenderTarget)
			: m_pDeviceContext(NULL)
		{
			QueryDeviceContext(pHwndRenderTarget);
		}

		CRenderer::~CRenderer()
		{
			if (m_pDeviceContext)
			{
				m_pDeviceContext->Release();
				m_pDeviceContext = NULL;
			}
		}

#pragma endregion
#pragma region Overridables

		void CRenderer::Clear(const D2D1_COLOR_F& clrClear)
		{
			ASSERT_VALID(this);

			m_pDeviceContext->Clear(clrClear);
		}

		void CRenderer::SetTransform(const D2D1::Matrix3x2F& mMatrix)
		{
			ASSERT_VALID(this);
			
			m_pDeviceContext->SetTransform(mMatrix);
		}

		void CRenderer::DrawBitmap()
		{
			ASSERT_VALID(this);

			// Ressource : ID2D1Bitmap
			// RECT Destination
			// FLOAT Opacity
			// INTERPOLATION_MODE
			// RECT Source
			m_pDeviceContext->DrawBitmap(NULL, NULL, 1.f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
		}

		void CRenderer::DrawGeometry()
		{
			ASSERT_VALID(this);
			
			// Ressource : ID2D1Geometry
			// Ressource : ID2D1Brush
			// FLOAT StrokeWidth
			// Ressource : ID2D1StrokeStyle
			m_pDeviceContext->DrawGeometry(NULL, NULL, 1.f, NULL);
		}

		void CRenderer::FillGeometry()
		{
			ASSERT_VALID(this);

			// Ressource : ID2D1Geometry
			// Ressource : ID2D1Brush
			// Ressource : ID2D1Brush Opacity
			m_pDeviceContext->FillGeometry(NULL, NULL, NULL);
		}

		void CRenderer::DrawText()
		{
			ASSERT_VALID(this);
			
			// CString texte
			// Ressource : IDWriteTextFormat
			// RECT Layout
			// Ressource : ID2D1Brush DefaultFill
			// TEXT_OPTIONS
			// MEASURING_MODE
			m_pDeviceContext->DrawText(_T(""), 0, NULL, NULL, NULL, D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);
		}

		void CRenderer::DrawSpriteBatch()
		{
			ASSERT_VALID(this);
			
			// Ressource : ID2D1SpriteBatch
			// UINT StartIndex
			// UINT SpriteCount
			// Ressource : ID2D1Bitmap
			// INTERPOLATION_MODE
			// SPRITE_OPTIONS
			m_pDeviceContext->DrawSpriteBatch(NULL, 0, 0, NULL, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, D2D1_SPRITE_OPTIONS_NONE);
		}

#if defined(_DEBUG) || defined(_AFXDLL)

		void CRenderer::AssertValid() const
		{
			CObject::AssertValid();

			ASSERT_POINTER(m_pDeviceContext, ID2D1DeviceContext7);
		}

		void CRenderer::Dump(CDumpContext& dc) const
		{
			CObject::Dump(dc);

		}

#endif

#pragma endregion
#pragma region Implementations

		void CRenderer::QueryDeviceContext(CHwndRenderTarget* pHwndRenderTarget)
		{
			ID2D1DeviceContext7* pD2DDeviceContext = NULL;

			ASSERT_POINTER(pHwndRenderTarget, CHwndRenderTarget);
			ASSERT_VALID(pHwndRenderTarget);

			ID2D1HwndRenderTarget* pD2DRenderTarget = pHwndRenderTarget->GetHwndRenderTarget();
			ASSERT_POINTER(pD2DRenderTarget, ID2D1HwndRenderTarget);

			HRESULT hr = pD2DRenderTarget->QueryInterface(__uuidof(ID2D1DeviceContext7), reinterpret_cast<void**>(&pD2DDeviceContext));
			ASSERT(SUCCEEDED(hr));
			ASSERT_POINTER(pD2DDeviceContext, ID2D1DeviceContext7);

			m_pDeviceContext = pD2DDeviceContext;
		}

#pragma endregion
	}
}