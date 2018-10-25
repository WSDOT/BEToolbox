///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2014  Washington State Department of Transportation
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

// BEToolboxDoc.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "BEToolboxDoc.h"
#include "BEToolboxStatusBar.h"
#include "BEToolboxPlugin.h"
#include "AboutDlg.h"

// CBEToolboxDoc

IMPLEMENT_DYNAMIC(CBEToolboxDoc, CEAFDocument)

#define ID_MYTOOLBAR ID_MAINFRAME_TOOLBAR+1
#define PLUGIN_COMMAND_COUNT 256

static const Float64 FILE_VERSION = 1.0;

CBEToolboxDoc::CBEToolboxDoc()
{
   m_pMyToolBar = NULL;

   // Reserve command IDs for document plug ins
   GetPluginCommandManager()->ReserveCommandIDRange(PLUGIN_COMMAND_COUNT);
}

CBEToolboxDoc::~CBEToolboxDoc()
{
}


BEGIN_MESSAGE_MAP(CBEToolboxDoc, CEAFDocument)
   ON_COMMAND(ID_HELP_ABOUTBETOOLBOX,OnAbout)
END_MESSAGE_MAP()

void CBEToolboxDoc::OnAbout()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CAboutDlg dlg;
   dlg.DoModal();
}


// CBoxGdrDoc diagnostics

#ifdef _DEBUG
void CBEToolboxDoc::AssertValid() const
{
	CEAFDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CBEToolboxDoc::Dump(CDumpContext& dc) const
{
	CEAFDocument::Dump(dc);
}
#endif
#endif //_DEBUG

BOOL CBEToolboxDoc::Init()
{
   if ( !CEAFDocument::Init() )
      return FALSE;


   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   LPCTSTR lpszHelpFile = AfxGetApp()->m_pszHelpFilePath;
   EAFGetApp()->SetHelpFileName(lpszHelpFile);

   return TRUE;
}

CString CBEToolboxDoc::GetRootNodeName()
{
   return _T("BEToolbox");
}

Float64 CBEToolboxDoc::GetRootNodeVersion()
{
   return FILE_VERSION;
}

BOOL CBEToolboxDoc::OpenTheDocument(LPCTSTR lpszPathName)
{
   // Open the file and see if it starts with XML... if it does
   // pass it on for normal processing, otherwise, assume it is an old fortran fixed format file
   DWORD dwResults = GetFileAttributes(lpszPathName);
   if ( dwResults == INVALID_FILE_ATTRIBUTES ) // if we couldn't get the file attributes, it probably doesn't exist
   {
      CString strError;
      strError.Format(_T("Error opening %s. File not found."),lpszPathName);
      AfxMessageBox(strError);
      return FALSE;
   }

   std::_tifstream is;
   is.open(lpszPathName);

   if ( !is.is_open() )
   {
      CString strError;
      strError.Format(_T("Error: % could not be opened."),lpszPathName);
      AfxMessageBox(strError);
      return FALSE;
   }

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

BOOL CBEToolboxDoc::OpenOldFormat(LPCTSTR lpszPathName)
{
   // Do nothing by default
   return TRUE;
}

void CBEToolboxDoc::OnCloseDocument()
{
   EAFGetApp()->SetHelpFileName(NULL);

   CEAFDocument::OnCloseDocument();
}

void CBEToolboxDoc::LoadToolbarState()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   __super::LoadToolbarState();
}

void CBEToolboxDoc::SaveToolbarState()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   __super::SaveToolbarState();
}

void CBEToolboxDoc::DoIntegrateWithUI(BOOL bIntegrate)
{
   CEAFMainFrame* pFrame = EAFGetMainFrame();

   if ( bIntegrate )
   {
      // set up the toolbar here
      {
      AFX_MANAGE_STATE(AfxGetStaticModuleState());
      UINT tbID = pFrame->CreateToolBar(GetToolbarSectionName(),GetPluginCommandManager());
      m_pMyToolBar = pFrame->GetToolBar(tbID);
      m_pMyToolBar->LoadToolBar(IDR_TOOLBAR,NULL);
      m_pMyToolBar->CreateDropDownButton(ID_FILE_OPEN,NULL,BTNS_DROPDOWN);

      // Put our icon on the main frame window
      m_hMainFrameBigIcon = pFrame->GetIcon(TRUE);
      m_hMainFrameSmallIcon = pFrame->GetIcon(FALSE);
      HICON hIcon = AfxGetApp()->LoadIcon(IDR_BETOOLBOX);
      pFrame->SetIcon(hIcon,TRUE);
      pFrame->SetIcon(hIcon,FALSE);
      }

      // use our status bar
      CBEToolboxStatusBar* pSB = new CBEToolboxStatusBar;
      pSB->Create(pFrame);
      pFrame->SetStatusBar(pSB);
   }
   else
   {
      // remove toolbar here
      pFrame->DestroyToolBar(m_pMyToolBar);
      m_pMyToolBar = NULL;

      // Put the main frame icon back the way it was
      pFrame->SetIcon(m_hMainFrameBigIcon,TRUE);
      pFrame->SetIcon(m_hMainFrameSmallIcon,FALSE);

      // reset the status bar
      pFrame->SetStatusBar(NULL);
   }
}

BOOL CBEToolboxDoc::GetStatusBarMessageString(UINT nID,CString& rMessage) const
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return __super::GetStatusBarMessageString(nID,rMessage);
}

BOOL CBEToolboxDoc::GetToolTipMessageString(UINT nID, CString& rMessage) const
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return __super::GetToolTipMessageString(nID,rMessage);
}
