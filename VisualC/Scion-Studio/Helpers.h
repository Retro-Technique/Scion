#pragma once

static inline CString Translate(UINT uID)
{
	CString strTranslate;

	BOOL bRet = strTranslate.LoadString(uID);
	ASSERT(bRet);

	return strTranslate;
}

static inline CView* AfxGetActiveView(CRuntimeClass* pClass)
{
	ASSERT(AfxIsValidAddress(pClass, sizeof(CRuntimeClass), FALSE));
	ASSERT(pClass->IsDerivedFrom(RUNTIME_CLASS(CView)));

	CWnd* pMainWnd = AfxGetMainWnd();
	if (!pMainWnd)
	{
		return NULL;
	}

	if (pMainWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))
	{
		CFrameWnd* pFrame = STATIC_DOWNCAST(CMDIFrameWnd, pMainWnd)->GetActiveFrame();
		if (!pFrame)
		{
			return NULL;
		}

		CView* pView = pFrame->GetActiveView();
		if (pView && pView->IsKindOf(pClass))
		{
			return pView;
		}
	}

	if (pMainWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
	{
		return STATIC_DOWNCAST(CFrameWnd, pMainWnd)->GetActiveView();
	}

	return NULL;
}

static inline CDocument* AfxGetActiveDocument(CRuntimeClass* pClass)
{
	ASSERT(AfxIsValidAddress(pClass, sizeof(CRuntimeClass), FALSE));
	ASSERT(pClass->IsDerivedFrom(RUNTIME_CLASS(CDocument)));

	CView* pView = AfxGetActiveView(RUNTIME_CLASS(CView));
	
	return pView ? pView->GetDocument() : NULL;
}

#define I18N(id) Translate(id)
#define AFX_GET_ACTIVE_DOCUMENT(x) STATIC_DOWNCAST(x, AfxGetActiveDocument(RUNTIME_CLASS(x)))
#define AFX_GET_ACTIVE_VIEW(x) STATIC_DOWNCAST(x, AfxGetActiveView(RUNTIME_CLASS(x)))
#define UPDATE_DATA_BACK_TO_FRONT FALSE
#define UPDATE_DATA_FRONT_TO_BACK TRUE