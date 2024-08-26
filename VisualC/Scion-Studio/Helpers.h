#pragma once

static inline CString Translate(UINT uID)
{
	CString strTranslate;

	BOOL bRet = strTranslate.LoadString(uID);
	ASSERT(bRet);

	return strTranslate;
}

static inline CDocument* AcquireActiveDocument(CRuntimeClass* pClass)
{
	ASSERT(AfxIsValidAddress(pClass, sizeof(CRuntimeClass), FALSE));
	ASSERT(pClass->IsDerivedFrom(RUNTIME_CLASS(CDocument)));

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

		CDocument* pDocument = pFrame->GetActiveDocument();
		if (pDocument && pDocument->IsKindOf(pClass))
		{
			return pDocument;
		}
	}

	if (pMainWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
	{
		return STATIC_DOWNCAST(CFrameWnd, pMainWnd)->GetActiveDocument();
	}

	return NULL;
}

#define I18N(id) Translate(id)
#define ACQUIRE_ACTIVE_DOCUMENT(x) STATIC_DOWNCAST(x, AcquireActiveDocument(RUNTIME_CLASS(x)))
#define UPDATE_DATA_BACK_TO_FRONT FALSE
#define UPDATE_DATA_FRONT_TO_BACK TRUE