///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2017  Washington State Department of Transportation
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

// PGStableHaulingCriteria.cpp : implementation file
//

#include "stdafx.h"
#include "PGStableHaulingCriteria.h"

CPGStableHaulingCriteria::CPGStableHaulingCriteria()
{
   MinFScr = 1.0;
   MinFSf = 1.5;
   CompressionCoefficient = (lrfdVersionMgr::GetVersion() < lrfdVersionMgr::SeventhEditionWith2016Interims ? 0.60 : 0.65);
   TensionCoefficient[stbTypes::CrownSlope] = ::ConvertToSysUnits(0.0948,unitMeasure::SqrtKSI);
   bMaxTension[stbTypes::CrownSlope] = false;
   MaxTension[stbTypes::CrownSlope] = ::ConvertToSysUnits(0.2,unitMeasure::KSI);
   TensionCoefficientWithRebar[stbTypes::CrownSlope] = ::ConvertToSysUnits(0.1900,unitMeasure::SqrtKSI);

   TensionCoefficient[stbTypes::MaxSuper] = ::ConvertToSysUnits(0.0948,unitMeasure::SqrtKSI);
   bMaxTension[stbTypes::MaxSuper] = false;
   MaxTension[stbTypes::MaxSuper] = ::ConvertToSysUnits(0.2,unitMeasure::KSI);
   TensionCoefficientWithRebar[stbTypes::MaxSuper] = ::ConvertToSysUnits(0.1900,unitMeasure::SqrtKSI);

   MaxClearSpan = ::ConvertToSysUnits(230.0,unitMeasure::Feet);
   MaxLeadingOverhang = ::ConvertToSysUnits(15,unitMeasure::Feet);
   MaxGirderWeight = ::ConvertToSysUnits(200.0,unitMeasure::Kip);
}

CPGStableHaulingCriteria::~CPGStableHaulingCriteria()
{
}

bool CPGStableHaulingCriteria::operator==(const CPGStableHaulingCriteria& other) const
{
   if ( !IsEqual(MinFScr,other.MinFScr) )
      return false;
   
   if ( !IsEqual(MinFSf,other.MinFSf) )
      return false;
   
   if ( !IsEqual(CompressionCoefficient,other.CompressionCoefficient) )
      return false;
   
   for ( int s = 0; s < 2; s++ )
   {
      stbTypes::HaulingSlope slope = (stbTypes::HaulingSlope)s;

      if ( !IsEqual(TensionCoefficient[slope],other.TensionCoefficient[slope]) )
         return false;

      if ( bMaxTension[slope] != other.bMaxTension[slope] )
         return false;
      
      if ( !IsEqual(MaxTension[slope],other.MaxTension[slope]) )
         return false;
      
      if ( !IsEqual(TensionCoefficientWithRebar[slope],other.TensionCoefficientWithRebar[slope]) )
         return false;
   }

   if ( !IsEqual(MaxClearSpan,other.MaxClearSpan) )
      return false;
   
   if ( !IsEqual(MaxLeadingOverhang,other.MaxLeadingOverhang) )
      return false;

   if ( !IsEqual(MaxGirderWeight,other.MaxGirderWeight) )
      return false;

   return true;
}

bool CPGStableHaulingCriteria::operator!=(const CPGStableHaulingCriteria& other) const
{
   return !(*this == other);
}

void CPGStableHaulingCriteria::operator=(const stbHaulingCriteria& other)
{
   *((stbHaulingCriteria*)this) = other;
}

HRESULT CPGStableHaulingCriteria::Save(IStructuredSave* pStrSave)
{
   HRESULT hr = pStrSave->BeginUnit(_T("Criteria"),1.0);
   if ( FAILED(hr) )
      return hr;

   pStrSave->put_Property(_T("FScr"),CComVariant(MinFScr));
   pStrSave->put_Property(_T("FSf"),CComVariant(MinFSf));
   pStrSave->put_Property(_T("CompressionCoefficient"),CComVariant(CompressionCoefficient));
   pStrSave->BeginUnit(_T("CrownSlope"),1.0);
   pStrSave->put_Property(_T("TensionCoefficient"),CComVariant(TensionCoefficient[stbTypes::CrownSlope]));
   pStrSave->put_Property(_T("UseMaxTension"),CComVariant(bMaxTension[stbTypes::CrownSlope]));
   pStrSave->put_Property(_T("MaxTension"),CComVariant(MaxTension[stbTypes::CrownSlope]));
   pStrSave->put_Property(_T("TensionCoefficientWithRebar"),CComVariant(TensionCoefficientWithRebar[stbTypes::CrownSlope]));
   pStrSave->EndUnit(); // CrownSlope
   pStrSave->BeginUnit(_T("MaxSuper"),1.0);
   pStrSave->put_Property(_T("TensionCoefficient"),CComVariant(TensionCoefficient[stbTypes::MaxSuper]));
   pStrSave->put_Property(_T("UseMaxTension"),CComVariant(bMaxTension[stbTypes::MaxSuper]));
   pStrSave->put_Property(_T("MaxTension"),CComVariant(MaxTension[stbTypes::MaxSuper]));
   pStrSave->put_Property(_T("TensionCoefficientWithRebar"),CComVariant(TensionCoefficientWithRebar[stbTypes::MaxSuper]));
   pStrSave->EndUnit(); // MaxSuper
   pStrSave->put_Property(_T("MaxClearSpan"),CComVariant(MaxClearSpan));
   pStrSave->put_Property(_T("MaxLeadingOverhang"),CComVariant(MaxLeadingOverhang));
   pStrSave->put_Property(_T("MaxGirderWeight"),CComVariant(MaxGirderWeight));

   hr = pStrSave->EndUnit(); // Criteria
   if ( FAILED(hr) )
      return hr;

   return S_OK;
}

HRESULT CPGStableHaulingCriteria::Load(IStructuredLoad* pStrLoad)
{
   CHRException hr;

   try
   {
      hr = pStrLoad->BeginUnit(_T("Criteria"));

      CComVariant var;

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("FScr"),&var);
      MinFScr = var.dblVal;

      hr = pStrLoad->get_Property(_T("FSf"),&var);
      MinFSf = var.dblVal;

      hr = pStrLoad->get_Property(_T("CompressionCoefficient"),&var);
      CompressionCoefficient = var.dblVal;

      hr = pStrLoad->BeginUnit(_T("CrownSlope"));
      hr = pStrLoad->get_Property(_T("TensionCoefficient"),&var);
      TensionCoefficient[stbTypes::CrownSlope] = var.dblVal;

      var.vt = VT_BOOL;
      hr = pStrLoad->get_Property(_T("UseMaxTension"),&var);
      bMaxTension[stbTypes::CrownSlope] = (var.boolVal == VARIANT_TRUE ? true : false);

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("MaxTension"),&var);
      MaxTension[stbTypes::CrownSlope] = var.dblVal;

      hr = pStrLoad->get_Property(_T("TensionCoefficientWithRebar"),&var);
      TensionCoefficientWithRebar[stbTypes::CrownSlope] = var.dblVal;
      pStrLoad->EndUnit(); // CrownSlope


      hr = pStrLoad->BeginUnit(_T("MaxSuper"));
      hr = pStrLoad->get_Property(_T("TensionCoefficient"),&var);
      TensionCoefficient[stbTypes::MaxSuper] = var.dblVal;

      var.vt = VT_BOOL;
      hr = pStrLoad->get_Property(_T("UseMaxTension"),&var);
      bMaxTension[stbTypes::MaxSuper] = (var.boolVal == VARIANT_TRUE ? true : false);

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("MaxTension"),&var);
      MaxTension[stbTypes::MaxSuper] = var.dblVal;

      hr = pStrLoad->get_Property(_T("TensionCoefficientWithRebar"),&var);
      TensionCoefficientWithRebar[stbTypes::MaxSuper] = var.dblVal;
      pStrLoad->EndUnit(); // MaxSuper

      hr = pStrLoad->get_Property(_T("MaxClearSpan"),&var);
      MaxClearSpan = var.dblVal;

      hr = pStrLoad->get_Property(_T("MaxLeadingOverhang"),&var);
      MaxLeadingOverhang = var.dblVal;

      hr = pStrLoad->get_Property(_T("MaxGirderWeight"),&var);
      MaxGirderWeight = var.dblVal;

      hr = pStrLoad->EndUnit(); // Criteria
   }
   catch(...)
   {
      THROW_LOAD(InvalidFileFormat,pStrLoad);
   }

   return S_OK;
}

