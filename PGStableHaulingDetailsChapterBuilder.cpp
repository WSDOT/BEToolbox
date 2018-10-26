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
#include "PGStableHaulingDetailsChapterBuilder.h"
#include <Reporter\Reporter.h>

#include <GraphicsLib\GraphicsLib.h>

//////////////////////////////////
CPGStableHaulingDetailsChapterBuilder::CPGStableHaulingDetailsChapterBuilder(CPGStableDoc* pDoc)
{
   m_pDoc = pDoc;
}

CPGStableHaulingDetailsChapterBuilder::~CPGStableHaulingDetailsChapterBuilder(void)
{
}

LPCTSTR CPGStableHaulingDetailsChapterBuilder::GetName() const
{
   return _T("Hauling Details");
}

Uint16 CPGStableHaulingDetailsChapterBuilder::GetMaxLevel() const
{
   return 1;
}

rptChapter* CPGStableHaulingDetailsChapterBuilder::Build(CReportSpecification* pRptSpec,Uint16 level) const
{
   int girderType = m_pDoc->GetGirderType();
   stbGirder girder = m_pDoc->GetGirder(girderType);
   stbHaulingResults results = m_pDoc->GetHaulingResults();
   const stbHaulingStabilityProblem& problem = m_pDoc->GetHaulingStabilityProblem();

   rptChapter* pChapter = new rptChapter;
   stbHaulingStabilityReporter reporter;
   reporter.BuildDetailsChapter(&girder,&problem,&results,pChapter);

   return pChapter;
}

CChapterBuilder* CPGStableHaulingDetailsChapterBuilder::Clone() const
{
   return new CPGStableHaulingDetailsChapterBuilder(m_pDoc);
}
