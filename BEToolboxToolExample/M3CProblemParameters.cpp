///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2018  Washington State Department of Transportation
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

   fco = ::ConvertToSysUnits(5.2, unitMeasure::KSI);
   eco = 0.002;
   R = 5;

   Cover = ::ConvertToSysUnits(1.5, unitMeasure::Inch);
   As_per_bar = ::ConvertToSysUnits(1.56, unitMeasure::Inch2);
   nBars = 28;
   fy = ::ConvertToSysUnits(66.0, unitMeasure::KSI);
   fu = ::ConvertToSysUnits(92.4, unitMeasure::KSI);
   Es = ::ConvertToSysUnits(29000, unitMeasure::KSI);
   esh = 0.0115;
   efr = 0.06;

   AsSpiral = ::ConvertToSysUnits(0.44, unitMeasure::Inch2);
   db = ::ConvertToSysUnits(0.75, unitMeasure::Inch);
   S = ::ConvertToSysUnits(3, unitMeasure::Inch);
   FySpiral = ::ConvertToSysUnits(66, unitMeasure::KSI);
   esu = 0.06;

   nSlices = 50;
   initialStep = ::ConvertToSysUnits(0.00001,unitMeasure::PerInch);
   P = 0;
   NASlope = 0;
}

bool CM3CProblemParameters::operator==(const CM3CProblemParameters& other) const
{
   if (!IsEqual(D, other.D))
      return false;
   
   if (!IsEqual(fco, other.fco))
      return false;

   if (!IsEqual(eco, other.eco))
      return false;

   if (!IsEqual(R, other.R))
      return false;

   if (!IsEqual(Cover, other.Cover))
      return false;

   if (!IsEqual(As_per_bar, other.As_per_bar))
      return false;

   if (nBars != other.nBars)
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


   if (nSlices != other.nSlices)
      return false;

   if (!IsEqual(initialStep, other.initialStep))
      return false;

   if (!IsEqual(P, other.P))
      return false;

   if (!IsEqual(NASlope, other.NASlope))
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

   hr = pStrSave->put_Property(_T("fco"), CComVariant(fco));
   if (FAILED(hr))
      return hr;

   hr = pStrSave->put_Property(_T("eco"), CComVariant(eco));
   if (FAILED(hr))
      return hr;

   hr = pStrSave->put_Property(_T("R"), CComVariant(R));
   if (FAILED(hr))
      return hr;

   hr = pStrSave->put_Property(_T("Cover"), CComVariant(Cover));
   if (FAILED(hr))
      return hr;

   hr = pStrSave->put_Property(_T("As_per_bar"), CComVariant(As_per_bar));
   if (FAILED(hr))
      return hr;

   hr = pStrSave->put_Property(_T("nBars"), CComVariant(nBars));
   if (FAILED(hr))
      return hr;

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

   hr = pStrSave->put_Property(_T("nSlices"), CComVariant(nSlices));
   if (FAILED(hr))
      return hr;

   hr = pStrSave->put_Property(_T("InitialStep"), CComVariant(initialStep));
   if (FAILED(hr))
      return hr;

   hr = pStrSave->put_Property(_T("P"), CComVariant(P));
   if (FAILED(hr))
      return hr;

   hr = pStrSave->put_Property(_T("NASlope"), CComVariant(NASlope));
   if (FAILED(hr))
      return hr;

   hr = pStrSave->EndUnit();
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

   hr = pStrLoad->get_Property(_T("R"), &var);
   if (FAILED(hr))
      return hr;
   R = var.dblVal;

   hr = pStrLoad->get_Property(_T("Cover"), &var);
   if (FAILED(hr))
      return hr;
   Cover = var.dblVal;

   hr = pStrLoad->get_Property(_T("As_per_bar"), &var);
   if (FAILED(hr))
      return hr;
   As_per_bar = var.dblVal;

   var.vt = VT_I4;
   hr = pStrLoad->get_Property(_T("nBars"), &var);
   if (FAILED(hr))
      return hr;
   nBars = var.lVal;

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

   hr = pStrLoad->get_Property(_T("NASlope"), &var);
   if (FAILED(hr))
      return hr;
   NASlope = var.dblVal;

   hr = pStrLoad->EndUnit();
   if (FAILED(hr))
      return hr;

   return S_OK;
}
