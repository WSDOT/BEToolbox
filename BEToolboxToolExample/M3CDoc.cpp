///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2018  Washington State Department of Transportation
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
#include "BEToolboxStatusBar.h"

#include <EAF\EAFUtilities.h>
#include <EAF\EAFApp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CM3CDoc

IMPLEMENT_DYNCREATE(CM3CDoc, CBEToolboxDoc)

CM3CDoc::CM3CDoc()
{
   std::unique_ptr<CReportBuilder> pRptBuilder(std::make_unique<CReportBuilder>(_T("M3C")));

   std::shared_ptr<CTitlePageBuilder> pTitlePageBuilder(std::make_shared<CM3CTitlePageBuilder>());
   pRptBuilder->AddTitlePageBuilder( pTitlePageBuilder );

   pRptBuilder->AddChapterBuilder(std::shared_ptr<CChapterBuilder>(new CM3CChapterBuilder(this)));
   pRptBuilder->AddChapterBuilder(std::shared_ptr<CChapterBuilder>(new CM3CDetailsChapterBuilder(this)));

   m_RptMgr.AddReportBuilder(pRptBuilder.release());

   m_bIsSolutionValid = false;

   UIHints(FALSE); // not using UIHints feature
}

CM3CDoc::~CM3CDoc()
{
}


BEGIN_MESSAGE_MAP(CM3CDoc, CBEToolboxDoc)
   ON_COMMAND(ID_REFRESH_REPORT, &CM3CDoc::OnRefreshReport)
   ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
END_MESSAGE_MAP()

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
      m_bIsSolutionValid = false;
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

void CM3CDoc::GetMomentCurvature(IMomentCurvatureSolution** ppSolution) const
{
   if (m_bIsSolutionValid)
   {
      m_Solution.CopyTo(ppSolution);
      return;
   }

   CWaitCursor wait;

   CComPtr<IGeneralSection> column_section;
   BuildColumnSection(&column_section);

   CComPtr<IMomentCurvatureSolver> solver;
   solver.CoCreateInstance(CLSID_MomentCurvatureSolver);

   solver->put_Slices(m_ProblemParams.nSlices);
   //solver->put_SliceGrowthFactor(2);
   solver->put_InitialCurvatureStep(m_ProblemParams.initialStep);
   solver->putref_Section(column_section);
   solver->put_AxialTolerance(::ConvertToSysUnits(0.01, unitMeasure::Kip));
   m_Solution.Release();
   solver->Solve(m_ProblemParams.P, m_ProblemParams.NASlope, &m_Solution);
   m_bIsSolutionValid = true;
   m_Solution.CopyTo(ppSolution);
}

void CM3CDoc::BuildConcreteModel(IStressStrain** ppConcrete) const
{
   CComPtr<ICircularManderSection> section;
   section.CoCreateInstance(CLSID_CircularManderSection);
   section->put_As(m_ProblemParams.As_per_bar * m_ProblemParams.nBars);
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
   model->put_R(m_ProblemParams.R);
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

void CM3CDoc::BuildColumnSection(IGeneralSection** ppSection) const
{
   CComQIPtr<IStressStrain> ss_concrete;
   BuildConcreteModel(&ss_concrete);

   CComPtr<IStressStrain> ss_rebar;
   BuildRebarModel(&ss_rebar);


   CComPtr<IGeneralSection> column_section;
   column_section.CoCreateInstance(CLSID_GeneralSection);
   CComPtr<ICircle> column;
   column.CoCreateInstance(CLSID_Circle);
   column->put_Radius(m_ProblemParams.D / 2);
   CComQIPtr<IShape> column_shape(column);
   column_section->AddShape(column_shape, ss_concrete, nullptr, 0.0);

   CComPtr<IGeomUtil2d> geom_util;
   geom_util.CoCreateInstance(CLSID_GeomUtil);
   CComPtr<IPoint2d> center;
   column->get_Center(&center);

   CComPtr<IPoint2dCollection> points;
   Float64 radius = (m_ProblemParams.D - m_ProblemParams.Cover) / 2;
   geom_util->GenerateCircle(m_ProblemParams.nBars, center, radius, 0.0, &points);

   IndexType nBars;
   points->get_Count(&nBars);
   for (IndexType i = 0; i < nBars; i++)
   {
      CComPtr<IPoint2d> pnt;
      points->get_Item(i, &pnt);

      CComPtr<IGenericShape> bar;
      bar.CoCreateInstance(CLSID_GenericShape);
      bar->put_Area(m_ProblemParams.As_per_bar);
      bar->putref_Centroid(pnt);

      CComQIPtr<IShape> bar_shape(bar);
      column_section->AddShape(bar_shape, ss_rebar, nullptr, 0.0);
   }

   column_section.CopyTo(ppSection);
}