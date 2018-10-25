// QConLEEngine.cpp : Implementation of CQConLEEngine
#include "stdafx.h"
#include "QConLE.h"
#include "QConLEEngine.h"

#include "ModelReporter.h"

// free functions
void SetupSegment(ISegment* pseg, double Length, double EA, double EI);
void AddDistLoad(IDistributedLoads* distLoads, BSTR stage, BSTR loadgroup, long spanNum, double load);
void AddDistributionFactors(IDistributionFactors* distFactors, double length, double gMp, double gMn, double gV);
void ConfigureLrfdLoadCombinations(ILBAMModel* model);
void AddLoadCase(ILoadCases* loadCases, BSTR name, BSTR description);
long GetCfPointsInRange(IDblArray* cfLocs, double spanStart, double spanEnd, double* ptsInrg);

// keep two statics around for expediency
static CHRTextHucker create_hr("An error occured while creating an LBAM object - Perhaps the LBAM components are not installed?");
static CHRTextHucker hr;


/////////////////////////////////////////////////////////////////////////////
// CQConLEEngine

STDMETHODIMP CQConLEEngine::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IQConLEEngine
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CQConLEEngine::FinalConstruct( )
{
   Init();

   HRESULT hr;
   hr = m_Spans.CoCreateInstance(CLSID_QConLESpans);
   if (FAILED(hr))
      return hr;


   return S_OK;
}

void CQConLEEngine::Init()
{
   m_ModelType = qcmtSDCL;
   m_UnitType = qcmtUS;
   m_EnvelopeSimpleContinuous = VARIANT_FALSE;
   m_IncludeLowBoy = VARIANT_FALSE;
   m_DoServiceI   = VARIANT_FALSE;
   m_DoServiceII  = VARIANT_FALSE;
   m_DoServiceIII = VARIANT_FALSE;
   m_DoStrengthI  = VARIANT_FALSE;
   m_DoFatigue = VARIANT_FALSE;

   m_DoDetailedReport = VARIANT_FALSE;
}


STDMETHODIMP CQConLEEngine::get_ModelType(QCLEModelType *pVal)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*pVal = m_ModelType;

	return S_OK;
}

STDMETHODIMP CQConLEEngine::put_ModelType(QCLEModelType newVal)
{
   if (newVal==qcmtS2CC || newVal==qcmtSDCL|| newVal==qcmtS1SS || newVal==qcmtS1CS || newVal==qcmtS2SS)
   	m_ModelType = newVal;
   else
      return E_INVALIDARG;

	return S_OK;
}

STDMETHODIMP CQConLEEngine::get_UnitType(QCLEUnitType *pVal)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*pVal = m_UnitType;

	return S_OK;
}

STDMETHODIMP CQConLEEngine::put_UnitType(QCLEUnitType newVal)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (newVal==qcmtUS || newVal==qcmtSI)
      m_UnitType = newVal;
   else
      return E_INVALIDARG;

	return S_OK;
}

STDMETHODIMP CQConLEEngine::get_EnvelopeSimpleContinuous(VARIANT_BOOL *pVal)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*pVal = m_EnvelopeSimpleContinuous;

	return S_OK;
}

STDMETHODIMP CQConLEEngine::put_EnvelopeSimpleContinuous(VARIANT_BOOL newVal)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (newVal!=VARIANT_FALSE)
      m_EnvelopeSimpleContinuous = VARIANT_TRUE;
   else
      m_EnvelopeSimpleContinuous = VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CQConLEEngine::get_IncludeLowBoy(VARIANT_BOOL *pVal)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*pVal = m_IncludeLowBoy;

	return S_OK;
}

STDMETHODIMP CQConLEEngine::put_IncludeLowBoy(VARIANT_BOOL newVal)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (newVal!=VARIANT_FALSE)
      m_IncludeLowBoy = VARIANT_TRUE;
   else
      m_IncludeLowBoy = VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CQConLEEngine::get_DoServiceI(VARIANT_BOOL *pVal)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*pVal = m_DoServiceI;

	return S_OK;
}

STDMETHODIMP CQConLEEngine::put_DoServiceI(VARIANT_BOOL newVal)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (newVal!=VARIANT_FALSE)
      m_DoServiceI = VARIANT_TRUE;
   else
      m_DoServiceI = VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CQConLEEngine::get_DoServiceII(VARIANT_BOOL *pVal)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*pVal = m_DoServiceII;

	return S_OK;
}

STDMETHODIMP CQConLEEngine::put_DoServiceII(VARIANT_BOOL newVal)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (newVal!=VARIANT_FALSE)
      m_DoServiceII = VARIANT_TRUE;
   else
      m_DoServiceII = VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CQConLEEngine::get_DoServiceIII(VARIANT_BOOL *pVal)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*pVal = m_DoServiceIII;

	return S_OK;
}

STDMETHODIMP CQConLEEngine::put_DoServiceIII(VARIANT_BOOL newVal)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (newVal!=VARIANT_FALSE)
      m_DoServiceIII = VARIANT_TRUE;
   else
      m_DoServiceIII = VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CQConLEEngine::get_DoStrengthI(VARIANT_BOOL *pVal)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*pVal = m_DoStrengthI;

	return S_OK;
}

STDMETHODIMP CQConLEEngine::put_DoStrengthI(VARIANT_BOOL newVal)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (newVal!=VARIANT_FALSE)
      m_DoStrengthI = VARIANT_TRUE;
   else
      m_DoStrengthI = VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CQConLEEngine::get_DoFatigue(VARIANT_BOOL *pVal)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*pVal = m_DoFatigue;

	return S_OK;
}

STDMETHODIMP CQConLEEngine::put_DoFatigue(VARIANT_BOOL newVal)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (newVal!=VARIANT_FALSE)
      m_DoFatigue = VARIANT_TRUE;
   else
      m_DoFatigue = VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CQConLEEngine::get_Spans(IQConLESpans **pVal)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_Spans.CopyTo(pVal);
}

STDMETHODIMP CQConLEEngine::putref_Spans(IQConLESpans *newVal)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_Spans = newVal;

	return S_OK;
}

STDMETHODIMP CQConLEEngine::get_DoDetailedReport(VARIANT_BOOL *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = m_DoDetailedReport;

	return S_OK;
}

STDMETHODIMP CQConLEEngine::put_DoDetailedReport(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

   m_DoDetailedReport = (newVal==VARIANT_FALSE) ? VARIANT_FALSE : VARIANT_TRUE;

	return S_OK;
}


// IStructuredStorage2
static const double MY_VER=1.0;

STDMETHODIMP CQConLEEngine::Load(IStructuredLoad2 * pload)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   HRESULT hr;

   hr = pload->BeginUnit(CComBSTR("CQConLEEngine"));
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
      hr = pload->get_Property(_bstr_t("m_ModelType"),&var);
      if (FAILED(hr))
         return hr;

      long imt = var;
      if (imt==qcmtS2CC)
         m_ModelType = qcmtS2CC;
      else if (imt==qcmtSDCL)
         m_ModelType = qcmtSDCL;
      else if (imt==qcmtS1SS)
         m_ModelType = qcmtS1SS;
      else if (imt==qcmtS1CS)
         m_ModelType = qcmtS1CS;
      else if (imt==qcmtS2SS)
         m_ModelType = qcmtS2SS;
      else
         return E_INVALIDARG;


      var.Clear();
      hr = pload->get_Property(_bstr_t("m_UnitType"),&var);
      if (FAILED(hr))
         return hr;

      long iut = var;
      if (iut==qcmtUS)
         m_UnitType = qcmtUS;
      else if (iut==qcmtSI)
         m_UnitType = qcmtSI;
      else
         return E_INVALIDARG;

      var.Clear();
      hr = pload->get_Property(_bstr_t("m_EnvelopeSimpleContinuous"),&var);
      if (FAILED(hr))
         return hr;

      m_EnvelopeSimpleContinuous = var;

      var.Clear();
      hr = pload->get_Property(_bstr_t("m_IncludeLowBoy"),&var);
      if (FAILED(hr))
         return hr;

      m_IncludeLowBoy = var;

      var.Clear();
      hr = pload->get_Property(_bstr_t("m_DoServiceI"),&var);
      if (FAILED(hr))
         return hr;

      m_DoServiceI = var;

      var.Clear();
      hr = pload->get_Property(_bstr_t("m_DoServiceII"),&var);
      if (FAILED(hr))
         return hr;

      m_DoServiceII = var;

      var.Clear();
      hr = pload->get_Property(_bstr_t("m_DoServiceIII"),&var);
      if (FAILED(hr))
         return hr;

      m_DoServiceIII = var;

      var.Clear();
      hr = pload->get_Property(_bstr_t("m_DoStrengthI"),&var);
      if (FAILED(hr))
         return hr;

      m_DoStrengthI = var;

      var.Clear();
      hr = pload->get_Property(_bstr_t("m_DoFatigue"),&var);
      if (FAILED(hr))
         return hr;

      m_DoFatigue = var;

   CComPtr<IQConLESpans> ;


      var.Clear();
      hr = pload->get_Property(_bstr_t("m_Spans"),&var);
      if (FAILED(hr))
         return hr;

      // get variant into more convenient form
      CComPtr<IQConLESpans> pisps;
      hr = _CopyVariantToInterface<IQConLESpans>::copy(&pisps, &var);
      if (FAILED(hr))
         return STRLOAD_E_INVALIDFORMAT;

      hr = putref_Spans(pisps);
      if (FAILED(hr))
         return hr;
   }

   VARIANT_BOOL eb;
   hr = pload->EndUnit(&eb);
   if (FAILED(hr))
      return hr;

   if (eb!=VARIANT_TRUE)
      return STRLOAD_E_INVALIDFORMAT;

   return S_OK;
}

STDMETHODIMP CQConLEEngine::Save(IStructuredSave2 * psave)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   HRESULT hr;
   try
   {
      hr = psave->BeginUnit(CComBSTR("CQConLEEngine"), MY_VER);
      if (FAILED(hr))
         return hr;

      hr = psave->put_Property(CComBSTR("m_ModelType"),_variant_t((long)m_ModelType));
      if (FAILED(hr))
         return hr;

      hr = psave->put_Property(CComBSTR("m_UnitType"),_variant_t((long)m_UnitType));
      if (FAILED(hr))
         return hr;

      hr = psave->put_Property(CComBSTR("m_EnvelopeSimpleContinuous"),_variant_t(m_EnvelopeSimpleContinuous));
      if (FAILED(hr))
         return hr;

      hr = psave->put_Property(CComBSTR("m_IncludeLowBoy"),_variant_t(m_IncludeLowBoy));
      if (FAILED(hr))
         return hr;

      hr = psave->put_Property(CComBSTR("m_DoServiceI"),_variant_t(m_DoServiceI));
      if (FAILED(hr))
         return hr;

      hr = psave->put_Property(CComBSTR("m_DoServiceII"),_variant_t(m_DoServiceII));
      if (FAILED(hr))
         return hr;

      hr = psave->put_Property(CComBSTR("m_DoServiceIII"),_variant_t(m_DoServiceIII));
      if (FAILED(hr))
         return hr;

      hr = psave->put_Property(CComBSTR("m_DoStrengthI"),_variant_t(m_DoStrengthI));
      if (FAILED(hr))
         return hr;

      hr = psave->put_Property(CComBSTR("m_DoFatigue"),_variant_t(m_DoFatigue));
      if (FAILED(hr))
         return hr;

      hr = psave->put_Property(CComBSTR("m_Spans"),_variant_t(m_Spans));
      if (FAILED(hr))
         return hr;

      hr = psave->EndUnit();
   }
   catch(...)
   {
      ATLASSERT(0);
      return E_FAIL;
   }

   return hr;
}


STDMETHODIMP CQConLEEngine::Clear()
{
   Init();
   m_Spans->Clear();

   return S_OK;
}

STDMETHODIMP CQConLEEngine::LoadFile(VARIANT_BOOL* pCancelled)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   *pCancelled = VARIANT_FALSE;

   // first put up a prompt to open the file
  // Prepare the open file dialog
    static char szFilter[] = _T( "QConBridge LE Files (*.qcl)|*.qcl" );

    CFileDialog dlg(TRUE, "qcl", NULL, OFN_FILEMUSTEXIST, szFilter);

    // open file dialog
    if(dlg.DoModal() != IDOK)
    {
      *pCancelled = VARIANT_TRUE;
       return S_OK;
    }

    CComBSTR strFile(dlg.m_ofn.lpstrFile);

    // create our file loader
    CComPtr<IStructuredLoad2> loader;
    HRESULT hr = loader.CoCreateInstance(CLSID_StructuredLoad2);
    if (FAILED(hr)) 
    {
       ::AfxMessageBox("Failed to Create CLSID_StructuredLoad2 - component is probably not installed");
       return hr;
    }

    hr = loader->Open(strFile);
    if (FAILED(hr)) 
    {
       ::AfxMessageBox("Unable to open file");
       return hr;
    }


    // clean up ourself
    this->Clear();

    hr = this->Load(loader);
    if (FAILED(hr)) 
    {
       ::AfxMessageBox("Failed to load file - possibly invalid file format");
       return hr;
    }

    loader->Close();

	return S_OK;
}

STDMETHODIMP CQConLEEngine::SaveFile()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   // first put up a prompt to open the file
  // Prepare the open file dialog
    static char szFilter[] = _T( "QConBridge LE Files (*.qcl)|*.qcl" );

    CFileDialog dlg(FALSE, "qcl", NULL, NULL, szFilter);

    // open file dialog
    if(dlg.DoModal() != IDOK)
    {
       return S_OK;
    }

    CComBSTR strFile(dlg.m_ofn.lpstrFile);

    // create our file loader
    CComPtr<IStructuredSave2> saver;
    HRESULT hr = saver.CoCreateInstance(CLSID_StructuredSave2);
    if (FAILED(hr)) 
    {
       ::AfxMessageBox("Failed to Create CLSID_StructuredSave2 - component is probably not installed");
       return hr;
    }

    hr = saver->Open(strFile);
    if (FAILED(hr)) 
    {
       ::AfxMessageBox("Unable to open file");
       return hr;
    }

    hr = this->Save(saver);
    if (FAILED(hr)) 
    {
       ::AfxMessageBox("Failed to save file");
       return hr;
    }

    saver->Close();

	return S_OK;
}

STDMETHODIMP CQConLEEngine::Process(BSTR *Report)
{
   USES_CONVERSION;
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

   try
   {
      if (m_ModelType==qcmtSDCL && m_EnvelopeSimpleContinuous != VARIANT_FALSE)
      {
         // we are enveloping two models
         CComPtr<ILBAMModel> ss2s_model, sdcl_model;
         ss2s_model.Attach( BuildModel(qcmtS2SS) );
         sdcl_model.Attach( BuildModel(m_ModelType) );

         CComPtr<IUnkArray> model_array;
         create_hr = model_array.CoCreateInstance(CLSID_UnkArray);
         hr = model_array->Add(ss2s_model);
         hr = model_array->Add(sdcl_model);

         CComQIPtr<ILBAMModelEnveloper> the_enveloper;
         create_hr = the_enveloper.CoCreateInstance(CLSID_LBAMModelEnveloper);
         hr = the_enveloper->Initialize(model_array, atForce, optMaximize);

         // distribute distribution factors across our models using contraflexure locations
         CComPtr<IUnkArray> engine_array;
         hr = the_enveloper->get_Engines(&engine_array);

         CComPtr<IUnknown> ss2s_unkengine, sdcl_unkengine;
         hr = engine_array->get_Item(0, &ss2s_unkengine);
         hr = engine_array->get_Item(1, &sdcl_unkengine);

         CComQIPtr<ILBAMAnalysisEngine> ss2s_engine(ss2s_unkengine);
         ATLASSERT(ss2s_unkengine);
         CComQIPtr<ILBAMAnalysisEngine> sdcl_engine(sdcl_unkengine);
         ATLASSERT(sdcl_unkengine);

         ConfigureDistributionFactors(ss2s_engine, ss2s_model);
         ConfigureDistributionFactors(sdcl_engine, sdcl_model);

         // create our reports
         // first for each type of model
         std::wostringstream os;
         os<<L"MODEL RESPONSES ARE ENVELOPED FOR SIMPLE AND CONTINUOUS BEHAVIOR"<<std::endl;
         os<<L"================================================================"<<std::endl;
         os<<L" "<<std::endl;
         os<<L" "<<std::endl;
         os<<L"INDIVIDUAL MODEL RESULTS - SIMPLY SUPPORTED BEHAVIOR FOR BOTH STAGES"<<std::endl;
         os<<L"===================================================================="<<std::endl;
         os<<L" "<<std::endl;

         // temporarly change our model type
         m_ModelType=qcmtS2SS;

         ModelReporter ss2s_reporter(ss2s_model, ss2s_engine, this, m_DoDetailedReport!=VARIANT_FALSE);
         ss2s_reporter.CreateReport(os);

         os<<L" "<<std::endl;
         os<<L" "<<std::endl;
         os<<L"INDIVIDUAL MODEL RESULTS - SIMPLE FOR DEAD, CONTINUOUS FOR LIVE LOAD BEHAVIOR"<<std::endl;
         os<<L"============================================================================="<<std::endl;

         m_ModelType=qcmtSDCL;

         ModelReporter sdcl_reporter(sdcl_model, sdcl_engine, this, m_DoDetailedReport!=VARIANT_FALSE);
         sdcl_reporter.CreateReport(os);

         os<<L" "<<std::endl;
         os<<L" "<<std::endl;
         os<<L"FINAL ENVELOPED, COMBINED RESULTS"<<std::endl;
         os<<L"================================="<<std::endl;

         CComPtr<ILoadCombinationResponse> comb_response;
         hr = the_enveloper->get_LoadCombinationResponse(&comb_response);

         // use sdcl reporter to create report - although it is arbitrary which one is used
         sdcl_reporter.ReportCombinedResponse(comb_response, os);

         // convert stream to our BSTR
	      CComBSTR bstr(os.str().c_str());
         *Report = bstr.Detach();
      }
      else
      {
         // single model - much simpler to deal with
         // build the model
         CComPtr<ILBAMModel> model;
         model.Attach( BuildModel(m_ModelType) );

         // set up our engine
         CComQIPtr<ILBAMAnalysisEngine> the_engine;
         create_hr = the_engine.CoCreateInstance(CLSID_LBAMAnalysisEngine);
         hr = the_engine->Initialize(model, atForce);

         // distribute distribution factors across our model using contraflexure locations
         ConfigureDistributionFactors(the_engine, model);

         // use a utility class to create our report
         std::wostringstream os;
         ModelReporter reporter(model, the_engine, this, m_DoDetailedReport!=VARIANT_FALSE);
         reporter.CreateReport(os);

         // create our BSTR
	      CComBSTR bstr(os.str().c_str());
         *Report = bstr.Detach();
      }
   }
   catch(std::string& rstring)
   {
      // these are "normal errors"
	   CComBSTR bstr(rstring.c_str());
      *Report = bstr.Detach();
   }
   catch(...)
   {
      ATLASSERT(0);
	   CComBSTR bstr("An unknown error happened in the QConLEEngine class");
      *Report = bstr.Detach();
   }

	return S_OK;
}

ILBAMModel* CQConLEEngine::BuildModel(QCLEModelType type)
{
   CComPtr<ILBAMModel> psm;
   create_hr = psm.CoCreateInstance( CLSID_LBAMModel );

   // set up control data for model type
   bool do_release;  // superstructuremember end release
   CComBSTR rem_release_stage;
   switch(type)
   {
   case qcmtS2CC:
      {
         m_NumStages=2;
         do_release=false;
         break;
      }
   case qcmtSDCL:
      {
         m_NumStages=2;
         do_release=true;
         rem_release_stage="Stage 2";
         break;
      }
   case qcmtS2SS:
      {
         m_NumStages=2;
         do_release=true;
         rem_release_stage="";
         break;
      }
   case qcmtS1SS:
      {
         m_NumStages=1;
         do_release=true;
         break;
      }
   case qcmtS1CS:
      {
         m_NumStages=1;
         do_release=false;
         break;
      }
   default:
      {
         throw std::string("Invalid model type");
         break;
      }
   }

   // stages
   CComPtr<IStages> pStages;
   hr=psm->get_Stages(&pStages);

   CComPtr<IStage> pStage1, pStage2;
   create_hr = pStage1.CoCreateInstance( CLSID_Stage );
   hr=pStage1->put_Name(_bstr_t("Stage 1"));
   hr=pStages->Add(pStage1);

   if (m_NumStages==2)
   {
      hr=pStage2.CoCreateInstance( CLSID_Stage );
      hr=pStage2->put_Name(_bstr_t("Stage 2"));
      hr=pStages->Add(pStage2);
   }

   // set up LoadGroups, LoadCases, and LoadCombinations
   ConfigureLrfdLoadCombinations(psm);

   // set up live loads
   ConfigureLiveLoads(psm);

   // next, get to building our model's spans, supports...
   long num_spans;
   hr=m_Spans->get_Count(&num_spans);
   if (num_spans<=0)
   {
      throw std::string("Number of spans must be greater than zero");
   }

   // add left-most support
   CComPtr<ISupports> psups;
   hr=psm->get_Supports(&psups);

   CComPtr<ISupport> psup0;
   create_hr=psup0.CoCreateInstance( CLSID_Support );

   // all Supports are zero length
   hr=psup0->put_BoundaryCondition(bcRoller);
   hr=psups->Add(psup0);

   // add our spans, superstructuremembers, and the rest of the supports
   CComPtr<ISpans> pspns;
   psm->get_Spans(&pspns);

   CComPtr<ISuperstructureMembers> pssms;
   psm->get_SuperstructureMembers(&pssms);


   for (long ns=0; ns<num_spans; ns++)
   {
      // add right support
      CComPtr<ISupport> psup;
      create_hr=psup.CoCreateInstance( CLSID_Support );

      hr=psup->put_BoundaryCondition(bcPinned);
      hr=psups->Add(psup);

      // get data for our span
      CComPtr<IQConLESpan> lespan;
      hr=m_Spans->get_Item(ns, &lespan);

      double length, stageIEI, stageIIEI;
      double stageIDC, stageIIDC, stageIDW, stageIIDW;

      hr=lespan->get_Length(&length);
      hr=lespan->get_StageIEI(&stageIEI);
      hr=lespan->get_StageIIEI(&stageIIEI);

      if (m_NumStages==2 &&stageIEI>stageIIEI)
      {
         throw std::string("Span Stiffnesses must increase or stay equal from Stage I to Stage II");
      }

      hr=lespan->get_StageIDC(&stageIDC);
      hr=lespan->get_StageIIDC(&stageIIDC);
      hr=lespan->get_StageIDW(&stageIDW);
      hr=lespan->get_StageIIDW(&stageIIDW);

      // create a span and add it to the Spans collection
      CComPtr<ISpan> pspan;
      create_hr=pspan.CoCreateInstance( CLSID_Span );

      hr=pspan->put_Length(length);
      hr=pspns->Add(pspan);

      // use a superstructuremember per span to set its properties
      CComPtr<ISuperstructureMember> pss_super;
      create_hr=pss_super.CoCreateInstance( CLSID_SuperstructureMember );
      hr=pss_super->put_Length(length);

      // segments are different for each stage
      CComPtr<ISegment> psegs1, psegs2;

      create_hr=psegs1.CoCreateInstance( CLSID_Segment ) ;
      SetupSegment(psegs1, length, stageIEI, stageIEI);
      hr=pss_super->AddSegment(_bstr_t("Stage 1"), psegs1);

      if (m_NumStages==2)
      {
         psegs2.CoCreateInstance( CLSID_Segment ) ;
         SetupSegment(psegs2, length, stageIIEI, stageIIEI);
         hr=pss_super->AddSegment(_bstr_t("Stage 2"), psegs2);
      }

      // deal with continuous/ss end conditions by releasing the right end of all spans
      // except for the last one
      if (do_release && ns<num_spans-1)
      {
         pss_super->SetEndRelease(ssRight,rem_release_stage,mrtPinned);
      }

      hr=pssms->Add(pss_super);

      // next, apply uniform loads for this span in the correct load groups
      CComPtr<IDistributedLoads> dist_loads;
      hr=psm->get_DistributedLoads(&dist_loads);

      if (stageIDC!=0.0)
      {
         AddDistLoad(dist_loads, CComBSTR("Stage 1"), CComBSTR("DCLG"), ns, stageIDC);
      }

      if (stageIDW!=0.0)
      {
         AddDistLoad(dist_loads, CComBSTR("Stage 1"), CComBSTR("DWLG"), ns, stageIDW);
      }

      if (m_NumStages==2)
      {
         if (stageIIDC!=0.0)
         {
            AddDistLoad(dist_loads, CComBSTR("Stage 2"), CComBSTR("DCLG"), ns, stageIIDC);
         }

         if (stageIIDW!=0.0)
         {
            AddDistLoad(dist_loads, CComBSTR("Stage 2"), CComBSTR("DWLG"), ns, stageIIDW);
         }
      }
   }

   // now that the entire structure is built, we can add pois at tenth points
   CComPtr<ILBAMFactory> poi_util;
   poi_util.CoCreateInstance(CLSID_LRFDFactory);

   long last_val;
   hr=poi_util->GeneratePOIsOnSuperstructure(psm, 1, 10, &last_val);

   // return the completed model
   return psm.Detach();
}


void CQConLEEngine::ConfigureDistributionFactors(ILBAMAnalysisEngine* engine, ILBAMModel* model)
{

   long num_spans;
   hr=m_Spans->get_Count(&num_spans);
   if (num_spans<=0)
   {
      throw std::string("Number of spans must be greater than zero");
   }

   // simply supported and continuous models are treated differently
   if (num_spans==1 || m_ModelType==qcmtS1SS)
   {
      ConfigureSsDistributionFactors(num_spans, model);
   }
   else
   {
      ConfigureCsDistributionFactors(num_spans, engine, model);
   }
}

void CQConLEEngine::ConfigureSsDistributionFactors(long numSpans, ILBAMModel* model)
{
   // simple span models only require assignment of span's distribution factors alone entire span
   // don't need to mess with contraflexure

   // model's distribution factors collection
   CComPtr<IDistributionFactors> dist_factors;
   hr=model->get_DistributionFactors(&dist_factors);

   // loop over each span and layout df's for each
   double span_end=0.0;
   for (long ns=0; ns<numSpans; ns++)
   {
      // get data for our span
      CComPtr<IQConLESpan> currspan;
      hr=m_Spans->get_Item(ns, &currspan);

      double length;
      hr=currspan->get_Length(&length);

      double gMp, gMn, gV;
      hr=currspan->get_gMp(&gMp);
      hr=currspan->get_gMn(&gMn);
      hr=currspan->get_gV(&gV);

      // Apply distribution factors
      AddDistributionFactors(dist_factors, length, gMp, gMn, gV);
   }
}

void CQConLEEngine::ConfigureCsDistributionFactors(long numSpans, ILBAMAnalysisEngine* engine, ILBAMModel* model)
{
   // continuous model - need to deal with contraflexure
   ATLASSERT(numSpans>1);

   // model's distribution factors collection
   CComPtr<IDistributionFactors> dist_factors;
   hr=model->get_DistributionFactors(&dist_factors);

   // get locations of points of contraflexure for last stage of model
   CComBSTR bsstage(  (m_NumStages==1?"Stage 1":"Stage 2"));

   CComPtr<ILoadGroupResponse> response;
   hr = engine->get_LoadGroupResponse(&response);

   CComQIPtr<IContraflexureResponse> cfr(response);
   ATLASSERT(cfr!=NULL);

   CComPtr<IDblArray> cf_locs;
   hr=cfr->ComputeContraflexureLocations(bsstage, &cf_locs);

   // loop over each span and layout df's for each
   double span_end=0.0;
   for (long ns=0; ns<numSpans; ns++)
   {
      // get data for our span
      CComPtr<IQConLESpan> currspan;
      hr=m_Spans->get_Item(ns, &currspan);

      double length;
      hr=currspan->get_Length(&length);

      double span_start=span_end;
      span_end+=length;

      double gmp, gmn, gmn_left_sup, gv;
      hr=currspan->get_gMp(&gmp);
      hr=currspan->get_gMn(&gmn);
      hr=currspan->get_gMnSupport(&gmn_left_sup);
      hr=currspan->get_gV(&gv);

      // determine how many contraflexure points lie within this span
      double cf_points_in_span[2];
      long num_cf_points_in_span = GetCfPointsInRange(cf_locs, span_start, span_end, cf_points_in_span);

      if (ns==0) // left_exterior_span
      {
         CComPtr<IQConLESpan> rightspan;
         hr=m_Spans->get_Item(ns+1, &rightspan);
         double gmn_right_sup;
         hr=rightspan->get_gMnSupport(&gmn_right_sup);

         if (num_cf_points_in_span==0)
         {
            // split at mid-span and use span_df and right suprt_df
            double l2 = length/2.0;
            AddDistributionFactors(dist_factors, l2, gmp, gmn, gv);
            AddDistributionFactors(dist_factors, l2, gmp, gmn_right_sup, gv);
         }
         else if (num_cf_points_in_span==1)
         {
            // split at cf_point and use span_df and right suprt_df;
            double l1 = cf_points_in_span[0] - span_start;
            AddDistributionFactors(dist_factors, l1, gmp, gmn, gv);
            double l2 = span_end - cf_points_in_span[0];
            AddDistributionFactors(dist_factors, l2, gmp, gmn_right_sup, gv);
         }
         else
         {
            ATLASSERT(0);
            throw std::string("Exterior spans should only have zero or one contraflexure point");
         }
      }
      else if (ns==numSpans-1) //  right_exterior_span
      {
         if (num_cf_points_in_span==0)
         {
            // split at mid-span and use left suprt_df and span_df
            double l2 = length/2.0;
            AddDistributionFactors(dist_factors, l2, gmp, gmn_left_sup, gv);
            AddDistributionFactors(dist_factors, l2, gmp, gmn, gv);
         }
         else if (num_cf_points_in_span==1)
         {
            // split at cf_point and use left suprt_df and span_df
            double l1 = cf_points_in_span[0] - span_start;
            AddDistributionFactors(dist_factors, l1, gmp, gmn_left_sup, gv);
            double l2 = span_end - cf_points_in_span[0];
            AddDistributionFactors(dist_factors, l2, gmp, gmn, gv);
         }
         else
         {
            ATLASSERT(0);
            throw std::string("Exterior spans should only have zero or one contraflexure point");
         }
      }
      else // interior_span
      {
         CComPtr<IQConLESpan> rightspan;
         hr=m_Spans->get_Item(ns+1, &rightspan);
         double gmn_right_sup;
         hr=currspan->get_gMnSupport(&gmn_right_sup);

         if (num_cf_points_in_span==0)
         {
            // split at mid-span and use left and right suprt_df's - don't use span_df;
            double l2 = length/2.0;
            AddDistributionFactors(dist_factors, l2, gmp, gmn_left_sup, gv);
            AddDistributionFactors(dist_factors, l2, gmp, gmn_right_sup, gv);
         }
         else if (num_cf_points_in_span==1)
         {
            // split at cf_point use left and right suprt_df's and don't use span_df
            double l1 = cf_points_in_span[0] - span_start;
            AddDistributionFactors(dist_factors, l1, gmp, gmn_left_sup, gv);
            double l2 = span_end - cf_points_in_span[0];
            AddDistributionFactors(dist_factors, l2, gmp, gmn_right_sup, gv);
         }
         else if (num_cf_points_in_span==2)
         {
             // split at cf_point's and use left and right suprt_df's and span_df
            double l1 = cf_points_in_span[0] - span_start;
            AddDistributionFactors(dist_factors, l1, gmp, gmn_left_sup, gv);
            double l2 = cf_points_in_span[1] - cf_points_in_span[0];
            AddDistributionFactors(dist_factors, l2, gmp, gmn, gv);
            double l3 = span_end - cf_points_in_span[1];
            AddDistributionFactors(dist_factors, l3, gmp, gmn_right_sup, gv);
         }
         else
         {
            ATLASSERT(0);
            throw std::string("Interior spans should only have zero, one or two contraflexure points");
         }
      }
   }
}

long GetCfPointsInRange(IDblArray* cfLocs, double spanStart, double spanEnd, double* ptsInrg)
{
   // we don't want to pick up cf points at the very ends
   const double TOL=1.0e-07;
   spanStart+= TOL;
   spanEnd-=TOL;

   long cf_cnt=0;

   // assumption here is that the cfLocs are sorted
   long siz;
   cfLocs->get_Count(&siz);
   for (long ic=0; ic<siz; ic++)
   {
      double cfl;
      cfLocs->get_Item(ic,&cfl);
      if (cfl>spanStart && cfl<spanEnd)
      {
         // cf is within span
         cf_cnt++;
         if (cf_cnt<3)
         {
            ptsInrg[cf_cnt-1] = cfl;
         }
         else
         {
            ATLASSERT(0);
            throw std::string("More than two contraflexure points in a Span - This should be impossible");
         }
      }
      
      // no use continuing loop if we are past the span end
      if (cfl>spanEnd)
         break;
   }


   return cf_cnt;
}


void AddDistributionFactors(IDistributionFactors* distFactors, double length, double gMp, double gMn, double gV)
{
   CComPtr<IDistributionFactor> df;
   create_hr=df.CoCreateInstance( CLSID_DistributionFactor );

   hr=df->SetG(gMp, gMp, gMn, gMn, gV, gV, gMp, gMp, gV, gV, gMp, gMp, 1.0);

   CComPtr<IDistributionFactorSegment> dfs;
   create_hr=dfs.CoCreateInstance( CLSID_DistributionFactorSegment );

   hr=dfs->putref_DistributionFactor(df);
   hr=dfs->put_Length(length);

   hr=distFactors->Add(dfs);
}

void AddDistLoad(IDistributedLoads* distLoads, BSTR stage, BSTR loadgroup, long spanNum, double load)
{
   CComPtr<IDistributedLoad> pdl;
   create_hr=pdl.CoCreateInstance( CLSID_DistributedLoad );

   hr=pdl->put_Direction(ldFy);
   hr=pdl->put_StartLocation(0.0);
   hr=pdl->put_EndLocation(-1.0);
   hr=pdl->put_WStart(-load);
   hr=pdl->put_WEnd(-load);

   hr=pdl->put_MemberType(mtSpan);
   hr=pdl->put_MemberID(spanNum);

   CComPtr<IDistributedLoadItem> pdli;
   hr=distLoads->Add(stage, loadgroup, pdl, &pdli);
}


void SetupSegment(ISegment* pseg, double Length, double EA, double EI)
{

   CComPtr<ICrossSection> pcs;
   create_hr=pcs.CoCreateInstance( CLSID_CrossSection );

   double ea, ei;
   ea = EA;
   ei = EI;
   double depth=1.0;

   hr=pcs->put_EAForce(ea);
   hr=pcs->put_EIForce(ei);
   hr=pcs->put_EADefl(ea);
   hr=pcs->put_EIDefl(ei);
   hr=pcs->put_ThermalCoeff(1.0);
   hr=pcs->put_Depth(depth);

   hr=pseg->putref_CrossSection(pcs);
   hr=pseg->put_Length(Length);
}

void AddLoadGroup(ILoadGroups* loadGroups, BSTR name, BSTR description)
{
   CComPtr<ILoadGroup> load_group;
   create_hr = load_group.CoCreateInstance(CLSID_LoadGroup);
   hr = load_group->put_Name(name);
   hr = load_group->put_Description(name);
   hr = loadGroups->Add(load_group);
}

void AddLoadCase(ILoadCases* loadCases, BSTR name, BSTR description, BSTR loadgroup)
{
   CComPtr<ILoadCase> load_case;
   create_hr = load_case.CoCreateInstance(CLSID_LoadCase);
   hr = load_case->put_Name(name);
   hr = load_case->put_Description(name);
   hr = load_case->AddLoadGroup(loadgroup);
   hr = loadCases->Add(load_case);
}


void ApplyTypicalLoadFactors(ILoadCombination* loadCombo)
{
   // apply the most common load factors
   hr = loadCombo->AddLoadCaseFactor(CComBSTR("DC"), 0.90, 1.25);
   hr = loadCombo->AddLoadCaseFactor(CComBSTR("DW"), 0.65, 1.50);
}

void ApplyTypicalServiceLoadFactors(ILoadCombination* loadCombo)
{
   // apply the most common load factors
   hr = loadCombo->AddLoadCaseFactor(CComBSTR("DC"), 1.0, 1.0);
   hr = loadCombo->AddLoadCaseFactor(CComBSTR("DW"), 1.0, 1.0);
}


void ConfigureLrfdLoadCombinations(ILBAMModel* model)
{
   // This function adds load cases and combinations as per LRFD 3.4.1
   // Add load groups
   CComPtr<ILoadGroups> loadgroups;
   hr = model->get_LoadGroups(&loadgroups);

   AddLoadGroup(loadgroups, CComBSTR("DCLG"), CComBSTR("LoadGroup for DC LoadCase"));
   AddLoadGroup(loadgroups, CComBSTR("DWLG"), CComBSTR("LoadGroup for DW LoadCase"));

   // Add load cases
   CComPtr<ILoadCases> loadcases;
   hr = model->get_LoadCases(&loadcases);

   AddLoadCase(loadcases, CComBSTR("DC"), CComBSTR("Component and Attachments"), CComBSTR("DCLG"));
   AddLoadCase(loadcases, CComBSTR("DW"), CComBSTR("Wearing Surfaces and Utilities"), CComBSTR("DWLG"));

   CComPtr<ILoadCombinations> loadcombos;
   hr = model->get_LoadCombinations(&loadcombos);

   // add load combinations
   // STRENGTH-I
   CComPtr<ILoadCombination> strength1;
   create_hr = strength1.CoCreateInstance(CLSID_LoadCombination);
   hr = strength1->put_Name(CComBSTR("STRENGTH-I"));
   hr = strength1->put_LoadCombinationType(lctStrength);
   hr = strength1->put_LiveLoadFactor(1.75);
   hr = strength1->AddLiveLoadModel(lltDesign);

   ApplyTypicalLoadFactors(strength1);

   hr = loadcombos->Add(strength1);

   // STRENGTH-II
   CComPtr<ILoadCombination> strength2;
   create_hr = strength2.CoCreateInstance(CLSID_LoadCombination);
   hr = strength2->put_Name(CComBSTR("STRENGTH-II"));
   hr = strength2->put_LoadCombinationType(lctStrength);
   hr = strength2->put_LiveLoadFactor(1.35);
   hr = strength2->AddLiveLoadModel(lltPermit);

   ApplyTypicalLoadFactors(strength2);

   hr = loadcombos->Add(strength2);

   // SERVICE-I
   CComPtr<ILoadCombination> service1;
   create_hr = service1.CoCreateInstance(CLSID_LoadCombination);
   hr = service1->put_Name(CComBSTR("SERVICE-I"));
   hr = service1->put_LoadCombinationType(lctService);
   hr = service1->put_LiveLoadFactor(1.00);
   hr = service1->AddLiveLoadModel(lltDesign);

   ApplyTypicalServiceLoadFactors(service1);

   hr = loadcombos->Add(service1);

   // SERVICE-II
   CComPtr<ILoadCombination> service2;
   create_hr = service2.CoCreateInstance(CLSID_LoadCombination);
   hr = service2->put_Name(CComBSTR("SERVICE-II"));
   hr = service2->put_LoadCombinationType(lctService);
   hr = service2->put_LiveLoadFactor(1.30);
   hr = service2->AddLiveLoadModel(lltDesign);

   ApplyTypicalServiceLoadFactors(service2);

   hr = loadcombos->Add(service2);

   // SERVICE-III
   CComPtr<ILoadCombination> service3;
   hr = service3.CoCreateInstance(CLSID_LoadCombination) ;
   hr = service3->put_Name(CComBSTR("SERVICE-III")) ;
   hr = service3->put_LoadCombinationType(lctService) ;
   hr = service3->put_LiveLoadFactor(0.80) ;
   hr = service3->AddLiveLoadModel(lltDesign) ;

   ApplyTypicalServiceLoadFactors(service3);

   hr = loadcombos->Add(service3) ;

   // FATIGUE
   CComPtr<ILoadCombination> fatigue;
   hr = fatigue.CoCreateInstance(CLSID_LoadCombination) ;
   hr = fatigue->put_Name(CComBSTR("FATIGUE")) ;
   hr = fatigue->put_LoadCombinationType(lctFatigue) ;
   hr = fatigue->put_LiveLoadFactor(0.75) ;
   hr = fatigue->AddLiveLoadModel(lltFatigue) ;

   hr = loadcombos->Add(fatigue) ;
}

void CQConLEEngine::ConfigureLiveLoads(ILBAMModel* psm)
{
   CComPtr<ILBAMLRFDFactory> gen_util;
   gen_util.CoCreateInstance(CLSID_LRFDFactory);

   CComPtr<IUnitServer> unitserver;
   unitserver.CoCreateInstance(CLSID_UnitServer);

   SpecUnitType sut;
   if (qcmtUS==m_UnitType)
   {
      hr=unitserver->SetBaseUnits(CComBSTR("kslug"), CComBSTR("ft"), CComBSTR("sec"), CComBSTR("F"), CComBSTR("rad"));
      sut = suUS;
   }
   else
   {
      hr=unitserver->SetBaseUnits(CComBSTR("Mg"), CComBSTR("m"), CComBSTR("sec"), CComBSTR("C"), CComBSTR("rad"));
      sut = suSI;
   }

   hr = gen_util->ConfigureLiveLoad(psm, m_IncludeLowBoy, sut, unitserver);
}


STDMETHODIMP CQConLEEngine::ExportLBAM()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

   // first put up a prompt to open the file
  // Prepare the open file dialog
    static char szFilter[] = _T( "XML Files (*.xml)|*.xml" );

   CFileDialog dlg(FALSE, "xml", NULL, NULL, szFilter);

   // open file dialog
   if(dlg.DoModal() != IDOK)
   {
    return S_OK;
   }

   CComBSTR strFile(dlg.m_ofn.lpstrFile);

   // create our file loader
   CComPtr<IStructuredSave2> saver;
   HRESULT hr = saver.CoCreateInstance(CLSID_StructuredSave2);
   if (FAILED(hr)) 
   {
    ::AfxMessageBox("Failed to Create CLSID_StructuredSave2 - component is probably not installed");
    return hr;
   }

   hr = saver->Open(strFile);
   if (FAILED(hr)) 
   {
    ::AfxMessageBox("Unable to open file");
    return hr;
   }

   try
   {
      // build the model
      CComPtr<ILBAMModel> model (BuildModel(m_ModelType));

      CComQIPtr<IStructuredStorage2> piss(model);
      ATLASSERT(model!=NULL);

      hr = piss->Save(saver);
      if (FAILED(hr)) 
      {
       ::AfxMessageBox("Failed to save file");
       return hr;
      }

      hr=saver->Close();
   }
   catch(std::string& rstring)
   {
      // these are "normal errors"
	   CString cstr(rstring.c_str());
      AfxMessageBox(cstr, MB_OK);
   }
   catch(...)
   {
      ATLASSERT(0);
	   CString cstr("An unknown error happened in the QConLEEngine class");
      AfxMessageBox(cstr, MB_OK);
   }

	return S_OK;
}

