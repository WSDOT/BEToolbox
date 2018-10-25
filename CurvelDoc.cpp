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


#define ID_MYTOOLBAR ID_MAINFRAME_TOOLBAR+1
#define CURVEL_PLUGIN_COMMAND_COUNT 256

// CCurvelDoc

IMPLEMENT_DYNCREATE(CCurvelDoc, CEAFDocument)

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


   // Other initialization
   m_pMyToolBar = NULL;

   // Reserve command IDs for document plug ins
   GetPluginCommandManager()->ReserveCommandIDRange(CURVEL_PLUGIN_COMMAND_COUNT);

}

CCurvelDoc::~CCurvelDoc()
{
}


BEGIN_MESSAGE_MAP(CCurvelDoc, CEAFDocument)
END_MESSAGE_MAP()


// CCurvelDoc diagnostics

#ifdef _DEBUG
void CCurvelDoc::AssertValid() const
{
	CEAFDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CCurvelDoc::Dump(CDumpContext& dc) const
{
	CEAFDocument::Dump(dc);
}
#endif
#endif //_DEBUG

BOOL CCurvelDoc::Init()
{
   if ( !CEAFDocument::Init() )
      return FALSE;

   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   LPCTSTR lpszHelpFile = AfxGetApp()->m_pszHelpFilePath;
   EAFGetApp()->SetHelpFileName(lpszHelpFile);

   return TRUE;
}

void CCurvelDoc::OnCloseDocument()
{
   EAFGetApp()->SetHelpFileName(NULL);
   EAFGetApp()->SetUnitsMode(eafTypes::umUS);

   CEAFDocument::OnCloseDocument();
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

void CCurvelDoc::LoadToolbarState()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   __super::LoadToolbarState();
}

void CCurvelDoc::SaveToolbarState()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   __super::SaveToolbarState();
}

void CCurvelDoc::DoIntegrateWithUI(BOOL bIntegrate)
{
   CEAFMainFrame* pFrame = EAFGetMainFrame();

   if ( bIntegrate )
   {
      // set up the toolbar here
      {
      AFX_MANAGE_STATE(AfxGetStaticModuleState());
      UINT tbID = pFrame->CreateToolBar(_T("Curvel"),GetPluginCommandManager());
      m_pMyToolBar = pFrame->GetToolBar(tbID);
      m_pMyToolBar->LoadToolBar(IDR_TOOLBAR,NULL);
      m_pMyToolBar->CreateDropDownButton(ID_FILE_OPEN,NULL,BTNS_DROPDOWN);
      }

      // use our status bar
      CBEToolboxStatusBar* pSB = new CBEToolboxStatusBar;
      pSB->Create(EAFGetMainFrame());
      pFrame->SetStatusBar(pSB);
   }
   else
   {
      // remove toolbar here
      pFrame->DestroyToolBar(m_pMyToolBar);
      m_pMyToolBar = NULL;
   }
}

BOOL CCurvelDoc::GetStatusBarMessageString(UINT nID,CString& rMessage) const
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return __super::GetStatusBarMessageString(nID,rMessage);
}

BOOL CCurvelDoc::GetToolTipMessageString(UINT nID, CString& rMessage) const
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return __super::GetToolTipMessageString(nID,rMessage);
}

BOOL CCurvelDoc::OpenTheDocument(LPCTSTR lpszPathName)
{
   // Open the file and see if it starts with XML... if it does
   // pass it on for normal processing, otherwise, assume it is an old fortran fixed format file
   std::_tifstream is;
   is.open(lpszPathName);
   TCHAR buffer[80];
   is.getline(buffer,ARRAYSIZE(buffer));

   std::_tstring strHeader;
   strHeader.insert(strHeader.end(),&buffer[0],&buffer[0]+5);

   is.close();

   if ( strHeader != _T("<?xml") )
      return OpenOldFormat(lpszPathName);
   else
      return CEAFDocument::OpenTheDocument(lpszPathName);
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
