///////////////////////////////////////////////////////////////////////
// BEToolbox
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

// PGStableStrands.cpp : implementation file
//

#include "stdafx.h"
#include "PGStableStrands.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CPGStableFpe::CPGStableFpe()
{
   memset((void*)this,0,sizeof(CPGStableFpe));
}

CPGStableFpe::CPGStableFpe(Float64 X,Float64 FpeStraight,Float64 XpsStraight,Float64 YpsStraight,int YpsStraightMeasure,Float64 FpeHarped,Float64 XpsHarped,Float64 YpsHarped,int YpsHarpedMeasure,Float64 FpeTemp,Float64 XpsTemp,Float64 YpsTemp,int YpsTempMeasure) :
X(X),FpeStraight(FpeStraight),XpsStraight(XpsStraight),YpsStraight(YpsStraight),YpsStraightMeasure(YpsStraightMeasure),FpeHarped(FpeHarped),XpsHarped(XpsHarped),YpsHarped(YpsHarped),YpsHarpedMeasure(YpsHarpedMeasure),FpeTemp(FpeTemp),XpsTemp(XpsTemp),YpsTemp(YpsTemp),YpsTempMeasure(YpsTempMeasure)
{
}

bool CPGStableFpe::operator<(const CPGStableFpe& other) const
{ 
   return X < other.X; 
}

bool CPGStableFpe::operator==(const CPGStableFpe& other) const
{
   if ( !IsEqual(X,other.X) ||
        !IsEqual(FpeStraight,other.FpeStraight) ||
        !IsEqual(XpsStraight, other.XpsStraight) ||
        !IsEqual(YpsStraight, other.YpsStraight) ||
        YpsStraightMeasure != other.YpsStraightMeasure ||
        !IsEqual(FpeHarped,other.FpeHarped) ||
        !IsEqual(XpsHarped, other.XpsHarped) ||
        !IsEqual(YpsHarped, other.YpsHarped) ||
        YpsHarpedMeasure != YpsHarpedMeasure ||
        !IsEqual(FpeTemp,other.FpeTemp) ||
        !IsEqual(XpsTemp, other.XpsTemp) ||
        !IsEqual(YpsTemp, other.YpsTemp) ||
        YpsTempMeasure != other.YpsTempMeasure )
   {
      return false;
   }

   return true;
}

/////////////////////////////////////////////
CPGStableStrands::CPGStableStrands()
{
   strandMethod = Simplified;

   ex = 0;

   XferLength = WBFL::Units::ConvertToSysUnits(36.0,WBFL::Units::Measure::Inch);

   Ys = WBFL::Units::ConvertToSysUnits(3.0,WBFL::Units::Measure::Inch);
   YsMeasure = BOTTOM;

   Yt = WBFL::Units::ConvertToSysUnits(2.0,WBFL::Units::Measure::Inch);
   YtMeasure = TOP;

   Xh1 = 0.0;
   Xh1Measure = FRACTION;

   Yh1 = WBFL::Units::ConvertToSysUnits(4.0,WBFL::Units::Measure::Inch);
   Yh1Measure = TOP;

   Xh2 = 0.4;
   Xh2Measure = FRACTION;
   
   Yh2 = WBFL::Units::ConvertToSysUnits(5.0,WBFL::Units::Measure::Inch);
   Yh2Measure = BOTTOM;

   Xh3 = 0.6;
   Xh3Measure = FRACTION;
   
   Yh3 = WBFL::Units::ConvertToSysUnits(5.0,WBFL::Units::Measure::Inch);
   Yh3Measure = BOTTOM;

   Xh4 = 1.0;
   Xh4Measure = FRACTION;

   Yh4 = WBFL::Units::ConvertToSysUnits(4.0,WBFL::Units::Measure::Inch);
   Yh4Measure = TOP;

   FpeStraight = 0;
   FpeHarped   = 0;
   FpeTemp     = 0;
}

bool CPGStableStrands::operator==(const CPGStableStrands& other) const
{
   if ( strandMethod != other.strandMethod )
      return false;

   if (!IsEqual(ex, other.ex))
   {
      return false;
   }

   if ( strandMethod == CPGStableStrands::Simplified )
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

      if ( !IsEqual(FpeStraight,other.FpeStraight) )
         return false;

      if ( !IsEqual(FpeHarped,other.FpeHarped) )
         return false;

      if ( !IsEqual(FpeTemp,other.FpeTemp) )
         return false;
   }
   else
   {
      if (m_vFpe != other.m_vFpe)
      {
         return false;
      }
   }

   return true;
}

bool CPGStableStrands::operator!=(const CPGStableStrands& other) const
{
   return !(*this == other);
}

HRESULT CPGStableStrands::Save(IStructuredSave* pStrSave)
{
   pStrSave->BeginUnit(_T("Strands"),3.0);

   pStrSave->put_Property(_T("DefinitionMethod"),CComVariant(strandMethod));
   // pStrSave->put_Property(_T("ex"), CComVariant(ex)); // added in version 2, removed version 3

   if ( strandMethod == CPGStableStrands::Simplified )
   {
      pStrSave->put_Property(_T("XferLength"), CComVariant(XferLength));

      pStrSave->put_Property(_T("ex"), CComVariant(ex)); // added in version 3
                                                         
      // Straight Strands
      pStrSave->put_Property(_T("FpeStraight"),CComVariant(FpeStraight));
      pStrSave->put_Property(_T("Ys"),CComVariant(Ys));
      pStrSave->put_Property(_T("YsMeasure"),CComVariant(YsMeasure));

      // Harped Strands
      pStrSave->put_Property(_T("FpeHarped"),CComVariant(FpeHarped));
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
      pStrSave->put_Property(_T("FpeTemp"),CComVariant(FpeTemp));
      pStrSave->put_Property(_T("Yt"),CComVariant(Yt));
      pStrSave->put_Property(_T("YtMeasure"),CComVariant(YtMeasure));
   }
   else
   {
      for (const auto& fpe : m_vFpe)
      {
         pStrSave->BeginUnit(_T("Fpe"),1.0);
         pStrSave->put_Property(_T("X"),CComVariant(fpe.X));
         pStrSave->put_Property(_T("FpeStraight"),CComVariant(fpe.FpeStraight));
         pStrSave->put_Property(_T("XpsStraight"), CComVariant(fpe.XpsStraight)); // added in version 3
         pStrSave->put_Property(_T("YpsStraight"), CComVariant(fpe.YpsStraight));
         pStrSave->put_Property(_T("YpsStraightMeasure"),CComVariant(fpe.YpsStraightMeasure));

         pStrSave->put_Property(_T("FpeHarped"),CComVariant(fpe.FpeHarped));
         pStrSave->put_Property(_T("XpsHarped"), CComVariant(fpe.XpsHarped)); // added in version 3
         pStrSave->put_Property(_T("YpsHarped"), CComVariant(fpe.YpsHarped));
         pStrSave->put_Property(_T("YpsHarpedMeasure"),CComVariant(fpe.YpsHarpedMeasure));

         pStrSave->put_Property(_T("FpeTemp"),CComVariant(fpe.FpeTemp));
         pStrSave->put_Property(_T("XpsTemp"), CComVariant(fpe.XpsTemp)); // added in version 3
         pStrSave->put_Property(_T("YpsTemp"), CComVariant(fpe.YpsTemp));
         pStrSave->put_Property(_T("YpsTempMeasure"),CComVariant(fpe.YpsTempMeasure));

         pStrSave->EndUnit(); // Fpe
      }
   }

   pStrSave->EndUnit();
   return S_OK;
}

HRESULT CPGStableStrands::Load(IStructuredLoad* pStrLoad)
{
   CHRException hr;
   try
   {
      hr = pStrLoad->BeginUnit(_T("Strands"));
      Float64 version;
      pStrLoad->get_Version(&version);

      CComVariant var;

      var.vt = VT_I4;
      hr = pStrLoad->get_Property(_T("DefinitionMethod"),&var);
      strandMethod = (CPGStableStrands::StrandMethod)var.lVal;

      if (1 < version && version < 3)
      {
         // added in version 2, removed in version 3
         var.vt = VT_R8;
         hr = pStrLoad->get_Property(_T("ex"), &var);
         ex = var.dblVal;
      }

      
      if ( strandMethod == CPGStableStrands::Simplified )
      {
         var.vt = VT_R8;
         hr = pStrLoad->get_Property(_T("XferLength"),&var);
         XferLength = var.dblVal;

         if (2 < version)
         {
            // added in version 3
            var.vt = VT_R8;
            hr = pStrLoad->get_Property(_T("ex"), &var);
            ex = var.dblVal;
         }

         // Straight Strands
         var.vt = VT_R8;
         hr = pStrLoad->get_Property(_T("FpeStraight"),&var);
         FpeStraight = var.dblVal;
         hr = pStrLoad->get_Property(_T("Ys"),&var);
         Ys = var.dblVal;
         var.vt = VT_I4;
         hr = pStrLoad->get_Property(_T("YsMeasure"),&var);
         YsMeasure = var.lVal;

         // Harped Strands
         var.vt = VT_R8;
         hr = pStrLoad->get_Property(_T("FpeHarped"),&var);
         FpeHarped = var.dblVal;
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
         hr = pStrLoad->get_Property(_T("FpeTemp"),&var);
         FpeTemp = var.dblVal;
         hr = pStrLoad->get_Property(_T("Yt"),&var);
         Yt = var.dblVal;
         var.vt = VT_I4;
         hr = pStrLoad->get_Property(_T("YtMeasure"),&var);
         YtMeasure = var.lVal;
      }
      else
      {
         m_vFpe.clear();
         while ( SUCCEEDED(pStrLoad->BeginUnit(_T("Fpe"))) )
         {
            CPGStableFpe fpe;
            var.vt = VT_R8;
            hr = pStrLoad->get_Property(_T("X"),&var);
            fpe.X = var.dblVal;

            hr = pStrLoad->get_Property(_T("FpeStraight"),&var);
            fpe.FpeStraight = var.dblVal;

            if (2 < version)
            {
               // added in version 3
               hr = pStrLoad->get_Property(_T("XpsStraight"), &var);
               fpe.XpsStraight = var.dblVal;
            }

            hr = pStrLoad->get_Property(_T("YpsStraight"),&var);
            fpe.YpsStraight = var.dblVal;

            var.vt = VT_I4;
            hr = pStrLoad->get_Property(_T("YpsStraightMeasure"),&var);
            fpe.YpsStraightMeasure = var.lVal;

            var.vt = VT_R8;
            hr = pStrLoad->get_Property(_T("FpeHarped"),&var);
            fpe.FpeHarped = var.dblVal;

            if (2 < version)
            {
               // added in version 3
               hr = pStrLoad->get_Property(_T("XpsHarped"), &var);
               fpe.XpsHarped = var.dblVal;
            }

            hr = pStrLoad->get_Property(_T("YpsHarped"),&var);
            fpe.YpsHarped = var.dblVal;

            var.vt = VT_I4;
            hr = pStrLoad->get_Property(_T("YpsHarpedMeasure"),&var);
            fpe.YpsHarpedMeasure = var.lVal;

            var.vt = VT_R8;
            hr = pStrLoad->get_Property(_T("FpeTemp"),&var);
            fpe.FpeTemp = var.dblVal;

            if (2 < version)
            {
               // added in version 3
               hr = pStrLoad->get_Property(_T("XpsTemp"), &var);
               fpe.XpsTemp = var.dblVal;
            }

            hr = pStrLoad->get_Property(_T("YpsTemp"),&var);
            fpe.YpsTemp = var.dblVal;

            var.vt = VT_I4;
            hr = pStrLoad->get_Property(_T("YpsTempMeasure"),&var);
            fpe.YpsTempMeasure = var.lVal;

            m_vFpe.insert(fpe);

            hr = pStrLoad->EndUnit(); // Fpe
         }
      }

      hr = pStrLoad->EndUnit();
   }
   catch(...)
   {
      THROW_LOAD(InvalidFileFormat,pStrLoad);
   }

   return S_OK;
}
