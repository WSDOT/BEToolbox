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

// BoxGdrDoc.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "BoxGdrDoc.h"
#include "BoxGdrTitlePageBuilder.h"
#include "BoxGdrChapterBuilder.h"
#include "BoxGdrChildFrame.h"
#include "BEToolboxStatusBar.h"

#include <EAF\EAFUtilities.h>
#include <EAF\EAFApp.h>

// CBoxGdrDoc

IMPLEMENT_DYNCREATE(CBoxGdrDoc, CBEToolboxDoc)

CBoxGdrDoc::CBoxGdrDoc()
{
   CReportBuilder* pRptBuilder = new CReportBuilder(_T("BoxGdr"));

   boost::shared_ptr<CTitlePageBuilder> pTitlePageBuilder(new CBoxGdrTitlePageBuilder());
   pRptBuilder->AddTitlePageBuilder( pTitlePageBuilder );

   boost::shared_ptr<CChapterBuilder> pChBuilder( new CBoxGdrChapterBuilder(this) );
   pRptBuilder->AddChapterBuilder(pChBuilder);

   m_RptMgr.AddReportBuilder(pRptBuilder);
}

CBoxGdrDoc::~CBoxGdrDoc()
{
}


BEGIN_MESSAGE_MAP(CBoxGdrDoc, CBEToolboxDoc)
END_MESSAGE_MAP()


// CBoxGdrDoc diagnostics

#ifdef _DEBUG
void CBoxGdrDoc::AssertValid() const
{
	CBEToolboxDoc::AssertValid();
}

#ifndef _WIN32_WCE
void CBoxGdrDoc::Dump(CDumpContext& dc) const
{
	CBEToolboxDoc::Dump(dc);
}
#endif
#endif //_DEBUG

BOOL CBoxGdrDoc::Init()
{
   if ( !CBEToolboxDoc::Init() )
      return FALSE;

   // initialize with some data
   BOXGDRDIMENSIONS problem;
   problem.D = ::ConvertToSysUnits(8.0, unitMeasure::Feet);
   problem.T = ::ConvertToSysUnits(12.0, unitMeasure::Inch);
   problem.N = 3;
   problem.W = ::ConvertToSysUnits(24.0, unitMeasure::Feet);
   problem.ST = ::ConvertToSysUnits(7.5, unitMeasure::Inch);
   problem.SB = ::ConvertToSysUnits(7.0, unitMeasure::Inch);
   problem.FT = ::ConvertToSysUnits(6.0, unitMeasure::Inch);
   problem.FB = ::ConvertToSysUnits(6.0, unitMeasure::Inch);
   problem.EL = ::ConvertToSysUnits(5.0, unitMeasure::Feet);
   problem.CL = ::ConvertToSysUnits(8.5, unitMeasure::Inch);
   problem.BL = ::ConvertToSysUnits(1.21, unitMeasure::Feet);
   problem.ER = ::ConvertToSysUnits(5.0, unitMeasure::Feet);
   problem.CR = ::ConvertToSysUnits(8.5, unitMeasure::Inch);
   problem.BR = ::ConvertToSysUnits(1.21, unitMeasure::Feet);

   m_Problems.push_back(problem);

   return TRUE;
}

void CBoxGdrDoc::OnCloseDocument()
{
   EAFGetApp()->SetUnitsMode(eafTypes::umUS);

   CBEToolboxDoc::OnCloseDocument();
}

HRESULT CBoxGdrDoc::WriteTheDocument(IStructuredSave* pStrSave)
{
   HRESULT hr = pStrSave->BeginUnit(_T("BoxGdr"),1.0);
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

   std::vector<BOXGDRDIMENSIONS>::iterator iter(m_Problems.begin());
   std::vector<BOXGDRDIMENSIONS>::iterator end(m_Problems.end());
   for ( ; iter != end; iter++ )
   {
      BOXGDRDIMENSIONS& dimensions = *iter;

      hr = pStrSave->BeginUnit(_T("Dimensions"),1.0);
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("D"),CComVariant(dimensions.D));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("T"),CComVariant(dimensions.T));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("N"),CComVariant(dimensions.N));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("W"),CComVariant(dimensions.W));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("ST"),CComVariant(dimensions.ST));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("SB"),CComVariant(dimensions.SB));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("FT"),CComVariant(dimensions.FT));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("FB"),CComVariant(dimensions.FB));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("EL"),CComVariant(dimensions.EL));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("CL"),CComVariant(dimensions.CL));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("BL"),CComVariant(dimensions.BL));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("ER"),CComVariant(dimensions.ER));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("CR"),CComVariant(dimensions.CR));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("BR"),CComVariant(dimensions.BR));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->EndUnit(); // Dimensions
      if ( FAILED(hr) )
         return hr;
   }

   hr = pStrSave->EndUnit(); // Problems
   if ( FAILED(hr) )
      return hr;

   hr = pStrSave->EndUnit(); // BoxGdr
   if ( FAILED(hr) )
      return hr;

   return S_OK;
}

HRESULT CBoxGdrDoc::LoadTheDocument(IStructuredLoad* pStrLoad)
{
   ClearProblems();

   HRESULT hr = pStrLoad->BeginUnit(_T("BoxGdr"));
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
      BOXGDRDIMENSIONS dimensions;

      hr = pStrLoad->BeginUnit(_T("Dimensions"));
      if ( FAILED(hr) )
         return hr;

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("D"),&var);
      if ( FAILED(hr) )
         return hr;

      dimensions.D = var.dblVal;

      hr = pStrLoad->get_Property(_T("T"),&var);
      if ( FAILED(hr) )
         return hr;

      dimensions.T = var.dblVal;

      var.vt = VT_INDEX;
      hr = pStrLoad->get_Property(_T("N"),&var);
      if ( FAILED(hr) )
         return hr;

      dimensions.N = VARIANT2INDEX(var);

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("W"),&var);
      if ( FAILED(hr) )
         return hr;

      dimensions.W = var.dblVal;

      hr = pStrLoad->get_Property(_T("ST"),&var);
      if ( FAILED(hr) )
         return hr;

      dimensions.ST = var.dblVal;

      hr = pStrLoad->get_Property(_T("SB"),&var);
      if ( FAILED(hr) )
         return hr;

      dimensions.SB = var.dblVal;

      hr = pStrLoad->get_Property(_T("FT"),&var);
      if ( FAILED(hr) )
         return hr;

      dimensions.FT = var.dblVal;

      hr = pStrLoad->get_Property(_T("FB"),&var);
      if ( FAILED(hr) )
         return hr;

      dimensions.FB = var.dblVal;

      hr = pStrLoad->get_Property(_T("EL"),&var);
      if ( FAILED(hr) )
         return hr;

      dimensions.EL = var.dblVal;

      hr = pStrLoad->get_Property(_T("CL"),&var);
      if ( FAILED(hr) )
         return hr;

      dimensions.CL = var.dblVal;

      hr = pStrLoad->get_Property(_T("BL"),&var);
      if ( FAILED(hr) )
         return hr;

      dimensions.BL = var.dblVal;

      hr = pStrLoad->get_Property(_T("ER"),&var);
      if ( FAILED(hr) )
         return hr;

      dimensions.ER = var.dblVal;

      hr = pStrLoad->get_Property(_T("CR"),&var);
      if ( FAILED(hr) )
         return hr;

      dimensions.CR = var.dblVal;

      hr = pStrLoad->get_Property(_T("BR"),&var);
      if ( FAILED(hr) )
         return hr;

      dimensions.BR = var.dblVal;


      m_Problems.push_back(dimensions);

      hr = pStrLoad->EndUnit(); // Dimensions
      if ( FAILED(hr) )
         return hr;
   }

   hr = pStrLoad->EndUnit(); // Problems
   if ( FAILED(hr) )
      return hr;

   hr = pStrLoad->EndUnit(); // BoxGdr
   if ( FAILED(hr) )
      return hr;

   return S_OK;
}

CString CBoxGdrDoc::GetToolbarSectionName()
{
   return _T("BoxGdr");
}

BOOL CBoxGdrDoc::OpenOldFormat(LPCTSTR lpszPathName)
{
   std::_tifstream is;
   is.open(lpszPathName);
   
   TCHAR buffer[80];
   is.getline(buffer,ARRAYSIZE(buffer));

   sysTokenizer tokenizer(_T(","));
   tokenizer.push_back(buffer);
   sysTokenizer::size_type nTokens = tokenizer.size();
   ATLASSERT(nTokens == 3);
   std::_tstring strIdent = tokenizer[0];
   std::_tstring strX = tokenizer[1];

   ClearProblems();

   CEAFApp* pApp = EAFGetApp();
   pApp->SetUnitsMode(eafTypes::umUS);
   const unitmgtIndirectMeasure* pDisplayUnits = pApp->GetDisplayUnits();

   long nProblems;
   sysTokenizer::ParseLong(tokenizer[2].c_str(),&nProblems);
   for ( long i = 0; i < nProblems; i++ )
   {
      is.getline(buffer,ARRAYSIZE(buffer));
      tokenizer.push_back(buffer);
      BOXGDRDIMENSIONS dimensions;
      sysTokenizer::iterator iter(tokenizer.begin());
      iter++; // skip problem number

      sysTokenizer::ParseDouble(iter->c_str(),&dimensions.D);
      iter++;
      sysTokenizer::ParseDouble(iter->c_str(),&dimensions.T);
      iter++;
      long n;
      sysTokenizer::ParseLong(iter->c_str(),&n);
      dimensions.N = n;
      iter++;
      sysTokenizer::ParseDouble(iter->c_str(),&dimensions.W);
      iter++;
      sysTokenizer::ParseDouble(iter->c_str(),&dimensions.ST);
      iter++;
      sysTokenizer::ParseDouble(iter->c_str(),&dimensions.SB);
      iter++;
      sysTokenizer::ParseDouble(iter->c_str(),&dimensions.FT);
      iter++;
      sysTokenizer::ParseDouble(iter->c_str(),&dimensions.FB);
      iter++;
      sysTokenizer::ParseDouble(iter->c_str(),&dimensions.EL);
      iter++;
      sysTokenizer::ParseDouble(iter->c_str(),&dimensions.CL);
      iter++;
      sysTokenizer::ParseDouble(iter->c_str(),&dimensions.BL);
      iter++;
      sysTokenizer::ParseDouble(iter->c_str(),&dimensions.ER);
      iter++;
      sysTokenizer::ParseDouble(iter->c_str(),&dimensions.CR);
      iter++;
      sysTokenizer::ParseDouble(iter->c_str(),&dimensions.BR);
      iter++;

      dimensions.D = ::ConvertToSysUnits(dimensions.D,pDisplayUnits->SpanLength.UnitOfMeasure);
      dimensions.T = ::ConvertToSysUnits(dimensions.T,pDisplayUnits->ComponentDim.UnitOfMeasure);
      dimensions.W = ::ConvertToSysUnits(dimensions.W,pDisplayUnits->SpanLength.UnitOfMeasure);
      dimensions.ST = ::ConvertToSysUnits(dimensions.ST,pDisplayUnits->ComponentDim.UnitOfMeasure);
      dimensions.SB = ::ConvertToSysUnits(dimensions.SB,pDisplayUnits->ComponentDim.UnitOfMeasure);
      dimensions.FT = ::ConvertToSysUnits(dimensions.FT,pDisplayUnits->ComponentDim.UnitOfMeasure);
      dimensions.FB = ::ConvertToSysUnits(dimensions.FB,pDisplayUnits->ComponentDim.UnitOfMeasure);
      dimensions.EL = ::ConvertToSysUnits(dimensions.EL,pDisplayUnits->SpanLength.UnitOfMeasure);
      dimensions.CL = ::ConvertToSysUnits(dimensions.CL,pDisplayUnits->ComponentDim.UnitOfMeasure);
      dimensions.BL = ::ConvertToSysUnits(dimensions.BL,pDisplayUnits->SpanLength.UnitOfMeasure);
      dimensions.ER = ::ConvertToSysUnits(dimensions.ER,pDisplayUnits->SpanLength.UnitOfMeasure);
      dimensions.CR = ::ConvertToSysUnits(dimensions.CR,pDisplayUnits->ComponentDim.UnitOfMeasure);
      dimensions.BR = ::ConvertToSysUnits(dimensions.BR,pDisplayUnits->SpanLength.UnitOfMeasure);

      AddProblem(dimensions);
   }

   is.close();

   return TRUE;
}

IndexType CBoxGdrDoc::GetProblemCount() const
{
   return m_Problems.size();
}

void CBoxGdrDoc::AddProblem(const BOXGDRDIMENSIONS& problem)
{
   m_Problems.push_back(problem);
}

void CBoxGdrDoc::AddProblems(const std::vector<BOXGDRDIMENSIONS>& problems)
{
   m_Problems.insert(m_Problems.end(),problems.begin(),problems.end());
}

void CBoxGdrDoc::RemoveProblem(IndexType idx)
{
   m_Problems.erase(m_Problems.begin() + idx);
}

const BOXGDRDIMENSIONS& CBoxGdrDoc::GetProblem(IndexType idx)
{
   return m_Problems[idx];
}

void CBoxGdrDoc::ClearProblems()
{
   m_Problems.clear();
}

void CBoxGdrDoc::ComputeShapeProperties(IndexType idx,IShapeProperties** ppShapeProperties)
{
   const BOXGDRDIMENSIONS& dimensions = GetProblem(idx);
   CComPtr<ICompositeShape> compositeShape;
   compositeShape.CoCreateInstance(CLSID_CompositeShape);

   CComPtr<IPolyShape> mainShape;
   mainShape.CoCreateInstance(CLSID_PolyShape);

   mainShape->AddPoint(0,0);
   mainShape->AddPoint(dimensions.W/2,0);
   mainShape->AddPoint(dimensions.W/2,dimensions.D-dimensions.BR);
   mainShape->AddPoint(dimensions.W/2+dimensions.ER,dimensions.D-dimensions.CR);
   mainShape->AddPoint(dimensions.W/2+dimensions.ER,dimensions.D);
   mainShape->AddPoint(-(dimensions.W/2+dimensions.EL),dimensions.D);
   mainShape->AddPoint(-(dimensions.W/2+dimensions.EL),dimensions.D-dimensions.CL);
   mainShape->AddPoint(-dimensions.W/2,dimensions.D-dimensions.BL);
   mainShape->AddPoint(-dimensions.W/2,0);
   mainShape->AddPoint(0,0);

   CComQIPtr<IShape> s(mainShape);
   compositeShape->AddShape(s,VARIANT_FALSE);

   Float64 H = dimensions.D - dimensions.ST - dimensions.SB; // void height
   IndexType nVoids = dimensions.N-1;
   Float64 V = (dimensions.W - dimensions.N*dimensions.T)/(dimensions.N-1); // void width
   Float64 S = V + dimensions.T; // void spacing
   Float64 xOffset = -S*(nVoids-1)/2;
   Float64 yOffset = dimensions.SB;

   for (IndexType voidIdx = 0; voidIdx < nVoids; voidIdx++ )
   {
      CComPtr<IPolyShape> voidShape;
      voidShape.CoCreateInstance(CLSID_PolyShape);
      Float64 x1,x2,x3,x4,x5;
      Float64 y1,y2,y3,y4,y5;
      x1 = 0;                   y1 = 0;
      x2 = V/2 - dimensions.FB; y2 = 0;
      x3 = V/2;                 y3 = dimensions.FB;
      x4 = V/2;                 y4 = H - dimensions.FT;
      x5 = V/2 - dimensions.FT; y5 = H;

      voidShape->AddPoint(x1,y1);
      voidShape->AddPoint(x2,y2);
      voidShape->AddPoint(x3,y3);
      voidShape->AddPoint(x4,y4);
      voidShape->AddPoint(x5,y5);
      voidShape->AddPoint(-x5,y5);
      voidShape->AddPoint(-x4,y4);
      voidShape->AddPoint(-x3,y3);
      voidShape->AddPoint(-x2,y2);
      voidShape->AddPoint(-x1,y1);

      CComQIPtr<IXYPosition> position(voidShape);
      position->Offset(xOffset,yOffset);

      CComQIPtr<IShape> s(voidShape);
      compositeShape->AddShape(s,VARIANT_TRUE);

      xOffset += S;
   }

   CComQIPtr<IShape> shape(compositeShape);
   shape->get_ShapeProperties(ppShapeProperties);
}
