// ModelReporter.cpp: implementation of the ModelReporter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QConLE.h"
#include "ModelReporter.h"

#include "QConLEEngine.h"
#include "MathEx.h"

#include <ProgressMonitorUtils.h>

#include <iomanip>
#include <fstream>

// keep two statics around for expediency
static CHRTextHucker create_hr("An error occured while creating an LBAM object - Perhaps the LBAM components are not installed?");
static CHRTextHucker hr;

// free functions
void GetSpanTenth(long poiId, long* span, long* tenth)
{
   // decode poi id into a span and tenth value
   *span  = (poiId-1)/11 + 1;
   *tenth = (poiId-1)%11;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ModelReporter::ModelReporter(ILBAMModel* theModel, ILBAMAnalysisEngine* theEngine, CQConLEEngine* leEngine, bool doDetailed):
m_TheModel(theModel),
m_TheEngine(theEngine),
m_pLEEngine(leEngine),
m_LoadGroupCookie(1),
m_LiveLoadCookie(2),
m_LoadCombinationCookie(3),
m_DoDetailedReport(doDetailed)
{
   // create and wire up progress monitoring window
   hr = m_pProgressMonitorWindow.CoCreateInstance(CLSID_ProgressMonitorWindow);
   ATLASSERT(SUCCEEDED(hr));

   CComQIPtr<IProgressMonitor> ipm(m_pProgressMonitorWindow);

   // engine has its own interface for wiring up progress monitoring 
   CComQIPtr<ISupportProgressMonitorForEngine> ispme(m_TheEngine);
   hr = ispme->InitializeProgressMonitor4E(ipm, m_LoadGroupCookie, m_LiveLoadCookie, m_LoadCombinationCookie);
   ATLASSERT(SUCCEEDED(hr));

   // get sorted list of pois and their locations for use in reports
   CComPtr<ILBAMFactory> poi_util;
   create_hr=poi_util.CoCreateInstance(CLSID_LRFDFactory);
   hr = poi_util->GetSuperstructurePOIs(m_TheModel, &m_PoiIds, &m_PoiLocs);

   // a list of supports
   m_NumSpans;
   hr = m_pLEEngine->m_Spans->get_Count(&m_NumSpans);
   m_SupportIds.CoCreateInstance(CLSID_LngArray);
   hr = m_SupportIds->Reserve(m_NumSpans+1);
   for (long is=0; is<m_NumSpans+1; is++)
   {
      m_SupportIds->Add(is);
   }

   // some needed constants
   m_UnitType = m_pLEEngine->m_UnitType; 
   m_NumStages = m_pLEEngine->m_NumStages; 

   // formatters
   hr = m_FLen.Init(10, 3);
   hr = m_FEI.Init(10, 2);
   hr = m_Fg.Init(5, 2);
   hr = m_Fw.Init(10, 2);
   hr = m_Fm.Init(11, 3);
   hr = m_Fv.Init(11, 3);
   hr = m_Fd.Init(11, 3);
}

void ModelReporter::CreateReport(std::wostringstream& os)
{

   // this will be time consuming - put up progress monitor window
   m_pProgressMonitorWindow->Show(CComBSTR("Processing report..."));
   CProgressAutoHide autohide(m_pProgressMonitorWindow); // make sure Hide() gets called even if exception gets thrown
   CComQIPtr<IProgressMonitor> prog_mon(m_pProgressMonitorWindow);

   // input, contraflexure, and dead load responses
   ReportInputData(os);

   if (m_DoDetailedReport)
   {
      if(m_NumSpans>1 && (m_pLEEngine->m_ModelType!=qcmtS1SS || m_pLEEngine->m_ModelType!=qcmtS2SS))
      {
         ReportContraflexureLocations(os);
      }

      ReportDistributionFactors(os);
   }

   ReportLoadCaseResponse(os);

   // live load responses
   prog_mon->put_Message(m_LoadCombinationCookie, CComBSTR("Processing Individual Live Load Response")); // cookie is arbitrary
   prog_mon->put_GaugeValue(m_LoadCombinationCookie,10);
   ReportVehicularResponse(os);

   prog_mon->put_Message(m_LoadCombinationCookie, CComBSTR("Processing Combined Live Load Response"));
   prog_mon->put_GaugeValue(m_LoadCombinationCookie,50);
   ReportLlmResponse(os);

   // combined responses
   CComPtr<ILoadCombinationResponse> response;
   hr = m_TheEngine->get_LoadCombinationResponse(&response);


   prog_mon->put_Message(m_LoadCombinationCookie, CComBSTR("Processing Load Combination Response"));
   prog_mon->put_GaugeValue(m_LoadCombinationCookie,75);
   ReportCombinedResponse(response, os);

}

void ModelReporter::ReportInputData(std::wostringstream& os)
{
   os<<L"INPUT DATA"<<std::endl;
   os<<L"=========="<<std::endl<<L" "<<std::endl;
   os<<L"  Model Type: ";
   if(m_pLEEngine->m_ModelType==qcmtS2CC)
      os<<L"Two Stage, Continuous/Continuous Spans"<<std::endl;
   else if(m_pLEEngine->m_ModelType==qcmtSDCL)
      os<<L"Two Stage, Simple/Continuous Spans"<<std::endl;
   else if(m_pLEEngine->m_ModelType==qcmtS1SS)
      os<<L"One Stage, Simple Spans"<<std::endl;
   else if(m_pLEEngine->m_ModelType==qcmtS1CS)
      os<<L"One Stage, Continuous Spans"<<std::endl;
   else if(m_pLEEngine->m_ModelType==qcmtS2SS)
      os<<L"Two Stage, Simple Spans"<<std::endl;
   else
   {
      throw std::string("Invalid Model Type");
   }

   os<<L"  Units: "<<(m_pLEEngine->m_UnitType==qcmtUS ? L"kip-ft" : L"kN-m")<<std::endl;

   if (m_NumSpans<=0)
   {
      throw std::string("Number of spans must be greater than zero");
   }

   if(m_pLEEngine->m_IncludeLowBoy && m_NumSpans>1 && m_pLEEngine->m_ModelType!=qcmtS1SS)
      os<<L"  Results of LRFD Dual Tandem Train (\"Low Boy\") were used in analysis"<<std::endl<<std::endl;

   os<<L"  Limit States selected";
   if (m_pLEEngine->m_DoServiceI!=VARIANT_FALSE)
      os <<L"; Service-I";

   if (m_pLEEngine->m_DoServiceII!=VARIANT_FALSE)
      os <<L"; Service-II";

   if (m_pLEEngine->m_DoServiceIII!=VARIANT_FALSE)
      os <<L"; Service-III";

   if (m_pLEEngine->m_DoStrengthI!=VARIANT_FALSE)
      os <<L"; Strength-I";

   if (m_pLEEngine->m_DoFatigue!=VARIANT_FALSE)
      os <<L"; Fatigue";

   os<<std::endl<<L" "<<std::endl;

   os<<L"SPAN DATA"<<std::endl;
   os<<L"---------"<<std::endl;

   if (m_NumStages==2)
   {
      // have to print span table in two tables due to width constraints
      os<<L"                  Stage I   Stage II"                  <<std::endl;
      os<<L"       Length       EI         EI"                     <<std::endl;
      if (m_pLEEngine->m_UnitType==qcmtUS)
      {
      os<<L"Span   (ft)    (kip-ft^2) (kip-ft^2)  gM+   gM-   gV" <<std::endl;
      }
      else
      {
      os<<L"Span    (m)     (kN-m^2)   (kN-m^2)   gM+   gM-   gV" <<std::endl;
      }
      os<<L"---- --------- ---------- ---------- ----- ----- -----"<<std::endl;

      for (long ns=0; ns<m_NumSpans; ns++) 
      {
         // get data for our span
         CComPtr<IQConLESpan> lespan;
         hr=m_pLEEngine->m_Spans->get_Item(ns, &lespan);

         double length, stageIEI, stageIIEI;
         double gMp, gMn, gMnSupport, gV;
         hr=lespan->get_Length(&length);
         hr=lespan->get_StageIEI(&stageIEI);
         hr=lespan->get_StageIIEI(&stageIIEI);
         hr=lespan->get_gMp(&gMp);
         hr=lespan->get_gMn(&gMn);
         hr=lespan->get_gMnSupport(&gMnSupport);
         hr=lespan->get_gV(&gV);

         if (ns>0)
            os<<std::setw(43)<<L""<<m_Fg.Str(gMnSupport)<<std::endl;

         os<<std::setw(4)<<ns+1<<m_FLen.Str(length)<<L" "<<m_FEI.Str(stageIEI)<<L" "<<m_FEI.Str(stageIIEI)<<
            L" "<<m_Fg.Str(gMp)<<L" "<<m_Fg.Str(gMn)<<m_Fg.Str(gV)<<std::endl;
      }

      os<<L" "<<std::endl;
 
      // load data
      os<<L"      Stage I     Stage I   Stage II   Stage II "<<std::endl;
      os<<L"        DC          DW         DC         DW    "  <<std::endl;
      if (m_pLEEngine->m_UnitType==qcmtUS)
      {
      os<<L"Span  (kip/ft)   (kip/ft)   (kip/ft)   (kip/ft) " <<std::endl;
      }
      else
      {
      os<<L"Span   (kN/m)     (kN/m)     (kN/m)     (kN/m)  "<<std::endl;
      }
      os<<L"---- ---------- ---------- ---------- ----------"<<std::endl;

      for (ns=0; ns<m_NumSpans; ns++)
      {
         CComPtr<IQConLESpan> lespan;
         hr=m_pLEEngine->m_Spans->get_Item(ns, &lespan);

         double stageIDC, stageIDW, stageIIDC, stageIIDW;
         hr=lespan->get_StageIDC(&stageIDC);
         hr=lespan->get_StageIDW(&stageIDW);
         hr=lespan->get_StageIIDC(&stageIIDC);
         hr=lespan->get_StageIIDW(&stageIIDW);

         os<<std::setw(4)<<ns+1<<L" "<<m_Fw.Str(stageIDC)<<L" "<<m_Fw.Str(stageIDW)<<
           L" "<<m_Fw.Str(stageIIDC)<<L" "<<m_Fw.Str(stageIIDW)<<std::endl;
      }
   }
   else
   {
      os<<L"                  Stage I                    Stage I   Stage I"<<std::endl;
      os<<L"       Length       EI                         DC        DW"  <<std::endl;
      if (m_pLEEngine->m_UnitType==qcmtUS)
      {
      os<<L" Span   (ft)    (kip-ft^2) gM+   gM-   gV    (kip/ft)  (kip/ft)" <<std::endl;
      }
      else
      {
      os<<L" Span   (m)     (kN-m^2)   gM+   gM-   gV     (kN/m)    (kN/m)"<<std::endl;
      }
      os<<L"---- --------- ---------- ----- ----- ----- --------- ----------"<<std::endl;
//         12345678911234567892123456789312345678941234567890

      for (long ns=0; ns<m_NumSpans; ns++)
      {
         CComPtr<IQConLESpan> lespan;
         hr=m_pLEEngine->m_Spans->get_Item(ns, &lespan);

         double length, stageIEI;
         double gMp, gMn, gMns, gV;
         double stageIDC, stageIDW;

         hr=lespan->get_Length(&length);
         hr=lespan->get_StageIEI(&stageIEI);

         hr=lespan->get_gMp(&gMp);
         hr=lespan->get_gMn(&gMn);
         hr=lespan->get_gMnSupport(&gMns);
         hr=lespan->get_gV(&gV);

         hr=lespan->get_StageIDC(&stageIDC);
         hr=lespan->get_StageIDW(&stageIDW);

         if (ns>0)
            os<<std::setw(32)<<L""<<m_Fg.Str(gMns)<<std::endl;

         os<<std::setw(4)<<ns+1<<m_FLen.Str(length)<<L" "<<m_FEI.Str(stageIEI)<<L" "<<
             m_Fg.Str(gMp)<<L" "<<m_Fg.Str(gMn)<<L" "<<m_Fg.Str(gV)<<L""<<
             m_Fw.Str(stageIDC)<<L" "<<m_Fw.Str(stageIDW)<<std::endl;
      }
   }

   os<<L" "<<std::endl;
}

void ModelReporter::ReportDistributionFactors(std::wostringstream& os)
{
   CComBSTR stage((m_NumStages==1?"Stage 1":"Stage 2"));

   os << L"LIVE LOAD DISTRIBUTION FACTORS"<<std::endl;
   os << L"=============================="<<std::endl<<L" "<<std::endl;
   os<<L"  Reported for "<<(m_NumStages==1? L"Stage I only":L"Stage II only")<<std::endl<<L" "<<std::endl;

   os<<L"          Location   gM+    gM-     gV "<<std::endl;
   if (m_UnitType==qcmtUS)
   {
   os<<L"Span POI    (ft)"   <<std::endl;
   }
   else
   {
   os<<L"Span POI     (m)"   <<std::endl;
   }
   os<<L"---- ---- --------- ------ ------ ------"<<std::endl;

   // we can get df interface from LoadGroupResponse 
   CComPtr<ILoadGroupResponse> response;
   hr = m_TheEngine->get_LoadGroupResponse(&response);

   CComQIPtr<IGetDistributionFactors> igdf(response);
   ATLASSERT(igdf!=NULL);


   long num_pois;
   hr = m_PoiIds->get_Count(&num_pois);
   for (long ip=0; ip<num_pois; ip++)
   {
      long poi_id;
      hr = m_PoiIds->get_Item(ip,&poi_id);
      long span, tenth;
      GetSpanTenth(poi_id, &span, &tenth);

      CComPtr<IDistributionFactor> left_df, right_df;
      hr=igdf->GetPOIDistributionFactor(poi_id, stage, &left_df, &right_df);

      double gmp, gmn, gv;
      hr=left_df->get_GPMMul(&gmp);
      hr=left_df->get_GNMMul(&gmn);
      hr=left_df->get_GVMul(&gv);

      double poi_loc;
      hr = m_PoiLocs->get_Item(ip, &poi_loc);

      os<<std::setw(4)<<span<<L" "<<std::setw(4)<<tenth<<m_FLen.Str(poi_loc)
        <<L"  "<<m_Fg.Str(gmp)<<L"  "<<m_Fg.Str(gmn)<<L"  "<<m_Fg.Str(gv)<<std::endl;

      if (tenth!=0 && tenth!=10 && right_df!=NULL)
      {
         hr=right_df->get_GPMMul(&gmp);
         hr=right_df->get_GNMMul(&gmn);
         hr=right_df->get_GVMul(&gv);

         os<<std::setw(4)<<span<<L" "<<std::setw(4)<<tenth<<m_FLen.Str(poi_loc)
           <<L"  "<<m_Fg.Str(gmp)<<L"  "<<m_Fg.Str(gmn)<<L"  "<<m_Fg.Str(gv)<<std::endl;
      }
   }

   os<<L" "<<std::endl;
   os<<L" "<<std::endl;
}

void ModelReporter::ReportLoadCaseResponse(std::wostringstream& os)
{
   os << L"DEAD LOAD RESPONSES"<<std::endl;
   os << L"==================="<<std::endl<<L" "<<std::endl;

   // determine if responses exist for our loadcases
   bool is_s1dc, is_s1dw, is_s2dc, is_s2dw;
   GetLoadCaseControl(is_s1dc, is_s1dw, is_s2dc, is_s2dw);

   os <<std::endl<<L"DC, Stage I Dead Load"<<std::endl
                 <<L"---------------------"<<std::endl;
   if (is_s1dc)
   {
      ReportSpecificLoadCase(CComBSTR("DC"), CComBSTR("Stage 1"), rsIncremental, os);
   }
   else
   {
      os<<L"  Response for this load case is zero - No results to report"<<std::endl<<L" "<<std::endl;
   }

   os <<std::endl<<L"DW, Stage I Dead Load"<<std::endl
                 <<L"---------------------"<<std::endl;
   if (is_s1dw)
   {
      ReportSpecificLoadCase(CComBSTR("DW"), CComBSTR("Stage 1"), rsIncremental, os);
   }
   else
   {
      os<<L"  Response for this load case is zero - No results to report"<<std::endl<<L" "<<std::endl;
   }

   if (m_NumStages>1)
   {
      os <<std::endl<<L"DC, Stage II Incremental Dead Load"<<std::endl
                    <<L"----------------------------------"<<std::endl;
      if (is_s2dc)
      {
         ReportSpecificLoadCase(CComBSTR("DC"), CComBSTR("Stage 2"), rsIncremental, os);
      }
      else
      {
         os<<L"  Response for this load case is zero - No results to report"<<std::endl<<L" "<<std::endl;
      }

      os <<std::endl<<L"DW, Stage II Incremental Dead Load"<<std::endl
                    <<L"----------------------------------"<<std::endl;
      if (is_s2dw)
      {
         ReportSpecificLoadCase(CComBSTR("DW"), CComBSTR("Stage 2"), rsIncremental, os);
      }
      else
      {
         os<<L"  Response for this load case is zero - No results to report"<<std::endl<<L" "<<std::endl;
      }

      os <<std::endl<<L"DC, Stage II Cumulative Dead Load"<<std::endl
                    <<L"---------------------------------"<<std::endl;
      if (is_s1dc||is_s2dc)
      {
         ReportSpecificLoadCase(CComBSTR("DC"), CComBSTR("Stage 2"), rsCumulative, os);
      }
      else
      {
         os<<L"  Response for this load case is zero - No results to report"<<std::endl<<L" "<<std::endl;
      }

      os <<std::endl<<L"DW, Stage II Cumulative Dead Load"<<std::endl
                    <<L"---------------------------------"<<std::endl;
      if (is_s1dw||is_s2dw)
      {
         ReportSpecificLoadCase(CComBSTR("DW"), CComBSTR("Stage 2"), rsCumulative, os);
      }
      else
      {
         os<<L"  Response for this load case is zero - No results to report"<<std::endl<<L" "<<std::endl;
      }
   }
}

void ModelReporter::ReportSpecificLoadCase(BSTR loadCase, BSTR stage, ResultsSummationType summ, std::wostringstream& os)
{
   os<<L" "<<std::endl;
   os<<L"          Location    Moment     Shear    Deflection"<<std::endl;
   if (m_UnitType==qcmtUS)
   {
   os<<L"Span POI    (ft)     (kip-ft)    (kip)       (ft)"   <<std::endl;
   }
   else
   {
   os<<L"Span POI     (m)      (kN-m)      (kN)       (m)"   <<std::endl;
   }
   os<<L"---- ---- --------- ---------- ---------- ----------"<<std::endl;

   // compute member responses
   CComPtr<ILoadCaseResponse> response;
   hr = m_TheEngine->get_LoadCaseResponse(&response);

   CComPtr<ISectionResult3Ds> force_results, defl_results;
   hr=response->ComputeForces(loadCase, m_PoiIds, stage, roMember, summ, &force_results);
   hr=response->ComputeDeflections(loadCase, m_PoiIds, stage, summ, &defl_results);

   long num_pois;
   hr = m_PoiIds->get_Count(&num_pois);
   for (long ip=0; ip<num_pois; ip++)
   {
      long poi_id;
      hr = m_PoiIds->get_Item(ip, &poi_id);
      long span, tenth;
      GetSpanTenth(poi_id, &span, &tenth);
      double moment, shear, defl;
      CComPtr<ISectionResult3D> force_result;
      hr = force_results->get_Item(ip, &force_result);
      hr = force_result->get_ZLeft(&moment);
      hr = force_result->get_YLeft(&shear);

      CComPtr<ISectionResult3D> defl_result;
      hr = defl_results->get_Item(ip, &defl_result);
      hr = defl_result->get_YLeft(&defl);
      
      double poi_loc;
      hr = m_PoiLocs->get_Item(ip,&poi_loc);
      os<<std::setw(4)<<span<<L" "<<std::setw(4)<<tenth<<m_FLen.Str(poi_loc)
        <<m_Fm.Str(moment)<<m_Fv.Str(shear)<<m_Fd.Str(defl)<<std::endl;
   }

   // reactions
   CComPtr<IResult3Ds> react_results;
   hr=response->ComputeReactions(loadCase, m_SupportIds, stage, summ, &react_results);

   os<<L" "<<std::endl;
   os<<L" "<<std::endl;

   os<<L"         Reaction"<<std::endl;
   if (m_UnitType==qcmtUS)
   {
   os<<L"Support   (kip)"   <<std::endl;
   }
   else
   {
   os<<L"Support    (kN)"   <<std::endl;
   }
   os<<L"------- ----------"<<std::endl;

   long num_supp;
   hr = m_SupportIds->get_Count(&num_supp);
   for (long is=0; is<num_supp; is++)
   {
      double reaction;
      CComPtr<IResult3D> react_result;
      hr = react_results->get_Item(is, &react_result);
      hr = react_result->get_Y(&reaction);

      os<<std::setw(7)<<is+1<<m_Fv.Str(reaction)<<std::endl;
   }

   os<<L" "<<std::endl;
}

void ModelReporter::ReportVehicularResponse(std::wostringstream& os)
{
   os<< L"INDIVIDUAL VEHICULAR LOAD RESPONSES"<<std::endl;
   os<< L"==================================="<<std::endl;
   os<<L"  Live load responses are reported for a full lane and for "<<(m_NumStages==1? L"Stage I only":L"Stage II only")<<std::endl<<L" "<<std::endl;

   os <<std::endl<<L" Design Truck + Lane"<<std::endl
                 <<L" -------------------"<<std::endl;

   ReportSpecificVehicle(lltDesign, dvoDesignTruck, os);

   os <<std::endl<<L" Design Tandem + Lane"<<std::endl
                 <<L" --------------------"<<std::endl;

   ReportSpecificVehicle(lltDesign, dvoDesignTandem, os);

   os <<std::endl<<L" Dual Truck/Train + Lane"<<std::endl
                 <<L" -----------------------"<<std::endl;

   ReportSpecificVehicle(lltDesign, dvoDualTruckTrain, os);

   if (m_pLEEngine->m_IncludeLowBoy!=VARIANT_FALSE)
   {
      os <<std::endl<<L" Optional Low Boy + Lane"<<std::endl
                    <<L" -----------------------"<<std::endl;

      ReportSpecificVehicle(lltDesign, dvoOptionalLowBoy, os);
   }
}

void ModelReporter::ReportSpecificVehicle(LiveLoadModelType llType, long vehIdx, std::wostringstream& os)
{
   CComBSTR stage((m_NumStages==1?"Stage 1":"Stage 2"));

   os<<L" "<<std::endl;
   os<<L"                        Min        Max        Min        Max        Min        Max"<<std::endl;
   os<<L"          Location     Moment     Moment     Shear      Shear   Deflection Deflection"<<std::endl;
   if (m_UnitType==qcmtUS)
   {
   os<<L"Span POI    (ft)     (kip-ft)    (kip-ft)    (kip)       (kip)     (ft)       (ft)"   <<std::endl;
   }
   else
   {
   os<<L"Span POI     (m)      (kN-m)      (kN-m)     (kN)        (kN)      (m)        (m)"   <<std::endl;
   }
   os<<L"---- --- --------- ---------- ---------- ---------- ---------- ---------- ----------"<<std::endl;

   // compute member responses
   DistributionFactorType dft = dftNone;

   CComPtr<IEnvelopedVehicularResponse> response;
   hr = m_TheEngine->get_EnvelopedVehicularResponse(&response);

   CComPtr<ILiveLoadModelSectionResults> max_moments, min_moments, max_shears, min_shears, max_defls, min_defls;
   hr=response->ComputeForces(m_PoiIds, stage, llType, vehIdx, roMember, 
                              fetMz, optMinimize, vlcDefault, VARIANT_TRUE, dft,
                              VARIANT_FALSE, &min_moments);

   hr=response->ComputeForces(m_PoiIds, stage, llType, vehIdx, roMember, 
                              fetMz, optMaximize, vlcDefault, VARIANT_TRUE, dft,
                              VARIANT_FALSE, &max_moments);
      
   hr=response->ComputeForces(m_PoiIds, stage, llType, vehIdx, roMember, 
                              fetFy, optMinimize, vlcDefault, VARIANT_TRUE, dft,
                              VARIANT_FALSE, &min_shears);

   hr=response->ComputeForces(m_PoiIds, stage, llType, vehIdx, roMember, 
                              fetFy, optMaximize, vlcDefault, VARIANT_TRUE, dft,
                              VARIANT_FALSE, &max_shears);


   hr=response->ComputeDeflections(m_PoiIds, stage, llType, vehIdx, 
                              fetFy, optMinimize, vlcDefault, VARIANT_TRUE, dft,
                              VARIANT_FALSE, &min_defls);

   hr=response->ComputeDeflections(m_PoiIds, stage, llType, vehIdx, 
                              fetFy, optMaximize, vlcDefault, VARIANT_TRUE, dft,
                              VARIANT_FALSE, &max_defls);


   long num_pois;
   hr = m_PoiIds->get_Count(&num_pois);
   for (long ip=0; ip<num_pois; ip++)
   {
      long poi_id;
      hr = m_PoiIds->get_Item(ip, &poi_id);
      long span, tenth;
      GetSpanTenth(poi_id, &span, &tenth);

      double poi_loc;
      hr = m_PoiLocs->get_Item(ip,&poi_loc);

      double min_moment, max_moment, min_shear, max_shear, min_defl, max_defl, bogus;
      hr = min_moments->GetResult(ip, &min_moment, NULL, &bogus, NULL);
      hr = max_moments->GetResult(ip, &max_moment, NULL, &bogus, NULL);
      hr = min_shears->GetResult(ip, &min_shear, NULL, &bogus, NULL);
      hr = max_shears->GetResult(ip, &max_shear, NULL, &bogus, NULL);
      hr = min_defls->GetResult(ip, &min_defl, NULL, &bogus, NULL);
      hr = max_defls->GetResult(ip, &max_defl, NULL, &bogus, NULL);
      
      os<<std::setw(4)<<span<<L" "<<std::setw(3)<<tenth<<m_FLen.Str(poi_loc)
        <<m_Fm.Str(min_moment)<<m_Fm.Str(max_moment)
        <<m_Fv.Str(min_shear)<<m_Fv.Str(max_shear)
        <<m_Fd.Str(min_defl)<<m_Fd.Str(max_defl)<<std::endl;
   }

   // reactions
   CComPtr<ILiveLoadModelResults> max_reacts, min_reacts;

   hr=response->ComputeReactions(m_SupportIds, stage, llType, vehIdx, 
                              fetFy, optMinimize, vlcDefault, VARIANT_TRUE, dft,
                              VARIANT_FALSE, &min_reacts);

   hr=response->ComputeReactions(m_SupportIds, stage, llType, vehIdx, 
                              fetFy, optMaximize, vlcDefault, VARIANT_TRUE, dft,
                              VARIANT_FALSE, &max_reacts);

   os<<L" "<<std::endl;

   os<<L"            Min        Max"<<std::endl;
   os<<L"         Reaction   Reaction"<<std::endl;
   if (m_UnitType==qcmtUS)
   {
   os<<L"Support   (kip)       (kip)"   <<std::endl;
   }
   else
   {
   os<<L"Support    (kN)       (kN)"   <<std::endl;
   }
   os<<L"------- ---------- ----------"<<std::endl;

   long num_supp;
   hr = m_SupportIds->get_Count(&num_supp);
   for (long is=0; is<num_supp; is++)
   {
      double min_react, max_react;
      hr = min_reacts->GetResult(is, &min_react, NULL);
      hr = max_reacts->GetResult(is, &max_react, NULL);

      os<<std::setw(7)<<is+1<<m_Fv.Str(min_react)<<m_Fv.Str(max_react)<<std::endl;
   }

   os<<L" "<<std::endl;
   os<<L" "<<std::endl;
}


void ModelReporter::ReportLlmResponse(std::wostringstream& os)
{
   os<<L" "<<std::endl;
   os<< L"COMBINED VEHICULAR LOAD RESPONSES"<<std::endl;
   os<< L"================================="<<std::endl;
   os<<L"  Live load responses are reported a full lane and for "<<(m_NumStages==1? L"Stage I only":L"Stage II only")<<std::endl<<L" "<<std::endl;

   os <<std::endl<<L" LRFD HL-93 Design Vehicular Live Load"<<std::endl
                 <<L" -------------------------------------"<<std::endl;

   ReportSpecificLlm(lltDesign, os);

   if (m_pLEEngine->m_DoFatigue!=VARIANT_FALSE)
   {
      os <<std::endl<<L" Fatigue Truck"<<std::endl
                    <<L" -------------"<<std::endl;

      ReportSpecificLlm(lltFatigue, os);
   }
}

void ModelReporter::ReportSpecificLlm(LiveLoadModelType llType, std::wostringstream& os)
{
   CComBSTR stage((m_NumStages==1?"Stage 1":"Stage 2"));

   os<<L" "<<std::endl;
   os<<L"                        Min        Max        Min        Max        Min        Max"<<std::endl;
   os<<L"          Location     Moment     Moment     Shear      Shear   Deflection Deflection"<<std::endl;
   if (m_UnitType==qcmtUS)
   {
   os<<L"Span POI    (ft)     (kip-ft)    (kip-ft)    (kip)       (kip)     (ft)       (ft)"   <<std::endl;
   }
   else
   {
   os<<L"Span POI     (m)      (kN-m)      (kN-m)     (kN)        (kN)      (m)        (m)"   <<std::endl;
   }
   os<<L"---- --- --------- ---------- ---------- ---------- ---------- ---------- ----------"<<std::endl;

   // compute member responses
   DistributionFactorType dft = dftNone;


   CComPtr<ILiveLoadModelResponse> response;
   hr = m_TheEngine->get_LiveLoadModelResponse(&response);

   CComPtr<ILiveLoadModelSectionResults> max_moments, min_moments, max_shears, min_shears, max_defls, min_defls;
   hr=response->ComputeForces(m_PoiIds, stage, llType, roMember, 
                              fetMz, optMinimize, vlcDefault, VARIANT_TRUE, dft,
                              VARIANT_FALSE, &min_moments);

   hr=response->ComputeForces(m_PoiIds, stage, llType, roMember, 
                              fetMz, optMaximize, vlcDefault, VARIANT_TRUE, dft,
                              VARIANT_FALSE, &max_moments);
      
   hr=response->ComputeForces(m_PoiIds, stage, llType, roMember, 
                              fetFy, optMinimize, vlcDefault, VARIANT_TRUE, dft,
                              VARIANT_FALSE, &min_shears);

   hr=response->ComputeForces(m_PoiIds, stage, llType, roMember, 
                              fetFy, optMaximize, vlcDefault, VARIANT_TRUE, dft,
                              VARIANT_FALSE, &max_shears);


   hr=response->ComputeDeflections(m_PoiIds, stage, llType,
                              fetFy, optMinimize, vlcDefault, VARIANT_TRUE, dft,
                              VARIANT_FALSE, &min_defls);

   hr=response->ComputeDeflections(m_PoiIds, stage, llType,
                              fetFy, optMaximize, vlcDefault, VARIANT_TRUE, dft,
                              VARIANT_FALSE, &max_defls);


   long num_pois;
   hr = m_PoiIds->get_Count(&num_pois);
   for (long ip=0; ip<num_pois; ip++)
   {
      long poi_id;
      hr = m_PoiIds->get_Item(ip, &poi_id);
      long span, tenth;
      GetSpanTenth(poi_id, &span, &tenth);

      double poi_loc;
      hr = m_PoiLocs->get_Item(ip,&poi_loc);

      double min_moment, max_moment, min_shear, max_shear, min_defl, max_defl, bogus;
      hr = min_moments->GetResult(ip, &min_moment, NULL, &bogus, NULL);
      hr = max_moments->GetResult(ip, &max_moment, NULL, &bogus, NULL);
      hr = min_shears->GetResult(ip, &min_shear, NULL, &bogus, NULL);
      hr = max_shears->GetResult(ip, &max_shear, NULL, &bogus, NULL);
      hr = min_defls->GetResult(ip, &min_defl, NULL, &bogus, NULL);
      hr = max_defls->GetResult(ip, &max_defl, NULL, &bogus, NULL);
      
      os<<std::setw(4)<<span<<L" "<<std::setw(3)<<tenth<<m_FLen.Str(poi_loc)
        <<m_Fm.Str(min_moment)<<m_Fm.Str(max_moment)
        <<m_Fv.Str(min_shear)<<m_Fv.Str(max_shear)
        <<m_Fd.Str(min_defl)<<m_Fd.Str(max_defl)<<std::endl;

   }

   // reactions
   CComPtr<ILiveLoadModelResults> max_reacts, min_reacts;

   hr=response->ComputeReactions(m_SupportIds, stage, llType,
                              fetFy, optMinimize, vlcDefault, VARIANT_TRUE, dft,
                              VARIANT_FALSE, &min_reacts);

   hr=response->ComputeReactions(m_SupportIds, stage, llType,
                              fetFy, optMaximize, vlcDefault, VARIANT_TRUE, dft,
                              VARIANT_FALSE, &max_reacts);

   os<<L" "<<std::endl;

   os<<L"            Min        Max"<<std::endl;
   os<<L"         Reaction   Reaction"<<std::endl;
   if (m_UnitType==qcmtUS)
   {
   os<<L"Support   (kip)       (kip)"   <<std::endl;
   }
   else
   {
   os<<L"Support    (kN)       (kN)"   <<std::endl;
   }
   os<<L"------- ---------- ----------"<<std::endl;

   long num_supp;
   hr = m_SupportIds->get_Count(&num_supp);
   for (long is=0; is<num_supp; is++)
   {
      double min_react, max_react;
      hr = min_reacts->GetResult(is, &min_react, NULL);
      hr = max_reacts->GetResult(is, &max_react, NULL);

      os<<std::setw(7)<<is+1<<m_Fv.Str(min_react)<<m_Fv.Str(max_react)<<std::endl;
   }

   os<<L" "<<std::endl;
   os<<L" "<<std::endl;
}



void ModelReporter::ReportCombinedResponse(ILoadCombinationResponse* response, std::wostringstream& os)
{
   os<<L" "<<std::endl;
   os<< L"LOAD COMBINATION RESPONSES"<<std::endl;
   os<< L"=========================="<<std::endl;
   os<<L" "<<std::endl;

   // stage 1 - if two stage
   if (m_NumStages>1)
   {
      if (m_pLEEngine->m_DoServiceI != VARIANT_FALSE)
      {
         os <<std::endl<<L" Service I - Stage I"<<std::endl
                       <<L" -------------------"<<std::endl;

         ReportSpecificCombo(response, CComBSTR("SERVICE-I"), CComBSTR("Stage 1"), os);
      }

      if (m_pLEEngine->m_DoStrengthI != VARIANT_FALSE)
      {
         os <<std::endl<<L" Strength I - Stage I"<<std::endl
                       <<L" -------------------"<<std::endl;

         ReportSpecificCombo(response, CComBSTR("STRENGTH-I"), CComBSTR("Stage 1"), os);
      }
   }

   // final stage
   std::wstring stage((m_NumStages==1?L"Stage 1":L"Stage 2"));
   CComBSTR bsstage(  (m_NumStages==1?"Stage 1":"Stage 2"));

   if (m_pLEEngine->m_DoServiceI != VARIANT_FALSE)
   {
      os <<std::endl<<L" Service I - "<<stage<<std::endl
                    <<L" -------------------"<<std::endl;

      ReportSpecificCombo(response, CComBSTR("SERVICE-I"), bsstage, os);
   }

   if (m_pLEEngine->m_DoServiceII != VARIANT_FALSE)
   {
      os <<std::endl<<L" Service II - "<<stage<<std::endl
                    <<L" --------------------"<<std::endl;

      ReportSpecificCombo(response, CComBSTR("SERVICE-II"), bsstage, os);
   }

   if (m_pLEEngine->m_DoServiceIII != VARIANT_FALSE)
   {
      os <<std::endl<<L" Service III - "<<stage<<std::endl
                    <<L" ---------------------"<<std::endl;

      ReportSpecificCombo(response, CComBSTR("SERVICE-III"), bsstage, os);
   }


   if (m_pLEEngine->m_DoStrengthI != VARIANT_FALSE)
   {
      os <<std::endl<<L" Strength I -"<<stage<<std::endl
                    <<L" -------------------"<<std::endl;

      ReportSpecificCombo(response, CComBSTR("STRENGTH-I"), bsstage, os);
   }

   if (m_pLEEngine->m_DoFatigue != VARIANT_FALSE)
   {
      os <<std::endl<<L" Fatigue -"<<stage<<std::endl
                    <<L" ----------------"<<std::endl;

      ReportSpecificCombo(response, CComBSTR("FATIGUE"), bsstage, os);
   }
}

void ModelReporter::ReportSpecificCombo(ILoadCombinationResponse* response, BSTR loadCombo, BSTR stage, std::wostringstream& os)
{

   os<<L" "<<std::endl;
   os<<L"                        Min        Max        Min        Max        Min        Max"<<std::endl;
   os<<L"          Location     Moment     Moment     Shear      Shear   Deflection Deflection"<<std::endl;
   if (m_UnitType==qcmtUS)
   {
   os<<L"Span POI    (ft)     (kip-ft)    (kip-ft)    (kip)       (kip)     (ft)       (ft)"   <<std::endl;
   }
   else
   {
   os<<L"Span POI     (m)      (kN-m)      (kN-m)     (kN)        (kN)      (m)        (m)"   <<std::endl;
   }
   os<<L"---- --- --------- ---------- ---------- ---------- ---------- ---------- ----------"<<std::endl;

   // compute member responses

   CComPtr<ILoadCombinationSectionResults> max_moments, min_moments, max_shears, min_shears, max_defls, min_defls;
   hr=response->ComputeForces(loadCombo, m_PoiIds, stage, roMember, rsCumulative,
                              fetMz, optMinimize, VARIANT_TRUE,
                              VARIANT_FALSE, &min_moments);

   hr=response->ComputeForces(loadCombo, m_PoiIds, stage, roMember, rsCumulative,
                              fetMz, optMaximize, VARIANT_TRUE,
                              VARIANT_FALSE, &max_moments);

   hr=response->ComputeForces(loadCombo, m_PoiIds, stage, roMember, rsCumulative,
                              fetFy, optMinimize, VARIANT_TRUE,
                              VARIANT_FALSE, &min_shears);

   hr=response->ComputeForces(loadCombo, m_PoiIds, stage, roMember, rsCumulative,
                              fetFy, optMaximize, VARIANT_TRUE,
                              VARIANT_FALSE, &max_shears);

   hr=response->ComputeDeflections(loadCombo, m_PoiIds, stage, rsCumulative,
                              fetFy, optMinimize, VARIANT_TRUE,
                              VARIANT_FALSE, &min_defls);

   hr=response->ComputeDeflections(loadCombo, m_PoiIds, stage, rsCumulative,
                              fetFy, optMaximize, VARIANT_TRUE,
                              VARIANT_FALSE, &max_defls);

   long num_pois;
   hr = m_PoiIds->get_Count(&num_pois);
   for (long ip=0; ip<num_pois; ip++)
   {
      long poi_id;
      hr = m_PoiIds->get_Item(ip, &poi_id);
      long span, tenth;
      GetSpanTenth(poi_id, &span, &tenth);

      double poi_loc;
      hr = m_PoiLocs->get_Item(ip,&poi_loc);

      double lft_min_moment, lft_max_moment, lft_min_shear, lft_max_shear, lft_min_defl, lft_max_defl;
      double rgt_min_moment, rgt_max_moment, rgt_min_shear, rgt_max_shear, rgt_min_defl, rgt_max_defl;
      hr = min_moments->GetResult(ip, &lft_min_moment, NULL, &rgt_min_moment, NULL);
      hr = max_moments->GetResult(ip, &lft_max_moment, NULL, &rgt_max_moment, NULL);
      hr = min_shears->GetResult(ip,  &lft_min_shear,  NULL, &rgt_min_shear,  NULL);
      hr = max_shears->GetResult(ip,  &lft_max_shear,  NULL, &rgt_max_shear,  NULL);
      hr = min_defls->GetResult(ip,   &lft_min_defl,   NULL, &rgt_min_defl,   NULL);
      hr = max_defls->GetResult(ip,   &lft_max_defl,   NULL, &rgt_max_defl,   NULL);
      
      // left result
      os<<std::setw(4)<<span<<L" "<<std::setw(3)<<tenth<<m_FLen.Str(poi_loc)
        <<m_Fm.Str(lft_min_moment)<<m_Fm.Str(lft_max_moment)
        <<m_Fv.Str(lft_min_shear)<<m_Fv.Str(lft_max_shear)
        <<m_Fd.Str(lft_min_defl)<<m_Fd.Str(lft_max_defl)<<std::endl;

      // only print right result if needed
      if (tenth!=0 && tenth!=10 &&
          (!IsEqual(lft_max_moment, -rgt_max_moment) || !IsEqual(lft_max_shear, -rgt_max_shear) ||
           !IsEqual(lft_min_moment, -rgt_min_moment) || !IsEqual(lft_min_shear, -rgt_min_shear) ))
      {
         os<<std::setw(4)<<span<<L" "<<std::setw(3)<<tenth<<m_FLen.Str(poi_loc)
           <<m_Fm.Str(-rgt_min_moment)<<m_Fm.Str(-rgt_max_moment)
           <<m_Fv.Str(-rgt_min_shear)<<m_Fv.Str(-rgt_max_shear)<<std::endl;
      }
   }

/*
   // reactions
   CComPtr<ILoadCombinationResults> max_reacts, min_reacts;

   hr=response->ComputeReactions(loadCombo, m_SupportIds, stage, rsCumulative,
                                 fetFy, optMinimize, VARIANT_TRUE, VARIANT_FALSE, &min_reacts);

   hr=response->ComputeReactions(loadCombo, m_SupportIds, stage, rsCumulative,
                                 fetFy, optMaximize, VARIANT_TRUE, VARIANT_FALSE, &max_reacts);

   os<<L" "<<std::endl;

   os<<L"            Min        Max"<<std::endl;
   os<<L"         Reaction   Reaction"<<std::endl;
   if (m_UnitType==qcmtUS)
   {
   os<<L"Support   (kip)       (kip)"   <<std::endl;
   }
   else
   {
   os<<L"Support    (kN)       (kN)"   <<std::endl;
   }
   os<<L"------- ---------- ----------"<<std::endl;

   long num_supp = m_SupportIds.Size();
   for (long is=0; is<num_supp; is++)
   {
      double min_react, max_react;
      hr = min_reacts->GetResult(is, &min_react, NULL);
      hr = max_reacts->GetResult(is, &max_react, NULL);

      os<<std::setw(7)<<is+1<<L" "<<m_Fv.Str(min_react)<<L" "<<m_Fv.Str(max_react)<<std::endl;
   }
*/
   os<<L" "<<std::endl;
   os<<L" "<<std::endl;
}


void ModelReporter::ReportContraflexureLocations(std::wostringstream& os)
{
   std::wstring stage((m_NumStages==1?L"Stage 1":L"Stage 2"));
   CComBSTR bsstage(  (m_NumStages==1?"Stage 1":"Stage 2"));

   os<<L" "<<std::endl;
   os<<L"Locations of Points of Contraflexure Along Superstructure"<<std::endl;
   os<<L"--------------------------------------------------------- "<<std::endl;
   os<<L" Computed for "<<stage<<std::endl;
   os<<L" "<<std::endl;
   os<<L"     Location "<<std::endl;
   if (m_UnitType==qcmtUS)
   {
   os<<L"       (ft)"   <<std::endl;
   }
   else
   {
   os<<L"       (m)"   <<std::endl;
   }
   os<<L" --- ---------"<<std::endl;

   CComPtr<ILoadGroupResponse> response;
   hr = m_TheEngine->get_LoadGroupResponse(&response);

   CComQIPtr<IContraflexureResponse> igcf(response);
   ATLASSERT(igcf!=NULL);

   CComPtr<IDblArray> locs;
   hr=igcf->ComputeContraflexureLocations(bsstage, &locs);

   long cnt;
   hr =locs->get_Count(&cnt);
   for(long i=0; i<cnt; i++)
   {
      double loc;
      hr = locs->get_Item(i,&loc);
      os<<std::setw(4)<<i+1<<m_FLen.Str(loc)<<std::endl;
   }

   os<<L" "<<std::endl;
}



void ModelReporter::GetLoadCaseControl(bool& isS1dc, bool& isS1dw, bool& isS2dc, bool& isS2dw)
{
   isS1dc=false;
   isS1dw=false;
   isS2dc=false;
   isS2dw=false;

   // loop over each of our spans and see if we have active loads for our cases
   for (long ns=0; ns<m_NumSpans; ns++)
   {
      CComPtr<IQConLESpan> lespan;
      hr=m_pLEEngine->m_Spans->get_Item(ns, &lespan);

      double stageIDC, stageIDW, stageIIDC, stageIIDW;
      hr=lespan->get_StageIDC(&stageIDC);
      hr=lespan->get_StageIDW(&stageIDW);
      hr=lespan->get_StageIIDC(&stageIIDC);
      hr=lespan->get_StageIIDW(&stageIIDW);

      if (stageIDC!=0.0)
         isS1dc=true;

      if (stageIDW!=0.0)
         isS1dw=true;

      if (stageIIDC!=0.0)
         isS2dc=true;

      if (stageIIDW!=0.0)
         isS2dw=true;
   }

   // cant have a load if the stage doesn't exist
   if (m_NumStages==1)
   {
      isS2dc=false;
      isS2dw=false;
   }
}