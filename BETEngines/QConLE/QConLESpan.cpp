// QConLESpan.cpp : Implementation of CQConLESpan
#include "stdafx.h"
#include "QConLE.h"
#include "QConLESpan.h"

/////////////////////////////////////////////////////////////////////////////
// CQConLESpan

STDMETHODIMP CQConLESpan::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IQConLESpan
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CQConLESpan::get_Length(double *pVal)
{
	*pVal = m_Length;

	return S_OK;
}

STDMETHODIMP CQConLESpan::put_Length(double newVal)
{
	if (newVal>=0.0)
      m_Length = newVal;
   else
      return E_INVALIDARG;

	return S_OK;
}

STDMETHODIMP CQConLESpan::get_StageIEI(double *pVal)
{
	*pVal = m_StageIEI;

	return S_OK;
}

STDMETHODIMP CQConLESpan::put_StageIEI(double newVal)
{
	if (newVal>=0.0)
      m_StageIEI = newVal;
   else
      return E_INVALIDARG;

	return S_OK;
}

STDMETHODIMP CQConLESpan::get_StageIIEI(double *pVal)
{
	*pVal = m_StageIIEI;

	return S_OK;
}

STDMETHODIMP CQConLESpan::put_StageIIEI(double newVal)
{
	if (newVal>=0.0)
      m_StageIIEI = newVal;
   else
      return E_INVALIDARG;

	return S_OK;
}

STDMETHODIMP CQConLESpan::get_gMp(double *pVal)
{
	*pVal = m_gMp;

	return S_OK;
}

STDMETHODIMP CQConLESpan::put_gMp(double newVal)
{
	if (newVal>=0.0)
      m_gMp = newVal;
   else
      return E_INVALIDARG;

	return S_OK;
}

STDMETHODIMP CQConLESpan::get_gMn(double *pVal)
{
	*pVal = m_gMn;

	return S_OK;
}

STDMETHODIMP CQConLESpan::put_gMn(double newVal)
{
	if (newVal>=0.0)
      m_gMn = newVal;
   else
      return E_INVALIDARG;

	return S_OK;
}

STDMETHODIMP CQConLESpan::get_gMnSupport(double *pVal)
{
	*pVal = m_gMnSupport;

	return S_OK;
}

STDMETHODIMP CQConLESpan::put_gMnSupport(double newVal)
{
	if (newVal>=0.0)
      m_gMnSupport = newVal;
   else
      return E_INVALIDARG;

	return S_OK;
}


STDMETHODIMP CQConLESpan::get_gV(double *pVal)
{
	*pVal = m_gV;

	return S_OK;
}

STDMETHODIMP CQConLESpan::put_gV(double newVal)
{
	if (newVal>=0.0)
      m_gV = newVal;
   else
      return E_INVALIDARG;

	return S_OK;
}

STDMETHODIMP CQConLESpan::get_StageIDC(double *pVal)
{
	*pVal = m_StageIDC;

	return S_OK;
}

STDMETHODIMP CQConLESpan::put_StageIDC(double newVal)
{
	if (newVal>=0.0)
      m_StageIDC = newVal;
   else
      return E_INVALIDARG;

	return S_OK;
}

STDMETHODIMP CQConLESpan::get_StageIIDC(double *pVal)
{
	*pVal = m_StageIIDC;

	return S_OK;
}

STDMETHODIMP CQConLESpan::put_StageIIDC(double newVal)
{
	if (newVal>=0.0)
      m_StageIIDC = newVal;
   else
      return E_INVALIDARG;

	return S_OK;
}

STDMETHODIMP CQConLESpan::get_StageIDW(double *pVal)
{
	*pVal = m_StageIDW;

	return S_OK;
}

STDMETHODIMP CQConLESpan::put_StageIDW(double newVal)
{
	if (newVal>=0.0)
      m_StageIDW = newVal;
   else
      return E_INVALIDARG;

	return S_OK;
}

STDMETHODIMP CQConLESpan::get_StageIIDW(double *pVal)
{
	*pVal = m_StageIIDW;

	return S_OK;
}

STDMETHODIMP CQConLESpan::put_StageIIDW(double newVal)
{
	if (newVal>=0.0)
      m_StageIIDW = newVal;
   else
      return E_INVALIDARG;

	return S_OK;
}


static const double MY_VER=1.0;

STDMETHODIMP CQConLESpan::Load(IStructuredLoad2 * pload)
{
   HRESULT hr;
   hr = pload->BeginUnit(CComBSTR("CQConLESpan"));
   if (FAILED(hr))
      return hr;

   double ver;
   hr = pload->get_Version(&ver);
   if (FAILED(hr))
      return hr;

   if (ver!=MY_VER)
      return STRLOAD_E_BADVERSION;

   {        
      _variant_t var;
      hr = pload->get_Property(_bstr_t("m_Length"),&var);
      if (FAILED(hr))
         return hr;

      m_Length = var;

      var.Clear();
      hr = pload->get_Property(_bstr_t("m_StageIEI"),&var);
      if (FAILED(hr))
         return hr;

      m_StageIEI = var;

      var.Clear();
      hr = pload->get_Property(_bstr_t("m_StageIIEI"),&var);
      if (FAILED(hr))
         return hr;

      m_StageIIEI = var;

      var.Clear();
      hr = pload->get_Property(_bstr_t("m_gMp"),&var);
      if (FAILED(hr))
         return hr;

      m_gMp = var;

      var.Clear();
      hr = pload->get_Property(_bstr_t("m_gMn"),&var);
      if (FAILED(hr))
         return hr;

      m_gMn = var;

      var.Clear();
      hr = pload->get_Property(_bstr_t("m_gMnSupport"),&var);
      if (FAILED(hr))
         return hr;

      m_gMnSupport = var;


      var.Clear();
      hr = pload->get_Property(_bstr_t("m_gV"),&var);
      if (FAILED(hr))
         return hr;

      m_gV = var;

      var.Clear();
      hr = pload->get_Property(_bstr_t("m_StageIDC"),&var);
      if (FAILED(hr))
         return hr;

      m_StageIDC = var;

      var.Clear();
      hr = pload->get_Property(_bstr_t("m_StageIIDC"),&var);
      if (FAILED(hr))
         return hr;

      m_StageIIDC = var;

      var.Clear();
      hr = pload->get_Property(_bstr_t("m_StageIDW"),&var);
      if (FAILED(hr))
         return hr;

      m_StageIDW = var;

      var.Clear();
      hr = pload->get_Property(_bstr_t("m_StageIIDW"),&var);
      if (FAILED(hr))
         return hr;

      m_StageIIDW = var;
   }

   VARIANT_BOOL eb;
   hr = pload->EndUnit(&eb);
   if (FAILED(hr))
      return hr;

   if (eb!=VARIANT_TRUE)
      return STRLOAD_E_INVALIDFORMAT;

   return S_OK;
}

STDMETHODIMP CQConLESpan::Save(IStructuredSave2 * psave)
{
   HRESULT hr;

   hr = psave->BeginUnit(CComBSTR("CQConLESpan"), MY_VER);
   if (FAILED(hr))
      return hr;

   hr = psave->put_Property(CComBSTR("m_Length"),_variant_t(m_Length));
   if (FAILED(hr))
      return hr;

   hr = psave->put_Property(CComBSTR("m_StageIEI"),_variant_t(m_StageIEI));
   if (FAILED(hr))
      return hr;

   hr = psave->put_Property(CComBSTR("m_StageIIEI"),_variant_t(m_StageIIEI));
   if (FAILED(hr))
      return hr;

   hr = psave->put_Property(CComBSTR("m_gMp"),_variant_t(m_gMp));
   if (FAILED(hr))
      return hr;

   hr = psave->put_Property(CComBSTR("m_gMn"),_variant_t(m_gMn));
   if (FAILED(hr))
      return hr;

   hr = psave->put_Property(CComBSTR("m_gMnSupport"),_variant_t(m_gMnSupport));
   if (FAILED(hr))
      return hr;

   hr = psave->put_Property(CComBSTR("m_gV"),_variant_t(m_gV));
   if (FAILED(hr))
      return hr;

   hr = psave->put_Property(CComBSTR("m_StageIDC"),_variant_t(m_StageIDC));
   if (FAILED(hr))
      return hr;

   hr = psave->put_Property(CComBSTR("m_StageIIDC"),_variant_t(m_StageIIDC));
   if (FAILED(hr))
      return hr;

   hr = psave->put_Property(CComBSTR("m_StageIDW"),_variant_t(m_StageIDW));
   if (FAILED(hr))
      return hr;

   hr = psave->put_Property(CComBSTR("m_StageIIDW"),_variant_t(m_StageIIDW));
   if (FAILED(hr))
      return hr;

   hr = psave->EndUnit();
   return hr;
} 

