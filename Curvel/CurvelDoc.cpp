///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright � 1999-2025  Washington State Department of Transportation
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

// CurvelDoc.cpp : implementation file
//

#include "stdafx.h"
#include "..\resource.h"


#include "CurvelDoc.h"
#include "CurvelTitlePageBuilder.h"
#include "CurvelChapterBuilder.h"
#include "CurvelChildFrame.h"
#include "CurvelReportSpecification.h"
#include "CurvelReportSpecificationBuilder.h"
#include "CurvelRptView.h"
#include "..\BEToolboxStatusBar.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CCurvelDoc

IMPLEMENT_DYNCREATE(CCurvelDoc, CBEToolboxDoc)

CCurvelDoc::CCurvelDoc() : CBEToolboxDoc()
{
   // The reporting sub-system doesn't use the WBFLUnitServer implementation. It uses the old regular C++
   // units sytem. That system is in kms units, so we will create a unit server here also in the kms system
   // so that the curvel data, after loading is in set of consistent base units we want.
   // If the report system could handle the WBFLUnitServer, the <ConsistentUnits> declaration in the
   // CurvelXML instance document would work throughout this program because Curvel works exclusively in
   // consistent units.
   m_DocUnitServer.CoCreateInstance(CLSID_UnitServer);
   m_DocUnitServer->SetSystemUnits(CComBSTR(WBFL::Units::System::GetMassUnit().UnitTag().c_str()),
                            CComBSTR(WBFL::Units::System::GetLengthUnit().UnitTag().c_str()),
                            CComBSTR(WBFL::Units::System::GetTimeUnit().UnitTag().c_str()),
                            CComBSTR(WBFL::Units::System::GetTemperatureUnit().UnitTag().c_str()),
                            CComBSTR(WBFL::Units::System::GetAngleUnit().UnitTag().c_str()));  
   m_DocUnitServer->QueryInterface(&m_DocConvert);

   // Setup the reporting mechanism
   std::shared_ptr<WBFL::Reporting::ReportSpecificationBuilder> pRptSpecBuilder( std::make_shared<CCurvelReportSpecificationBuilder>() );
   m_pRptSpecBuilder = pRptSpecBuilder;

   std::shared_ptr<WBFL::Reporting::ReportBuilder> pRptBuilder(std::make_shared<WBFL::Reporting::ReportBuilder>(_T("Curvel")));
   pRptBuilder->SetReportSpecificationBuilder(pRptSpecBuilder);
   GetReportManager()->AddReportBuilder(pRptBuilder);

   std::shared_ptr<WBFL::Reporting::TitlePageBuilder> pTitlePageBuilder(std::make_shared<CCurvelTitlePageBuilder>());
   pRptBuilder->AddTitlePageBuilder( pTitlePageBuilder );

   std::shared_ptr<WBFL::Reporting::ChapterBuilder> pChBuilder(std::make_shared<CCurvelChapterBuilder>(this) );
   pRptBuilder->AddChapterBuilder(pChBuilder);

   WBFL::Reporting::ReportDescription rptDesc = pRptBuilder->GetReportDescription();
   std::shared_ptr<CCurvelReportSpecification> pCurvelRptSpec(std::make_shared<CCurvelReportSpecification>(rptDesc.GetReportName()) );
   std::shared_ptr<WBFL::Reporting::ReportSpecification> pRptSpec = std::dynamic_pointer_cast<WBFL::Reporting::ReportSpecification,CCurvelReportSpecification>(pCurvelRptSpec);
   rptDesc.ConfigureReportSpecification(pRptSpec);

   m_pDefaultRptSpec = pCurvelRptSpec;

   EnableUIHints(FALSE); // not using UIHints feature
}

CCurvelDoc::~CCurvelDoc()
{
}


BEGIN_MESSAGE_MAP(CCurvelDoc, CBEToolboxDoc)
   ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
END_MESSAGE_MAP()

void CCurvelDoc::OnHelpFinder()
{
   EAFHelp(EAFGetDocument()->GetDocumentationSetName(),IDH_CURVEL);
}


// CCurvelDoc diagnostics

#ifdef _DEBUG
void CCurvelDoc::AssertValid() const
{
	CBEToolboxDoc::AssertValid();
}

#ifndef _WIN32_WCE
void CCurvelDoc::Dump(CDumpContext& dc) const
{
	CBEToolboxDoc::Dump(dc);
}
#endif
#endif //_DEBUG

CString CCurvelDoc::GetToolbarSectionName()
{
   return _T("Curvel");
}

BOOL CCurvelDoc::OnNewDocument()
{
   if ( !CBEToolboxDoc::OnNewDocument() )
      return FALSE;

   m_CurvelXML = CreateCurvelModel();
   if ( m_CurvelXML.get() == nullptr )
      return FALSE;

   return TRUE;
}

BOOL CCurvelDoc::OpenTheDocument(LPCTSTR lpszPathName)
{
   USES_CONVERSION;

   m_CurvelXML = CreateCurvelModel(lpszPathName,m_DocUnitServer);
   if ( m_CurvelXML.get() == nullptr )
      return FALSE;

   return TRUE;
}

BOOL CCurvelDoc::SaveTheDocument(LPCTSTR lpszPathName)
{
   return SaveCurvelModel(lpszPathName,m_CurvelXML.get());
}

void CCurvelDoc::LoadDocumentSettings()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   __super::LoadDocumentSettings();
}

void CCurvelDoc::SaveDocumentSettings()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   __super::SaveDocumentSettings();
}

CString CCurvelDoc::GetDocumentationRootLocation()
{
   CEAFApp* pApp = EAFGetApp();
   return pApp->GetDocumentationRootLocation();
}

void CCurvelDoc::OnOldFormat(LPCTSTR lpszPathName)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   AfxMessageBox(_T("Error: Invalid file format.\n\nThis is not a Curvel file or it is in a legacy format that is not supported"),MB_OK | MB_ICONEXCLAMATION);
}

const std::shared_ptr<WBFL::Reporting::ReportSpecificationBuilder>& CCurvelDoc::GetReportSpecificationBuilder() const
{
   return m_pRptSpecBuilder;
}

const std::shared_ptr<WBFL::Reporting::ReportSpecification>& CCurvelDoc::GetDefaultReportSpecification() const
{
   return m_pDefaultRptSpec;
}

Curvel* CCurvelDoc::GetCurvelXML()
{
   return m_CurvelXML.get();
}
