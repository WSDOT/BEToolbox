///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2020  Washington State Department of Transportation
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
#include "M3CProblemParameters.h"

CM3CProblemParameters::CM3CProblemParameters()
{
   D = ::ConvertToSysUnits(72, unitMeasure::Inch);

   fco = ::ConvertToSysUnits(5.0, unitMeasure::KSI);
   eco = 0.003;

   AsSpiral = ::ConvertToSysUnits(0.44, unitMeasure::Inch2);
   db = ::ConvertToSysUnits(0.75, unitMeasure::Inch);
   S = ::ConvertToSysUnits(3, unitMeasure::Inch);
   FySpiral = ::ConvertToSysUnits(60, unitMeasure::KSI);
   esu = 0.06;

   fy = ::ConvertToSysUnits(60.0, unitMeasure::KSI);
   fu = ::ConvertToSysUnits(90.0, unitMeasure::KSI);
   Es = ::ConvertToSysUnits(29000, unitMeasure::KSI);
   esh = 0.0115;
   efr = 0.06;

   Cover = ::ConvertToSysUnits(1.5, unitMeasure::Inch);

   Bonded_Rebar_As_per_bar = ::ConvertToSysUnits(1.56, unitMeasure::Inch2);
   Bonded_Rebar_nBars = 28;

   Unbonded_Rebar_As_per_bar = 0;
   Unbonded_Rebar_nBars = 0;
   Unbonded_Rebar_Lu = ::ConvertToSysUnits(5.0, unitMeasure::Feet);

   lrfdStrandPool* pPool = lrfdStrandPool::GetInstance();
   pStrand = pPool->GetStrand(matPsStrand::Gr1860, matPsStrand::LowRelaxation, matPsStrand::None, matPsStrand::D1524);
   Tendon_Ring_Diameter = ::ConvertToSysUnits(12, unitMeasure::Inch);
   Tendon_nStrands = 0;
   Tendon_fpe = ::ConvertToSysUnits(0.85*202.5, unitMeasure::KSI); // assume 15% loss
   Tendon_Lu = ::ConvertToSysUnits(20.0, unitMeasure::Feet);

   nSlices = 20;
   initialStep = ::ConvertToSysUnits(0.00001,unitMeasure::PerInch);
   P = 0;
}

bool CM3CProblemParameters::operator==(const CM3CProblemParameters& other) const
{
   if (!IsEqual(D, other.D))
      return false;
   
   if (!IsEqual(fco, other.fco))
      return false;

   if (!IsEqual(eco, other.eco))
      return false;

   if (!IsEqual(Cover, other.Cover))
      return false;

   if (!IsEqual(Bonded_Rebar_As_per_bar, other.Bonded_Rebar_As_per_bar))
      return false;

   if (Bonded_Rebar_nBars != other.Bonded_Rebar_nBars)
      return false;


   if (!IsEqual(Unbonded_Rebar_As_per_bar, other.Unbonded_Rebar_As_per_bar))
      return false;

   if (Unbonded_Rebar_nBars != other.Unbonded_Rebar_nBars)
      return false;

   if (!IsEqual(Unbonded_Rebar_Lu, other.Unbonded_Rebar_Lu))
      return false;


   if (!IsEqual(fy, other.fy))
      return false;

   if (!IsEqual(fu, other.fu))
      return false;

   if (!IsEqual(Es, other.Es))
      return false;

   if (!IsEqual(esh, other.esh))
      return false;

   if (!IsEqual(efr, other.efr))
      return false;


   if (!IsEqual(AsSpiral, other.AsSpiral))
      return false;

   if (!IsEqual(db, other.db))
      return false;

   if (!IsEqual(S, other.S))
      return false;

   if (!IsEqual(FySpiral, other.FySpiral))
      return false;

   if (!IsEqual(esu, other.esu))
      return false;


   if (pStrand != other.pStrand)
      return false;
   
   
   if ( !IsEqual(Tendon_Ring_Diameter,other.Tendon_Ring_Diameter) )
      return false;

   if (Tendon_nStrands != other.Tendon_nStrands)
      return false;

   if (!IsEqual(Tendon_fpe, other.Tendon_fpe))
      return false;

   if (!IsEqual(Tendon_Lu, other.Tendon_Lu))
      return false;


   if (nSlices != other.nSlices)
      return false;

   if (!IsEqual(initialStep, other.initialStep))
      return false;

   if (!IsEqual(P, other.P))
      return false;

   return true;
}

bool CM3CProblemParameters::operator!=(const CM3CProblemParameters& other) const
{
   return !(*this == other);
}

HRESULT CM3CProblemParameters::Save(IStructuredSave* pStrSave)
{
   HRESULT hr = pStrSave->BeginUnit(_T("ProblemParameters"), 1.0);
   if (FAILED(hr))
      return hr;

   hr = pStrSave->put_Property(_T("Diameter"), CComVariant(D));
   if (FAILED(hr))
      return hr;

   // Confined Concrete Model
   hr = pStrSave->put_Property(_T("fco"), CComVariant(fco));
   if (FAILED(hr))
      return hr;

   hr = pStrSave->put_Property(_T("eco"), CComVariant(eco));
   if (FAILED(hr))
      return hr;


   hr = pStrSave->put_Property(_T("AsSpiral"), CComVariant(AsSpiral));
   if (FAILED(hr))
      return hr;

   hr = pStrSave->put_Property(_T("db"), CComVariant(db));
   if (FAILED(hr))
      return hr;

   hr = pStrSave->put_Property(_T("S"), CComVariant(S));
   if (FAILED(hr))
      return hr;

   hr = pStrSave->put_Property(_T("FySpiral"), CComVariant(FySpiral));
   if (FAILED(hr))
      return hr;

   hr = pStrSave->put_Property(_T("esu"), CComVariant(esu));
   if (FAILED(hr))
      return hr;

   // Reinforcement
   hr = pStrSave->put_Property(_T("fy"), CComVariant(fy));
   if (FAILED(hr))
      return hr;

   hr = pStrSave->put_Property(_T("fu"), CComVariant(fu));
   if (FAILED(hr))
      return hr;

   hr = pStrSave->put_Property(_T("Es"), CComVariant(Es));
   if (FAILED(hr))
      return hr;

   hr = pStrSave->put_Property(_T("esh"), CComVariant(esh));
   if (FAILED(hr))
      return hr;

   hr = pStrSave->put_Property(_T("efr"), CComVariant(efr));
   if (FAILED(hr))
      return hr;

   hr = pStrSave->put_Property(_T("Cover"), CComVariant(Cover));
   if (FAILED(hr))
      return hr;

   hr = pStrSave->BeginUnit(_T("BondedReinforcement"), 1.0);
   {
      hr = pStrSave->put_Property(_T("As_per_bar"), CComVariant(Bonded_Rebar_As_per_bar));
      if (FAILED(hr))
         return hr;

      hr = pStrSave->put_Property(_T("nBars"), CComVariant(Bonded_Rebar_nBars));
      if (FAILED(hr))
         return hr;
   }
   hr = pStrSave->EndUnit(); // BondedReinforcement


   hr = pStrSave->BeginUnit(_T("UnbondedReinforcement"), 1.0);
   {
      hr = pStrSave->put_Property(_T("As_per_bar"), CComVariant(Unbonded_Rebar_As_per_bar));
      if (FAILED(hr))
         return hr;

      hr = pStrSave->put_Property(_T("nBars"), CComVariant(Unbonded_Rebar_nBars));
      if (FAILED(hr))
         return hr;

      hr = pStrSave->put_Property(_T("Lu"), CComVariant(Unbonded_Rebar_Lu));
      if (FAILED(hr))
         return hr;
   }
   hr = pStrSave->EndUnit(); // BondedReinforcement

   // Controls
   hr = pStrSave->put_Property(_T("nSlices"), CComVariant(nSlices));
   if (FAILED(hr))
      return hr;

   hr = pStrSave->put_Property(_T("InitialStep"), CComVariant(initialStep));
   if (FAILED(hr))
      return hr;

   hr = pStrSave->put_Property(_T("P"), CComVariant(P));
   if (FAILED(hr))
      return hr;

   hr = pStrSave->EndUnit(); // ProblemParameters
   if (FAILED(hr))
      return hr;

   return S_OK;
}

HRESULT CM3CProblemParameters::Load(IStructuredLoad* pStrLoad)
{
   HRESULT hr = pStrLoad->BeginUnit(_T("ProblemParameters"));
   if (FAILED(hr))
      return hr;

   CComVariant var;
   var.vt = VT_R8;

   hr = pStrLoad->get_Property(_T("Diameter"), &var);
   if (FAILED(hr))
      return hr;
   D = var.dblVal;

   hr = pStrLoad->get_Property(_T("fco"), &var);
   if (FAILED(hr))
      return hr;
   fco = var.dblVal;

   hr = pStrLoad->get_Property(_T("eco"), &var);
   if (FAILED(hr))
      return hr;
   eco = var.dblVal;

   hr = pStrLoad->get_Property(_T("AsSpiral"), &var);
   if (FAILED(hr))
      return hr;
   AsSpiral = var.dblVal;

   hr = pStrLoad->get_Property(_T("db"), &var);
   if (FAILED(hr))
      return hr;
   db = var.dblVal;

   hr = pStrLoad->get_Property(_T("S"), &var);
   if (FAILED(hr))
      return hr;
   S = var.dblVal;

   hr = pStrLoad->get_Property(_T("FySpiral"), &var);
   if (FAILED(hr))
      return hr;
   FySpiral = var.dblVal;

   hr = pStrLoad->get_Property(_T("esu"), &var);
   if (FAILED(hr))
      return hr;
   esu = var.dblVal;


   var.vt = VT_R8;
   hr = pStrLoad->get_Property(_T("fy"), &var);
   if (FAILED(hr))
      return hr;
   fy = var.dblVal;

   hr = pStrLoad->get_Property(_T("fu"), &var);
   if (FAILED(hr))
      return hr;
   fu = var.dblVal;

   hr = pStrLoad->get_Property(_T("Es"), &var);
   if (FAILED(hr))
      return hr;
   Es = var.dblVal;

   hr = pStrLoad->get_Property(_T("esh"), &var);
   if (FAILED(hr))
      return hr;
   esh = var.dblVal;

   hr = pStrLoad->get_Property(_T("efr"), &var);
   if (FAILED(hr))
      return hr;
   efr = var.dblVal;

   hr = pStrLoad->get_Property(_T("Cover"), &var);
   if (FAILED(hr))
      return hr;
   Cover = var.dblVal;

   hr = pStrLoad->BeginUnit(_T("BondedReinforcement"));
   {
      hr = pStrLoad->get_Property(_T("As_per_bar"), &var);
      if (FAILED(hr))
         return hr;
      Bonded_Rebar_As_per_bar = var.dblVal;

      var.vt = VT_I4;
      hr = pStrLoad->get_Property(_T("nBars"), &var);
      if (FAILED(hr))
         return hr;
      Bonded_Rebar_nBars = var.lVal;
   }
   hr = pStrLoad->EndUnit(); // BondedReinforcement

   hr = pStrLoad->BeginUnit(_T("UnbondedReinforcement"));
   {
      hr = pStrLoad->get_Property(_T("As_per_bar"), &var);
      if (FAILED(hr))
         return hr;
      Unbonded_Rebar_As_per_bar = var.dblVal;

      var.vt = VT_I4;
      hr = pStrLoad->get_Property(_T("nBars"), &var);
      if (FAILED(hr))
         return hr;
      Unbonded_Rebar_nBars = var.lVal;

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("Lu"), &var);
      if (FAILED(hr))
         return hr;
      Unbonded_Rebar_Lu = var.dblVal;
   }
   hr = pStrLoad->EndUnit(); // UnbondedReinforcement

   var.vt = VT_I4;
   hr = pStrLoad->get_Property(_T("nSlices"), &var);
   if (FAILED(hr))
      return hr;
   nSlices = var.lVal;

   var.vt = VT_R8;
   hr = pStrLoad->get_Property(_T("InitialStep"), &var);
   if (FAILED(hr))
      return hr;
   initialStep = var.dblVal;

   hr = pStrLoad->get_Property(_T("P"), &var);
   if (FAILED(hr))
      return hr;
   P = var.dblVal;

   hr = pStrLoad->EndUnit();
   if (FAILED(hr))
      return hr;

   return S_OK;
}
