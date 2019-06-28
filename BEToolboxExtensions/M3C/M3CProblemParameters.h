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

#pragma once

#include <LRFD\LRFD.h>

class CM3CProblemParameters
{
public:
   CM3CProblemParameters();

   bool operator==(const CM3CProblemParameters& other) const;
   bool operator!=(const CM3CProblemParameters& other) const;

   HRESULT Save(IStructuredSave* pStrSave);
   HRESULT Load(IStructuredLoad* pStrLoad);

   // Column Section
   Float64 D; // Circular column defined by diameter

   // Confined Concrete Parameters (Mander's model)
   Float64 fco;
   Float64 eco;

   // Confining Spiral Reinforcement
   Float64 AsSpiral;
   Float64 db;
   Float64 S;
   Float64 FySpiral;
   Float64 esu;

   // Longitudinal Reinforcement
   Float64 fy;
   Float64 fu;
   Float64 Es;
   Float64 esh;
   Float64 efr;
   Float64 Cover;

   // Bonded Reinforcement
   Float64 Bonded_Rebar_As_per_bar;
   long Bonded_Rebar_nBars;

   // Unbonded Reinforcement
   Float64 Unbonded_Rebar_As_per_bar;
   long Unbonded_Rebar_nBars;
   Float64 Unbonded_Rebar_Lu;

   // Unbonded Tendons
   const matPsStrand* pStrand;
   Float64 Tendon_Ring_Diameter;
   long Tendon_nStrands;
   Float64 Tendon_fpe;
   Float64 Tendon_Lu;

   // Analysis Controls
   long nSlices;
   Float64 initialStep;
   Float64 P;
};
