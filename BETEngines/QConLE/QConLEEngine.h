// QConLEEngine.h : Declaration of the CQConLEEngine

#ifndef __QCONLEENGINE_H_
#define __QCONLEENGINE_H_

#include "resource.h"       // main symbols

#include <sstream>

#include <LBAM.h>
#include <LBAMAnalysis.h>
#include <LBAMLiveLoader.h>
#include <LBAMLoadCombiner.h>
#include <LBAMUtility.h>
#include <LBAMAnalysisUtility.h>
#include <WBFLUnitServer.h>
#include <WBFLTools.h>

class ModelReporter;
/////////////////////////////////////////////////////////////////////////////
// CQConLEEngine
class ATL_NO_VTABLE CQConLEEngine : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CQConLEEngine, &CLSID_QConLEEngine>,
	public ISupportErrorInfo,
	public IDispatchImpl<IQConLEEngine, &IID_IQConLEEngine, &LIBID_QCONLELib>,
   public IObjectSafetyImpl<CQConLEEngine, INTERFACESAFE_FOR_UNTRUSTED_CALLER | INTERFACESAFE_FOR_UNTRUSTED_DATA>
{
      friend ModelReporter;

public:
   CQConLEEngine()
	{
	}

   STDMETHOD(FinalConstruct)();


DECLARE_REGISTRY_RESOURCEID(IDR_QCONLEENGINE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CQConLEEngine)
	COM_INTERFACE_ENTRY(IQConLEEngine)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
   COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IQConLEEngine
public:
	STDMETHOD(get_DoDetailedReport)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_DoDetailedReport)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(ExportLBAM)();
	STDMETHOD(Process)(/*[out,retval]*/BSTR* Report);
	STDMETHOD(SaveFile)();
	STDMETHOD(LoadFile)(VARIANT_BOOL* pCancelled);
	STDMETHOD(get_Spans)(/*[out, retval]*/ IQConLESpans* *pVal);
	STDMETHOD(putref_Spans)(/*[in]*/ IQConLESpans* newVal);
	STDMETHOD(get_DoFatigue)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_DoFatigue)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_DoStrengthI)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_DoStrengthI)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_DoServiceIII)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_DoServiceIII)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_DoServiceII)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_DoServiceII)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_DoServiceI)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_DoServiceI)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_IncludeLowBoy)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_IncludeLowBoy)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_EnvelopeSimpleContinuous)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_EnvelopeSimpleContinuous)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_UnitType)(/*[out, retval]*/ QCLEUnitType *pVal);
	STDMETHOD(put_UnitType)(/*[in]*/ QCLEUnitType newVal);
	STDMETHOD(get_ModelType)(/*[out, retval]*/ QCLEModelType *pVal);
	STDMETHOD(put_ModelType)(/*[in]*/ QCLEModelType newVal);
   STDMETHOD(Clear)();

// IStructuredStorage2
	STDMETHOD(Load)(IStructuredLoad2 * Load);
	STDMETHOD(Save)(IStructuredSave2 * Save);


private:
   void Init();

   ILBAMModel* BuildModel(QCLEModelType type);
   void ConfigureLiveLoads(ILBAMModel* psm);
   void ConfigureDistributionFactors(ILBAMAnalysisEngine* engine, ILBAMModel* model);
   void ConfigureSsDistributionFactors(long numSpans, ILBAMModel* model);
   void ConfigureCsDistributionFactors(long numSpans, ILBAMAnalysisEngine* engine, ILBAMModel* model);

   QCLEModelType m_ModelType;
   QCLEUnitType  m_UnitType;
   VARIANT_BOOL  m_EnvelopeSimpleContinuous;
   VARIANT_BOOL  m_IncludeLowBoy;
   VARIANT_BOOL  m_DoServiceI;
   VARIANT_BOOL  m_DoServiceII;
   VARIANT_BOOL  m_DoServiceIII;
   VARIANT_BOOL  m_DoStrengthI;
   VARIANT_BOOL  m_DoFatigue;
   VARIANT_BOOL  m_DoDetailedReport;  // not persisted
   CComPtr<IQConLESpans> m_Spans;

   // local 
   long m_NumStages;

};

#endif //__QCONLEENGINE_H_
