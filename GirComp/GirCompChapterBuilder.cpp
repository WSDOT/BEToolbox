///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2024  Washington State Department of Transportation
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
#include "GirCompChapterBuilder.h"
#include <Reporter\Reporter.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////
CGirCompChapterBuilder::CGirCompChapterBuilder(CGirCompDoc* pDoc) :
m_Length(WBFL::Units::Measure::Inch,0.001,6,2),
m_Area(WBFL::Units::Measure::Feet2,0.001,6,2),
m_SectionModulus(WBFL::Units::Measure::Feet3,0.001,10,2),
m_MomentOfInertia(WBFL::Units::Measure::Feet4,0.001,10,2)
{
   m_Length.Format          = WBFL::System::NumericFormatTool::Format::Fixed;
   m_Area.Format            = WBFL::System::NumericFormatTool::Format::Fixed;
   m_SectionModulus.Format  = WBFL::System::NumericFormatTool::Format::Fixed;
   m_MomentOfInertia.Format = WBFL::System::NumericFormatTool::Format::Fixed;

   m_pDoc = pDoc;
}

CGirCompChapterBuilder::~CGirCompChapterBuilder(void)
{
}

LPCTSTR CGirCompChapterBuilder::GetName() const
{
   return _T("GirComp");
}

Uint16 CGirCompChapterBuilder::GetMaxLevel() const
{
   return 1;
}

rptChapter* CGirCompChapterBuilder::Build(const std::shared_ptr<const WBFL::Reporting::ReportSpecification>& pRptSpec,Uint16 level) const
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

   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   m_Length.UnitOfMeasure          = (pApp->GetUnitsMode() == eafTypes::umUS ? WBFL::Units::Measure::Inch  : WBFL::Units::Measure::Millimeter);
   m_Area.UnitOfMeasure            = (pApp->GetUnitsMode() == eafTypes::umUS ? WBFL::Units::Measure::Inch2 : WBFL::Units::Measure::Millimeter2);
   m_SectionModulus.UnitOfMeasure  = (pApp->GetUnitsMode() == eafTypes::umUS ? WBFL::Units::Measure::Inch3 : WBFL::Units::Measure::Millimeter3);
   m_MomentOfInertia.UnitOfMeasure = (pApp->GetUnitsMode() == eafTypes::umUS ? WBFL::Units::Measure::Inch4 : WBFL::Units::Measure::Millimeter4);


   IndexType nProblems = m_pDoc->GetProblemCount();
   for ( IndexType idx = 0; idx < nProblems; idx++ )
   {
      pPara = new rptParagraph(rptStyleManager::GetHeadingStyle());
      (*pChapter) << pPara;

      *pPara << _T("Problem No. ") << (idx+1) << rptNewLine;
      const GIRCOMPDIMENSIONS& dimensions = m_pDoc->GetProblem(idx);

      switch(dimensions.Type)
      {
      case GIRCOMPDIMENSIONS::Rolled:
         ReportRolledSectionProperties(pChapter,idx,dimensions);
         break;

      case GIRCOMPDIMENSIONS::General:
         ReportGeneralSectionProperties(pChapter,idx,dimensions);
         break;

      case GIRCOMPDIMENSIONS::BuiltUp:
         ReportBuiltUpSectionProperties(pChapter,idx,dimensions);
         break;

      default:
         ATLASSERT(false); // is this a new section type?
      }

      pPara = new rptParagraph;
      (*pChapter) << pPara;
      *pPara << rptNewPage;
   }

   return pChapter;
}

std::unique_ptr<WBFL::Reporting::ChapterBuilder> CGirCompChapterBuilder::Clone() const
{
   return std::make_unique<CGirCompChapterBuilder>(m_pDoc);
}

void CGirCompChapterBuilder::ReportRolledSectionProperties(rptChapter* pChapter,IndexType idx,const GIRCOMPDIMENSIONS& dimensions) const
{
   rptParagraph* pPara = new rptParagraph;
   *pChapter << pPara;

   INIT_UV_PROTOTYPE( rptLengthUnitValue,  length,          m_Length,          true);
   INIT_UV_PROTOTYPE( rptLength2UnitValue, area,            m_Area,            true);
   INIT_UV_PROTOTYPE( rptLength4UnitValue, momentOfInertia, m_MomentOfInertia, true);
   INIT_UV_PROTOTYPE( rptLength3UnitValue, sectionModulus,  m_SectionModulus,  true);


   *pPara << _T("Rolled Beam Section Properties (") << dimensions.RolledSectionName << _T(")") << rptNewLine;
   *pPara << _T("Area = ") << area.SetValue(dimensions.Area) << _T(" ");
   *pPara << _T("Depth = ") << length.SetValue(dimensions.D) << _T(" ");
   *pPara << _T("I = ") << momentOfInertia.SetValue(dimensions.MomentOfInertia) << rptNewLine;

   *pPara << _T(" Slab ") << length.SetValue(dimensions.wSlab) << _T(" by ");
   *pPara << length.SetValue(dimensions.tSlab);
   *pPara << _T(" G = ") << length.SetValue(dimensions.G) << rptNewLine;

   ReportCompositeSectionProperties(pChapter,idx,dimensions,0);
   ReportCompositeSectionProperties(pChapter,idx,dimensions,1);
   ReportCompositeSectionProperties(pChapter,idx,dimensions,2);
}

void CGirCompChapterBuilder::ReportGeneralSectionProperties(rptChapter* pChapter,IndexType idx,const GIRCOMPDIMENSIONS& dimensions) const
{
   rptParagraph* pPara = new rptParagraph;
   *pChapter << pPara;

   INIT_UV_PROTOTYPE( rptLengthUnitValue,  length,          m_Length,          true);
   INIT_UV_PROTOTYPE( rptLength2UnitValue, area,            m_Area,            true);
   INIT_UV_PROTOTYPE( rptLength4UnitValue, momentOfInertia, m_MomentOfInertia, true);
   INIT_UV_PROTOTYPE( rptLength3UnitValue, sectionModulus,  m_SectionModulus,  true);


   *pPara << _T("General Beam Section Properties") << rptNewLine;
   *pPara << _T("Area = ") << area.SetValue(dimensions.Area) << _T(" ");
   *pPara << _T("Depth = ") << length.SetValue(dimensions.D) << _T(" ");
   *pPara << _T("I = ") << momentOfInertia.SetValue(dimensions.MomentOfInertia) << rptNewLine;

   *pPara << _T(" Slab ") << length.SetValue(dimensions.wSlab) << _T(" by ");
   *pPara << length.SetValue(dimensions.tSlab);
   *pPara << _T(" G = ") << length.SetValue(dimensions.G) << rptNewLine;

   ReportCompositeSectionProperties(pChapter,idx,dimensions,0);
   ReportCompositeSectionProperties(pChapter,idx,dimensions,1);
   ReportCompositeSectionProperties(pChapter,idx,dimensions,2);
}

void CGirCompChapterBuilder::ReportBuiltUpSectionProperties(rptChapter* pChapter,IndexType idx,const GIRCOMPDIMENSIONS& dimensions) const
{
   rptParagraph* pPara = new rptParagraph;
   *pChapter << pPara;

   INIT_UV_PROTOTYPE( rptLengthUnitValue,  length,          m_Length,          true);
   INIT_UV_PROTOTYPE( rptLength2UnitValue, area,            m_Area,            true);
   INIT_UV_PROTOTYPE( rptLength4UnitValue, momentOfInertia, m_MomentOfInertia, true);
   INIT_UV_PROTOTYPE( rptLength3UnitValue, sectionModulus,  m_SectionModulus,  true);


   *pPara << _T("Built-Up Beam Dimensions") << rptNewLine;
   *pPara << _T("Web ") << length.SetValue(dimensions.D) << _T(" by ");
   *pPara << length.SetValue(dimensions.tWeb);
   *pPara << _T(" Top Flange ") << length.SetValue(dimensions.wTopFlange) << _T(" by ");
   *pPara << length.SetValue(dimensions.tTopFlange) << rptNewLine;

   *pPara << _T("Bot Flange ") << length.SetValue(dimensions.wBotFlange) << _T(" by ");
   *pPara << length.SetValue(dimensions.tBotFlange);
   *pPara << _T(" Slab ") << length.SetValue(dimensions.wSlab) << _T(" by ");
   *pPara << length.SetValue(dimensions.tSlab);
   *pPara << _T(" G = ") << length.SetValue(dimensions.G) << rptNewLine;

   auto girder = m_pDoc->GetBuiltUpGirder(idx);

   Float64 Qt = girder.GetQTopFlange();
   Float64 Qb = girder.GetQBottomFlange();

   WBFL::Geometry::ShapeProperties shapeProperties = girder.GetProperties();

   Float64 Ax = shapeProperties.GetArea();
   Float64 Ix = shapeProperties.GetIxx();
   Float64 Yt = shapeProperties.GetYtop();
   Float64 Yb = shapeProperties.GetYbottom();
   Float64 St = IsZero(Yt) ? 0 : Ix/Yt;
   Float64 Sb = IsZero(Yb) ? 0 : Ix/Yb;

   pPara = new rptParagraph;
   *pChapter << pPara;
   *pPara << _T("Girder Properties") << rptNewLine;
   *pPara << Sub2(_T("I"),_T("g")) << _T(" = ") << momentOfInertia.SetValue(Ix) << _T(" ");
   *pPara << Sub2(_T("S"),_T("b")) << _T(" = ") << sectionModulus.SetValue(Sb) << _T(" ");
   *pPara << Sub2(_T("S"),_T("t")) << _T(" = ") << sectionModulus.SetValue(St) << rptNewLine;

   *pPara << Sub2(_T("Y"),_T("b")) << _T(" = ") << length.SetValue(Yb) << _T(" ");
   *pPara << Sub2(_T("Y"),_T("t")) << _T(" = ") << length.SetValue(Yt) << rptNewLine;

   *pPara << _T("Q Bot Flange") << _T(" = ") << sectionModulus.SetValue(Qb) << _T(" ");
   *pPara << _T("Q Top Flange") << _T(" = ") << sectionModulus.SetValue(Qt) << _T(" ");
   *pPara << _T("Steel Area = ") << area.SetValue(Ax) << rptNewLine;

   ReportCompositeSectionProperties(pChapter,idx,dimensions,0);
   ReportCompositeSectionProperties(pChapter,idx,dimensions,1);
   ReportCompositeSectionProperties(pChapter,idx,dimensions,2);
}

void CGirCompChapterBuilder::ReportCompositeSectionProperties(rptChapter* pChapter,IndexType idx,const GIRCOMPDIMENSIONS& dimensions,int n) const
{
   rptParagraph* pPara = new rptParagraph;
   *pChapter << pPara;

   INIT_UV_PROTOTYPE( rptLengthUnitValue,  length,          m_Length,          true);
   INIT_UV_PROTOTYPE( rptLength2UnitValue, area,            m_Area,            true);
   INIT_UV_PROTOTYPE( rptLength4UnitValue, momentOfInertia, m_MomentOfInertia, true);
   INIT_UV_PROTOTYPE( rptLength3UnitValue, sectionModulus,  m_SectionModulus,  true);
   
   Float64 N;
   if ( n == 0 )
      N = dimensions.N1;
   else if ( n == 1 )
      N = dimensions.N2;
   else if ( n == 2 )
      N = dimensions.N3;

   *pPara << _T("Composite Girder Properties N = ") << N << rptNewLine;

   auto compBeam = m_pDoc->GetCompositeBeam(idx,n);
   auto elasticProps = compBeam.GetElasticProperties();
   auto shapeProperties = elasticProps.TransformProperties(1.0);

   Float64 Ax = shapeProperties.GetArea();
   Float64 Ix = shapeProperties.GetIxx();
   Float64 Yt = shapeProperties.GetYtop();
   Float64 Yb = shapeProperties.GetYbottom();

   Float64 Hg;
   if ( dimensions.Type == GIRCOMPDIMENSIONS::BuiltUp )
      Hg = dimensions.tTopFlange + dimensions.D + dimensions.tBotFlange;
   else
      Hg = dimensions.D;

   if ( Hg < Yb )
   {
      *pPara << _T("Neutral Axis Lies in Slab") << rptNewLine;
      return;
   }

   // we want Yt of girder and St of girder
   Yt -= (dimensions.tSlab + dimensions.G);

   Float64 St = IsZero(Yt) ? 0 : Ix/Yt;
   Float64 Sb = IsZero(Yb) ? 0 : Ix/Yb;

   Float64 Qslab = compBeam.GetQslab();
   Float64 Qb = compBeam.GetQ(dimensions.tBotFlange);
   Float64 Qt = compBeam.GetQ(dimensions.tBotFlange + dimensions.D);

   *pPara << Sub2(_T("I"),_T("c")) << _T(" = ") << momentOfInertia.SetValue(Ix) << _T(" ");
   *pPara << Sub2(_T("S"),_T("b")) << _T(" = ") << sectionModulus.SetValue(Sb) << _T(" ");
   *pPara << Sub2(_T("S"),_T("t")) << _T(" = ") << sectionModulus.SetValue(St) << rptNewLine;

   *pPara << Sub2(_T("Y"),_T("b")) << _T(" = ") << length.SetValue(Yb) << _T(" ");
   *pPara << Sub2(_T("Y"),_T("t")) << _T(" = ") << length.SetValue(Yt) << rptNewLine;

   if ( dimensions.Type == GIRCOMPDIMENSIONS::BuiltUp )
   {
      *pPara << _T("Q Bot Flange") << _T(" = ") << sectionModulus.SetValue(Qb) << _T(" ");
   }

   *pPara << _T("Q Slab") << _T(" = ") << sectionModulus.SetValue(Qslab) << _T(" ");

   if ( dimensions.Type == GIRCOMPDIMENSIONS::BuiltUp )
   {
      *pPara << _T("Q Slab & Top Flange") << _T(" = ") << sectionModulus.SetValue(Qt) << _T(" ");
   }
}
