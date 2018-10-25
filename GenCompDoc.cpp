///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2013  Washington State Department of Transportation
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

// GenCompDoc.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "GenCompDoc.h"
#include "GenCompTitlePageBuilder.h"
#include "GenCompChapterBuilder.h"
#include "GenCompChildFrame.h"
#include "BEToolboxStatusBar.h"

#include <EAF\EAFUtilities.h>
#include <EAF\EAFApp.h>

// CGenCompDoc

IMPLEMENT_DYNCREATE(CGenCompDoc, CBEToolboxDoc)

CGenCompDoc::CGenCompDoc()
{
   CReportBuilder* pRptBuilder = new CReportBuilder(_T("GenComp"));

   boost::shared_ptr<CTitlePageBuilder> pTitlePageBuilder(new CGenCompTitlePageBuilder());
   pRptBuilder->AddTitlePageBuilder( pTitlePageBuilder );

   boost::shared_ptr<CChapterBuilder> pChBuilder( new CGenCompChapterBuilder(this) );
   pRptBuilder->AddChapterBuilder(pChBuilder);

   m_RptMgr.AddReportBuilder(pRptBuilder);

   m_ModularRatio = 1.0;
}

CGenCompDoc::~CGenCompDoc()
{
}


BEGIN_MESSAGE_MAP(CGenCompDoc, CBEToolboxDoc)
END_MESSAGE_MAP()


// CGenCompDoc diagnostics

#ifdef _DEBUG
void CGenCompDoc::AssertValid() const
{
	CBEToolboxDoc::AssertValid();
}

#ifndef _WIN32_WCE
void CGenCompDoc::Dump(CDumpContext& dc) const
{
	CBEToolboxDoc::Dump(dc);
}
#endif
#endif //_DEBUG

BOOL CGenCompDoc::Init()
{
   if ( !CBEToolboxDoc::Init() )
      return FALSE;

   return TRUE;
}

void CGenCompDoc::OnCloseDocument()
{
   CEAFDocument::OnCloseDocument();
}

HRESULT CGenCompDoc::WriteTheDocument(IStructuredSave* pStrSave)
{
   UpdateAllViews(NULL,0,NULL);

   HRESULT hr = pStrSave->BeginUnit(_T("GenComp"),1.0);
   if ( FAILED(hr) )
      return hr;

   CEAFApp* pApp = EAFGetApp();

   hr = pStrSave->put_Property(_T("Units"),CComVariant(pApp->GetUnitsMode()));
   if ( FAILED(hr) )
      return hr;

   hr = pStrSave->put_Property(_T("ModularRatio"),CComVariant(m_ModularRatio));
   if ( FAILED(hr) )
      return hr;

   hr = pStrSave->BeginUnit(_T("PrimaryShape"),1.0);
   if ( FAILED(hr) )
      return hr;

   hr = pStrSave->put_Property(_T("PointCount"),CComVariant(m_PrimaryPoints.size()));
   if ( FAILED(hr) )
      return hr;

   std::vector<std::pair<Float64,Float64>>::iterator iter(m_PrimaryPoints.begin());
   std::vector<std::pair<Float64,Float64>>::iterator end(m_PrimaryPoints.end());
   for ( ; iter != end; iter++ )
   {
      hr = pStrSave->BeginUnit(_T("Point"),1.0);
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("X"),CComVariant(iter->first));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("Y"),CComVariant(iter->second));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->EndUnit(); // Point
      if ( FAILED(hr) )
         return hr;
   }

   hr = pStrSave->EndUnit(); // PrimaryShape
   if ( FAILED(hr) )
      return hr;


   hr = pStrSave->BeginUnit(_T("SecondaryShape"),1.0);
   if ( FAILED(hr) )
      return hr;

   hr = pStrSave->put_Property(_T("PointCount"),CComVariant(m_SecondaryPoints.size()));
   if ( FAILED(hr) )
      return hr;

   iter = m_SecondaryPoints.begin();
   end = m_SecondaryPoints.end();
   for ( ; iter != end; iter++ )
   {
      hr = pStrSave->BeginUnit(_T("Point"),1.0);
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("X"),CComVariant(iter->first));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->put_Property(_T("Y"),CComVariant(iter->second));
      if ( FAILED(hr) )
         return hr;

      hr = pStrSave->EndUnit(); // Point
      if ( FAILED(hr) )
         return hr;
   }

   hr = pStrSave->EndUnit(); // SecondaryShape
   if ( FAILED(hr) )
      return hr;


   hr = pStrSave->EndUnit(); // GenComp
   if ( FAILED(hr) )
      return hr;

   return S_OK;
}

HRESULT CGenCompDoc::LoadTheDocument(IStructuredLoad* pStrLoad)
{
   Clear();

   HRESULT hr = pStrLoad->BeginUnit(_T("GenComp"));
   if ( FAILED(hr) )
      return hr;

   CComVariant var;

   CEAFApp* pApp = EAFGetApp();

   var.vt = VT_I4;
   hr = pStrLoad->get_Property(_T("Units"),&var);
   if ( FAILED(hr) )
      return hr;

   pApp->SetUnitsMode(eafTypes::UnitMode(var.lVal));

   var.vt = VT_R8;
   hr = pStrLoad->get_Property(_T("ModularRatio"),&var);
   if ( FAILED(hr) )
      return hr;

   m_ModularRatio = var.dblVal;

   hr = pStrLoad->BeginUnit(_T("PrimaryShape"));
   if ( FAILED(hr) )
      return hr;

   var.vt = VT_INDEX;
   hr = pStrLoad->get_Property(_T("PointCount"),&var);
   if ( FAILED(hr) )
      return hr;

   IndexType nPoints = VARIANT2INDEX(var);
   for ( IndexType idx = 0; idx < nPoints; idx++ )
   {
      hr = pStrLoad->BeginUnit(_T("Point"));
      if ( FAILED(hr) )
         return hr;

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("X"),&var);
      if ( FAILED(hr) )
         return hr;

      Float64 x = var.dblVal;

      hr = pStrLoad->get_Property(_T("Y"),&var);
      if ( FAILED(hr) )
         return hr;

      Float64 y = var.dblVal;

      m_PrimaryPoints.push_back(std::make_pair(x,y));

      hr = pStrLoad->EndUnit(); // Point
      if ( FAILED(hr) )
         return hr;
   }

   hr = pStrLoad->EndUnit(); // PrimaryShape
   if ( FAILED(hr) )
      return hr;


   hr = pStrLoad->BeginUnit(_T("SecondaryShape"));
   if ( FAILED(hr) )
      return hr;

   var.vt = VT_INDEX;
   hr = pStrLoad->get_Property(_T("PointCount"),&var);
   if ( FAILED(hr) )
      return hr;

   nPoints = VARIANT2INDEX(var);
   for ( IndexType idx = 0; idx < nPoints; idx++ )
   {
      hr = pStrLoad->BeginUnit(_T("Point"));
      if ( FAILED(hr) )
         return hr;

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("X"),&var);
      if ( FAILED(hr) )
         return hr;

      Float64 x = var.dblVal;

      hr = pStrLoad->get_Property(_T("Y"),&var);
      if ( FAILED(hr) )
         return hr;

      Float64 y = var.dblVal;

      m_SecondaryPoints.push_back(std::make_pair(x,y));

      hr = pStrLoad->EndUnit(); // Point
      if ( FAILED(hr) )
         return hr;
   }

   hr = pStrLoad->EndUnit(); // SecondaryShape
   if ( FAILED(hr) )
      return hr;


   hr = pStrLoad->EndUnit(); // GenComp
   if ( FAILED(hr) )
      return hr;

   return S_OK;
}

CString CGenCompDoc::GetToolbarSectionName()
{
   return _T("GenComp");
}

BOOL CGenCompDoc::OpenTheDocument(LPCTSTR lpszPathName)
{
   // Open the file and see if it starts with XML... if it does
   // pass it on for normal processing, otherwise, assume it is an old fortran fixed format file
   std::_tifstream is;
   is.open(lpszPathName);
   TCHAR buffer[80];
   is.getline(buffer,ARRAYSIZE(buffer));

   std::_tstring strHeader;
   strHeader.insert(strHeader.end(),&buffer[0],&buffer[0]+5);

   is.close();

   if ( strHeader != _T("<?xml") )
      return OpenOldFormat(lpszPathName);
   else
      return CEAFDocument::OpenTheDocument(lpszPathName);
}

BOOL CGenCompDoc::OpenOldFormat(LPCTSTR lpszPathName)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   AfxMessageBox(_T("Error: Invalid file format.\n\nThis is not a GenComp file or it is in a legacy format that is not supported"),MB_OK | MB_ICONEXCLAMATION);
   return FALSE;
}

void CGenCompDoc::Clear()
{
   ClearPrimaryPoints();
   ClearSecondaryPoints();
}

void CGenCompDoc::SetModularRatio(Float64 n)
{
   m_ModularRatio = n;
}

Float64 CGenCompDoc::GetModularRatio() const
{
   return m_ModularRatio;
}

void CGenCompDoc::ClearPrimaryPoints()
{
   m_PrimaryPoints.clear();
}

void CGenCompDoc::AddPrimaryPoint(Float64 x,Float64 y)
{
   m_PrimaryPoints.push_back(std::make_pair(x,y));
}

void CGenCompDoc::AddPrimaryPoints(const std::vector<std::pair<Float64,Float64>>& points)
{
   m_PrimaryPoints.insert(m_PrimaryPoints.end(),points.begin(),points.end());
}

const std::vector<std::pair<Float64,Float64>>& CGenCompDoc::GetPrimaryPoints()
{
   return m_PrimaryPoints;
}

void CGenCompDoc::ClearSecondaryPoints()
{
   m_SecondaryPoints.clear();
}

void CGenCompDoc::AddSecondaryPoint(Float64 x,Float64 y)
{
   m_SecondaryPoints.push_back(std::make_pair(x,y));
}

void CGenCompDoc::AddSecondaryPoints(const std::vector<std::pair<Float64,Float64>>& points)
{
   m_SecondaryPoints.insert(m_SecondaryPoints.end(),points.begin(),points.end());
}

const std::vector<std::pair<Float64,Float64>>& CGenCompDoc::GetSecondaryPoints()
{
   return m_SecondaryPoints;
}

CComPtr<IPolyShape> CGenCompDoc::GetPrimaryShape()
{
   CComPtr<IPolyShape> polyShape;
   polyShape.CoCreateInstance(CLSID_PolyShape);
   std::vector<std::pair<Float64,Float64>>::iterator iter(m_PrimaryPoints.begin());
   std::vector<std::pair<Float64,Float64>>::iterator end(m_PrimaryPoints.end());
   for ( ; iter != end; iter++ )
   {
      Float64 x = iter->first;
      Float64 y = iter->second;

      polyShape->AddPoint(x,y);
   }

   return polyShape;
}

CComPtr<ICompositeSection> CGenCompDoc::GetCompositeSection()
{
   CComPtr<IPolyShape> primaryShape(GetPrimaryShape());
   CComPtr<IPolyShape> secondaryShape;
   secondaryShape.CoCreateInstance(CLSID_PolyShape);
   std::vector<std::pair<Float64,Float64>>::iterator iter(m_SecondaryPoints.begin());
   std::vector<std::pair<Float64,Float64>>::iterator end(m_SecondaryPoints.end());
   for ( ; iter != end; iter++ )
   {
      Float64 x = iter->first;
      Float64 y = iter->second;

      secondaryShape->AddPoint(x,y);
   }

   CComPtr<ICompositeSection> section;
   section.CoCreateInstance(CLSID_CompositeSection);

   CComQIPtr<IShape> shape1(primaryShape);
   section->AddSection(shape1,1.0,1.0,VARIANT_FALSE,VARIANT_TRUE);

   CComQIPtr<IShape> shape2(secondaryShape);
   section->AddSection(shape2,m_ModularRatio,1.0,VARIANT_FALSE,VARIANT_TRUE);

   return section;
}
