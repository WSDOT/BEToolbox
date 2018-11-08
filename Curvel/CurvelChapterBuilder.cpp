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
#include "CurvelChapterBuilder.h"
#include "CurvelReportSpecification.h"
#include "..\BEToolboxUtilities.h"
#include <Math\QuadraticSolver.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////
CCurvelChapterBuilder::CCurvelChapterBuilder(CCurvelDoc* pDoc)
{
   m_pDoc = pDoc;

   m_SkewAngle.CoCreateInstance(CLSID_Angle);

   m_AngleFormatter.CoCreateInstance(CLSID_AngleDisplayUnitFormatter);
   m_AngleFormatter->put_CondensedFormat(VARIANT_FALSE);
   m_AngleFormatter->put_Signed(VARIANT_FALSE);
   m_bstrAngleFormat = CComBSTR("°,\',\"");
}

CCurvelChapterBuilder::~CCurvelChapterBuilder(void)
{
}

LPCTSTR CCurvelChapterBuilder::GetName() const
{
   return _T("Curvel");
}

Uint16 CCurvelChapterBuilder::GetMaxLevel() const
{
   return 1;
}

rptChapter* CCurvelChapterBuilder::Build(CReportSpecification* pRptSpec,Uint16 level) const
{
   CCurvelReportSpecification* pTheRptSpec = (CCurvelReportSpecification*)pRptSpec;
   Init(pTheRptSpec);

   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();
   INIT_UV_PROTOTYPE( rptLengthUnitValue,  alignment,  pDispUnits->AlignmentLength, true);

   rptRcScalar scalar;
   scalar.SetWidth(7);
   scalar.SetPrecision(4);
   scalar.SetFormat(sysNumericFormatTool::Fixed);
   scalar.SetTolerance(1.0e-6);

   std::_tstring strSlopeTag = pDispUnits->AlignmentLength.UnitOfMeasure.UnitTag();

   rptChapter* pChapter = new rptChapter;
   rptParagraph* pPara;

   pPara = new rptParagraph(rptStyleManager::GetHeadingStyle());
   (*pChapter) << pPara;
   *pPara << _T("Vertical Curve Data");

   pPara = new rptParagraph;
   (*pChapter) << pPara;

   Float64 g1,g2,pviStation,pviElevation,length;
   pTheRptSpec->GetVerticalCurveParameters(&g1,&g2,&pviStation,&pviElevation,&length);

   Float64 profileGradeOffset = pTheRptSpec->GetProfileGradeOffset();

   bool bCorrectForSuperelevation = pTheRptSpec->CorrectForSuperelevation();

   *pPara << _T("PVI Station = ") << rptRcStation(pviStation,&pDispUnits->StationFormat);
   *pPara << _T(" ");
   *pPara << _T("PVI Elevation = ") << alignment.SetValue(pviElevation) << rptNewLine;

   *pPara << _T("Length = ") << alignment.SetValue(length);
   *pPara << _T(" ");
   *pPara << _T("g1 = ") << scalar.SetValue(g1*100) << _T("%");
   *pPara << _T(" ");
   *pPara << _T("g2 = ") << scalar.SetValue(g2*100) << _T("%") << rptNewLine;

   if ( pTheRptSpec->CorrectForSuperelevation() )
   {
      pPara = new rptParagraph(rptStyleManager::GetHeadingStyle());
      (*pChapter) << pPara;
      *pPara << _T("Superelevation Data");

      pPara = new rptParagraph;
      (*pChapter) << pPara;

      rptRcTable* pTable = rptStyleManager::CreateDefaultTable(3);
      *pPara << pTable << rptNewLine;

      *pPara << _T("The profile grade point is ") << RPT_OFFSET(pTheRptSpec->GetProfileGradeOffset(),alignment) << _T(" of crown point");


      (*pTable)(0,0) << _T("Station");
      (*pTable)(0,1) << _T("Left Slope") << rptNewLine << _T("(") << strSlopeTag << _T("\\") << strSlopeTag << _T(")");
      (*pTable)(0,2) << _T("Right Slope") << rptNewLine << _T("(") << strSlopeTag << _T("\\") << strSlopeTag << _T(")");

      RowIndexType row = pTable->GetNumberOfHeaderRows();
      for ( IndexType idx = 0; idx < 3; idx++,row++ )
      {
         (*pTable)(row,0) << rptRcStation(pTheRptSpec->GetSuperelevationPoint(idx).Station,&pDispUnits->StationFormat);
         (*pTable)(row,1) << scalar.SetValue(pTheRptSpec->GetSuperelevationPoint(idx).LeftSlope);
         (*pTable)(row,2) << scalar.SetValue(pTheRptSpec->GetSuperelevationPoint(idx).RightSlope);
      }
   }

   alignment.ShowUnitTag(false);

   pPara = new rptParagraph(rptStyleManager::GetHeadingStyle());
   (*pChapter) << pPara;
   *pPara << _T("Vertical Curve Elevations");

   pPara = new rptParagraph;
   (*pChapter) << pPara;

   rptRcTable* pTable = rptStyleManager::CreateDefaultTable(bCorrectForSuperelevation ? 8 : 4);
   *pPara << pTable << rptNewLine;


   ColumnIndexType col = 0;
   (*pTable)(0,col++) << _T("");
   (*pTable)(0,col++) << _T("Station");
   (*pTable)(0,col++) << COLHDR(_T("Profile") << rptNewLine << _T("Grade") << rptNewLine << _T("Elevation"),rptLengthUnitTag,pDispUnits->AlignmentLength);
   (*pTable)(0,col++) << _T("Grade") << rptNewLine << _T("(%)");

   if ( bCorrectForSuperelevation )
   {
      (*pTable)(0,col++) << COLHDR(_T("Offset") << rptNewLine << _T("from") << rptNewLine << _T("Crown"),rptLengthUnitTag,pDispUnits->AlignmentLength);
      (*pTable)(0,col++) << COLHDR(_T("Crown") << rptNewLine << _T("Offset") << rptNewLine << _T("Elevation"),rptLengthUnitTag,pDispUnits->AlignmentLength);
      (*pTable)(0,col++) << _T("Crown") << rptNewLine << _T("Slope") << rptNewLine << _T("Left") << rptNewLine << _T("(") << strSlopeTag << _T("\\") << strSlopeTag << _T(")");
      (*pTable)(0,col++) << _T("Crown") << rptNewLine << _T("Slope") << rptNewLine << _T("Right") << rptNewLine << _T("(") << strSlopeTag << _T("\\") << strSlopeTag << _T(")");
   }

   RowIndexType row = pTable->GetNumberOfHeaderRows();

   // Report BVC
   CComPtr<IProfileElement> profileElement;
   m_Profile->get_Item(0,&profileElement);
   CComPtr<IUnknown> pUnk;
   profileElement->get_Value(&pUnk);
   CComQIPtr<IVertCurve> vc(pUnk);
   CComPtr<IProfilePoint> bvc;
   vc->get_BVC(&bvc);
   CComPtr<IStation> bvcStation;
   bvc->get_Station(&bvcStation);
   Float64 bvcStationValue;
   bvcStation->get_Value(&bvcStationValue);
   Float64 bvcElevation;
   bvc->get_Elevation(&bvcElevation);

   col = 0;
   (*pTable)(row,col++) << _T("BVC");
   (*pTable)(row,col++) << rptRcStation(bvcStationValue,&pDispUnits->StationFormat);

   (*pTable)(row,col++) << alignment.SetValue(bvcElevation);

   Float64 grade;
   m_Profile->Grade(CComVariant(bvcStation),&grade);
   (*pTable)(row,col++) << scalar.SetValue(grade*100);

   if ( bCorrectForSuperelevation )
   {
      Float64 offset = 0.0; // elevations computed at this offset from the alignment
      (*pTable)(row,col++) << RPT_OFFSET(offset,alignment);

      Float64 elevation;
      m_Profile->Elevation(CComVariant(bvcStation),offset,&elevation);
      (*pTable)(row,col++) << alignment.SetValue(elevation);

      Float64 slope;
      m_Profile->TemplateSegmentSlope(COGO_FINISHED_SURFACE_ID,CComVariant(bvcStation),1,&slope);
      (*pTable)(row,col++) << scalar.SetValue(-slope);

      m_Profile->TemplateSegmentSlope(COGO_FINISHED_SURFACE_ID,CComVariant(bvcStation),2,&slope);
      (*pTable)(row,col++) << scalar.SetValue(slope);
   }

   row++;


   // Report EVC
   CComPtr<IProfilePoint> evc;
   vc->get_EVC(&evc);
   CComPtr<IStation> evcStation;
   evc->get_Station(&evcStation);
   Float64 evcStationValue;
   evcStation->get_Value(&evcStationValue);
   Float64 evcElevation;
   evc->get_Elevation(&evcElevation);

   col = 0;
   (*pTable)(row,col++) << _T("EVC");
   (*pTable)(row,col++) << rptRcStation(evcStationValue,&pDispUnits->StationFormat);

   (*pTable)(row,col++) << alignment.SetValue(evcElevation);

   m_Profile->Grade(CComVariant(evcStation),&grade);
   (*pTable)(row,col++) << scalar.SetValue(grade*100);

   if ( bCorrectForSuperelevation )
   {
      Float64 offset = 0.0; // elevations computed at this offset from the alignment
      (*pTable)(row,col++) << RPT_OFFSET(offset,alignment);

      Float64 elevation;
      m_Profile->Elevation(CComVariant(evcStation),offset,&elevation);
      (*pTable)(row,col++) << alignment.SetValue(elevation);

      Float64 slope;
      m_Profile->TemplateSegmentSlope(COGO_FINISHED_SURFACE_ID,CComVariant(evcStation),1,&slope);
      (*pTable)(row,col++) << scalar.SetValue(-slope);

      m_Profile->TemplateSegmentSlope(COGO_FINISHED_SURFACE_ID,CComVariant(evcStation),2,&slope);
      (*pTable)(row,col++) << scalar.SetValue(slope);
   }


   row++;

   // Report Low point
   CComPtr<IProfilePoint> lowPt;
   vc->get_LowPoint(&lowPt);
   CComPtr<IStation> lpStation;
   lowPt->get_Station(&lpStation);
   Float64 lpStationValue;
   lpStation->get_Value(&lpStationValue);
   Float64 lpElevation;
   lowPt->get_Elevation(&lpElevation);

   col = 0;
   (*pTable)(row,col++) << _T("Low");
   (*pTable)(row,col++) << rptRcStation(lpStationValue,&pDispUnits->StationFormat);

   (*pTable)(row,col++) << alignment.SetValue(lpElevation);

   m_Profile->Grade(CComVariant(lpStation),&grade);
   (*pTable)(row,col++) << scalar.SetValue(grade*100);

   if ( bCorrectForSuperelevation )
   {
      Float64 offset = 0.0; // elevations computed at this offset from the alignment
      (*pTable)(row,col++) << RPT_OFFSET(offset,alignment);

      Float64 elevation;
      m_Profile->Elevation(CComVariant(lpStation),offset,&elevation);
      (*pTable)(row,col++) << alignment.SetValue(elevation);

      Float64 slope;
      m_Profile->TemplateSegmentSlope(COGO_FINISHED_SURFACE_ID,CComVariant(lpStation),1,&slope);
      (*pTable)(row,col++) << scalar.SetValue(-slope);

      m_Profile->TemplateSegmentSlope(COGO_FINISHED_SURFACE_ID,CComVariant(lpStation),2,&slope);
      (*pTable)(row,col++) << scalar.SetValue(slope);
   }

   row++;

   // Report Hight point
   CComPtr<IProfilePoint> highPt;
   vc->get_HighPoint(&highPt);
   CComPtr<IStation> hpStation;
   highPt->get_Station(&hpStation);
   Float64 hpStationValue;
   hpStation->get_Value(&hpStationValue);
   Float64 hpElevation;
   highPt->get_Elevation(&hpElevation);

   col = 0;
   (*pTable)(row,col++) << _T("High");
   (*pTable)(row,col++) << rptRcStation(hpStationValue,&pDispUnits->StationFormat);

   (*pTable)(row,col++) << alignment.SetValue(hpElevation);

   m_Profile->Grade(CComVariant(hpStation),&grade);
   (*pTable)(row,col++) << scalar.SetValue(grade*100);

   if ( bCorrectForSuperelevation )
   {
      Float64 offset = 0.0; // elevations computed at this offset from the alignment
      (*pTable)(row,col++) << RPT_OFFSET(offset,alignment);

      Float64 elevation;
      m_Profile->Elevation(CComVariant(hpStation),offset,&elevation);
      (*pTable)(row,col++) << alignment.SetValue(elevation);

      Float64 slope;
      m_Profile->TemplateSegmentSlope(COGO_FINISHED_SURFACE_ID,CComVariant(hpStation),1,&slope);
      (*pTable)(row,col++) << scalar.SetValue(-slope);

      m_Profile->TemplateSegmentSlope(COGO_FINISHED_SURFACE_ID,CComVariant(hpStation),2,&slope);
      (*pTable)(row,col++) << scalar.SetValue(slope);
   }

   const std::vector<IndividualStation>& stations(pTheRptSpec->GetIndividualStations());
   const std::vector<StationRange>& ranges(pTheRptSpec->GetStationRanges());
   if ( 0 < stations.size() + ranges.size() ) 
   {
      // report input stations
      pPara = new rptParagraph(rptStyleManager::GetHeadingStyle());
      (*pChapter) << pPara;
      *pPara << _T("Elevations");

      pPara = new rptParagraph;
      (*pChapter) << pPara;

      pTable = rptStyleManager::CreateDefaultTable(bCorrectForSuperelevation ? 7 : 3);
      *pPara << pTable << rptNewLine;

      col = 0;
      (*pTable)(0,col++) << _T("Station");
      (*pTable)(0,col++) << COLHDR(_T("Profile") << rptNewLine << _T("Grade") << rptNewLine << _T("Elevation"),rptLengthUnitTag,pDispUnits->AlignmentLength);
      (*pTable)(0,col++) << _T("Grade") << rptNewLine << _T("(%)");

      if ( bCorrectForSuperelevation )
      {
         (*pTable)(0,col++) << COLHDR(_T("Offset") << rptNewLine << _T("from") << rptNewLine << _T("Crown"),rptLengthUnitTag,pDispUnits->AlignmentLength);
         (*pTable)(0,col++) << COLHDR(_T("Crown") << rptNewLine << _T("Offset") << rptNewLine << _T("Elevation"),rptLengthUnitTag,pDispUnits->AlignmentLength);
         (*pTable)(0,col++) << _T("Crown") << rptNewLine << _T("Slope") << rptNewLine << _T("Left") << rptNewLine << _T("(") << strSlopeTag << _T("\\") << strSlopeTag << _T(")");
         (*pTable)(0,col++) << _T("Crown") << rptNewLine << _T("Slope") << rptNewLine << _T("Right") << rptNewLine << _T("(") << strSlopeTag << _T("\\") << strSlopeTag << _T(")");
      }

      row = pTable->GetNumberOfHeaderRows();

      std::vector<IndividualStation>::const_iterator iter(stations.begin());
      std::vector<IndividualStation>::const_iterator end(stations.end());
      for ( ; iter != end; iter++, row++ )
      {
         const IndividualStation& station(*iter);
         col = 0;
         (*pTable)(row,col++) << rptRcStation(station.Station,&pDispUnits->StationFormat);

         Float64 elevation;
         m_Profile->Elevation(CComVariant(station.Station),profileGradeOffset,&elevation);
         (*pTable)(row,col++) << alignment.SetValue(elevation);

         Float64 grade;
         m_Profile->Grade(CComVariant(station.Station),&grade);
         (*pTable)(row,col++) << scalar.SetValue(grade*100);

         if ( bCorrectForSuperelevation )
         {
            (*pTable)(row,col++) << RPT_OFFSET(station.Offset,alignment);

            m_Profile->Elevation(CComVariant(station.Station),station.Offset,&elevation);
            (*pTable)(row,col++) << alignment.SetValue(elevation);

            Float64 slope;
            m_Profile->TemplateSegmentSlope(COGO_FINISHED_SURFACE_ID,CComVariant(station.Station),1,&slope);
            (*pTable)(row,col++) << scalar.SetValue(-slope);

            m_Profile->TemplateSegmentSlope(COGO_FINISHED_SURFACE_ID,CComVariant(station.Station),2,&slope);
            (*pTable)(row,col++) << scalar.SetValue(slope);
         }
      }

      std::vector<StationRange>::const_iterator srIter(ranges.begin());
      std::vector<StationRange>::const_iterator srIterEnd(ranges.end());
      for ( ; srIter != srIterEnd; srIter++, row++ )
      {
         const StationRange& range(*srIter);
         Float64 station = range.StartStation;
         Float64 inc = (range.EndStation - range.StartStation)/range.nSpaces;
         for ( IndexType i = 0; i <= range.nSpaces; i++, station += inc, row++ )
         {
            col = 0;
            (*pTable)(row,col++) << rptRcStation(station,&pDispUnits->StationFormat);

            Float64 elevation;
            m_Profile->Elevation(CComVariant(station),profileGradeOffset,&elevation);
            (*pTable)(row,col++) << alignment.SetValue(elevation);

            Float64 grade;
            m_Profile->Grade(CComVariant(station),&grade);
            (*pTable)(row,col++) << scalar.SetValue(grade*100);

           if ( bCorrectForSuperelevation )
            {
               (*pTable)(row,col++) << RPT_OFFSET(range.Offset,alignment);

               m_Profile->Elevation(CComVariant(station),range.Offset,&elevation);
               (*pTable)(row,col++) << alignment.SetValue(elevation);

               Float64 slope;
               m_Profile->TemplateSegmentSlope(COGO_FINISHED_SURFACE_ID,CComVariant(station),1,&slope);
               (*pTable)(row,col++) << scalar.SetValue(-slope);

               m_Profile->TemplateSegmentSlope(COGO_FINISHED_SURFACE_ID,CComVariant(station),2,&slope);
               (*pTable)(row,col++) << scalar.SetValue(slope);
           }
         }
      }
   }

   const std::vector<SkewLine>& skewLines(pTheRptSpec->GetSkewLines());
   if ( 0 < skewLines.size() )
   {
      pPara = new rptParagraph(rptStyleManager::GetHeadingStyle());
      (*pChapter) << pPara;
      *pPara << _T("Skew Line Elevations");

      pPara = new rptParagraph;
      (*pChapter) << pPara;

      pTable = rptStyleManager::CreateDefaultTable(12);
      *pPara << pTable << rptNewLine;


      col = 0;
      (*pTable)(0,col++) << _T("Skew Angle");
      (*pTable)(0,col++) << COLHDR(_T("Radius"),rptLengthUnitTag,pDispUnits->AlignmentLength);
      (*pTable)(0,col++) << COLHDR(_T("Crown") << rptNewLine << _T("Offset"),rptLengthUnitTag,pDispUnits->AlignmentLength);
      (*pTable)(0,col++) << _T("L-Line") << rptNewLine << _T("Station");
      (*pTable)(0,col++) << COLHDR(_T("Skew") << rptNewLine << _T("Distance"), rptLengthUnitTag,pDispUnits->AlignmentLength);
      (*pTable)(0,col++) << _T("Offset") << rptNewLine << _T("Point") << rptNewLine << _T("Station");
      (*pTable)(0,col++) << COLHDR(_T("Profile") << rptNewLine << _T("Grade") << rptNewLine << _T("Elevation"),rptLengthUnitTag,pDispUnits->AlignmentLength);
      (*pTable)(0,col++) << _T("Grade") << rptNewLine << _T("(%)");
      (*pTable)(0,col++) << COLHDR(_T("Offset") << rptNewLine << _T("from") << rptNewLine << _T("Crown"),rptLengthUnitTag,pDispUnits->AlignmentLength);
      (*pTable)(0,col++) << COLHDR(_T("Crown") << rptNewLine << _T("Offset") << rptNewLine << _T("Elevation"),rptLengthUnitTag,pDispUnits->AlignmentLength);
      (*pTable)(0,col++) << _T("Crown") << rptNewLine << _T("Slope") << rptNewLine << _T("Left") << rptNewLine << _T("(") << strSlopeTag << _T("\\") << strSlopeTag << _T(")");
      (*pTable)(0,col++) << _T("Crown") << rptNewLine << _T("Slope") << rptNewLine << _T("Right") << rptNewLine << _T("(") << strSlopeTag << _T("\\") << strSlopeTag << _T(")");

      row = pTable->GetNumberOfHeaderRows();

      std::vector<SkewLine>::const_iterator skewIter(skewLines.begin());
      std::vector<SkewLine>::const_iterator skewIterEnd(skewLines.end());
      for ( ; skewIter != skewIterEnd; skewIter++, row++ )
      {
         const SkewLine& skewLine(*skewIter);
         UpdateAlignment(skewLine);
         col = 0;

         m_SkewAngle->FromString(CComBSTR(skewLine.strSkewAngle.c_str()));
         Float64 skewAngleValue;
         m_SkewAngle->get_Value(&skewAngleValue);

         CComBSTR bstrAngle;
         m_AngleFormatter->Format(skewAngleValue,m_bstrAngleFormat,&bstrAngle);
         (*pTable)(row,col++) << bstrAngle;

         (*pTable)(row,col++) << RPT_OFFSET(skewLine.Radius,alignment);

         (*pTable)(row,col++) << RPT_OFFSET(skewLine.CrownOffset,alignment);

         (*pTable)(row,col++) << rptRcStation(skewLine.Station,&pDispUnits->StationFormat);

         // Get the direction of the skew line
         CComPtr<IDirection> direction;
         m_Alignment->Normal(CComVariant(skewLine.Station),&direction);
         direction->IncrementBy(CComVariant(m_SkewAngle));

         Float64 skewDistance = skewLine.Offset;

         if ( skewLine.OffsetType == SkewLine::RadialFromCrownLine )
         {
            if ( IsZero(skewLine.Radius) )
            {
               skewDistance = (skewLine.Offset - skewLine.CrownOffset)/cos(fabs(skewAngleValue));
            }
            else
            {
               // Using the law of cosines, compute the distance along the skew line
               Float64 A,B,a;
               A = skewLine.Radius - (skewLine.Offset - skewLine.CrownOffset);
               B = skewLine.Radius;
               a = M_PI - skewAngleValue;

               Float64 k1 = 1.0;
               Float64 k2 = -2*B*cos(a);
               Float64 k3 = B*B - A*A;

               mathQuadraticSolver qSolver(k1,k2,k3);
               Float64 C1,C2;
               int nRoots = qSolver.Solve(&C1,&C2);
               ATLASSERT(nRoots == 2);
               Float64 C = (skewLine.Radius < 0 ? C2 : C1);

               skewDistance = -C;
            }
         }

         (*pTable)(row,col++) << RPT_OFFSET(skewDistance,alignment);

         // Locate the point at the end of the skewline
         CComPtr<IPoint2d> offsetPoint;
         m_Alignment->LocatePoint(CComVariant(skewLine.Station),omtAlongDirection,skewDistance,CComVariant(direction),&offsetPoint);

         // Get station and offset of offsetPoint
         CComPtr<IStation> offsetStation;
         Float64 offsetDistance;
         m_Alignment->Offset(offsetPoint,&offsetStation,&offsetDistance);

         Float64 offsetStationValue;
         offsetStation->get_Value(&offsetStationValue);

         (*pTable)(row,col++) << rptRcStation(offsetStationValue,&pDispUnits->StationFormat);

         Float64 elevation;
         m_Profile->Elevation(CComVariant(offsetStation),profileGradeOffset,&elevation);
         (*pTable)(row,col++) << alignment.SetValue(elevation);

         Float64 grade;
         m_Profile->Grade(CComVariant(offsetStation),&grade);
         (*pTable)(row,col++) << scalar.SetValue(grade*100);

         (*pTable)(row,col++) << RPT_OFFSET(offsetDistance+skewLine.CrownOffset,alignment);

         m_Profile->Elevation(CComVariant(offsetStation),offsetDistance+skewLine.CrownOffset,&elevation);
         (*pTable)(row,col++) << alignment.SetValue(elevation);

         Float64 slope;
         m_Profile->TemplateSegmentSlope(COGO_FINISHED_SURFACE_ID,CComVariant(offsetStation),1,&slope);
         (*pTable)(row,col++) << scalar.SetValue(-slope);

         m_Profile->TemplateSegmentSlope(COGO_FINISHED_SURFACE_ID,CComVariant(offsetStation),2,&slope);
         (*pTable)(row,col++) << scalar.SetValue(slope);
      }
   }

   return pChapter;
}

CChapterBuilder* CCurvelChapterBuilder::Clone() const
{
   return new CCurvelChapterBuilder(m_pDoc);
}

void CCurvelChapterBuilder::Init(CCurvelReportSpecification* pRptSpec) const
{
   m_Alignment.Release();
   m_Profile.Release();

   m_Alignment.CoCreateInstance(CLSID_Alignment);
   m_Alignment->get_Profile(&m_Profile);

   // Build the profile
   CComPtr<IVertCurve> vertCurve;
   vertCurve.CoCreateInstance(CLSID_VertCurve);

   Float64 g1,g2,pviStation,pviElevation,length;
   pRptSpec->GetVerticalCurveParameters(&g1,&g2,&pviStation,&pviElevation,&length);

   Float64 pbgStation   = pviStation   - Max(100.0,length);
   Float64 pbgElevation = pviElevation - g1*Max(100.0,length);

   Float64 pfgStation   = pviStation   + Max(100.0,length);
   Float64 pfgElevation = pviElevation + g2*Max(100.0,length);

   CComPtr<IProfilePoint> PBG;
   PBG.CoCreateInstance(CLSID_ProfilePoint);
   PBG->put_Station(CComVariant(pbgStation));
   PBG->put_Elevation(pbgElevation);

   CComPtr<IProfilePoint> PFG;
   PFG.CoCreateInstance(CLSID_ProfilePoint);
   PFG->put_Station(CComVariant(pfgStation));
   PFG->put_Elevation(pfgElevation);

   CComPtr<IProfilePoint> PVI;
   PVI.CoCreateInstance(CLSID_ProfilePoint);
   PVI->put_Station(CComVariant(pviStation));
   PVI->put_Elevation(pviElevation);

   vertCurve->putref_PBG(PBG);
   vertCurve->putref_PVI(PVI);
   vertCurve->putref_PFG(PFG);
   vertCurve->put_L1(length/2);
   vertCurve->put_L2(length/2);

   m_Profile->AddEx(vertCurve);

   Float64 profileGradeOffset = pRptSpec->GetProfileGradeOffset();

   // Create the roadway surface model
   CComPtr<ISurfaceCollection> surfaces;
   m_Profile->get_Surfaces(&surfaces);

   CComPtr<ISurface> surface;
   surface.CoCreateInstance(CLSID_Surface);
   surfaces->Add(surface);
   surface->put_ID(COGO_FINISHED_SURFACE_ID);

   CComPtr<ISurfaceTemplateCollection> templates;
   surface->get_SurfaceTemplates(&templates);

   surface->put_AlignmentPoint(2);
   if ( 0 < profileGradeOffset )
   {
      surface->put_ProfileGradePoint(3);
   }
   else if ( profileGradeOffset < 0 )
   {
      surface->put_ProfileGradePoint(1);
   }
   else
   {
      surface->put_ProfileGradePoint(2);
   }

   // determine the size of the surface
   Float64 minStation = DBL_MAX;
   Float64 maxStation = -DBL_MAX;
   Float64 width = 0;
   const std::vector<IndividualStation>& stations(pRptSpec->GetIndividualStations());
   std::vector<IndividualStation>::const_iterator isIter(stations.begin());
   std::vector<IndividualStation>::const_iterator isIterEnd(stations.end());
   for ( ; isIter != isIterEnd; isIter++ )
   {
      const IndividualStation& station(*isIter);
      minStation = Min(minStation,station.Station);
      maxStation = Max(maxStation,station.Station);
      width = Max(width,fabs(station.Offset));
   }

   const std::vector<StationRange>& ranges(pRptSpec->GetStationRanges());
   std::vector<StationRange>::const_iterator rangeIter(ranges.begin());
   std::vector<StationRange>::const_iterator rangeIterEnd(ranges.end());
   for ( ; rangeIter != rangeIterEnd; rangeIter++ )
   {
      const StationRange& range(*rangeIter);
      minStation = Min(minStation,range.StartStation);
      maxStation = Max(maxStation,range.EndStation);
      width = Max(width,fabs(range.Offset));
   }
   
   const std::vector<SkewLine>& skewLines(pRptSpec->GetSkewLines());
   std::vector<SkewLine>::const_iterator skewIter(skewLines.begin());
   std::vector<SkewLine>::const_iterator skewIterEnd(skewLines.end());
   for ( ; skewIter != skewIterEnd; skewIter++ )
   {
      const SkewLine& skew(*skewIter);
      minStation = Min(minStation,skew.Station);
      maxStation = Max(maxStation,skew.Station);
      width = Max(width,fabs(skew.Offset));
   }

   // vertical curve stations
   minStation = Min(minStation,pbgStation);
   maxStation = Max(maxStation,pfgStation);

   // expand the surface so it will fit without question
   minStation /= 2;
   maxStation *= 2;
   width *= 2;

   for ( IndexType i = 0; i < 3; i++ )
   {
      SuperelevationProfilePoint superelevationPoint = pRptSpec->GetSuperelevationPoint(i);

      if ( i == 0 )
      {
         CComPtr<ISurfaceTemplate> surfaceTemplate;
         surfaceTemplate.CoCreateInstance(CLSID_SurfaceTemplate);
         surfaceTemplate->put_Station(CComVariant(minStation));
         surfaceTemplate->AddSegment(width,-superelevationPoint.LeftSlope,tsHorizontal);
         surfaceTemplate->AddSegment(fabs(profileGradeOffset),-superelevationPoint.LeftSlope,tsHorizontal);
         surfaceTemplate->AddSegment(fabs(profileGradeOffset), superelevationPoint.RightSlope,tsHorizontal);
         surfaceTemplate->AddSegment(width, superelevationPoint.RightSlope,tsHorizontal);

         templates->Add(surfaceTemplate);
      }

      CComPtr<ISurfaceTemplate> surfaceTemplate;
      surfaceTemplate.CoCreateInstance(CLSID_SurfaceTemplate);
      surfaceTemplate->put_Station(CComVariant(superelevationPoint.Station));
      surfaceTemplate->AddSegment(width,-superelevationPoint.LeftSlope,tsHorizontal);
      surfaceTemplate->AddSegment(fabs(profileGradeOffset),-superelevationPoint.LeftSlope,tsHorizontal);
      surfaceTemplate->AddSegment(fabs(profileGradeOffset), superelevationPoint.RightSlope,tsHorizontal);
      surfaceTemplate->AddSegment(width, superelevationPoint.RightSlope,tsHorizontal);

      templates->Add(surfaceTemplate);

      if ( i == 2 )
      {
         CComPtr<ISurfaceTemplate> surfaceTemplate;
         surfaceTemplate.CoCreateInstance(CLSID_SurfaceTemplate);
         surfaceTemplate->put_Station(CComVariant(maxStation));
         surfaceTemplate->AddSegment(width,-superelevationPoint.LeftSlope,tsHorizontal);
         surfaceTemplate->AddSegment(fabs(profileGradeOffset),-superelevationPoint.LeftSlope,tsHorizontal);
         surfaceTemplate->AddSegment(fabs(profileGradeOffset), superelevationPoint.RightSlope,tsHorizontal);
         surfaceTemplate->AddSegment(width, superelevationPoint.RightSlope,tsHorizontal);

         templates->Add(surfaceTemplate);
      }
   }
}

void CCurvelChapterBuilder::UpdateAlignment(const SkewLine& skewLine) const
{
   m_Alignment->Clear();

   if ( IsZero(skewLine.Radius) )
   {
      m_Alignment->put_RefStation(CComVariant(0.0));
      return; // no need for horizontal curve objects if there is no radius
   }

   CComPtr<IHorzCurve> hc;
   hc.CoCreateInstance(CLSID_HorzCurve);

   CComPtr<IPoint2d> PBT, PFT, PI;
   hc->get_PBT(&PBT);
   hc->get_PI(&PI);
   hc->get_PFT(&PFT);

   Float64 angle = ::ToRadians(15.0);
   PBT->Move(0,0);
   PI->Move(3*fabs(skewLine.Radius),0);
   PFT->Move(3*fabs(skewLine.Radius) + 3*fabs(skewLine.Radius)*cos(angle),-3*skewLine.Radius*sin(angle));

   hc->put_Radius(fabs(skewLine.Radius));

   m_Alignment->AddEx(hc);

   Float64 Length;
   hc->get_TotalLength(&Length);

   // Define station at the TS point such that the station under consideration
   // is at the mid point of the curve
   m_Alignment->put_RefStation(CComVariant(skewLine.Station - Length/2));
}