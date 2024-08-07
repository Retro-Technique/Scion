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
#error Do not include IRenderer.h directly, include the Graphics.h file
#endif

#pragma once

namespace scion
{
	namespace gfx
	{
		
		class IRenderer
		{
		public:
			
			virtual void SetTransform(const D2D1::Matrix3x2F& mMatrix) = 0;
			virtual void Clear(const D2D1_COLOR_F& clrClear) = 0;
			virtual void DrawBitmap(IBitmapResource* pBitmap, const D2D_RECT_F& rcDst, EBitmapInterpolationMode eInterpolationMode, const D2D_RECT_F& rcSrc) = 0;
			virtual void DrawGeometry(IGeometryResource* pGeometry, IBrushResource* pBrush, FLOAT fStrokeWidth, IStrokeStyleResource* pStrokeStyle) = 0;
			virtual void FillGeometry(IGeometryResource* pGeometry, IBrushResource* pBrush, IBrushResource* pOpacityBrush) = 0;
			virtual void DrawText() = 0;
			virtual void DrawSpriteBatch(ISpriteBatchResource* pSpriteBatch, UINT uStartIndex, UINT uSpriteCount, IBitmapResource* pBitmap, EBitmapInterpolationMode eInterpolationMode, EBitmapSpriteOptions eSpriteOptions) = 0;

		public:

			static AFX_EXT_API IRenderer* Create(CHwndRenderTarget* pRenderTarget);
			static AFX_EXT_API void Free(IRenderer* pRenderer);

		};

		enum EBitmapInterpolationMode { };
		enum EBitmapSpriteOptions { };


		class IResource { };
		class IBitmapResource : public IResource { };
		class IGeometryResource : public IResource { };
		class ITextFormatResource : public IResource { };
		class ISpriteBatchResource : public IResource { };
		class IStrokeStyleResource :public IResource { };
		class IBrushResource : public IResource { };
		class ISolidColorResource : public IBrushResource { };
		class ILinearGradientResource : public IBrushResource { };

	}
}