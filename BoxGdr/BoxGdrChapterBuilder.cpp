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
#include "BoxGdrChapterBuilder.h"
#include <Reporter\Reporter.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////
CBoxGdrChapterBuilder::CBoxGdrChapterBuilder(CBoxGdrDoc* pDoc) :
m_Area(WBFL::Units::Measure::Feet2,0.001,6,2),
m_SectionModulus(WBFL::Units::Measure::Feet3,0.001,10,2),
m_MomentOfInertia(WBFL::Units::Measure::Feet4,0.001,10,2)
{
   m_Area.Format = WBFL::System::NumericFormatTool::Format::Fixed;
   m_SectionModulus.Format = WBFL::System::NumericFormatTool::Format::Fixed;
   m_MomentOfInertia.Format = WBFL::System::NumericFormatTool::Format::Fixed;

   m_pDoc = pDoc;
}

CBoxGdrChapterBuilder::~CBoxGdrChapterBuilder(void)
{
}

LPCTSTR CBoxGdrChapterBuilder::GetName() const
{
   return _T("BoxGdr");
}

Uint16 CBoxGdrChapterBuilder::GetMaxLevel() const
{
   return 1;
}

rptChapter* CBoxGdrChapterBuilder::Build(const std::shared_ptr<const WBFL::Reporting::ReportSpecification>& pRptSpec,Uint16 level) const
{
   rptChapter* pChapter = new rptChapter;
   rptParagraph* pPara;

   // NOTE: need to update if the section properties are computed.... problem count can be non-zero
   // and still need an update
   if ( m_pDoc->GetProblemCount() == 0 )
   {
       pPara = new rptParagraph;
      (*pChapter) << pPara;
      *pPara << _T("Press the Update button to compute the section properties") << rptNewLine;
      return pChapter;
   }

   pPara = new rptParagraph;
   (*pChapter) << pPara;

   rptRcTable* pTable = rptStyleManager::CreateDefaultTable(22);
   *pPara << pTable << rptNewLine;

   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   m_Area.UnitOfMeasure = (pApp->GetUnitsMode() == eafTypes::umUS ? WBFL::Units::Measure::Feet2 : WBFL::Units::Measure::Meter2);
   m_SectionModulus.UnitOfMeasure = (pApp->GetUnitsMode() == eafTypes::umUS ? WBFL::Units::Measure::Feet3 : WBFL::Units::Measure::Meter3);
   m_MomentOfInertia.UnitOfMeasure = (pApp->GetUnitsMode() == eafTypes::umUS ? WBFL::Units::Measure::Feet4 : WBFL::Units::Measure::Meter4);

   INIT_UV_PROTOTYPE( rptLengthUnitValue,  longLength,  pDispUnits->SpanLength, false);
   INIT_UV_PROTOTYPE( rptLengthUnitValue,  shortLength, pDispUnits->ComponentDim, false);
   INIT_UV_PROTOTYPE( rptLength2UnitValue,  area, m_Area, false);
   INIT_UV_PROTOTYPE( rptLength4UnitValue,  momentOfInertia, m_MomentOfInertia, false);
   INIT_UV_PROTOTYPE( rptLength3UnitValue,  sectionModulus, m_SectionModulus, false);
   INIT_UV_PROTOTYPE( rptForcePerLengthUnitValue, weight, pDispUnits->ForcePerLength, false );

   pTable->SetNumberOfHeaderRows(2);

   ColumnIndexType col = 0;
   pTable->SetRowSpan(0,col,2);
   (*pTable)(0,col++) << _T("PROB");

   pTable->SetRowSpan(0,col,2);
   (*pTable)(0,col++) << COLHDR(_T("D"), rptLengthUnitTag, pDispUnits->SpanLength);

   pTable->SetColumnSpan(0,col,2);
   (*pTable)(0,col) << _T("WEB");
   (*pTable)(1,col++) << COLHDR(_T("T"), rptLengthUnitTag, pDispUnits->ComponentDim);
   (*pTable)(1,col++) << _T("No.");


   pTable->SetRowSpan(0,col,2);
   (*pTable)(0,col++) << COLHDR(_T("W"),  rptLengthUnitTag, pDispUnits->SpanLength);

   pTable->SetRowSpan(0,col,2);
   (*pTable)(0,col++) << COLHDR(_T("ST"), rptLengthUnitTag, pDispUnits->ComponentDim);

   pTable->SetRowSpan(0,col,2);
   (*pTable)(0,col++) << COLHDR(_T("SB"), rptLengthUnitTag, pDispUnits->ComponentDim);

   pTable->SetRowSpan(0,col,2);
   (*pTable)(0,col++) << COLHDR(_T("FT"), rptLengthUnitTag, pDispUnits->ComponentDim);

   pTable->SetRowSpan(0,col,2);
   (*pTable)(0,col++) << COLHDR(_T("FB"), rptLengthUnitTag, pDispUnits->ComponentDim);

   pTable->SetColumnSpan(0,col,3);
   (*pTable)(0,col) << _T("LEFT CANTILEVER");
   (*pTable)(1,col++) << COLHDR(_T("EL"), rptLengthUnitTag, pDispUnits->SpanLength);
   (*pTable)(1,col++) << COLHDR(_T("CL"), rptLengthUnitTag, pDispUnits->ComponentDim);
   (*pTable)(1,col++) << COLHDR(_T("BL"), rptLengthUnitTag, pDispUnits->SpanLength);

   pTable->SetColumnSpan(0,col,3);
   (*pTable)(0,col) << _T("RIGHT CANTILEVER");
   (*pTable)(1,col++) << COLHDR(_T("ER"), rptLengthUnitTag, pDispUnits->SpanLength);
   (*pTable)(1,col++) << COLHDR(_T("CR"), rptLengthUnitTag, pDispUnits->ComponentDim);
   (*pTable)(1,col++) << COLHDR(_T("BR"), rptLengthUnitTag, pDispUnits->SpanLength);

   pTable->SetColumnSpan(0,col,7);
   (*pTable)(0,col) << _T("PROPERTIES");
   (*pTable)(1,col++) << COLHDR(_T("I"), rptLength4UnitTag, m_MomentOfInertia);
   (*pTable)(1,col++) << COLHDR(_T("YT"), rptLengthUnitTag, pDispUnits->SpanLength);
   (*pTable)(1,col++) << COLHDR(_T("YB"), rptLengthUnitTag, pDispUnits->SpanLength);
   (*pTable)(1,col++) << COLHDR(_T("AREA"), rptLength2UnitTag, m_Area);
   (*pTable)(1,col++) << COLHDR(_T("WEIGHT"), rptForcePerLengthUnitTag, pDispUnits->ForcePerLength);
   (*pTable)(1,col++) << COLHDR(_T("ZT"), rptLength3UnitTag, m_SectionModulus);
   (*pTable)(1,col++) << COLHDR(_T("ZB"), rptLength3UnitTag, m_SectionModulus);

   RowIndexType row = pTable->GetNumberOfHeaderRows();
   IndexType nProblems = m_pDoc->GetProblemCount();
   for ( IndexType idx = 0; idx < nProblems; idx++, row++ )
   {
      col = 0;
      const BOXGDRDIMENSIONS& problem = m_pDoc->GetProblem(idx);
      (*pTable)(row,col++) << (idx+1);
      (*pTable)(row,col++) << longLength.SetValue(problem.D);
      (*pTable)(row,col++) << shortLength.SetValue(problem.T);
      (*pTable)(row,col++) << problem.N;
      (*pTable)(row,col++) << longLength.SetValue(problem.W);
      (*pTable)(row,col++) << shortLength.SetValue(problem.ST);
      (*pTable)(row,col++) << shortLength.SetValue(problem.SB);
      (*pTable)(row,col++) << shortLength.SetValue(problem.FT);
      (*pTable)(row,col++) << shortLength.SetValue(problem.FB);
      (*pTable)(row,col++) << longLength.SetValue(problem.EL);
      (*pTable)(row,col++) << shortLength.SetValue(problem.CL);
      (*pTable)(row,col++) << longLength.SetValue(problem.BL);
      (*pTable)(row,col++) << longLength.SetValue(problem.ER);
      (*pTable)(row,col++) << shortLength.SetValue(problem.CR);
      (*pTable)(row,col++) << longLength.SetValue(problem.BR);

      auto shapeProperties = m_pDoc->ComputeShapeProperties(idx);

      Float64 Area = shapeProperties.GetArea();
      Float64 Ix = shapeProperties.GetIxx();
      Float64 Yt = shapeProperties.GetYtop();
      Float64 Yb = shapeProperties.GetYbottom();

      Float64 St = IsZero(Yt) ? 0 : Ix/Yt;
      Float64 Sb = IsZero(Yb) ? 0 : Ix/Yb;

      Float64 Weight;
      Weight = Area*WBFL::Units::ConvertToSysUnits(160.0,WBFL::Units::Measure::PCF);

      (*pTable)(row,col++) << momentOfInertia.SetValue(Ix);
      (*pTable)(row,col++) << longLength.SetValue(Yt);
      (*pTable)(row,col++) << longLength.SetValue(Yb);
      (*pTable)(row,col++) << area.SetValue(Area);
      (*pTable)(row,col++) << weight.SetValue(Weight);
      (*pTable)(row,col++) << sectionModulus.SetValue(St);
      (*pTable)(row,col++) << sectionModulus.SetValue(Sb);
   }

   return pChapter;
}

std::unique_ptr<WBFL::Reporting::ChapterBuilder> CBoxGdrChapterBuilder::Clone() const
{
   return std::make_unique<CBoxGdrChapterBuilder>(m_pDoc);
}
