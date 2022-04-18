///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2022  Washington State Department of Transportation
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CPGStableDoc

IMPLEMENT_DYNCREATE(CPGStableDoc, CBEToolboxDoc)

CPGStableDoc::CPGStableDoc()
{
   std::shared_ptr<CTitlePageBuilder> pLiftingTitlePageBuilder(std::make_shared<CPGStableTitlePageBuilder>());

   std::unique_ptr<CReportBuilder> pLiftingReportBuilder(std::make_unique<CReportBuilder>(_T("Lifting")));
   pLiftingReportBuilder->AddTitlePageBuilder(pLiftingTitlePageBuilder);
   pLiftingReportBuilder->AddChapterBuilder(std::dynamic_pointer_cast<CChapterBuilder>(std::make_shared<CPGStableLiftingSummaryChapterBuilder>(this)));
   pLiftingReportBuilder->AddChapterBuilder(std::dynamic_pointer_cast<CChapterBuilder>(std::make_shared<CPGStableLiftingDetailsChapterBuilder>(this)));

   m_RptMgr.AddReportBuilder(pLiftingReportBuilder.release());

   std::shared_ptr<CTitlePageBuilder> pHaulingTitlePageBuilder(std::make_shared<CPGStableTitlePageBuilder>());

   std::unique_ptr<CReportBuilder> pHaulingReportBuilder(std::make_unique<CReportBuilder>(_T("Hauling")));
   pHaulingReportBuilder->AddTitlePageBuilder(pHaulingTitlePageBuilder);
   pHaulingReportBuilder->AddChapterBuilder(std::dynamic_pointer_cast<CChapterBuilder>(std::make_shared<CPGStableHaulingSummaryChapterBuilder>(this)));
   pHaulingReportBuilder->AddChapterBuilder(std::dynamic_pointer_cast<CChapterBuilder>(std::make_shared<CPGStableHaulingDetailsChapterBuilder>(this)));

   m_RptMgr.AddReportBuilder(pHaulingReportBuilder.release());

   m_strProjectCriteria = gs_strCriteria;
   m_strHaulTruck = gs_strHaulTruck;
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

   FileStream ifile;
   if ( ifile.open(bpath) )
   {
      // try to load file
      try
      {
         // clear out library
         m_LibMgr.ClearAllEntries();

         sysStructuredLoadXmlPrs load;
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
      catch( sysXStructuredLoad& e )
      {
         CString strMsg;
         if ( e.GetExplicitReason() == sysXStructuredLoad::CantInitializeTheParser )
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

   pStrSave->BeginUnit(_T("LibraryReferences"),1.0);
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

      WBFL::Stability::LiftingStabilityProblem liftingProblem = GetLiftingStabilityProblem();
      liftingProblem.SetImpact(pSpec->GetLiftingUpwardImpactFactor(),pSpec->GetLiftingDownwardImpactFactor());

      liftingProblem.SetSupportPlacementTolerance(pSpec->GetLiftingLoopTolerance());
      liftingProblem.SetLiftAngle(pSpec->GetMinCableInclination());
      liftingProblem.SetSweepTolerance(pSpec->GetLiftingMaximumGirderSweepTolerance());
      liftingProblem.SetSweepGrowth(0); // no sweep growth for initial lifting problem
      liftingProblem.SetWindLoading((WBFL::Stability::WindType)pSpec->GetLiftingWindType(),pSpec->GetLiftingWindLoad());
      liftingProblem.SetYRollAxis(pSpec->GetPickPointHeight());
      SetLiftingStabilityProblem(liftingProblem);

      CPGStableLiftingCriteria liftingCriteria = GetLiftingCriteria();
      liftingCriteria.MinFSf = pSpec->GetLiftingFailureFOS();
      liftingCriteria.MinFScr = pSpec->GetCrackingFOSLifting();
      liftingCriteria.CompressionCoefficient_GlobalStress = pSpec->GetLiftingCompressionGlobalStressFactor();
      liftingCriteria.CompressionCoefficient_PeakStress = pSpec->GetLiftingCompressionPeakStressFactor();

      auto* pLiftingTensionStressLimit = dynamic_cast<WBFL::Stability::CCLiftingTensionStressLimit*>(liftingCriteria.TensionStressLimit.get());

      pLiftingTensionStressLimit->TensionCoefficient = pSpec->GetLiftingTensionStressFactor();
      pSpec->GetLiftingMaximumTensionStress(&pLiftingTensionStressLimit->bMaxTension,&pLiftingTensionStressLimit->MaxTension);
      pLiftingTensionStressLimit->TensionCoefficientWithRebar = pSpec->GetLiftingTensionStressFactorWithRebar();
      SetLiftingCriteria(liftingCriteria);


      Float64 Fc,FrCoefficient;
      bool bComputeEc;
      GetLiftingMaterials(&Fc,&bComputeEc,&FrCoefficient);
      SetLiftingMaterials(Fc,bComputeEc,pSpec->GetLiftingModulusOfRuptureFactor(pgsTypes::Normal));


      WBFL::Stability::HaulingStabilityProblem haulingProblem = GetHaulingStabilityProblem();
      haulingProblem.SetImpactUsage((WBFL::Stability::HaulingImpact)pSpec->GetHaulingImpactUsage());
      haulingProblem.SetImpact(pSpec->GetHaulingUpwardImpactFactor(),pSpec->GetHaulingDownwardImpactFactor());
      haulingProblem.SetCrownSlope(pSpec->GetRoadwayCrownSlope());
      haulingProblem.SetSuperelevation(pSpec->GetRoadwaySuperelevation());

      haulingProblem.SetSupportPlacementTolerance(pSpec->GetHaulingSupportPlacementTolerance());
      haulingProblem.SetSweepTolerance(pSpec->GetHaulingMaximumGirderSweepTolerance());
      haulingProblem.SetSweepGrowth(pSpec->GetHaulingSweepGrowth());
      haulingProblem.SetWindLoading((WBFL::Stability::WindType)pSpec->GetHaulingWindType(),pSpec->GetHaulingWindLoad());
      haulingProblem.SetCentrifugalForceType((WBFL::Stability::CFType)pSpec->GetCentrifugalForceType());
      haulingProblem.SetVelocity(pSpec->GetHaulingSpeed());
      haulingProblem.SetTurningRadius(pSpec->GetTurningRadius());
      SetHaulingStabilityProblem(haulingProblem);

      CPGStableHaulingCriteria haulingCriteria = GetHaulingCriteria();
      haulingCriteria.MinFSf = pSpec->GetHaulingFailureFOS();
      haulingCriteria.MinFScr = pSpec->GetHaulingCrackingFOS();
      haulingCriteria.CompressionCoefficient_GlobalStress = pSpec->GetHaulingCompressionGlobalStressFactor();
      haulingCriteria.CompressionCoefficient_PeakStress = pSpec->GetHaulingCompressionPeakStressFactor();
      
      auto* pHaulingTensionStressLimit = dynamic_cast<WBFL::Stability::CCHaulingTensionStressLimit*>(haulingCriteria.TensionStressLimit.get());

      pHaulingTensionStressLimit->TensionCoefficient[WBFL::Stability::CrownSlope] = pSpec->GetHaulingTensionStressFactor(pgsTypes::CrownSlope);


      pSpec->GetHaulingMaximumTensionStress(pgsTypes::CrownSlope, &pHaulingTensionStressLimit->bMaxTension[WBFL::Stability::CrownSlope],&pHaulingTensionStressLimit->MaxTension[WBFL::Stability::CrownSlope]);
      pHaulingTensionStressLimit->TensionCoefficientWithRebar[WBFL::Stability::CrownSlope] = pSpec->GetHaulingTensionStressFactorWithRebar(pgsTypes::CrownSlope);
      pHaulingTensionStressLimit->TensionCoefficient[WBFL::Stability::Superelevation] = pSpec->GetHaulingTensionStressFactor(pgsTypes::Superelevation);
      
      pSpec->GetHaulingMaximumTensionStress(pgsTypes::Superelevation, &pHaulingTensionStressLimit->bMaxTension[WBFL::Stability::Superelevation],&pHaulingTensionStressLimit->MaxTension[WBFL::Stability::Superelevation]);
      pHaulingTensionStressLimit->TensionCoefficientWithRebar[WBFL::Stability::Superelevation] = pSpec->GetHaulingTensionStressFactorWithRebar(pgsTypes::Superelevation);
      SetHaulingCriteria(haulingCriteria);

      GetHaulingMaterials(&Fc,&bComputeEc,&FrCoefficient);
      SetHaulingMaterials(Fc,bComputeEc,pSpec->GetHaulingModulusOfRuptureFactor(pgsTypes::Normal));
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

void CPGStableDoc::SetGirderType(int girderType)
{
   if ( m_Model.SetGirderType(girderType) )
   {
      SetModifiedFlag();
   }
}

int CPGStableDoc::GetGirderType() const
{
   return m_Model.GetGirderType();
}

const WBFL::Stability::Girder& CPGStableDoc::GetGirder(int girderType) const
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

void CPGStableDoc::SetGirder(int girderType,const WBFL::Stability::Girder& girder)
{
   if ( m_Model.SetGirder(girderType,girder) )
   {
      SetModifiedFlag();
   }
}

const CPGStableStrands& CPGStableDoc::GetStrands(int girderType,int modelType) const
{
   return m_Model.GetStrands(girderType,modelType);
}

void CPGStableDoc::SetStrands(int girderType,int modelType,const CPGStableStrands& strands)
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

void CPGStableDoc::SetLiftingStabilityProblem(const WBFL::Stability::LiftingStabilityProblem& liftingStability)
{
   if ( m_Model.SetLiftingStabilityProblem(liftingStability) )
   {
      SetModifiedFlag();
   }
}

const WBFL::Stability::HaulingStabilityProblem& CPGStableDoc::GetHaulingStabilityProblem() const
{
   return m_Model.GetHaulingStabilityProblem();
}

void CPGStableDoc::SetHaulingStabilityProblem(const WBFL::Stability::HaulingStabilityProblem& HaulingStability)
{
   if ( m_Model.SetHaulingStabilityProblem(HaulingStability) )
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

matConcrete::Type CPGStableDoc::GetConcreteType() const
{
   return m_Model.GetConcreteType();
}

void CPGStableDoc::SetConcreteType(matConcrete::Type type)
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

CString CPGStableDoc::UpdateEc(const CString& strFc,const CString& strDensity,const CString& strK1,const CString& strK2)
{
   CString strEc;
   Float64 fc, density, k1,k2;
   Float64 ec = 0;
   if (sysTokenizer::ParseDouble(strFc, &fc) && 
       sysTokenizer::ParseDouble(strDensity,&density) &&
       sysTokenizer::ParseDouble(strK1,&k1) &&
       sysTokenizer::ParseDouble(strK2,&k2) &&
       0 < density && 0 < fc && 0 < k1 && 0 < k2
       )
   {
         CEAFApp* pApp = EAFGetApp();
         const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

         const unitPressure& stress_unit = pDispUnits->Stress.UnitOfMeasure;
         const unitDensity& density_unit = pDispUnits->Density.UnitOfMeasure;

         fc       = ::ConvertToSysUnits(fc,      stress_unit);
         density  = ::ConvertToSysUnits(density, density_unit);

         ec = k1*k2*lrfdConcreteUtil::ModE(m_Model.GetConcreteType(),fc,density,false);

         strEc.Format(_T("%s"),FormatDimension(ec,pDispUnits->ModE,false));
   }

   return strEc;
}

void CPGStableDoc::ResolveStrandLocations(const CPGStableStrands& strands,const WBFL::Stability::Girder& girder,Float64* pXs,Float64* pYs,Float64* pXh,Float64* pXh1,Float64* pYh1,Float64* pXh2,Float64* pYh2,Float64* pXh3,Float64* pYh3,Float64* pXh4,Float64* pYh4,Float64* pXt,Float64* pYt)
{
   m_Model.ResolveStrandLocations(strands,girder,pXs,pYs,pXh,pXh1,pYh1,pXh2,pYh2,pXh3,pYh3,pXh4,pYh4,pXt,pYt);
}

void CPGStableDoc::GetStrandProfiles(const CPGStableStrands& strands,const WBFL::Stability::Girder& girder,std::vector<std::pair<Float64,Float64>>* pvStraight,std::vector<std::pair<Float64,Float64>>* pvHarped,std::vector<std::pair<Float64,Float64>>* pvTemp)
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

WBFL::Stability::LiftingCheckArtifact CPGStableDoc::GetLiftingCheckArtifact() const
{
   return m_Model.GetLiftingCheckArtifact();
}

WBFL::Stability::HaulingCheckArtifact CPGStableDoc::GetHaulingCheckArtifact() const
{
   return m_Model.GetHaulingCheckArtifact();
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
   if ( m_strHaulTruck == gs_strHaulTruck )
   {
      return nullptr;
   }
   else
   {
      const HaulTruckLibrary* pLib = GetHaulTruckLibrary();
      return (const HaulTruckLibraryEntry*)(pLib->GetEntry(m_strHaulTruck));
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
