///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2025  Washington State Department of Transportation
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

// UltColDoc.cpp : implementation file
//

#include "stdafx.h"
#include "..\resource.h"
#include "UltColDoc.h"
#include "UltColTitlePageBuilder.h"
#include "UltColChapterBuilder.h"
#include "..\BEToolboxStatusBar.h"

#include <EAF\EAFUtilities.h>
#include <EAF\EAFApp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CUltColDoc

IMPLEMENT_DYNCREATE(CUltColDoc, CBEToolboxDoc)

CUltColDoc::CUltColDoc() : CBEToolboxDoc()
{
   std::shared_ptr<WBFL::Reporting::ReportBuilder> pRptBuilder(std::make_shared<WBFL::Reporting::ReportBuilder>(_T("UltCol")));
   GetReportManager()->AddReportBuilder(pRptBuilder);

   std::shared_ptr<WBFL::Reporting::TitlePageBuilder> pTitlePageBuilder(std::make_shared<CUltColTitlePageBuilder>());
   pRptBuilder->AddTitlePageBuilder( pTitlePageBuilder );

   std::shared_ptr<WBFL::Reporting::ChapterBuilder> pChBuilder(std::make_shared<CUltColChapterBuilder>(this) );
   pRptBuilder->AddChapterBuilder(pChBuilder);

   EnableUIHints(FALSE); // not using UIHints feature
}

CUltColDoc::~CUltColDoc()
{
}


BEGIN_MESSAGE_MAP(CUltColDoc, CBEToolboxDoc)
   ON_COMMAND(ID_REFRESH_REPORT, &CUltColDoc::OnRefreshReport)
   ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
END_MESSAGE_MAP()

void CUltColDoc::OnHelpFinder()
{
   EAFHelp(EAFGetDocument()->GetDocumentationSetName(),IDH_ULTCOL);
}


// CUltColDoc diagnostics

#ifdef _DEBUG
void CUltColDoc::AssertValid() const
{
	CBEToolboxDoc::AssertValid();
}

#ifndef _WIN32_WCE
void CUltColDoc::Dump(CDumpContext& dc) const
{
	CBEToolboxDoc::Dump(dc);
}
#endif
#endif //_DEBUG

BOOL CUltColDoc::Init()
{
   if ( !CBEToolboxDoc::Init() )
      return FALSE;

   // initialize with some defaults
   m_Column.SetDiameter( WBFL::Units::ConvertToSysUnits(72.0,WBFL::Units::Measure::Inch) );
   m_Column.SetCover( WBFL::Units::ConvertToSysUnits(2.0,WBFL::Units::Measure::Inch));
   m_Column.SetAs( WBFL::Units::ConvertToSysUnits(10.0,WBFL::Units::Measure::Inch2));
   m_Column.SetFc( WBFL::Units::ConvertToSysUnits(4.0,WBFL::Units::Measure::KSI));
   m_Column.SetFy( WBFL::Units::ConvertToSysUnits(60.0,WBFL::Units::Measure::KSI));
   m_Column.SetEs( WBFL::Units::ConvertToSysUnits(29000.0,WBFL::Units::Measure::KSI));

   m_ecl = 0.0020;
   m_etl = 0.0050;

   return TRUE;
}

void CUltColDoc::SetColumn(const WBFL::RCSection::CircularColumn& column, Float64 ecl, Float64 etl)
{
   m_Column = column;
   m_ecl = ecl;
   m_etl = etl;
   SetModifiedFlag();
   UpdateAllViews(nullptr);
}

void CUltColDoc::GetColumn(WBFL::RCSection::CircularColumn& column, Float64& ecl, Float64& etl) const
{
   column = m_Column;
   ecl = m_ecl;
   etl = m_etl;
}

void CUltColDoc::OnRefreshReport()
{
   UpdateAllViews(nullptr);
}

HRESULT CUltColDoc::WriteTheDocument(IStructuredSave* pStrSave)
{
   HRESULT hr = pStrSave->BeginUnit(_T("UltCol"),2.0);
   if ( FAILED(hr) )
      return hr;

   Float64 diameter = m_Column.GetDiameter();
   Float64 cover = m_Column.GetCover();
   Float64 As = m_Column.GetAs();
   Float64 fc = m_Column.GetFc();
   Float64 fy = m_Column.GetFy();
   Float64 Es = m_Column.GetEs();

   CEAFApp* pApp = EAFGetApp();

   hr = pStrSave->put_Property(_T("Units"),CComVariant(pApp->GetUnitsMode()));
   if ( FAILED(hr) )
      return hr;

   hr = pStrSave->put_Property(_T("Diameter"),CComVariant(diameter));
   if ( FAILED(hr) )
      return hr;

   hr = pStrSave->put_Property(_T("Cover"),CComVariant(cover));
   if ( FAILED(hr) )
      return hr;

   hr = pStrSave->put_Property(_T("As"),CComVariant(As));
   if ( FAILED(hr) )
      return hr;

   hr = pStrSave->put_Property(_T("fc"),CComVariant(fc));
   if ( FAILED(hr) )
      return hr;

   hr = pStrSave->put_Property(_T("fy"),CComVariant(fy));
   if ( FAILED(hr) )
      return hr;

   hr = pStrSave->put_Property(_T("Es"),CComVariant(Es));
   if ( FAILED(hr) )
      return hr;

   // added in version 2.0
   hr = pStrSave->put_Property(_T("ecl"),CComVariant(m_ecl));
   if ( FAILED(hr) )
      return hr;

   // added in version 2.0
   hr = pStrSave->put_Property(_T("etl"),CComVariant(m_etl));
   if ( FAILED(hr) )
      return hr;

   hr = pStrSave->EndUnit();
   if ( FAILED(hr) )
      return hr;

   return S_OK;
}

HRESULT CUltColDoc::LoadTheDocument(IStructuredLoad* pStrLoad)
{
   HRESULT hr = pStrLoad->BeginUnit(_T("UltCol"));
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
   hr = pStrLoad->get_Property(_T("Diameter"),&var);
   if ( FAILED(hr) )
      return hr;
   m_Column.SetDiameter(var.dblVal);

   hr = pStrLoad->get_Property(_T("Cover"),&var);
   if ( FAILED(hr) )
      return hr;
   m_Column.SetCover(var.dblVal);

   hr = pStrLoad->get_Property(_T("As"),&var);
   if ( FAILED(hr) )
      return hr;
   m_Column.SetAs(var.dblVal);

   hr = pStrLoad->get_Property(_T("fc"),&var);
   if ( FAILED(hr) )
      return hr;
   m_Column.SetFc(var.dblVal);

   hr = pStrLoad->get_Property(_T("fy"),&var);
   if ( FAILED(hr) )
      return hr;
   m_Column.SetFy(var.dblVal);

   hr = pStrLoad->get_Property(_T("Es"),&var);
   if ( FAILED(hr) )
      return hr;
   m_Column.SetEs(var.dblVal);

   Float64 version;
   pStrLoad->get_Version(&version);
   if ( 1 < version )
   {
      hr = pStrLoad->get_Property(_T("ecl"),&var);
      if ( FAILED(hr) )
         return hr;
      m_ecl = var.dblVal;

      hr = pStrLoad->get_Property(_T("etl"),&var);
      if ( FAILED(hr) )
         return hr;
      m_etl = var.dblVal;
   }

   hr = pStrLoad->EndUnit();
   if ( FAILED(hr) )
      return hr;

   return S_OK;
}

CString CUltColDoc::GetToolbarSectionName()
{
   return _T("UltCol");
}

void CUltColDoc::LoadDocumentSettings()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   __super::LoadDocumentSettings();
}

void CUltColDoc::SaveDocumentSettings()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   __super::SaveDocumentSettings();
}

CString CUltColDoc::GetDocumentationRootLocation()
{
   CEAFApp* pApp = EAFGetApp();
   return pApp->GetDocumentationRootLocation();
}
