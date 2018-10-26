///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2018  Washington State Department of Transportation
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
#include "PGStableLiftingDetailsChapterBuilder.h"
#include <Reporter\Reporter.h>

#include <GraphicsLib\GraphicsLib.h>

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
   stbGirder girder = m_pDoc->GetGirder(girderType);
   stbLiftingResults results = m_pDoc->GetLiftingResults();
   const stbLiftingStabilityProblem& problem = m_pDoc->GetLiftingStabilityProblem();

   rptChapter* pChapter = new rptChapter;
   stbLiftingStabilityReporter reporter;
   reporter.BuildDetailsChapter(&girder,&problem,&results,pChapter);

   return pChapter;
}

CChapterBuilder* CPGStableLiftingDetailsChapterBuilder::Clone() const
{
   return new CPGStableLiftingDetailsChapterBuilder(m_pDoc);
}
