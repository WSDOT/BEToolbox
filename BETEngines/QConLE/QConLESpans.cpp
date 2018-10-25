// QConLESpans.cpp : Implementation of CQConLESpans
#include "stdafx.h"
#include "QConLE.h"
#include "QConLESpans.h"

/////////////////////////////////////////////////////////////////////////////
// CQConLESpans

STDMETHODIMP CQConLESpans::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IQConLESpans
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}
