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
#include "PGStableLiftingDetailsChapterBuilder.h"
#include <Reporter\Reporter.h>

#include <GraphicsLib\GraphicsLib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////
CPGStableLiftingDetailsChapterBuilder::CPGStableLiftingDetailsChapterBuilder(CPGStableDoc* pDoc)
{
   m_pDoc = pDoc;
}

CPGStableLiftingDetailsChapterBuilder::~CPGStableLiftingDetailsChapterBuilder(void)
{
}

LPCTSTR CPGStableLiftingDetailsChapterBuilder::GetName() const
{
   return _T("Lifting Details");
}

Uint16 CPGStableLiftingDetailsChapterBuilder::GetMaxLevel() const
{
   return 1;
}

rptChapter* CPGStableLiftingDetailsChapterBuilder::Build(CReportSpecification* pRptSpec,Uint16 level) const
{
   int girderType = m_pDoc->GetGirderType();
   WBFL::Stability::Girder girder = m_pDoc->GetGirder(girderType);
   WBFL::Stability::LiftingResults results = m_pDoc->GetLiftingResults();
   const WBFL::Stability::LiftingStabilityProblem& problem = m_pDoc->GetLiftingStabilityProblem();

   rptChapter* pChapter = new rptChapter;
   WBFL::Stability::LiftingStabilityReporter reporter;
   reporter.BuildDetailsChapter(&girder,&problem,&results,pChapter);

   return pChapter;
}

CChapterBuilder* CPGStableLiftingDetailsChapterBuilder::Clone() const
{
   return new CPGStableLiftingDetailsChapterBuilder(m_pDoc);
}
