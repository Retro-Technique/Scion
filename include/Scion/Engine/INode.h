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

#pragma once

#include <RetroCode/Geometry.h>
#include <RetroCode/Image.h>

namespace scion
{
	namespace engine // Moteur interne
	{
		class CGameEngine;
		class CResource;
		class CNode;
		class CTransformableNode;
		class CDrawableNode;

		namespace gfx // Moteur graphique interne
		{
			class CRenderer;
			
			namespace priv
			{
				// direct2d
				// directwrite

				class CMatrixStack;
				class CTextureResource;
				class CBrushResource;
				class CSolidColorBrushResource;
				class CTextFormatResource;
			}
		}

		namespace sfx // Moteur audio interne
		{
			class CAudioManager;

			namespace priv
			{
				// directsound

				class CSoundResource;
			}
		}

		namespace physics
		{
			class CPhysicsManager;


		}

		namespace priv
		{
			class CSceneManager;
			class CResourceManager;
		}
	}

	namespace game  // API pour les projets de jeu
	{
		class CGameDocument;
		class CSceneView;
		class CScriptable;
		class CScriptableFactory;
	}

	namespace studio // API spécifique pour Scion-Studio
	{
		// utilise scion::game
		// utilise scion::engine
	}
}

namespace novastorm
{
	// utilise scion::game
}

namespace scion
{
	namespace api
	{

		enum EComponent : INT
		{
			EComponent_Transformable,
			EComponent_Drawable,
			EComponent_Line,

			EComponent_COUNT
		};

		class IDrawableComponent
		{
		public:

			virtual void SetVisible(BOOL bVisible) = 0;
			virtual void SetColor(const retro::image::CColorRGBA& clrComp) = 0;
			virtual const retro::image::CColorRGBA& GetColor() const = 0;

		};

		class ILineComponent
		{
		public:

			virtual void SetStart(FLOAT fStartX, FLOAT fStartY) = 0;
			virtual void SetStart(const retro::geometry::CVector2f& vStart) = 0;
			virtual const retro::geometry::CVector2f& GetStart() const = 0;
			virtual void SetEnd(FLOAT fEndX, FLOAT fEndY) = 0;
			virtual void SetEnd(const retro::geometry::CVector2f& vEnd) = 0;
			virtual const retro::geometry::CVector2f& GetEnd() const = 0;

		};

		class INode
		{
		public:

			virtual void SetActive(BOOL bActive) = 0;
			virtual LPCTSTR GetName() const = 0;
			virtual BOOL IsActive() const = 0;
			virtual void AddChild(INode* pChild) = 0;
			virtual void RemoveChild(INode* pChild) = 0;
			virtual void RemoveChildren() = 0;
			virtual void Detach() = 0;
			virtual const INode* GetParent() const = 0;
			virtual INode* GetParent() = 0;
			virtual const INode* GetNodeFromName(LPCTSTR pszName) const = 0;
			virtual INode* GetNodeFromName(LPCTSTR pszName) = 0;
			virtual BOOL IsWorldActive() const = 0;
			virtual BOOL IsWorldVisible() const = 0;
			virtual BOOL IsVisible() const = 0;
			virtual HRESULT QueryInterface(EComponent eComponent, LPCVOID* ppComponent) = 0;

		};

	}
}