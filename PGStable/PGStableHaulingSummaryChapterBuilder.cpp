///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2021  Washington State Department of Transportation
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
#include "PGStableHaulingSummaryChapterBuilder.h"
#include <Reporter\Reporter.h>

#include <GraphicsLib\GraphicsLib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////
CPGStableHaulingSummaryChapterBuilder::CPGStableHaulingSummaryChapterBuilder(CPGStableDoc* pDoc)
{
   m_pDoc = pDoc;
}

CPGStableHaulingSummaryChapterBuilder::~CPGStableHaulingSummaryChapterBuilder(void)
{
}

LPCTSTR CPGStableHaulingSummaryChapterBuilder::GetName() const
{
   return _T("Hauling Summary");
}

Uint16 CPGStableHaulingSummaryChapterBuilder::GetMaxLevel() const
{
   return 1;
}

rptChapter* CPGStableHaulingSummaryChapterBuilder::Build(CReportSpecification* pRptSpec,Uint16 level) const
{
   int girderType = m_pDoc->GetGirderType();
   stbGirder girder = m_pDoc->GetGirder(girderType);
   stbHaulingCheckArtifact artifact = m_pDoc->GetHaulingCheckArtifact();
   const stbHaulingStabilityProblem& problem = m_pDoc->GetHaulingStabilityProblem();

   rptChapter* pChapter = new rptChapter;
   stbHaulingStabilityReporter reporter;
   reporter.BuildSpecCheckChapter(&girder,&problem,&artifact,pChapter);

   return pChapter;
}

CChapterBuilder* CPGStableHaulingSummaryChapterBuilder::Clone() const
{
   return new CPGStableHaulingSummaryChapterBuilder(m_pDoc);
}
