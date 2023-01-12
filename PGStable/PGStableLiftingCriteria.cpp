///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2023  Washington State Department of Transportation
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

   WBFL::Stability::CCLiftingTensionStressLimit* pTensionStressLimit = new WBFL::Stability::CCLiftingTensionStressLimit;
   pTensionStressLimit->Lambda = 1.0;
   pTensionStressLimit->TensionCoefficient = ::ConvertToSysUnits(0.0948,unitMeasure::SqrtKSI);
   pTensionStressLimit->bMaxTension = false;
   pTensionStressLimit->MaxTension = ::ConvertToSysUnits(0.2,unitMeasure::KSI);
   pTensionStressLimit->bWithRebarLimit = false;
   pTensionStressLimit->TensionCoefficientWithRebar = ::ConvertToSysUnits(0.1900,unitMeasure::SqrtKSI);

   TensionStressLimit.reset(pTensionStressLimit);
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

   auto* pTensionStressLimit = dynamic_cast<WBFL::Stability::CCLiftingTensionStressLimit*>(TensionStressLimit.get());
   auto* pOtherTensionStressLimit = dynamic_cast<WBFL::Stability::CCLiftingTensionStressLimit*>(other.TensionStressLimit.get());

   if ( !IsEqual(pTensionStressLimit->TensionCoefficient, pOtherTensionStressLimit->TensionCoefficient) )
      return false;

   if (pTensionStressLimit->bMaxTension != pOtherTensionStressLimit->bMaxTension )
      return false;
   
   if ( !IsEqual(pTensionStressLimit->MaxTension, pOtherTensionStressLimit->MaxTension) )
      return false;
   
   if ( !IsEqual(pTensionStressLimit->TensionCoefficientWithRebar, pOtherTensionStressLimit->TensionCoefficientWithRebar) )
      return false;

   return true;
}

bool CPGStableLiftingCriteria::operator!=(const CPGStableLiftingCriteria& other) const
{
   return !(*this == other);
}

void CPGStableLiftingCriteria::operator=(const WBFL::Stability::LiftingCriteria& other)
{
   *((WBFL::Stability::LiftingCriteria*)this) = other;
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

   auto* pTensionStressLimit = dynamic_cast<WBFL::Stability::CCLiftingTensionStressLimit*>(TensionStressLimit.get());
   pStrSave->put_Property(_T("TensionCoefficient"),CComVariant(pTensionStressLimit->TensionCoefficient));
   pStrSave->put_Property(_T("UseMaxTension"),CComVariant(pTensionStressLimit->bMaxTension));
   pStrSave->put_Property(_T("MaxTension"),CComVariant(pTensionStressLimit->MaxTension));
   pStrSave->put_Property(_T("TensionCoefficientWithRebar"),CComVariant(pTensionStressLimit->TensionCoefficientWithRebar));

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

      auto* pTensionStressLimit = dynamic_cast<WBFL::Stability::CCLiftingTensionStressLimit*>(TensionStressLimit.get());

      hr = pStrLoad->get_Property(_T("TensionCoefficient"),&var);
      pTensionStressLimit->TensionCoefficient = var.dblVal;

      var.vt = VT_BOOL;
      hr = pStrLoad->get_Property(_T("UseMaxTension"),&var);
      pTensionStressLimit->bMaxTension = (var.boolVal == VARIANT_TRUE ? true : false);

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("MaxTension"),&var);
      pTensionStressLimit->MaxTension = var.dblVal;

      hr = pStrLoad->get_Property(_T("TensionCoefficientWithRebar"),&var);
      pTensionStressLimit->TensionCoefficientWithRebar = var.dblVal;

      hr = pStrLoad->EndUnit(); // Criteria
   }
   catch(...)
   {
      THROW_LOAD(InvalidFileFormat,pStrLoad);
   }

   return S_OK;
}

