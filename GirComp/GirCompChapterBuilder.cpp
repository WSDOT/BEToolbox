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
#include "GirCompChapterBuilder.h"
#include <Reporter\Reporter.h>

#include <GraphicsLib\GraphicsLib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////
CGirCompChapterBuilder::CGirCompChapterBuilder(CGirCompDoc* pDoc) :
m_Length(unitMeasure::Inch,0.001,6,2),
m_Area(unitMeasure::Feet2,0.001,6,2),
m_SectionModulus(unitMeasure::Feet3,0.001,10,2),
m_MomentOfInertia(unitMeasure::Feet4,0.001,10,2)
{
   m_Length.Format          = sysNumericFormatTool::Fixed;
   m_Area.Format            = sysNumericFormatTool::Fixed;
   m_SectionModulus.Format  = sysNumericFormatTool::Fixed;
   m_MomentOfInertia.Format = sysNumericFormatTool::Fixed;

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

rptChapter* CGirCompChapterBuilder::Build(CReportSpecification* pRptSpec,Uint16 level) const
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
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   m_Length.UnitOfMeasure          = (pApp->GetUnitsMode() == eafTypes::umUS ? unitMeasure::Inch  : unitMeasure::Millimeter);
   m_Area.UnitOfMeasure            = (pApp->GetUnitsMode() == eafTypes::umUS ? unitMeasure::Inch2 : unitMeasure::Millimeter2);
   m_SectionModulus.UnitOfMeasure  = (pApp->GetUnitsMode() == eafTypes::umUS ? unitMeasure::Inch3 : unitMeasure::Millimeter3);
   m_MomentOfInertia.UnitOfMeasure = (pApp->GetUnitsMode() == eafTypes::umUS ? unitMeasure::Inch4 : unitMeasure::Millimeter4);


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

CChapterBuilder* CGirCompChapterBuilder::Clone() const
{
   return new CGirCompChapterBuilder(m_pDoc);
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

   CComPtr<IPlateGirder> girder;
   m_pDoc->GetBuiltUpGirder(idx,&girder);

   Float64 Qt, Qb;
   girder->get_QTopFlange(&Qt);
   girder->get_QBottomFlange(&Qb);

   CComQIPtr<IShape> shape(girder);
   CComPtr<IShapeProperties> shapeProperties;
   shape->get_ShapeProperties(&shapeProperties);

   Float64 Ax, Ix, St, Sb, Yt, Yb;
   shapeProperties->get_Area(&Ax);
   shapeProperties->get_Ixx(&Ix);
   shapeProperties->get_Ytop(&Yt);
   shapeProperties->get_Ybottom(&Yb);
   St = IsZero(Yt) ? 0 : Ix/Yt;
   Sb = IsZero(Yb) ? 0 : Ix/Yb;

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

   CComPtr<ICompositeBeam> compBeam;
   m_pDoc->GetCompositeBeam(idx,n,&compBeam);

   CComQIPtr<ISection> section(compBeam);
   CComPtr<IElasticProperties> elasticProps;
   section->get_ElasticProperties(&elasticProps);

   CComPtr<IShapeProperties> shapeProperties;
   elasticProps->TransformProperties(1.0,&shapeProperties);

   Float64 Ax, Ix, St, Sb, Yt, Yb;
   shapeProperties->get_Area(&Ax);
   shapeProperties->get_Ixx(&Ix);
   shapeProperties->get_Ytop(&Yt);
   shapeProperties->get_Ybottom(&Yb);

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

   St = IsZero(Yt) ? 0 : Ix/Yt;
   Sb = IsZero(Yb) ? 0 : Ix/Yb;

   Float64 Qslab;
   compBeam->get_QSlab(&Qslab);

   Float64 Qb;
   compBeam->get_Q(dimensions.tBotFlange,&Qb);

   Float64 Qt;
   compBeam->get_Q(dimensions.tBotFlange + dimensions.D,&Qt);

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
