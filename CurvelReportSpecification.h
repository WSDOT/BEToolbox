///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2015  Washington State Department of Transportation
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

#pragma once
#include <ReportManager\ReportSpecification.h>

class CCurvelReportSpecification :
   public CReportSpecification
{
public:
	CCurvelReportSpecification(LPCTSTR strReportName);
   ~CCurvelReportSpecification(void);

   HRESULT Save(IStructuredSave* pStrSave);
   HRESULT Load(IStructuredLoad* pStrLoad);

   void SetVerticalCurveParameters(Float64 g1,Float64 g2,Float64 PVIStation,Float64 PVIElevation,Float64 length);
   void GetVerticalCurveParameters(Float64* g1,Float64* g2,Float64* PVIStation,Float64* PVIElevation,Float64* length) const;

   bool CorrectForSuperelevation();
   void CorrectForSuperelevation(bool bCorrect);

   void SetProfileGradeOffset(Float64 offset);
   Float64 GetProfileGradeOffset();

   SuperelevationProfilePoint GetSuperelevationPoint(IndexType idx);
   void SetSuperelevationPoint(IndexType idx,SuperelevationProfilePoint& point);

   const std::vector<IndividualStation>& GetIndividualStations() const;
   void SetIndividualStations(const std::vector<IndividualStation>& stations);

   const std::vector<StationRange>& GetStationRanges() const;
   void SetStationRanges(const std::vector<StationRange>& stations);

   const std::vector<SkewLine>& GetSkewLines() const;
   void SetSkewLines(const std::vector<SkewLine>& skewLines);

   virtual HRESULT Validate() const;

protected:
   Float64 m_g1;
   Float64 m_g2;
   Float64 m_PVIStation;
   Float64 m_PVIElevation;
   Float64 m_Length;

   bool m_bCorrectForSuperelevation;
   Float64 m_ProfileGradeOffset;
   SuperelevationProfilePoint m_SuperelevationPoint[3];

   std::vector<IndividualStation> m_IndividualStations;
   std::vector<StationRange> m_StationRanges;
   std::vector<SkewLine> m_SkewLines;
};
