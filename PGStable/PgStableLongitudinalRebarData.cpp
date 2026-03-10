///////////////////////////////////////////////////////////////////////
// PGSuper - Prestressed Girder SUPERstructure Design and Analysis
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
#include "PGStableLongitudinalRebarData.h"
#include <EAF\EAFApp.h>
#include <Units\Convert.h>
#include <StdIo.h>
#include <StrData.cpp>
#include <atlbase.h>
#include <algorithm>
#include <psgLib\GirderLibraryEntry.h>
#include <LRFD\RebarPool.h>

CPGStableLongitudinalRebarData::CPGStableLongitudinalRebarData() :
BarType(WBFL::Materials::Rebar::Type::A615),
BarGrade(WBFL::Materials::Rebar::Grade::Grade60)
{
}

bool CPGStableLongitudinalRebarData::operator == (const CPGStableLongitudinalRebarData& rOther) const
{
   if ( BarType != rOther.BarType )
   {
      return false;
   }

   if ( BarGrade != rOther.BarGrade )
   {
      return false;
   }

   if (!IsEqual(MaxCoverToUseHigherTensionStressLimit, rOther.MaxCoverToUseHigherTensionStressLimit))
   {
      return false;
   }

   return (RebarRows == rOther.RebarRows);
}

bool CPGStableLongitudinalRebarData::operator != (const CPGStableLongitudinalRebarData& rOther) const
{
   return !operator==( rOther );
}

HRESULT CPGStableLongitudinalRebarData::Load(IStructuredLoad* pStrLoad)
{
   USES_CONVERSION;

   HRESULT hr = S_OK;

   pStrLoad->BeginUnit(_T("PGStableLongitudinalRebar")); 
   Float64 version;
   pStrLoad->get_Version(&version);

   CComVariant var;

   var.vt = VT_I4;

   pStrLoad->get_Property(_T("BarGrade"), &var );
   BarGrade = WBFL::Materials::Rebar::Grade(var.lVal);

   pStrLoad->get_Property(_T("BarType"), &var );
   BarType = WBFL::Materials::Rebar::Type(var.lVal);

   var.Clear();
   var.vt = VT_R8;
   pStrLoad->get_Property(_T("MaxCoverToUseHigherTensionStressLimit"), &var);
   MaxCoverToUseHigherTensionStressLimit = var.dblVal;

   var.Clear();
   var.vt = VT_INDEX;
   pStrLoad->get_Property(_T("RebarRowCount"), &var );
   IndexType count = VARIANT2INDEX(var);

   RebarRows.clear();
   for ( IndexType row = 0; row < count; row++ )
   {
      pStrLoad->BeginUnit(_T("RebarRow"));

      Float64 bar_version;
      pStrLoad->get_Version(&bar_version);

      RebarRow rebar_row;

      var.vt = VT_I4;
      pStrLoad->get_Property(_T("Face"),         &var);
      rebar_row.Face = (pgsTypes::FaceType)(var.lVal);

      var.vt = VT_R8;
      pStrLoad->get_Property(_T("Cover"),        &var);
      rebar_row.Cover = var.dblVal;

      var.vt = VT_I4;
      pStrLoad->get_Property(_T("NumberOfBars"), &var);
      rebar_row.NumberOfBars = var.lVal;

      var.vt = VT_I4;
      pStrLoad->get_Property(_T("BarSize"), &var );
      rebar_row.BarSize = WBFL::Materials::Rebar::Size(var.lVal);

      var.vt = VT_R8;
      pStrLoad->get_Property(_T("BarSpacing"), &var);
      rebar_row.BarSpacing = var.dblVal;

      RebarRows.push_back(rebar_row);

      pStrLoad->EndUnit();
   }

   pStrLoad->EndUnit();

   return hr;
}

HRESULT CPGStableLongitudinalRebarData::Save(IStructuredSave* pStrSave)
{
   HRESULT hr = S_OK;

   pStrSave->BeginUnit(_T("PGStableLongitudinalRebar"),1.0);

   pStrSave->put_Property(_T("BarGrade"),     CComVariant(+BarGrade));
   pStrSave->put_Property(_T("BarType"),      CComVariant(+BarType));

   pStrSave->put_Property(_T("MaxCoverToUseHigherTensionStressLimit"), CComVariant(MaxCoverToUseHigherTensionStressLimit));

   IndexType count = RebarRows.size();
   pStrSave->put_Property(_T("RebarRowCount"),CComVariant(count));
   std::vector<RebarRow>::iterator iter;
   for ( iter = RebarRows.begin(); iter != RebarRows.end(); iter++ )
   {
      pStrSave->BeginUnit(_T("RebarRow"),1.0);
      const RebarRow& rebar_row = *iter;

      pStrSave->put_Property(_T("Face"),         CComVariant(rebar_row.Face));
      pStrSave->put_Property(_T("Cover"),        CComVariant(rebar_row.Cover));
      pStrSave->put_Property(_T("NumberOfBars"), CComVariant(rebar_row.NumberOfBars));
      pStrSave->put_Property(_T("BarSize"),      CComVariant(+rebar_row.BarSize));
      pStrSave->put_Property(_T("BarSpacing"),   CComVariant(rebar_row.BarSpacing));

      pStrSave->EndUnit();
   }


   pStrSave->EndUnit();

   return hr;
}

bool CPGStableLongitudinalRebarData::RebarRow::operator==(const RebarRow& other) const
{
   if (Face != other.Face) return false;
   if (BarSize != other.BarSize) return false;
   if (!IsEqual(Cover, other.Cover)) return false;
   if (!IsEqual(BarSpacing, other.BarSpacing)) return false;
   if (NumberOfBars != other.NumberOfBars) return false;

   return true;
};
