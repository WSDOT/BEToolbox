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

#include "StdAfx.h"
#include "CurvelReportSpecification.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCurvelReportSpecification::CCurvelReportSpecification(LPCTSTR strReportName) :
CReportSpecification(strReportName)
{
   m_g1 = 0;
   m_g2 = 0;
   m_PVIStation = 0;
   m_PVIElevation = 0;
   m_Length = 0;

   m_bCorrectForSuperelevation = false;
   m_ProfileGradeOffset = 0;

   for ( IndexType i = 0; i < 3; i++ )
   {
      m_SuperelevationPoint[i].Station    = 0;
      m_SuperelevationPoint[i].LeftSlope  = 0;
      m_SuperelevationPoint[i].RightSlope = 0;
   }
}

CCurvelReportSpecification::~CCurvelReportSpecification(void)
{
}

HRESULT CCurvelReportSpecification::Save(IStructuredSave* pStrSave)
{
   HRESULT hr = pStrSave->BeginUnit(_T("Curvel"),1.0);
   if ( FAILED(hr) )
      return hr;

   hr = pStrSave->BeginUnit(_T("VerticalCurveData"),1.0);
   hr = pStrSave->put_Property(_T("g1"),CComVariant(m_g1));
   hr = pStrSave->put_Property(_T("g2"),CComVariant(m_g2));
   hr = pStrSave->put_Property(_T("PVIStation"),CComVariant(m_PVIStation));
   hr = pStrSave->put_Property(_T("PVIElevation"),CComVariant(m_PVIElevation));
   hr = pStrSave->put_Property(_T("Length"),CComVariant(m_Length));
   hr = pStrSave->EndUnit(); // VerticalCurveData

   hr = pStrSave->BeginUnit(_T("SuperelevationData"),1.0);
   hr = pStrSave->put_Property(_T("CorrectForSuperelevation"),CComVariant(m_bCorrectForSuperelevation));
   if (m_bCorrectForSuperelevation)
   {
      hr = pStrSave->put_Property(_T("ProfileGradeOffset"),CComVariant(m_ProfileGradeOffset));
      hr = pStrSave->BeginUnit(_T("SuperelevationProfilePoint"),1.0);
      for ( int i = 0; i < 3; i++ )
      {
         hr = pStrSave->put_Property(_T("Station"),CComVariant(m_SuperelevationPoint[i].Station));
         hr = pStrSave->put_Property(_T("LeftSlope"),CComVariant(m_SuperelevationPoint[i].LeftSlope));
         hr = pStrSave->put_Property(_T("RightSlope"),CComVariant(m_SuperelevationPoint[i].RightSlope));
      }
      hr = pStrSave->EndUnit(); // SuperelevationProfilePoint
   }
   hr = pStrSave->EndUnit(); // SuperelevationData

   hr = pStrSave->BeginUnit(_T("IndividualStations"),1.0);
   hr = pStrSave->put_Property(_T("NumPoints"),CComVariant(m_IndividualStations.size()));
   std::vector<IndividualStation>::iterator isIter(m_IndividualStations.begin());
   std::vector<IndividualStation>::iterator isIterEnd(m_IndividualStations.end());
   for ( ; isIter != isIterEnd; isIter++ )
   {
      IndividualStation& is = *isIter;
      hr = pStrSave->BeginUnit(_T("IndividualStation"),1.0);
      hr = pStrSave->put_Property(_T("Station"),CComVariant(is.Station));
      hr = pStrSave->put_Property(_T("Offset"),CComVariant(is.Offset));
      hr = pStrSave->EndUnit(); // IndividualStation
   }
   hr = pStrSave->EndUnit(); // IndividualStations

   hr = pStrSave->BeginUnit(_T("StationRanges"),1.0);
   hr = pStrSave->put_Property(_T("NumPoints"),CComVariant(m_StationRanges.size()));
   std::vector<StationRange>::iterator srIter(m_StationRanges.begin());
   std::vector<StationRange>::iterator srIterEnd(m_StationRanges.end());
   for ( ; srIter != srIterEnd; srIter++ )
   {
      StationRange& sr = *srIter;
      hr = pStrSave->BeginUnit(_T("StationRange"),1.0);
      hr = pStrSave->put_Property(_T("StartStation"),CComVariant(sr.StartStation));
      hr = pStrSave->put_Property(_T("EndStation"),CComVariant(sr.EndStation));
      hr = pStrSave->put_Property(_T("NumSpaces"),CComVariant(sr.nSpaces));
      hr = pStrSave->put_Property(_T("Offset"),CComVariant(sr.Offset));
      hr = pStrSave->EndUnit(); // StationRange
   }
   hr = pStrSave->EndUnit(); // StationRanges

   hr = pStrSave->BeginUnit(_T("SkewLines"),1.0);
   hr = pStrSave->put_Property(_T("NumPoints"),CComVariant(m_SkewLines.size()));
   std::vector<SkewLine>::iterator slIter(m_SkewLines.begin());
   std::vector<SkewLine>::iterator slIterEnd(m_SkewLines.end());
   for ( ; slIter != slIterEnd; slIter++ )
   {
      SkewLine& sl = *slIter;
      hr = pStrSave->BeginUnit(_T("SkewLine"),1.0);
      hr = pStrSave->put_Property(_T("Station"),CComVariant(sl.Station));
      hr = pStrSave->put_Property(_T("OffsetType"),CComVariant(sl.OffsetType));
      hr = pStrSave->put_Property(_T("Offset"),CComVariant(sl.Offset));
      hr = pStrSave->put_Property(_T("SkewAngle"),CComVariant(sl.strSkewAngle.c_str()));
      hr = pStrSave->put_Property(_T("Radius"),CComVariant(sl.Radius));
      hr = pStrSave->put_Property(_T("CrownOffset"),CComVariant(sl.CrownOffset));
      hr = pStrSave->EndUnit(); // SkewLine
   }
   hr = pStrSave->EndUnit(); // SkewLines

   hr = pStrSave->EndUnit(); // Curvel
   if ( FAILED(hr) )
      return hr;

   return S_OK;
}

HRESULT CCurvelReportSpecification::Load(IStructuredLoad* pStrLoad)
{
   HRESULT hr = pStrLoad->BeginUnit(_T("Curvel"));
   if ( FAILED(hr) )
      return hr;

   CComVariant var;

   hr = pStrLoad->BeginUnit(_T("VerticalCurveData"));
   var.vt = VT_R8;
   hr = pStrLoad->get_Property(_T("g1"),&var);
   m_g1 = var.dblVal;

   hr = pStrLoad->get_Property(_T("g2"),&var);
   m_g2 = var.dblVal;

   hr = pStrLoad->get_Property(_T("PVIStation"),&var);
   m_PVIStation = var.dblVal;

   hr = pStrLoad->get_Property(_T("PVIElevation"),&var);
   m_PVIElevation = var.dblVal;

   hr = pStrLoad->get_Property(_T("Length"),&var);
   m_Length = var.dblVal;
   hr = pStrLoad->EndUnit(); // VerticalCurveData

   hr = pStrLoad->BeginUnit(_T("SuperelevationData"));
   var.vt = VT_BOOL;
   hr = pStrLoad->get_Property(_T("CorrectForSuperelevation"),&var);
   m_bCorrectForSuperelevation = (var.boolVal == VARIANT_TRUE ? true : false);
   if (m_bCorrectForSuperelevation)
   {
      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("ProfileGradeOffset"),&var);
      m_ProfileGradeOffset = var.dblVal;

      hr = pStrLoad->BeginUnit(_T("SuperelevationProfilePoint"));
      for ( int i = 0; i < 3; i++ )
      {
         hr = pStrLoad->get_Property(_T("Station"),&var);
         m_SuperelevationPoint[i].Station = var.dblVal;

         hr = pStrLoad->get_Property(_T("LeftSlope"),&var);
         m_SuperelevationPoint[i].LeftSlope = var.dblVal;

         hr = pStrLoad->get_Property(_T("RightSlope"),&var);
         m_SuperelevationPoint[i].RightSlope = var.dblVal;
      }
      hr = pStrLoad->EndUnit(); // SuperelevationProfilePoint
   }
   hr = pStrLoad->EndUnit(); // SuperelevationData

   hr = pStrLoad->BeginUnit(_T("IndividualStations"));
   var.vt = VT_INDEX;
   hr = pStrLoad->get_Property(_T("NumPoints"),&var);
   IndexType nPoints = VARIANT2INDEX(var);
   m_IndividualStations.clear();
   for ( IndexType i = 0; i < nPoints; i++ )
   {
      IndividualStation is;
      hr = pStrLoad->BeginUnit(_T("IndividualStation"));
      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("Station"),&var);
      is.Station = var.dblVal;

      hr = pStrLoad->get_Property(_T("Offset"),&var);
      is.Offset = var.dblVal;

      m_IndividualStations.push_back(is);

      hr = pStrLoad->EndUnit(); // IndividualStation
   }
   hr = pStrLoad->EndUnit(); // IndividualStations

   hr = pStrLoad->BeginUnit(_T("StationRanges"));
   var.vt = VT_INDEX;
   hr = pStrLoad->get_Property(_T("NumPoints"),&var);
   nPoints = VARIANT2INDEX(var);
   m_StationRanges.clear();
   for ( IndexType i = 0; i < nPoints; i++ )
   {
      StationRange sr;
      hr = pStrLoad->BeginUnit(_T("StationRange"));
      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("StartStation"),&var);
      sr.StartStation = var.dblVal;

      hr = pStrLoad->get_Property(_T("EndStation"),&var);
      sr.EndStation = var.dblVal;

      var.vt = VT_INDEX;
      hr = pStrLoad->get_Property(_T("NumSpaces"),&var);
      sr.nSpaces = VARIANT2INDEX(var);

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("Offset"),&var);
      sr.Offset = var.dblVal;

      m_StationRanges.push_back(sr);

      hr = pStrLoad->EndUnit(); // StationRange
   }
   hr = pStrLoad->EndUnit(); // StationRanges

   hr = pStrLoad->BeginUnit(_T("SkewLines"));
   var.vt = VT_INDEX;
   hr = pStrLoad->get_Property(_T("NumPoints"),&var);
   nPoints = VARIANT2INDEX(var);
   m_SkewLines.clear();
   for ( IndexType i = 0; i < nPoints; i++ )
   {
      SkewLine sl;
      hr = pStrLoad->BeginUnit(_T("SkewLine"));
      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("Station"),&var);
      sl.Station = var.dblVal;

      var.vt = VT_I4;
      hr = pStrLoad->get_Property(_T("OffsetType"),&var);
      sl.OffsetType = (SkewLine::Type)(var.lVal);

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("Offset"),&var);
      sl.Offset = var.dblVal;

      var.vt = VT_BSTR;
      hr = pStrLoad->get_Property(_T("SkewAngle"),&var);
      sl.strSkewAngle = OLE2T(var.bstrVal);

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("Radius"),&var);
      sl.Radius = var.dblVal;

      hr = pStrLoad->get_Property(_T("CrownOffset"),&var);
      sl.CrownOffset = var.dblVal;

      m_SkewLines.push_back(sl);

      hr = pStrLoad->EndUnit(); // SkewLine
   }
   hr = pStrLoad->EndUnit(); // SkewLines


   hr = pStrLoad->EndUnit(); // Curvel
   if ( FAILED(hr) )
      return hr;

   return S_OK;
}

HRESULT CCurvelReportSpecification::Validate() const
{
   return S_OK;
}

void CCurvelReportSpecification::SetVerticalCurveParameters(Float64 g1,Float64 g2,Float64 PVIStation,Float64 PVIElevation,Float64 length)
{
   m_g1 = g1;
   m_g2 = g2;
   m_PVIStation = PVIStation;
   m_PVIElevation = PVIElevation;
   m_Length = length;
}

void CCurvelReportSpecification::GetVerticalCurveParameters(Float64* g1,Float64* g2,Float64* PVIStation,Float64* PVIElevation,Float64* length) const
{
   *g1 = m_g1;
   *g2 = m_g2;
   *PVIStation = m_PVIStation;
   *PVIElevation = m_PVIElevation;
   *length = m_Length;
}

bool CCurvelReportSpecification::CorrectForSuperelevation()
{
   return m_bCorrectForSuperelevation;
}

void CCurvelReportSpecification::CorrectForSuperelevation(bool bCorrect)
{
   m_bCorrectForSuperelevation = bCorrect;
}

void CCurvelReportSpecification::SetProfileGradeOffset(Float64 offset)
{
   m_ProfileGradeOffset = offset;
}

Float64 CCurvelReportSpecification::GetProfileGradeOffset()
{
   return m_ProfileGradeOffset;
}

SuperelevationProfilePoint CCurvelReportSpecification::GetSuperelevationPoint(IndexType idx)
{
   ATLASSERT(0 <= idx && idx <= 2);
   return m_SuperelevationPoint[idx];
}

void CCurvelReportSpecification::SetSuperelevationPoint(IndexType idx,SuperelevationProfilePoint& point)
{
   ATLASSERT(0 <= idx && idx <= 2);
   m_SuperelevationPoint[idx] = point;
}

const std::vector<IndividualStation>& CCurvelReportSpecification::GetIndividualStations() const
{
   return m_IndividualStations;
}

void CCurvelReportSpecification::SetIndividualStations(const std::vector<IndividualStation>& stations)
{
   m_IndividualStations = stations;
}

const std::vector<StationRange>& CCurvelReportSpecification::GetStationRanges() const
{
   return m_StationRanges;
}

void CCurvelReportSpecification::SetStationRanges(const std::vector<StationRange>& stations)
{
   m_StationRanges = stations;
}

const std::vector<SkewLine>& CCurvelReportSpecification::GetSkewLines() const
{
   return m_SkewLines;
}

void CCurvelReportSpecification::SetSkewLines(const std::vector<SkewLine>& skewLines)
{
   m_SkewLines = skewLines;
}
