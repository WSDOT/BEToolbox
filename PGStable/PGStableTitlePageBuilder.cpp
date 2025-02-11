///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright � 1999-2025  Washington State Department of Transportation
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
#include "PGStableTitlePageBuilder.h"
#include <Reporter\Reporter.h>
#include <MFCTools\VersionInfo.h>
#include "..\BEToolboxUtilities.h"
#include "PGStableDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CPGStableTitlePageBuilder::CPGStableTitlePageBuilder()
{
}

CPGStableTitlePageBuilder::CPGStableTitlePageBuilder(const CPGStableTitlePageBuilder& other) :
WBFL::Reporting::TitlePageBuilder(other)
{
}

CPGStableTitlePageBuilder::~CPGStableTitlePageBuilder(void)
{
}

rptChapter* CPGStableTitlePageBuilder::Build(const std::shared_ptr<const WBFL::Reporting::ReportSpecification>& pRptSpec) const
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   rptChapter* pTitlePage = new rptChapter;

   rptParagraph* pPara = new rptParagraph(_T("BEToolboxReportTitle"));
   (*pTitlePage) << pPara;
   (*pPara) << _T("PGStable") << Super(symbol(TRADEMARK)) << rptNewLine;

   pPara = new rptParagraph(_T("BEToolboxReportSubtitle"));
   *pTitlePage << pPara;
   (*pPara) << _T("Prestressed Girder Stability Analysis") << rptNewLine;

   pPara = new rptParagraph(_T("BEToolboxCopyright"));
   *pTitlePage << pPara;
   *pPara << _T("Copyright ") << symbol(COPYRIGHT) << _T(" ") << WBFL::System::Date().Year() << _T(", WSDOT, All Rights Reserved") << rptNewLine;

   pPara = new rptParagraph(_T("BEToolboxReportSubtitle"));
   *pTitlePage << pPara;

   CVersionInfo verInfo;
   CString strAppName = AfxGetAppName();
   strAppName += _T(".dll");
   verInfo.Load(strAppName);

   bool bIncludeBuildNumber = false;
#if defined _DEBUG || defined _BETA_VERSION
   bIncludeBuildNumber = true;
#endif
   CString strVersion = verInfo.GetProductVersionAsString(bIncludeBuildNumber);

   CString str(_T("Version "));
   str += strVersion;
#if defined _BETA_VERSION
   str += CString(_T(" BETA"));
#endif

   str += CString(_T(" - Built on "));
   str += CString(__DATE__);

   *pPara << str << rptNewLine;

   *pPara << rptNewLine;

   // Not doing a report cover image, but if we wanted to, here is the code
   //CString strImage;
   //strImage.Format(_T("%s%s"),rptStyleManager::GetImagePath(),m_Type == LIFTING_REPORT ? _T("PGStableLifting.png") : _T("PGStableHauling.png"));
   //*pPara << rptRcImage(strImage.GetBuffer()) << rptNewLine;

   pPara = new rptParagraph(_T("BEToolboxPrintOnly"));
   *pTitlePage << pPara;

   rptRcTable* pPropertiesTable = rptStyleManager::CreateTableNoHeading(2,_T("Project Properties"));
   *pPara << pPropertiesTable << rptNewLine;

   CPGStableDoc* pDoc = (CPGStableDoc*)EAFGetDocument();
   CString strEngineer, strCompany, strJob, strComments;
   pDoc->GetProjectProperties(&strEngineer,&strCompany,&strJob,&strComments);
   RowIndexType row = 0;
   (*pPropertiesTable)(row,0) << _T("Engineer");
   (*pPropertiesTable)(row,1) << strEngineer;
   row++;

   (*pPropertiesTable)(row,0) << _T("Company");
   (*pPropertiesTable)(row,1) << strCompany;
   row++;

   (*pPropertiesTable)(row,0) << _T("Job");
   (*pPropertiesTable)(row,1) << strJob;
   row++;

   (*pPropertiesTable)(row,0) << _T("Comments");
   (*pPropertiesTable)(row,1) << strComments;
   row++;

   return pTitlePage;
}

std::unique_ptr<WBFL::Reporting::TitlePageBuilder> CPGStableTitlePageBuilder::Clone() const
{
   return std::make_unique<CPGStableTitlePageBuilder>(*this);
}
