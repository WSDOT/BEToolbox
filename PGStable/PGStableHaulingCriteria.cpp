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

// PGStableHaulingCriteria.cpp : implementation file
//

#include "stdafx.h"
#include "PGStableHaulingCriteria.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CPGStableHaulingCriteria::CPGStableHaulingCriteria()
{
   MinFScr = 1.0;
   MinFSf = 1.5;
   CompressionCoefficient_GlobalStress = 0.65;
   CompressionCoefficient_PeakStress = 0.70;

   MaxClearSpan = WBFL::Units::ConvertToSysUnits(130.0, WBFL::Units::Measure::Feet);
   MaxLeadingOverhang = WBFL::Units::ConvertToSysUnits(15, WBFL::Units::Measure::Feet);
   MaxGirderWeight = WBFL::Units::ConvertToSysUnits(200.0, WBFL::Units::Measure::Kip);

   WBFL::Stability::CCHaulingTensionStressLimit* pTensionStressLimit = new WBFL::Stability::CCHaulingTensionStressLimit;

   pTensionStressLimit->TensionCoefficient[+WBFL::Stability::HaulingImpact::NormalCrown] = WBFL::Units::ConvertToSysUnits(0.0948,WBFL::Units::Measure::SqrtKSI);
   pTensionStressLimit->bMaxTension[+WBFL::Stability::HaulingImpact::NormalCrown] = false;
   pTensionStressLimit->MaxTension[+WBFL::Stability::HaulingImpact::NormalCrown] = WBFL::Units::ConvertToSysUnits(0.2,WBFL::Units::Measure::KSI);
   pTensionStressLimit->TensionCoefficientWithRebar[+WBFL::Stability::HaulingImpact::NormalCrown] = WBFL::Units::ConvertToSysUnits(0.1900,WBFL::Units::Measure::SqrtKSI);

   pTensionStressLimit->TensionCoefficient[+WBFL::Stability::HaulingImpact::MaxSuper] = WBFL::Units::ConvertToSysUnits(0.0948,WBFL::Units::Measure::SqrtKSI);
   pTensionStressLimit->bMaxTension[+WBFL::Stability::HaulingImpact::MaxSuper] = false;
   pTensionStressLimit->MaxTension[+WBFL::Stability::HaulingImpact::MaxSuper] = WBFL::Units::ConvertToSysUnits(0.2,WBFL::Units::Measure::KSI);
   pTensionStressLimit->TensionCoefficientWithRebar[+WBFL::Stability::HaulingImpact::MaxSuper] = WBFL::Units::ConvertToSysUnits(0.24,WBFL::Units::Measure::SqrtKSI);

   TensionStressLimit.reset(pTensionStressLimit);
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

   if (!IsEqual(CompressionCoefficient_GlobalStress, other.CompressionCoefficient_GlobalStress))
      return false;

   if (!IsEqual(CompressionCoefficient_PeakStress, other.CompressionCoefficient_PeakStress))
      return false;

   auto* pTensionStressLimit = dynamic_cast<WBFL::Stability::CCHaulingTensionStressLimit*>(TensionStressLimit.get());
   auto* pOtherTensionStressLimit = dynamic_cast<WBFL::Stability::CCHaulingTensionStressLimit*>(other.TensionStressLimit.get());

   for ( int s = 0; s < 2; s++ )
   {
      WBFL::Stability::HaulingSlope slope = (WBFL::Stability::HaulingSlope)s;

      if ( !IsEqual(pTensionStressLimit->TensionCoefficient[+slope], pOtherTensionStressLimit->TensionCoefficient[+slope]) )
         return false;

      if (pTensionStressLimit->bMaxTension[+slope] != pOtherTensionStressLimit->bMaxTension[+slope] )
         return false;
      
      if ( !IsEqual(pTensionStressLimit->MaxTension[+slope], pOtherTensionStressLimit->MaxTension[+slope]) )
         return false;
      
      if ( !IsEqual(pTensionStressLimit->TensionCoefficientWithRebar[+slope], pOtherTensionStressLimit->TensionCoefficientWithRebar[+slope]) )
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

void CPGStableHaulingCriteria::operator=(const WBFL::Stability::HaulingCriteria& other)
{
   *((WBFL::Stability::HaulingCriteria*)this) = other;
}

HRESULT CPGStableHaulingCriteria::Save(IStructuredSave* pStrSave)
{
   HRESULT hr = pStrSave->BeginUnit(_T("Criteria"),2.0);
   if ( FAILED(hr) )
      return hr;

   pStrSave->put_Property(_T("FScr"),CComVariant(MinFScr));
   pStrSave->put_Property(_T("FSf"),CComVariant(MinFSf));
   //pStrSave->put_Property(_T("CompressionCoefficient"),CComVariant(CompressionCoefficient)); // removed in version 2
   pStrSave->put_Property(_T("GlobalCompressionCoefficient"), CComVariant(CompressionCoefficient_GlobalStress)); // add in version 2
   pStrSave->put_Property(_T("PeakCompressionCoefficient"), CComVariant(CompressionCoefficient_PeakStress)); // add in version 2

   auto* pTensionStressLimit = dynamic_cast<WBFL::Stability::CCHaulingTensionStressLimit*>(TensionStressLimit.get());

   pStrSave->BeginUnit(_T("CrownSlope"),1.0);
   pStrSave->put_Property(_T("TensionCoefficient"),CComVariant(pTensionStressLimit->TensionCoefficient[+WBFL::Stability::HaulingSlope::CrownSlope]));
   pStrSave->put_Property(_T("UseMaxTension"),CComVariant(pTensionStressLimit->bMaxTension[+WBFL::Stability::HaulingSlope::CrownSlope]));
   pStrSave->put_Property(_T("MaxTension"),CComVariant(pTensionStressLimit->MaxTension[+WBFL::Stability::HaulingSlope::CrownSlope]));
   pStrSave->put_Property(_T("TensionCoefficientWithRebar"),CComVariant(pTensionStressLimit->TensionCoefficientWithRebar[+WBFL::Stability::HaulingSlope::CrownSlope]));
   pStrSave->EndUnit(); // CrownSlope
   pStrSave->BeginUnit(_T("MaxSuper"),1.0); // we mistakenly used MaxSuper when we meant Superelevation.... don't change the data block name since it would cause unnecessary changes in the file format
   pStrSave->put_Property(_T("TensionCoefficient"),CComVariant(pTensionStressLimit->TensionCoefficient[+WBFL::Stability::HaulingSlope::Superelevation]));
   pStrSave->put_Property(_T("UseMaxTension"),CComVariant(pTensionStressLimit->bMaxTension[+WBFL::Stability::HaulingSlope::Superelevation]));
   pStrSave->put_Property(_T("MaxTension"),CComVariant(pTensionStressLimit->MaxTension[+WBFL::Stability::HaulingSlope::Superelevation]));
   pStrSave->put_Property(_T("TensionCoefficientWithRebar"),CComVariant(pTensionStressLimit->TensionCoefficientWithRebar[+WBFL::Stability::HaulingSlope::Superelevation]));
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
      }
      else
      {
         hr = pStrLoad->get_Property(_T("GlobalCompressionCoefficient"), &var);
         CompressionCoefficient_GlobalStress = var.dblVal;

         hr = pStrLoad->get_Property(_T("PeakCompressionCoefficient"), &var);
         CompressionCoefficient_PeakStress = var.dblVal;
      }

      auto* pTensionStressLimit = dynamic_cast<WBFL::Stability::CCHaulingTensionStressLimit*>(TensionStressLimit.get());

      hr = pStrLoad->BeginUnit(_T("CrownSlope"));
      hr = pStrLoad->get_Property(_T("TensionCoefficient"),&var);
      pTensionStressLimit->TensionCoefficient[+WBFL::Stability::HaulingSlope::CrownSlope] = var.dblVal;

      var.vt = VT_BOOL;
      hr = pStrLoad->get_Property(_T("UseMaxTension"),&var);
      pTensionStressLimit->bMaxTension[+WBFL::Stability::HaulingSlope::CrownSlope] = (var.boolVal == VARIANT_TRUE ? true : false);

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("MaxTension"),&var);
      pTensionStressLimit->MaxTension[+WBFL::Stability::HaulingSlope::CrownSlope] = var.dblVal;

      hr = pStrLoad->get_Property(_T("TensionCoefficientWithRebar"),&var);
      pTensionStressLimit->TensionCoefficientWithRebar[+WBFL::Stability::HaulingSlope::CrownSlope] = var.dblVal;
      pStrLoad->EndUnit(); // CrownSlope


      hr = pStrLoad->BeginUnit(_T("MaxSuper"));
      hr = pStrLoad->get_Property(_T("TensionCoefficient"),&var);
      pTensionStressLimit->TensionCoefficient[+WBFL::Stability::HaulingSlope::Superelevation] = var.dblVal;

      var.vt = VT_BOOL;
      hr = pStrLoad->get_Property(_T("UseMaxTension"),&var);
      pTensionStressLimit->bMaxTension[+WBFL::Stability::HaulingSlope::Superelevation] = (var.boolVal == VARIANT_TRUE ? true : false);

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("MaxTension"),&var);
      pTensionStressLimit->MaxTension[+WBFL::Stability::HaulingSlope::Superelevation] = var.dblVal;

      hr = pStrLoad->get_Property(_T("TensionCoefficientWithRebar"),&var);
      pTensionStressLimit->TensionCoefficientWithRebar[+WBFL::Stability::HaulingSlope::Superelevation] = var.dblVal;
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

