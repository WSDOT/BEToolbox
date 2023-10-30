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

// GenCompRptView.cpp : implementation file
//

#include "stdafx.h"
#include "GenCompDoc.h"
#include "GenCompRptView.h"
#include "GenCompChildFrame.h"

#include <EAF\EAFHints.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CGenCompRptView

IMPLEMENT_DYNCREATE(CGenCompRptView, CEAFReportView)

CGenCompRptView::CGenCompRptView()
{

}

CGenCompRptView::~CGenCompRptView()
{
}

BEGIN_MESSAGE_MAP(CGenCompRptView, CEAFReportView)
   ON_COMMAND(ID_FILE_PRINT_DIRECT,&CGenCompRptView::OnFilePrint)
END_MESSAGE_MAP()

std::shared_ptr<const WBFL::Reporting::ReportSpecification> CGenCompRptView::CreateReportSpecification() const
{
   CGenCompDoc* pDoc = (CGenCompDoc*)GetDocument();

   std::vector<std::_tstring> rptNames = pDoc->GetReportManager()->GetReportNames();

   std::shared_ptr<WBFL::Reporting::ReportBuilder> pRptBuilder = pDoc->GetReportManager()->GetReportBuilder(rptNames.front());
   WBFL::Reporting::ReportDescription rptDesc = pRptBuilder->GetReportDescription();

   std::shared_ptr<WBFL::Reporting::ReportSpecificationBuilder> pRptSpecBuilder = pRptBuilder->GetReportSpecificationBuilder();
   auto pRptSpec = pRptSpecBuilder->CreateDefaultReportSpec(rptDesc);

   return pRptSpec;
}

std::shared_ptr<WBFL::Reporting::ReportBrowser> CGenCompRptView::CreateReportBrowser()
{
   if (m_pReportSpec == nullptr)
      return nullptr;

   CGenCompDoc* pDoc = (CGenCompDoc*)GetDocument();
   return pDoc->GetReportManager()->CreateReportBrowser(GetSafeHwnd(),m_pReportSpec,std::shared_ptr<const WBFL::Reporting::ReportSpecificationBuilder>());
}

void CGenCompRptView::RefreshReport()
{
   if ( m_pReportSpec == nullptr )
      return;

   CGenCompDoc* pDoc = (CGenCompDoc*)GetDocument();

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

// CGenCompRptView diagnostics

#ifdef _DEBUG
void CGenCompRptView::AssertValid() const
{
	CEAFReportView::AssertValid();
}

#ifndef _WIN32_WCE
void CGenCompRptView::Dump(CDumpContext& dc) const
{
	CEAFReportView::Dump(dc);
}
#endif
#endif //_DEBUG


// CGenCompRptView message handlers

void CGenCompRptView::OnInitialUpdate()
{
   CGenCompDoc* pDoc = (CGenCompDoc*)GetDocument();

   CEAFReportViewCreationData data;
   data.m_RptIdx = 0;
   data.m_bPromptForSpec = false;
   data.m_pReportBuilderMgr = pDoc->GetReportManager();
   CEAFDocTemplate* pDocTemplate = (CEAFDocTemplate*)pDoc->GetDocTemplate();
   pDocTemplate->SetViewCreationData((void*)&data);

   m_pFrame = (CGenCompChildFrame*)GetParentFrame();
   m_pFrame->UpdateUnitsDisplayMode();
   m_pFrame->UpdateData(FALSE);

   CEAFReportView::OnInitialUpdate();
}

void CGenCompRptView::OnUpdate(CView* pSender,LPARAM lHint,CObject* pHint)
{
   RefreshReport();
}
