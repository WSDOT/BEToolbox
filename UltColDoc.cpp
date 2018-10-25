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

// UltColDoc.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "UltColDoc.h"
#include "UltColTitlePageBuilder.h"
#include "UltColChapterBuilder.h"

#include <EAF\EAFUtilities.h>
#include <EAF\EAFApp.h>

#define ID_MYTOOLBAR ID_MAINFRAME_TOOLBAR+1
#define ULTCOL_PLUGIN_COMMAND_COUNT 256

// CUltColDoc

IMPLEMENT_DYNCREATE(CUltColDoc, CEAFDocument)

CUltColDoc::CUltColDoc()
{
   CReportBuilder* pRptBuilder = new CReportBuilder(_T("Ult Col"));

   boost::shared_ptr<CTitlePageBuilder> pTitlePageBuilder(new CUltColTitlePageBuilder());
   pRptBuilder->AddTitlePageBuilder( pTitlePageBuilder );

   boost::shared_ptr<CChapterBuilder> pChBuilder( new CUltColChapterBuilder(this) );
   pRptBuilder->AddChapterBuilder(pChBuilder);

   m_RptMgr.AddReportBuilder(pRptBuilder);

   m_pMyToolBar = NULL;

   // Reserve command IDs for document plug ins
   GetPluginCommandManager()->ReserveCommandIDRange(ULTCOL_PLUGIN_COMMAND_COUNT);

}

CUltColDoc::~CUltColDoc()
{
}


BEGIN_MESSAGE_MAP(CUltColDoc, CEAFDocument)
   ON_COMMAND(ID_REFRESH_REPORT, &CUltColDoc::OnRefreshReport)
END_MESSAGE_MAP()


// CUltColDoc diagnostics

#ifdef _DEBUG
void CUltColDoc::AssertValid() const
{
	CEAFDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CUltColDoc::Dump(CDumpContext& dc) const
{
	CEAFDocument::Dump(dc);
}
#endif
#endif //_DEBUG

BOOL CUltColDoc::CreateColumn()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   if ( m_Column )
      m_Column.Release();

   m_Column.CoCreateInstance(CLSID_RoundColumn);

   return (m_Column == NULL ? FALSE : TRUE);
}

BOOL CUltColDoc::Init()
{
   if ( !CEAFDocument::Init() )
      return FALSE;

   if ( !CreateColumn() )
      return FALSE;

   // initialize with some defaults
   m_Column->put_Diameter( ::ConvertToSysUnits(72.0,unitMeasure::Inch) );
   m_Column->put_Cover( ::ConvertToSysUnits(2.0,unitMeasure::Inch));
   m_Column->put_As( ::ConvertToSysUnits(10.0,unitMeasure::Inch2));
   m_Column->put_fc( ::ConvertToSysUnits(4.0,unitMeasure::KSI));
   m_Column->put_fy( ::ConvertToSysUnits(60.0,unitMeasure::KSI));
   m_Column->put_Es( ::ConvertToSysUnits(29000.0,unitMeasure::KSI));


   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   LPCTSTR lpszHelpFile = AfxGetApp()->m_pszHelpFilePath;
   EAFGetApp()->SetHelpFileName(lpszHelpFile);

   return TRUE;
}

void CUltColDoc::OnCloseDocument()
{
   EAFGetApp()->SetHelpFileName(NULL);

   m_Column.Release();
   CEAFDocument::OnCloseDocument();
}

void CUltColDoc::OnRefreshReport()
{
   UpdateAllViews(NULL);
}

HRESULT CUltColDoc::WriteTheDocument(IStructuredSave* pStrSave)
{
   HRESULT hr = pStrSave->BeginUnit(_T("UltCol"),1.0);
   if ( FAILED(hr) )
      return hr;

   Float64 diameter, cover, As, fc, fy, Es;
   m_Column->get_Diameter(&diameter);
   m_Column->get_Cover(&cover);
   m_Column->get_As(&As);
   m_Column->get_fc(&fc);
   m_Column->get_fy(&fy);
   m_Column->get_Es(&Es);

   CEAFApp* pApp = EAFGetApp();

   hr = pStrSave->put_Property(_T("Units"),CComVariant(pApp->GetUnitsMode()));
   if ( FAILED(hr) )
      return hr;

   hr = pStrSave->put_Property(_T("Diameter"),CComVariant(diameter));
   if ( FAILED(hr) )
      return hr;

   hr = pStrSave->put_Property(_T("Cover"),CComVariant(cover));
   if ( FAILED(hr) )
      return hr;

   hr = pStrSave->put_Property(_T("As"),CComVariant(As));
   if ( FAILED(hr) )
      return hr;

   hr = pStrSave->put_Property(_T("fc"),CComVariant(fc));
   if ( FAILED(hr) )
      return hr;

   hr = pStrSave->put_Property(_T("fy"),CComVariant(fy));
   if ( FAILED(hr) )
      return hr;

   hr = pStrSave->put_Property(_T("Es"),CComVariant(Es));
   if ( FAILED(hr) )
      return hr;

   hr = pStrSave->EndUnit();
   if ( FAILED(hr) )
      return hr;

   return S_OK;
}

HRESULT CUltColDoc::LoadTheDocument(IStructuredLoad* pStrLoad)
{
   HRESULT hr = pStrLoad->BeginUnit(_T("UltCol"));
   if ( FAILED(hr) )
      return hr;

   CComVariant var;

   CEAFApp* pApp = EAFGetApp();

   var.vt = VT_I4;
   hr = pStrLoad->get_Property(_T("Units"),&var);
   if ( FAILED(hr) )
      return hr;
   pApp->SetUnitsMode(eafTypes::UnitMode(var.lVal));

   var.vt = VT_R8;
   hr = pStrLoad->get_Property(_T("Diameter"),&var);
   if ( FAILED(hr) )
      return hr;
   m_Column->put_Diameter(var.dblVal);

   hr = pStrLoad->get_Property(_T("Cover"),&var);
   if ( FAILED(hr) )
      return hr;
   m_Column->put_Cover(var.dblVal);

   hr = pStrLoad->get_Property(_T("As"),&var);
   if ( FAILED(hr) )
      return hr;
   m_Column->put_As(var.dblVal);

   hr = pStrLoad->get_Property(_T("fc"),&var);
   if ( FAILED(hr) )
      return hr;
   m_Column->put_fc(var.dblVal);

   hr = pStrLoad->get_Property(_T("fy"),&var);
   if ( FAILED(hr) )
      return hr;
   m_Column->put_fy(var.dblVal);

   hr = pStrLoad->get_Property(_T("Es"),&var);
   if ( FAILED(hr) )
      return hr;
   m_Column->put_Es(var.dblVal);

   hr = pStrLoad->EndUnit();
   if ( FAILED(hr) )
      return hr;

   return S_OK;
}

CString CUltColDoc::GetToolbarSectionName()
{
   return _T("UltCol");
}

void CUltColDoc::LoadToolbarState()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   __super::LoadToolbarState();
}

void CUltColDoc::SaveToolbarState()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   __super::SaveToolbarState();
}

void CUltColDoc::DoIntegrateWithUI(BOOL bIntegrate)
{
   CEAFMainFrame* pFrame = EAFGetMainFrame();

   if ( bIntegrate )
   {
      // set up the toolbar here
      {
      AFX_MANAGE_STATE(AfxGetStaticModuleState());
      UINT tbID = pFrame->CreateToolBar(_T("UltCol"),GetPluginCommandManager());
#if defined _EAF_USING_MFC_FEATURE_PACK
      m_pMyToolBar = pFrame->GetToolBarByID(tbID);
      m_pMyToolBar->LoadToolBar(IDR_TOOLBAR,NULL);
#else
      m_pMyToolBar = pFrame->GetToolBar(tbID);
      m_pMyToolBar->LoadToolBar(IDR_TOOLBAR,NULL);
      m_pMyToolBar->CreateDropDownButton(ID_FILE_OPEN,NULL,BTNS_DROPDOWN);
#endif
      }

      //// use our status bar
      //CUltColStatusBar* pSB = new CUltColStatusBar;
      //pSB->Create(EAFGetMainFrame());
      //pFrame->SetStatusBar(pSB);
   }
   else
   {
      // remove toolbar here
      pFrame->DestroyToolBar(m_pMyToolBar);
      m_pMyToolBar = NULL;
   }
}

BOOL CUltColDoc::GetStatusBarMessageString(UINT nID,CString& rMessage) const
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return __super::GetStatusBarMessageString(nID,rMessage);
}

BOOL CUltColDoc::GetToolTipMessageString(UINT nID, CString& rMessage) const
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return __super::GetToolTipMessageString(nID,rMessage);
}
