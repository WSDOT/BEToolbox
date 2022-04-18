///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2022  Washington State Department of Transportation
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
#include "CurvelReportSpecification.h"
#include "CurvelDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCurvelReportSpecification::CCurvelReportSpecification(LPCTSTR strReportName) :
CReportSpecification(strReportName)
{
}

CCurvelReportSpecification::~CCurvelReportSpecification(void)
{
}

HRESULT CCurvelReportSpecification::Validate() const
{
   return S_OK;
}

void CCurvelReportSpecification::SetVerticalCurveParameters(Float64 g1,Float64 g2,Float64 PVIStation,Float64 PVIElevation,Float64 length)
{
   CCurvelDoc* pDoc = (CCurvelDoc*)EAFGetDocument();
   Curvel* pCurvel = pDoc->GetCurvelXML();

   VerticalCurveDataType& vc = pCurvel->VerticalCurveData();
   vc.g1(g1);
   vc.g2(g2);
   vc.PVIStation(PVIStation);
   vc.PVIElevation(PVIElevation);
   vc.Length(length);
}

void CCurvelReportSpecification::GetVerticalCurveParameters(Float64* g1,Float64* g2,Float64* PVIStation,Float64* PVIElevation,Float64* length) const
{
   CCurvelDoc* pDoc = (CCurvelDoc*)EAFGetDocument();
   Curvel* pCurvel = pDoc->GetCurvelXML();

   VerticalCurveDataType& vc = pCurvel->VerticalCurveData();
   *g1           = vc.g1();
   *g2           = vc.g2();
   *PVIStation   = vc.PVIStation();
   *PVIElevation = vc.PVIElevation();
   *length       = vc.Length();
}

bool CCurvelReportSpecification::CorrectForSuperelevation()
{
   CCurvelDoc* pDoc = (CCurvelDoc*)EAFGetDocument();
   Curvel* pCurvel = pDoc->GetCurvelXML();

   return pCurvel->SuperelevationData().present();
}

void CCurvelReportSpecification::CorrectForSuperelevation(bool bCorrect)
{
   CCurvelDoc* pDoc = (CCurvelDoc*)EAFGetDocument();
   Curvel* pCurvel = pDoc->GetCurvelXML();

   if ( pCurvel->SuperelevationData().present() && !bCorrect )
   {
      // we have superelevatoin data and no longer want it
      pCurvel->SuperelevationData().reset();
   }
   else if ( !pCurvel->SuperelevationData().present() && bCorrect )
   {
      // we don't have superelevation data but need it
      pCurvel->SuperelevationData(Curvel::SuperelevationData_type(0.0,CrownSlopeType()));
   }
}

void CCurvelReportSpecification::SetProfileGradeOffset(Float64 offset)
{
   CCurvelDoc* pDoc = (CCurvelDoc*)EAFGetDocument();
   Curvel* pCurvel = pDoc->GetCurvelXML();

   Curvel::SuperelevationData_optional& super = pCurvel->SuperelevationData();
   ATLASSERT(super.present());
   super->ProfileGradeOffset(offset);
}

Float64 CCurvelReportSpecification::GetProfileGradeOffset()
{
   CCurvelDoc* pDoc = (CCurvelDoc*)EAFGetDocument();
   Curvel* pCurvel = pDoc->GetCurvelXML();

   Curvel::SuperelevationData_optional& super = pCurvel->SuperelevationData();
   if ( super.present() )
      return super->ProfileGradeOffset();
   else
      return 0.0;
}

SuperelevationProfilePoint CCurvelReportSpecification::GetSuperelevationPoint(IndexType idx)
{
   CCurvelDoc* pDoc = (CCurvelDoc*)EAFGetDocument();
   Curvel* pCurvel = pDoc->GetCurvelXML();

   Curvel::SuperelevationData_optional& super = pCurvel->SuperelevationData();
   SuperelevationProfilePoint p;
   if ( super.present() )
   {
      CrownSlopeType& crownSlope = super->CrownSlope();
      CrownSlopeType::SuperelevationProfilePoint_sequence& superPP = crownSlope.SuperelevationProfilePoint();

      ATLASSERT(superPP.size() == 3);
      ATLASSERT(0 <= idx && idx <= 2);
      p.Station    = superPP[idx].Station();
      p.LeftSlope  = superPP[idx].LeftSlope();
      p.RightSlope = superPP[idx].RightSlope();
   }
   else
   {
      p.Station    = idx*::ConvertToSysUnits(50.0,unitMeasure::Feet);
      p.LeftSlope  = -0.02;
      p.RightSlope = -0.02;
   }

   return p;
}

void CCurvelReportSpecification::SetSuperelevationPoint(IndexType idx,SuperelevationProfilePoint& point)
{
   CCurvelDoc* pDoc = (CCurvelDoc*)EAFGetDocument();
   Curvel* pCurvel = pDoc->GetCurvelXML();

   Curvel::SuperelevationData_optional& super = pCurvel->SuperelevationData();
   ATLASSERT(super.present());
   CrownSlopeType& crownSlope = super->CrownSlope();
   CrownSlopeType::SuperelevationProfilePoint_sequence& superPP = crownSlope.SuperelevationProfilePoint();

   if ( superPP.size() < 3 )
      superPP.resize(3,SuperelevationProfilePointType(0,0,0));

   ATLASSERT(superPP.size() == 3);
   ATLASSERT(0 <= idx && idx <= 2);
   superPP[idx].Station(    point.Station    );
   superPP[idx].LeftSlope(  point.LeftSlope  );
   superPP[idx].RightSlope( point.RightSlope );
}

const std::vector<IndividualStation>& CCurvelReportSpecification::GetIndividualStations() const
{
   CCurvelDoc* pDoc = (CCurvelDoc*)EAFGetDocument();
   Curvel* pCurvel = pDoc->GetCurvelXML();

   m_IndividualStations.clear();

   Curvel::IndividualStations_optional& individualStationsOptional(pCurvel->IndividualStations());
   if ( individualStationsOptional.present() )
   {
      IndividualStationsType::IndividualStation_sequence& individualStations(individualStationsOptional->IndividualStation());
      IndividualStationsType::IndividualStation_iterator iter(individualStations.begin());
      IndividualStationsType::IndividualStation_iterator end(individualStations.end());
      for ( ; iter != end; iter++ )
      {
         IndividualStationType& individualStation(*iter);

         IndividualStation i;
         i.Station = individualStation.Station();
         i.Offset  = individualStation.Offset();
         m_IndividualStations.push_back(i);
      }
   }

   return m_IndividualStations;
}

void CCurvelReportSpecification::SetIndividualStations(const std::vector<IndividualStation>& stations)
{
   CCurvelDoc* pDoc = (CCurvelDoc*)EAFGetDocument();
   Curvel* pCurvel = pDoc->GetCurvelXML();

   Curvel::IndividualStations_optional& individualStationsOptional(pCurvel->IndividualStations());
   if ( stations.size() == 0 )
   {
      individualStationsOptional.reset();
   }
   else
   {
      if ( !individualStationsOptional.present() )
      {
         pCurvel->IndividualStations(IndividualStationsType());
         ATLASSERT(individualStationsOptional.present() == true);
      }

      IndividualStationsType::IndividualStation_sequence& individualStations(individualStationsOptional->IndividualStation());
      individualStations.clear();
      std::vector<IndividualStation>::const_iterator iter(stations.begin());
      std::vector<IndividualStation>::const_iterator end(stations.end());
      for ( ; iter != end; iter++ )
      {
         const IndividualStation& i(*iter);
         individualStations.push_back(IndividualStationType(i.Station,i.Offset));
      }
   }
}

const std::vector<StationRange>& CCurvelReportSpecification::GetStationRanges() const
{
   CCurvelDoc* pDoc = (CCurvelDoc*)EAFGetDocument();
   Curvel* pCurvel = pDoc->GetCurvelXML();

   m_StationRanges.clear();

   Curvel::StationRanges_optional& stationRangesOptional(pCurvel->StationRanges());
   if ( stationRangesOptional.present() )
   {
      StationRangesType::StationRange_sequence& stationRanges(stationRangesOptional->StationRange());
      StationRangesType::StationRange_iterator iter(stationRanges.begin());
      StationRangesType::StationRange_iterator end(stationRanges.end());
      for ( ; iter != end; iter++ )
      {
         StationRangeType& stationRange(*iter);

         StationRange sr;
         sr.StartStation = stationRange.StartStation();
         sr.EndStation   = stationRange.EndStation();
         sr.nSpaces      = (IndexType)stationRange.NumSpaces();
         sr.Offset       = stationRange.Offset();
         m_StationRanges.push_back(sr);
      }
   }

   return m_StationRanges;
}

void CCurvelReportSpecification::SetStationRanges(const std::vector<StationRange>& stations)
{
   CCurvelDoc* pDoc = (CCurvelDoc*)EAFGetDocument();
   Curvel* pCurvel = pDoc->GetCurvelXML();

   Curvel::StationRanges_optional& stationRangesOptional(pCurvel->StationRanges());
   if ( stations.size() == 0 )
   {
      stationRangesOptional.reset();
   }
   else
   {
      if ( !stationRangesOptional.present() )
      {
         pCurvel->StationRanges(StationRangesType());
         ATLASSERT(stationRangesOptional.present() == true);
      }

      StationRangesType::StationRange_sequence& stationRanges(stationRangesOptional->StationRange());
      stationRanges.clear();
      std::vector<StationRange>::const_iterator iter(stations.begin());
      std::vector<StationRange>::const_iterator end(stations.end());
      for ( ; iter != end; iter++ )
      {
         const StationRange& i(*iter);
         stationRanges.push_back(StationRangeType(i.StartStation,i.EndStation,i.nSpaces,i.Offset));
      }
   }
}

const std::vector<SkewLine>& CCurvelReportSpecification::GetSkewLines() const
{
   CCurvelDoc* pDoc = (CCurvelDoc*)EAFGetDocument();
   Curvel* pCurvel = pDoc->GetCurvelXML();

   m_SkewLines.clear();

   Curvel::SkewLines_optional& skewLinesOptional(pCurvel->SkewLines());
   if ( skewLinesOptional.present() )
   {
      SkewLinesType::SkewLine_sequence& skewLines(skewLinesOptional->SkewLine());
      SkewLinesType::SkewLine_iterator iter(skewLines.begin());
      SkewLinesType::SkewLine_iterator end(skewLines.end());
      for ( ; iter != end; iter++ )
      {
         SkewLineType& skewLine(*iter);

         SkewLine sl;
         sl.Station = skewLine.Station();
         sl.OffsetType = (SkewLine::Type)(OffsetType::value)skewLine.OffsetType();
         sl.Offset = skewLine.Offset();
         sl.strSkewAngle = skewLine.SkewAngle().c_str();
         sl.Radius = skewLine.Radius();
         sl.CrownOffset = skewLine.CrownOffset();
         m_SkewLines.push_back(sl);
      }
   }

   return m_SkewLines;
}

void CCurvelReportSpecification::SetSkewLines(const std::vector<SkewLine>& skewLines)
{
   CCurvelDoc* pDoc = (CCurvelDoc*)EAFGetDocument();
   Curvel* pCurvel = pDoc->GetCurvelXML();

   Curvel::SkewLines_optional& skewLinesOptional(pCurvel->SkewLines());
   if ( skewLines.size() == 0 )
   {
      skewLinesOptional.reset();
   }
   else
   {
      if ( !skewLinesOptional.present() )
      {
         pCurvel->SkewLines(SkewLinesType());
         ATLASSERT(skewLinesOptional.present() == true);
      }

      SkewLinesType::SkewLine_sequence& skewLinesContainer(skewLinesOptional->SkewLine());
      skewLinesContainer.clear();
      std::vector<SkewLine>::const_iterator iter(skewLines.begin());
      std::vector<SkewLine>::const_iterator end(skewLines.end());
      for ( ; iter != end; iter++ )
      {
         const SkewLine& sl(*iter);
         skewLinesContainer.push_back(SkewLineType(sl.Station,(OffsetType::value)sl.OffsetType,sl.Offset,sl.strSkewAngle.c_str(),sl.Radius,sl.CrownOffset));
      }
   }
}
