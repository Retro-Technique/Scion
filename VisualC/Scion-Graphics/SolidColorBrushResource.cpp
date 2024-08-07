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
#include "SolidColorBrushResource.h"

namespace scion
{
	namespace gfx
	{

#pragma region Constructors

		IMPLEMENT_SERIAL(CSolidColorBrushResource, CBrushResource, 1);

		CSolidColorBrushResource::CSolidColorBrushResource()
			: m_clrBrush(RGB(0, 0, 0))
		{
			
		}

		CSolidColorBrushResource::~CSolidColorBrushResource()
		{

		}

#pragma endregion
#pragma region Overridables

		void CSolidColorBrushResource::SetProperty(INT nProperty, const COleVariant& varValue)
		{
			CBrushResource::SetProperty(nProperty, varValue);

			switch (nProperty)
			{
			case EProperty_Color:
				SetColor(varValue.uintVal);
				break;
			default:
				break;
			}
		}

		HRESULT CSolidColorBrushResource::Load(ID2D1HwndRenderTarget* pRenderTarget)
		{
			ASSERT_POINTER(pRenderTarget, ID2D1HwndRenderTarget);
			
			if (!m_pBrush)
			{
				ID2D1SolidColorBrush* pSolidColorBrush = NULL;
				HRESULT hr = pRenderTarget->CreateSolidColorBrush(m_clrBrush, m_propBrush, &pSolidColorBrush);
				if (SUCCEEDED(hr))
				{
					m_pBrush = pSolidColorBrush;
				}
				
				return hr;
			}
	
			if (m_pBrush)
			{
				ID2D1SolidColorBrush* pSolidColorBrush = static_cast<ID2D1SolidColorBrush*>(m_pBrush);

				m_pBrush->SetOpacity(m_propBrush.opacity);
				pSolidColorBrush->SetColor(m_clrBrush);
			}

			return S_OK;
		}

		void CSolidColorBrushResource::Serialize(CArchive& ar)
		{
			CBrushResource::Serialize(ar);

			if (ar.IsStoring())
			{
				ar << m_clrBrush.r << m_clrBrush.g << m_clrBrush.b << m_clrBrush.a;
			}
			else
			{
				ar >> m_clrBrush.r >> m_clrBrush.g >> m_clrBrush.b >> m_clrBrush.a;
			}
		}

#ifdef _DEBUG

		void CSolidColorBrushResource::AssertValid() const
		{
			CBrushResource::AssertValid();

		}

		void CSolidColorBrushResource::Dump(CDumpContext& dc) const
		{
			CBrushResource::Dump(dc);

			dc << _T("Color: (") << m_clrBrush.r << _T(";") << m_clrBrush.g << _T(";") << m_clrBrush.b << _T(";") << m_clrBrush.a << _T(")\n");
		}

#endif

#pragma endregion
	}
}