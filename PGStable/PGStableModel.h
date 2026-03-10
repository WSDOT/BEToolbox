///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2025  Washington State Department of Transportation
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
#include "..\BEToolboxLib.h"

#include "PGStableStrands.h"
#include "PGStableLiftingCriteria.h"
#include "PGStableHaulingCriteria.h"
#include "PGStableOneEndSeatedCriteria.h"
#include "PGStableLongitudinalRebarData.h"

// Girder Type
enum GirderType
{
   Prismatic,
   Nonprismatic
};

// Model Type
enum ModelType
{
   Lifting,
   Hauling,
   OneEndSeated
};

// Stress Point Type
#define COMPUTE_STRESS_POINTS 0
#define DEFINE_STRESS_POINTS  1

class BETCLASS CPGStableModel
{
public:
	CPGStableModel();
	virtual ~CPGStableModel();

   WBFL::Stability::LiftingCheckArtifact GetLiftingCheckArtifact() const;
   WBFL::Stability::HaulingCheckArtifact GetHaulingCheckArtifact() const;
   WBFL::Stability::OneEndSeatedCheckArtifact GetOneEndSeatedCheckArtifact() const;

   WBFL::Stability::LiftingResults GetLiftingResults() const;
   WBFL::Stability::HaulingResults GetHaulingResults() const;
   WBFL::Stability::OneEndSeatedResults GetOneEndSeatedResults() const;

   // NOTE: For all the "Get" methods.... returns true if the value changed and false if it didn't

   bool SetConcreteType(WBFL::Materials::ConcreteType type);
   WBFL::Materials::ConcreteType GetConcreteType() const;

   bool SetDensity(Float64 density);
   Float64 GetDensity() const;

   Float64 GetDensityWithRebar() const;
   bool SetDensityWithRebar(Float64 density);

   bool SetGirderType(GirderType girderType);
   GirderType GetGirderType() const;

   bool SetStressPointType(int stressPointType);
   int GetStressPointType() const;

   bool SetStrands(GirderType girderType,ModelType modelType,const CPGStableStrands& strands);
   const CPGStableStrands& GetStrands(GirderType girderType,ModelType modelType) const;

   bool SetGirder(GirderType girderType,const WBFL::Stability::Girder& girder);
   const WBFL::Stability::Girder& GetGirder(GirderType girderType) const;

   bool SetLiftingStabilityProblem(const WBFL::Stability::LiftingStabilityProblem& problem);
   const WBFL::Stability::LiftingStabilityProblem& GetLiftingStabilityProblem() const;

   bool SetHaulingStabilityProblem(const WBFL::Stability::HaulingStabilityProblem& problem);
   const WBFL::Stability::HaulingStabilityProblem& GetHaulingStabilityProblem() const;

   bool SetOneEndSeatedStabilityProblem(const WBFL::Stability::OneEndSeatedStabilityProblem& problem);
   const WBFL::Stability::OneEndSeatedStabilityProblem& GetOneEndSeatedStabilityProblem() const;

   bool SetLiftingCriteria(const CPGStableLiftingCriteria& criteria);
   const CPGStableLiftingCriteria& GetLiftingCriteria() const;

   bool SetHaulingCriteria(const CPGStableHaulingCriteria& criteria);
   const CPGStableHaulingCriteria& GetHaulingCriteria() const;

   bool SetOneEndSeatedCriteria(const CPGStableOneEndSeatedCriteria& criteria);
   const CPGStableOneEndSeatedCriteria& GetOneEndSeatedCriteria() const;

   Float64 GetK1() const;
   bool SetK1(Float64 k1);
   Float64 GetK2() const;
   bool SetK2(Float64 k2);

   void GetLiftingMaterials(Float64* pFci,bool* pbComputeEci,Float64* pFrCoefficient) const;
   bool SetLiftingMaterials(Float64 fci,bool bComputeEci,Float64 frCoefficient);

   void GetHaulingMaterials(Float64* pFc, bool* pbComputeEc, Float64* pFrCoefficient) const;
   bool SetHaulingMaterials(Float64 fc, bool bComputeEc, Float64 frCoefficient);

   void GetOneEndSeatedMaterials(Float64* pFc, bool* pbComputeEc, Float64* pFrCoefficient) const;
   bool SetOneEndSeatedMaterials(Float64 fc, bool bComputeEc, Float64 frCoefficient);

   Float64 GetHeightOfGirderBottomAboveRoadway() const;
   bool SetHeightOfGirderBottomAboveRoadway(Float64 Hgb);

   void SetPGStableLongitudinalRebarData(const CPGStableLongitudinalRebarData& rData);
   CPGStableLongitudinalRebarData GetPGStableLongitudinalRebarData() const;

   void ResolveStrandLocations(const CPGStableStrands& strands,const WBFL::Stability::Girder& girder,Float64* pXs,Float64* pYs,Float64* pXh,Float64* pXh1,Float64* pYh1,Float64* pXh2,Float64* pYh2,Float64* pXh3,Float64* pYh3,Float64* pXh4,Float64* pYh4,Float64* pXt,Float64* pYt);
   void GetStrandProfiles(const CPGStableStrands& strands,const WBFL::Stability::Girder& girder,std::vector<std::pair<Float64,Float64>>* pvStraight,std::vector<std::pair<Float64,Float64>>* pvHarped,std::vector<std::pair<Float64,Float64>>* pvTemp) const;

   HRESULT Save(IStructuredSave* pStrSave);
   HRESULT Load(IStructuredLoad* pStrLoad);

protected:
   GirderType m_GirderType;
   int m_StressPointType;
   int m_ConcreteType;

   std::array<std::array<CPGStableStrands,3>,2> m_Strands; // array index [girderType][modelType]
   mutable std::array<WBFL::Stability::Girder, 2> m_Girder; // array index [girderType]

   mutable WBFL::Stability::LiftingStabilityProblem m_LiftingStabilityProblem;
   mutable WBFL::Stability::HaulingStabilityProblem m_HaulingStabilityProblem;
   mutable WBFL::Stability::OneEndSeatedStabilityProblem m_OneEndSeatedStabilityProblem;

   mutable CPGStableLiftingCriteria m_LiftingCriteria;
   mutable CPGStableHaulingCriteria m_HaulingCriteria;
   mutable CPGStableOneEndSeatedCriteria m_OneEndSeatedCriteria;

   Float64 m_K1; // averaging factor for computing Ec
   Float64 m_K2; // bounding factor for computing Ec
   Float64 m_LiftingFrCoefficient;
   Float64 m_HaulingFrCoefficient;
   Float64 m_OneEndSeatedFrCoefficient;
   bool m_bComputeEci;
   bool m_bComputeEc;
   bool m_bComputeEcOneEndSeated;

   Float64 m_Hgb; // height of the girder bottom above the roadway

   CPGStableLongitudinalRebarData m_LongitudinalRebarData;

   CComPtr<IUnitServer> m_DocUnitServer;
   CComPtr<IUnitConvert> m_DocConvert;


   void ResolveLiftingStrandLocations() const;
   void ResolveHaulingStrandLocations() const;
   void ResolveOneEndSeatedStrandLocations() const;
   Float64 GetHarpedStrandLocation(Float64 X,Float64 X1,Float64 Y1,Float64 X2,Float64 Y2,Float64 X3,Float64 Y3,Float64 X4,Float64 Y4) const;

   void ResolveSimplifedLiftingStrandLocations() const;
   void ResolveExactLiftingStrandLocations() const;
   void ResolveSimplifedHaulingStrandLocations() const;
   void ResolveExactHaulingStrandLocations() const;
   void ResolveSimplifedOneEndSeatedStrandLocations() const;
   void ResolveExactOneEndSeatedStrandLocations() const;
   void MapSimplifiedToExactStrandLocations(CPGStableStrands* pStrands);
   void GetSimplifiedStrandLocations(const CPGStableStrands* pStrands,const WBFL::Stability::Girder* pGirder,Float64* pXpsStraight,Float64* pYpsStraight,Float64* pXpsHarped,Float64* pXh1,Float64* pYh1,Float64* pXh2,Float64* pYh2,Float64* pXh3,Float64* pYh3,Float64* pXh4,Float64* pYh4,Float64* pXpsTemp,Float64* pYpsTemp) const;
   void GetStrandLocations(const CPGStableFpe& fpe,const WBFL::Stability::Girder* pGirder,Float64* pXs,Float64* pYs,Float64* pXh,Float64* pYh,Float64* pXt,Float64* pYt) const;

   // Return true if UI input shows longitudinal rebar input
   bool DoUseTensileLongRebar() const;
};
