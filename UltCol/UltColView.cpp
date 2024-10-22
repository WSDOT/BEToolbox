///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2024  Washington State Department of Transportation
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

// UltColView.cpp : implementation file
//

#include "stdafx.h"
#include "UltColDoc.h"
#include "UltColView.h"
#include "UltColChildFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CUltColView

IMPLEMENT_DYNCREATE(CUltColView, CEAFReportView)

CUltColView::CUltColView()
{

}

CUltColView::~CUltColView()
{
}

BEGIN_MESSAGE_MAP(CUltColView, CEAFReportView)
   ON_COMMAND(ID_FILE_PRINT_DIRECT,&CUltColView::OnFilePrint)
END_MESSAGE_MAP()

std::shared_ptr<const WBFL::Reporting::ReportSpecification> CUltColView::CreateReportSpecification() const
{
   CUltColDoc* pDoc = (CUltColDoc*)GetDocument();

   std::vector<std::_tstring> rptNames = pDoc->GetReportManager()->GetReportNames();

   auto pRptBuilder = pDoc->GetReportManager()->GetReportBuilder(rptNames.front());
   WBFL::Reporting::ReportDescription rptDesc = pRptBuilder->GetReportDescription();

   auto pRptSpecBuilder = pRptBuilder->GetReportSpecificationBuilder();
   auto pRptSpec = pRptSpecBuilder->CreateDefaultReportSpec(rptDesc);

   return pRptSpec;
}

std::shared_ptr<WBFL::Reporting::ReportBrowser> CUltColView::CreateReportBrowser()
{
   if (m_pReportSpec == nullptr)
      return nullptr;

   CUltColDoc* pDoc = (CUltColDoc*)GetDocument();
   return pDoc->GetReportManager()->CreateReportBrowser(GetSafeHwnd(), 0, m_pReportSpec, std::shared_ptr<const WBFL::Reporting::ReportSpecificationBuilder>());
}

void CUltColView::RefreshReport()
{
   CUltColDoc* pDoc = (CUltColDoc*)GetDocument();


   CEAFReportViewCreationData data;
   data.m_RptIdx = 0;
   data.m_bPromptForSpec = false;
   data.m_pReportBuilderMgr = pDoc->GetReportManager();
   CEAFDocTemplate* pDocTemplate = (CEAFDocTemplate*)pDoc->GetDocTemplate();
   pDocTemplate->SetViewCreationData((void*)&data);

   // refresh the report
   auto pBuilder = pDoc->GetReportManager()->GetReportBuilder(m_pReportSpec->GetReportName());
   auto pReport = pBuilder->CreateReport( m_pReportSpec );
   m_pReportBrowser->UpdateReport( pReport, true );
}

// CUltColView diagnostics

#ifdef _DEBUG
void CUltColView::AssertValid() const
{
	CEAFReportView::AssertValid();
}

#ifndef _WIN32_WCE
void CUltColView::Dump(CDumpContext& dc) const
{
	CEAFReportView::Dump(dc);
}
#endif
#endif //_DEBUG


// CUltColView message handlers

void CUltColView::OnInitialUpdate()
{
   CUltColDoc* pDoc = (CUltColDoc*)GetDocument();

   CEAFReportViewCreationData data;
   data.m_RptIdx = 0;
   data.m_bPromptForSpec = false;
   data.m_pReportBuilderMgr = pDoc->GetReportManager();
   CEAFDocTemplate* pDocTemplate = (CEAFDocTemplate*)pDoc->GetDocTemplate();
   pDocTemplate->SetViewCreationData((void*)&data);

   CEAFReportView::OnInitialUpdate();

   // TODO: Add your specialized code here and/or call the base class
   WBFL::RCSection::CircularColumn column;
   Float64 ecl, etl;
   pDoc->GetColumn(column, ecl, etl);

   CUltColChildFrame* pFrame = (CUltColChildFrame*)GetParentFrame();
   pFrame->SetColumnParameters(column, ecl, etl);
}
