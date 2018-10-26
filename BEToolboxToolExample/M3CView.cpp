///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2018  Washington State Department of Transportation
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

// M3CView.cpp : implementation file
//

#include "stdafx.h"
#include "M3CDoc.h"
#include "M3CView.h"
#include "M3CChildFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CM3CView

IMPLEMENT_DYNCREATE(CM3CView, CEAFReportView)

CM3CView::CM3CView()
{

}

CM3CView::~CM3CView()
{
}

BEGIN_MESSAGE_MAP(CM3CView, CEAFReportView)
   ON_COMMAND(ID_FILE_PRINT_DIRECT,&CM3CView::OnFilePrint)
END_MESSAGE_MAP()

std::shared_ptr<CReportSpecification> CM3CView::CreateReportSpecification()
{
   CM3CDoc* pDoc = (CM3CDoc*)GetDocument();

   std::shared_ptr<CReportSpecification> pRptSpec;
   std::vector<std::_tstring> rptNames = pDoc->m_RptMgr.GetReportNames();

   std::shared_ptr<CReportBuilder> pRptBuilder = pDoc->m_RptMgr.GetReportBuilder(rptNames.front());
   CReportDescription rptDesc = pRptBuilder->GetReportDescription();

   std::shared_ptr<CReportSpecificationBuilder> pRptSpecBuilder = pRptBuilder->GetReportSpecificationBuilder();
   pRptSpec = pRptSpecBuilder->CreateDefaultReportSpec(rptDesc);

   return pRptSpec;
}

std::shared_ptr<CReportBrowser> CM3CView::CreateReportBrowser()
{
   if (m_pReportSpec == nullptr)
   {
      return nullptr;
   }

   CWaitCursor wait;
   CM3CDoc* pDoc = (CM3CDoc*)GetDocument();
   return pDoc->m_RptMgr.CreateReportBrowser(GetSafeHwnd(),m_pReportSpec, std::shared_ptr<CReportSpecificationBuilder>());
}

void CM3CView::RefreshReport()
{
   CWaitCursor wait;

   CM3CDoc* pDoc = (CM3CDoc*)GetDocument();


   CEAFReportViewCreationData data;
   data.m_RptIdx = 0;
   data.m_bPromptForSpec = false;
   data.m_pReportBuilderMgr = &pDoc->m_RptMgr;
   CEAFDocTemplate* pDocTemplate = (CEAFDocTemplate*)pDoc->GetDocTemplate();
   pDocTemplate->SetViewCreationData((void*)&data);

   // refresh the report
   std::shared_ptr<CReportBuilder> pBuilder = pDoc->m_RptMgr.GetReportBuilder( m_pReportSpec->GetReportName() );
   std::shared_ptr<rptReport> pReport = pBuilder->CreateReport( m_pReportSpec );
   m_pReportBrowser->UpdateReport( pReport, true );
}

// CM3CView diagnostics

#ifdef _DEBUG
void CM3CView::AssertValid() const
{
	CEAFReportView::AssertValid();
}

#ifndef _WIN32_WCE
void CM3CView::Dump(CDumpContext& dc) const
{
	CEAFReportView::Dump(dc);
}
#endif
#endif //_DEBUG


// CM3CView message handlers

void CM3CView::OnInitialUpdate()
{
   CM3CDoc* pDoc = (CM3CDoc*)GetDocument();

   CM3CChildFrame* pFrame = (CM3CChildFrame*)GetParent();
   pFrame->SetProblemParameters(pDoc->GetProblemParameters());

   CEAFReportViewCreationData data;
   data.m_RptIdx = 0;
   data.m_bPromptForSpec = false;
   data.m_pReportBuilderMgr = &pDoc->m_RptMgr;
   CEAFDocTemplate* pDocTemplate = (CEAFDocTemplate*)pDoc->GetDocTemplate();
   pDocTemplate->SetViewCreationData((void*)&data);

   CEAFReportView::OnInitialUpdate();

   //// TODO: Add your specialized code here and/or call the base class
   //Float64 diameter, fc, cover, As, Es, fy, ecl, etl;
   //pDoc->m_Column->get_Diameter(&diameter);
   //pDoc->m_Column->get_fc(&fc);
   //pDoc->m_Column->get_Cover(&cover);
   //pDoc->m_Column->get_As(&As);
   //pDoc->m_Column->get_Es(&Es);
   //pDoc->m_Column->get_fy(&fy);
   //ecl = pDoc->m_ecl;
   //etl = pDoc->m_etl;

   //CM3CChildFrame* pFrame = (CM3CChildFrame*)GetParentFrame();
   //pFrame->SetColumnParameters(diameter, fc, cover, As, Es, fy, ecl, etl);
}
