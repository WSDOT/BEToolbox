// ModelReporter.h: interface for the ModelReporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODELREPORTER_H__10667864_1FBF_4366_841E_E57069C3CD24__INCLUDED_)
#define AFX_MODELREPORTER_H__10667864_1FBF_4366_841E_E57069C3CD24__INCLUDED_

#include <sstream>

#include <LBAM.h>
#include <LBAMAnalysis.h>
#include <LBAMLiveLoader.h>
#include <LBAMLoadCombiner.h>
#include <LBAMUtility.h>
#include <LBAMAnalysisUtility.h>
#include <WBFLUnitServer.h>
#include <WBFLTools.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CQConLEEngine;

class ModelReporter  
{
public:
	ModelReporter(ILBAMModel* theModel,ILBAMAnalysisEngine* theEngine,  CQConLEEngine* leEngine, bool doDetailed);
	virtual ~ModelReporter()
   {;}

   void CreateReport(std::wostringstream& os);
   void ReportCombinedResponse(ILoadCombinationResponse* response, std::wostringstream& os);

private:
   void ReportInputData(std::wostringstream& os);
   void ReportDistributionFactors(std::wostringstream& os);
   void ReportLoadCaseResponse(std::wostringstream& os);
   void ReportSpecificLoadCase(BSTR loadCase, BSTR stage, ResultsSummationType summ, std::wostringstream& os);
   void ReportVehicularResponse(std::wostringstream& os);
   void ReportSpecificVehicle(LiveLoadModelType llType, long vehIdx, std::wostringstream& os);
   void ReportLlmResponse(std::wostringstream& os);
   void ReportSpecificLlm(LiveLoadModelType llType, std::wostringstream& os);
   void ReportSpecificCombo(ILoadCombinationResponse* response, BSTR loadCombo, BSTR stage, std::wostringstream& os);
   void ReportContraflexureLocations(std::wostringstream& os);

   void GetLoadCaseControl(bool& isS1dc, bool& isS1dw, bool& isS2dc, bool& isS2dw);

private:
	ModelReporter();

   CComPtr<ILBAMModel>            m_TheModel;
   CQConLEEngine*                 m_pLEEngine;
   CComQIPtr<ILBAMAnalysisEngine> m_TheEngine;

   long m_NumStages;
   long m_NumSpans;
   bool m_DoDetailedReport;
   CComPtr<ILngArray> m_PoiIds;
   CComPtr<IDblArray> m_PoiLocs;
   CComPtr<ILngArray> m_SupportIds;
   QCLEUnitType  m_UnitType;

   // progress monitoring
   CComPtr<IProgressMonitorWindow>           m_pProgressMonitorWindow;
   long                                      m_LoadGroupCookie;
   long                                      m_LiveLoadCookie;
   long                                      m_LoadCombinationCookie;


   // formatting objects
   class SimpleUnitFormatter
   {
   public:
      std::wstring Str(double val)
      {
         ATLASSERT(m_Formatter!=NULL);

         CComBSTR bstr;
         HRESULT hr=m_Formatter->Format(val,NULL,&bstr);


         return std::wstring(bstr.m_str);
      }

      HRESULT Init(long width, long precision)
      {
         HRESULT hr=m_Formatter.CoCreateInstance(CLSID_DisplayUnitFormatter);
         if(FAILED(hr))return hr;

         hr=m_Formatter->FormatSpecifiers(width, precision,tjRight,nftFixed,1.0e-7);
         if(FAILED(hr))return hr;

         return S_OK;
      }
   private:
      CComPtr<IDisplayUnitFormatter> m_Formatter;
   };

   SimpleUnitFormatter m_FLen;
   SimpleUnitFormatter m_FEI;
   SimpleUnitFormatter m_Fg;
   SimpleUnitFormatter m_Fw;
   SimpleUnitFormatter m_Fm;
   SimpleUnitFormatter m_Fv;
   SimpleUnitFormatter m_Fd;

};

// class to make dealing with hresults easier
class CHRTextHucker
{
public:
   CHRTextHucker():
   m_Result(S_OK),
   m_Msg("An unknown error occurred while calling the LBAM object model")
   {};

   CHRTextHucker(LPCSTR str):
   m_Result(S_OK),
   m_Msg(str)
   {};


   // Throw if assigned to a FAILED HRESULT.
   operator=(HRESULT hr)
   {
      m_Result = hr;
      if (FAILED(hr))
      {
         ATLASSERT(0);
         throw m_Msg;
      }
      else if (hr==S_FALSE)
      {
         throw std::string("The analysis was cancelled. \n\n Click on the Input frame and back to this frame to restart the analysis");
      }
      else
      {
         return hr;
      }
   }

   // Hand off most recent hr if asked for it
   operator HRESULT()
   {
      return m_Result;
   }

private:
   std::string m_Msg;
   HRESULT m_Result; 
};


#endif // !defined(AFX_MODELREPORTER_H__10667864_1FBF_4366_841E_E57069C3CD24__INCLUDED_)
