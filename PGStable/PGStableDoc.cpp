///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright � 1999-2025  Washington State Department of Transportation
//                        Bridge and Structures Office
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the Alternate Route Open Source License as 
// published by the Washington State Department of Transportation, 
// Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful, but 
// distribution is AS IS, WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See 
// the Alternate Route Open Source License for more details.
//
// You should have received a copy of the Alternate Route Open Source 
// License along with this program; if not, write to the Washington 
// State Department of Transportation, Bridge and Structures Office, 
// P.O. Box  47340, Olympia, WA 98503, USA or e-mail 
// Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\resource.h"
#include "PGStableDoc.h"
#include "..\BEToolboxStatusBar.h"

#include <EAF\EAFUtilities.h>
#include <EAF\EAFApp.h>
#include <MFCTools\AutoRegistry.h>
#include <System\System.h>

#include <IFace\BeamFactory.h>
#include <Plugins\BeamFamilyCLSID.h>

#include <memory>

#include "PGStableTitlePageBuilder.h"
#include "PGStableLiftingSummaryChapterBuilder.h"
#include "PGStableLiftingDetailsChapterBuilder.h"
#include "PGStableHaulingSummaryChapterBuilder.h"
#include "PGStableHaulingDetailsChapterBuilder.h"
#include "PGStableOneEndSeatedSummaryChapterBuilder.h"
#include "PGStableOneEndSeatedDetailsChapterBuilder.h"

#include <psgLib/LiftingCriteria.h>
#include <psgLib/HaulingCriteria.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CPGStableDoc

IMPLEMENT_DYNCREATE(CPGStableDoc, CBEToolboxDoc)

CPGStableDoc::CPGStableDoc() : CBEToolboxDoc()
{
   std::shared_ptr<WBFL::Reporting::TitlePageBuilder> pLiftingTitlePageBuilder(std::make_shared<CPGStableTitlePageBuilder>());

   std::shared_ptr<WBFL::Reporting::ReportBuilder> pLiftingReportBuilder(std::make_shared<WBFL::Reporting::ReportBuilder>(_T("Lifting")));
   pLiftingReportBuilder->AddTitlePageBuilder(pLiftingTitlePageBuilder);
   pLiftingReportBuilder->AddChapterBuilder(std::dynamic_pointer_cast<WBFL::Reporting::ChapterBuilder>(std::make_shared<CPGStableLiftingSummaryChapterBuilder>(this)));
   pLiftingReportBuilder->AddChapterBuilder(std::dynamic_pointer_cast<WBFL::Reporting::ChapterBuilder>(std::make_shared<CPGStableLiftingDetailsChapterBuilder>(this)));
   GetReportManager()->AddReportBuilder(pLiftingReportBuilder);

   std::shared_ptr<WBFL::Reporting::TitlePageBuilder> pHaulingTitlePageBuilder(std::make_shared<CPGStableTitlePageBuilder>());

   std::shared_ptr<WBFL::Reporting::ReportBuilder> pHaulingReportBuilder(std::make_shared<WBFL::Reporting::ReportBuilder>(_T("Hauling")));
   pHaulingReportBuilder->AddTitlePageBuilder(pHaulingTitlePageBuilder);
   pHaulingReportBuilder->AddChapterBuilder(std::dynamic_pointer_cast<WBFL::Reporting::ChapterBuilder>(std::make_shared<CPGStableHaulingSummaryChapterBuilder>(this)));
   pHaulingReportBuilder->AddChapterBuilder(std::dynamic_pointer_cast<WBFL::Reporting::ChapterBuilder>(std::make_shared<CPGStableHaulingDetailsChapterBuilder>(this)));

   GetReportManager()->AddReportBuilder(pHaulingReportBuilder);

   std::shared_ptr<WBFL::Reporting::TitlePageBuilder> pOneEndSeatedTitlePageBuilder(std::make_shared<CPGStableTitlePageBuilder>());

   std::shared_ptr<WBFL::Reporting::ReportBuilder> pOneEndSeatedReportBuilder(std::make_shared<WBFL::Reporting::ReportBuilder>(_T("OneEndSeated")));
   pOneEndSeatedReportBuilder->AddTitlePageBuilder(pOneEndSeatedTitlePageBuilder);
   pOneEndSeatedReportBuilder->AddChapterBuilder(std::dynamic_pointer_cast<WBFL::Reporting::ChapterBuilder>(std::make_shared<CPGStableOneEndSeatedSummaryChapterBuilder>(this)));
   pOneEndSeatedReportBuilder->AddChapterBuilder(std::dynamic_pointer_cast<WBFL::Reporting::ChapterBuilder>(std::make_shared<CPGStableOneEndSeatedDetailsChapterBuilder>(this)));

   GetReportManager()->AddReportBuilder(pOneEndSeatedReportBuilder);

   m_strProjectCriteria = gs_strCriteria;
   m_strHaulTruck = gs_strHaulTruck;
   m_strOneEndSeatedHaulTruck = gs_strHaulTruck;
   m_strGirder = gs_strGirder;

   EnableUIHints(FALSE); // not using UIHints feature
}

CPGStableDoc::~CPGStableDoc()
{
}


BEGIN_MESSAGE_MAP(CPGStableDoc, CBEToolboxDoc)
   ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
END_MESSAGE_MAP()

void CPGStableDoc::OnHelpFinder()
{
   EAFHelp(EAFGetDocument()->GetDocumentationSetName(), IDH_PGSTABLE);
}

// CPGStableDoc diagnostics

#ifdef _DEBUG
void CPGStableDoc::AssertValid() const
{
	CBEToolboxDoc::AssertValid();
}

#ifndef _WIN32_WCE
void CPGStableDoc::Dump(CDumpContext& dc) const
{
	CBEToolboxDoc::Dump(dc);
}
#endif
#endif //_DEBUG

BOOL CPGStableDoc::Init()
{
   if ( !CBEToolboxDoc::Init() )
      return FALSE;

   LoadPGSLibrary();

   // get the common engineer and company name from the main application level
   // this should be updated in the future so that BridgeLink can just return the values
   CEAFApp* pApp = EAFGetApp();
   CAutoRegistry autoReg(_T("BridgeLink"),pApp);
   m_strEngineer = pApp->GetProfileString(_T("Options"),_T("EngineerName"), _T("Your Name"));
   m_strCompany  = pApp->GetProfileString(_T("Options"),_T("CompanyName"), _T("Your Company"));

   // we only load this information as default, but never update the common data
   // this data is stored in our file

   return TRUE;
}

void CPGStableDoc::LoadPGSLibrary()
{
   CAutoRegistry autoReg(_T("PGSuper"));

   CEAFApp* pApp = EAFGetApp();

   CString strMasterLibaryFile = pApp->GetProfileString(_T("Options"),_T("MasterLibraryCache2"));

   if (strMasterLibaryFile.IsEmpty())
   {
      AfxMessageBox(_T("Cannot ready PGSuper Configuration.\r\nProceeding without configuration information.\r\nUse the BridgeLink Configuration Wizard to update the software configuration."),MB_OK | MB_ICONEXCLAMATION);
      return; // there isn't a master library defined
   }

   CString strURL = pApp->GetProfileString(_T("Options"),_T("MasterLibraryURL2"));

   if (strURL.IsEmpty())
   {
      strURL = strMasterLibaryFile;
   }

   CString strPublisher = pApp->GetProfileString(_T("Options"), _T("Publisher2"));
   CString strServer = pApp->GetProfileString(_T("Options"), _T("CatalogServer2"));

   m_LibMgr.SetName( _T("PGSLibrary") );
   m_LibMgr.SetMasterLibraryInfo(strPublisher,strServer,strURL);

   CComBSTR bpath(strMasterLibaryFile);

   WBFL::System::FileStream ifile;
   if ( ifile.open(bpath) )
   {
      // try to load file
      try
      {
         // clear out library
         m_LibMgr.ClearAllEntries();

         WBFL::System::StructuredLoadXml load;
         load.BeginLoad( &ifile );

         // Problem : Library Editor application specific code is in the
         // master library file. We have to read it or get an error.
         load.BeginUnit(_T("PGSuperLibrary"));
         load.BeginUnit(_T("LIBRARY_EDITOR"));
         std::_tstring str;
         load.Property(_T("EDIT_UNITS"), &str);

         if ( !m_LibMgr.LoadMe( &load ) )
         {
            AfxMessageBox(_T("Failed to load the PGSuper Configuration information"),MB_OK | MB_ICONEXCLAMATION);
            return;
         }

         load.EndUnit(); //"LIBRARY_EDITOR"
         load.EndUnit(); //"PGSuperLibrary"
         load.EndLoad();

         // success!
         WATCH(_T("Master Library loaded successfully"));
      }
      catch(WBFL::System::XStructuredLoad& e )
      {
         CString strMsg;
         if ( e.GetReasonCode() == WBFL::System::XStructuredLoad::CantInitializeTheParser )
         {
            strMsg = _T("Failed to initialize the xml parser. This is an installation issue.");
         }
         else
         {
            ASSERT(0);
            strMsg = _T("An unknown error occurred while loading the master library.");
         }

         AfxMessageBox(strMsg,MB_OK | MB_ICONEXCLAMATION);
         return;
      }
      catch(...)
      {
         AfxMessageBox(_T("An unknown error occurred while loading the master library."),MB_OK | MB_ICONEXCLAMATION);
         return;
      }
   }
   else
   {
      CString strMsg;
      strMsg.Format(_T("Failed to open the master library file: %s"),strMasterLibaryFile);
      AfxMessageBox(strMsg,MB_OK | MB_ICONEXCLAMATION);
      return;
   }

   // make all entries in master library read-only
   m_LibMgr.EnableEditingForAllEntries(false);
}

HRESULT CPGStableDoc::WriteTheDocument(IStructuredSave* pStrSave)
{
   UpdateAllViews(NULL, HINT_UPDATE_DATA);

   pStrSave->BeginUnit(_T("ProjectProperties"),1.0);
   pStrSave->put_Property(_T("Engineer"),CComVariant(m_strEngineer));
   pStrSave->put_Property(_T("Company"),CComVariant(m_strCompany));
   pStrSave->put_Property(_T("Job"),CComVariant(m_strJob));
   pStrSave->put_Property(_T("Comments"),CComVariant(m_strComments));
   pStrSave->EndUnit(); // ProjectProperties

   pStrSave->BeginUnit(_T("LibraryReferences"),3.0);
   if ( m_strProjectCriteria == gs_strCriteria )
   {
      pStrSave->put_Property(_T("ProjectCriteria"),CComVariant(_T("None")));
   }
   else
   {
      pStrSave->put_Property(_T("ProjectCriteria"),CComVariant(m_strProjectCriteria));
   }

   if ( m_strGirder == gs_strGirder )
   {
      pStrSave->put_Property(_T("Girder"),CComVariant(_T("None")));
   }
   else
   {
      pStrSave->put_Property(_T("Girder"),CComVariant(m_strGirder));
   }

   if ( m_strHaulTruck == gs_strHaulTruck )
   {
      pStrSave->put_Property(_T("HaulTruck"),CComVariant(_T("None")));
   }
   else
   {
      pStrSave->put_Property(_T("HaulTruck"),CComVariant(m_strHaulTruck));
   }

   // added in version 2 of LibraryReferences data block
   if (m_strOneEndSeatedHaulTruck == gs_strHaulTruck)
   {
      pStrSave->put_Property(_T("OneEndSeatedHaulTruck"), CComVariant(_T("None")));
   }
   else
   {
      pStrSave->put_Property(_T("OneEndSeatedHaulTruck"), CComVariant(m_strOneEndSeatedHaulTruck));
   }

   pStrSave->EndUnit(); // LibraryReferences
   return m_Model.Save(pStrSave);
}

HRESULT CPGStableDoc::LoadTheDocument(IStructuredLoad* pStrLoad)
{
   CHRException hr;
   try
   {
      CComVariant var;
      var.vt = VT_BSTR;
      hr = pStrLoad->BeginUnit(_T("ProjectProperties"));

      hr = pStrLoad->get_Property(_T("Engineer"),&var);
      m_strEngineer = var.bstrVal;

      hr = pStrLoad->get_Property(_T("Company"),&var);
      m_strCompany = var.bstrVal;

      hr = pStrLoad->get_Property(_T("Job"),&var);
      m_strJob = var.bstrVal;

      hr = pStrLoad->get_Property(_T("Comments"),&var);
      m_strComments = var.bstrVal;

      hr = pStrLoad->EndUnit(); // ProjectProperties

      hr = pStrLoad->BeginUnit(_T("LibraryReferences"));
      Float64 lib_ref_version;
      pStrLoad->get_Version(&lib_ref_version);

      var.vt = VT_BSTR;
      hr = pStrLoad->get_Property(_T("ProjectCriteria"),&var);
      if ( var.bstrVal == CComBSTR(_T("None")) )
      {
         m_strProjectCriteria = gs_strCriteria;
      }
      else
      {
         m_strProjectCriteria = var.bstrVal;
         const SpecLibraryEntry* pSpec = GetSpecLibraryEntry();
         if ( pSpec == nullptr )
         {
            m_strProjectCriteria = gs_strCriteria;
         }
      }

      hr = pStrLoad->get_Property(_T("Girder"),&var);
      if ( var.bstrVal == CComBSTR(_T("None")) )
      {
         m_strGirder = gs_strGirder;
      }
      else
      {
         m_strGirder = var.bstrVal;
         const GirderLibraryEntry* pGirderEntry = GetGirderLibraryEntry();
         if (pGirderEntry == nullptr || !IsPermittedGirderEntry(pGirderEntry))
         {
            m_strGirder = gs_strGirder;
         }
      }

      hr = pStrLoad->get_Property(_T("HaulTruck"),&var);
      if ( var.bstrVal == CComBSTR(_T("None")) )
      {
         m_strHaulTruck = gs_strHaulTruck;
      }
      else
      {
         m_strHaulTruck = var.bstrVal;
         const HaulTruckLibraryEntry* pTruck = GetHaulTruckLibraryEntry();
         if ( pTruck == nullptr )
         {
            m_strHaulTruck = gs_strHaulTruck;
         }
      }

      if (1 < lib_ref_version)
      {
         // added in version 2
         hr = pStrLoad->get_Property(_T("OneEndSeatedHaulTruck"), &var);
         if (var.bstrVal == CComBSTR(_T("None")))
         {
            m_strOneEndSeatedHaulTruck = gs_strHaulTruck;
         }
         else
         {
            m_strOneEndSeatedHaulTruck = var.bstrVal;
            const HaulTruckLibraryEntry* pTruck = GetOneEndSeatedHaulTruckLibraryEntry();
            if (pTruck == nullptr)
            {
               m_strOneEndSeatedHaulTruck = gs_strHaulTruck;
            }
         }
      }
      else
      {
         m_strOneEndSeatedHaulTruck = m_strHaulTruck;
      }

      hr = pStrLoad->EndUnit(); // LibraryReferences
   }
   catch(...)
   {
      THROW_LOAD(InvalidFileFormat,pStrLoad);
   }

   return m_Model.Load(pStrLoad);
}

CString CPGStableDoc::GetToolbarSectionName()
{
   return _T("PGStable");
}

void CPGStableDoc::LoadDocumentSettings()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   __super::LoadDocumentSettings();
}

void CPGStableDoc::SaveDocumentSettings()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   __super::SaveDocumentSettings();
}

CString CPGStableDoc::GetDocumentationRootLocation()
{
   CEAFApp* pApp = EAFGetApp();
   return pApp->GetDocumentationRootLocation();
}

UINT CPGStableDoc::GetToolbarID()
{
   return IDR_PGSTOOLBAR;
}

const CString& CPGStableDoc::GetCriteria() const
{
   return m_strProjectCriteria;
}

void CPGStableDoc::SetCriteria(LPCTSTR lpszCriteria)
{
   m_strProjectCriteria = lpszCriteria;

   if ( m_strProjectCriteria != gs_strCriteria )
   {
      // update input parameters to match library
      const SpecLibraryEntry* pSpec = GetSpecLibraryEntry();
      const auto& lifting_criteria = pSpec->GetLiftingCriteria();
      const auto& hauling_criteria = pSpec->GetHaulingCriteria();

      // lifting
      WBFL::Stability::LiftingStabilityProblem liftingProblem = GetLiftingStabilityProblem();
      liftingProblem.SetImpact(lifting_criteria.ImpactUp,lifting_criteria.ImpactDown);

      liftingProblem.SetSupportPlacementTolerance(lifting_criteria.LiftingLoopTolerance);
      liftingProblem.SetLiftAngle(lifting_criteria.MinCableInclination);
      liftingProblem.SetSweepTolerance(lifting_criteria.SweepTolerance);
      liftingProblem.SetSweepGrowth(0); // no sweep growth for initial lifting problem
      liftingProblem.SetWindLoading(lifting_criteria.WindLoadType,lifting_criteria.WindLoad);
      liftingProblem.SetYRollAxis(lifting_criteria.PickPointHeight);
      SetLiftingStabilityProblem(liftingProblem);

      CPGStableLiftingCriteria liftingCriteria = GetLiftingCriteria();
      liftingCriteria.MinFSf = lifting_criteria.FsFailure;
      liftingCriteria.MinFScr = lifting_criteria.FsCracking;
      liftingCriteria.CompressionCoefficient_GlobalStress = lifting_criteria.CompressionStressCoefficient_GlobalStress;
      liftingCriteria.CompressionCoefficient_PeakStress = lifting_criteria.CompressionStressCoefficient_PeakStress;

      auto* pLiftingTensionStressLimit = dynamic_cast<WBFL::Stability::CCLiftingTensionStressLimit*>(liftingCriteria.TensionStressLimit.get());

      pLiftingTensionStressLimit->TensionCoefficient = lifting_criteria.TensionStressLimitWithoutReinforcement.Coefficient;
      pLiftingTensionStressLimit->bMaxTension = lifting_criteria.TensionStressLimitWithoutReinforcement.bHasMaxValue;
      pLiftingTensionStressLimit->MaxTension = lifting_criteria.TensionStressLimitWithoutReinforcement.MaxValue;
      pLiftingTensionStressLimit->TensionCoefficientWithRebar = lifting_criteria.TensionStressLimitWithReinforcement.Coefficient;
      SetLiftingCriteria(liftingCriteria);


      Float64 Fc,FrCoefficient;
      bool bComputeEc;
      GetLiftingMaterials(&Fc,&bComputeEc,&FrCoefficient);
      SetLiftingMaterials(Fc, bComputeEc, lifting_criteria.ModulusOfRuptureCoefficient[pgsTypes::Normal]);

      // Hauling
      WBFL::Stability::HaulingStabilityProblem haulingProblem = GetHaulingStabilityProblem();
      haulingProblem.SetImpactUsage(hauling_criteria.WSDOT.ImpactUsage);
      haulingProblem.SetImpact(hauling_criteria.WSDOT.ImpactUp,hauling_criteria.WSDOT.ImpactDown);
      haulingProblem.SetSupportSlope(hauling_criteria.WSDOT.RoadwayCrownSlope);
      haulingProblem.SetSuperelevation(hauling_criteria.WSDOT.RoadwaySuperelevation);

      haulingProblem.SetSupportPlacementTolerance(hauling_criteria.WSDOT.SupportPlacementTolerance);
      haulingProblem.SetSweepTolerance(hauling_criteria.WSDOT.SweepTolerance);
      haulingProblem.SetSweepGrowth(hauling_criteria.WSDOT.SweepGrowth);
      haulingProblem.SetWindLoading(hauling_criteria.WSDOT.WindLoadType, hauling_criteria.WSDOT.WindLoad);
      haulingProblem.SetCentrifugalForceType(hauling_criteria.WSDOT.CentrifugalForceType);
      haulingProblem.SetVelocity(hauling_criteria.WSDOT.HaulingSpeed);
      haulingProblem.SetTurningRadius(hauling_criteria.WSDOT.TurningRadius);
      SetHaulingStabilityProblem(haulingProblem);

      CPGStableHaulingCriteria haulingCriteria = GetHaulingCriteria();
      haulingCriteria.MinFSf = hauling_criteria.WSDOT.FsFailure;
      haulingCriteria.MinFScr = hauling_criteria.WSDOT.FsCracking;
      haulingCriteria.CompressionCoefficient_GlobalStress = hauling_criteria.WSDOT.CompressionStressCoefficient_GlobalStress;
      haulingCriteria.CompressionCoefficient_PeakStress = hauling_criteria.WSDOT.CompressionStressCoefficient_PeakStress;
      
      auto* pHaulingTensionStressLimit = dynamic_cast<WBFL::Stability::CCHaulingTensionStressLimit*>(haulingCriteria.TensionStressLimit.get());

      pHaulingTensionStressLimit->TensionCoefficient[+WBFL::Stability::HaulingSlope::CrownSlope] = hauling_criteria.WSDOT.TensionStressLimitWithoutReinforcement[+WBFL::Stability::HaulingSlope::CrownSlope].Coefficient;
      pHaulingTensionStressLimit->bMaxTension[+WBFL::Stability::HaulingSlope::CrownSlope] = hauling_criteria.WSDOT.TensionStressLimitWithoutReinforcement[+WBFL::Stability::HaulingSlope::CrownSlope].bHasMaxValue;
      pHaulingTensionStressLimit->MaxTension[+WBFL::Stability::HaulingSlope::CrownSlope] = hauling_criteria.WSDOT.TensionStressLimitWithoutReinforcement[+WBFL::Stability::HaulingSlope::CrownSlope].MaxValue;

      pHaulingTensionStressLimit->TensionCoefficientWithRebar[+WBFL::Stability::HaulingSlope::CrownSlope] = hauling_criteria.WSDOT.TensionStressLimitWithReinforcement[+WBFL::Stability::HaulingSlope::CrownSlope].Coefficient;

      pHaulingTensionStressLimit->TensionCoefficient[+WBFL::Stability::HaulingSlope::Superelevation] = hauling_criteria.WSDOT.TensionStressLimitWithoutReinforcement[+WBFL::Stability::HaulingSlope::Superelevation].Coefficient;
      pHaulingTensionStressLimit->bMaxTension[+WBFL::Stability::HaulingSlope::Superelevation] = hauling_criteria.WSDOT.TensionStressLimitWithoutReinforcement[+WBFL::Stability::HaulingSlope::Superelevation].bHasMaxValue;
      pHaulingTensionStressLimit->MaxTension[+WBFL::Stability::HaulingSlope::Superelevation] = hauling_criteria.WSDOT.TensionStressLimitWithoutReinforcement[+WBFL::Stability::HaulingSlope::Superelevation].MaxValue;


      pHaulingTensionStressLimit->TensionCoefficientWithRebar[+WBFL::Stability::HaulingSlope::Superelevation] = hauling_criteria.WSDOT.TensionStressLimitWithReinforcement[+WBFL::Stability::HaulingSlope::Superelevation].Coefficient;

      SetHaulingCriteria(haulingCriteria);

      GetHaulingMaterials(&Fc,&bComputeEc,&FrCoefficient);
      SetHaulingMaterials(Fc, bComputeEc, hauling_criteria.WSDOT.ModulusOfRuptureCoefficient[pgsTypes::Normal]);


      // One end seated - PGSuper/PGSplice project spec criteria doesn't have values for one end seated analysis so we will use some of the hauling values
      WBFL::Stability::OneEndSeatedStabilityProblem OneEndSeatedProblem = GetOneEndSeatedStabilityProblem();
      OneEndSeatedProblem.SetImpact(0.0, 0.0);
      OneEndSeatedProblem.SetSupportSlope(hauling_criteria.WSDOT.RoadwayCrownSlope);

      OneEndSeatedProblem.SetSupportPlacementTolerance(hauling_criteria.WSDOT.SupportPlacementTolerance);
      OneEndSeatedProblem.SetSweepTolerance(hauling_criteria.WSDOT.SweepTolerance);
      OneEndSeatedProblem.SetSweepGrowth(hauling_criteria.WSDOT.SweepGrowth);
      OneEndSeatedProblem.SetWindLoading(hauling_criteria.WSDOT.WindLoadType, hauling_criteria.WSDOT.WindLoad);
      SetOneEndSeatedStabilityProblem(OneEndSeatedProblem);

      CPGStableOneEndSeatedCriteria OneEndSeatedCriteria = GetOneEndSeatedCriteria();
      OneEndSeatedCriteria.MinFSf = hauling_criteria.WSDOT.FsFailure;
      OneEndSeatedCriteria.MinFScr = hauling_criteria.WSDOT.FsCracking;
      OneEndSeatedCriteria.CompressionCoefficient_GlobalStress = hauling_criteria.WSDOT.CompressionStressCoefficient_GlobalStress;
      OneEndSeatedCriteria.CompressionCoefficient_PeakStress = hauling_criteria.WSDOT.CompressionStressCoefficient_PeakStress;

      auto* pOneEndSeatedTensionStressLimit = dynamic_cast<WBFL::Stability::CCOneEndSeatedTensionStressLimit*>(OneEndSeatedCriteria.TensionStressLimit.get());

      pOneEndSeatedTensionStressLimit->TensionCoefficient = hauling_criteria.WSDOT.TensionStressLimitWithoutReinforcement[+WBFL::Stability::HaulingSlope::CrownSlope].Coefficient;
      pOneEndSeatedTensionStressLimit->bMaxTension = hauling_criteria.WSDOT.TensionStressLimitWithoutReinforcement[+WBFL::Stability::HaulingSlope::CrownSlope].bHasMaxValue;
      pOneEndSeatedTensionStressLimit->bMaxTension = hauling_criteria.WSDOT.TensionStressLimitWithoutReinforcement[+WBFL::Stability::HaulingSlope::CrownSlope].MaxValue;

      pOneEndSeatedTensionStressLimit->TensionCoefficientWithRebar = hauling_criteria.WSDOT.TensionStressLimitWithReinforcement[+WBFL::Stability::HaulingSlope::CrownSlope].Coefficient;
      SetOneEndSeatedCriteria(OneEndSeatedCriteria);

      GetOneEndSeatedMaterials(&Fc, &bComputeEc, &FrCoefficient);
      SetOneEndSeatedMaterials(Fc, bComputeEc, hauling_criteria.WSDOT.ModulusOfRuptureCoefficient[pgsTypes::Normal]);
   }

   SetModifiedFlag();
}

const CString& CPGStableDoc::GetHaulTruck() const
{
   return m_strHaulTruck;
}

void CPGStableDoc::SetHaulTruck(LPCTSTR lpszHaulTruck)
{
   if ( m_strHaulTruck != lpszHaulTruck )
   {
      m_strHaulTruck = lpszHaulTruck;
      SetModifiedFlag();
   }
}

const CString& CPGStableDoc::GetOneEndSeatedHaulTruck() const
{
   return m_strOneEndSeatedHaulTruck;
}

void CPGStableDoc::SetOneEndSeatedHaulTruck(LPCTSTR lpszHaulTruck)
{
   if (m_strOneEndSeatedHaulTruck != lpszHaulTruck)
   {
      m_strOneEndSeatedHaulTruck = lpszHaulTruck;
      SetModifiedFlag();
   }
}

const CString& CPGStableDoc::GetGirder() const
{
   return m_strGirder;
}

void CPGStableDoc::SetGirder(LPCTSTR lpszGirder)
{
   if ( m_strGirder != lpszGirder )
   {
      m_strGirder = lpszGirder;
      SetModifiedFlag();
   }
}


void CPGStableDoc::SetProjectProperties(const CString& strEngineer,const CString& strCompany,const CString& strJob,const CString& strComments)
{
   bool bModified = false;
   if ( m_strEngineer != strEngineer )
   {
      m_strEngineer = strEngineer;
      bModified = true;
   }

   if ( m_strCompany != strCompany )
   {
      m_strCompany = strCompany;
      bModified = true;
   }

   if ( m_strJob != strJob )
   {
      m_strJob = strJob;
      bModified = true;
   }

   if ( m_strComments != strComments )
   {
      m_strComments = strComments;
      bModified = true;
   }

   if ( bModified )
   {
      SetModifiedFlag();
   }
}

void CPGStableDoc::GetProjectProperties(CString* pstrEngineer,CString* pstrCompany,CString* pstrJob,CString* pstrComments) const
{
   *pstrEngineer = m_strEngineer;
   *pstrCompany = m_strCompany;
   *pstrJob = m_strJob;
   *pstrComments = m_strComments;
}

void CPGStableDoc::SetGirderType(GirderType girderType)
{
   if ( m_Model.SetGirderType(girderType) )
   {
      SetModifiedFlag();
   }
}

GirderType CPGStableDoc::GetGirderType() const
{
   return m_Model.GetGirderType();
}

const WBFL::Stability::Girder& CPGStableDoc::GetGirder(GirderType girderType) const
{
   return m_Model.GetGirder(girderType);
}

void CPGStableDoc::SetStressPointType(int stressPointType)
{
   if (m_Model.SetStressPointType(stressPointType))
   {
      SetModifiedFlag();
   }
}

int CPGStableDoc::GetStressPointType() const
{
   return m_Model.GetStressPointType();
}

void CPGStableDoc::SetGirder(GirderType girderType,const WBFL::Stability::Girder& girder)
{
   if ( m_Model.SetGirder(girderType,girder) )
   {
      SetModifiedFlag();
   }
}

const CPGStableStrands& CPGStableDoc::GetStrands(GirderType girderType,ModelType modelType) const
{
   return m_Model.GetStrands(girderType,modelType);
}

void CPGStableDoc::SetStrands(GirderType girderType,ModelType modelType,const CPGStableStrands& strands)
{
   if ( m_Model.SetStrands(girderType,modelType,strands) )
   {
      SetModifiedFlag();
   }
}

const WBFL::Stability::LiftingStabilityProblem& CPGStableDoc::GetLiftingStabilityProblem() const
{
   return m_Model.GetLiftingStabilityProblem();
}

void CPGStableDoc::SetLiftingStabilityProblem(const WBFL::Stability::LiftingStabilityProblem& stabilityProblem)
{
   if ( m_Model.SetLiftingStabilityProblem(stabilityProblem) )
   {
      SetModifiedFlag();
   }
}

const WBFL::Stability::HaulingStabilityProblem& CPGStableDoc::GetHaulingStabilityProblem() const
{
   return m_Model.GetHaulingStabilityProblem();
}

void CPGStableDoc::SetHaulingStabilityProblem(const WBFL::Stability::HaulingStabilityProblem& stabilityProblem)
{
   if ( m_Model.SetHaulingStabilityProblem(stabilityProblem) )
   {
      SetModifiedFlag();
   }
}

const WBFL::Stability::OneEndSeatedStabilityProblem& CPGStableDoc::GetOneEndSeatedStabilityProblem() const
{
   return m_Model.GetOneEndSeatedStabilityProblem();
}

void CPGStableDoc::SetOneEndSeatedStabilityProblem(const WBFL::Stability::OneEndSeatedStabilityProblem& stabilityProblem)
{
   if (m_Model.SetOneEndSeatedStabilityProblem(stabilityProblem))
   {
      SetModifiedFlag();
   }
}

const CPGStableLiftingCriteria& CPGStableDoc::GetLiftingCriteria() const
{
   return m_Model.GetLiftingCriteria();
}

void CPGStableDoc::SetLiftingCriteria(const CPGStableLiftingCriteria& criteria)
{
   if ( m_Model.SetLiftingCriteria(criteria) )
   {
      SetModifiedFlag();
   }
}

const CPGStableHaulingCriteria& CPGStableDoc::GetHaulingCriteria() const
{
   return m_Model.GetHaulingCriteria();
}

void CPGStableDoc::SetHaulingCriteria(const CPGStableHaulingCriteria& criteria)
{
   if ( m_Model.SetHaulingCriteria(criteria) )
   {
      SetModifiedFlag();
   }
}

const CPGStableOneEndSeatedCriteria& CPGStableDoc::GetOneEndSeatedCriteria() const
{
   return m_Model.GetOneEndSeatedCriteria();
}

void CPGStableDoc::SetOneEndSeatedCriteria(const CPGStableOneEndSeatedCriteria& criteria)
{
   if (m_Model.SetOneEndSeatedCriteria(criteria))
   {
      SetModifiedFlag();
   }
}

Float64 CPGStableDoc::GetDensity() const
{
   return m_Model.GetDensity();
}

void CPGStableDoc::SetDensity(Float64 density)
{
   if ( m_Model.SetDensity(density) )
   {
      SetModifiedFlag();
   }
}

Float64 CPGStableDoc::GetDensityWithRebar() const
{
   return m_Model.GetDensityWithRebar();
}

void CPGStableDoc::SetDensityWithRebar(Float64 density)
{
   if ( m_Model.SetDensityWithRebar(density) )
   {
      SetModifiedFlag();
   }
}

WBFL::Materials::ConcreteType CPGStableDoc::GetConcreteType() const
{
   return m_Model.GetConcreteType();
}

void CPGStableDoc::SetConcreteType(WBFL::Materials::ConcreteType type)
{
   if (m_Model.SetConcreteType(type))
   {
      SetModifiedFlag();
   }
}

Float64 CPGStableDoc::GetK1() const
{
   return m_Model.GetK1();
}

void CPGStableDoc::SetK1(Float64 k1)
{
   if ( m_Model.SetK1(k1) )
   {
      SetModifiedFlag();
   }
}

Float64 CPGStableDoc::GetK2() const
{
   return m_Model.GetK2();
}

void CPGStableDoc::SetK2(Float64 k2)
{
   if ( m_Model.SetK2(k2) )
   {
      SetModifiedFlag();
   }
}

void CPGStableDoc::GetLiftingMaterials(Float64* pFci,bool* pbComputeEci,Float64* pFrCoefficient) const
{
   m_Model.GetLiftingMaterials(pFci,pbComputeEci,pFrCoefficient);
}

void CPGStableDoc::SetLiftingMaterials(Float64 fci,bool bComputeEci,Float64 frCoefficient)
{
   if ( m_Model.SetLiftingMaterials(fci,bComputeEci,frCoefficient) )
   {
      SetModifiedFlag();
   }
}

void CPGStableDoc::GetHaulingMaterials(Float64* pFc,bool* pbComputeEc,Float64* pFrCoefficient) const
{
   m_Model.GetHaulingMaterials(pFc,pbComputeEc,pFrCoefficient);
}

void CPGStableDoc::SetHaulingMaterials(Float64 fc,bool bComputeEc,Float64 frCoefficient)
{
   if ( m_Model.SetHaulingMaterials(fc,bComputeEc,frCoefficient) )
   {
      SetModifiedFlag();
   }
}

void CPGStableDoc::GetOneEndSeatedMaterials(Float64* pFc, bool* pbComputeEc, Float64* pFrCoefficient) const
{
   m_Model.GetOneEndSeatedMaterials(pFc, pbComputeEc, pFrCoefficient);
}

void CPGStableDoc::SetOneEndSeatedMaterials(Float64 fc, bool bComputeEc, Float64 frCoefficient)
{
   if (m_Model.SetOneEndSeatedMaterials(fc, bComputeEc, frCoefficient))
   {
      SetModifiedFlag();
   }
}

Float64 CPGStableDoc::GetHeightOfGirderBottomAboveRoadway() const
{
   return m_Model.GetHeightOfGirderBottomAboveRoadway();
}

void CPGStableDoc::SetHeightOfGirderBottomAboveRoadway(Float64 Hgb)
{
   if ( m_Model.SetHeightOfGirderBottomAboveRoadway(Hgb) )
   {
      SetModifiedFlag();
   }
}

CString CPGStableDoc::UpdateEc(const CString& strFc)
{
   CString strEc;
   Float64 fc;
   Float64 ec = 0;
   if (WBFL::System::Tokenizer::ParseDouble(strFc, &fc) && 0 < fc)
   {
         CEAFApp* pApp = EAFGetApp();
         const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

         const WBFL::Units::Pressure& stress_unit = pDispUnits->Stress.UnitOfMeasure;

         fc       = WBFL::Units::ConvertToSysUnits(fc,      stress_unit);

         ec = m_Model.GetK1()*m_Model.GetK2()*WBFL::LRFD::ConcreteUtil::ModE(m_Model.GetConcreteType(),fc,m_Model.GetDensity(),false);

         strEc.Format(_T("%s"),FormatDimension(ec,pDispUnits->ModE,false));
   }

   return strEc;
}

void CPGStableDoc::ResolveStrandLocations(const CPGStableStrands& strands,const WBFL::Stability::Girder& girder,Float64* pXs,Float64* pYs,Float64* pXh,Float64* pXh1,Float64* pYh1,Float64* pXh2,Float64* pYh2,Float64* pXh3,Float64* pYh3,Float64* pXh4,Float64* pYh4,Float64* pXt,Float64* pYt) const
{
   m_Model.ResolveStrandLocations(strands,girder,pXs,pYs,pXh,pXh1,pYh1,pXh2,pYh2,pXh3,pYh3,pXh4,pYh4,pXt,pYt);
}

void CPGStableDoc::GetStrandProfiles(const CPGStableStrands& strands,const WBFL::Stability::Girder& girder,std::vector<std::pair<Float64,Float64>>* pvStraight,std::vector<std::pair<Float64,Float64>>* pvHarped,std::vector<std::pair<Float64,Float64>>* pvTemp) const
{
   m_Model.GetStrandProfiles(strands,girder,pvStraight,pvHarped,pvTemp);
}

WBFL::Stability::LiftingResults CPGStableDoc::GetLiftingResults() const
{
   return m_Model.GetLiftingResults();
}

WBFL::Stability::HaulingResults CPGStableDoc::GetHaulingResults() const
{
   return m_Model.GetHaulingResults();
}

WBFL::Stability::OneEndSeatedResults CPGStableDoc::GetOneEndSeatedResults() const
{
   return m_Model.GetOneEndSeatedResults();
}

WBFL::Stability::LiftingCheckArtifact CPGStableDoc::GetLiftingCheckArtifact() const
{
   return m_Model.GetLiftingCheckArtifact();
}

WBFL::Stability::HaulingCheckArtifact CPGStableDoc::GetHaulingCheckArtifact() const
{
   return m_Model.GetHaulingCheckArtifact();
}

WBFL::Stability::OneEndSeatedCheckArtifact CPGStableDoc::GetOneEndSeatedCheckArtifact() const
{
   return m_Model.GetOneEndSeatedCheckArtifact();
}

const SpecLibrary* CPGStableDoc::GetSpecLibrary() const
{
   return m_LibMgr.GetSpecLibrary();
}

const SpecLibraryEntry* CPGStableDoc::GetSpecLibraryEntry() const
{
   if ( m_strProjectCriteria == gs_strCriteria )
   {
      return nullptr;
   }
   else
   {
      const SpecLibrary* pLib = GetSpecLibrary();
      return (const SpecLibraryEntry*)(pLib->GetEntry(m_strProjectCriteria));
   }
}

const HaulTruckLibrary* CPGStableDoc::GetHaulTruckLibrary() const
{
   return m_LibMgr.GetHaulTruckLibrary();
}

const HaulTruckLibraryEntry* CPGStableDoc::GetHaulTruckLibraryEntry() const
{
   if (m_strHaulTruck == gs_strHaulTruck)
   {
      return nullptr;
   }
   else
   {
      const HaulTruckLibrary* pLib = GetHaulTruckLibrary();
      return (const HaulTruckLibraryEntry*)(pLib->GetEntry(m_strHaulTruck));
   }
}

const HaulTruckLibraryEntry* CPGStableDoc::GetOneEndSeatedHaulTruckLibraryEntry() const
{
   if (m_strOneEndSeatedHaulTruck == gs_strHaulTruck)
   {
      return nullptr;
   }
   else
   {
      const HaulTruckLibrary* pLib = GetHaulTruckLibrary();
      return (const HaulTruckLibraryEntry*)(pLib->GetEntry(m_strOneEndSeatedHaulTruck));
   }
}

const GirderLibrary* CPGStableDoc::GetGirderLibrary() const
{
   return &(m_LibMgr.GetGirderLibrary());
}

const GirderLibraryEntry* CPGStableDoc::GetGirderLibraryEntry() const
{
   if ( m_strGirder == gs_strGirder )
   {
      return nullptr;
   }
   else
   {
      const GirderLibrary* pLib = GetGirderLibrary();
      return (const GirderLibraryEntry*)(pLib->GetEntry(m_strGirder));
   }
}

bool CPGStableDoc::IsPermittedGirderEntry(const GirderLibraryEntry* pGirderEntry) const
{
   const GirderLibraryEntry::Dimensions& dimensions = pGirderEntry->GetDimensions();
   CComPtr<IBeamFactory> factory;
   pGirderEntry->GetBeamFactory(&factory);

   // Stabilty analysis is only applicable to I-Beam type girders
   auto clsidFamily = factory->GetFamilyCLSID();
   return ::IsEqualGUID(clsidFamily, CLSID_WFBeamFamily) || ::IsEqualGUID(clsidFamily, CLSID_DeckBulbTeeBeamFamily);
}
