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

// BearingRptView.cpp : implementation file
//

#include "stdafx.h"
#include "BearingDoc.h"
#include "BearingRptView.h"
#include "BearingChildFrame.h"

#include <EAF\EAFHints.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CBearingRptView

IMPLEMENT_DYNCREATE(CBearingRptView, CEAFReportView)

CBearingRptView::CBearingRptView()
{

}

CBearingRptView::~CBearingRptView()
{
}

BEGIN_MESSAGE_MAP(CBearingRptView, CEAFReportView)
   ON_COMMAND(ID_FILE_PRINT_DIRECT,&CBearingRptView::OnFilePrint)
END_MESSAGE_MAP()

std::shared_ptr<const WBFL::Reporting::ReportSpecification> CBearingRptView::CreateReportSpecification() const
{
   CBearingDoc* pDoc = (CBearingDoc*)GetDocument();

   std::vector<std::_tstring> rptNames = pDoc->GetReportManager()->GetReportNames();

   std::shared_ptr<WBFL::Reporting::ReportBuilder> pRptBuilder = pDoc->GetReportManager()->GetReportBuilder(rptNames.front());
   WBFL::Reporting::ReportDescription rptDesc = pRptBuilder->GetReportDescription();

   std::shared_ptr<WBFL::Reporting::ReportSpecificationBuilder> pRptSpecBuilder = pRptBuilder->GetReportSpecificationBuilder();
   auto pRptSpec = pRptSpecBuilder->CreateDefaultReportSpec(rptDesc);

   return pRptSpec;
}

std::shared_ptr<WBFL::Reporting::ReportBrowser> CBearingRptView::CreateReportBrowser()
{
   if (m_pReportSpec == nullptr)
      return nullptr;

   CBearingDoc* pDoc = (CBearingDoc*)GetDocument();
   return pDoc->GetReportManager()->CreateReportBrowser(GetSafeHwnd(),m_pReportSpec, std::shared_ptr<const WBFL::Reporting::ReportSpecificationBuilder>());
}

void CBearingRptView::RefreshReport()
{
   if ( m_pReportSpec == nullptr )
      return;

   CBearingDoc* pDoc = (CBearingDoc*)GetDocument();


   CEAFReportViewCreationData data;
   data.m_RptIdx = 0;
   data.m_bPromptForSpec = false;
   data.m_pReportBuilderMgr = pDoc->GetReportManager();
   CEAFDocTemplate* pDocTemplate = (CEAFDocTemplate*)pDoc->GetDocTemplate();
   pDocTemplate->SetViewCreationData((void*)&data);

   // refresh the report
   std::shared_ptr<WBFL::Reporting::ReportBuilder> pBuilder = pDoc->GetReportManager()->GetReportBuilder( m_pReportSpec->GetReportName() );
   std::shared_ptr<rptReport> pReport = pBuilder->CreateReport( m_pReportSpec );
   m_pReportBrowser->UpdateReport( pReport, true );
}

// CBearingRptView diagnostics

#ifdef _DEBUG
void CBearingRptView::AssertValid() const
{
	CEAFReportView::AssertValid();
}

#ifndef _WIN32_WCE
void CBearingRptView::Dump(CDumpContext& dc) const
{
	CEAFReportView::Dump(dc);
}
#endif
#endif //_DEBUG


// CBearingRptView message handlers

void CBearingRptView::OnInitialUpdate()
{
   CBearingDoc* pDoc = (CBearingDoc*)GetDocument();

   CEAFReportViewCreationData data;
   data.m_RptIdx = 0;
   data.m_bPromptForSpec = false;
   data.m_pReportBuilderMgr = pDoc->GetReportManager();
   CEAFDocTemplate* pDocTemplate = (CEAFDocTemplate*)pDoc->GetDocTemplate();
   pDocTemplate->SetViewCreationData((void*)&data);


   CEAFReportView::OnInitialUpdate();


   m_pFrame = (CBearingChildFrame*)GetParentFrame();
   m_pFrame->UpdateData(FALSE);


   const auto brg{ pDoc->GetBearing() };
   const auto brg_loads{ pDoc->GetBearingLoads()};
   const auto brg_calc{ pDoc->GetBearingCalculator() };
   CBearingChildFrame* pFrame = (CBearingChildFrame*)GetParentFrame();
   pFrame->SetBearingParameters(brg,brg_loads,brg_calc);
   

}

void CBearingRptView::OnUpdate(CView* pSender,LPARAM lHint,CObject* pHint)
{
   RefreshReport();
}
