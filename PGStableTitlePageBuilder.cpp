///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2016  Washington State Department of Transportation
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

#include "StdAfx.h"
#include "PGStableTitlePageBuilder.h"
#include <Reporter\Reporter.h>
#include <MFCTools\VersionInfo.h>
#include "BEToolboxUtilities.h"


CPGStableTitlePageBuilder::CPGStableTitlePageBuilder(void)
{
}

CPGStableTitlePageBuilder::CPGStableTitlePageBuilder(const CPGStableTitlePageBuilder& other) :
CTitlePageBuilder(other)
{
}

CPGStableTitlePageBuilder::~CPGStableTitlePageBuilder(void)
{
}

rptChapter* CPGStableTitlePageBuilder::Build(boost::shared_ptr<CReportSpecification>& pRptSpec)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   rptChapter* pTitlePage = new rptChapter;

   rptParagraph* pPara = new rptParagraph(rptStyleManager::GetReportTitleStyle());
   (*pTitlePage) << pPara;
   (*pPara) << _T("PGStable") << Super(symbol(TRADEMARK)) << rptNewLine;

   pPara = new rptParagraph(rptStyleManager::GetReportSubtitleStyle());
   *pTitlePage << pPara;
   (*pPara) << _T("Prestressed Girder Stability Analysis") << rptNewLine;

   pPara = new rptParagraph(rptStyleManager::GetCopyrightStyle());
   *pTitlePage << pPara;
   *pPara << _T("Copyright ") << symbol(COPYRIGHT) << _T(" ") << sysDate().Year() << _T(", WSDOT, All Rights Reserved") << rptNewLine;

   pPara = new rptParagraph(rptStyleManager::GetReportSubtitleStyle());
   *pTitlePage << pPara;

   CVersionInfo verInfo;
   CString strAppName = AfxGetAppName();
   strAppName += _T(".dll");
   verInfo.Load(strAppName);

   CString strVersion = verInfo.GetProductVersionAsString();

#ifndef _DEBUG 
   // remove the build number
   int pos = strVersion.ReverseFind('.'); // find the last '.'
   strVersion = strVersion.Left(pos);
#endif

   CString str(_T("Version "));
   str += strVersion;
   str += CString(_T(" - Built on "));
   str += CString(__DATE__);

   *pPara << str << rptNewLine;

   *pPara << rptNewLine;

   //*pPara << rptRcImage(GetImagePath() + _T("PGStable.png"));

   return pTitlePage;
}

CTitlePageBuilder* CPGStableTitlePageBuilder::Clone() const
{
   return new CPGStableTitlePageBuilder(*this);
}
