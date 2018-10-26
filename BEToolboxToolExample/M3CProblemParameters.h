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

#pragma once

class CM3CProblemParameters
{
public:
   CM3CProblemParameters();

   bool operator==(const CM3CProblemParameters& other) const;
   bool operator!=(const CM3CProblemParameters& other) const;

   HRESULT Save(IStructuredSave* pStrSave);
   HRESULT Load(IStructuredLoad* pStrLoad);

   Float64 D;

   Float64 fco;
   Float64 eco;
   Float64 R;

   Float64 Cover;
   Float64 As_per_bar;
   long nBars;
   Float64 fy;
   Float64 fu;
   Float64 Es;
   Float64 esh;
   Float64 efr;

   Float64 AsSpiral;
   Float64 db;
   Float64 S;
   Float64 FySpiral;
   Float64 esu;

   long nSlices;
   Float64 initialStep;
   Float64 P;
   Float64 NASlope;
};
