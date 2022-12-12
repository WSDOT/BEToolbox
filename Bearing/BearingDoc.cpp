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

// BearingDoc.cpp : implementation file
//

#include "stdafx.h"
#include "..\resource.h"
#include "BearingDoc.h"
#include "BearingTitlePageBuilder.h"
#include "BearingChapterBuilder.h"
#include "BearingChildFrame.h"
#include "..\BEToolboxStatusBar.h"

#include <EAF\EAFUtilities.h>
#include <EAF\EAFApp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CBearingDoc

IMPLEMENT_DYNCREATE(CBearingDoc, CBEToolboxDoc)

CBearingDoc::CBearingDoc() : CBEToolboxDoc()
{
   std::shared_ptr<WBFL::Reporting::ReportBuilder> pRptBuilder(std::make_shared<WBFL::Reporting::ReportBuilder>(_T("Bearing")));
   GetReportManager()->AddReportBuilder(pRptBuilder);

   std::shared_ptr<WBFL::Reporting::TitlePageBuilder> pTitlePageBuilder(std::make_shared<CBearingTitlePageBuilder>());
   pRptBuilder->AddTitlePageBuilder( pTitlePageBuilder );

   std::shared_ptr<WBFL::Reporting::ChapterBuilder> pChBuilder(std::make_shared<CBearingChapterBuilder>(this) );
   pRptBuilder->AddChapterBuilder(pChBuilder);

   EnableUIHints(FALSE); // not using UIHints feature
}

CBearingDoc::~CBearingDoc()
{
}

CString CBearingDoc::GetToolbarSectionName()
{
   return _T("Bearing");
}

BEGIN_MESSAGE_MAP(CBearingDoc, CBEToolboxDoc)
   ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
END_MESSAGE_MAP()

void CBearingDoc::OnHelpFinder()
{
   EAFHelp(EAFGetDocument()->GetDocumentationSetName(),IDH_BEARING);
}


// CBearingDoc diagnostics

#ifdef _DEBUG
void CBearingDoc::AssertValid() const
{
	CBEToolboxDoc::AssertValid();
}

#ifndef _WIN32_WCE
void CBearingDoc::Dump(CDumpContext& dc) const
{
	CBEToolboxDoc::Dump(dc);
}
#endif
#endif //_DEBUG

BOOL CBearingDoc::Init()
{
   if ( !CBEToolboxDoc::Init() )
      return FALSE;

   return TRUE;
}

void CBearingDoc::OnCloseDocument()
{
   CBEToolboxDoc::OnCloseDocument();
}

HRESULT CBearingDoc::WriteTheDocument(IStructuredSave* pStrSave)
{
   HRESULT hr = pStrSave->BeginUnit(_T("Bearing"),1.0);
   if ( FAILED(hr) )
      return hr;


   hr = pStrSave->EndUnit(); // Bearing
   if ( FAILED(hr) )
      return hr;

   return S_OK;
}

HRESULT CBearingDoc::LoadTheDocument(IStructuredLoad* pStrLoad)
{
   HRESULT hr = pStrLoad->BeginUnit(_T("Bearing"));
   if ( FAILED(hr) )
      return hr;

   Float64 version;
   pStrLoad->get_Version(&version);

   CComVariant var;


   hr = pStrLoad->EndUnit(); // Bearing
   if ( FAILED(hr) )
      return hr;

   return S_OK;
}

void CBearingDoc::LoadDocumentSettings()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   __super::LoadDocumentSettings();
}

void CBearingDoc::SaveDocumentSettings()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   __super::SaveDocumentSettings();
}

CString CBearingDoc::GetDocumentationRootLocation()
{
   CEAFApp* pApp = EAFGetApp();
   return pApp->GetDocumentationRootLocation();
}
