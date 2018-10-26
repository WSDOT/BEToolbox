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
   rptChapter* pChapter = new rptChapter;
   rptParagraph* pPara;

   pPara = new rptParagraph;
   (*pChapter) << pPara;

   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   INIT_SCALAR_PROTOTYPE(rptRcScalar, scalar, pDispUnits->Scalar);
   INIT_UV_PROTOTYPE( rptForceUnitValue,   force,       pDispUnits->GeneralForce, true);
   INIT_UV_PROTOTYPE( rptLengthUnitValue,  longLength,  pDispUnits->SpanLength,   true);
   INIT_UV_PROTOTYPE( rptLengthUnitValue,  shortLength, pDispUnits->ComponentDim, true);
   INIT_UV_PROTOTYPE( rptMomentUnitValue,  moment,      pDispUnits->Moment,       false);
   INIT_UV_PROTOTYPE( rptStressUnitValue,  stress,      pDispUnits->Stress,       false);
   INIT_UV_PROTOTYPE( rptAngleUnitValue,   tiltAngle,   pDispUnits->RadAngle,     true);
   INIT_UV_PROTOTYPE( rptAngleUnitValue,   crackAngle,  pDispUnits->RadAngle,     false);

   int girderType = m_pDoc->GetGirderType();
   stbGirder girder = m_pDoc->GetGirder(girderType);
   stbHaulingResults results = m_pDoc->GetHaulingResults();

   pPara = new rptParagraph(rptStyleManager::GetSubheadingStyle());
   *pChapter << pPara;
   *pPara << _T("General Parameters") << rptNewLine;
   pPara = new rptParagraph;
   *pChapter << pPara;

   *pPara << _T("Girder Length, ") << Sub2(_T("L"),_T("g")) << _T(" = ") << longLength.SetValue(girder.GetGirderLength()) << rptNewLine;
   *pPara << _T("Girder Weight, ") << Sub2(_T("W"),_T("g")) << _T(" = ") << force.SetValue(girder.GetGirderWeight()) << rptNewLine;

   //Float64 Ll,Lr;
   //pStabilityProblem->GetSupportLocations(&Ll,&Lr);
   //*pPara << _T("Left support overhang = ") << longLength.SetValue(Ll) << rptNewLine;
   //*pPara << _T("Right support overhang = ") << longLength.SetValue(Lr) << rptNewLine;
   //*pPara << _T("Clear span between lift points, ") << Sub2(_T("L"),_T("s")) << _T(" = ") << longLength.SetValue(pResults->Ls) << rptNewLine;
   //
   *pPara << rptNewLine;

   *pPara << _T("Offset Factor = ") << scalar.SetValue(results.OffsetFactor) << rptNewLine;
   *pPara << _T("Lateral Deflection (Sweep) = ") << shortLength.SetValue(results.LateralSweep) << rptNewLine;

   *pPara << _T("Eccentricity of center of gravity due to lateral sweep:") << rptNewLine;
   *pPara << _T(" No Impact = ") << shortLength.SetValue(results.EccLateralSweep[IMPACT_NONE]);
   *pPara << _T(", Impact Up = ") << shortLength.SetValue(results.EccLateralSweep[IMPACT_UP]);
   *pPara << _T(", Impact Down = ") << shortLength.SetValue(results.EccLateralSweep[IMPACT_DOWN]) << rptNewLine;

   *pPara << _T("Height of center of gravity of the cambered arch below pick points = ") << shortLength.SetValue(results.Dra[IMPACT_NONE]) << rptNewLine;
   
   *pPara << _T("Lateral eccentricity of Girder Self Weight due to Wind Load towards the left (") << Sub2(_T("e"),_T("wind")) << _T("):") << rptNewLine;
   *pPara << _T(" No Impact = ") << shortLength.SetValue(results.EccWind[IMPACT_NONE]);
   *pPara << _T(", Impact Up = ") << shortLength.SetValue(results.EccWind[IMPACT_UP]);
   *pPara << _T(", Impact Down = ") << shortLength.SetValue(results.EccWind[IMPACT_DOWN]) << rptNewLine;
   
   *pPara << _T("Mid-Height of cambered are below pick points = ") << shortLength.SetValue(results.Ywind[IMPACT_NONE]) << rptNewLine;
   
   *pPara << _T("Lateral Deflection due to girder weight on weak axis:");
   *pPara << _T(" No Impact = ") << shortLength.SetValue(results.Zo[IMPACT_NONE]);
   *pPara << _T(" Impact Up = ") << shortLength.SetValue(results.Zo[IMPACT_UP]);
   *pPara << _T(", Impact Down = ") << shortLength.SetValue(results.Zo[IMPACT_DOWN]) << rptNewLine;

   *pPara << _T("Lateral Deflection due to wind (Left):");
   *pPara << _T(" No Impact = ") << shortLength.SetValue(results.ZoWind[IMPACT_NONE]);
   *pPara << _T(", Impact Up = ") << shortLength.SetValue(results.ZoWind[IMPACT_UP]);
   *pPara << _T(", Impact Down = ") << shortLength.SetValue(results.ZoWind[IMPACT_DOWN]) << rptNewLine;

   rptRcTable* pTable1 = new rptRcTable(15,0);
   (*pPara) << pTable1 << rptNewLine;

   ColumnIndexType colIdx = 0;
   (*pTable1)(0,colIdx++) << COLHDR(_T("Dist from left") << rptNewLine << _T("end of girder"),rptLengthUnitTag,pDispUnits->SpanLength);
   (*pTable1)(0,colIdx++) << COLHDR(Sub2(_T("M"),_T("g")), rptMomentUnitTag, pDispUnits->Moment);
   (*pTable1)(0,colIdx++) << COLHDR(Sub2(_T("M"),_T("w")), rptMomentUnitTag, pDispUnits->Moment);
   (*pTable1)(0,colIdx++) << COLHDR(Sub2(_T("f"),_T("ps")) << rptNewLine << _T("Top"), rptStressUnitTag, pDispUnits->Stress);
   (*pTable1)(0,colIdx++) << COLHDR(Sub2(_T("f"),_T("ps")) << rptNewLine << _T("Bottom"), rptStressUnitTag, pDispUnits->Stress);
   (*pTable1)(0,colIdx++) << COLHDR(Sub2(_T("f"),_T("g")) << rptNewLine << _T("Top"), rptStressUnitTag, pDispUnits->Stress);
   (*pTable1)(0,colIdx++) << COLHDR(Sub2(_T("f"),_T("g")) << rptNewLine << _T("Bottom"), rptStressUnitTag, pDispUnits->Stress);
   (*pTable1)(0,colIdx++) << COLHDR(Sub2(_T("f"),_T("w")) << rptNewLine << _T("Top Left - Left"), rptStressUnitTag, pDispUnits->Stress);
   (*pTable1)(0,colIdx++) << COLHDR(Sub2(_T("f"),_T("w")) << rptNewLine << _T("Top Right - Left"), rptStressUnitTag, pDispUnits->Stress);
   (*pTable1)(0,colIdx++) << COLHDR(Sub2(_T("f"),_T("w")) << rptNewLine << _T("Bottom Left - Left"), rptStressUnitTag, pDispUnits->Stress);
   (*pTable1)(0,colIdx++) << COLHDR(Sub2(_T("f"),_T("w")) << rptNewLine << _T("Bottom Right - Left"), rptStressUnitTag, pDispUnits->Stress);
   (*pTable1)(0,colIdx++) << COLHDR(Sub2(_T("f"),_T("w")) << rptNewLine << _T("Top Left - Right"), rptStressUnitTag, pDispUnits->Stress);
   (*pTable1)(0,colIdx++) << COLHDR(Sub2(_T("f"),_T("w")) << rptNewLine << _T("Top Right - Right"), rptStressUnitTag, pDispUnits->Stress);
   (*pTable1)(0,colIdx++) << COLHDR(Sub2(_T("f"),_T("w")) << rptNewLine << _T("Bottom Left - Right"), rptStressUnitTag, pDispUnits->Stress);
   (*pTable1)(0,colIdx++) << COLHDR(Sub2(_T("f"),_T("w")) << rptNewLine << _T("Bottom Right - Right"), rptStressUnitTag, pDispUnits->Stress);

   longLength.ShowUnitTag(false);

   RowIndexType rowIdx = pTable1->GetNumberOfHeaderRows();
   BOOST_FOREACH(stbHaulingSectionResult& sectionResult,results.vSectionResults)
   {
      colIdx = 0;
      (*pTable1)(rowIdx,colIdx++) << longLength.SetValue(sectionResult.X);
      (*pTable1)(rowIdx,colIdx++) << moment.SetValue(sectionResult.Mg);
      (*pTable1)(rowIdx,colIdx++) << moment.SetValue(sectionResult.Mw);

      (*pTable1)(rowIdx,colIdx++) << stress.SetValue(sectionResult.fps[TOP]);
      (*pTable1)(rowIdx,colIdx++) << stress.SetValue(sectionResult.fps[BOTTOM]);

      (*pTable1)(rowIdx,colIdx++) << stress.SetValue(sectionResult.fg[TOP]);
      (*pTable1)(rowIdx,colIdx++) << stress.SetValue(sectionResult.fg[BOTTOM]);

      (*pTable1)(rowIdx,colIdx++) << stress.SetValue(sectionResult.fw[TOP_LEFT]);
      (*pTable1)(rowIdx,colIdx++) << stress.SetValue(sectionResult.fw[TOP_RIGHT]);
      (*pTable1)(rowIdx,colIdx++) << stress.SetValue(sectionResult.fw[BOTTOM_LEFT]);
      (*pTable1)(rowIdx,colIdx++) << stress.SetValue(sectionResult.fw[BOTTOM_RIGHT]);

      (*pTable1)(rowIdx,colIdx++) << stress.SetValue(-sectionResult.fw[TOP_LEFT]);
      (*pTable1)(rowIdx,colIdx++) << stress.SetValue(-sectionResult.fw[TOP_RIGHT]);
      (*pTable1)(rowIdx,colIdx++) << stress.SetValue(-sectionResult.fw[BOTTOM_LEFT]);
      (*pTable1)(rowIdx,colIdx++) << stress.SetValue(-sectionResult.fw[BOTTOM_RIGHT]);

      rowIdx++;
   }

   std::_tstring strImpact[] = {_T("No impact"),_T("Impact Up"),_T("Impact Down")};
   std::_tstring strWind[] = {_T("Wind Left"), _T("Wind Right")};
   std::_tstring strCF[] = {_T("Centrifugal Force Left"), _T("Centrifugal Force Right")};
   std::_tstring strFlange[] = {_T("Top Left"),_T("Top Right"),_T("Bottom Left"),_T("Bottom Right")};

   for ( int impact = 0; impact < 3; impact++ )
   {
      for ( int wind = 0; wind < 2; wind++ )
      {
         for ( int cf = 0; cf < 2; cf++ )
         {
            *pPara << strImpact[impact].c_str() << _T(" ") << strWind[wind].c_str() << _T(" ") << strCF[cf].c_str() << rptNewLine;
            *pPara << Sub2(symbol(theta),_T("eq")) << _T(" = ") << tiltAngle.SetValue(results.ThetaEq[impact][wind][cf]);

            rptRcTable* pTable2 = new rptRcTable(13,0);
            (*pPara) << pTable2 << rptNewLine;
            colIdx = 0;
            (*pTable2)(0,colIdx++) << COLHDR(_T("Dist from left") << rptNewLine << _T("end of girder"),rptLengthUnitTag,pDispUnits->SpanLength);
            (*pTable2)(0,colIdx++) << COLHDR(Sub2(_T("f"),_T("direct")) << rptNewLine << _T("Top Left"),rptStressUnitTag,pDispUnits->Stress);
            (*pTable2)(0,colIdx++) << COLHDR(Sub2(_T("f"),_T("direct")) << rptNewLine << _T("Top Right"),rptStressUnitTag,pDispUnits->Stress);
            (*pTable2)(0,colIdx++) << COLHDR(Sub2(_T("f"),_T("direct")) << rptNewLine << _T("Bottom Left"),rptStressUnitTag,pDispUnits->Stress);
            (*pTable2)(0,colIdx++) << COLHDR(Sub2(_T("f"),_T("direct")) << rptNewLine << _T("Bottom Right"),rptStressUnitTag,pDispUnits->Stress);
            (*pTable2)(0,colIdx++) << COLHDR(Sub2(_T("f"),_T("tilt")) << rptNewLine << _T("Top Left"),rptStressUnitTag,pDispUnits->Stress);
            (*pTable2)(0,colIdx++) << COLHDR(Sub2(_T("f"),_T("tilt")) << rptNewLine << _T("Top Right"),rptStressUnitTag,pDispUnits->Stress);
            (*pTable2)(0,colIdx++) << COLHDR(Sub2(_T("f"),_T("tilt")) << rptNewLine << _T("Bottom Left"),rptStressUnitTag,pDispUnits->Stress);
            (*pTable2)(0,colIdx++) << COLHDR(Sub2(_T("f"),_T("tilt")) << rptNewLine << _T("Bottom Right"),rptStressUnitTag,pDispUnits->Stress);
            (*pTable2)(0,colIdx++) << COLHDR(Sub2(_T("f"),_T("total")) << rptNewLine << _T("Top Left"),rptStressUnitTag,pDispUnits->Stress);
            (*pTable2)(0,colIdx++) << COLHDR(Sub2(_T("f"),_T("total")) << rptNewLine << _T("Top Right"),rptStressUnitTag,pDispUnits->Stress);
            (*pTable2)(0,colIdx++) << COLHDR(Sub2(_T("f"),_T("total")) << rptNewLine << _T("Bottom Left"),rptStressUnitTag,pDispUnits->Stress);
            (*pTable2)(0,colIdx++) << COLHDR(Sub2(_T("f"),_T("total")) << rptNewLine << _T("Bottom Right"),rptStressUnitTag,pDispUnits->Stress);

            rptRcTable* pTable3 = new rptRcTable(6,0);
            (*pPara) << pTable3 << rptNewLine;
            colIdx = 0;
            (*pTable3)(0,colIdx++) << COLHDR(_T("Dist from left") << rptNewLine << _T("end of girder"),rptLengthUnitTag,pDispUnits->SpanLength);
            (*pTable3)(0,colIdx++) << COLHDR(Sub2(symbol(theta),_T("eq")),rptAngleUnitTag,pDispUnits->RadAngle);
            (*pTable3)(0,colIdx++) << COLHDR(Sub2(_T("M"),_T("cr")),rptMomentUnitTag,pDispUnits->Moment);
            (*pTable3)(0,colIdx++) << _T("Cracked Flange");
            (*pTable3)(0,colIdx++) << COLHDR(Sub2(symbol(theta),_T("cr")),rptAngleUnitTag,pDispUnits->RadAngle);
            (*pTable3)(0,colIdx++) << Sub2(_T("FS"),_T("cr"));
            
            rowIdx = pTable2->GetNumberOfHeaderRows();
            BOOST_FOREACH(stbHaulingSectionResult& sectionResult,results.vSectionResults)
            {
               colIdx = 0;
               (*pTable2)(rowIdx,colIdx++) << longLength.SetValue(sectionResult.X);
               (*pTable2)(rowIdx,colIdx++) << stress.SetValue(sectionResult.fDirect[impact][wind][cf][TOP_LEFT]);
               (*pTable2)(rowIdx,colIdx++) << stress.SetValue(sectionResult.fDirect[impact][wind][cf][TOP_RIGHT]);
               (*pTable2)(rowIdx,colIdx++) << stress.SetValue(sectionResult.fDirect[impact][wind][cf][BOTTOM_LEFT]);
               (*pTable2)(rowIdx,colIdx++) << stress.SetValue(sectionResult.fDirect[impact][wind][cf][BOTTOM_RIGHT]);

               (*pTable2)(rowIdx,colIdx++) << stress.SetValue(sectionResult.fTilt[impact][wind][cf][TOP_LEFT]);
               (*pTable2)(rowIdx,colIdx++) << stress.SetValue(sectionResult.fTilt[impact][wind][cf][TOP_RIGHT]);
               (*pTable2)(rowIdx,colIdx++) << stress.SetValue(sectionResult.fTilt[impact][wind][cf][BOTTOM_LEFT]);
               (*pTable2)(rowIdx,colIdx++) << stress.SetValue(sectionResult.fTilt[impact][wind][cf][BOTTOM_RIGHT]);

               (*pTable2)(rowIdx,colIdx++) << stress.SetValue(sectionResult.f[impact][wind][cf][TOP_LEFT]);
               (*pTable2)(rowIdx,colIdx++) << stress.SetValue(sectionResult.f[impact][wind][cf][TOP_RIGHT]);
               (*pTable2)(rowIdx,colIdx++) << stress.SetValue(sectionResult.f[impact][wind][cf][BOTTOM_LEFT]);
               (*pTable2)(rowIdx,colIdx++) << stress.SetValue(sectionResult.f[impact][wind][cf][BOTTOM_RIGHT]);


               colIdx = 0;
               (*pTable3)(rowIdx,colIdx++) << longLength.SetValue(sectionResult.X);
               (*pTable3)(rowIdx,colIdx++) << moment.SetValue(sectionResult.Mcr[impact][wind][cf]);
               (*pTable3)(rowIdx,colIdx++) << strFlange[sectionResult.CrackedFlange[impact][wind][cf]].c_str();
               (*pTable3)(rowIdx,colIdx++) << crackAngle.SetValue(sectionResult.ThetaCrack[impact][wind][cf]);
               (*pTable3)(rowIdx,colIdx++) << scalar.SetValue(sectionResult.FScr[impact][wind][cf]);

               rowIdx++;
            }
         } // next cf direction
      } // next wind direction
   } // next impact direction

   *pPara << _T("The minimum factor of safety against cracking occurs at ") << longLength.SetValue(results.vSectionResults[results.FScrAnalysisPointIndex].X) << _T(" ");
   *pPara << strFlange[results.vSectionResults[results.FScrAnalysisPointIndex].CrackedFlange[results.FScrImpactDirection][results.FScrWindDirection][results.FScrCFDirection]].c_str() << _T(" flange tip with ");
   *pPara << strImpact[results.FScrImpactDirection].c_str() << _T(" and ") << strWind[results.FScrWindDirection] << rptNewLine;
   *pPara << Sub2(_T("FS"),_T("cr")) << _T(" Min = ") << scalar.SetValue(results.MinFScr) << rptNewLine;

   for ( int impact = 0; impact < 3; impact++ )
   {
      for (int wind = 0; wind < 2; wind++ )
      {
         for ( int cf = 0; cf < 2; cf++ )
         {
            *pPara << strImpact[impact].c_str() << _T(" ") << strWind[wind].c_str() << _T(" ") << strCF[cf].c_str() << rptNewLine;
            *pPara << Sub2(_T("FS"),_T("f")) << _T(" = ") << scalar.SetValue(results.FsFailure[impact][wind][cf]) << rptNewLine;
         }
      }
   }

   *pPara << rptNewLine;
   *pPara << Sub2(_T("FS"),_T("f")) << _T(" Min = ") << scalar.SetValue(results.MinFsFailure) << rptNewLine;

   *pPara << rptNewLine;
   *pPara << Sub2(_T("FS"),_T("ro")) << _T(" Min = ") << scalar.SetValue(results.MinFsRollover) << rptNewLine;

   return pChapter;
}

CChapterBuilder* CPGStableHaulingDetailsChapterBuilder::Clone() const
{
   return new CPGStableHaulingDetailsChapterBuilder(m_pDoc);
}
