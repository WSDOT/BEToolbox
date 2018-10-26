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

// PGStableStrands.cpp : implementation file
//

#include "stdafx.h"
#include "PGStableStrands.h"

CPGStableStrands::CPGStableStrands()
{
   XferLength = ::ConvertToSysUnits(36.0,unitMeasure::Inch);

   Ys = ::ConvertToSysUnits(3.0,unitMeasure::Inch);
   YsMeasure = BOTTOM;

   Yt = ::ConvertToSysUnits(2.0,unitMeasure::Inch);
   YtMeasure = TOP;

   Xh1 = 0.0;
   Xh1Measure = FRACTION;

   Yh1 = ::ConvertToSysUnits(4.0,unitMeasure::Inch);
   Yh1Measure = TOP;

   Xh2 = 0.4;
   Xh2Measure = FRACTION;
   
   Yh2 = ::ConvertToSysUnits(5.0,unitMeasure::Inch);
   Yh2Measure = BOTTOM;

   Xh3 = 0.6;
   Xh3Measure = FRACTION;
   
   Yh3 = ::ConvertToSysUnits(5.0,unitMeasure::Inch);
   Yh3Measure = BOTTOM;

   Xh4 = 1.0;
   Xh4Measure = FRACTION;

   Yh4 = ::ConvertToSysUnits(4.0,unitMeasure::Inch);
   Yh4Measure = TOP;
}

bool CPGStableStrands::operator==(const CPGStableStrands& other) const
{
   if ( !IsEqual(XferLength,other.XferLength) )
      return false;

   if ( !IsEqual(Ys,other.Ys) || YsMeasure != other.YsMeasure )
      return false;

   if ( !IsEqual(Yt,other.Yt) || YtMeasure != other.YtMeasure )
      return false;

   if ( !IsEqual(Xh1,other.Xh1) || Xh1Measure != other.Xh1Measure )
      return false;

   if ( !IsEqual(Yh1,other.Yh1) || Yh1Measure != other.Yh1Measure )
      return false;

   if ( !IsEqual(Xh2,other.Xh2) || Xh2Measure != other.Xh2Measure )
      return false;

   if ( !IsEqual(Yh2,other.Yh2) || Yh2Measure != other.Yh2Measure )
      return false;

   if ( !IsEqual(Xh3,other.Xh3) || Xh3Measure != other.Xh3Measure )
      return false;

   if ( !IsEqual(Yh3,other.Yh3) || Yh3Measure != other.Yh3Measure )
      return false;
 
   if ( !IsEqual(Xh4,other.Xh4) || Xh4Measure != other.Xh4Measure )
      return false;

   if ( !IsEqual(Yh4,other.Yh4) || Yh4Measure != other.Yh4Measure )
      return false;

   return true;
}

bool CPGStableStrands::operator!=(const CPGStableStrands& other) const
{
   return !(*this == other);
}

HRESULT CPGStableStrands::Save(IStructuredSave* pStrSave)
{
   pStrSave->BeginUnit(_T("Strands"),1.0);

   pStrSave->put_Property(_T("XferLength"),CComVariant(XferLength));

   // Straight Strands
   pStrSave->put_Property(_T("Ys"),CComVariant(Ys));
   pStrSave->put_Property(_T("YsMeasure"),CComVariant(YsMeasure));

   // Harped Strands
   pStrSave->put_Property(_T("Xh1"),CComVariant(Xh1));
   pStrSave->put_Property(_T("Xh1Measure"),CComVariant(Xh1Measure));
   pStrSave->put_Property(_T("Yh1"),CComVariant(Yh1));
   pStrSave->put_Property(_T("Yh1Measure"),CComVariant(Yh1Measure));

   pStrSave->put_Property(_T("Xh2"),CComVariant(Xh2));
   pStrSave->put_Property(_T("Xh2Measure"),CComVariant(Xh2Measure));
   pStrSave->put_Property(_T("Yh2"),CComVariant(Yh2));
   pStrSave->put_Property(_T("Yh2Measure"),CComVariant(Yh2Measure));

   pStrSave->put_Property(_T("Xh3"),CComVariant(Xh3));
   pStrSave->put_Property(_T("Xh3Measure"),CComVariant(Xh3Measure));
   pStrSave->put_Property(_T("Yh3"),CComVariant(Yh3));
   pStrSave->put_Property(_T("Yh3Measure"),CComVariant(Yh3Measure));

   pStrSave->put_Property(_T("Xh4"),CComVariant(Xh4));
   pStrSave->put_Property(_T("Xh4Measure"),CComVariant(Xh4Measure));
   pStrSave->put_Property(_T("Yh4"),CComVariant(Yh4));
   pStrSave->put_Property(_T("Yh4Measure"),CComVariant(Yh4Measure));

   // Temporary Strands
   pStrSave->put_Property(_T("Yt"),CComVariant(Yt));
   pStrSave->put_Property(_T("YtMeasure"),CComVariant(YtMeasure));

   pStrSave->EndUnit();
   return S_OK;
}

HRESULT CPGStableStrands::Load(IStructuredLoad* pStrLoad)
{
   CHRException hr;
   try
   {
      hr = pStrLoad->BeginUnit(_T("Strands"));

      CComVariant var;

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("XferLength"),&var);
      XferLength = var.dblVal;

      // Straight Strands
      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("Ys"),&var);
      Ys = var.dblVal;
      var.vt = VT_I4;
      hr = pStrLoad->get_Property(_T("YsMeasure"),&var);
      YsMeasure = var.lVal;

      // Harped Strands
      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("Xh1"),&var);
      Xh1 = var.dblVal;
      var.vt = VT_I4;
      hr = pStrLoad->get_Property(_T("Xh1Measure"),&var);
      Xh1Measure = var.lVal;
      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("Yh1"),&var);
      Yh1 = var.dblVal;
      var.vt = VT_I4;
      hr = pStrLoad->get_Property(_T("Yh1Measure"),&var);
      Yh1Measure = var.lVal;

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("Xh2"),&var);
      Xh2 = var.dblVal;
      var.vt = VT_I4;
      hr = pStrLoad->get_Property(_T("Xh2Measure"),&var);
      Xh2Measure = var.lVal;
      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("Yh2"),&var);
      Yh2 = var.dblVal;
      var.vt = VT_I4;
      hr = pStrLoad->get_Property(_T("Yh2Measure"),&var);
      Yh2Measure = var.lVal;

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("Xh3"),&var);
      Xh3 = var.dblVal;
      var.vt = VT_I4;
      hr = pStrLoad->get_Property(_T("Xh3Measure"),&var);
      Xh3Measure = var.lVal;
      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("Yh3"),&var);
      Yh3 = var.dblVal;
      var.vt = VT_I4;
      hr = pStrLoad->get_Property(_T("Yh3Measure"),&var);
      Yh3Measure = var.lVal;

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("Xh4"),&var);
      Xh4 = var.dblVal;
      var.vt = VT_I4;
      hr = pStrLoad->get_Property(_T("Xh4Measure"),&var);
      Xh4Measure = var.lVal;
      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("Yh4"),&var);
      Yh4 = var.dblVal;
      var.vt = VT_I4;
      hr = pStrLoad->get_Property(_T("Yh4Measure"),&var);
      Yh4Measure = var.lVal;

      // Temporary Strands
      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("Yt"),&var);
      Yt = var.dblVal;
      var.vt = VT_I4;
      hr = pStrLoad->get_Property(_T("YtMeasure"),&var);
      YtMeasure = var.lVal;

      hr = pStrLoad->EndUnit();
   }
   catch(...)
   {
      THROW_LOAD(InvalidFileFormat,pStrLoad);
   }

   return S_OK;
}
