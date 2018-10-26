///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2016  Washington State Department of Transportation
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

#include "stdafx.h"
#include "PGStableDoc.h"
#include "PGStableReportView.h"
#include "PGStableChildFrame.h"

// CPGStableReportView

IMPLEMENT_DYNCREATE(CPGStableReportView, CEAFReportView)

CPGStableReportView::CPGStableReportView()
{

}

CPGStableReportView::~CPGStableReportView()
{
}

BEGIN_MESSAGE_MAP(CPGStableReportView, CEAFReportView)
   ON_COMMAND(ID_FILE_PRINT_DIRECT,&CPGStableReportView::OnFilePrint)
END_MESSAGE_MAP()

boost::shared_ptr<CReportSpecification> CPGStableReportView::CreateReportSpecification()
{
   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();

   boost::shared_ptr<CReportSpecification> pRptSpec;
   std::vector<std::_tstring> rptNames = pDoc->m_RptMgr.GetReportNames();

   boost::shared_ptr<CReportBuilder> pRptBuilder = pDoc->m_RptMgr.GetReportBuilder(rptNames.front());
   CReportDescription rptDesc = pRptBuilder->GetReportDescription();

   boost::shared_ptr<CReportSpecificationBuilder> pRptSpecBuilder = pRptBuilder->GetReportSpecificationBuilder();
   pRptSpec = pRptSpecBuilder->CreateDefaultReportSpec(rptDesc);

   return pRptSpec;
}

boost::shared_ptr<CReportBrowser> CPGStableReportView::CreateReportBrowser()
{
   if ( m_pReportSpec == NULL )
      return boost::shared_ptr<CReportBrowser>();

   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();
   boost::shared_ptr<CReportSpecificationBuilder> nullSpecBuilder;
   return pDoc->m_RptMgr.CreateReportBrowser(GetSafeHwnd(),m_pReportSpec,nullSpecBuilder);
}

void CPGStableReportView::RefreshReport()
{
   if ( m_pReportSpec == NULL )
      return;

   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();


   CEAFReportViewCreationData data;
   data.m_RptIdx = 0;
   data.m_bPromptForSpec = false;
   data.m_pReportBuilderMgr = &pDoc->m_RptMgr;
   CEAFDocTemplate* pDocTemplate = (CEAFDocTemplate*)pDoc->GetDocTemplate();
   pDocTemplate->SetViewCreationData((void*)&data);

   // refresh the report
   m_pReportSpec = m_pReportBrowser->GetReportSpecification();
   boost::shared_ptr<CReportBuilder> pBuilder = pDoc->m_RptMgr.GetReportBuilder( m_pReportSpec->GetReportName() );
   boost::shared_ptr<rptReport> pReport = pBuilder->CreateReport( m_pReportSpec );
   m_pReportBrowser->UpdateReport( pReport, true );

   UpdateViewTitle();
}

#ifdef _DEBUG
void CPGStableReportView::AssertValid() const
{
   AFX_MANAGE_STATE(AfxGetAppModuleState());
	CEAFReportView::AssertValid();
}

#ifndef _WIN32_WCE
void CPGStableReportView::Dump(CDumpContext& dc) const
{
	CEAFReportView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPGStableView message handlers

void CPGStableReportView::OnInitialUpdate()
{
   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();

   CEAFReportViewCreationData data;
   data.m_RptIdx = 0;
   data.m_bPromptForSpec = false;
   data.m_pReportBuilderMgr = &pDoc->m_RptMgr;
   CEAFDocTemplate* pDocTemplate = (CEAFDocTemplate*)pDoc->GetDocTemplate();
   pDocTemplate->SetViewCreationData((void*)&data);

   CEAFReportView::OnInitialUpdate();
}

void CPGStableReportView::OnActivateView(BOOL bActivate,CView* pActivateView,CView* pDeactiveView)
{
   if ( bActivate )
   {
      RefreshReport();
   }
}

void CPGStableReportView::OnUpdate(CView* pSender,LPARAM lHint,CObject* pHint)
{
   if ( EAFGetActiveView() == this )
   {
      if ( lHint == EAF_HINT_UNITS_CHANGED )
      {
         RefreshReport();
      }
   }

   CEAFReportView::OnUpdate(pSender,lHint,pHint);
}
