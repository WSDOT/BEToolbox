///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2020  Washington State Department of Transportation
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

// M3CDoc.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "M3CDoc.h"
#include "M3CTitlePageBuilder.h"
#include "M3CChapterBuilder.h"
#include "M3CDetailsChapterBuilder.h"
#include "M3CAnalysisDetailsChapterBuilder.h"
#include "M3CAnalysisDetailsReportSpecificationBuilder.h"
#include "BEToolboxStatusBar.h"

#include <EAF\EAFUtilities.h>
#include <EAF\EAFApp.h>

#include "M3C.hh"
#include <EAF\EAFHelp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CM3CDoc

IMPLEMENT_DYNCREATE(CM3CDoc, CBEToolboxDoc)

CM3CDoc::CM3CDoc()
{
   std::shared_ptr<CTitlePageBuilder> pTitlePageBuilder(std::make_shared<CM3CTitlePageBuilder>());

   std::unique_ptr<CReportBuilder> pRptBuilder(std::make_unique<CReportBuilder>(_T("M3C")));
   pRptBuilder->AddTitlePageBuilder( pTitlePageBuilder );
   pRptBuilder->AddChapterBuilder(std::shared_ptr<CChapterBuilder>(new CM3CChapterBuilder(this)));
   m_RptMgr.AddReportBuilder(pRptBuilder.release());

   pRptBuilder = std::make_unique<CReportBuilder>(_T("M3CMaterialsDetails"));
   pRptBuilder->AddTitlePageBuilder(pTitlePageBuilder);
   pRptBuilder->AddChapterBuilder(std::shared_ptr<CChapterBuilder>(new CM3CMaterialDetailsChapterBuilder(this)));
   m_RptMgr.AddReportBuilder(pRptBuilder.release());

   pRptBuilder = std::make_unique<CReportBuilder>(_T("M3CAnalysisDetails"));
   std::shared_ptr<CReportSpecificationBuilder> pRptSpecBuilder(std::make_shared<CM3CAnalysisDetailsReportSpecificationBuilder>());
   pRptBuilder->SetReportSpecificationBuilder(pRptSpecBuilder);
   pRptBuilder->AddTitlePageBuilder(pTitlePageBuilder);
   pRptBuilder->AddChapterBuilder(std::shared_ptr<CChapterBuilder>(new CM3CAnalysisDetailsChapterBuilder(this)));
   m_RptMgr.AddReportBuilder(pRptBuilder.release());

   m_bIsSolutionValid = false;
   m_bIsModelValid = false;

   UIHints(FALSE); // not using UIHints feature
}

CM3CDoc::~CM3CDoc()
{
}

void CM3CDoc::LoadToolbarResource()
{
   // we must override this method and set the modulue state
   // so the base class version can load our toolbar
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CBEToolboxDoc::LoadToolbarResource();
}

BEGIN_MESSAGE_MAP(CM3CDoc, CBEToolboxDoc)
   ON_COMMAND(ID_REFRESH_REPORT, &CM3CDoc::OnRefreshReport)
   ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
   ON_UPDATE_COMMAND_UI(ID_HELP_FINDER,OnUpdateHelpFinder)
END_MESSAGE_MAP()

void CM3CDoc::OnUpdateHelpFinder(CCmdUI* pUI)
{
   pUI->Enable(FALSE); // we don't have HELP yet... so disable the command
}

void CM3CDoc::OnHelpFinder()
{
   //EAFHelp(EAFGetDocument()->GetDocumentationSetName(),IDH_M3C);
}


// CM3CDoc diagnostics

#ifdef _DEBUG
void CM3CDoc::AssertValid() const
{
	CBEToolboxDoc::AssertValid();
}

#ifndef _WIN32_WCE
void CM3CDoc::Dump(CDumpContext& dc) const
{
	CBEToolboxDoc::Dump(dc);
}
#endif
#endif //_DEBUG

BOOL CM3CDoc::Init()
{
   if ( !CBEToolboxDoc::Init() )
      return FALSE;

   return TRUE;
}

void CM3CDoc::OnCloseDocument()
{
   CBEToolboxDoc::OnCloseDocument();
}

void CM3CDoc::OnRefreshReport()
{
   UpdateAllViews(nullptr);
}

HRESULT CM3CDoc::WriteTheDocument(IStructuredSave* pStrSave)
{
   HRESULT hr = pStrSave->BeginUnit(_T("M3C"),1.0);
   if ( FAILED(hr) )
      return hr;

   CEAFApp* pApp = EAFGetApp();

   hr = pStrSave->put_Property(_T("Units"),CComVariant(pApp->GetUnitsMode()));
   if ( FAILED(hr) )
      return hr;

   hr = m_ProblemParams.Save(pStrSave);
   if (FAILED(hr))
      return hr;

   hr = pStrSave->EndUnit();
   if ( FAILED(hr) )
      return hr;

   return S_OK;
}

HRESULT CM3CDoc::LoadTheDocument(IStructuredLoad* pStrLoad)
{
   HRESULT hr = pStrLoad->BeginUnit(_T("M3C"));
   if ( FAILED(hr) )
      return hr;

   CComVariant var;

   CEAFApp* pApp = EAFGetApp();

   var.vt = VT_I4;
   hr = pStrLoad->get_Property(_T("Units"),&var);
   if ( FAILED(hr) )
      return hr;
   pApp->SetUnitsMode(eafTypes::UnitMode(var.lVal));

   hr = m_ProblemParams.Load(pStrLoad);
   if (FAILED(hr))
      return hr;

   hr = pStrLoad->EndUnit();
   if ( FAILED(hr) )
      return hr;

   return S_OK;
}

CString CM3CDoc::GetToolbarSectionName()
{
   return _T("M3C");
}

void CM3CDoc::LoadDocumentSettings()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   __super::LoadDocumentSettings();
}

void CM3CDoc::SaveDocumentSettings()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   __super::SaveDocumentSettings();
}

CString CM3CDoc::GetDocumentationRootLocation()
{
   CEAFApp* pApp = EAFGetApp();
   return pApp->GetDocumentationRootLocation();
}

void CM3CDoc::SetProblemParameters(const CM3CProblemParameters& params,BOOL bUpdate)
{
   if (m_ProblemParams != params)
   {
      m_ProblemParams = params;

      InvalidateModel();

      SetModifiedFlag(TRUE);
      if (bUpdate)
      {
         OnRefreshReport();
      }
   }
}

const CM3CProblemParameters& CM3CDoc::GetProblemParameters() const
{
   return m_ProblemParams;
}

void CM3CDoc::GetConcreteModel(IStressStrain** ppConcrete) const
{
   ValidateModel();
   m_Concrete.CopyTo(ppConcrete);
}

void CM3CDoc::GetRebarModel(IStressStrain** ppRebar) const
{
   ValidateModel();
   m_Rebar.CopyTo(ppRebar);
}

void CM3CDoc::GetStrandModel(IStressStrain** ppStrand) const
{
   ValidateModel();
   m_Strand.CopyTo(ppStrand);
}

void CM3CDoc::GetSection(IGeneralSection** ppSection) const
{
   ValidateModel();
   m_Section.CopyTo(ppSection);
}

void CM3CDoc::GetMomentCurvature(IMomentCurvatureSolution** ppSolution) const
{
   if (m_bIsSolutionValid)
   {
      m_Solution.CopyTo(ppSolution);
      return;
   }

   CWaitCursor wait;

   CComPtr<IGeneralSection> column_section;
   GetSection(&column_section);

   CComPtr<IMomentCurvatureSolver> solver;
   solver.CoCreateInstance(CLSID_MomentCurvatureSolver);

   solver->put_Slices(m_ProblemParams.nSlices);
   solver->put_SliceGrowthFactor(2);
   solver->put_InitialCurvatureStep(m_ProblemParams.initialStep);
   solver->putref_Section(column_section);
   solver->put_AxialTolerance(::ConvertToSysUnits(0.01, unitMeasure::Kip));
   
   m_Solution.Release();
   Float64 NASlope = 0.0;
   solver->Solve(-m_ProblemParams.P, NASlope, &m_Solution);
   m_bIsSolutionValid = true;
   m_Solution.CopyTo(ppSolution);
}

void CM3CDoc::InvalidateModel()
{
   m_bIsSolutionValid = false;
   m_bIsModelValid = false;
   m_Solution.Release();
   m_Section.Release();
   m_Concrete.Release();
   m_Rebar.Release();
   m_Strand.Release();
}

void CM3CDoc::ValidateModel() const
{
   if (m_bIsModelValid)
      return;

   m_Section.Release();
   BuildColumnModel(&m_Section);
   m_bIsModelValid = true;
}

void CM3CDoc::BuildConcreteModel(IStressStrain** ppConcrete) const
{
   CComPtr<ICircularManderSection> section;
   section.CoCreateInstance(CLSID_CircularManderSection);

   Float64 As_bonded = m_ProblemParams.Bonded_Rebar_As_per_bar * m_ProblemParams.Bonded_Rebar_nBars;
   section->put_As(As_bonded);
   section->put_Asp(m_ProblemParams.AsSpiral);
   section->put_Cover(m_ProblemParams.Cover);
   section->put_db(m_ProblemParams.db);
   section->put_Diameter(m_ProblemParams.D);
   section->put_S(m_ProblemParams.S);
   section->put_TransvReinforcementType(trtSpiral);
   section->put_TransvYieldStrength(m_ProblemParams.FySpiral);
   section->put_TransvReinforcementRuptureStrain(m_ProblemParams.esu);

   CComPtr<IManderModel> model;
   model.CoCreateInstance(CLSID_ManderModel);
   model->put_eco(m_ProblemParams.eco);
   model->put_fco(m_ProblemParams.fco);
   model->put_R(5);
   model->putref_Section(section);

   model.QueryInterface(ppConcrete);
}

void CM3CDoc::BuildRebarModel(IStressStrain** ppSteel) const
{
   CComPtr<IStrainHardenedRebarModel> rebar;
   rebar.CoCreateInstance(CLSID_StrainHardenedRebarModel);
   rebar->put_fy(m_ProblemParams.fy);
   rebar->put_fu(m_ProblemParams.fu);
   rebar->put_Es(m_ProblemParams.Es);
   rebar->put_esh(m_ProblemParams.esh);
   rebar->put_efr(m_ProblemParams.efr);

   rebar.QueryInterface(ppSteel);
}

void CM3CDoc::BuildStrandModel(IStressStrain** ppStrand) const
{
   StrandGradeType grade = m_ProblemParams.pStrand->GetGrade() == matPsStrand::Gr1725 ? sgtGrade250 : sgtGrade270;
   ProductionMethodType type = m_ProblemParams.pStrand->GetType() == matPsStrand::LowRelaxation ? pmtLowRelaxation : pmtStressRelieved;

   CComPtr<IPowerFormula> powerFormula;
   powerFormula.CoCreateInstance(CLSID_PSPowerFormula);
   powerFormula->put_Grade(grade);
   powerFormula->put_ProductionMethod(type);

   CComQIPtr<IStressStrain> ssStrand(powerFormula);
   ssStrand.CopyTo(ppStrand);
}

void CM3CDoc::BuildColumnModel(IGeneralSection** ppSection) const
{
   ATLASSERT(!m_bIsModelValid);

   BuildConcreteModel(&m_Concrete);
   BuildRebarModel(&m_Rebar);
   BuildStrandModel(&m_Strand);

   CComPtr<IGeneralSection> column_section;
   column_section.CoCreateInstance(CLSID_GeneralSection);

   CComPtr<ICircle> column;
   column.CoCreateInstance(CLSID_Circle);
   column->put_Radius(m_ProblemParams.D / 2);
   CComQIPtr<IShape> column_shape(column);
   Float64 ei = 0.0; // initial strain
   Float64 Le = 1.0; // elongation length
   column_section->AddShape(column_shape, m_Concrete, nullptr, ei, Le);

   CComPtr<IGeomUtil2d> geom_util;
   geom_util.CoCreateInstance(CLSID_GeomUtil);
   CComPtr<IPoint2d> center;
   column->get_Center(&center);

   CComPtr<IPoint2dCollection> points;
   Float64 bar_radius = sqrt(m_ProblemParams.Bonded_Rebar_As_per_bar / M_PI);
   Float64 radius = (m_ProblemParams.D - 2*(m_ProblemParams.Cover + m_ProblemParams.db)  - bar_radius) / 2;
   geom_util->GenerateCircle(m_ProblemParams.Bonded_Rebar_nBars, center, radius, 0.0, &points);

   IndexType nBars;
   points->get_Count(&nBars);
   for (IndexType i = 0; i < nBars; i++)
   {
      CComPtr<IPoint2d> pnt;
      points->get_Item(i, &pnt);

      CComPtr<IGenericShape> bar;
      bar.CoCreateInstance(CLSID_GenericShape);
      bar->put_Area(m_ProblemParams.Bonded_Rebar_As_per_bar);
      bar->putref_Centroid(pnt);

      CComQIPtr<IShape> bar_shape(bar);

      Float64 ei = 0.0; // initial strain
      Float64 Le = 1.0; // elongation length

      column_section->AddShape(bar_shape, m_Rebar, nullptr, ei, Le);
   }

   points.Release();
   bar_radius = sqrt(m_ProblemParams.Unbonded_Rebar_As_per_bar / M_PI);
   radius = (m_ProblemParams.D - 2 * (m_ProblemParams.Cover + m_ProblemParams.db) - bar_radius) / 2;
   geom_util->GenerateCircle(m_ProblemParams.Unbonded_Rebar_nBars, center, radius, 0.0, &points);

   points->get_Count(&nBars);
   for (IndexType i = 0; i < nBars; i++)
   {
      CComPtr<IPoint2d> pnt;
      points->get_Item(i, &pnt);

      CComPtr<IGenericShape> bar;
      bar.CoCreateInstance(CLSID_GenericShape);
      bar->put_Area(m_ProblemParams.Unbonded_Rebar_As_per_bar);
      bar->putref_Centroid(pnt);

      CComQIPtr<IShape> bar_shape(bar);

      Float64 ei = 0.0; // initial strain
      Float64 Le = m_ProblemParams.Unbonded_Rebar_Lu; // elongation length

      column_section->AddShape(bar_shape, m_Rebar, nullptr, ei, Le);
   }


   points.Release();
   radius = (m_ProblemParams.Tendon_Ring_Diameter) / 2;
   geom_util->GenerateCircle(m_ProblemParams.Tendon_nStrands, center, radius, 0.0, &points);

   Float64 aps = m_ProblemParams.pStrand->GetNominalArea();
   points->get_Count(&nBars);

   Float64 Eps = ::ConvertToSysUnits(m_ProblemParams.pStrand->GetE(), unitMeasure::KSI);
   for (IndexType i = 0; i < nBars; i++)
   {
      CComPtr<IPoint2d> pnt;
      points->get_Item(i, &pnt);

      CComPtr<IGenericShape> strand;
      strand.CoCreateInstance(CLSID_GenericShape);
      strand->put_Area(aps);
      strand->putref_Centroid(pnt);

      CComQIPtr<IShape> strand_shape(strand);

      Float64 ei = m_ProblemParams.Tendon_fpe/Eps; // initial strain
      Float64 Le = m_ProblemParams.Tendon_Lu; // elongation length

      column_section->AddShape(strand_shape, m_Strand, nullptr, ei, Le);
   }

   column_section.CopyTo(ppSection);
}