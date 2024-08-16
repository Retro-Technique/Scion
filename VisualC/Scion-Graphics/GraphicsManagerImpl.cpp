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
#include "GraphicsManagerImpl.h"

namespace scion
{
	namespace engine
	{
		namespace gfx
		{
			namespace priv
			{

#pragma region Constructors

				CGraphicsManagerImpl CGraphicsManagerImpl::ms_Instance;

				IMPLEMENT_DYNAMIC(CGraphicsManagerImpl, CObject)

				CGraphicsManagerImpl::CGraphicsManagerImpl()
					: m_pD2DFactory(NULL)
					, m_pDWriteFactory(NULL)
					, m_pWICFactory(NULL)
				{

				}

				CGraphicsManagerImpl::~CGraphicsManagerImpl()
				{

				}

#pragma endregion
#pragma region Operations

				CGraphicsManagerImpl& CGraphicsManagerImpl::GetInstance()
				{
					return ms_Instance;
				}

				HRESULT CGraphicsManagerImpl::Initialize(_AFX_D2D_STATE* pD2DState)
				{
					if (!pD2DState)
					{
						return E_INVALIDARG;
					}

					if (!pD2DState->IsD2DInitialized())
					{
						return E_FAIL;
					}

					ASSERT_NULL_OR_POINTER(m_pD2DFactory, ID2D1Factory8);
					ASSERT_NULL_OR_POINTER(m_pDWriteFactory, IDWriteFactory8);
					ASSERT_NULL_OR_POINTER(m_pWICFactory, IWICImagingFactory2);

					ID2D1Factory* pD2DFactory = pD2DState->GetDirect2dFactory();
					HRESULT hr = pD2DFactory->QueryInterface(__uuidof(ID2D1Factory8), reinterpret_cast<void**>(&m_pD2DFactory));
					ASSERT(SUCCEEDED(hr));
					ASSERT_POINTER(m_pD2DFactory, ID2D1Factory8);

					IDWriteFactory* pDWFactory = pD2DState->GetWriteFactory();
					hr = pDWFactory->QueryInterface(__uuidof(IDWriteFactory8), reinterpret_cast<void**>(&m_pDWriteFactory));
					ASSERT(SUCCEEDED(hr));
					ASSERT_POINTER(m_pDWriteFactory, IDWriteFactory8);

					IWICImagingFactory* pWICFactory = pD2DState->GetWICFactory();
					hr = pWICFactory->QueryInterface(__uuidof(IWICImagingFactory2), reinterpret_cast<void**>(&m_pWICFactory));
					ASSERT(SUCCEEDED(hr));
					ASSERT_POINTER(m_pWICFactory, IWICImagingFactory2);

					return hr;
				}

				void CGraphicsManagerImpl::Quit()
				{
					if (m_pWICFactory)
					{
						m_pWICFactory->Release();
						m_pWICFactory = NULL;
					}

					if (m_pDWriteFactory)
					{
						m_pDWriteFactory->Release();
						m_pDWriteFactory = NULL;
					}

					if (m_pD2DFactory)
					{
						m_pD2DFactory->Release();
						m_pD2DFactory = NULL;
					}
				}

#pragma endregion
#pragma region Overridables

#ifdef _DEBUG

				void CGraphicsManagerImpl::AssertValid() const
				{
					CObject::AssertValid();

					ASSERT_POINTER(m_pD2DFactory, ID2D1Factory8);
					ASSERT_POINTER(m_pDWriteFactory, IDWriteFactory8);
					ASSERT_POINTER(m_pWICFactory, IWICImagingFactory2);
				}

				void CGraphicsManagerImpl::Dump(CDumpContext& dc) const
				{
					CObject::Dump(dc);
				}

#endif

#pragma endregion

			}
		}
	}
}