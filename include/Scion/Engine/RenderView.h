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

#ifndef __SCION_ENGINE_H_INCLUDED__
#error Do not include RenderView.h directly, include the Engine.h file
#endif

#pragma once

namespace scion
{
	namespace engine
	{

		class AFX_EXT_CLASS CRenderView : public CView
		{
#pragma region Constructors

			DECLARE_DYNCREATE(CRenderView)

		protected:

			CRenderView() noexcept;
			virtual ~CRenderView();

#pragma endregion
#pragma region Attributes

		private:

			static constexpr const UINT_PTR TIMER_REFRESH_ID = 666;
			static constexpr const UINT		TIMER_REFRESH_FRAME_RATE = 25;
			static constexpr const UINT		TIMER_REFRESH_ELAPSE = 1000 / TIMER_REFRESH_FRAME_RATE;

		public:

			gfx::IRenderWindow* m_pRenderWindow;
			//CMainDocument* GetDocument() const;

#pragma endregion
#pragma region Overridables

		public:

			BOOL PreCreateWindow(CREATESTRUCT& cs) override;
			void OnInitialUpdate() override;
			void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/) override;
			void OnDraw(CDC* pDC) override;

#ifdef _DEBUG
			void AssertValid() const override;
			void Dump(CDumpContext& dc) const override;
#endif

#pragma endregion
#pragma region Messages

		protected:

			DECLARE_MESSAGE_MAP()

			afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
			afx_msg void OnTimer(UINT_PTR nIDEvent);
			afx_msg LRESULT OnAfxDraw2d(WPARAM wParam, LPARAM lParam);
			afx_msg LRESULT OnAfxRecreated2dresources(WPARAM wParam, LPARAM lParam);

#pragma endregion
		};

//#ifndef _DEBUG 
//		inline CMainDocument* CRenderView::GetDocument() const
//		{
//			return STATIC_DOWNCAST(CMainDocument, m_pDocument);
//		}
//#endif

	}
}