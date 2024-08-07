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

#ifndef __SCION_GRAPHICS_H_INCLUDED__
#error Do not include SolidColorBrushResource.h directly, include the Graphics.h file
#endif

#pragma once

#include "BrushResource.h"

namespace scion
{
	namespace gfx
	{

		class AFX_EXT_CLASS CSolidColorBrushResource : public CBrushResource
		{
#pragma region Constructors

		public:

			enum EProperty : INT
			{
				EProperty_Color = CBrushResource::EProperty_COUNT,

				EProperty_COUNT
			};

		public:

			DECLARE_SERIAL(CSolidColorBrushResource);

		public:

			CSolidColorBrushResource();
			virtual ~CSolidColorBrushResource();

#pragma endregion
#pragma region Attributes

		private:

			D2D1::ColorF	m_clrBrush;

		public:

			inline void SetColor(const D2D1::ColorF& clrBrush) { m_clrBrush = clrBrush; m_bNeedUpdate = TRUE; }
			inline const D2D1::ColorF& GetColor() const { return m_clrBrush; }

#pragma endregion
#pragma region Overridables

		public:

			void SetProperty(INT nProperty, const COleVariant& varValue) override;
			HRESULT Load(ID2D1HwndRenderTarget* pRenderTarget) override;
			void Serialize(CArchive& ar) override;
#ifdef _DEBUG
			void AssertValid() const override;
			void Dump(CDumpContext& dc) const override;
#endif

#pragma endregion
		};

	}
}