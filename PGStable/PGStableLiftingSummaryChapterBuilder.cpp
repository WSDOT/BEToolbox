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
#include "PGStableLiftingSummaryChapterBuilder.h"
#include <Reporter\Reporter.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////
CPGStableLiftingSummaryChapterBuilder::CPGStableLiftingSummaryChapterBuilder(CPGStableDoc* pDoc)
{
   m_pDoc = pDoc;
}

CPGStableLiftingSummaryChapterBuilder::~CPGStableLiftingSummaryChapterBuilder(void)
{
}

LPCTSTR CPGStableLiftingSummaryChapterBuilder::GetName() const
{
   return _T("Lifting Summary");
}

Uint16 CPGStableLiftingSummaryChapterBuilder::GetMaxLevel() const
{
   return 1;
}

rptChapter* CPGStableLiftingSummaryChapterBuilder::Build(const std::shared_ptr<const WBFL::Reporting::ReportSpecification>& pRptSpec,Uint16 level) const
{
   GirderType girderType = m_pDoc->GetGirderType();
   WBFL::Stability::Girder girder = m_pDoc->GetGirder(girderType);
   WBFL::Stability::LiftingCheckArtifact artifact = m_pDoc->GetLiftingCheckArtifact();
   const WBFL::Stability::LiftingStabilityProblem& problem = m_pDoc->GetLiftingStabilityProblem();

   rptChapter* pChapter = new rptChapter;
   WBFL::Stability::LiftingStabilityReporter reporter;
   reporter.BuildSpecCheckChapter(&girder,&problem,&artifact,pChapter);

   return pChapter;
}

std::unique_ptr<WBFL::Reporting::ChapterBuilder> CPGStableLiftingSummaryChapterBuilder::Clone() const
{
   return std::make_unique<CPGStableLiftingSummaryChapterBuilder>(m_pDoc);
}
