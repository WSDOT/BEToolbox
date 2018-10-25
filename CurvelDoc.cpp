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

// CurvelDoc.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "CurvelDoc.h"
#include "CurvelTitlePageBuilder.h"
#include "CurvelChapterBuilder.h"
#include "CurvelChildFrame.h"
#include "CurvelReportSpecification.h"
#include "CurvelReportSpecificationBuilder.h"
#include "CurvelRptView.h"
#include "BEToolboxStatusBar.h"

// CCurvelDoc

IMPLEMENT_DYNCREATE(CCurvelDoc, CBEToolboxDoc)

CCurvelDoc::CCurvelDoc()
{
   // Setup the reporting mechanism
   boost::shared_ptr<CReportSpecificationBuilder> pRptSpecBuilder( new CCurvelReportSpecificationBuilder() );
   m_pRptSpecBuilder = pRptSpecBuilder;

   CReportBuilder* pRptBuilder = new CReportBuilder(_T("Curvel"));
   
   pRptBuilder->SetReportSpecificationBuilder(pRptSpecBuilder);

   boost::shared_ptr<CTitlePageBuilder> pTitlePageBuilder(new CCurvelTitlePageBuilder());
   pRptBuilder->AddTitlePageBuilder( pTitlePageBuilder );

   boost::shared_ptr<CChapterBuilder> pChBuilder( new CCurvelChapterBuilder(this) );
   pRptBuilder->AddChapterBuilder(pChBuilder);

   m_RptMgr.AddReportBuilder(pRptBuilder);

   CReportDescription rptDesc = pRptBuilder->GetReportDescription();
   boost::shared_ptr<CCurvelReportSpecification> pCurvelRptSpec( new CCurvelReportSpecification(rptDesc.GetReportName()) );
   boost::shared_ptr<CReportSpecification> pRptSpec = boost::dynamic_pointer_cast<CReportSpecification,CCurvelReportSpecification>(pCurvelRptSpec);
   rptDesc.ConfigureReportSpecification(pRptSpec);

   m_pDefaultRptSpec = pCurvelRptSpec;
}

CCurvelDoc::~CCurvelDoc()
{
}


BEGIN_MESSAGE_MAP(CCurvelDoc, CBEToolboxDoc)
END_MESSAGE_MAP()


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

BOOL CCurvelDoc::Init()
{
   if ( !CBEToolboxDoc::Init() )
      return FALSE;

   return TRUE;
}

void CCurvelDoc::OnCloseDocument()
{
   CBEToolboxDoc::OnCloseDocument();
}

HRESULT CCurvelDoc::WriteTheDocument(IStructuredSave* pStrSave)
{
   // The real data is in the report spec owned by the report view
   CView* pActiveView = EAFGetActiveView();
   CEAFReportView* pRptView = (CEAFReportView*)pActiveView;
   boost::shared_ptr<CReportSpecification> pRptSpec = pRptView->GetReportSpecification();

   boost::shared_ptr<CCurvelReportSpecification> pCurvelRptSpec = boost::dynamic_pointer_cast<CCurvelReportSpecification,CReportSpecification>(pRptSpec);

   return pCurvelRptSpec->Save(pStrSave);
}

HRESULT CCurvelDoc::LoadTheDocument(IStructuredLoad* pStrLoad)
{
   // Load data from input stream into the default report spec
   return m_pDefaultRptSpec->Load(pStrLoad);
}

CString CCurvelDoc::GetToolbarSectionName()
{
   return _T("Curvel");
}

BOOL CCurvelDoc::OpenOldFormat(LPCTSTR lpszPathName)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   AfxMessageBox(_T("Error: Invalid file format.\n\nThis is not a Curvel file or it is in a legacy format that is not supported"),MB_OK | MB_ICONEXCLAMATION);
   return FALSE;
}

CReportBuilderManager* CCurvelDoc::GetReportBuilderManager()
{
   return &m_RptMgr;
}

boost::shared_ptr<CReportSpecificationBuilder> CCurvelDoc::GetReportSpecificationBuilder()
{
   return m_pRptSpecBuilder;
}

boost::shared_ptr<CReportSpecification> CCurvelDoc::GetDefaultReportSpecification()
{
   return m_pDefaultRptSpec;
}
