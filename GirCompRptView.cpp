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

// GirCompRptView.cpp : implementation file
//

#include "stdafx.h"
#include "GirCompDoc.h"
#include "GirCompRptView.h"
#include "GirCompChildFrame.h"

#include <EAF\EAFHints.h>

// CGirCompRptView

IMPLEMENT_DYNCREATE(CGirCompRptView, CEAFReportView)

CGirCompRptView::CGirCompRptView()
{

}

CGirCompRptView::~CGirCompRptView()
{
}

BEGIN_MESSAGE_MAP(CGirCompRptView, CEAFReportView)
   ON_COMMAND(ID_FILE_PRINT_DIRECT,&CGirCompRptView::OnFilePrint)
END_MESSAGE_MAP()

boost::shared_ptr<CReportSpecification> CGirCompRptView::CreateReportSpecification()
{
   CGirCompDoc* pDoc = (CGirCompDoc*)GetDocument();

   boost::shared_ptr<CReportSpecification> pRptSpec;
   std::vector<std::_tstring> rptNames = pDoc->m_RptMgr.GetReportNames();

   boost::shared_ptr<CReportBuilder> pRptBuilder = pDoc->m_RptMgr.GetReportBuilder(rptNames.front());
   CReportDescription rptDesc = pRptBuilder->GetReportDescription();

   boost::shared_ptr<CReportSpecificationBuilder> pRptSpecBuilder = pRptBuilder->GetReportSpecificationBuilder();
   pRptSpec = pRptSpecBuilder->CreateDefaultReportSpec(rptDesc);

   return pRptSpec;
}

boost::shared_ptr<CReportBrowser> CGirCompRptView::CreateReportBrowser()
{
   if ( m_pReportSpec == NULL )
      return boost::shared_ptr<CReportBrowser>();

   CGirCompDoc* pDoc = (CGirCompDoc*)GetDocument();
   boost::shared_ptr<CReportSpecificationBuilder> nullSpecBuilder;
   return pDoc->m_RptMgr.CreateReportBrowser(GetSafeHwnd(),m_pReportSpec,nullSpecBuilder);
}

void CGirCompRptView::RefreshReport()
{
   if ( m_pReportSpec == NULL )
      return;

   CGirCompDoc* pDoc = (CGirCompDoc*)GetDocument();


   CEAFReportViewCreationData data;
   data.m_RptIdx = 0;
   data.m_bPromptForSpec = false;
   data.m_pReportBuilderMgr = &pDoc->m_RptMgr;
   CEAFDocTemplate* pDocTemplate = (CEAFDocTemplate*)pDoc->GetDocTemplate();
   pDocTemplate->SetViewCreationData((void*)&data);

   // refresh the report
   boost::shared_ptr<CReportBuilder> pBuilder = pDoc->m_RptMgr.GetReportBuilder( m_pReportSpec->GetReportName() );
   boost::shared_ptr<rptReport> pReport = pBuilder->CreateReport( m_pReportSpec );
   m_pReportBrowser->UpdateReport( pReport, true );
}

// CGirCompRptView diagnostics

#ifdef _DEBUG
void CGirCompRptView::AssertValid() const
{
	CEAFReportView::AssertValid();
}

#ifndef _WIN32_WCE
void CGirCompRptView::Dump(CDumpContext& dc) const
{
	CEAFReportView::Dump(dc);
}
#endif
#endif //_DEBUG


// CGirCompRptView message handlers

void CGirCompRptView::OnInitialUpdate()
{
   CGirCompDoc* pDoc = (CGirCompDoc*)GetDocument();

   CEAFReportViewCreationData data;
   data.m_RptIdx = 0;
   data.m_bPromptForSpec = false;
   data.m_pReportBuilderMgr = &pDoc->m_RptMgr;
   CEAFDocTemplate* pDocTemplate = (CEAFDocTemplate*)pDoc->GetDocTemplate();
   pDocTemplate->SetViewCreationData((void*)&data);

   CEAFReportView::OnInitialUpdate();

   m_pFrame = (CGirCompChildFrame*)GetParentFrame();
   m_pFrame->UpdateUnitsDisplayMode();
   m_pFrame->UpdateData(FALSE);
}

void CGirCompRptView::OnUpdate(CView* pSender,LPARAM lHint,CObject* pHint)
{
   RefreshReport();
}

void CGirCompRptView::CreateEditButton()
{
   // we don't want an edit button in the report view, so don't create one
   // and don't let the base class do it either.
}
