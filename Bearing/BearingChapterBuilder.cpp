///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2022  Washington State Department of Transportation
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
#include "BearingChapterBuilder.h"
#include <Reporter\Reporter.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////
CBearingChapterBuilder::CBearingChapterBuilder(CBearingDoc* pDoc)
{
   m_pDoc = pDoc;
}

CBearingChapterBuilder::~CBearingChapterBuilder(void)
{
}

LPCTSTR CBearingChapterBuilder::GetName() const
{
   return _T("Bearing");
}

Uint16 CBearingChapterBuilder::GetMaxLevel() const
{
   return 1;
}

rptChapter* CBearingChapterBuilder::Build(const std::shared_ptr<const WBFL::Reporting::ReportSpecification>& pRptSpec,Uint16 level) const
{
   rptChapter* pChapter = new rptChapter;
   rptParagraph* pPara;

   pPara = new rptParagraph;
   (*pChapter) << pPara;

   *pPara << _T("Report Bearing Stuff Here") << rptNewLine;

   return pChapter;
}

std::unique_ptr<WBFL::Reporting::ChapterBuilder> CBearingChapterBuilder::Clone() const
{
   return std::make_unique<CBearingChapterBuilder>(m_pDoc);
}
