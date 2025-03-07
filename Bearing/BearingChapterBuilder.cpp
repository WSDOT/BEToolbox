///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2025  Washington State Department of Transportation
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
#include <EngTools\BearingReporter.h>
#include <EngTools\BearingDesignCriteria.h>

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
	CEAFApp* pApp = EAFGetApp();
	const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

	const auto& spec{ m_pDoc->GetSpecification() };
	const auto& brg{ m_pDoc->GetBearing()};
	const auto& brg_loads{ m_pDoc->GetBearingLoads()};
	const auto& brg_calc{ m_pDoc->GetBearingCalculator() };
	const auto& brg_criteria{m_pDoc->GetBearingDesignCriteria() };




	rptChapter* pChapter = new rptChapter;
	rptParagraph* pPara = new rptParagraph;
	(*pChapter) << pPara;
	

	std::unique_ptr<WBFL::EngTools::BearingReporter> BrgReporter;
	BrgReporter->BuildSpecCheckChapter(pDispUnits, pChapter, pPara, brg, brg_loads, brg_calc, spec, brg_criteria);

	return pChapter;
}

std::unique_ptr<WBFL::Reporting::ChapterBuilder> CBearingChapterBuilder::Clone() const
{
   return std::make_unique<CBearingChapterBuilder>(m_pDoc);
}
