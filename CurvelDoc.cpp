///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2015  Washington State Department of Transportation
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
   // The reporting sub-system doesn't use the WBFLUnitServer implementation. It uses the old regular C++
   // units sytem. That system is in kms units, so we will create a unit server here also in the kms system
   // so that the curvel data, after loading is in set of consistent base units we want.
   // If the report system could handle the WBFLUnitServer, the <ConsistentUnits> declaration in the
   // CurvelXML instance document would work throughout this program because Curvel works exclusively in
   // consistent units.
   m_DocUnitServer.CoCreateInstance(CLSID_UnitServer);
   m_DocUnitServer->SetBaseUnits(CComBSTR(unitSysUnitsMgr::GetMassUnit().UnitTag().c_str()),
                            CComBSTR(unitSysUnitsMgr::GetLengthUnit().UnitTag().c_str()),
                            CComBSTR(unitSysUnitsMgr::GetTimeUnit().UnitTag().c_str()),
                            CComBSTR(unitSysUnitsMgr::GetTemperatureUnit().UnitTag().c_str()),
                            CComBSTR(unitSysUnitsMgr::GetAngleUnit().UnitTag().c_str()));  
   m_DocUnitServer->QueryInterface(&m_DocConvert);

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

CString CCurvelDoc::GetToolbarSectionName()
{
   return _T("Curvel");
}

BOOL CCurvelDoc::OnNewDocument()
{
   if ( !CBEToolboxDoc::OnNewDocument() )
      return FALSE;

   m_CurvelXML = CreateCurvelModel();
   if ( m_CurvelXML.get() == NULL )
      return FALSE;

   return TRUE;
}

BOOL CCurvelDoc::OpenTheDocument(LPCTSTR lpszPathName)
{
   USES_CONVERSION;

   m_CurvelXML = CreateCurvelModel(lpszPathName,m_DocUnitServer);
   if ( m_CurvelXML.get() == NULL )
      return FALSE;

   return TRUE;
}

BOOL CCurvelDoc::SaveTheDocument(LPCTSTR lpszPathName)
{
   return SaveCurvelModel(lpszPathName,m_CurvelXML.get());
}

void CCurvelDoc::OnOldFormat(LPCTSTR lpszPathName)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   AfxMessageBox(_T("Error: Invalid file format.\n\nThis is not a Curvel file or it is in a legacy format that is not supported"),MB_OK | MB_ICONEXCLAMATION);
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

Curvel* CCurvelDoc::GetCurvelXML()
{
   return m_CurvelXML.get();
}
