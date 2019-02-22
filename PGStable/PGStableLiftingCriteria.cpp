///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2019  Washington State Department of Transportation
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

// PGStableLiftingCriteria.cpp : implementation file
//

#include "stdafx.h"
#include "PGStableLiftingCriteria.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CPGStableLiftingCriteria::CPGStableLiftingCriteria()
{
   MinFScr = 1.0;
   MinFSf = 1.5;
   CompressionCoefficient_GlobalStress = 0.65;
   CompressionCoefficient_PeakStress = 0.70;
   TensionCoefficient = ::ConvertToSysUnits(0.0948,unitMeasure::SqrtKSI);
   bMaxTension = false;
   MaxTension = ::ConvertToSysUnits(0.2,unitMeasure::KSI);
   TensionCoefficientWithRebar = ::ConvertToSysUnits(0.1900,unitMeasure::SqrtKSI);
}

CPGStableLiftingCriteria::~CPGStableLiftingCriteria()
{
}

bool CPGStableLiftingCriteria::operator==(const CPGStableLiftingCriteria& other) const
{
   if ( !IsEqual(MinFScr,other.MinFScr) )
      return false;
   
   if ( !IsEqual(MinFSf,other.MinFSf) )
      return false;

   if (!IsEqual(CompressionCoefficient_GlobalStress, other.CompressionCoefficient_GlobalStress))
      return false;

   if (!IsEqual(CompressionCoefficient_PeakStress, other.CompressionCoefficient_PeakStress))
      return false;

   if ( !IsEqual(TensionCoefficient,other.TensionCoefficient) )
      return false;

   if ( bMaxTension != other.bMaxTension )
      return false;
   
   if ( !IsEqual(MaxTension,other.MaxTension) )
      return false;
   
   if ( !IsEqual(TensionCoefficientWithRebar,other.TensionCoefficientWithRebar) )
      return false;

   return true;
}

bool CPGStableLiftingCriteria::operator!=(const CPGStableLiftingCriteria& other) const
{
   return !(*this == other);
}

void CPGStableLiftingCriteria::operator=(const stbLiftingCriteria& other)
{
   *((stbLiftingCriteria*)this) = other;
}

HRESULT CPGStableLiftingCriteria::Save(IStructuredSave* pStrSave)
{
   HRESULT hr = pStrSave->BeginUnit(_T("Criteria"),2.0);
   if ( FAILED(hr) )
      return hr;

   pStrSave->put_Property(_T("FScr"),CComVariant(MinFScr));
   pStrSave->put_Property(_T("FSf"),CComVariant(MinFSf));
   //pStrSave->put_Property(_T("CompressionCoefficient"),CComVariant(CompressionCoefficient)); // removed in version 2
   pStrSave->put_Property(_T("CompressionCoefficient_GlobalStress"),CComVariant(CompressionCoefficient_GlobalStress)); // added in version 2
   pStrSave->put_Property(_T("CompressionCoefficient_PeakStress"),CComVariant(CompressionCoefficient_PeakStress)); // added in version 2
   pStrSave->put_Property(_T("TensionCoefficient"),CComVariant(TensionCoefficient));
   pStrSave->put_Property(_T("UseMaxTension"),CComVariant(bMaxTension));
   pStrSave->put_Property(_T("MaxTension"),CComVariant(MaxTension));
   pStrSave->put_Property(_T("TensionCoefficientWithRebar"),CComVariant(TensionCoefficientWithRebar));

   hr = pStrSave->EndUnit(); // Criteria
   if ( FAILED(hr) )
      return hr;

   return S_OK;
}

HRESULT CPGStableLiftingCriteria::Load(IStructuredLoad* pStrLoad)
{
   CHRException hr;

   try
   {
      hr = pStrLoad->BeginUnit(_T("Criteria"));

      Float64 version;
      pStrLoad->get_Version(&version);

      CComVariant var;

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("FScr"),&var);
      MinFScr = var.dblVal;

      hr = pStrLoad->get_Property(_T("FSf"),&var);
      MinFSf = var.dblVal;

      if (version < 2)
      {
         hr = pStrLoad->get_Property(_T("CompressionCoefficient"), &var);
         CompressionCoefficient_GlobalStress = var.dblVal;
         CompressionCoefficient_PeakStress = CompressionCoefficient_GlobalStress;
      }
      else
      {
         hr = pStrLoad->get_Property(_T("CompressionCoefficient_GlobalStress"), &var);
         CompressionCoefficient_GlobalStress = var.dblVal;

         hr = pStrLoad->get_Property(_T("CompressionCoefficient_PeakStress"), &var);
         CompressionCoefficient_PeakStress = var.dblVal;
      }

      hr = pStrLoad->get_Property(_T("TensionCoefficient"),&var);
      TensionCoefficient = var.dblVal;

      var.vt = VT_BOOL;
      hr = pStrLoad->get_Property(_T("UseMaxTension"),&var);
      bMaxTension = (var.boolVal == VARIANT_TRUE ? true : false);

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("MaxTension"),&var);
      MaxTension = var.dblVal;

      hr = pStrLoad->get_Property(_T("TensionCoefficientWithRebar"),&var);
      TensionCoefficientWithRebar = var.dblVal;

      hr = pStrLoad->EndUnit(); // Criteria
   }
   catch(...)
   {
      THROW_LOAD(InvalidFileFormat,pStrLoad);
   }

   return S_OK;
}

