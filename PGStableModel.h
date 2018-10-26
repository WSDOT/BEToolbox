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

// CPGStableEngine document
#include "BEToolboxLib.h"

#include "PGStableStrands.h"
#include "PGStableLiftingCriteria.h"
#include "PGStableHaulingCriteria.h"

// Girder Type
#define PRISMATIC 0
#define NONPRISMATIC 1

// Model Type
#define LIFTING 0
#define HAULING 1

// Stress Point Type
#define COMPUTE_STRESS_POINTS 0
#define DEFINE_STRESS_POINTS  1

class BETCLASS CPGStableModel
{
public:
	CPGStableModel();
	virtual ~CPGStableModel();

   stbLiftingCheckArtifact GetLiftingCheckArtifact() const;
   stbHaulingCheckArtifact GetHaulingCheckArtifact() const;
   stbLiftingResults GetLiftingResults() const;
   stbHaulingResults GetHaulingResults() const;

   // NOTE: For all the "Get" methods.... returns true if the value changed and false if it didn't

   bool SetDensity(Float64 density);
   Float64 GetDensity() const;

   Float64 GetDensityWithRebar() const;
   bool SetDensityWithRebar(Float64 density);

   bool SetGirderType(int girderType);
   int GetGirderType() const;

   bool SetStressPointType(int stressPointType);
   int GetStressPointType() const;

   bool SetStrands(int girderType,int modelType,const CPGStableStrands& strands);
   const CPGStableStrands& GetStrands(int girderType,int modelType) const;

   bool SetGirder(int girderType,const stbGirder& girder);
   const stbGirder& GetGirder(int girderType) const;

   bool SetLiftingStabilityProblem(const stbLiftingStabilityProblem& problem);
   const stbLiftingStabilityProblem& GetLiftingStabilityProblem() const;

   bool SetHaulingStabilityProblem(const stbHaulingStabilityProblem& problem);
   const stbHaulingStabilityProblem& GetHaulingStabilityProblem() const;

   bool SetLiftingCriteria(const CPGStableLiftingCriteria& criteria);
   const CPGStableLiftingCriteria& GetLiftingCriteria() const;

   bool SetHaulingCriteria(const CPGStableHaulingCriteria& criteria);
   const CPGStableHaulingCriteria& GetHaulingCriteria() const;

   Float64 GetK1() const;
   bool SetK1(Float64 k1);
   Float64 GetK2() const;
   bool SetK2(Float64 k2);

   void GetLiftingMaterials(Float64* pFci,bool* pbComputeEci,Float64* pFrCoefficient) const;
   bool SetLiftingMaterials(Float64 fci,bool bComputeEci,Float64 frCoefficient);

   void GetHaulingMaterials(Float64* pFc,bool* pbComputeEc,Float64* pFrCoefficient) const;
   bool SetHaulingMaterials(Float64 fc,bool bComputeEc,Float64 frCoefficient);

   Float64 GetHeightOfGirderBottomAboveRoadway() const;
   bool SetHeightOfGirderBottomAboveRoadway(Float64 Hgb);

   void ResolveStrandLocations(const CPGStableStrands& strands,const stbGirder& girder,Float64* pXs,Float64* pYs,Float64* pXh,Float64* pXh1,Float64* pYh1,Float64* pXh2,Float64* pYh2,Float64* pXh3,Float64* pYh3,Float64* pXh4,Float64* pYh4,Float64* pXt,Float64* pYt);
   void GetStrandProfiles(const CPGStableStrands& strands,const stbGirder& girder,std::vector<std::pair<Float64,Float64>>* pvStraight,std::vector<std::pair<Float64,Float64>>* pvHarped,std::vector<std::pair<Float64,Float64>>* pvTemp) const;

   HRESULT Save(IStructuredSave* pStrSave);
   HRESULT Load(IStructuredLoad* pStrLoad);

protected:
   int m_GirderType;
   int m_StressPointType;

   CPGStableStrands m_Strands[2][2]; // array index [girderType][modelType]
   mutable stbGirder m_Girder[2];

   mutable stbLiftingStabilityProblem m_LiftingStabilityProblem;
   mutable stbHaulingStabilityProblem m_HaulingStabilityProblem;

   mutable CPGStableLiftingCriteria m_LiftingCriteria;
   mutable CPGStableHaulingCriteria m_HaulingCriteria;

   Float64 m_K1; // averaging factor for computing Ec
   Float64 m_K2; // bounding factor for computing Ec
   Float64 m_LiftingFrCoefficient;
   Float64 m_HaulingFrCoefficient;
   bool m_bComputeEci;
   bool m_bComputeEc;

   Float64 m_Hgb; // height of the girder bottom above the roadway

   CComPtr<IUnitServer> m_DocUnitServer;
   CComPtr<IUnitConvert> m_DocConvert;


   void ResolveLiftingStrandLocations() const;
   void ResolveHaulingStrandLocations() const;
   Float64 GetHarpedStrandLocation(Float64 X,Float64 X1,Float64 Y1,Float64 X2,Float64 Y2,Float64 X3,Float64 Y3,Float64 X4,Float64 Y4) const;

   void ResolveSimplifedLiftingStrandLocations() const;
   void ResolveExactLiftingStrandLocations() const;
   void ResolveSimplifedHaulingStrandLocations() const;
   void ResolveExactHaulingStrandLocations() const;
   void MapSimplifiedToExactStrandLocations(CPGStableStrands* pStrands);
   void GetSimplifiedStrandLocations(const CPGStableStrands* pStrands,const stbGirder* pGirder,Float64* pXpsStraight,Float64* pYpsStraight,Float64* pXpsHarped,Float64* pXh1,Float64* pYh1,Float64* pXh2,Float64* pYh2,Float64* pXh3,Float64* pYh3,Float64* pXh4,Float64* pYh4,Float64* pXpsTemp,Float64* pYpsTemp) const;
   void GetStrandLocations(const CPGStableFpe& fpe,const stbGirder* pGirder,Float64* pXs,Float64* pYs,Float64* pXh,Float64* pYh,Float64* pXt,Float64* pYt) const;

};
