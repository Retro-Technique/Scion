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
#include "GraphicsManager.h"

namespace scion
{
	namespace engine
	{
		namespace gfx
		{

			class CRenderWindow : public CObject, public IRenderWindow
			{
#pragma region Constructors

				DECLARE_DYNAMIC(CRenderWindow)

			public:

				CRenderWindow(CGraphicsManager* pGraphicsManager);
				virtual ~CRenderWindow();

#pragma endregion
#pragma region Attributes

			private:

				CGraphicsManager*		m_pGraphicsManager;
				ID2D1DeviceContext7*	m_pD2DDeviceContext;

			public:

				inline ID2D1DeviceContext7* GetD2DDeviceContext() const { return m_pD2DDeviceContext; }

#pragma endregion
#pragma region Overridables

			public:

				HRESULT Create(CWnd* pWnd) override;
				void Destroy() override;
				void SelfDestroy() override;
#ifdef _DEBUG
				void AssertValid() const override;
				void Dump(CDumpContext& dc) const override;
#endif

#pragma endregion
			};

		}
	}
}