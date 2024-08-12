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

		IMPLEMENT_SERIAL(CNode, CObject, 1);

		CNode::CNode()
			: m_pParent(NULL)
			, m_bActive(TRUE)
		{

		}

		CNode::~CNode()
		{
			RemoveChildren();
		}

#pragma endregion
#pragma region Operations

		void CNode::AddChild(CNode* pChild)
		{
			ASSERT_POINTER(pChild, CNode);
			ASSERT_VALID(pChild);

			if (pChild && (pChild != this))
			{
				//pChild->AddRef();
				pChild->Detach();

				m_listChildren.AddTail(pChild);

				pChild->m_pParent = this;
			}
		}

		void CNode::RemoveChild(CNode* pChild)
		{
			ASSERT_POINTER(pChild, CNode);
			ASSERT_VALID(pChild);

			if (pChild && pChild->GetParent() == this)
			{
				POSITION pos = m_listChildren.Find(pChild);
				if (pos)
				{
					m_listChildren.RemoveAt(pos);
					pChild->m_pParent = NULL;
					//pChild->Release();
				}
			}
		}

		void CNode::RemoveChildren()
		{
			POSITION pos = m_listChildren.GetHeadPosition();
			while (pos)
			{
				CObject* pObject = m_listChildren.GetNext(pos);
				ASSERT(pObject);
				ASSERT_KINDOF(CNode, pObject);
				CNode* pNode = STATIC_DOWNCAST(CNode, pObject);
				ASSERT_VALID(pNode);

				pNode->m_pParent = NULL;
				//pNode->Release();
			}

			m_listChildren.RemoveAll();
		}

		void CNode::Detach()
		{
			if (m_pParent)
			{
				m_pParent->RemoveChild(this);
			}
		}

		const CNode* CNode::GetParent() const
		{
			return m_pParent;
		}

		CNode* CNode::GetParent()
		{
			return m_pParent;
		}

		const CNode* CNode::GetNodeFromName(LPCTSTR pszName) const
		{
			if (!AfxIsValidString(pszName))
			{
				return NULL;
			}

			if (StrCmp(m_strName.GetString(), pszName) == 0)
			{
				return this;
			}

			POSITION pos = m_listChildren.GetHeadPosition();
			while (pos)
			{
				const CObject* pObject = m_listChildren.GetNext(pos);
				ASSERT(pObject);
				ASSERT_KINDOF(CNode, pObject);
				const CNode* pEntry = STATIC_DOWNCAST_CONST(CNode, pObject);
				ASSERT_VALID(pEntry);

				const CNode* pNode = pEntry->GetNodeFromName(pszName);
				if (pNode)
				{
					return pNode;
				}
			}

			return NULL;
		}

		CNode* CNode::GetNodeFromName(LPCTSTR pszName)
		{
			if (!AfxIsValidString(pszName))
			{
				return NULL;
			}

			if (StrCmp(m_strName.GetString(), pszName) == 0)
			{
				return this;
			}

			POSITION pos = m_listChildren.GetHeadPosition();
			while (pos)
			{
				CObject* pObject = m_listChildren.GetNext(pos);
				ASSERT(pObject);
				ASSERT_KINDOF(CNode, pObject);
				CNode* pEntry = STATIC_DOWNCAST(CNode, pObject);
				ASSERT_VALID(pEntry);

				CNode* pNode = pEntry->GetNodeFromName(pszName);
				if (pNode)
				{
					 return pNode;
				}
			}

			return NULL;
		}

		BOOL CNode::IsWorldActive() const
		{
			if (!IsActive())
			{
				return FALSE;
			}

			if (!m_pParent)
			{
				return TRUE;
			}

			return m_pParent->IsWorldActive();
		}

		BOOL CNode::IsWorldVisible() const
		{
			if (!IsVisible())
			{
				return FALSE;
			}

			if (!m_pParent)
			{
				return TRUE;
			}

			return m_pParent->IsWorldVisible();
		}

#pragma endregion
#pragma region Overridables

		BOOL CNode::IsVisible() const
		{
			return FALSE;
		}

		void CNode::Serialize(CArchive& ar)
		{
			CObject::Serialize(ar);

			if (ar.IsStoring())
			{
				ar << m_strName << m_bActive;
			}
			else
			{
				ar >> m_strName >> m_bActive;
			}

			m_listChildren.Serialize(ar);
		}

#ifdef _DEBUG

		void CNode::AssertValid() const
		{
			CObject::AssertValid();

			ASSERT_VALID(&m_listChildren);
		}

		void CNode::Dump(CDumpContext& dc) const
		{
			CObject::Dump(dc);

			dc << _T("Name: ") << m_strName << _T("\n");
			dc << _T("Active: ") << m_bActive << _T("\n");
			AFXDUMP(m_listChildren);
		}

#endif

#pragma endregion
	}
}