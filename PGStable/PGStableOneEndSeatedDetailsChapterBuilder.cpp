///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2023  Washington State Department of Transportation
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
#include "PGStableOneEndSeatedDetailsChapterBuilder.h"
#include <Reporter\Reporter.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////
CPGStableOneEndSeatedDetailsChapterBuilder::CPGStableOneEndSeatedDetailsChapterBuilder(CPGStableDoc* pDoc)
{
   m_pDoc = pDoc;
}

CPGStableOneEndSeatedDetailsChapterBuilder::~CPGStableOneEndSeatedDetailsChapterBuilder(void)
{
}

LPCTSTR CPGStableOneEndSeatedDetailsChapterBuilder::GetName() const
{
   return _T("One End Seated Details");
}

Uint16 CPGStableOneEndSeatedDetailsChapterBuilder::GetMaxLevel() const
{
   return 1;
}

rptChapter* CPGStableOneEndSeatedDetailsChapterBuilder::Build(const std::shared_ptr<const WBFL::Reporting::ReportSpecification>& pRptSpec,Uint16 level) const
{
   GirderType girderType = m_pDoc->GetGirderType();
   WBFL::Stability::Girder girder = m_pDoc->GetGirder(girderType);
   WBFL::Stability::OneEndSeatedResults results = m_pDoc->GetOneEndSeatedResults();
   const WBFL::Stability::OneEndSeatedStabilityProblem& problem = m_pDoc->GetOneEndSeatedStabilityProblem();

   rptChapter* pChapter = new rptChapter;
   WBFL::Stability::OneEndSeatedStabilityReporter reporter;
   auto* pApp = EAFGetApp();
   reporter.BuildDetailsChapter(&girder,&problem,&results,pChapter,pApp->GetDisplayUnits());

   return pChapter;
}

std::unique_ptr<WBFL::Reporting::ChapterBuilder> CPGStableOneEndSeatedDetailsChapterBuilder::Clone() const
{
   return std::make_unique<CPGStableOneEndSeatedDetailsChapterBuilder>(m_pDoc);
}
