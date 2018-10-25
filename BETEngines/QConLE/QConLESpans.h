// QConLESpans.h : Declaration of the CQConLESpans

#ifndef __QCONLESPANS_H_
#define __QCONLESPANS_H_

#include "resource.h"       // main symbols
#include "QConLESpan.h"

// WbflComCollections.h has been updated so that they support non-IDispatch collections
// For this app we need IDispatch based collections
// To remedy this, I placed a renamed copy of the old WBFLComCollection.h in this project
#include "ComCollections.h"

class CQConLESpans;

typedef CComVectorCollection<IQConLESpans, IQConLESpan, IEnumQConLESpan, &IID_IEnumQConLESpan,&LIBID_QCONLELib> QConLESpanVectorImpl;
typedef CPersistentCollection<CQConLESpans, QConLESpanVectorImpl, 1.0,&LIBID_QCONLELib> PersistentSPCollImpl;
/////////////////////////////////////////////////////////////////////////////
// CQConLESpans
class ATL_NO_VTABLE CQConLESpans : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CQConLESpans, &CLSID_QConLESpans>,
	public ISupportErrorInfo,
   public PersistentSPCollImpl
{
public:
	CQConLESpans()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_QCONLESPANS)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CQConLESpans)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
   COM_INTERFACE_ENTRY_CHAIN(PersistentSPCollImpl)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IQConLESpans
public:
private:
// implementation of needed virtual functions
   virtual HRESULT OnBeforeAdd ( QConLESpanVectorImpl::StoredType* pVal)
   {
      return S_OK;
   }

   virtual HRESULT OnBeforeRemove ( QConLESpanVectorImpl::StoredType* pVal, long id)
   {
      return S_OK;
   }

   virtual HRESULT OnAfterRemove ( long id)
   {
      return S_OK;
   }

   virtual HRESULT OnAfterAdd ( QConLESpanVectorImpl::StoredType* pVal, long id)
   {
      return S_OK;
   }

};

#endif //__QCONLESPANS_H_
