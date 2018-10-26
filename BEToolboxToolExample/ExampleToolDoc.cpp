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

// ExampleToolDoc.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ExampleToolDoc.h"
#include "ExampleToolChildFrame.h"
#include "BEToolboxStatusBar.h"

#include <EAF\EAFUtilities.h>
#include <EAF\EAFApp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CExampleToolDoc

IMPLEMENT_DYNCREATE(CExampleToolDoc, CBEToolboxDoc)

CExampleToolDoc::CExampleToolDoc()
{
}

CExampleToolDoc::~CExampleToolDoc()
{
}


BEGIN_MESSAGE_MAP(CExampleToolDoc, CBEToolboxDoc)
   //ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
END_MESSAGE_MAP()

void CExampleToolDoc::OnHelpFinder()
{
   //EAFHelp(EAFGetDocument()->GetDocumentationSetName(),IDH_EXAMPLE);
}


// CBoxGdrDoc diagnostics

#ifdef _DEBUG
void CExampleToolDoc::AssertValid() const
{
	CBEToolboxDoc::AssertValid();
}

#ifndef _WIN32_WCE
void CExampleToolDoc::Dump(CDumpContext& dc) const
{
	CBEToolboxDoc::Dump(dc);
}
#endif
#endif //_DEBUG

BOOL CExampleToolDoc::Init()
{
   if (!CBEToolboxDoc::Init())
   {
      return FALSE;
   }

   // initialize with some data
   return TRUE;
}

void CExampleToolDoc::OnCloseDocument()
{
   CBEToolboxDoc::OnCloseDocument();
}

HRESULT CExampleToolDoc::WriteTheDocument(IStructuredSave* pStrSave)
{
   HRESULT hr = pStrSave->BeginUnit(_T("ExampleTool"),1.0);
   if ( FAILED(hr) )
      return hr;

   hr = pStrSave->EndUnit(); // ExampleTool
   if ( FAILED(hr) )
      return hr;

   return S_OK;
}

HRESULT CExampleToolDoc::LoadTheDocument(IStructuredLoad* pStrLoad)
{
   HRESULT hr = pStrLoad->BeginUnit(_T("ExampleTool"));
   if ( FAILED(hr) )
      return hr;


   hr = pStrLoad->EndUnit(); // ExampleTool
   if ( FAILED(hr) )
      return hr;

   return S_OK;
}

void CExampleToolDoc::LoadDocumentSettings()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   __super::LoadDocumentSettings();
}

void CExampleToolDoc::SaveDocumentSettings()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   __super::SaveDocumentSettings();
}

CString CExampleToolDoc::GetDocumentationRootLocation()
{
   CEAFApp* pApp = EAFGetApp();
   return pApp->GetDocumentationRootLocation();
}

CString CExampleToolDoc::GetToolbarSectionName()
{
   return _T("ExampleTool");
}
