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

#pragma once

// CPGStableEngine document
#include "PGStableStrands.h"
#include "PGStableCriteria.h"
#include <Stability\Stability.h>

#define PRISMATIC 0
#define NONPRISMATIC 1

#define CONSTANT_FPE 0
#define VARYING_FPE  1

class CPGStableModel
{
public:
	CPGStableModel();
	virtual ~CPGStableModel();

   stbLiftingCheckArtifact GetLiftingCheckArtifact() const;
   stbHaulingCheckArtifact GetHaulingCheckArtifact() const;
   stbLiftingResults GetLiftingResults() const;
   stbHaulingResults GetHaulingResults() const;
   void ResolveStrandLocations(const CPGStableStrands* pStrands,stbGirder* pGirder) const;

   HRESULT Save(IStructuredSave* pStrSave);
   HRESULT Load(IStructuredLoad* pStrLoad);

   int m_GirderType;

   CPGStableStrands m_Strands[2];
   mutable stbGirder m_Girder[2];

   mutable int m_LiftingFpeType;
   mutable int m_HaulingFpeType;
   mutable stbLiftingStabilityProblem m_LiftingStabilityProblem;
   mutable stbHaulingStabilityProblem m_HaulingStabilityProblem;

   mutable CPGStableCriteria m_LiftingCriteria;
   mutable CPGStableCriteria m_HaulingCriteria;

   Float64 m_Density; // density of plain concrete, used to compute modulus of elasticity
   Float64 m_K1; // averaging factor for computing Ec
   Float64 m_K2; // bounding factor for computing Ec
   Float64 m_LiftingFrCoefficient;
   Float64 m_HaulingFrCoefficient;
   Float64 m_Fci;
   bool m_bComputeEci;
   Float64 m_Fc;
   bool m_bComputeEc;

   Float64 m_Hgb; // height of the girder bottom above the roadway

};
