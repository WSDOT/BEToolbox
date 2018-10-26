///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2016  Washington State Department of Transportation
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
#include "resource.h"
#include "PGStableDoc.h"
#include "BEToolboxStatusBar.h"

#include <EAF\EAFUtilities.h>
#include <EAF\EAFApp.h>
#include <MFCTools\AutoRegistry.h>
#include <System\System.h>

#include "PGStableTitlePageBuilder.h"
#include "PGStableLiftingSummaryChapterBuilder.h"
#include "PGStableLiftingDetailsChapterBuilder.h"
#include "PGStableHaulingSummaryChapterBuilder.h"
#include "PGStableHaulingDetailsChapterBuilder.h"

// CPGStableDoc

IMPLEMENT_DYNCREATE(CPGStableDoc, CBEToolboxDoc)

CPGStableDoc::CPGStableDoc()
{
   CReportBuilder* pRptBuilder = new CReportBuilder(_T("PGStable"));

   boost::shared_ptr<CTitlePageBuilder> pTitlePageBuilder(new CPGStableTitlePageBuilder());
   pRptBuilder->AddTitlePageBuilder( pTitlePageBuilder );

   pRptBuilder->AddChapterBuilder(boost::shared_ptr<CChapterBuilder>(new CPGStableLiftingSummaryChapterBuilder(this)));
   pRptBuilder->AddChapterBuilder(boost::shared_ptr<CChapterBuilder>(new CPGStableLiftingDetailsChapterBuilder(this)));
   pRptBuilder->AddChapterBuilder(boost::shared_ptr<CChapterBuilder>(new CPGStableHaulingSummaryChapterBuilder(this)));
   pRptBuilder->AddChapterBuilder(boost::shared_ptr<CChapterBuilder>(new CPGStableHaulingDetailsChapterBuilder(this)));

   m_RptMgr.AddReportBuilder(pRptBuilder);

   UIHints(FALSE); // not using UIHints feature
}

CPGStableDoc::~CPGStableDoc()
{
}


BEGIN_MESSAGE_MAP(CPGStableDoc, CBEToolboxDoc)
   ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
END_MESSAGE_MAP()

void CPGStableDoc::OnHelpFinder()
{
   EAFHelp(EAFGetDocument()->GetDocumentationSetName(),IDH_PGSTABLE);
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

   //LoadPGSLibrary();

   return TRUE;
}

//void CPGStableDoc::LoadPGSLibrary()
//{
//   CAutoRegistry autoReg(_T("PGSuper"));
//
//   CEAFApp* pApp = EAFGetApp();
//
//   CString strMasterLibaryFile = pApp->GetProfileString(_T("Options"),_T("MasterLibraryCache2"));
//
//   if (strMasterLibaryFile.IsEmpty())
//   {
//#pragma Reminder("WORKING HERE - deal with case of no library information")
//      //// PGSuper's installer should take care of this, but just in case:
//      //TxDOTBrokerRetrieverException exc;
//      //exc.Message = _T("The location of the PGSuper Master Library file is not in the registry. You must run PGSuper at least once before you can run TOGA. All it takes is opening and closing a .pgs file. You can do this now.");
//      //WATCH(exc.Message);
//      //throw exc;
//   }
//
//   CString strURL = pApp->GetProfileString(_T("Options"),_T("MasterLibraryURL2"));
//
//   if (strURL.IsEmpty())
//   {
//      strURL = strMasterLibaryFile;
//   }
//
//   CString strServer = pApp->GetProfileString(_T("Options"),_T("CatalogServer2"));
//
//   m_LibMgr.SetName( _T("PGS Library") );
//   m_LibMgr.SetMasterLibraryInfo(strServer,strURL);
//
//   CComBSTR bpath(strMasterLibaryFile);
//
//   FileStream ifile;
//   if ( ifile.open(bpath) )
//   {
//      // try to load file
//      try
//      {
//         // clear out library
//         m_LibMgr.ClearAllEntries();
//
//         sysStructuredLoadXmlPrs load;
//         load.BeginLoad( &ifile );
//
//         // Problem : Library Editor application specific code is in the
//         // master library file. We have to read it or get an error.
//         load.BeginUnit(_T("PGSuperLibrary"));
//         load.BeginUnit(_T("LIBRARY_EDITOR"));
//         std::_tstring str;
//         load.Property(_T("EDIT_UNITS"), &str);
//
//         if ( !m_LibMgr.LoadMe( &load ) )
//         {
//#pragma Reminder("WORKING HERE - deal with case of failed library load")
//            //TxDOTBrokerRetrieverException exc;
//            //exc.Message = _T("An unknown error occurred while loading the master library file.");
//            //WATCH(exc.Message);
//            //throw exc;
//         }
//
//         load.EndUnit(); //"LIBRARY_EDITOR"
//         load.EndUnit(); //"PGSuperLibrary"
//         load.EndLoad();
//
//         // success!
//         WATCH(_T("Master Library loaded successfully"));
//      }
//      catch( sysXStructuredLoad& e )
//      {
//#pragma Reminder("WORKING HERE - deal with library load errors")
//         //TxDOTBrokerRetrieverException exc;
//         //if ( e.GetExplicitReason() == sysXStructuredLoad::CantInitializeTheParser )
//         //{
//         //   exc.Message = _T("Failed to initialize the xml parser. This is an installation issue.");
//         //}
//         //else
//         //{
//         //   ASSERT(0);
//         //   exc.Message = _T("An unknown error occurred while loading the master library file.");
//         //}
//
//         //WATCH(exc.Message);
//         //throw exc;
//
//      }
//      catch(...)
//      {
//#pragma Reminder("WORKING HERE - deal with library load errors")
//         //TxDOTBrokerRetrieverException exc;
//         //exc.Message = _T("An unknown error occurred while loading the master library file.");
//         //WATCH(exc.Message);
//         //throw exc;
//      }
//   }
//   else
//   {
//#pragma Reminder("WORKING HERE - deal with library load errors")
//      //TxDOTBrokerRetrieverException exc;
//      //exc.Message.Format(_T("Failed to open the master library file: %s"),strMasterLibaryFile);
//      //WATCH(exc.Message);
//      //throw exc;
//   }
//
//   // make all entries in master library read-only
//   m_LibMgr.EnableEditingForAllEntries(false);
//}

HRESULT CPGStableDoc::WriteTheDocument(IStructuredSave* pStrSave)
{
   return m_Model.Save(pStrSave);
}

HRESULT CPGStableDoc::LoadTheDocument(IStructuredLoad* pStrLoad)
{
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

void CPGStableDoc::SetGirderType(int type)
{
   if ( m_Model.m_GirderType != type )
   {
      m_Model.m_GirderType = type;
      SetModifiedFlag();
   }
}

int CPGStableDoc::GetGirderType() const
{
   return m_Model.m_GirderType;
}

void CPGStableDoc::SetLiftingFpeType(int type)
{
   if ( m_Model.m_LiftingFpeType != type )
   {
      m_Model.m_LiftingFpeType = type;
      SetModifiedFlag();
   }
}

int CPGStableDoc::GetLiftingFpeType() const
{
   return m_Model.m_LiftingFpeType;
}

void CPGStableDoc::SetHaulingFpeType(int type)
{
   if ( m_Model.m_HaulingFpeType != type )
   {
      m_Model.m_HaulingFpeType = type;
      SetModifiedFlag();
   }
}

int CPGStableDoc::GetHaulingFpeType() const
{
   return m_Model.m_HaulingFpeType;
}

const stbGirder& CPGStableDoc::GetGirder(int type) const
{
   return m_Model.m_Girder[type];
}

void CPGStableDoc::SetGirder(int type,const stbGirder& girder)
{
   if ( m_Model.m_Girder[type] != girder )
   {
      m_Model.m_Girder[type] = girder;
      SetModifiedFlag();
   }
}

const CPGStableStrands& CPGStableDoc::GetStrands(int type) const
{
   return m_Model.m_Strands[type];
}

void CPGStableDoc::SetStrands(int type,const CPGStableStrands& strands)
{
   if ( m_Model.m_Strands[type] != strands )
   {
      m_Model.m_Strands[type] = strands;
      SetModifiedFlag();
   }
}

const stbLiftingStabilityProblem& CPGStableDoc::GetLiftingStabilityProblem() const
{
   return m_Model.m_LiftingStabilityProblem;
}

void CPGStableDoc::SetLiftingStabilityProblem(const stbLiftingStabilityProblem& liftingStability)
{
   if ( m_Model.m_LiftingStabilityProblem != liftingStability )
   {
      m_Model.m_LiftingStabilityProblem = liftingStability;
      SetModifiedFlag();
   }
}

const stbHaulingStabilityProblem& CPGStableDoc::GetHaulingStabilityProblem() const
{
   return m_Model.m_HaulingStabilityProblem;
}

void CPGStableDoc::SetHaulingStabilityProblem(const stbHaulingStabilityProblem& HaulingStability)
{
   if ( m_Model.m_HaulingStabilityProblem != HaulingStability )
   {
      m_Model.m_HaulingStabilityProblem = HaulingStability;
      SetModifiedFlag();
   }
}

const CPGStableCriteria& CPGStableDoc::GetLiftingCriteria() const
{
   return m_Model.m_LiftingCriteria;
}

void CPGStableDoc::SetLiftingCriteria(const CPGStableCriteria& criteria)
{
   if ( m_Model.m_LiftingCriteria != criteria )
   {
      m_Model.m_LiftingCriteria = criteria;
      SetModifiedFlag();
   }
}

const CPGStableCriteria& CPGStableDoc::GetHaulingCriteria() const
{
   return m_Model.m_HaulingCriteria;
}

void CPGStableDoc::SetHaulingCriteria(const CPGStableCriteria& criteria)
{
   if ( m_Model.m_HaulingCriteria != criteria )
   {
      m_Model.m_HaulingCriteria = criteria;
      SetModifiedFlag();
   }
}

Float64 CPGStableDoc::GetDensity() const
{
   return m_Model.m_Density;
}

void CPGStableDoc::SetDensity(Float64 density)
{
   if ( !IsEqual(m_Model.m_Density,density) )
   {
      m_Model.m_Density = density;
      SetModifiedFlag();
   }
}

Float64 CPGStableDoc::GetK1() const
{
   return m_Model.m_K1;
}

void CPGStableDoc::SetK1(Float64 k1)
{
   if ( !IsEqual(m_Model.m_K1,k1) )
   {
      m_Model.m_K1 = k1;
      SetModifiedFlag();
   }
}

Float64 CPGStableDoc::GetK2() const
{
   return m_Model.m_K2;
}

void CPGStableDoc::SetK2(Float64 k2)
{
   if ( !IsEqual(m_Model.m_K2,k2) )
   {
      m_Model.m_K2 = k2;
      SetModifiedFlag();
   }
}

void CPGStableDoc::GetLiftingMaterials(Float64* pFci,bool* pbComputeEci,Float64* pFrCoefficient) const
{
   *pFci = m_Model.m_Fci;
   *pbComputeEci = m_Model.m_bComputeEci;
   *pFrCoefficient = m_Model.m_LiftingFrCoefficient;
}

void CPGStableDoc::SetLiftingMaterials(Float64 fci,bool bComputeEci,Float64 frCoefficient)
{
   if ( !IsEqual(m_Model.m_Fci,fci) || m_Model.m_bComputeEci != bComputeEci || !IsEqual(m_Model.m_LiftingFrCoefficient,frCoefficient) )
   {
      m_Model.m_Fci = fci;
      m_Model.m_bComputeEci = bComputeEci;
      m_Model.m_LiftingFrCoefficient = frCoefficient;
      SetModifiedFlag();
   }
}

void CPGStableDoc::GetHaulingMaterials(Float64* pFc,bool* pbComputeEc,Float64* pFrCoefficient) const
{
   *pFc = m_Model.m_Fc;
   *pbComputeEc = m_Model.m_bComputeEc;
   *pFrCoefficient = m_Model.m_HaulingFrCoefficient;
}

void CPGStableDoc::SetHaulingMaterials(Float64 fc,bool bComputeEc,Float64 frCoefficient)
{
   if ( !IsEqual(m_Model.m_Fc,fc) || m_Model.m_bComputeEc != bComputeEc || !IsEqual(m_Model.m_HaulingFrCoefficient,frCoefficient) )
   {
      m_Model.m_Fc = fc;
      m_Model.m_bComputeEc = bComputeEc;
      m_Model.m_HaulingFrCoefficient = frCoefficient;
      SetModifiedFlag();
   }
}

void CPGStableDoc::SetHeightOfGirderBottomAboveRoadway(Float64 Hgb)
{
   if ( !IsEqual(m_Model.m_Hgb,Hgb) )
   {
      m_Model.m_Hgb = Hgb;
      SetModifiedFlag();
   }
}

Float64 CPGStableDoc::GetHeightOfGirderBottomAboveRoadway() const
{
   return m_Model.m_Hgb;
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

         ec = k1*k2*lrfdConcreteUtil::ModE(fc,density,false);

         strEc.Format(_T("%s"),FormatDimension(ec,pDispUnits->ModE,false));
   }

   return strEc;
}

void CPGStableDoc::ResolveStrandLocations(const CPGStableStrands* pStrands,stbGirder* pGirder) const
{
   m_Model.ResolveStrandLocations(pStrands,pGirder);
}

stbLiftingResults CPGStableDoc::GetLiftingResults() const
{
   return m_Model.GetLiftingResults();
}

stbHaulingResults CPGStableDoc::GetHaulingResults() const
{
   return m_Model.GetHaulingResults();
}

stbLiftingCheckArtifact CPGStableDoc::GetLiftingCheckArtifact() const
{
   return m_Model.GetLiftingCheckArtifact();
}

stbHaulingCheckArtifact CPGStableDoc::GetHaulingCheckArtifact() const
{
   return m_Model.GetHaulingCheckArtifact();
}

//const SpecLibrary* CPGStableDoc::GetSpecLibrary() const
//{
//   return m_LibMgr.GetSpecLibrary();
//}
