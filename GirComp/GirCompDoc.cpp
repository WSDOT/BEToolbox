///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2023  Washington State Department of Transportation
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

// GirCompDoc.cpp : implementation file
//

#include "stdafx.h"
#include "..\resource.h"
#include "GirCompDoc.h"
#include "GirCompTitlePageBuilder.h"
#include "GirCompChapterBuilder.h"
#include "GirCompChildFrame.h"
#include "..\BEToolboxStatusBar.h"

#include <EAF\EAFUtilities.h>
#include <EAF\EAFApp.h>

#include <GeomModel/GenericShape.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


GIRCOMPDIMENSIONS::GIRCOMPDIMENSIONS()
{
   Type = General;

   Area = 0;
   MomentOfInertia = 0;

   D = 0;
   tWeb = 0;
   wTopFlange = 0;
   tTopFlange = 0;
   wBotFlange = 0;
   tBotFlange = 0;
   
   G = 0;
   tSlab = 0;
   wSlab = 0;
   N1 = 0;
   N2 = 0;
   N3 = 0;
}

// CGirCompDoc

IMPLEMENT_DYNCREATE(CGirCompDoc, CBEToolboxDoc)

CGirCompDoc::CGirCompDoc() : CBEToolboxDoc()
{
   std::shared_ptr<WBFL::Reporting::ReportBuilder> pRptBuilder(std::make_shared<WBFL::Reporting::ReportBuilder>(_T("GirComp")));
   GetReportManager()->AddReportBuilder(pRptBuilder);

   std::shared_ptr<WBFL::Reporting::TitlePageBuilder> pTitlePageBuilder(std::make_shared<CGirCompTitlePageBuilder>());
   pRptBuilder->AddTitlePageBuilder( pTitlePageBuilder );

   std::shared_ptr<WBFL::Reporting::ChapterBuilder> pChBuilder(std::make_shared<CGirCompChapterBuilder>(this) );
   pRptBuilder->AddChapterBuilder(pChBuilder);

   EnableUIHints(FALSE); // not using UIHints feature
}

CGirCompDoc::~CGirCompDoc()
{
}


BEGIN_MESSAGE_MAP(CGirCompDoc, CBEToolboxDoc)
   ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
END_MESSAGE_MAP()

void CGirCompDoc::OnHelpFinder()
{
   EAFHelp(EAFGetDocument()->GetDocumentationSetName(),IDH_GIRCOMP);
}


// CGirCompDoc diagnostics

#ifdef _DEBUG
void CGirCompDoc::AssertValid() const
{
	CBEToolboxDoc::AssertValid();
}

#ifndef _WIN32_WCE
void CGirCompDoc::Dump(CDumpContext& dc) const
{
	CBEToolboxDoc::Dump(dc);
}
#endif
#endif //_DEBUG

BOOL CGirCompDoc::Init()
{
   if ( !CBEToolboxDoc::Init() )
      return FALSE;

   InitRolledSections();

   // initialize with some data
   GIRCOMPDIMENSIONS problem;
   problem.Type = GIRCOMPDIMENSIONS::BuiltUp;
   problem.D = WBFL::Units::ConvertToSysUnits(122, WBFL::Units::Measure::Inch);
   problem.tWeb = WBFL::Units::ConvertToSysUnits(0.438, WBFL::Units::Measure::Inch);
   problem.wTopFlange = WBFL::Units::ConvertToSysUnits(22, WBFL::Units::Measure::Inch);
   problem.tTopFlange = WBFL::Units::ConvertToSysUnits(1.25, WBFL::Units::Measure::Inch);
   problem.wBotFlange = WBFL::Units::ConvertToSysUnits(22, WBFL::Units::Measure::Inch);
   problem.tBotFlange = WBFL::Units::ConvertToSysUnits(2.25, WBFL::Units::Measure::Inch);
   problem.wSlab = WBFL::Units::ConvertToSysUnits(84, WBFL::Units::Measure::Inch);
   problem.tSlab = WBFL::Units::ConvertToSysUnits(7, WBFL::Units::Measure::Inch);
   problem.G = WBFL::Units::ConvertToSysUnits(0.5, WBFL::Units::Measure::Inch);
   problem.N1 = 8;
   problem.N2 = 16;
   problem.N3 = 24;

   m_Problems.push_back(problem);

   return TRUE;
}

void CGirCompDoc::OnCloseDocument()
{
   EAFGetApp()->SetUnitsMode(eafTypes::umUS);

   CEAFDocument::OnCloseDocument();
}

void CGirCompDoc::LoadDocumentSettings()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   __super::LoadDocumentSettings();
}

void CGirCompDoc::SaveDocumentSettings()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   __super::SaveDocumentSettings();
}

CString CGirCompDoc::GetDocumentationRootLocation()
{
   CEAFApp* pApp = EAFGetApp();
   return pApp->GetDocumentationRootLocation();
}

HRESULT CGirCompDoc::WriteTheDocument(IStructuredSave* pStrSave)
{
   HRESULT hr = pStrSave->BeginUnit(_T("GirComp"),1.0);
   if ( FAILED(hr) )
      return hr;

   CEAFApp* pApp = EAFGetApp();

   hr = pStrSave->put_Property(_T("Units"),CComVariant(pApp->GetUnitsMode()));
   if ( FAILED(hr) )
      return hr;

   hr = pStrSave->put_Property(_T("ProblemCount"),CComVariant(GetProblemCount()));
   if ( FAILED(hr) )
      return hr;

   hr = pStrSave->BeginUnit(_T("Problems"),1.0);
   if ( FAILED(hr) )
      return hr;

   std::vector<GIRCOMPDIMENSIONS>::iterator iter(m_Problems.begin());
   std::vector<GIRCOMPDIMENSIONS>::iterator end(m_Problems.end());
   for ( ; iter != end; iter++ )
   {
      GIRCOMPDIMENSIONS& dimensions = *iter;

      hr = pStrSave->BeginUnit(_T("Dimensions"),1.0);
      if ( FAILED(hr) )
         return hr;

      switch(dimensions.Type)
      {
      case GIRCOMPDIMENSIONS::General:
         hr = pStrSave->put_Property(_T("ProblemType"),CComVariant(_T("General")));
         break;

      case GIRCOMPDIMENSIONS::Rolled:
         hr = pStrSave->put_Property(_T("ProblemType"),CComVariant(_T("Rolled")));
         break;

      case GIRCOMPDIMENSIONS::BuiltUp:
         hr = pStrSave->put_Property(_T("ProblemType"),CComVariant(_T("BuiltUp")));
         break;
      }
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("RolledSectionName"),CComVariant(dimensions.RolledSectionName.c_str()));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("Area"),CComVariant(dimensions.Area));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("MomentOfInertia"),CComVariant(dimensions.MomentOfInertia));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("D"),CComVariant(dimensions.D));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("tWeb"),CComVariant(dimensions.tWeb));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("wTopFlange"),CComVariant(dimensions.wTopFlange));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("tTopFlange"),CComVariant(dimensions.tTopFlange));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("wBotFlange"),CComVariant(dimensions.wBotFlange));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("tBotFlange"),CComVariant(dimensions.tBotFlange));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("G"),CComVariant(dimensions.G));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("tSlab"),CComVariant(dimensions.tSlab));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("wSlab"),CComVariant(dimensions.wSlab));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("N1"),CComVariant(dimensions.N1));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("N2"),CComVariant(dimensions.N2));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("N3"),CComVariant(dimensions.N3));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->EndUnit(); // Dimensions
      if ( FAILED(hr) )
         return hr;
   }

   hr = pStrSave->EndUnit(); // Problems
   if ( FAILED(hr) )
      return hr;

   hr = pStrSave->EndUnit(); // GirComp
   if ( FAILED(hr) )
      return hr;

   return S_OK;
}

HRESULT CGirCompDoc::LoadTheDocument(IStructuredLoad* pStrLoad)
{
   USES_CONVERSION;

   ClearProblems();

   HRESULT hr = pStrLoad->BeginUnit(_T("GirComp"));
   if ( FAILED(hr) )
      return hr;

   CComVariant var;

   CEAFApp* pApp = EAFGetApp();

   var.vt = VT_I4;
   hr = pStrLoad->get_Property(_T("Units"),&var);
   if ( FAILED(hr) )
      return hr;

   pApp->SetUnitsMode(eafTypes::UnitMode(var.lVal));

   var.vt = VT_INDEX;
   hr = pStrLoad->get_Property(_T("ProblemCount"),&var);
   if ( FAILED(hr) )
      return hr;

   IndexType nProblems = VARIANT2INDEX(var);

   hr = pStrLoad->BeginUnit(_T("Problems"));
   if ( FAILED(hr) )
      return hr;

   for ( IndexType idx = 0; idx < nProblems; idx++ )
   {
      GIRCOMPDIMENSIONS dimensions;

      hr = pStrLoad->BeginUnit(_T("Dimensions"));
      if ( FAILED(hr) )
         return hr;

      var.vt = VT_BSTR;
      hr = pStrLoad->get_Property(_T("ProblemType"),&var);
      if ( FAILED(hr) )
         return hr;

      std::_tstring strType(OLE2T(var.bstrVal));
      if ( strType == _T("General") )
         dimensions.Type = GIRCOMPDIMENSIONS::General;
      else if ( strType == _T("Rolled") )
         dimensions.Type = GIRCOMPDIMENSIONS::Rolled;
      else if ( strType == _T("BuiltUp") )
         dimensions.Type = GIRCOMPDIMENSIONS::BuiltUp;
      else
         ATLASSERT(false);

      var.vt = VT_BSTR;
      hr = pStrLoad->get_Property(_T("RolledSectionName"),&var);
      if ( FAILED(hr) )
         return hr;
      dimensions.RolledSectionName = OLE2T(var.bstrVal);

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("Area"),&var);
      if ( FAILED(hr) )
         return hr;
      dimensions.Area = var.dblVal;

      hr = pStrLoad->get_Property(_T("MomentOfInertia"),&var);
      if ( FAILED(hr) )
         return hr;
      dimensions.MomentOfInertia = var.dblVal;

      hr = pStrLoad->get_Property(_T("D"),&var);
      if ( FAILED(hr) )
         return hr;
      dimensions.D = var.dblVal;

      hr = pStrLoad->get_Property(_T("tWeb"),&var);
      if ( FAILED(hr) )
         return hr;
      dimensions.tWeb = var.dblVal;

      hr = pStrLoad->get_Property(_T("wTopFlange"),&var);
      if ( FAILED(hr) )
         return hr;
      dimensions.wTopFlange = var.dblVal;

      hr = pStrLoad->get_Property(_T("tTopFlange"),&var);
      if ( FAILED(hr) )
         return hr;
      dimensions.tTopFlange = var.dblVal;

      hr = pStrLoad->get_Property(_T("wBotFlange"),&var);
      if ( FAILED(hr) )
         return hr;
      dimensions.wBotFlange = var.dblVal;

      hr = pStrLoad->get_Property(_T("tBotFlange"),&var);
      if ( FAILED(hr) )
         return hr;
      dimensions.tBotFlange = var.dblVal;

      hr = pStrLoad->get_Property(_T("G"),&var);
      if ( FAILED(hr) )
         return hr;
      dimensions.G = var.dblVal;

      hr = pStrLoad->get_Property(_T("tSlab"),&var);
      if ( FAILED(hr) )
         return hr;
      dimensions.tSlab = var.dblVal;

      hr = pStrLoad->get_Property(_T("wSlab"),&var);
      if ( FAILED(hr) )
         return hr;
      dimensions.wSlab = var.dblVal;

      hr = pStrLoad->get_Property(_T("N1"),&var);
      if ( FAILED(hr) )
         return hr;
      dimensions.N1 = var.dblVal;

      hr = pStrLoad->get_Property(_T("N2"),&var);
      if ( FAILED(hr) )
         return hr;
      dimensions.N2 = var.dblVal;

      hr = pStrLoad->get_Property(_T("N3"),&var);
      if ( FAILED(hr) )
         return hr;
      dimensions.N3 = var.dblVal;

      m_Problems.push_back(dimensions);

      hr = pStrLoad->EndUnit(); // Dimensions
      if ( FAILED(hr) )
         return hr;
   }

   hr = pStrLoad->EndUnit(); // Problems
   if ( FAILED(hr) )
      return hr;

   hr = pStrLoad->EndUnit(); // GirComp
   if ( FAILED(hr) )
      return hr;

   return S_OK;
}

CString CGirCompDoc::GetToolbarSectionName()
{
   return _T("GirComp");
}

void CGirCompDoc::OnOldFormat(LPCTSTR lpszPathName)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   AfxMessageBox(_T("Error: Invalid file format.\n\nThis is not a GirComp file or it is in a legacy format that is not supported"),MB_OK | MB_ICONEXCLAMATION);
}

IndexType CGirCompDoc::GetProblemCount() const
{
   return m_Problems.size();
}

void CGirCompDoc::AddProblem(const GIRCOMPDIMENSIONS& problem)
{
   m_Problems.push_back(problem);
}

void CGirCompDoc::AddProblems(const std::vector<GIRCOMPDIMENSIONS>& problems)
{
   m_Problems.insert(m_Problems.end(),problems.begin(),problems.end());
}

void CGirCompDoc::RemoveProblem(IndexType idx)
{
   m_Problems.erase(m_Problems.begin() + idx);
}

const GIRCOMPDIMENSIONS& CGirCompDoc::GetProblem(IndexType idx) const
{
   return m_Problems[idx];
}

void CGirCompDoc::ClearProblems()
{
   m_Problems.clear();
}

WBFL::Geometry::PlateGirder CGirCompDoc::GetBuiltUpGirder(IndexType idx) const
{
   const GIRCOMPDIMENSIONS& dimensions = GetProblem(idx);
   ATLASSERT(dimensions.Type == GIRCOMPDIMENSIONS::BuiltUp);

   WBFL::Geometry::PlateGirder girder;

   girder.SetTopFlangeWidth(dimensions.wTopFlange);
   girder.SetTopFlangeThickness(dimensions.tTopFlange);

   girder.SetBottomFlangeWidth(dimensions.wBotFlange);
   girder.SetBottomFlangeThickness(dimensions.tBotFlange);

   girder.SetWebHeight(dimensions.D);
   girder.SetWebWidth(dimensions.tWeb);

   return girder;
}

WBFL::Geometry::CompositeBeam CGirCompDoc::GetCompositeBeam(IndexType idx,int n) const
{
   const GIRCOMPDIMENSIONS& dimensions = GetProblem(idx);

   Float64 N;
   if ( n == 0 )
      N = dimensions.N1;
   else if ( n == 1 )
      N = dimensions.N2;
   else if ( n == 2 )
      N = dimensions.N3;

   WBFL::Geometry::CompositeBeam compBeam;

   compBeam.SetHaunchDepth(dimensions.G);
   compBeam.SetGrossSlabDepth(dimensions.tSlab);
   compBeam.SetTributarySlabWidth(dimensions.wSlab);
   compBeam.SetEffectiveSlabWidth(dimensions.wSlab);

   compBeam.SetBeamE(N);
   compBeam.SetSlabE(1.0);

   if ( dimensions.Type == GIRCOMPDIMENSIONS::Rolled || dimensions.Type == GIRCOMPDIMENSIONS::General )
   {
      WBFL::Geometry::GenericShape genericShape;

      genericShape.SetArea(dimensions.Area);
      genericShape.SetIxx(dimensions.MomentOfInertia);
      
      auto& centroid = genericShape.GetCentroid();
      centroid->Move(0,dimensions.D/2);
      genericShape.SetCentroid(centroid);

      genericShape.SetYtop(dimensions.D/2);
      genericShape.SetYbottom(dimensions.D/2);
      genericShape.SetXleft(dimensions.wTopFlange/2);
      genericShape.SetXright(dimensions.wTopFlange/2);

      compBeam.SetHaunchWidth(dimensions.wTopFlange);

      compBeam.SetBeam(genericShape);
   }
   else if ( dimensions.Type == GIRCOMPDIMENSIONS::BuiltUp )
   {
      auto girder = GetBuiltUpGirder(idx);

      Float64 wTF = girder.GetTopFlangeWidth();
      compBeam.SetHaunchWidth(wTF);

      compBeam.SetBeam(girder);
   }
   else
   {
      ATLASSERT(false); // should never get here
   }

   return compBeam;
}

const std::vector<ROLLEDSECTION>& CGirCompDoc::GetRolledSections() const
{
   return m_RolledSections;
}

void CGirCompDoc::InitRolledSections()
{
   m_RolledSections.reserve(275);
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W44X335"),WBFL::Units::ConvertToSysUnits(98.5,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(44.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(31100,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W44X290"),WBFL::Units::ConvertToSysUnits(85.4,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(43.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(27000,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W44X262"),WBFL::Units::ConvertToSysUnits(77.2,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(43.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(24100,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W44X230"),WBFL::Units::ConvertToSysUnits(67.8,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(42.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(20800,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W40X593"),WBFL::Units::ConvertToSysUnits(174,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(43.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(50400,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W40X503"),WBFL::Units::ConvertToSysUnits(148,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(42.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(41600,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W40X431"),WBFL::Units::ConvertToSysUnits(127,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(41.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(34800,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W40X397"),WBFL::Units::ConvertToSysUnits(117,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(41.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(32000,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W40X372"),WBFL::Units::ConvertToSysUnits(110,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(40.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(29600,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W40X362"),WBFL::Units::ConvertToSysUnits(106,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(40.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(28900,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W40X324"),WBFL::Units::ConvertToSysUnits(95.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(40.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(25600,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W40X297"),WBFL::Units::ConvertToSysUnits(87.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(39.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(23200,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W40X277"),WBFL::Units::ConvertToSysUnits(81.5,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(39.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(21900,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W40X249"),WBFL::Units::ConvertToSysUnits(73.5,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(39.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(19600,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W40X215"),WBFL::Units::ConvertToSysUnits(63.5,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(39.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16700,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W40X199"),WBFL::Units::ConvertToSysUnits(58.8,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(38.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(14900,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W40X392"),WBFL::Units::ConvertToSysUnits(116,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(41.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(29900,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W40X331"),WBFL::Units::ConvertToSysUnits(97.7,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(40.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(24700,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W40X327"),WBFL::Units::ConvertToSysUnits(95.9,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(40.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(24500,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W40X294"),WBFL::Units::ConvertToSysUnits(86.2,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(40.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(21900,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W40X278"),WBFL::Units::ConvertToSysUnits(82.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(40.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(20500,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W40X264"),WBFL::Units::ConvertToSysUnits(77.4,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(40.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(19400,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W40X235"),WBFL::Units::ConvertToSysUnits(69.1,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(39.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(17400,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W40X211"),WBFL::Units::ConvertToSysUnits(62.1,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(39.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15500,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W40X183"),WBFL::Units::ConvertToSysUnits(53.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(39.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(13200,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W40X167"),WBFL::Units::ConvertToSysUnits(49.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(38.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11600,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W40X149"),WBFL::Units::ConvertToSysUnits(43.8,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(38.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(9800,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W36X652"),WBFL::Units::ConvertToSysUnits(192,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(41.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(17.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(50600,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W36X529"),WBFL::Units::ConvertToSysUnits(156,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(39.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(17.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(39600,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W36X487"),WBFL::Units::ConvertToSysUnits(143,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(39.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(17.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(36000,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W36X441"),WBFL::Units::ConvertToSysUnits(130,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(38.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(17.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(32100,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W36X395"),WBFL::Units::ConvertToSysUnits(116,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(38.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(28500,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W36X361"),WBFL::Units::ConvertToSysUnits(106,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(38.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(25700,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W36X330"),WBFL::Units::ConvertToSysUnits(96.9,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(37.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(23300,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W36X302"),WBFL::Units::ConvertToSysUnits(89.0,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(37.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(21100,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W36X282"),WBFL::Units::ConvertToSysUnits(82.9,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(37.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(19600,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W36X262"),WBFL::Units::ConvertToSysUnits(77.2,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(36.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(17900,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W36X247"),WBFL::Units::ConvertToSysUnits(72.5,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(36.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16700,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W36X231"),WBFL::Units::ConvertToSysUnits(68.2,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(36.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15600,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W36X256"),WBFL::Units::ConvertToSysUnits(75.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(37.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16800,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W36X232"),WBFL::Units::ConvertToSysUnits(68.0,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(37.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15000,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W36X210"),WBFL::Units::ConvertToSysUnits(61.9,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(36.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(13200,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W36X194"),WBFL::Units::ConvertToSysUnits(57.0,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(36.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12100,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W36X182"),WBFL::Units::ConvertToSysUnits(53.6,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(36.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11300,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W36X170"),WBFL::Units::ConvertToSysUnits(50.0,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(36.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10500,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W36X160"),WBFL::Units::ConvertToSysUnits(47.0,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(36.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(9760,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W36X150"),WBFL::Units::ConvertToSysUnits(44.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(35.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(9040,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W36X135"),WBFL::Units::ConvertToSysUnits(39.9,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(35.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(7800,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W33X387"),WBFL::Units::ConvertToSysUnits(114,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(36.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(24300,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W33X354"),WBFL::Units::ConvertToSysUnits(104,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(35.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(22000,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W33X318"),WBFL::Units::ConvertToSysUnits(93.7,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(35.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(19500,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W33X291"),WBFL::Units::ConvertToSysUnits(85.6,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(34.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(17700,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W33X263"),WBFL::Units::ConvertToSysUnits(77.4,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(34.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15900,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W33X241"),WBFL::Units::ConvertToSysUnits(71.1,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(34.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(14200,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W33X221"),WBFL::Units::ConvertToSysUnits(65.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(33.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12900,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W33X201"),WBFL::Units::ConvertToSysUnits(59.1,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(33.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11600,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W33X169"),WBFL::Units::ConvertToSysUnits(49.5,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(33.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(9290,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W33X152"),WBFL::Units::ConvertToSysUnits(44.9,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(33.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(8160,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W33X141"),WBFL::Units::ConvertToSysUnits(41.5,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(33.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(7450,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W33X130"),WBFL::Units::ConvertToSysUnits(38.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(33.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(6710,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W33X118"),WBFL::Units::ConvertToSysUnits(34.7,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(32.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(5900,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W30X391"),WBFL::Units::ConvertToSysUnits(115,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(33.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(20700,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W30X357"),WBFL::Units::ConvertToSysUnits(105,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(32.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(18700,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W30X326"),WBFL::Units::ConvertToSysUnits(95.9,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(32.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16800,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W30X292"),WBFL::Units::ConvertToSysUnits(86.0,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(32.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(14900,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W30X261"),WBFL::Units::ConvertToSysUnits(77.0,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(31.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(13100,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W30X235"),WBFL::Units::ConvertToSysUnits(69.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(31.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11700,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W30X211"),WBFL::Units::ConvertToSysUnits(62.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(30.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10300,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W30X191"),WBFL::Units::ConvertToSysUnits(56.1,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(30.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(9200,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W30X173"),WBFL::Units::ConvertToSysUnits(50.9,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(30.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(8230,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W30X148"),WBFL::Units::ConvertToSysUnits(43.6,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(30.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(6680,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W30X132"),WBFL::Units::ConvertToSysUnits(38.8,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(30.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(5770,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W30X124"),WBFL::Units::ConvertToSysUnits(36.5,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(30.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(5360,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W30X116"),WBFL::Units::ConvertToSysUnits(34.2,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(30.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(4930,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W30X108"),WBFL::Units::ConvertToSysUnits(31.7,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(29.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(4470,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W30X99"),WBFL::Units::ConvertToSysUnits(29.0,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(29.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(3990,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W30X90"),WBFL::Units::ConvertToSysUnits(26.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(29.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(3610,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W27X539"),WBFL::Units::ConvertToSysUnits(159,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(32.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(25600,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W27X368"),WBFL::Units::ConvertToSysUnits(109,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(30.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(14.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16200,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W27X336"),WBFL::Units::ConvertToSysUnits(99.2,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(30.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(14.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(14600,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W27X307"),WBFL::Units::ConvertToSysUnits(90.2,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(29.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(14.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(13100,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W27X281"),WBFL::Units::ConvertToSysUnits(83.1,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(29.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(14.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11900,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W27X258"),WBFL::Units::ConvertToSysUnits(76.1,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(29.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(14.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10800,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W27X235"),WBFL::Units::ConvertToSysUnits(69.4,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(28.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(14.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(9700,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W27X217"),WBFL::Units::ConvertToSysUnits(63.9,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(28.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(14.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(8910,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W27X194"),WBFL::Units::ConvertToSysUnits(57.1,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(28.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(14.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(7860,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W27X178"),WBFL::Units::ConvertToSysUnits(52.5,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(27.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(14.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(7020,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W27X161"),WBFL::Units::ConvertToSysUnits(47.6,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(27.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(14.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(6310,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W27X146"),WBFL::Units::ConvertToSysUnits(43.2,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(27.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(14.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(5660,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W27X129"),WBFL::Units::ConvertToSysUnits(37.8,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(27.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(4760,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W27X114"),WBFL::Units::ConvertToSysUnits(33.6,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(27.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(4080,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W27X102"),WBFL::Units::ConvertToSysUnits(30.0,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(27.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(3620,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W27X94"),WBFL::Units::ConvertToSysUnits(27.6,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(26.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(3270,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W27X84"),WBFL::Units::ConvertToSysUnits(24.7,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(26.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(2850,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W24X370"),WBFL::Units::ConvertToSysUnits(109,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(28.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(13.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(13400,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W24X335"),WBFL::Units::ConvertToSysUnits(98.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(27.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(13.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11900,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W24X306"),WBFL::Units::ConvertToSysUnits(89.7,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(27.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(13.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10700,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W24X279"),WBFL::Units::ConvertToSysUnits(81.9,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(26.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(13.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(9600,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W24X250"),WBFL::Units::ConvertToSysUnits(73.5,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(26.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(13.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(8490,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W24X229"),WBFL::Units::ConvertToSysUnits(67.2,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(26.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(13.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(7650,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W24X207"),WBFL::Units::ConvertToSysUnits(60.7,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(25.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(13.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(6820,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W24X192"),WBFL::Units::ConvertToSysUnits(56.5,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(25.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(13.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(6260,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W24X176"),WBFL::Units::ConvertToSysUnits(51.7,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(25.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(5680,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W24X162"),WBFL::Units::ConvertToSysUnits(47.8,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(25.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(13.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(5170,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W24X146"),WBFL::Units::ConvertToSysUnits(43.0,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(24.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(4580,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W24X131"),WBFL::Units::ConvertToSysUnits(38.6,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(24.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(4020,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W24X117"),WBFL::Units::ConvertToSysUnits(34.4,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(24.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(3540,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W24X104"),WBFL::Units::ConvertToSysUnits(30.7,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(24.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(3100,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W24X103"),WBFL::Units::ConvertToSysUnits(30.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(24.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(9.00,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(3000,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W24X94"),WBFL::Units::ConvertToSysUnits(27.7,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(24.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(9.07,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(2700,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W24X84"),WBFL::Units::ConvertToSysUnits(24.7,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(24.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(9.02,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(2370,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W24X76"),WBFL::Units::ConvertToSysUnits(22.4,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(23.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(8.99,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(2100,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W24X68"),WBFL::Units::ConvertToSysUnits(20.1,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(23.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(8.97,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(1830,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W24X62"),WBFL::Units::ConvertToSysUnits(18.2,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(23.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(7.04,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(1550,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W24X55"),WBFL::Units::ConvertToSysUnits(16.2,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(23.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(7.01,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(1350,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W21X201"),WBFL::Units::ConvertToSysUnits(59.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(23.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(5310,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W21X182"),WBFL::Units::ConvertToSysUnits(53.6,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(22.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(4730,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W21X166"),WBFL::Units::ConvertToSysUnits(48.8,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(22.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(4280,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W21X147"),WBFL::Units::ConvertToSysUnits(43.2,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(22.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(3630,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W21X132"),WBFL::Units::ConvertToSysUnits(38.8,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(21.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(3220,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W21X122"),WBFL::Units::ConvertToSysUnits(35.9,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(21.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(2960,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W21X111"),WBFL::Units::ConvertToSysUnits(32.6,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(21.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(2670,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W21X101"),WBFL::Units::ConvertToSysUnits(29.8,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(21.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(2420,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W21X93"),WBFL::Units::ConvertToSysUnits(27.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(21.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(8.42,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(2070,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W21X83"),WBFL::Units::ConvertToSysUnits(24.4,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(21.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(8.36,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(1830,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W21X73"),WBFL::Units::ConvertToSysUnits(21.5,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(21.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(8.30,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(1600,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W21X68"),WBFL::Units::ConvertToSysUnits(20.0,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(21.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(8.27,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(1480,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W21X62"),WBFL::Units::ConvertToSysUnits(18.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(21.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(8.24,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(1330,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W21X55"),WBFL::Units::ConvertToSysUnits(16.2,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(20.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(8.22,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(1140,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W21X48"),WBFL::Units::ConvertToSysUnits(14.1,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(20.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(8.14,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(959,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W21X57"),WBFL::Units::ConvertToSysUnits(16.7,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(21.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(6.56,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(1170,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W21X50"),WBFL::Units::ConvertToSysUnits(14.7,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(20.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(6.53,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(984,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W21X44"),WBFL::Units::ConvertToSysUnits(13.0,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(20.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(6.50,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(843,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W18X311"),WBFL::Units::ConvertToSysUnits(91.6,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(22.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(6970,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W18X283"),WBFL::Units::ConvertToSysUnits(83.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(21.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(6170,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W18X258"),WBFL::Units::ConvertToSysUnits(76.0,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(21.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(5510,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W18X234"),WBFL::Units::ConvertToSysUnits(68.6,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(21.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(4900,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W18X211"),WBFL::Units::ConvertToSysUnits(62.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(20.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(4330,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W18X192"),WBFL::Units::ConvertToSysUnits(56.2,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(20.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(3870,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W18X175"),WBFL::Units::ConvertToSysUnits(51.4,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(20.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(3450,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W18X158"),WBFL::Units::ConvertToSysUnits(46.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(19.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(3060,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W18X143"),WBFL::Units::ConvertToSysUnits(42.0,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(19.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(2750,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W18X130"),WBFL::Units::ConvertToSysUnits(38.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(19.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(2460,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W18X119"),WBFL::Units::ConvertToSysUnits(35.1,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(19.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(2190,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W18X106"),WBFL::Units::ConvertToSysUnits(31.1,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(18.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(1910,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W18X97"),WBFL::Units::ConvertToSysUnits(28.5,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(18.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(1750,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W18X86"),WBFL::Units::ConvertToSysUnits(25.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(18.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(1530,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W18X76"),WBFL::Units::ConvertToSysUnits(22.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(18.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(1330,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W18X71"),WBFL::Units::ConvertToSysUnits(20.9,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(18.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(7.64,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(1170,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W18X65"),WBFL::Units::ConvertToSysUnits(19.1,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(18.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(7.59,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(1070,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W18X60"),WBFL::Units::ConvertToSysUnits(17.6,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(18.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(7.56,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(984,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W18X55"),WBFL::Units::ConvertToSysUnits(16.2,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(18.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(7.53,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(890,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W18X50"),WBFL::Units::ConvertToSysUnits(14.7,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(18.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(7.50,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(800,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W18X46"),WBFL::Units::ConvertToSysUnits(13.5,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(18.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(6.06,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(712,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W18X40"),WBFL::Units::ConvertToSysUnits(11.8,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(17.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(6.02,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(612,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W18X35"),WBFL::Units::ConvertToSysUnits(10.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(17.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(6.00,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(510,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W16X100"),WBFL::Units::ConvertToSysUnits(29.4,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(17.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(1490,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W16X89"),WBFL::Units::ConvertToSysUnits(26.2,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(16.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(1300,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W16X77"),WBFL::Units::ConvertToSysUnits(22.6,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(16.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(1110,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W16X67"),WBFL::Units::ConvertToSysUnits(19.6,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(16.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(954,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W16X57"),WBFL::Units::ConvertToSysUnits(16.8,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(16.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(7.12,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(758,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W16X50"),WBFL::Units::ConvertToSysUnits(14.7,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(16.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(7.07,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(659,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W16X45"),WBFL::Units::ConvertToSysUnits(13.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(16.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(7.04,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(586,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W16X40"),WBFL::Units::ConvertToSysUnits(11.8,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(16.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(7.00,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(518,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W16X36"),WBFL::Units::ConvertToSysUnits(10.6,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(15.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(6.99,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(448,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W16X31"),WBFL::Units::ConvertToSysUnits(9.13,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(15.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(5.53,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(375,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W16X26"),WBFL::Units::ConvertToSysUnits(7.68,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(15.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(5.50,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(301,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X730"),WBFL::Units::ConvertToSysUnits(215,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(22.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(17.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(14300,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X665"),WBFL::Units::ConvertToSysUnits(196,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(21.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(17.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12400,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X605"),WBFL::Units::ConvertToSysUnits(178,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(20.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(17.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10800,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X550"),WBFL::Units::ConvertToSysUnits(162,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(20.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(17.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(9430,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X500"),WBFL::Units::ConvertToSysUnits(147,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(19.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(17.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(8210,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X455"),WBFL::Units::ConvertToSysUnits(134,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(19.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(7190,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X426"),WBFL::Units::ConvertToSysUnits(125,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(18.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(6600,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X398"),WBFL::Units::ConvertToSysUnits(117,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(18.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(6000,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X370"),WBFL::Units::ConvertToSysUnits(109,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(17.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(5440,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X342"),WBFL::Units::ConvertToSysUnits(101,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(17.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(4900,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X311"),WBFL::Units::ConvertToSysUnits(91.4,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(17.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(4330,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X283"),WBFL::Units::ConvertToSysUnits(83.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(16.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(3840,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X257"),WBFL::Units::ConvertToSysUnits(75.6,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(16.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(3400,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X233"),WBFL::Units::ConvertToSysUnits(68.5,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(16.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(3010,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X211"),WBFL::Units::ConvertToSysUnits(62.0,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(15.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(2660,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X193"),WBFL::Units::ConvertToSysUnits(56.8,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(15.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(2400,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X176"),WBFL::Units::ConvertToSysUnits(51.8,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(15.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(2140,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X159"),WBFL::Units::ConvertToSysUnits(46.7,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(15.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(1900,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X145"),WBFL::Units::ConvertToSysUnits(42.7,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(14.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(15.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(1710,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X132"),WBFL::Units::ConvertToSysUnits(38.8,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(14.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(14.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(1530,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X120"),WBFL::Units::ConvertToSysUnits(35.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(14.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(14.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(1380,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X109"),WBFL::Units::ConvertToSysUnits(32.0,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(14.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(14.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(1240,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X99"),WBFL::Units::ConvertToSysUnits(29.1,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(14.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(14.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(1110,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X90"),WBFL::Units::ConvertToSysUnits(26.5,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(14.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(14.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(999,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X82"),WBFL::Units::ConvertToSysUnits(24.0,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(14.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(881,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X74"),WBFL::Units::ConvertToSysUnits(21.8,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(14.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(795,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X68"),WBFL::Units::ConvertToSysUnits(20.0,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(14.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(722,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X61"),WBFL::Units::ConvertToSysUnits(17.9,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(13.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(640,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X53"),WBFL::Units::ConvertToSysUnits(15.6,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(13.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(8.06,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(541,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X48"),WBFL::Units::ConvertToSysUnits(14.1,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(13.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(8.03,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(484,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X43"),WBFL::Units::ConvertToSysUnits(12.6,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(13.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(8.00,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(428,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X38"),WBFL::Units::ConvertToSysUnits(11.2,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(14.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(6.77,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(385,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X34"),WBFL::Units::ConvertToSysUnits(10.0,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(14.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(6.75,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(340,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X30"),WBFL::Units::ConvertToSysUnits(8.85,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(13.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(6.73,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(291,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X26"),WBFL::Units::ConvertToSysUnits(7.69,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(13.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(5.03,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(245,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W14X22"),WBFL::Units::ConvertToSysUnits(6.49,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(13.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(5.00,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(199,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W12X336"),WBFL::Units::ConvertToSysUnits(98.9,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(16.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(13.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(4060,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W12X305"),WBFL::Units::ConvertToSysUnits(89.5,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(16.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(13.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(3550,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W12X279"),WBFL::Units::ConvertToSysUnits(81.9,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(15.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(13.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(3110,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W12X252"),WBFL::Units::ConvertToSysUnits(74.1,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(15.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(13.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(2720,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W12X230"),WBFL::Units::ConvertToSysUnits(67.7,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(15.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(2420,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W12X210"),WBFL::Units::ConvertToSysUnits(61.8,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(14.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(2140,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W12X190"),WBFL::Units::ConvertToSysUnits(56.0,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(14.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(1890,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W12X170"),WBFL::Units::ConvertToSysUnits(50.0,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(14.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(1650,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W12X152"),WBFL::Units::ConvertToSysUnits(44.7,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(13.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(1430,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W12X136"),WBFL::Units::ConvertToSysUnits(39.9,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(13.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(1240,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W12X120"),WBFL::Units::ConvertToSysUnits(35.2,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(13.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(1070,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W12X106"),WBFL::Units::ConvertToSysUnits(31.2,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(12.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(933,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W12X96"),WBFL::Units::ConvertToSysUnits(28.2,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(12.7,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(833,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W12X87"),WBFL::Units::ConvertToSysUnits(25.6,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(12.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(740,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W12X79"),WBFL::Units::ConvertToSysUnits(23.2,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(12.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(662,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W12X72"),WBFL::Units::ConvertToSysUnits(21.1,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(12.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(597,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W12X65"),WBFL::Units::ConvertToSysUnits(19.1,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(12.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(12.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(533,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W12X58"),WBFL::Units::ConvertToSysUnits(17.0,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(12.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(475,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W12X53"),WBFL::Units::ConvertToSysUnits(15.6,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(12.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(425,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W12X50"),WBFL::Units::ConvertToSysUnits(14.6,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(12.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(8.08,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(391,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W12X45"),WBFL::Units::ConvertToSysUnits(13.1,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(12.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(8.05,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(348,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W12X40"),WBFL::Units::ConvertToSysUnits(11.7,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(11.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(8.01,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(307,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W12X35"),WBFL::Units::ConvertToSysUnits(10.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(12.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(6.56,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(285,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W12X30"),WBFL::Units::ConvertToSysUnits(8.79,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(12.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(6.52,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(238,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W12X26"),WBFL::Units::ConvertToSysUnits(7.65,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(12.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(6.49,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(204,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W12X22"),WBFL::Units::ConvertToSysUnits(6.48,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(12.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(4.03,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(156,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W12X19"),WBFL::Units::ConvertToSysUnits(5.57,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(12.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(4.01,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(130,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W12X16"),WBFL::Units::ConvertToSysUnits(4.71,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(12.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(3.99,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(103,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W12X14"),WBFL::Units::ConvertToSysUnits(4.16,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(11.9,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(3.97,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(88.6,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W10X112"),WBFL::Units::ConvertToSysUnits(32.9,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(11.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(716,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W10X100"),WBFL::Units::ConvertToSysUnits(29.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(11.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(623,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W10X88"),WBFL::Units::ConvertToSysUnits(26.0,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(10.8,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(534,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W10X77"),WBFL::Units::ConvertToSysUnits(22.7,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(10.6,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(455,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W10X68"),WBFL::Units::ConvertToSysUnits(19.9,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(10.4,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(394,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W10X60"),WBFL::Units::ConvertToSysUnits(17.7,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(10.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(341,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W10X54"),WBFL::Units::ConvertToSysUnits(15.8,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(10.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(303,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W10X49"),WBFL::Units::ConvertToSysUnits(14.4,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(10.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(10.0,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(272,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W10X45"),WBFL::Units::ConvertToSysUnits(13.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(10.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(8.02,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(248,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W10X39"),WBFL::Units::ConvertToSysUnits(11.5,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(9.92,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(7.99,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(209,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W10X33"),WBFL::Units::ConvertToSysUnits(9.71,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(9.73,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(7.96,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(171,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W10X30"),WBFL::Units::ConvertToSysUnits(8.84,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(10.5,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(5.81,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(170,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W10X26"),WBFL::Units::ConvertToSysUnits(7.61,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(10.3,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(5.77,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(144,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W10X22"),WBFL::Units::ConvertToSysUnits(6.49,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(10.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(5.75,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(118,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W10X19"),WBFL::Units::ConvertToSysUnits(5.62,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(10.2,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(4.02,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(96.3,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W10X17"),WBFL::Units::ConvertToSysUnits(4.99,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(10.1,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(4.01,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(81.9,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W10X15"),WBFL::Units::ConvertToSysUnits(4.41,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(9.99,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(4.00,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(68.9,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W10X12"),WBFL::Units::ConvertToSysUnits(3.54,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(9.87,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(3.96,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(53.8,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W8X67"),WBFL::Units::ConvertToSysUnits(19.7,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(9.00,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(8.28,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(272,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W8X58"),WBFL::Units::ConvertToSysUnits(17.1,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(8.75,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(8.22,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(228,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W8X48"),WBFL::Units::ConvertToSysUnits(14.1,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(8.50,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(8.11,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(184,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W8X40"),WBFL::Units::ConvertToSysUnits(11.7,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(8.25,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(8.07,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(146,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W8X35"),WBFL::Units::ConvertToSysUnits(10.3,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(8.12,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(8.02,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(127,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W8X31"),WBFL::Units::ConvertToSysUnits(9.13,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(8.00,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(8.00,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(110,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W8X28"),WBFL::Units::ConvertToSysUnits(8.25,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(8.06,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(6.54,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(98.0,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W8X24"),WBFL::Units::ConvertToSysUnits(7.08,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(7.93,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(6.50,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(82.7,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W8X21"),WBFL::Units::ConvertToSysUnits(6.16,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(8.28,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(5.27,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(75.3,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W8X18"),WBFL::Units::ConvertToSysUnits(5.26,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(8.14,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(5.25,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(61.9,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W8X15"),WBFL::Units::ConvertToSysUnits(4.44,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(8.11,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(4.02,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(48.0,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W8X13"),WBFL::Units::ConvertToSysUnits(3.84,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(7.99,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(4.00,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(39.6,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W8X10"),WBFL::Units::ConvertToSysUnits(2.96,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(7.89,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(3.94,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(30.8,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W6X25"),WBFL::Units::ConvertToSysUnits(7.34,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(6.38,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(6.08,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(53.4,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W6X20"),WBFL::Units::ConvertToSysUnits(5.87,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(6.20,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(6.02,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(41.4,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W6X15"),WBFL::Units::ConvertToSysUnits(4.43,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(5.99,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(5.99,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(29.1,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W6X16"),WBFL::Units::ConvertToSysUnits(4.74,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(6.28,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(4.03,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(32.1,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W6X12"),WBFL::Units::ConvertToSysUnits(3.55,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(6.03,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(4.00,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(22.1,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W6X9"),WBFL::Units::ConvertToSysUnits(2.68,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(5.90,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(3.94,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(16.4,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W6X8.5"),WBFL::Units::ConvertToSysUnits(2.52,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(5.83,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(3.94,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(14.9,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W5X19"),WBFL::Units::ConvertToSysUnits(5.56,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(5.15,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(5.03,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(26.3,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W5X16"),WBFL::Units::ConvertToSysUnits(4.71,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(5.01,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(5.00,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(21.4,WBFL::Units::Measure::Inch4)));
   m_RolledSections.emplace_back(ROLLEDSECTION(_T("W4X13"),WBFL::Units::ConvertToSysUnits(3.83,WBFL::Units::Measure::Inch2),WBFL::Units::ConvertToSysUnits(4.16,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(4.06,WBFL::Units::Measure::Inch),WBFL::Units::ConvertToSysUnits(11.3,WBFL::Units::Measure::Inch4)));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("M12.5X12.4"),3.63,12.5,3.75,89.3));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("M12.5X11.6"),3.40,12.5,3.50,80.3));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("M12X11.8"),3.47,12.0,3.07,72.2));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("M12X10.8"),3.18,12.0,3.07,66.7));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("M12X10"),2.95,12.0,3.25,61.7));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("M10X9"),2.65,10.0,2.69,39.0));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("M10X8"),2.37,9.95,2.69,34.6));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("M10X7.5"),2.22,9.99,2.69,33.0));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("M8X6.5"),1.92,8.00,2.28,18.5));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("M8X6.2"),1.82,8.00,2.28,17.6));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("M6X4.4"),1.29,6.00,1.84,7.23));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("M6X3.7"),1.09,5.92,2.00,5.96));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("M5X18.9"),5.56,5.00,5.00,24.2));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("M4X6"),1.75,3.80,3.80,4.72));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("M4X4.08"),1.27,4.00,2.25,3.53));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("M4X3.45"),1.01,4.00,2.25,2.86));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("M4X3.2"),1.01,4.00,2.25,2.86));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("M3X2.9"),0.914,3.00,2.25,1.50));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("HP18X204"),60.2,18.3,18.1,3480));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("HP18X181"),53.2,18.0,18.0,3020));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("HP18X157"),46.2,17.7,17.9,2570));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("HP18X135"),39.9,17.5,17.8,2200));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("HP16X183"),54.1,16.5,16.3,2510));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("HP16X162"),47.7,16.3,16.1,2190));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("HP16X141"),41.7,16.0,16.0,1870));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("HP16X121"),35.8,15.8,15.9,1590));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("HP16X101"),29.9,15.5,15.8,1300));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("HP16X88"),25.8,15.3,15.7,1110));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("HP14X117"),34.4,14.2,14.9,1220));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("HP14X102"),30.1,14.0,14.8,1050));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("HP14X89"),26.1,13.8,14.7,904));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("HP14X73"),21.4,13.6,14.6,729));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("HP12X84"),24.6,12.3,12.3,650));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("HP12X74"),21.8,12.1,12.2,569));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("HP12X63"),18.4,11.9,12.1,472));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("HP12X53"),15.5,11.8,12.0,393));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("HP10X57"),16.7,9.99,10.2,294));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("HP10X42"),12.4,9.70,10.1,210));
   //m_RolledSections.push_back(ROLLEDSECTION(_T("HP8X36"),10.6,8.02,8.16,119));
}
