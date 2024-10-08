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
#include "Image.h"
#include "GraphicsManager.h"

namespace scion
{
	namespace engine
	{
		namespace gfx
		{
			namespace priv
			{

#pragma region Constructors

				IMPLEMENT_DYNAMIC(CImage, CObject)

				CImage::CImage(const CGraphicsManager* pGraphicsManager)
					: m_pGraphicsManager(pGraphicsManager)
					, m_pWICBitmap(NULL)
				{
					m_pGraphicsManager->AddRef();
				}

				CImage::~CImage()
				{
					Unload();

					if (m_pGraphicsManager)
					{
						m_pGraphicsManager->Release();
						m_pGraphicsManager = NULL;
					}
				}

#pragma endregion
#pragma region Operations

				HRESULT CImage::LoadFromFile(LPCTSTR pszFileName)
				{
					ASSERT_VALID(this);

					if (!AfxIsValidString(pszFileName, MAX_PATH))
					{
						return E_INVALIDARG;
					}

					Unload();

					HRESULT hr = S_OK;
					IWICStream* pStream = NULL;
					IWICBitmapDecoder* pDecoder = NULL;
					IWICBitmapFrameDecode* pFrame = NULL;
					IWICFormatConverter* pConverterFrame = NULL;
					
					do
					{
						if (hr = m_pGraphicsManager->WICCreateStream(&pStream); FAILED(hr))
						{
							break;
						}

						if (hr = pStream->InitializeFromFilename(pszFileName, GENERIC_READ); FAILED(hr))
						{
							break;
						}

						if (hr = m_pGraphicsManager->WICCreateDecoderFromFilename(pszFileName, &pDecoder); FAILED(hr))
						{
							break;
						}

						if (hr = pDecoder->GetFrame(0, &pFrame); FAILED(hr))
						{
							break;
						}

						if (hr = m_pGraphicsManager->WICCreateFormatConverter(&pConverterFrame); FAILED(hr))
						{
							break;
						}

						if (hr = pConverterFrame->Initialize(pFrame, GUID_WICPixelFormat32bppPRGBA, WICBitmapDitherTypeNone, NULL, 0., WICBitmapPaletteTypeCustom); FAILED(hr))
						{
							break;
						}

						if (hr = m_pGraphicsManager->WICCreateBitmapFromSource(pConverterFrame, &m_pWICBitmap); FAILED(hr))
						{
							break;
						}

					} while (SCION_NULL_WHILE_LOOP_CONDITION);

					if (pConverterFrame)
					{
						pConverterFrame->Release();
						pConverterFrame = NULL;
					}

					if (pFrame)
					{
						pFrame->Release();
						pFrame = NULL;
					}

					if (pDecoder)
					{
						pDecoder->Release();
						pDecoder = NULL;
					}

					if (pStream)
					{
						pStream->Release();
						pStream = NULL;
					}

					return hr;
				}

				HRESULT CImage::LoadFromMemory(LPCVOID pData, DWORD uSize)
				{
					ASSERT_VALID(this);

					if (!AfxIsValidAddress(pData, uSize, FALSE))
					{
						return E_INVALIDARG;
					}

					HRESULT hr = S_OK;
					IWICStream* pStream = NULL;
					IWICBitmapDecoder* pDecoder = NULL;
					IWICBitmapFrameDecode* pFrame = NULL;
					IWICFormatConverter* pConverterFrame = NULL;
	
					do
					{
						if (hr = m_pGraphicsManager->WICCreateStream(&pStream); FAILED(hr))
						{
							break;
						}

						if (hr = pStream->InitializeFromMemory(reinterpret_cast<BYTE*>(const_cast<LPVOID>(pData)), uSize); FAILED(hr))
						{
							break;
						}

						if (hr = m_pGraphicsManager->WICCreateDecoderFromStream(pStream, &pDecoder); FAILED(hr))
						{
							break;
						}

						if (hr = pDecoder->GetFrame(0, &pFrame); FAILED(hr))						
						{
							break;
						}

						if (hr = m_pGraphicsManager->WICCreateFormatConverter(&pConverterFrame); FAILED(hr))
						{
							break;
						}

						if (hr = pConverterFrame->Initialize(pFrame, GUID_WICPixelFormat32bppPRGBA, WICBitmapDitherTypeNone, NULL, 0., WICBitmapPaletteTypeCustom); FAILED(hr))
						{
							break;
						}

						if (hr = m_pGraphicsManager->WICCreateBitmapFromSource(pConverterFrame, &m_pWICBitmap); FAILED(hr))
						{
							break;
						}

					} while (SCION_NULL_WHILE_LOOP_CONDITION);

					if (pConverterFrame)
					{
						pConverterFrame->Release();
						pConverterFrame = NULL;
					}

					if (pFrame)
					{
						pFrame->Release();
						pFrame = NULL;
					}

					if (pDecoder)
					{
						pDecoder->Release();
						pDecoder = NULL;
					}

					if (pStream)
					{
						pStream->Release();
						pStream = NULL;
					}

					return hr;
				}

				HRESULT CImage::LoadFromResource(HMODULE hModule, LPCTSTR pszResourceName)
				{
					ASSERT_VALID(this);

					if (!hModule)
					{
						return E_INVALIDARG;
					}

					if (!AfxIsValidString(pszResourceName))
					{
						return E_INVALIDARG;
					}

					HRSRC hrSrc = FindResource(hModule, pszResourceName, RT_RCDATA);
					if (!hrSrc)
					{
						return HRESULT_FROM_WIN32(GetLastError());
					}

					HGLOBAL hGlobal = LoadResource(hModule, hrSrc);
					if (!hGlobal)
					{
						return HRESULT_FROM_WIN32(GetLastError());
					}

					const DWORD uSize = SizeofResource(hModule, hrSrc);
					if (!uSize)
					{
						return HRESULT_FROM_WIN32(GetLastError());
					}

					LPVOID pData = LockResource(hGlobal);
					if (!pData)
					{
						return HRESULT_FROM_WIN32(GetLastError());
					}

					HRESULT hr = LoadFromMemory(pData, uSize);

					UnlockResource(hGlobal);

					return hr;
				}

				HRESULT CImage::LoadFromResource(LPCTSTR pszModule, LPCTSTR pszResourceName)
				{
					ASSERT_VALID(this);

					if (!AfxIsValidString(pszModule, MAX_PATH))
					{
						return E_INVALIDARG;
					}

					if (!AfxIsValidString(pszResourceName))
					{
						return E_INVALIDARG;
					}

					HMODULE hModule = NULL;
					HRESULT hr = S_OK;

					do
					{
						hModule = AfxLoadLibrary(pszModule);
						if (!hModule)
						{
							hr = HRESULT_FROM_WIN32(GetLastError());
							break;
						}

						hr = LoadFromResource(hModule, pszResourceName);

					} while (SCION_NULL_WHILE_LOOP_CONDITION);

					if (hModule)
					{
						AfxFreeLibrary(hModule);
					}

					return hr;
				}

				HRESULT CImage::SaveToFile(LPCTSTR pszFileName)
				{
					if (!AfxIsValidString(pszFileName, MAX_PATH))
					{
						return E_INVALIDARG;
					}

					HRESULT hr = S_OK;
					IWICStream* pStream = NULL;
					IWICBitmapEncoder* pEncoder = NULL;
					IWICBitmapFrameEncode* pFrame = NULL;
					IPropertyBag2* pPropertyBag = NULL;
				
					do
					{
						if (hr = m_pGraphicsManager->WICCreateStream(&pStream); FAILED(hr))						
						{
							break;
						}

						if (hr = pStream->InitializeFromFilename(pszFileName, GENERIC_WRITE); FAILED(hr))
						
						{
							break;
						}

						if (hr = m_pGraphicsManager->WICCreateEncoder(&pEncoder); FAILED(hr))						
						{
							break;
						}

						if (hr = pEncoder->Initialize(pStream, WICBitmapEncoderNoCache); FAILED(hr))
						{
							break;
						}

						if (hr = pEncoder->CreateNewFrame(&pFrame, &pPropertyBag); FAILED(hr))		
						{
							break;
						}

						if (hr = pFrame->Initialize(pPropertyBag); FAILED(hr))
						
						{
							break;
						}

						if (hr = pFrame->WriteSource(m_pWICBitmap, NULL); FAILED(hr))
						{
							break;
						}

						if (hr = pFrame->Commit(); FAILED(hr))
						{
							break;
						}

						if (hr = pEncoder->Commit(); FAILED(hr))
						{
							break;
						}

					} while (SCION_NULL_WHILE_LOOP_CONDITION);

					if (pPropertyBag)
					{
						pPropertyBag->Release();
						pPropertyBag = NULL;
					}

					if (pFrame)
					{
						pFrame->Release();
						pFrame = NULL;
					}

					if (pEncoder)
					{
						pEncoder->Release();
						pEncoder = NULL;
					}

					if (pStream)
					{
						pStream->Release();
						pStream = NULL;
					}

					return hr;
				}

				void CImage::Unload()
				{
					if (m_pWICBitmap)
					{
						m_pWICBitmap->Release();
						m_pWICBitmap = NULL;
					}
				}

				IWICBitmap* CImage::Lock()
				{
					ASSERT_POINTER(m_pWICBitmap, IWICBitmap);

					m_pWICBitmap->AddRef();
					return m_pWICBitmap;
				}

				void CImage::Unlock()
				{
					ASSERT_POINTER(m_pWICBitmap, IWICBitmap);

					m_pWICBitmap->Release();
				}

#pragma endregion
#pragma region Overridables

#ifdef _DEBUG

				void CImage::AssertValid() const
				{
					CObject::AssertValid();

				}

				void CImage::Dump(CDumpContext& dc) const
				{
					CObject::Dump(dc);
				}

#endif

#pragma endregion

			}
		}
	}
}