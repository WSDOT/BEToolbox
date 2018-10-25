///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2014  Washington State Department of Transportation
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
#include "resource.h"
#include "GirCompDoc.h"
#include "GirCompTitlePageBuilder.h"
#include "GirCompChapterBuilder.h"
#include "GirCompChildFrame.h"
#include "BEToolboxStatusBar.h"

#include <EAF\EAFUtilities.h>
#include <EAF\EAFApp.h>

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

CGirCompDoc::CGirCompDoc()
{
   CReportBuilder* pRptBuilder = new CReportBuilder(_T("GirComp"));

   boost::shared_ptr<CTitlePageBuilder> pTitlePageBuilder(new CGirCompTitlePageBuilder());
   pRptBuilder->AddTitlePageBuilder( pTitlePageBuilder );

   boost::shared_ptr<CChapterBuilder> pChBuilder( new CGirCompChapterBuilder(this) );
   pRptBuilder->AddChapterBuilder(pChBuilder);

   m_RptMgr.AddReportBuilder(pRptBuilder);
}

CGirCompDoc::~CGirCompDoc()
{
}


BEGIN_MESSAGE_MAP(CGirCompDoc, CBEToolboxDoc)
END_MESSAGE_MAP()


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
   problem.D = ::ConvertToSysUnits(122, unitMeasure::Inch);
   problem.tWeb = ::ConvertToSysUnits(0.438, unitMeasure::Inch);
   problem.wTopFlange = ::ConvertToSysUnits(22, unitMeasure::Inch);
   problem.tTopFlange = ::ConvertToSysUnits(1.25, unitMeasure::Inch);
   problem.wBotFlange = ::ConvertToSysUnits(22, unitMeasure::Inch);
   problem.tBotFlange = ::ConvertToSysUnits(2.25, unitMeasure::Inch);
   problem.wSlab = ::ConvertToSysUnits(84, unitMeasure::Inch);
   problem.tSlab = ::ConvertToSysUnits(7, unitMeasure::Inch);
   problem.G = ::ConvertToSysUnits(0.5, unitMeasure::Inch);
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

BOOL CGirCompDoc::OpenOldFormat(LPCTSTR lpszPathName)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   AfxMessageBox(_T("Error: Invalid file format.\n\nThis is not a GirComp file or it is in a legacy format that is not supported"),MB_OK | MB_ICONEXCLAMATION);
   return FALSE;
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

const GIRCOMPDIMENSIONS& CGirCompDoc::GetProblem(IndexType idx)
{
   return m_Problems[idx];
}

void CGirCompDoc::ClearProblems()
{
   m_Problems.clear();
}

void CGirCompDoc::ComputeShapeProperties(IndexType idx,IShapeProperties** ppShapeProperties)
{
   const GIRCOMPDIMENSIONS& dimensions = GetProblem(idx);
   switch (dimensions.Type)
   {
   case GIRCOMPDIMENSIONS::Rolled:
      ComputeRollSectionProperties(dimensions,ppShapeProperties);
      break;

   case GIRCOMPDIMENSIONS::General:
      ComputeGeneralSectionProperties(dimensions,ppShapeProperties);
      break;

   case GIRCOMPDIMENSIONS::BuiltUp:
      ComputeBuiltUpSectionProperties(dimensions,ppShapeProperties);
      break;

   default:
      ATLASSERT(false); // is there a new type?
   }
}

void CGirCompDoc::ComputeRollSectionProperties(const GIRCOMPDIMENSIONS& dimensions,IShapeProperties** ppShapeProperties)
{
   CComPtr<IShapeProperties> dummy;
   dummy.CoCreateInstance(CLSID_ShapeProperties);
   dummy.CopyTo(ppShapeProperties);
}

void CGirCompDoc::ComputeGeneralSectionProperties(const GIRCOMPDIMENSIONS& dimensions,IShapeProperties** ppShapeProperties)
{
   CComPtr<IShapeProperties> dummy;
   dummy.CoCreateInstance(CLSID_ShapeProperties);
   dummy.CopyTo(ppShapeProperties);
}

void CGirCompDoc::ComputeBuiltUpSectionProperties(const GIRCOMPDIMENSIONS& dimensions,IShapeProperties** ppShapeProperties)
{
   CComPtr<IShapeProperties> dummy;
   dummy.CoCreateInstance(CLSID_ShapeProperties);
   dummy.CopyTo(ppShapeProperties);
}

void CGirCompDoc::GetBuiltUpGirder(IndexType idx,IPlateGirder** ppGirder)
{
   const GIRCOMPDIMENSIONS& dimensions = GetProblem(idx);
   ATLASSERT(dimensions.Type == GIRCOMPDIMENSIONS::BuiltUp);

   CComPtr<IPlateGirder> girder;
   girder.CoCreateInstance(CLSID_PlateGirder);

   girder->put_TopFlangeWidth(dimensions.wTopFlange);
   girder->put_TopFlangeThick(dimensions.tTopFlange);

   girder->put_BottomFlangeWidth(dimensions.wBotFlange);
   girder->put_BottomFlangeThick(dimensions.tBotFlange);

   girder->put_WebHeight(dimensions.D);
   girder->put_WebThick(dimensions.tWeb);

   girder.CopyTo(ppGirder);
}

void CGirCompDoc::GetCompositeBeam(IndexType idx,int n,ICompositeBeam** ppCompBeam)
{
   const GIRCOMPDIMENSIONS& dimensions = GetProblem(idx);

   Float64 N;
   if ( n == 0 )
      N = dimensions.N1;
   else if ( n == 1 )
      N = dimensions.N2;
   else if ( n == 2 )
      N = dimensions.N3;

   CComPtr<ICompositeBeam> compBeam;
   compBeam.CoCreateInstance(CLSID_CompositeBeam);

   compBeam->put_HaunchDepth(dimensions.G);
   compBeam->put_GrossSlabDepth(dimensions.tSlab);
   compBeam->put_TributarySlabWidth(dimensions.wSlab);
   compBeam->put_EffectiveSlabWidth(dimensions.wSlab);

   compBeam->put_BeamE(N);
   compBeam->put_SlabE(1.0);

   if ( dimensions.Type == GIRCOMPDIMENSIONS::Rolled || dimensions.Type == GIRCOMPDIMENSIONS::General )
   {
      CComPtr<IGenericShape> genericShape;
      genericShape.CoCreateInstance(CLSID_GenericShape);

      genericShape->put_Area(dimensions.Area);
      genericShape->put_Ixx(dimensions.MomentOfInertia);
      
      CComPtr<IPoint2d> centroid;
      genericShape->get_Centroid(&centroid);
      centroid->Move(0,dimensions.D/2);
      genericShape->putref_Centroid(centroid);

      genericShape->put_Ytop(dimensions.D/2);
      genericShape->put_Ybottom(dimensions.D/2);
      genericShape->put_Xleft(dimensions.wTopFlange/2);
      genericShape->put_Xright(dimensions.wTopFlange/2);

      compBeam->put_HaunchWidth(dimensions.wTopFlange);

      CComQIPtr<IShape> shape(genericShape);
      compBeam->putref_Beam(shape);
   }
   else if ( dimensions.Type == GIRCOMPDIMENSIONS::BuiltUp )
   {
      CComPtr<IPlateGirder> girder;
      GetBuiltUpGirder(idx,&girder);
      CComQIPtr<IShape> shape(girder);

      Float64 wTF;
      girder->get_TopFlangeWidth(&wTF);
      compBeam->put_HaunchWidth(wTF);

      compBeam->putref_Beam(shape);
   }
   else
   {
      ATLASSERT(false); // should never get here
   }

   compBeam.CopyTo(ppCompBeam);
}

const std::vector<ROLLEDSECTION>& CGirCompDoc::GetRolledSections() const
{
   return m_RolledSections;
}

void CGirCompDoc::InitRolledSections()
{
   m_RolledSections.push_back(ROLLEDSECTION(_T("W44X335"),::ConvertToSysUnits(98.5,unitMeasure::Inch2),::ConvertToSysUnits(44.0,unitMeasure::Inch),::ConvertToSysUnits(15.9,unitMeasure::Inch),::ConvertToSysUnits(31100,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W44X290"),::ConvertToSysUnits(85.4,unitMeasure::Inch2),::ConvertToSysUnits(43.6,unitMeasure::Inch),::ConvertToSysUnits(15.8,unitMeasure::Inch),::ConvertToSysUnits(27000,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W44X262"),::ConvertToSysUnits(77.2,unitMeasure::Inch2),::ConvertToSysUnits(43.3,unitMeasure::Inch),::ConvertToSysUnits(15.8,unitMeasure::Inch),::ConvertToSysUnits(24100,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W44X230"),::ConvertToSysUnits(67.8,unitMeasure::Inch2),::ConvertToSysUnits(42.9,unitMeasure::Inch),::ConvertToSysUnits(15.8,unitMeasure::Inch),::ConvertToSysUnits(20800,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W40X593"),::ConvertToSysUnits(174,unitMeasure::Inch2),::ConvertToSysUnits(43.0,unitMeasure::Inch),::ConvertToSysUnits(16.7,unitMeasure::Inch),::ConvertToSysUnits(50400,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W40X503"),::ConvertToSysUnits(148,unitMeasure::Inch2),::ConvertToSysUnits(42.1,unitMeasure::Inch),::ConvertToSysUnits(16.4,unitMeasure::Inch),::ConvertToSysUnits(41600,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W40X431"),::ConvertToSysUnits(127,unitMeasure::Inch2),::ConvertToSysUnits(41.3,unitMeasure::Inch),::ConvertToSysUnits(16.2,unitMeasure::Inch),::ConvertToSysUnits(34800,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W40X397"),::ConvertToSysUnits(117,unitMeasure::Inch2),::ConvertToSysUnits(41.0,unitMeasure::Inch),::ConvertToSysUnits(16.1,unitMeasure::Inch),::ConvertToSysUnits(32000,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W40X372"),::ConvertToSysUnits(110,unitMeasure::Inch2),::ConvertToSysUnits(40.6,unitMeasure::Inch),::ConvertToSysUnits(16.1,unitMeasure::Inch),::ConvertToSysUnits(29600,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W40X362"),::ConvertToSysUnits(106,unitMeasure::Inch2),::ConvertToSysUnits(40.6,unitMeasure::Inch),::ConvertToSysUnits(16.0,unitMeasure::Inch),::ConvertToSysUnits(28900,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W40X324"),::ConvertToSysUnits(95.3,unitMeasure::Inch2),::ConvertToSysUnits(40.2,unitMeasure::Inch),::ConvertToSysUnits(15.9,unitMeasure::Inch),::ConvertToSysUnits(25600,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W40X297"),::ConvertToSysUnits(87.3,unitMeasure::Inch2),::ConvertToSysUnits(39.8,unitMeasure::Inch),::ConvertToSysUnits(15.8,unitMeasure::Inch),::ConvertToSysUnits(23200,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W40X277"),::ConvertToSysUnits(81.5,unitMeasure::Inch2),::ConvertToSysUnits(39.7,unitMeasure::Inch),::ConvertToSysUnits(15.8,unitMeasure::Inch),::ConvertToSysUnits(21900,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W40X249"),::ConvertToSysUnits(73.5,unitMeasure::Inch2),::ConvertToSysUnits(39.4,unitMeasure::Inch),::ConvertToSysUnits(15.8,unitMeasure::Inch),::ConvertToSysUnits(19600,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W40X215"),::ConvertToSysUnits(63.5,unitMeasure::Inch2),::ConvertToSysUnits(39.0,unitMeasure::Inch),::ConvertToSysUnits(15.8,unitMeasure::Inch),::ConvertToSysUnits(16700,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W40X199"),::ConvertToSysUnits(58.8,unitMeasure::Inch2),::ConvertToSysUnits(38.7,unitMeasure::Inch),::ConvertToSysUnits(15.8,unitMeasure::Inch),::ConvertToSysUnits(14900,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W40X392"),::ConvertToSysUnits(116,unitMeasure::Inch2),::ConvertToSysUnits(41.6,unitMeasure::Inch),::ConvertToSysUnits(12.4,unitMeasure::Inch),::ConvertToSysUnits(29900,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W40X331"),::ConvertToSysUnits(97.7,unitMeasure::Inch2),::ConvertToSysUnits(40.8,unitMeasure::Inch),::ConvertToSysUnits(12.2,unitMeasure::Inch),::ConvertToSysUnits(24700,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W40X327"),::ConvertToSysUnits(95.9,unitMeasure::Inch2),::ConvertToSysUnits(40.8,unitMeasure::Inch),::ConvertToSysUnits(12.1,unitMeasure::Inch),::ConvertToSysUnits(24500,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W40X294"),::ConvertToSysUnits(86.2,unitMeasure::Inch2),::ConvertToSysUnits(40.4,unitMeasure::Inch),::ConvertToSysUnits(12.0,unitMeasure::Inch),::ConvertToSysUnits(21900,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W40X278"),::ConvertToSysUnits(82.3,unitMeasure::Inch2),::ConvertToSysUnits(40.2,unitMeasure::Inch),::ConvertToSysUnits(12.0,unitMeasure::Inch),::ConvertToSysUnits(20500,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W40X264"),::ConvertToSysUnits(77.4,unitMeasure::Inch2),::ConvertToSysUnits(40.0,unitMeasure::Inch),::ConvertToSysUnits(11.9,unitMeasure::Inch),::ConvertToSysUnits(19400,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W40X235"),::ConvertToSysUnits(69.1,unitMeasure::Inch2),::ConvertToSysUnits(39.7,unitMeasure::Inch),::ConvertToSysUnits(11.9,unitMeasure::Inch),::ConvertToSysUnits(17400,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W40X211"),::ConvertToSysUnits(62.1,unitMeasure::Inch2),::ConvertToSysUnits(39.4,unitMeasure::Inch),::ConvertToSysUnits(11.8,unitMeasure::Inch),::ConvertToSysUnits(15500,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W40X183"),::ConvertToSysUnits(53.3,unitMeasure::Inch2),::ConvertToSysUnits(39.0,unitMeasure::Inch),::ConvertToSysUnits(11.8,unitMeasure::Inch),::ConvertToSysUnits(13200,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W40X167"),::ConvertToSysUnits(49.3,unitMeasure::Inch2),::ConvertToSysUnits(38.6,unitMeasure::Inch),::ConvertToSysUnits(11.8,unitMeasure::Inch),::ConvertToSysUnits(11600,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W40X149"),::ConvertToSysUnits(43.8,unitMeasure::Inch2),::ConvertToSysUnits(38.2,unitMeasure::Inch),::ConvertToSysUnits(11.8,unitMeasure::Inch),::ConvertToSysUnits(9800,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W36X652"),::ConvertToSysUnits(192,unitMeasure::Inch2),::ConvertToSysUnits(41.1,unitMeasure::Inch),::ConvertToSysUnits(17.6,unitMeasure::Inch),::ConvertToSysUnits(50600,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W36X529"),::ConvertToSysUnits(156,unitMeasure::Inch2),::ConvertToSysUnits(39.8,unitMeasure::Inch),::ConvertToSysUnits(17.2,unitMeasure::Inch),::ConvertToSysUnits(39600,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W36X487"),::ConvertToSysUnits(143,unitMeasure::Inch2),::ConvertToSysUnits(39.3,unitMeasure::Inch),::ConvertToSysUnits(17.1,unitMeasure::Inch),::ConvertToSysUnits(36000,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W36X441"),::ConvertToSysUnits(130,unitMeasure::Inch2),::ConvertToSysUnits(38.9,unitMeasure::Inch),::ConvertToSysUnits(17.0,unitMeasure::Inch),::ConvertToSysUnits(32100,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W36X395"),::ConvertToSysUnits(116,unitMeasure::Inch2),::ConvertToSysUnits(38.4,unitMeasure::Inch),::ConvertToSysUnits(16.8,unitMeasure::Inch),::ConvertToSysUnits(28500,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W36X361"),::ConvertToSysUnits(106,unitMeasure::Inch2),::ConvertToSysUnits(38.0,unitMeasure::Inch),::ConvertToSysUnits(16.7,unitMeasure::Inch),::ConvertToSysUnits(25700,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W36X330"),::ConvertToSysUnits(96.9,unitMeasure::Inch2),::ConvertToSysUnits(37.7,unitMeasure::Inch),::ConvertToSysUnits(16.6,unitMeasure::Inch),::ConvertToSysUnits(23300,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W36X302"),::ConvertToSysUnits(89.0,unitMeasure::Inch2),::ConvertToSysUnits(37.3,unitMeasure::Inch),::ConvertToSysUnits(16.7,unitMeasure::Inch),::ConvertToSysUnits(21100,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W36X282"),::ConvertToSysUnits(82.9,unitMeasure::Inch2),::ConvertToSysUnits(37.1,unitMeasure::Inch),::ConvertToSysUnits(16.6,unitMeasure::Inch),::ConvertToSysUnits(19600,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W36X262"),::ConvertToSysUnits(77.2,unitMeasure::Inch2),::ConvertToSysUnits(36.9,unitMeasure::Inch),::ConvertToSysUnits(16.6,unitMeasure::Inch),::ConvertToSysUnits(17900,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W36X247"),::ConvertToSysUnits(72.5,unitMeasure::Inch2),::ConvertToSysUnits(36.7,unitMeasure::Inch),::ConvertToSysUnits(16.5,unitMeasure::Inch),::ConvertToSysUnits(16700,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W36X231"),::ConvertToSysUnits(68.2,unitMeasure::Inch2),::ConvertToSysUnits(36.5,unitMeasure::Inch),::ConvertToSysUnits(16.5,unitMeasure::Inch),::ConvertToSysUnits(15600,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W36X256"),::ConvertToSysUnits(75.3,unitMeasure::Inch2),::ConvertToSysUnits(37.4,unitMeasure::Inch),::ConvertToSysUnits(12.2,unitMeasure::Inch),::ConvertToSysUnits(16800,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W36X232"),::ConvertToSysUnits(68.0,unitMeasure::Inch2),::ConvertToSysUnits(37.1,unitMeasure::Inch),::ConvertToSysUnits(12.1,unitMeasure::Inch),::ConvertToSysUnits(15000,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W36X210"),::ConvertToSysUnits(61.9,unitMeasure::Inch2),::ConvertToSysUnits(36.7,unitMeasure::Inch),::ConvertToSysUnits(12.2,unitMeasure::Inch),::ConvertToSysUnits(13200,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W36X194"),::ConvertToSysUnits(57.0,unitMeasure::Inch2),::ConvertToSysUnits(36.5,unitMeasure::Inch),::ConvertToSysUnits(12.1,unitMeasure::Inch),::ConvertToSysUnits(12100,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W36X182"),::ConvertToSysUnits(53.6,unitMeasure::Inch2),::ConvertToSysUnits(36.3,unitMeasure::Inch),::ConvertToSysUnits(12.1,unitMeasure::Inch),::ConvertToSysUnits(11300,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W36X170"),::ConvertToSysUnits(50.0,unitMeasure::Inch2),::ConvertToSysUnits(36.2,unitMeasure::Inch),::ConvertToSysUnits(12.0,unitMeasure::Inch),::ConvertToSysUnits(10500,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W36X160"),::ConvertToSysUnits(47.0,unitMeasure::Inch2),::ConvertToSysUnits(36.0,unitMeasure::Inch),::ConvertToSysUnits(12.0,unitMeasure::Inch),::ConvertToSysUnits(9760,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W36X150"),::ConvertToSysUnits(44.3,unitMeasure::Inch2),::ConvertToSysUnits(35.9,unitMeasure::Inch),::ConvertToSysUnits(12.0,unitMeasure::Inch),::ConvertToSysUnits(9040,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W36X135"),::ConvertToSysUnits(39.9,unitMeasure::Inch2),::ConvertToSysUnits(35.6,unitMeasure::Inch),::ConvertToSysUnits(12.0,unitMeasure::Inch),::ConvertToSysUnits(7800,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W33X387"),::ConvertToSysUnits(114,unitMeasure::Inch2),::ConvertToSysUnits(36.0,unitMeasure::Inch),::ConvertToSysUnits(16.2,unitMeasure::Inch),::ConvertToSysUnits(24300,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W33X354"),::ConvertToSysUnits(104,unitMeasure::Inch2),::ConvertToSysUnits(35.6,unitMeasure::Inch),::ConvertToSysUnits(16.1,unitMeasure::Inch),::ConvertToSysUnits(22000,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W33X318"),::ConvertToSysUnits(93.7,unitMeasure::Inch2),::ConvertToSysUnits(35.2,unitMeasure::Inch),::ConvertToSysUnits(16.0,unitMeasure::Inch),::ConvertToSysUnits(19500,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W33X291"),::ConvertToSysUnits(85.6,unitMeasure::Inch2),::ConvertToSysUnits(34.8,unitMeasure::Inch),::ConvertToSysUnits(15.9,unitMeasure::Inch),::ConvertToSysUnits(17700,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W33X263"),::ConvertToSysUnits(77.4,unitMeasure::Inch2),::ConvertToSysUnits(34.5,unitMeasure::Inch),::ConvertToSysUnits(15.8,unitMeasure::Inch),::ConvertToSysUnits(15900,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W33X241"),::ConvertToSysUnits(71.1,unitMeasure::Inch2),::ConvertToSysUnits(34.2,unitMeasure::Inch),::ConvertToSysUnits(15.9,unitMeasure::Inch),::ConvertToSysUnits(14200,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W33X221"),::ConvertToSysUnits(65.3,unitMeasure::Inch2),::ConvertToSysUnits(33.9,unitMeasure::Inch),::ConvertToSysUnits(15.8,unitMeasure::Inch),::ConvertToSysUnits(12900,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W33X201"),::ConvertToSysUnits(59.1,unitMeasure::Inch2),::ConvertToSysUnits(33.7,unitMeasure::Inch),::ConvertToSysUnits(15.7,unitMeasure::Inch),::ConvertToSysUnits(11600,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W33X169"),::ConvertToSysUnits(49.5,unitMeasure::Inch2),::ConvertToSysUnits(33.8,unitMeasure::Inch),::ConvertToSysUnits(11.5,unitMeasure::Inch),::ConvertToSysUnits(9290,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W33X152"),::ConvertToSysUnits(44.9,unitMeasure::Inch2),::ConvertToSysUnits(33.5,unitMeasure::Inch),::ConvertToSysUnits(11.6,unitMeasure::Inch),::ConvertToSysUnits(8160,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W33X141"),::ConvertToSysUnits(41.5,unitMeasure::Inch2),::ConvertToSysUnits(33.3,unitMeasure::Inch),::ConvertToSysUnits(11.5,unitMeasure::Inch),::ConvertToSysUnits(7450,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W33X130"),::ConvertToSysUnits(38.3,unitMeasure::Inch2),::ConvertToSysUnits(33.1,unitMeasure::Inch),::ConvertToSysUnits(11.5,unitMeasure::Inch),::ConvertToSysUnits(6710,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W33X118"),::ConvertToSysUnits(34.7,unitMeasure::Inch2),::ConvertToSysUnits(32.9,unitMeasure::Inch),::ConvertToSysUnits(11.5,unitMeasure::Inch),::ConvertToSysUnits(5900,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W30X391"),::ConvertToSysUnits(115,unitMeasure::Inch2),::ConvertToSysUnits(33.2,unitMeasure::Inch),::ConvertToSysUnits(15.6,unitMeasure::Inch),::ConvertToSysUnits(20700,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W30X357"),::ConvertToSysUnits(105,unitMeasure::Inch2),::ConvertToSysUnits(32.8,unitMeasure::Inch),::ConvertToSysUnits(15.5,unitMeasure::Inch),::ConvertToSysUnits(18700,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W30X326"),::ConvertToSysUnits(95.9,unitMeasure::Inch2),::ConvertToSysUnits(32.4,unitMeasure::Inch),::ConvertToSysUnits(15.4,unitMeasure::Inch),::ConvertToSysUnits(16800,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W30X292"),::ConvertToSysUnits(86.0,unitMeasure::Inch2),::ConvertToSysUnits(32.0,unitMeasure::Inch),::ConvertToSysUnits(15.3,unitMeasure::Inch),::ConvertToSysUnits(14900,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W30X261"),::ConvertToSysUnits(77.0,unitMeasure::Inch2),::ConvertToSysUnits(31.6,unitMeasure::Inch),::ConvertToSysUnits(15.2,unitMeasure::Inch),::ConvertToSysUnits(13100,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W30X235"),::ConvertToSysUnits(69.3,unitMeasure::Inch2),::ConvertToSysUnits(31.3,unitMeasure::Inch),::ConvertToSysUnits(15.1,unitMeasure::Inch),::ConvertToSysUnits(11700,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W30X211"),::ConvertToSysUnits(62.3,unitMeasure::Inch2),::ConvertToSysUnits(30.9,unitMeasure::Inch),::ConvertToSysUnits(15.1,unitMeasure::Inch),::ConvertToSysUnits(10300,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W30X191"),::ConvertToSysUnits(56.1,unitMeasure::Inch2),::ConvertToSysUnits(30.7,unitMeasure::Inch),::ConvertToSysUnits(15.0,unitMeasure::Inch),::ConvertToSysUnits(9200,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W30X173"),::ConvertToSysUnits(50.9,unitMeasure::Inch2),::ConvertToSysUnits(30.4,unitMeasure::Inch),::ConvertToSysUnits(15.0,unitMeasure::Inch),::ConvertToSysUnits(8230,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W30X148"),::ConvertToSysUnits(43.6,unitMeasure::Inch2),::ConvertToSysUnits(30.7,unitMeasure::Inch),::ConvertToSysUnits(10.5,unitMeasure::Inch),::ConvertToSysUnits(6680,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W30X132"),::ConvertToSysUnits(38.8,unitMeasure::Inch2),::ConvertToSysUnits(30.3,unitMeasure::Inch),::ConvertToSysUnits(10.5,unitMeasure::Inch),::ConvertToSysUnits(5770,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W30X124"),::ConvertToSysUnits(36.5,unitMeasure::Inch2),::ConvertToSysUnits(30.2,unitMeasure::Inch),::ConvertToSysUnits(10.5,unitMeasure::Inch),::ConvertToSysUnits(5360,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W30X116"),::ConvertToSysUnits(34.2,unitMeasure::Inch2),::ConvertToSysUnits(30.0,unitMeasure::Inch),::ConvertToSysUnits(10.5,unitMeasure::Inch),::ConvertToSysUnits(4930,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W30X108"),::ConvertToSysUnits(31.7,unitMeasure::Inch2),::ConvertToSysUnits(29.8,unitMeasure::Inch),::ConvertToSysUnits(10.5,unitMeasure::Inch),::ConvertToSysUnits(4470,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W30X99"),::ConvertToSysUnits(29.0,unitMeasure::Inch2),::ConvertToSysUnits(29.7,unitMeasure::Inch),::ConvertToSysUnits(10.5,unitMeasure::Inch),::ConvertToSysUnits(3990,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W30X90"),::ConvertToSysUnits(26.3,unitMeasure::Inch2),::ConvertToSysUnits(29.5,unitMeasure::Inch),::ConvertToSysUnits(10.4,unitMeasure::Inch),::ConvertToSysUnits(3610,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W27X539"),::ConvertToSysUnits(159,unitMeasure::Inch2),::ConvertToSysUnits(32.5,unitMeasure::Inch),::ConvertToSysUnits(15.3,unitMeasure::Inch),::ConvertToSysUnits(25600,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W27X368"),::ConvertToSysUnits(109,unitMeasure::Inch2),::ConvertToSysUnits(30.4,unitMeasure::Inch),::ConvertToSysUnits(14.7,unitMeasure::Inch),::ConvertToSysUnits(16200,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W27X336"),::ConvertToSysUnits(99.2,unitMeasure::Inch2),::ConvertToSysUnits(30.0,unitMeasure::Inch),::ConvertToSysUnits(14.6,unitMeasure::Inch),::ConvertToSysUnits(14600,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W27X307"),::ConvertToSysUnits(90.2,unitMeasure::Inch2),::ConvertToSysUnits(29.6,unitMeasure::Inch),::ConvertToSysUnits(14.4,unitMeasure::Inch),::ConvertToSysUnits(13100,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W27X281"),::ConvertToSysUnits(83.1,unitMeasure::Inch2),::ConvertToSysUnits(29.3,unitMeasure::Inch),::ConvertToSysUnits(14.4,unitMeasure::Inch),::ConvertToSysUnits(11900,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W27X258"),::ConvertToSysUnits(76.1,unitMeasure::Inch2),::ConvertToSysUnits(29.0,unitMeasure::Inch),::ConvertToSysUnits(14.3,unitMeasure::Inch),::ConvertToSysUnits(10800,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W27X235"),::ConvertToSysUnits(69.4,unitMeasure::Inch2),::ConvertToSysUnits(28.7,unitMeasure::Inch),::ConvertToSysUnits(14.2,unitMeasure::Inch),::ConvertToSysUnits(9700,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W27X217"),::ConvertToSysUnits(63.9,unitMeasure::Inch2),::ConvertToSysUnits(28.4,unitMeasure::Inch),::ConvertToSysUnits(14.1,unitMeasure::Inch),::ConvertToSysUnits(8910,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W27X194"),::ConvertToSysUnits(57.1,unitMeasure::Inch2),::ConvertToSysUnits(28.1,unitMeasure::Inch),::ConvertToSysUnits(14.0,unitMeasure::Inch),::ConvertToSysUnits(7860,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W27X178"),::ConvertToSysUnits(52.5,unitMeasure::Inch2),::ConvertToSysUnits(27.8,unitMeasure::Inch),::ConvertToSysUnits(14.1,unitMeasure::Inch),::ConvertToSysUnits(7020,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W27X161"),::ConvertToSysUnits(47.6,unitMeasure::Inch2),::ConvertToSysUnits(27.6,unitMeasure::Inch),::ConvertToSysUnits(14.0,unitMeasure::Inch),::ConvertToSysUnits(6310,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W27X146"),::ConvertToSysUnits(43.2,unitMeasure::Inch2),::ConvertToSysUnits(27.4,unitMeasure::Inch),::ConvertToSysUnits(14.0,unitMeasure::Inch),::ConvertToSysUnits(5660,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W27X129"),::ConvertToSysUnits(37.8,unitMeasure::Inch2),::ConvertToSysUnits(27.6,unitMeasure::Inch),::ConvertToSysUnits(10.0,unitMeasure::Inch),::ConvertToSysUnits(4760,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W27X114"),::ConvertToSysUnits(33.6,unitMeasure::Inch2),::ConvertToSysUnits(27.3,unitMeasure::Inch),::ConvertToSysUnits(10.1,unitMeasure::Inch),::ConvertToSysUnits(4080,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W27X102"),::ConvertToSysUnits(30.0,unitMeasure::Inch2),::ConvertToSysUnits(27.1,unitMeasure::Inch),::ConvertToSysUnits(10.0,unitMeasure::Inch),::ConvertToSysUnits(3620,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W27X94"),::ConvertToSysUnits(27.6,unitMeasure::Inch2),::ConvertToSysUnits(26.9,unitMeasure::Inch),::ConvertToSysUnits(10.0,unitMeasure::Inch),::ConvertToSysUnits(3270,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W27X84"),::ConvertToSysUnits(24.7,unitMeasure::Inch2),::ConvertToSysUnits(26.7,unitMeasure::Inch),::ConvertToSysUnits(10.0,unitMeasure::Inch),::ConvertToSysUnits(2850,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W24X370"),::ConvertToSysUnits(109,unitMeasure::Inch2),::ConvertToSysUnits(28.0,unitMeasure::Inch),::ConvertToSysUnits(13.7,unitMeasure::Inch),::ConvertToSysUnits(13400,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W24X335"),::ConvertToSysUnits(98.3,unitMeasure::Inch2),::ConvertToSysUnits(27.5,unitMeasure::Inch),::ConvertToSysUnits(13.5,unitMeasure::Inch),::ConvertToSysUnits(11900,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W24X306"),::ConvertToSysUnits(89.7,unitMeasure::Inch2),::ConvertToSysUnits(27.1,unitMeasure::Inch),::ConvertToSysUnits(13.4,unitMeasure::Inch),::ConvertToSysUnits(10700,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W24X279"),::ConvertToSysUnits(81.9,unitMeasure::Inch2),::ConvertToSysUnits(26.7,unitMeasure::Inch),::ConvertToSysUnits(13.3,unitMeasure::Inch),::ConvertToSysUnits(9600,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W24X250"),::ConvertToSysUnits(73.5,unitMeasure::Inch2),::ConvertToSysUnits(26.3,unitMeasure::Inch),::ConvertToSysUnits(13.2,unitMeasure::Inch),::ConvertToSysUnits(8490,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W24X229"),::ConvertToSysUnits(67.2,unitMeasure::Inch2),::ConvertToSysUnits(26.0,unitMeasure::Inch),::ConvertToSysUnits(13.1,unitMeasure::Inch),::ConvertToSysUnits(7650,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W24X207"),::ConvertToSysUnits(60.7,unitMeasure::Inch2),::ConvertToSysUnits(25.7,unitMeasure::Inch),::ConvertToSysUnits(13.0,unitMeasure::Inch),::ConvertToSysUnits(6820,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W24X192"),::ConvertToSysUnits(56.5,unitMeasure::Inch2),::ConvertToSysUnits(25.5,unitMeasure::Inch),::ConvertToSysUnits(13.0,unitMeasure::Inch),::ConvertToSysUnits(6260,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W24X176"),::ConvertToSysUnits(51.7,unitMeasure::Inch2),::ConvertToSysUnits(25.2,unitMeasure::Inch),::ConvertToSysUnits(12.9,unitMeasure::Inch),::ConvertToSysUnits(5680,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W24X162"),::ConvertToSysUnits(47.8,unitMeasure::Inch2),::ConvertToSysUnits(25.0,unitMeasure::Inch),::ConvertToSysUnits(13.0,unitMeasure::Inch),::ConvertToSysUnits(5170,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W24X146"),::ConvertToSysUnits(43.0,unitMeasure::Inch2),::ConvertToSysUnits(24.7,unitMeasure::Inch),::ConvertToSysUnits(12.9,unitMeasure::Inch),::ConvertToSysUnits(4580,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W24X131"),::ConvertToSysUnits(38.6,unitMeasure::Inch2),::ConvertToSysUnits(24.5,unitMeasure::Inch),::ConvertToSysUnits(12.9,unitMeasure::Inch),::ConvertToSysUnits(4020,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W24X117"),::ConvertToSysUnits(34.4,unitMeasure::Inch2),::ConvertToSysUnits(24.3,unitMeasure::Inch),::ConvertToSysUnits(12.8,unitMeasure::Inch),::ConvertToSysUnits(3540,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W24X104"),::ConvertToSysUnits(30.7,unitMeasure::Inch2),::ConvertToSysUnits(24.1,unitMeasure::Inch),::ConvertToSysUnits(12.8,unitMeasure::Inch),::ConvertToSysUnits(3100,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W24X103"),::ConvertToSysUnits(30.3,unitMeasure::Inch2),::ConvertToSysUnits(24.5,unitMeasure::Inch),::ConvertToSysUnits(9.00,unitMeasure::Inch),::ConvertToSysUnits(3000,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W24X94"),::ConvertToSysUnits(27.7,unitMeasure::Inch2),::ConvertToSysUnits(24.3,unitMeasure::Inch),::ConvertToSysUnits(9.07,unitMeasure::Inch),::ConvertToSysUnits(2700,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W24X84"),::ConvertToSysUnits(24.7,unitMeasure::Inch2),::ConvertToSysUnits(24.1,unitMeasure::Inch),::ConvertToSysUnits(9.02,unitMeasure::Inch),::ConvertToSysUnits(2370,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W24X76"),::ConvertToSysUnits(22.4,unitMeasure::Inch2),::ConvertToSysUnits(23.9,unitMeasure::Inch),::ConvertToSysUnits(8.99,unitMeasure::Inch),::ConvertToSysUnits(2100,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W24X68"),::ConvertToSysUnits(20.1,unitMeasure::Inch2),::ConvertToSysUnits(23.7,unitMeasure::Inch),::ConvertToSysUnits(8.97,unitMeasure::Inch),::ConvertToSysUnits(1830,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W24X62"),::ConvertToSysUnits(18.2,unitMeasure::Inch2),::ConvertToSysUnits(23.7,unitMeasure::Inch),::ConvertToSysUnits(7.04,unitMeasure::Inch),::ConvertToSysUnits(1550,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W24X55"),::ConvertToSysUnits(16.2,unitMeasure::Inch2),::ConvertToSysUnits(23.6,unitMeasure::Inch),::ConvertToSysUnits(7.01,unitMeasure::Inch),::ConvertToSysUnits(1350,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W21X201"),::ConvertToSysUnits(59.3,unitMeasure::Inch2),::ConvertToSysUnits(23.0,unitMeasure::Inch),::ConvertToSysUnits(12.6,unitMeasure::Inch),::ConvertToSysUnits(5310,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W21X182"),::ConvertToSysUnits(53.6,unitMeasure::Inch2),::ConvertToSysUnits(22.7,unitMeasure::Inch),::ConvertToSysUnits(12.5,unitMeasure::Inch),::ConvertToSysUnits(4730,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W21X166"),::ConvertToSysUnits(48.8,unitMeasure::Inch2),::ConvertToSysUnits(22.5,unitMeasure::Inch),::ConvertToSysUnits(12.4,unitMeasure::Inch),::ConvertToSysUnits(4280,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W21X147"),::ConvertToSysUnits(43.2,unitMeasure::Inch2),::ConvertToSysUnits(22.1,unitMeasure::Inch),::ConvertToSysUnits(12.5,unitMeasure::Inch),::ConvertToSysUnits(3630,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W21X132"),::ConvertToSysUnits(38.8,unitMeasure::Inch2),::ConvertToSysUnits(21.8,unitMeasure::Inch),::ConvertToSysUnits(12.4,unitMeasure::Inch),::ConvertToSysUnits(3220,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W21X122"),::ConvertToSysUnits(35.9,unitMeasure::Inch2),::ConvertToSysUnits(21.7,unitMeasure::Inch),::ConvertToSysUnits(12.4,unitMeasure::Inch),::ConvertToSysUnits(2960,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W21X111"),::ConvertToSysUnits(32.6,unitMeasure::Inch2),::ConvertToSysUnits(21.5,unitMeasure::Inch),::ConvertToSysUnits(12.3,unitMeasure::Inch),::ConvertToSysUnits(2670,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W21X101"),::ConvertToSysUnits(29.8,unitMeasure::Inch2),::ConvertToSysUnits(21.4,unitMeasure::Inch),::ConvertToSysUnits(12.3,unitMeasure::Inch),::ConvertToSysUnits(2420,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W21X93"),::ConvertToSysUnits(27.3,unitMeasure::Inch2),::ConvertToSysUnits(21.6,unitMeasure::Inch),::ConvertToSysUnits(8.42,unitMeasure::Inch),::ConvertToSysUnits(2070,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W21X83"),::ConvertToSysUnits(24.4,unitMeasure::Inch2),::ConvertToSysUnits(21.4,unitMeasure::Inch),::ConvertToSysUnits(8.36,unitMeasure::Inch),::ConvertToSysUnits(1830,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W21X73"),::ConvertToSysUnits(21.5,unitMeasure::Inch2),::ConvertToSysUnits(21.2,unitMeasure::Inch),::ConvertToSysUnits(8.30,unitMeasure::Inch),::ConvertToSysUnits(1600,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W21X68"),::ConvertToSysUnits(20.0,unitMeasure::Inch2),::ConvertToSysUnits(21.1,unitMeasure::Inch),::ConvertToSysUnits(8.27,unitMeasure::Inch),::ConvertToSysUnits(1480,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W21X62"),::ConvertToSysUnits(18.3,unitMeasure::Inch2),::ConvertToSysUnits(21.0,unitMeasure::Inch),::ConvertToSysUnits(8.24,unitMeasure::Inch),::ConvertToSysUnits(1330,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W21X55"),::ConvertToSysUnits(16.2,unitMeasure::Inch2),::ConvertToSysUnits(20.8,unitMeasure::Inch),::ConvertToSysUnits(8.22,unitMeasure::Inch),::ConvertToSysUnits(1140,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W21X48"),::ConvertToSysUnits(14.1,unitMeasure::Inch2),::ConvertToSysUnits(20.6,unitMeasure::Inch),::ConvertToSysUnits(8.14,unitMeasure::Inch),::ConvertToSysUnits(959,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W21X57"),::ConvertToSysUnits(16.7,unitMeasure::Inch2),::ConvertToSysUnits(21.1,unitMeasure::Inch),::ConvertToSysUnits(6.56,unitMeasure::Inch),::ConvertToSysUnits(1170,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W21X50"),::ConvertToSysUnits(14.7,unitMeasure::Inch2),::ConvertToSysUnits(20.8,unitMeasure::Inch),::ConvertToSysUnits(6.53,unitMeasure::Inch),::ConvertToSysUnits(984,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W21X44"),::ConvertToSysUnits(13.0,unitMeasure::Inch2),::ConvertToSysUnits(20.7,unitMeasure::Inch),::ConvertToSysUnits(6.50,unitMeasure::Inch),::ConvertToSysUnits(843,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W18X311"),::ConvertToSysUnits(91.6,unitMeasure::Inch2),::ConvertToSysUnits(22.3,unitMeasure::Inch),::ConvertToSysUnits(12.0,unitMeasure::Inch),::ConvertToSysUnits(6970,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W18X283"),::ConvertToSysUnits(83.3,unitMeasure::Inch2),::ConvertToSysUnits(21.9,unitMeasure::Inch),::ConvertToSysUnits(11.9,unitMeasure::Inch),::ConvertToSysUnits(6170,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W18X258"),::ConvertToSysUnits(76.0,unitMeasure::Inch2),::ConvertToSysUnits(21.5,unitMeasure::Inch),::ConvertToSysUnits(11.8,unitMeasure::Inch),::ConvertToSysUnits(5510,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W18X234"),::ConvertToSysUnits(68.6,unitMeasure::Inch2),::ConvertToSysUnits(21.1,unitMeasure::Inch),::ConvertToSysUnits(11.7,unitMeasure::Inch),::ConvertToSysUnits(4900,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W18X211"),::ConvertToSysUnits(62.3,unitMeasure::Inch2),::ConvertToSysUnits(20.7,unitMeasure::Inch),::ConvertToSysUnits(11.6,unitMeasure::Inch),::ConvertToSysUnits(4330,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W18X192"),::ConvertToSysUnits(56.2,unitMeasure::Inch2),::ConvertToSysUnits(20.4,unitMeasure::Inch),::ConvertToSysUnits(11.5,unitMeasure::Inch),::ConvertToSysUnits(3870,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W18X175"),::ConvertToSysUnits(51.4,unitMeasure::Inch2),::ConvertToSysUnits(20.0,unitMeasure::Inch),::ConvertToSysUnits(11.4,unitMeasure::Inch),::ConvertToSysUnits(3450,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W18X158"),::ConvertToSysUnits(46.3,unitMeasure::Inch2),::ConvertToSysUnits(19.7,unitMeasure::Inch),::ConvertToSysUnits(11.3,unitMeasure::Inch),::ConvertToSysUnits(3060,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W18X143"),::ConvertToSysUnits(42.0,unitMeasure::Inch2),::ConvertToSysUnits(19.5,unitMeasure::Inch),::ConvertToSysUnits(11.2,unitMeasure::Inch),::ConvertToSysUnits(2750,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W18X130"),::ConvertToSysUnits(38.3,unitMeasure::Inch2),::ConvertToSysUnits(19.3,unitMeasure::Inch),::ConvertToSysUnits(11.2,unitMeasure::Inch),::ConvertToSysUnits(2460,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W18X119"),::ConvertToSysUnits(35.1,unitMeasure::Inch2),::ConvertToSysUnits(19.0,unitMeasure::Inch),::ConvertToSysUnits(11.3,unitMeasure::Inch),::ConvertToSysUnits(2190,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W18X106"),::ConvertToSysUnits(31.1,unitMeasure::Inch2),::ConvertToSysUnits(18.7,unitMeasure::Inch),::ConvertToSysUnits(11.2,unitMeasure::Inch),::ConvertToSysUnits(1910,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W18X97"),::ConvertToSysUnits(28.5,unitMeasure::Inch2),::ConvertToSysUnits(18.6,unitMeasure::Inch),::ConvertToSysUnits(11.1,unitMeasure::Inch),::ConvertToSysUnits(1750,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W18X86"),::ConvertToSysUnits(25.3,unitMeasure::Inch2),::ConvertToSysUnits(18.4,unitMeasure::Inch),::ConvertToSysUnits(11.1,unitMeasure::Inch),::ConvertToSysUnits(1530,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W18X76"),::ConvertToSysUnits(22.3,unitMeasure::Inch2),::ConvertToSysUnits(18.2,unitMeasure::Inch),::ConvertToSysUnits(11.0,unitMeasure::Inch),::ConvertToSysUnits(1330,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W18X71"),::ConvertToSysUnits(20.9,unitMeasure::Inch2),::ConvertToSysUnits(18.5,unitMeasure::Inch),::ConvertToSysUnits(7.64,unitMeasure::Inch),::ConvertToSysUnits(1170,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W18X65"),::ConvertToSysUnits(19.1,unitMeasure::Inch2),::ConvertToSysUnits(18.4,unitMeasure::Inch),::ConvertToSysUnits(7.59,unitMeasure::Inch),::ConvertToSysUnits(1070,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W18X60"),::ConvertToSysUnits(17.6,unitMeasure::Inch2),::ConvertToSysUnits(18.2,unitMeasure::Inch),::ConvertToSysUnits(7.56,unitMeasure::Inch),::ConvertToSysUnits(984,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W18X55"),::ConvertToSysUnits(16.2,unitMeasure::Inch2),::ConvertToSysUnits(18.1,unitMeasure::Inch),::ConvertToSysUnits(7.53,unitMeasure::Inch),::ConvertToSysUnits(890,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W18X50"),::ConvertToSysUnits(14.7,unitMeasure::Inch2),::ConvertToSysUnits(18.0,unitMeasure::Inch),::ConvertToSysUnits(7.50,unitMeasure::Inch),::ConvertToSysUnits(800,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W18X46"),::ConvertToSysUnits(13.5,unitMeasure::Inch2),::ConvertToSysUnits(18.1,unitMeasure::Inch),::ConvertToSysUnits(6.06,unitMeasure::Inch),::ConvertToSysUnits(712,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W18X40"),::ConvertToSysUnits(11.8,unitMeasure::Inch2),::ConvertToSysUnits(17.9,unitMeasure::Inch),::ConvertToSysUnits(6.02,unitMeasure::Inch),::ConvertToSysUnits(612,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W18X35"),::ConvertToSysUnits(10.3,unitMeasure::Inch2),::ConvertToSysUnits(17.7,unitMeasure::Inch),::ConvertToSysUnits(6.00,unitMeasure::Inch),::ConvertToSysUnits(510,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W16X100"),::ConvertToSysUnits(29.4,unitMeasure::Inch2),::ConvertToSysUnits(17.0,unitMeasure::Inch),::ConvertToSysUnits(10.4,unitMeasure::Inch),::ConvertToSysUnits(1490,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W16X89"),::ConvertToSysUnits(26.2,unitMeasure::Inch2),::ConvertToSysUnits(16.8,unitMeasure::Inch),::ConvertToSysUnits(10.4,unitMeasure::Inch),::ConvertToSysUnits(1300,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W16X77"),::ConvertToSysUnits(22.6,unitMeasure::Inch2),::ConvertToSysUnits(16.5,unitMeasure::Inch),::ConvertToSysUnits(10.3,unitMeasure::Inch),::ConvertToSysUnits(1110,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W16X67"),::ConvertToSysUnits(19.6,unitMeasure::Inch2),::ConvertToSysUnits(16.3,unitMeasure::Inch),::ConvertToSysUnits(10.2,unitMeasure::Inch),::ConvertToSysUnits(954,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W16X57"),::ConvertToSysUnits(16.8,unitMeasure::Inch2),::ConvertToSysUnits(16.4,unitMeasure::Inch),::ConvertToSysUnits(7.12,unitMeasure::Inch),::ConvertToSysUnits(758,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W16X50"),::ConvertToSysUnits(14.7,unitMeasure::Inch2),::ConvertToSysUnits(16.3,unitMeasure::Inch),::ConvertToSysUnits(7.07,unitMeasure::Inch),::ConvertToSysUnits(659,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W16X45"),::ConvertToSysUnits(13.3,unitMeasure::Inch2),::ConvertToSysUnits(16.1,unitMeasure::Inch),::ConvertToSysUnits(7.04,unitMeasure::Inch),::ConvertToSysUnits(586,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W16X40"),::ConvertToSysUnits(11.8,unitMeasure::Inch2),::ConvertToSysUnits(16.0,unitMeasure::Inch),::ConvertToSysUnits(7.00,unitMeasure::Inch),::ConvertToSysUnits(518,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W16X36"),::ConvertToSysUnits(10.6,unitMeasure::Inch2),::ConvertToSysUnits(15.9,unitMeasure::Inch),::ConvertToSysUnits(6.99,unitMeasure::Inch),::ConvertToSysUnits(448,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W16X31"),::ConvertToSysUnits(9.13,unitMeasure::Inch2),::ConvertToSysUnits(15.9,unitMeasure::Inch),::ConvertToSysUnits(5.53,unitMeasure::Inch),::ConvertToSysUnits(375,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W16X26"),::ConvertToSysUnits(7.68,unitMeasure::Inch2),::ConvertToSysUnits(15.7,unitMeasure::Inch),::ConvertToSysUnits(5.50,unitMeasure::Inch),::ConvertToSysUnits(301,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X730"),::ConvertToSysUnits(215,unitMeasure::Inch2),::ConvertToSysUnits(22.4,unitMeasure::Inch),::ConvertToSysUnits(17.9,unitMeasure::Inch),::ConvertToSysUnits(14300,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X665"),::ConvertToSysUnits(196,unitMeasure::Inch2),::ConvertToSysUnits(21.6,unitMeasure::Inch),::ConvertToSysUnits(17.7,unitMeasure::Inch),::ConvertToSysUnits(12400,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X605"),::ConvertToSysUnits(178,unitMeasure::Inch2),::ConvertToSysUnits(20.9,unitMeasure::Inch),::ConvertToSysUnits(17.4,unitMeasure::Inch),::ConvertToSysUnits(10800,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X550"),::ConvertToSysUnits(162,unitMeasure::Inch2),::ConvertToSysUnits(20.2,unitMeasure::Inch),::ConvertToSysUnits(17.2,unitMeasure::Inch),::ConvertToSysUnits(9430,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X500"),::ConvertToSysUnits(147,unitMeasure::Inch2),::ConvertToSysUnits(19.6,unitMeasure::Inch),::ConvertToSysUnits(17.0,unitMeasure::Inch),::ConvertToSysUnits(8210,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X455"),::ConvertToSysUnits(134,unitMeasure::Inch2),::ConvertToSysUnits(19.0,unitMeasure::Inch),::ConvertToSysUnits(16.8,unitMeasure::Inch),::ConvertToSysUnits(7190,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X426"),::ConvertToSysUnits(125,unitMeasure::Inch2),::ConvertToSysUnits(18.7,unitMeasure::Inch),::ConvertToSysUnits(16.7,unitMeasure::Inch),::ConvertToSysUnits(6600,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X398"),::ConvertToSysUnits(117,unitMeasure::Inch2),::ConvertToSysUnits(18.3,unitMeasure::Inch),::ConvertToSysUnits(16.6,unitMeasure::Inch),::ConvertToSysUnits(6000,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X370"),::ConvertToSysUnits(109,unitMeasure::Inch2),::ConvertToSysUnits(17.9,unitMeasure::Inch),::ConvertToSysUnits(16.5,unitMeasure::Inch),::ConvertToSysUnits(5440,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X342"),::ConvertToSysUnits(101,unitMeasure::Inch2),::ConvertToSysUnits(17.5,unitMeasure::Inch),::ConvertToSysUnits(16.4,unitMeasure::Inch),::ConvertToSysUnits(4900,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X311"),::ConvertToSysUnits(91.4,unitMeasure::Inch2),::ConvertToSysUnits(17.1,unitMeasure::Inch),::ConvertToSysUnits(16.2,unitMeasure::Inch),::ConvertToSysUnits(4330,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X283"),::ConvertToSysUnits(83.3,unitMeasure::Inch2),::ConvertToSysUnits(16.7,unitMeasure::Inch),::ConvertToSysUnits(16.1,unitMeasure::Inch),::ConvertToSysUnits(3840,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X257"),::ConvertToSysUnits(75.6,unitMeasure::Inch2),::ConvertToSysUnits(16.4,unitMeasure::Inch),::ConvertToSysUnits(16.0,unitMeasure::Inch),::ConvertToSysUnits(3400,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X233"),::ConvertToSysUnits(68.5,unitMeasure::Inch2),::ConvertToSysUnits(16.0,unitMeasure::Inch),::ConvertToSysUnits(15.9,unitMeasure::Inch),::ConvertToSysUnits(3010,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X211"),::ConvertToSysUnits(62.0,unitMeasure::Inch2),::ConvertToSysUnits(15.7,unitMeasure::Inch),::ConvertToSysUnits(15.8,unitMeasure::Inch),::ConvertToSysUnits(2660,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X193"),::ConvertToSysUnits(56.8,unitMeasure::Inch2),::ConvertToSysUnits(15.5,unitMeasure::Inch),::ConvertToSysUnits(15.7,unitMeasure::Inch),::ConvertToSysUnits(2400,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X176"),::ConvertToSysUnits(51.8,unitMeasure::Inch2),::ConvertToSysUnits(15.2,unitMeasure::Inch),::ConvertToSysUnits(15.7,unitMeasure::Inch),::ConvertToSysUnits(2140,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X159"),::ConvertToSysUnits(46.7,unitMeasure::Inch2),::ConvertToSysUnits(15.0,unitMeasure::Inch),::ConvertToSysUnits(15.6,unitMeasure::Inch),::ConvertToSysUnits(1900,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X145"),::ConvertToSysUnits(42.7,unitMeasure::Inch2),::ConvertToSysUnits(14.8,unitMeasure::Inch),::ConvertToSysUnits(15.5,unitMeasure::Inch),::ConvertToSysUnits(1710,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X132"),::ConvertToSysUnits(38.8,unitMeasure::Inch2),::ConvertToSysUnits(14.7,unitMeasure::Inch),::ConvertToSysUnits(14.7,unitMeasure::Inch),::ConvertToSysUnits(1530,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X120"),::ConvertToSysUnits(35.3,unitMeasure::Inch2),::ConvertToSysUnits(14.5,unitMeasure::Inch),::ConvertToSysUnits(14.7,unitMeasure::Inch),::ConvertToSysUnits(1380,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X109"),::ConvertToSysUnits(32.0,unitMeasure::Inch2),::ConvertToSysUnits(14.3,unitMeasure::Inch),::ConvertToSysUnits(14.6,unitMeasure::Inch),::ConvertToSysUnits(1240,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X99"),::ConvertToSysUnits(29.1,unitMeasure::Inch2),::ConvertToSysUnits(14.2,unitMeasure::Inch),::ConvertToSysUnits(14.6,unitMeasure::Inch),::ConvertToSysUnits(1110,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X90"),::ConvertToSysUnits(26.5,unitMeasure::Inch2),::ConvertToSysUnits(14.0,unitMeasure::Inch),::ConvertToSysUnits(14.5,unitMeasure::Inch),::ConvertToSysUnits(999,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X82"),::ConvertToSysUnits(24.0,unitMeasure::Inch2),::ConvertToSysUnits(14.3,unitMeasure::Inch),::ConvertToSysUnits(10.1,unitMeasure::Inch),::ConvertToSysUnits(881,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X74"),::ConvertToSysUnits(21.8,unitMeasure::Inch2),::ConvertToSysUnits(14.2,unitMeasure::Inch),::ConvertToSysUnits(10.1,unitMeasure::Inch),::ConvertToSysUnits(795,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X68"),::ConvertToSysUnits(20.0,unitMeasure::Inch2),::ConvertToSysUnits(14.0,unitMeasure::Inch),::ConvertToSysUnits(10.0,unitMeasure::Inch),::ConvertToSysUnits(722,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X61"),::ConvertToSysUnits(17.9,unitMeasure::Inch2),::ConvertToSysUnits(13.9,unitMeasure::Inch),::ConvertToSysUnits(10.0,unitMeasure::Inch),::ConvertToSysUnits(640,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X53"),::ConvertToSysUnits(15.6,unitMeasure::Inch2),::ConvertToSysUnits(13.9,unitMeasure::Inch),::ConvertToSysUnits(8.06,unitMeasure::Inch),::ConvertToSysUnits(541,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X48"),::ConvertToSysUnits(14.1,unitMeasure::Inch2),::ConvertToSysUnits(13.8,unitMeasure::Inch),::ConvertToSysUnits(8.03,unitMeasure::Inch),::ConvertToSysUnits(484,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X43"),::ConvertToSysUnits(12.6,unitMeasure::Inch2),::ConvertToSysUnits(13.7,unitMeasure::Inch),::ConvertToSysUnits(8.00,unitMeasure::Inch),::ConvertToSysUnits(428,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X38"),::ConvertToSysUnits(11.2,unitMeasure::Inch2),::ConvertToSysUnits(14.1,unitMeasure::Inch),::ConvertToSysUnits(6.77,unitMeasure::Inch),::ConvertToSysUnits(385,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X34"),::ConvertToSysUnits(10.0,unitMeasure::Inch2),::ConvertToSysUnits(14.0,unitMeasure::Inch),::ConvertToSysUnits(6.75,unitMeasure::Inch),::ConvertToSysUnits(340,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X30"),::ConvertToSysUnits(8.85,unitMeasure::Inch2),::ConvertToSysUnits(13.8,unitMeasure::Inch),::ConvertToSysUnits(6.73,unitMeasure::Inch),::ConvertToSysUnits(291,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X26"),::ConvertToSysUnits(7.69,unitMeasure::Inch2),::ConvertToSysUnits(13.9,unitMeasure::Inch),::ConvertToSysUnits(5.03,unitMeasure::Inch),::ConvertToSysUnits(245,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W14X22"),::ConvertToSysUnits(6.49,unitMeasure::Inch2),::ConvertToSysUnits(13.7,unitMeasure::Inch),::ConvertToSysUnits(5.00,unitMeasure::Inch),::ConvertToSysUnits(199,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W12X336"),::ConvertToSysUnits(98.9,unitMeasure::Inch2),::ConvertToSysUnits(16.8,unitMeasure::Inch),::ConvertToSysUnits(13.4,unitMeasure::Inch),::ConvertToSysUnits(4060,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W12X305"),::ConvertToSysUnits(89.5,unitMeasure::Inch2),::ConvertToSysUnits(16.3,unitMeasure::Inch),::ConvertToSysUnits(13.2,unitMeasure::Inch),::ConvertToSysUnits(3550,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W12X279"),::ConvertToSysUnits(81.9,unitMeasure::Inch2),::ConvertToSysUnits(15.9,unitMeasure::Inch),::ConvertToSysUnits(13.1,unitMeasure::Inch),::ConvertToSysUnits(3110,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W12X252"),::ConvertToSysUnits(74.1,unitMeasure::Inch2),::ConvertToSysUnits(15.4,unitMeasure::Inch),::ConvertToSysUnits(13.0,unitMeasure::Inch),::ConvertToSysUnits(2720,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W12X230"),::ConvertToSysUnits(67.7,unitMeasure::Inch2),::ConvertToSysUnits(15.1,unitMeasure::Inch),::ConvertToSysUnits(12.9,unitMeasure::Inch),::ConvertToSysUnits(2420,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W12X210"),::ConvertToSysUnits(61.8,unitMeasure::Inch2),::ConvertToSysUnits(14.7,unitMeasure::Inch),::ConvertToSysUnits(12.8,unitMeasure::Inch),::ConvertToSysUnits(2140,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W12X190"),::ConvertToSysUnits(56.0,unitMeasure::Inch2),::ConvertToSysUnits(14.4,unitMeasure::Inch),::ConvertToSysUnits(12.7,unitMeasure::Inch),::ConvertToSysUnits(1890,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W12X170"),::ConvertToSysUnits(50.0,unitMeasure::Inch2),::ConvertToSysUnits(14.0,unitMeasure::Inch),::ConvertToSysUnits(12.6,unitMeasure::Inch),::ConvertToSysUnits(1650,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W12X152"),::ConvertToSysUnits(44.7,unitMeasure::Inch2),::ConvertToSysUnits(13.7,unitMeasure::Inch),::ConvertToSysUnits(12.5,unitMeasure::Inch),::ConvertToSysUnits(1430,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W12X136"),::ConvertToSysUnits(39.9,unitMeasure::Inch2),::ConvertToSysUnits(13.4,unitMeasure::Inch),::ConvertToSysUnits(12.4,unitMeasure::Inch),::ConvertToSysUnits(1240,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W12X120"),::ConvertToSysUnits(35.2,unitMeasure::Inch2),::ConvertToSysUnits(13.1,unitMeasure::Inch),::ConvertToSysUnits(12.3,unitMeasure::Inch),::ConvertToSysUnits(1070,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W12X106"),::ConvertToSysUnits(31.2,unitMeasure::Inch2),::ConvertToSysUnits(12.9,unitMeasure::Inch),::ConvertToSysUnits(12.2,unitMeasure::Inch),::ConvertToSysUnits(933,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W12X96"),::ConvertToSysUnits(28.2,unitMeasure::Inch2),::ConvertToSysUnits(12.7,unitMeasure::Inch),::ConvertToSysUnits(12.2,unitMeasure::Inch),::ConvertToSysUnits(833,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W12X87"),::ConvertToSysUnits(25.6,unitMeasure::Inch2),::ConvertToSysUnits(12.5,unitMeasure::Inch),::ConvertToSysUnits(12.1,unitMeasure::Inch),::ConvertToSysUnits(740,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W12X79"),::ConvertToSysUnits(23.2,unitMeasure::Inch2),::ConvertToSysUnits(12.4,unitMeasure::Inch),::ConvertToSysUnits(12.1,unitMeasure::Inch),::ConvertToSysUnits(662,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W12X72"),::ConvertToSysUnits(21.1,unitMeasure::Inch2),::ConvertToSysUnits(12.3,unitMeasure::Inch),::ConvertToSysUnits(12.0,unitMeasure::Inch),::ConvertToSysUnits(597,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W12X65"),::ConvertToSysUnits(19.1,unitMeasure::Inch2),::ConvertToSysUnits(12.1,unitMeasure::Inch),::ConvertToSysUnits(12.0,unitMeasure::Inch),::ConvertToSysUnits(533,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W12X58"),::ConvertToSysUnits(17.0,unitMeasure::Inch2),::ConvertToSysUnits(12.2,unitMeasure::Inch),::ConvertToSysUnits(10.0,unitMeasure::Inch),::ConvertToSysUnits(475,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W12X53"),::ConvertToSysUnits(15.6,unitMeasure::Inch2),::ConvertToSysUnits(12.1,unitMeasure::Inch),::ConvertToSysUnits(10.0,unitMeasure::Inch),::ConvertToSysUnits(425,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W12X50"),::ConvertToSysUnits(14.6,unitMeasure::Inch2),::ConvertToSysUnits(12.2,unitMeasure::Inch),::ConvertToSysUnits(8.08,unitMeasure::Inch),::ConvertToSysUnits(391,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W12X45"),::ConvertToSysUnits(13.1,unitMeasure::Inch2),::ConvertToSysUnits(12.1,unitMeasure::Inch),::ConvertToSysUnits(8.05,unitMeasure::Inch),::ConvertToSysUnits(348,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W12X40"),::ConvertToSysUnits(11.7,unitMeasure::Inch2),::ConvertToSysUnits(11.9,unitMeasure::Inch),::ConvertToSysUnits(8.01,unitMeasure::Inch),::ConvertToSysUnits(307,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W12X35"),::ConvertToSysUnits(10.3,unitMeasure::Inch2),::ConvertToSysUnits(12.5,unitMeasure::Inch),::ConvertToSysUnits(6.56,unitMeasure::Inch),::ConvertToSysUnits(285,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W12X30"),::ConvertToSysUnits(8.79,unitMeasure::Inch2),::ConvertToSysUnits(12.3,unitMeasure::Inch),::ConvertToSysUnits(6.52,unitMeasure::Inch),::ConvertToSysUnits(238,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W12X26"),::ConvertToSysUnits(7.65,unitMeasure::Inch2),::ConvertToSysUnits(12.2,unitMeasure::Inch),::ConvertToSysUnits(6.49,unitMeasure::Inch),::ConvertToSysUnits(204,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W12X22"),::ConvertToSysUnits(6.48,unitMeasure::Inch2),::ConvertToSysUnits(12.3,unitMeasure::Inch),::ConvertToSysUnits(4.03,unitMeasure::Inch),::ConvertToSysUnits(156,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W12X19"),::ConvertToSysUnits(5.57,unitMeasure::Inch2),::ConvertToSysUnits(12.2,unitMeasure::Inch),::ConvertToSysUnits(4.01,unitMeasure::Inch),::ConvertToSysUnits(130,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W12X16"),::ConvertToSysUnits(4.71,unitMeasure::Inch2),::ConvertToSysUnits(12.0,unitMeasure::Inch),::ConvertToSysUnits(3.99,unitMeasure::Inch),::ConvertToSysUnits(103,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W12X14"),::ConvertToSysUnits(4.16,unitMeasure::Inch2),::ConvertToSysUnits(11.9,unitMeasure::Inch),::ConvertToSysUnits(3.97,unitMeasure::Inch),::ConvertToSysUnits(88.6,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W10X112"),::ConvertToSysUnits(32.9,unitMeasure::Inch2),::ConvertToSysUnits(11.4,unitMeasure::Inch),::ConvertToSysUnits(10.4,unitMeasure::Inch),::ConvertToSysUnits(716,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W10X100"),::ConvertToSysUnits(29.3,unitMeasure::Inch2),::ConvertToSysUnits(11.1,unitMeasure::Inch),::ConvertToSysUnits(10.3,unitMeasure::Inch),::ConvertToSysUnits(623,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W10X88"),::ConvertToSysUnits(26.0,unitMeasure::Inch2),::ConvertToSysUnits(10.8,unitMeasure::Inch),::ConvertToSysUnits(10.3,unitMeasure::Inch),::ConvertToSysUnits(534,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W10X77"),::ConvertToSysUnits(22.7,unitMeasure::Inch2),::ConvertToSysUnits(10.6,unitMeasure::Inch),::ConvertToSysUnits(10.2,unitMeasure::Inch),::ConvertToSysUnits(455,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W10X68"),::ConvertToSysUnits(19.9,unitMeasure::Inch2),::ConvertToSysUnits(10.4,unitMeasure::Inch),::ConvertToSysUnits(10.1,unitMeasure::Inch),::ConvertToSysUnits(394,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W10X60"),::ConvertToSysUnits(17.7,unitMeasure::Inch2),::ConvertToSysUnits(10.2,unitMeasure::Inch),::ConvertToSysUnits(10.1,unitMeasure::Inch),::ConvertToSysUnits(341,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W10X54"),::ConvertToSysUnits(15.8,unitMeasure::Inch2),::ConvertToSysUnits(10.1,unitMeasure::Inch),::ConvertToSysUnits(10.0,unitMeasure::Inch),::ConvertToSysUnits(303,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W10X49"),::ConvertToSysUnits(14.4,unitMeasure::Inch2),::ConvertToSysUnits(10.0,unitMeasure::Inch),::ConvertToSysUnits(10.0,unitMeasure::Inch),::ConvertToSysUnits(272,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W10X45"),::ConvertToSysUnits(13.3,unitMeasure::Inch2),::ConvertToSysUnits(10.1,unitMeasure::Inch),::ConvertToSysUnits(8.02,unitMeasure::Inch),::ConvertToSysUnits(248,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W10X39"),::ConvertToSysUnits(11.5,unitMeasure::Inch2),::ConvertToSysUnits(9.92,unitMeasure::Inch),::ConvertToSysUnits(7.99,unitMeasure::Inch),::ConvertToSysUnits(209,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W10X33"),::ConvertToSysUnits(9.71,unitMeasure::Inch2),::ConvertToSysUnits(9.73,unitMeasure::Inch),::ConvertToSysUnits(7.96,unitMeasure::Inch),::ConvertToSysUnits(171,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W10X30"),::ConvertToSysUnits(8.84,unitMeasure::Inch2),::ConvertToSysUnits(10.5,unitMeasure::Inch),::ConvertToSysUnits(5.81,unitMeasure::Inch),::ConvertToSysUnits(170,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W10X26"),::ConvertToSysUnits(7.61,unitMeasure::Inch2),::ConvertToSysUnits(10.3,unitMeasure::Inch),::ConvertToSysUnits(5.77,unitMeasure::Inch),::ConvertToSysUnits(144,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W10X22"),::ConvertToSysUnits(6.49,unitMeasure::Inch2),::ConvertToSysUnits(10.2,unitMeasure::Inch),::ConvertToSysUnits(5.75,unitMeasure::Inch),::ConvertToSysUnits(118,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W10X19"),::ConvertToSysUnits(5.62,unitMeasure::Inch2),::ConvertToSysUnits(10.2,unitMeasure::Inch),::ConvertToSysUnits(4.02,unitMeasure::Inch),::ConvertToSysUnits(96.3,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W10X17"),::ConvertToSysUnits(4.99,unitMeasure::Inch2),::ConvertToSysUnits(10.1,unitMeasure::Inch),::ConvertToSysUnits(4.01,unitMeasure::Inch),::ConvertToSysUnits(81.9,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W10X15"),::ConvertToSysUnits(4.41,unitMeasure::Inch2),::ConvertToSysUnits(9.99,unitMeasure::Inch),::ConvertToSysUnits(4.00,unitMeasure::Inch),::ConvertToSysUnits(68.9,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W10X12"),::ConvertToSysUnits(3.54,unitMeasure::Inch2),::ConvertToSysUnits(9.87,unitMeasure::Inch),::ConvertToSysUnits(3.96,unitMeasure::Inch),::ConvertToSysUnits(53.8,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W8X67"),::ConvertToSysUnits(19.7,unitMeasure::Inch2),::ConvertToSysUnits(9.00,unitMeasure::Inch),::ConvertToSysUnits(8.28,unitMeasure::Inch),::ConvertToSysUnits(272,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W8X58"),::ConvertToSysUnits(17.1,unitMeasure::Inch2),::ConvertToSysUnits(8.75,unitMeasure::Inch),::ConvertToSysUnits(8.22,unitMeasure::Inch),::ConvertToSysUnits(228,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W8X48"),::ConvertToSysUnits(14.1,unitMeasure::Inch2),::ConvertToSysUnits(8.50,unitMeasure::Inch),::ConvertToSysUnits(8.11,unitMeasure::Inch),::ConvertToSysUnits(184,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W8X40"),::ConvertToSysUnits(11.7,unitMeasure::Inch2),::ConvertToSysUnits(8.25,unitMeasure::Inch),::ConvertToSysUnits(8.07,unitMeasure::Inch),::ConvertToSysUnits(146,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W8X35"),::ConvertToSysUnits(10.3,unitMeasure::Inch2),::ConvertToSysUnits(8.12,unitMeasure::Inch),::ConvertToSysUnits(8.02,unitMeasure::Inch),::ConvertToSysUnits(127,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W8X31"),::ConvertToSysUnits(9.13,unitMeasure::Inch2),::ConvertToSysUnits(8.00,unitMeasure::Inch),::ConvertToSysUnits(8.00,unitMeasure::Inch),::ConvertToSysUnits(110,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W8X28"),::ConvertToSysUnits(8.25,unitMeasure::Inch2),::ConvertToSysUnits(8.06,unitMeasure::Inch),::ConvertToSysUnits(6.54,unitMeasure::Inch),::ConvertToSysUnits(98.0,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W8X24"),::ConvertToSysUnits(7.08,unitMeasure::Inch2),::ConvertToSysUnits(7.93,unitMeasure::Inch),::ConvertToSysUnits(6.50,unitMeasure::Inch),::ConvertToSysUnits(82.7,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W8X21"),::ConvertToSysUnits(6.16,unitMeasure::Inch2),::ConvertToSysUnits(8.28,unitMeasure::Inch),::ConvertToSysUnits(5.27,unitMeasure::Inch),::ConvertToSysUnits(75.3,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W8X18"),::ConvertToSysUnits(5.26,unitMeasure::Inch2),::ConvertToSysUnits(8.14,unitMeasure::Inch),::ConvertToSysUnits(5.25,unitMeasure::Inch),::ConvertToSysUnits(61.9,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W8X15"),::ConvertToSysUnits(4.44,unitMeasure::Inch2),::ConvertToSysUnits(8.11,unitMeasure::Inch),::ConvertToSysUnits(4.02,unitMeasure::Inch),::ConvertToSysUnits(48.0,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W8X13"),::ConvertToSysUnits(3.84,unitMeasure::Inch2),::ConvertToSysUnits(7.99,unitMeasure::Inch),::ConvertToSysUnits(4.00,unitMeasure::Inch),::ConvertToSysUnits(39.6,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W8X10"),::ConvertToSysUnits(2.96,unitMeasure::Inch2),::ConvertToSysUnits(7.89,unitMeasure::Inch),::ConvertToSysUnits(3.94,unitMeasure::Inch),::ConvertToSysUnits(30.8,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W6X25"),::ConvertToSysUnits(7.34,unitMeasure::Inch2),::ConvertToSysUnits(6.38,unitMeasure::Inch),::ConvertToSysUnits(6.08,unitMeasure::Inch),::ConvertToSysUnits(53.4,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W6X20"),::ConvertToSysUnits(5.87,unitMeasure::Inch2),::ConvertToSysUnits(6.20,unitMeasure::Inch),::ConvertToSysUnits(6.02,unitMeasure::Inch),::ConvertToSysUnits(41.4,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W6X15"),::ConvertToSysUnits(4.43,unitMeasure::Inch2),::ConvertToSysUnits(5.99,unitMeasure::Inch),::ConvertToSysUnits(5.99,unitMeasure::Inch),::ConvertToSysUnits(29.1,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W6X16"),::ConvertToSysUnits(4.74,unitMeasure::Inch2),::ConvertToSysUnits(6.28,unitMeasure::Inch),::ConvertToSysUnits(4.03,unitMeasure::Inch),::ConvertToSysUnits(32.1,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W6X12"),::ConvertToSysUnits(3.55,unitMeasure::Inch2),::ConvertToSysUnits(6.03,unitMeasure::Inch),::ConvertToSysUnits(4.00,unitMeasure::Inch),::ConvertToSysUnits(22.1,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W6X9"),::ConvertToSysUnits(2.68,unitMeasure::Inch2),::ConvertToSysUnits(5.90,unitMeasure::Inch),::ConvertToSysUnits(3.94,unitMeasure::Inch),::ConvertToSysUnits(16.4,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W6X8.5"),::ConvertToSysUnits(2.52,unitMeasure::Inch2),::ConvertToSysUnits(5.83,unitMeasure::Inch),::ConvertToSysUnits(3.94,unitMeasure::Inch),::ConvertToSysUnits(14.9,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W5X19"),::ConvertToSysUnits(5.56,unitMeasure::Inch2),::ConvertToSysUnits(5.15,unitMeasure::Inch),::ConvertToSysUnits(5.03,unitMeasure::Inch),::ConvertToSysUnits(26.3,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W5X16"),::ConvertToSysUnits(4.71,unitMeasure::Inch2),::ConvertToSysUnits(5.01,unitMeasure::Inch),::ConvertToSysUnits(5.00,unitMeasure::Inch),::ConvertToSysUnits(21.4,unitMeasure::Inch4)));
   m_RolledSections.push_back(ROLLEDSECTION(_T("W4X13"),::ConvertToSysUnits(3.83,unitMeasure::Inch2),::ConvertToSysUnits(4.16,unitMeasure::Inch),::ConvertToSysUnits(4.06,unitMeasure::Inch),::ConvertToSysUnits(11.3,unitMeasure::Inch4)));
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
