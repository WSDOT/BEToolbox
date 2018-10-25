// QConLESpan.h : Declaration of the CQConLESpan

#ifndef __QCONLESPAN_H_
#define __QCONLESPAN_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CQConLESpan
class ATL_NO_VTABLE CQConLESpan : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CQConLESpan, &CLSID_QConLESpan>,
   public IPersistImpl<CQConLESpan>,
	public IDispatchImpl<IStructuredStorage2, &IID_IStructuredStorage2, &LIBID_QCONLELib>,
	public ISupportErrorInfo,
	public IDispatchImpl<IQConLESpan, &IID_IQConLESpan, &LIBID_QCONLELib>,
   public IObjectSafetyImpl<CQConLESpan, INTERFACESAFE_FOR_UNTRUSTED_CALLER | INTERFACESAFE_FOR_UNTRUSTED_DATA>
{
public:
   CQConLESpan():
      m_Length(0.0),
      m_StageIEI(0.0),
      m_StageIIEI(0.0),
      m_gMp(0.0),
      m_gMn(0.0),
      m_gMnSupport(0.0),
      m_gV(0.0),
      m_StageIDC(0.0),
      m_StageIIDC(0.0),
      m_StageIDW(0.0),
      m_StageIIDW(0.0)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_QCONLESPAN)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CQConLESpan)
	COM_INTERFACE_ENTRY(IQConLESpan)
	COM_INTERFACE_ENTRY2(IDispatch, IQConLESpan)
	COM_INTERFACE_ENTRY(IStructuredStorage2)
	COM_INTERFACE_ENTRY(IPersist)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
   COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IQConLESpan
public:
	STDMETHOD(get_gMnSupport)(/*[out, retval]*/ double  *pVal);
	STDMETHOD(put_gMnSupport)(/*[in]*/ double  newVal);
	STDMETHOD(get_StageIIDW)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_StageIIDW)(/*[in]*/ double newVal);
	STDMETHOD(get_StageIDW)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_StageIDW)(/*[in]*/ double newVal);
	STDMETHOD(get_StageIIDC)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_StageIIDC)(/*[in]*/ double newVal);
	STDMETHOD(get_StageIDC)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_StageIDC)(/*[in]*/ double newVal);
	STDMETHOD(get_gV)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_gV)(/*[in]*/ double newVal);
	STDMETHOD(get_gMn)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_gMn)(/*[in]*/ double newVal);
	STDMETHOD(get_gMp)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_gMp)(/*[in]*/ double newVal);
	STDMETHOD(get_StageIIEI)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_StageIIEI)(/*[in]*/ double newVal);
	STDMETHOD(get_StageIEI)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_StageIEI)(/*[in]*/ double newVal);
	STDMETHOD(get_Length)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_Length)(/*[in]*/ double newVal);

   
// IStructuredStorage2
	STDMETHOD(Load)(IStructuredLoad2 * Load);
	STDMETHOD(Save)(IStructuredSave2 * Save);

private:
   double m_Length;
   double m_StageIEI;
   double m_StageIIEI;
   double m_gMp;
   double m_gMn;
   double m_gMnSupport;
   double m_gV;
   double m_StageIDC;
   double m_StageIIDC;
   double m_StageIDW;
   double m_StageIIDW;
};

#endif //__QCONLESPAN_H_
