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

				CImage::CImage()
					: m_pWICBitmap(NULL)
				{

				}

				CImage::~CImage()
				{
					Unload();
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
					IWICImagingFactory2* pFactory = /*GraphicsManager.GetWICFactory()*/NULL; //TODO

					do
					{
						hr = pFactory->CreateStream(&pStream);
						if (FAILED(hr))
						{
							break;
						}

						hr = pStream->InitializeFromFilename(pszFileName, GENERIC_READ);
						if (FAILED(hr))
						{
							break;
						}

						hr = pFactory->CreateDecoderFromFilename(pszFileName, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder);
						if (FAILED(hr))
						{
							break;
						}

						hr = pDecoder->GetFrame(0, &pFrame);
						if (FAILED(hr))
						{
							break;
						}

						hr = pFactory->CreateFormatConverter(&pConverterFrame);
						if (FAILED(hr))
						{
							break;
						}

						hr = pConverterFrame->Initialize(pFrame,
							GUID_WICPixelFormat32bppPRGBA,
							WICBitmapDitherTypeNone,
							NULL,
							0.,
							WICBitmapPaletteTypeCustom);
						if (FAILED(hr))
						{
							break;
						}

						hr = pFactory->CreateBitmapFromSource(pConverterFrame, WICBitmapCacheOnDemand, &m_pWICBitmap);
						if (FAILED(hr))
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
					IWICImagingFactory2* pFactory = /*GraphicsManager.GetWICFactory()*/NULL; //TODO

					do
					{
						hr = pFactory->CreateStream(&pStream);
						if (FAILED(hr))
						{
							break;
						}

						hr = pStream->InitializeFromMemory(reinterpret_cast<BYTE*>(const_cast<LPVOID>(pData)), uSize);
						if (FAILED(hr))
						{
							break;
						}

						hr = pFactory->CreateDecoderFromStream(pStream, NULL, WICDecodeMetadataCacheOnDemand, &pDecoder);
						if (FAILED(hr))
						{
							break;
						}

						hr = pDecoder->GetFrame(0, &pFrame);
						if (FAILED(hr))
						{
							break;
						}

						hr = pFactory->CreateFormatConverter(&pConverterFrame);
						if (FAILED(hr))
						{
							break;
						}

						hr = pConverterFrame->Initialize(pFrame,
							GUID_WICPixelFormat32bppPRGBA,
							WICBitmapDitherTypeNone,
							NULL,
							0.,
							WICBitmapPaletteTypeCustom);
						if (FAILED(hr))
						{
							break;
						}

						hr = pFactory->CreateBitmapFromSource(pConverterFrame, WICBitmapCacheOnDemand, &m_pWICBitmap);
						if (FAILED(hr))
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
					IWICImagingFactory2* pFactory = /*GraphicsManager.GetWICFactory()*/NULL; //TODO

					do
					{
						hr = pFactory->CreateStream(&pStream);
						if (FAILED(hr))
						{
							break;
						}

						hr = pStream->InitializeFromFilename(pszFileName, GENERIC_WRITE);
						if (FAILED(hr))
						{
							break;
						}

						hr = pFactory->CreateEncoder(GUID_ContainerFormatPng, NULL, &pEncoder);
						if (FAILED(hr))
						{
							break;
						}

						hr = pEncoder->Initialize(pStream, WICBitmapEncoderNoCache);
						if (FAILED(hr))
						{
							break;
						}

						hr = pEncoder->CreateNewFrame(&pFrame, &pPropertyBag);
						if (FAILED(hr))
						{
							break;
						}

						hr = pFrame->Initialize(pPropertyBag);
						if (FAILED(hr))
						{
							break;
						}

						hr = pFrame->WriteSource(m_pWICBitmap, NULL);
						if (FAILED(hr))
						{
							break;
						}

						hr = pFrame->Commit();
						if (FAILED(hr))
						{
							break;
						}

						hr = pEncoder->Commit();
						if (FAILED(hr))
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