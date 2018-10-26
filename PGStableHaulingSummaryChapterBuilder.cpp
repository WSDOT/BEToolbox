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
#include "PGStableHaulingSummaryChapterBuilder.h"
#include <Reporter\Reporter.h>

#include <GraphicsLib\GraphicsLib.h>

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
   rptChapter* pChapter = new rptChapter;
   rptParagraph* pPara;

   pPara = new rptParagraph;
   (*pChapter) << pPara;

   *pPara << _T("Hauling Summary") << rptNewLine;

   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   INIT_SCALAR_PROTOTYPE(rptRcScalar, scalar, pDispUnits->Scalar);
   INIT_UV_PROTOTYPE( rptLengthUnitValue,  longLength,  pDispUnits->SpanLength,   true);

   int girderType = m_pDoc->GetGirderType();
   stbGirder girder = m_pDoc->GetGirder(girderType);
   stbHaulingResults results = m_pDoc->GetHaulingResults();

   std::_tstring strImpact[] = {_T("No Impact"), _T("Impact Up"),_T("Impact Down")};
   std::_tstring strWind[] = {_T("Wind Left"), _T("Wind Right")};
   std::_tstring strCF[] = {_T("Centrifugal Force Left"), _T("Centrifugal Force Right")};
   std::_tstring strFlange[] = {_T("Top Left"),_T("Top Right"),_T("Bottom Left"),_T("Bottom Right")};

   *pPara << _T("The minimum factor of safety against cracking occurs at ") << longLength.SetValue(results.vSectionResults[results.FScrAnalysisPointIndex].X) << _T(" ");
   *pPara << strFlange[results.vSectionResults[results.FScrAnalysisPointIndex].CrackedFlange[results.FScrImpactDirection][results.FScrWindDirection][results.FScrCFDirection]].c_str() << _T(" flange tip with ");
   *pPara << strImpact[results.FScrImpactDirection].c_str() << _T(", ") << strWind[results.FScrWindDirection] << _T(", and ") << strCF[results.FScrCFDirection].c_str() << rptNewLine;
   *pPara << Sub2(_T("FS"),_T("cr")) << _T(" Min = ") << scalar.SetValue(results.MinFScr) << rptNewLine;

   *pPara << rptNewLine;

   *pPara << _T("The minimum factor of safety against failure occurs with ") << strImpact[results.FSfImpactDirection].c_str() << _T(", ");
   *pPara << strWind[results.FSfWindDirection].c_str() << _T(", and ") << strCF[results.FSfCFDirection].c_str() << rptNewLine;
   *pPara << Sub2(_T("FS"),_T("f")) << _T(" = ") << scalar.SetValue(results.MinFsFailure) << rptNewLine;

   *pPara << rptNewLine;

   *pPara << _T("The minimum factor of safety against roll over occurs with ") << strImpact[results.FSroImpactDirection].c_str() << _T(", ");
   *pPara << strWind[results.FSroWindDirection].c_str() << _T(", and ") << strCF[results.FSroCFDirection].c_str() << rptNewLine;
   *pPara << Sub2(_T("FS"),_T("ro")) << _T(" = ") << scalar.SetValue(results.MinFsRollover) << rptNewLine;

   return pChapter;
}

CChapterBuilder* CPGStableHaulingSummaryChapterBuilder::Clone() const
{
   return new CPGStableHaulingSummaryChapterBuilder(m_pDoc);
}
