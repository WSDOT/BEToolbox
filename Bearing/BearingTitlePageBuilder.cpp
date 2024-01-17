///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright � 1999-2024  Washington State Department of Transportation
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
#include "BearingTitlePageBuilder.h"
#include <Reporter\Reporter.h>
#include <MFCTools\VersionInfo.h>
#include "..\BEToolboxUtilities.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CBearingTitlePageBuilder::CBearingTitlePageBuilder(void)
{
}

CBearingTitlePageBuilder::CBearingTitlePageBuilder(const CBearingTitlePageBuilder& other) :
WBFL::Reporting::TitlePageBuilder(other)
{
}

CBearingTitlePageBuilder::~CBearingTitlePageBuilder(void)
{
}

rptChapter* CBearingTitlePageBuilder::Build(const std::shared_ptr<const WBFL::Reporting::ReportSpecification>& pRptSpec) const
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   rptChapter* pTitlePage = new rptChapter;

   rptParagraph* pPara = new rptParagraph(_T("BEToolboxReportTitle"));
   (*pTitlePage) << pPara;
   (*pPara) << _T("BEToolbox") << Super(symbol(TRADEMARK)) << rptNewLine;

   pPara = new rptParagraph(_T("BEToolboxReportSubtitle"));
   *pTitlePage << pPara;
   (*pPara) << _T("BrgCalc") << rptNewLine;

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

   //*pPara << rptRcImage(std::_tstring(rptStyleManager::GetImagePath()) + _T("Bearing.PNG"));

   //(*pPara) << rptNewPage;
 
   return pTitlePage;
}

std::unique_ptr<WBFL::Reporting::TitlePageBuilder> CBearingTitlePageBuilder::Clone() const
{
   return std::make_unique<CBearingTitlePageBuilder>(*this);
}
