#pragma once

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

#define ACQUIRE_ACTIVE_DOCUMENT(x) STATIC_DOWNCAST(x, AcquireActiveDocument(RUNTIME_CLASS(x)))