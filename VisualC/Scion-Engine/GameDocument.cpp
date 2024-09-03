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

		IMPLEMENT_DYNCREATE(CGameDocument, CDocument)

		CGameDocument::CGameDocument()
		{

		}

		CGameDocument::~CGameDocument()
		{

		}

#pragma endregion
#pragma region Operations

		HRESULT CGameDocument::AddResource()
		{
			CWnd* pWnd = AfxGetMainWnd();
			ASSERT_POINTER(pWnd, CWnd);
			ASSERT_VALID(pWnd);
			
			//SendMessageToDescendantsEx(WM_UPDATE_PANE, EPane_ResourceList, this);

			return E_NOTIMPL;
		}

		void CGameDocument::EnumerateResources(CResourceManager::ENUMRESOURCEPROC pfnEnumResource, LPVOID pData)
		{
			m_ResourceManager.EnumerateResources(pfnEnumResource, pData);
		}

		void CGameDocument::OnUpdate()
		{
			m_ResourceManager.OnUpdate();
		}

#pragma endregion
#pragma region Overridables

#ifndef _WIN32_WCE

		void CGameDocument::Serialize(CArchive& ar)
		{
			if (ar.IsStoring())
			{
				// TODO: ajoutez ici le code de stockage
			}
			else
			{
				// TODO: ajoutez ici le code de chargement
			}
			
			m_ResourceManager.Serialize(ar);
		}
#endif

#ifdef _DEBUG
		void CGameDocument::AssertValid() const
		{
			CDocument::AssertValid();

			ASSERT_VALID(&m_ResourceManager);
		}

#ifndef _WIN32_WCE
		void CGameDocument::Dump(CDumpContext& dc) const
		{
			CDocument::Dump(dc);

			AFXDUMP(&m_ResourceManager);
		}
#endif
#endif 

#pragma endregion
#pragma region Messages

		BEGIN_MESSAGE_MAP(CGameDocument, CDocument)
		END_MESSAGE_MAP()

#pragma endregion
	}
}