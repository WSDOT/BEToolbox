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

// CurvelRptView.cpp : implementation file
//

#include "stdafx.h"
#include "CurvelDoc.h"
#include "CurvelRptView.h"
#include "CurvelChildFrame.h"
#include "CurvelReportSpecification.h"

// CCurvelRptView

IMPLEMENT_DYNCREATE(CCurvelRptView, CEAFReportView)

CCurvelRptView::CCurvelRptView()
{

}

CCurvelRptView::~CCurvelRptView()
{
}

BEGIN_MESSAGE_MAP(CCurvelRptView, CEAFReportView)
   ON_COMMAND(ID_FILE_PRINT_DIRECT,&CCurvelRptView::OnFilePrint)
END_MESSAGE_MAP()

std::shared_ptr<CReportBrowser> CCurvelRptView::CreateReportBrowser()
{
   if (m_pReportSpec == nullptr)
      return nullptr;

   CCurvelDoc* pDoc = (CCurvelDoc*)GetDocument();
   return pDoc->GetReportBuilderManager()->CreateReportBrowser(GetSafeHwnd(),m_pReportSpec, std::shared_ptr<CReportSpecificationBuilder>());
}

// CCurvelRptView diagnostics

#ifdef _DEBUG
void CCurvelRptView::AssertValid() const
{
	CEAFReportView::AssertValid();
}

#ifndef _WIN32_WCE
void CCurvelRptView::Dump(CDumpContext& dc) const
{
	CEAFReportView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCurvelRptView message handlers

void CCurvelRptView::OnInitialUpdate()
{
   CCurvelDoc* pDoc = (CCurvelDoc*)GetDocument();

   CEAFReportViewCreationData data;
   data.m_RptIdx = 0;
   data.m_bInitializeOnly = false;
   data.m_pRptSpecification = pDoc->GetDefaultReportSpecification();
   data.m_pRptSpecificationBuilder = pDoc->GetReportSpecificationBuilder();
   data.m_pReportBuilderMgr = pDoc->GetReportBuilderManager();
   CEAFDocTemplate* pDocTemplate = (CEAFDocTemplate*)pDoc->GetDocTemplate();
   pDocTemplate->SetViewCreationData((void*)&data);

   CEAFReportView::OnInitialUpdate();
}

void CCurvelRptView::OnUpdate(CView* pSender,LPARAM lHint,CObject* pHint)
{
   RefreshReport();
}
