///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2024  Washington State Department of Transportation
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

// PGStableModel.cpp : implementation file
//

#include "stdafx.h"
#include "PGStableModel.h"
#include <EAF\EAFApp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CPGStableModel::CPGStableModel()
{
   m_GirderType = GirderType::Prismatic;
   m_StressPointType = COMPUTE_STRESS_POINTS;

   Float64 Hg  = WBFL::Units::ConvertToSysUnits(72,WBFL::Units::Measure::Inch);
   Float64 Wtf = WBFL::Units::ConvertToSysUnits(42,WBFL::Units::Measure::Inch);
   Float64 Wbf = WBFL::Units::ConvertToSysUnits(26,WBFL::Units::Measure::Inch);
   
   Float64 Ag = WBFL::Units::ConvertToSysUnits(767,WBFL::Units::Measure::Inch2);
   Float64 Ixx = WBFL::Units::ConvertToSysUnits(545894,WBFL::Units::Measure::Inch4);
   Float64 Iyy = WBFL::Units::ConvertToSysUnits(37634,WBFL::Units::Measure::Inch4);
   Float64 Ixy = 0.0;
   Float64 Xleft = Wtf / 2;
   Float64 Ytop = WBFL::Units::ConvertToSysUnits(36.6-72,WBFL::Units::Measure::Inch); // want neg because we are in section coordinates
   Float64 L = WBFL::Units::ConvertToSysUnits(100,WBFL::Units::Measure::Feet);
   m_Girder[GirderType::Prismatic].AddSection(L,Ag,Ixx,Iyy,Ixy,Xleft,Ytop,Hg,Wtf,Wbf);

   m_Girder[GirderType::Nonprismatic] = m_Girder[GirderType::Prismatic];

   WBFL::Materials::SimpleConcrete liftingConcrete, haulingConcrete, oneEndSeatedConcrete;

   Float64 density = WBFL::Units::ConvertToSysUnits(0.155,WBFL::Units::Measure::KipPerFeet3); // without rebar (used to compute Ec)
   Float64 densityWithRebar = WBFL::Units::ConvertToSysUnits(0.165,WBFL::Units::Measure::KipPerFeet3); // including allowance for rebar (used for computing dead load)
   liftingConcrete.SetDensity(density);
   haulingConcrete.SetDensity(density);
   oneEndSeatedConcrete.SetDensity(density);

   liftingConcrete.SetDensityForWeight(densityWithRebar);
   haulingConcrete.SetDensityForWeight(densityWithRebar);
   oneEndSeatedConcrete.SetDensityForWeight(densityWithRebar);

   m_LiftingFrCoefficient = WBFL::Units::ConvertToSysUnits(0.24,WBFL::Units::Measure::SqrtKSI);
   m_HaulingFrCoefficient = WBFL::Units::ConvertToSysUnits(0.24, WBFL::Units::Measure::SqrtKSI);
   m_OneEndSeatedFrCoefficient = WBFL::Units::ConvertToSysUnits(0.24, WBFL::Units::Measure::SqrtKSI);

   m_bComputeEci = true;
   m_bComputeEc = true;
   m_bComputeEcOneEndSeated = true;

   Float64 fci = WBFL::Units::ConvertToSysUnits(5.5,WBFL::Units::Measure::KSI);
   Float64 fc  = WBFL::Units::ConvertToSysUnits(7.0,WBFL::Units::Measure::KSI);
   m_K1 = 1.0;
   m_K2 = 1.0;

   liftingConcrete.SetFc(fci);
   haulingConcrete.SetFc(fc);
   oneEndSeatedConcrete.SetFc(fc);

   Float64 Eci = ::WBFL::LRFD::ConcreteUtil::ModE(liftingConcrete.GetType(),fci,density,false/*ignore LRFD range checks*/);
   Float64 Ec  = ::WBFL::LRFD::ConcreteUtil::ModE(liftingConcrete.GetType(), fc, density,false/*ignore LRFD range checks*/);

   liftingConcrete.SetE(Eci);
   haulingConcrete.SetE(Ec);
   oneEndSeatedConcrete.SetE(Ec);

   liftingConcrete.SetFlexureFr(WBFL::Units::ConvertToSysUnits(0.24*sqrt(fci),WBFL::Units::Measure::KSI));
   haulingConcrete.SetFlexureFr(WBFL::Units::ConvertToSysUnits(0.24*sqrt(fc),WBFL::Units::Measure::KSI));
   oneEndSeatedConcrete.SetFlexureFr(WBFL::Units::ConvertToSysUnits(0.24 * sqrt(fc), WBFL::Units::Measure::KSI));

   m_LiftingStabilityProblem.SetConcrete(liftingConcrete);
   m_HaulingStabilityProblem.SetConcrete(haulingConcrete);
   m_OneEndSeatedStabilityProblem.SetConcrete(oneEndSeatedConcrete);

   m_LiftingStabilityProblem.SetCamber(WBFL::Units::ConvertToSysUnits(0,WBFL::Units::Measure::Inch));
   m_LiftingStabilityProblem.SetLateralCamber(0.0);
   m_LiftingStabilityProblem.IncludeLateralRollAxisOffset(false);

   m_LiftingStabilityProblem.SetSweepTolerance(WBFL::Units::ConvertToSysUnits(0.125,WBFL::Units::Measure::Inch)/WBFL::Units::ConvertToSysUnits(10.0,WBFL::Units::Measure::Feet));
   m_LiftingStabilityProblem.SetSupportPlacementTolerance( WBFL::Units::ConvertToSysUnits(0.25,WBFL::Units::Measure::Inch) );
   m_LiftingStabilityProblem.SetSupportLocations(WBFL::Units::ConvertToSysUnits(3,WBFL::Units::Measure::Feet),WBFL::Units::ConvertToSysUnits(3,WBFL::Units::Measure::Feet));
   m_LiftingStabilityProblem.SetLiftAngle(PI_OVER_2);
   m_LiftingStabilityProblem.SetYRollAxis(WBFL::Units::ConvertToSysUnits(0.0,WBFL::Units::Measure::Inch));
   m_LiftingStabilityProblem.SetImpact(0,0);

   ///////////////////////////////////////////////////

   m_HaulingStabilityProblem.SetCamber(WBFL::Units::ConvertToSysUnits(0,WBFL::Units::Measure::Inch));
   m_HaulingStabilityProblem.SetLateralCamber(0.0);
   m_HaulingStabilityProblem.IncludeLateralRollAxisOffset(false);

   m_HaulingStabilityProblem.SetSupportLocations(WBFL::Units::ConvertToSysUnits(5,WBFL::Units::Measure::Feet),WBFL::Units::ConvertToSysUnits(5,WBFL::Units::Measure::Feet));
   m_HaulingStabilityProblem.SetSweepTolerance(WBFL::Units::ConvertToSysUnits(0.125,WBFL::Units::Measure::Inch)/WBFL::Units::ConvertToSysUnits(10.0,WBFL::Units::Measure::Feet));
   m_HaulingStabilityProblem.SetSupportPlacementTolerance(WBFL::Units::ConvertToSysUnits(1.0,WBFL::Units::Measure::Inch));
   m_HaulingStabilityProblem.SetRotationalStiffness(WBFL::Units::ConvertToSysUnits(40500.,WBFL::Units::Measure::KipInchPerRadian));
   m_HaulingStabilityProblem.SetSupportSlope(0.02);
   m_HaulingStabilityProblem.SetSuperelevation(0.06);
   m_HaulingStabilityProblem.SetSupportWidth(WBFL::Units::ConvertToSysUnits(72.,WBFL::Units::Measure::Inch));
   m_HaulingStabilityProblem.SetHeightOfRollAxis(WBFL::Units::ConvertToSysUnits(24.,WBFL::Units::Measure::Inch));
   m_HaulingStabilityProblem.SetImpact(0,0);
   m_HaulingStabilityProblem.SetImpactUsage(WBFL::Stability::HaulingImpact::NormalCrown);
   m_HaulingStabilityProblem.SetVelocity(0);
   m_HaulingStabilityProblem.SetTurningRadius(WBFL::Units::ConvertToSysUnits(100,WBFL::Units::Measure::Feet));

   ///////////////////////////////////////////////////

   m_OneEndSeatedStabilityProblem.SetCamber(WBFL::Units::ConvertToSysUnits(0, WBFL::Units::Measure::Inch));
   m_OneEndSeatedStabilityProblem.SetLateralCamber(0.0);
   m_OneEndSeatedStabilityProblem.IncludeLateralRollAxisOffset(false);

   m_OneEndSeatedStabilityProblem.SetSupportLocations(WBFL::Units::ConvertToSysUnits(5, WBFL::Units::Measure::Feet), WBFL::Units::ConvertToSysUnits(5, WBFL::Units::Measure::Feet));
   m_OneEndSeatedStabilityProblem.SetSweepTolerance(WBFL::Units::ConvertToSysUnits(0.125, WBFL::Units::Measure::Inch) / WBFL::Units::ConvertToSysUnits(10.0, WBFL::Units::Measure::Feet));
   m_OneEndSeatedStabilityProblem.SetSupportPlacementTolerance(WBFL::Units::ConvertToSysUnits(1.0, WBFL::Units::Measure::Inch));
   m_OneEndSeatedStabilityProblem.SetRotationalStiffness(WBFL::Units::ConvertToSysUnits(40500., WBFL::Units::Measure::KipInchPerRadian));
   m_OneEndSeatedStabilityProblem.SetSupportSlope(0.02);
   m_OneEndSeatedStabilityProblem.SetSupportWidth(WBFL::Units::ConvertToSysUnits(72., WBFL::Units::Measure::Inch));
   m_OneEndSeatedStabilityProblem.SetHeightOfRollAxis(WBFL::Units::ConvertToSysUnits(24., WBFL::Units::Measure::Inch));
   m_OneEndSeatedStabilityProblem.SetImpact(0, 0);

   m_Hgb = WBFL::Units::ConvertToSysUnits(72.0,WBFL::Units::Measure::Inch);

   auto* pLiftingTensionStressLimit = new WBFL::Stability::CCLiftingTensionStressLimit;
   m_LiftingCriteria.TensionStressLimit.reset(pLiftingTensionStressLimit);
   pLiftingTensionStressLimit->bMaxTension = true;

   auto* pHaulingTensionStressLimit = new WBFL::Stability::CCHaulingTensionStressLimit;
   m_HaulingCriteria.TensionStressLimit.reset(pHaulingTensionStressLimit);
   pHaulingTensionStressLimit->bMaxTension[+WBFL::Stability::HaulingSlope::CrownSlope] = true;

   auto* pOneEndSeatedTensionStressLimit = new WBFL::Stability::CCOneEndSeatedTensionStressLimit;
   m_OneEndSeatedCriteria.TensionStressLimit.reset(pOneEndSeatedTensionStressLimit);
   pOneEndSeatedTensionStressLimit->bMaxTension = true;

   m_DocUnitServer.CoCreateInstance(CLSID_UnitServer);
   m_DocUnitServer->SetSystemUnits(CComBSTR(WBFL::Units::System::GetMassUnit().UnitTag().c_str()),
                            CComBSTR(WBFL::Units::System::GetLengthUnit().UnitTag().c_str()),
                            CComBSTR(WBFL::Units::System::GetTimeUnit().UnitTag().c_str()),
                            CComBSTR(WBFL::Units::System::GetTemperatureUnit().UnitTag().c_str()),
                            CComBSTR(WBFL::Units::System::GetAngleUnit().UnitTag().c_str()));  
   m_DocUnitServer->QueryInterface(&m_DocConvert);


   for ( int girderType = 0; girderType < 2; girderType++ )
   {
      for ( int modelType = 0; modelType < 2; modelType++ )
      {
         MapSimplifiedToExactStrandLocations(&m_Strands[girderType][modelType]);
      }
   }
}

CPGStableModel::~CPGStableModel()
{
}


WBFL::Stability::LiftingResults CPGStableModel::GetLiftingResults() const
{
   WBFL::Stability::LiftingCheckArtifact artifact = GetLiftingCheckArtifact();
   return artifact.GetLiftingResults();
}

WBFL::Stability::LiftingCheckArtifact CPGStableModel::GetLiftingCheckArtifact() const
{
   WBFL::Materials::SimpleConcrete concrete = m_LiftingStabilityProblem.GetConcrete();
   Float64 fci = concrete.GetFc();
   if ( m_bComputeEci )
   {
      Float64 density = concrete.GetDensity();
      Float64 Ec = WBFL::LRFD::ConcreteUtil::ModE(concrete.GetType(),fci,density,false/*ignore LRFD range checks*/);
      if ( WBFL::LRFD::BDSManager::Edition::ThirdEditionWith2005Interims <= WBFL::LRFD::BDSManager::GetEdition() )
      {
         Ec *= m_K1*m_K2;
      }
      concrete.SetE(Ec);
   }

   Float64 lambda = WBFL::LRFD::ConcreteUtil::ComputeConcreteDensityModificationFactor(concrete.GetType(), concrete.GetDensity(), false, 0, 0);
   concrete.SetLambda(lambda);

   auto* pTensionStressLimit = dynamic_cast<WBFL::Stability::CCLiftingTensionStressLimit*>(m_LiftingCriteria.TensionStressLimit.get());
   pTensionStressLimit->Lambda = concrete.GetLambda();

   Float64 fr = ::WBFL::LRFD::ConcreteUtil::ModRupture(fci,m_LiftingFrCoefficient);
   concrete.SetFlexureFr(lambda*fr);


   m_LiftingStabilityProblem.SetConcrete(concrete);

   WBFL::Stability::Girder* pGirder = &m_Girder[m_GirderType];
   Float64 L = pGirder->GetGirderLength();
   m_LiftingStabilityProblem.ClearAnalysisPoints();
   int n = 10;
   for ( int i = 0; i <= n; i++ )
   {
      Float64 X = i*L/n;
      m_LiftingStabilityProblem.AddAnalysisPoint(std::move(std::make_unique<WBFL::Stability::AnalysisPoint>(X)));
   }

   // when the lifting cables are inclined, the moment diagram due to the horizontal component of the lift force
   // jumps from zero to some value at the lift point. We need to add analysis points just outside of the lift
   // point to capture the effect of the jump.
   bool bAddExtraLiftPointAnalysisPoints(IsEqual(m_LiftingStabilityProblem.GetLiftAngle(), PI_OVER_2) ? false : true);

   Float64 Ll, Lr;
   m_LiftingStabilityProblem.GetSupportLocations(&Ll,&Lr);
   Float64 offset = WBFL::Units::ConvertToSysUnits(0.001, WBFL::Units::Measure::Feet);

   if (bAddExtraLiftPointAnalysisPoints)
   {
      m_LiftingStabilityProblem.AddAnalysisPoint(std::move(std::make_unique<WBFL::Stability::AnalysisPoint>(Ll - offset)));
   }

   m_LiftingStabilityProblem.AddAnalysisPoint(std::move(std::make_unique<WBFL::Stability::AnalysisPoint>(Ll)));
   m_LiftingStabilityProblem.AddAnalysisPoint(std::move(std::make_unique<WBFL::Stability::AnalysisPoint>(L - Lr)));

   if (bAddExtraLiftPointAnalysisPoints)
   {
      m_LiftingStabilityProblem.AddAnalysisPoint(std::move(std::make_unique<WBFL::Stability::AnalysisPoint>(L - Lr + offset)));
   }

   if (!IsEqual(Ll, m_Strands[m_GirderType][ModelType::Lifting].XferLength))
   {
      m_LiftingStabilityProblem.AddAnalysisPoint(std::move(std::make_unique<WBFL::Stability::AnalysisPoint>(m_Strands[m_GirderType][ModelType::Lifting].XferLength)));
   }
   if (!IsEqual(Lr, m_Strands[m_GirderType][ModelType::Lifting].XferLength))
   {
      m_LiftingStabilityProblem.AddAnalysisPoint(std::move(std::make_unique<WBFL::Stability::AnalysisPoint>(L - m_Strands[m_GirderType][ModelType::Lifting].XferLength)));
   }

   ResolveLiftingStrandLocations();

   // criteria
   m_LiftingCriteria.AllowableCompression_GlobalStress = -m_LiftingCriteria.CompressionCoefficient_GlobalStress*fci;
   m_LiftingCriteria.AllowableCompression_PeakStress   = -m_LiftingCriteria.CompressionCoefficient_PeakStress*fci;
   pTensionStressLimit->AllowableTension = pTensionStressLimit->Lambda * pTensionStressLimit->TensionCoefficient*sqrt(fci);
   if (pTensionStressLimit->bMaxTension )
   {
      pTensionStressLimit->AllowableTension = Min(pTensionStressLimit->AllowableTension, pTensionStressLimit->MaxTension);
   }
   pTensionStressLimit->AllowableTensionWithRebar = pTensionStressLimit->Lambda * pTensionStressLimit->TensionCoefficientWithRebar*sqrt(fci);

   WBFL::Stability::StabilityEngineer stabilityEngineer;
   WBFL::Stability::LiftingCheckArtifact artifact = stabilityEngineer.CheckLifting(pGirder,&m_LiftingStabilityProblem,m_LiftingCriteria);
   return artifact;
}

WBFL::Stability::HaulingResults CPGStableModel::GetHaulingResults() const
{
   WBFL::Stability::HaulingCheckArtifact artifact = GetHaulingCheckArtifact();
   return artifact.GetHaulingResults();
}

WBFL::Stability::OneEndSeatedResults CPGStableModel::GetOneEndSeatedResults() const
{
   WBFL::Stability::OneEndSeatedCheckArtifact artifact = GetOneEndSeatedCheckArtifact();
   return artifact.GetOneEndSeatedResults();
}

WBFL::Stability::HaulingCheckArtifact CPGStableModel::GetHaulingCheckArtifact() const
{
   WBFL::Materials::SimpleConcrete concrete = m_HaulingStabilityProblem.GetConcrete();
   Float64 fc = concrete.GetFc();
   if ( m_bComputeEc )
   {
      Float64 density = concrete.GetDensity();
      Float64 Ec = ::WBFL::LRFD::ConcreteUtil::ModE(concrete.GetType(),fc,density,false/*ignore LRFD range checks*/);
      if ( WBFL::LRFD::BDSManager::Edition::ThirdEditionWith2005Interims <= WBFL::LRFD::BDSManager::GetEdition() )
      {
         Ec *= m_K1*m_K2;
      }

      concrete.SetE(Ec);
   }

   Float64 lambda = WBFL::LRFD::ConcreteUtil::ComputeConcreteDensityModificationFactor(concrete.GetType(), concrete.GetDensity(), false, 0, 0);
   concrete.SetLambda(lambda);

   auto* pTensionStressLimit = dynamic_cast<WBFL::Stability::CCHaulingTensionStressLimit*>(m_HaulingCriteria.TensionStressLimit.get());
   pTensionStressLimit->Lambda = concrete.GetLambda();

   Float64 fr = ::WBFL::LRFD::ConcreteUtil::ModRupture(fc,m_HaulingFrCoefficient);
   concrete.SetFlexureFr(lambda*fr);


   m_HaulingStabilityProblem.SetConcrete(concrete);

   WBFL::Stability::Girder* pGirder = &m_Girder[m_GirderType];
   Float64 L = pGirder->GetGirderLength();
   m_HaulingStabilityProblem.ClearAnalysisPoints();
   int n = 10;
   for ( int i = 0; i <= n; i++ )
   {
      Float64 X = i*L/n;
      m_HaulingStabilityProblem.AddAnalysisPoint(std::move(std::make_unique<WBFL::Stability::AnalysisPoint>(X)));
   }

   Float64 Ll, Lr;
   m_HaulingStabilityProblem.GetSupportLocations(&Ll,&Lr);
   m_HaulingStabilityProblem.AddAnalysisPoint(std::move(std::make_unique<WBFL::Stability::AnalysisPoint>(Ll)));
   m_HaulingStabilityProblem.AddAnalysisPoint(std::move(std::make_unique<WBFL::Stability::AnalysisPoint>(L-Lr)));

   if (!IsEqual(Ll, m_Strands[m_GirderType][ModelType::Lifting].XferLength))
   {
      m_HaulingStabilityProblem.AddAnalysisPoint(std::move(std::make_unique<WBFL::Stability::AnalysisPoint>(m_Strands[m_GirderType][ModelType::Hauling].XferLength)));
   }
   if (!IsEqual(Lr, m_Strands[m_GirderType][ModelType::Lifting].XferLength))
   {
      m_HaulingStabilityProblem.AddAnalysisPoint(std::move(std::make_unique<WBFL::Stability::AnalysisPoint>(L - m_Strands[m_GirderType][ModelType::Hauling].XferLength)));
   }

   Float64 Ag,Ixx,Iyy,Ixy,Xleft,Ytop,Hg,Wtop,Wbot;
   pGirder->GetSectionProperties(0,WBFL::Stability::Section::Start,&Ag,&Ixx,&Iyy,&Ixy,&Xleft,&Ytop,&Hg,&Wtop,&Wbot);
   Float64 Yra = -(Hg + m_Hgb) + m_HaulingStabilityProblem.GetHeightOfRollAxis();
   ATLASSERT(Yra < 0);
   m_HaulingStabilityProblem.SetYRollAxis(Yra); // location of roll axes relative to the top of girder

   ResolveHaulingStrandLocations();

   m_HaulingCriteria.AllowableCompression_GlobalStress = -m_HaulingCriteria.CompressionCoefficient_GlobalStress*fc;
   m_HaulingCriteria.AllowableCompression_PeakStress = -m_HaulingCriteria.CompressionCoefficient_PeakStress*fc;

   pTensionStressLimit->AllowableTension[+WBFL::Stability::HaulingSlope::CrownSlope] = pTensionStressLimit->Lambda * pTensionStressLimit->TensionCoefficient[+WBFL::Stability::HaulingSlope::CrownSlope]*sqrt(fc);
   if (pTensionStressLimit->bMaxTension[+WBFL::Stability::HaulingSlope::CrownSlope] )
   {
      pTensionStressLimit->AllowableTension[+WBFL::Stability::HaulingSlope::CrownSlope] = Min(pTensionStressLimit->AllowableTension[+WBFL::Stability::HaulingSlope::CrownSlope], pTensionStressLimit->MaxTension[+WBFL::Stability::HaulingSlope::CrownSlope]);
   }
   pTensionStressLimit->AllowableTensionWithRebar[+WBFL::Stability::HaulingSlope::CrownSlope] = pTensionStressLimit->Lambda * pTensionStressLimit->TensionCoefficientWithRebar[+WBFL::Stability::HaulingSlope::CrownSlope]*sqrt(fc);


   pTensionStressLimit->AllowableTension[+WBFL::Stability::HaulingSlope::Superelevation] = concrete.GetFlexureFr();
   pTensionStressLimit->TensionCoefficient[+WBFL::Stability::HaulingSlope::Superelevation] = m_HaulingFrCoefficient;
   pTensionStressLimit->bMaxTension[+WBFL::Stability::HaulingSlope::Superelevation] = false;
   pTensionStressLimit->AllowableTensionWithRebar[+WBFL::Stability::HaulingSlope::Superelevation] = concrete.GetFlexureFr();
   pTensionStressLimit->TensionCoefficientWithRebar[+WBFL::Stability::HaulingSlope::Superelevation] = m_HaulingFrCoefficient;

   WBFL::Stability::StabilityEngineer stabilityEngineer;
   WBFL::Stability::HaulingCheckArtifact artifact = stabilityEngineer.CheckHauling(pGirder,&m_HaulingStabilityProblem,m_HaulingCriteria);
   return artifact;
}

WBFL::Stability::OneEndSeatedCheckArtifact CPGStableModel::GetOneEndSeatedCheckArtifact() const
{
   WBFL::Materials::SimpleConcrete concrete = m_OneEndSeatedStabilityProblem.GetConcrete();
   Float64 fc = concrete.GetFc();
   if (m_bComputeEc)
   {
      Float64 density = concrete.GetDensity();
      Float64 Ec = ::WBFL::LRFD::ConcreteUtil::ModE(concrete.GetType(), fc, density, false/*ignore LRFD range checks*/);
      if (WBFL::LRFD::BDSManager::Edition::ThirdEditionWith2005Interims <= WBFL::LRFD::BDSManager::GetEdition())
      {
         Ec *= m_K1 * m_K2;
      }

      concrete.SetE(Ec);
   }

   Float64 lambda = WBFL::LRFD::ConcreteUtil::ComputeConcreteDensityModificationFactor(concrete.GetType(), concrete.GetDensity(), false, 0, 0);
   concrete.SetLambda(lambda);

   auto* pTensionStressLimit = dynamic_cast<WBFL::Stability::CCOneEndSeatedTensionStressLimit*>(m_OneEndSeatedCriteria.TensionStressLimit.get());
   pTensionStressLimit->Lambda = concrete.GetLambda();

   Float64 fr = ::WBFL::LRFD::ConcreteUtil::ModRupture(fc, m_OneEndSeatedFrCoefficient);
   concrete.SetFlexureFr(lambda * fr);


   m_OneEndSeatedStabilityProblem.SetConcrete(concrete);

   WBFL::Stability::Girder* pGirder = &m_Girder[m_GirderType];
   Float64 L = pGirder->GetGirderLength();
   m_OneEndSeatedStabilityProblem.ClearAnalysisPoints();
   int n = 10;
   for (int i = 0; i <= n; i++)
   {
      Float64 X = i * L / n;
      m_OneEndSeatedStabilityProblem.AddAnalysisPoint(std::move(std::make_unique<WBFL::Stability::AnalysisPoint>(X)));
   }

   Float64 Ll, Lr;
   m_OneEndSeatedStabilityProblem.GetSupportLocations(&Ll, &Lr);
   m_OneEndSeatedStabilityProblem.AddAnalysisPoint(std::move(std::make_unique<WBFL::Stability::AnalysisPoint>(Ll)));
   m_OneEndSeatedStabilityProblem.AddAnalysisPoint(std::move(std::make_unique<WBFL::Stability::AnalysisPoint>(L - Lr)));

   if (!IsEqual(Ll, m_Strands[m_GirderType][ModelType::Lifting].XferLength))
   {
      m_OneEndSeatedStabilityProblem.AddAnalysisPoint(std::move(std::make_unique<WBFL::Stability::AnalysisPoint>(m_Strands[m_GirderType][ModelType::OneEndSeated].XferLength)));
   }
   if (!IsEqual(Lr, m_Strands[m_GirderType][ModelType::Lifting].XferLength))
   {
      m_OneEndSeatedStabilityProblem.AddAnalysisPoint(std::move(std::make_unique<WBFL::Stability::AnalysisPoint>(L - m_Strands[m_GirderType][ModelType::OneEndSeated].XferLength)));
   }

   Float64 Ag, Ixx, Iyy, Ixy, Xleft, Ytop, Hg, Wtop, Wbot;
   pGirder->GetSectionProperties(0, WBFL::Stability::Section::Start, &Ag, &Ixx, &Iyy, &Ixy, &Xleft, &Ytop, &Hg, &Wtop, &Wbot);
   Float64 Yra = -(Hg + m_Hgb) + m_OneEndSeatedStabilityProblem.GetHeightOfRollAxis();
   ATLASSERT(Yra < 0);
   m_OneEndSeatedStabilityProblem.SetYRollAxis(Yra); // location of roll axes relative to the top of girder

   ResolveOneEndSeatedStrandLocations();

   m_OneEndSeatedCriteria.AllowableCompression_GlobalStress = -m_OneEndSeatedCriteria.CompressionCoefficient_GlobalStress * fc;
   m_OneEndSeatedCriteria.AllowableCompression_PeakStress = -m_OneEndSeatedCriteria.CompressionCoefficient_PeakStress * fc;

   pTensionStressLimit->AllowableTension = pTensionStressLimit->Lambda * pTensionStressLimit->TensionCoefficient * sqrt(fc);
   if (pTensionStressLimit->bMaxTension)
   {
      pTensionStressLimit->AllowableTension = Min(pTensionStressLimit->AllowableTension, pTensionStressLimit->MaxTension);
   }
   pTensionStressLimit->AllowableTensionWithRebar = pTensionStressLimit->Lambda * pTensionStressLimit->TensionCoefficientWithRebar * sqrt(fc);

   WBFL::Stability::StabilityEngineer stabilityEngineer;
   WBFL::Stability::OneEndSeatedCheckArtifact artifact = stabilityEngineer.CheckOneEndSeated(pGirder, &m_OneEndSeatedStabilityProblem, m_OneEndSeatedCriteria);
   return artifact;
}

void CPGStableModel::ResolveLiftingStrandLocations() const
{
   if ( m_Strands[m_GirderType][ModelType::Lifting].strandMethod == CPGStableStrands::Simplified )
   {
      ResolveSimplifedLiftingStrandLocations();
   }
   else
   {
      ResolveExactLiftingStrandLocations();
   }
}

void CPGStableModel::ResolveSimplifedLiftingStrandLocations() const
{
   Float64 L = m_Girder[m_GirderType].GetGirderLength();
   m_LiftingStabilityProblem.AdjustForXferLength(true);
   m_LiftingStabilityProblem.SetXferLength(m_Strands[m_GirderType][ModelType::Lifting].XferLength,L);

   Float64 XpsStraight,YpsStraight,XpsHarped,Xh1,Yh1,Xh2,Yh2,Xh3,Yh3,Xh4,Yh4,XpsTemp,YpsTemp;
   GetSimplifiedStrandLocations(&m_Strands[m_GirderType][ModelType::Lifting],&m_Girder[m_GirderType],&XpsStraight,&YpsStraight,&XpsHarped,&Xh1,&Yh1,&Xh2,&Yh2,&Xh3,&Yh3,&Xh4,&Yh4,&XpsTemp,&YpsTemp);

   m_LiftingStabilityProblem.ClearFpe();

   Float64 FpeStraight = m_Strands[m_GirderType][ModelType::Lifting].FpeStraight;
   Float64 FpeHarped   = m_Strands[m_GirderType][ModelType::Lifting].FpeHarped;
   Float64 FpeTemp     = m_Strands[m_GirderType][ModelType::Lifting].FpeTemp;
   m_LiftingStabilityProblem.AddFpe(_T("Straight"), 0,   FpeStraight, XpsStraight, YpsStraight);
   m_LiftingStabilityProblem.AddFpe(_T("Straight"), Xh1, FpeStraight, XpsStraight, YpsStraight);
   m_LiftingStabilityProblem.AddFpe(_T("Straight"), Xh2, FpeStraight, XpsStraight, YpsStraight);
   m_LiftingStabilityProblem.AddFpe(_T("Straight"), Xh3, FpeStraight, XpsStraight, YpsStraight);
   m_LiftingStabilityProblem.AddFpe(_T("Straight"), Xh4, FpeStraight, XpsStraight, YpsStraight);
   m_LiftingStabilityProblem.AddFpe(_T("Straight"), L,   FpeStraight, XpsStraight, YpsStraight);

   m_LiftingStabilityProblem.AddFpe(_T("Harped"), 0,   FpeHarped, XpsHarped, Yh1);
   m_LiftingStabilityProblem.AddFpe(_T("Harped"), Xh1, FpeHarped, XpsHarped, Yh1);
   m_LiftingStabilityProblem.AddFpe(_T("Harped"), Xh2, FpeHarped, XpsHarped, Yh2);
   m_LiftingStabilityProblem.AddFpe(_T("Harped"), Xh3, FpeHarped, XpsHarped, Yh3);
   m_LiftingStabilityProblem.AddFpe(_T("Harped"), Xh4, FpeHarped, XpsHarped, Yh4);
   m_LiftingStabilityProblem.AddFpe(_T("Harped"), L,   FpeHarped, XpsHarped, Yh4);

   m_LiftingStabilityProblem.AddFpe(_T("Temporary"), 0,   FpeTemp, XpsTemp, YpsTemp);
   m_LiftingStabilityProblem.AddFpe(_T("Temporary"), Xh1, FpeTemp, XpsTemp, YpsTemp);
   m_LiftingStabilityProblem.AddFpe(_T("Temporary"), Xh2, FpeTemp, XpsTemp, YpsTemp);
   m_LiftingStabilityProblem.AddFpe(_T("Temporary"), Xh3, FpeTemp, XpsTemp, YpsTemp);
   m_LiftingStabilityProblem.AddFpe(_T("Temporary"), Xh4, FpeTemp, XpsTemp, YpsTemp);
   m_LiftingStabilityProblem.AddFpe(_T("Temporary"), L,   FpeTemp, XpsTemp, YpsTemp);
}

void CPGStableModel::ResolveExactLiftingStrandLocations() const
{
   Float64 Lg = m_Girder[m_GirderType].GetGirderLength();
   m_LiftingStabilityProblem.ClearFpe();
   for (const auto& fpe : m_Strands[m_GirderType][ModelType::Lifting].m_vFpe)
   {
      if ( ::InRange(0.0,fpe.X,Lg) )
      {
         Float64 Xs,Ys,Xh,Yh,Xt,Yt;
         GetStrandLocations(fpe,&m_Girder[m_GirderType],&Xs,&Ys,&Xh,&Yh,&Xt,&Yt);
         m_LiftingStabilityProblem.AddFpe(_T("Straight"),  fpe.X, fpe.FpeStraight, Xs, Ys);
         m_LiftingStabilityProblem.AddFpe(_T("Harped"),    fpe.X, fpe.FpeHarped,   Xh, Yh);
         m_LiftingStabilityProblem.AddFpe(_T("Temporary"), fpe.X, fpe.FpeTemp,     Xt, Yt);
      }
   }
}

void CPGStableModel::ResolveHaulingStrandLocations() const
{
   if ( m_Strands[m_GirderType][ModelType::Hauling].strandMethod == CPGStableStrands::Simplified )
   {
      ResolveSimplifedHaulingStrandLocations();
   }
   else
   {
      ResolveExactHaulingStrandLocations();
   }
}

void CPGStableModel::ResolveSimplifedHaulingStrandLocations() const
{
   Float64 L = m_Girder[m_GirderType].GetGirderLength();
   m_HaulingStabilityProblem.AdjustForXferLength(true);
   m_HaulingStabilityProblem.SetXferLength(m_Strands[m_GirderType][ModelType::Hauling].XferLength,L);

   Float64 XpsStraight,YpsStraight,XpsHarped,Xh1,Yh1,Xh2,Yh2,Xh3,Yh3,Xh4,Yh4,XpsTemp,YpsTemp;
   GetSimplifiedStrandLocations(&m_Strands[m_GirderType][ModelType::Hauling],&m_Girder[m_GirderType],&XpsStraight,&YpsStraight,&XpsHarped,&Xh1,&Yh1,&Xh2,&Yh2,&Xh3,&Yh3,&Xh4,&Yh4,&XpsTemp,&YpsTemp);

   m_HaulingStabilityProblem.ClearFpe();

   Float64 FpeStraight = m_Strands[m_GirderType][ModelType::Hauling].FpeStraight;
   Float64 FpeHarped   = m_Strands[m_GirderType][ModelType::Hauling].FpeHarped;
   Float64 FpeTemp     = m_Strands[m_GirderType][ModelType::Hauling].FpeTemp;

   m_HaulingStabilityProblem.AddFpe(_T("Straight"), 0, FpeStraight, XpsStraight, YpsStraight);
   m_HaulingStabilityProblem.AddFpe(_T("Straight"), Xh1, FpeStraight, XpsStraight, YpsStraight);
   m_HaulingStabilityProblem.AddFpe(_T("Straight"), Xh2, FpeStraight, XpsStraight, YpsStraight);
   m_HaulingStabilityProblem.AddFpe(_T("Straight"), Xh3, FpeStraight, XpsStraight, YpsStraight);
   m_HaulingStabilityProblem.AddFpe(_T("Straight"), Xh4, FpeStraight, XpsStraight, YpsStraight);
   m_HaulingStabilityProblem.AddFpe(_T("Straight"), L, FpeStraight, XpsStraight, YpsStraight);

   m_HaulingStabilityProblem.AddFpe(_T("Harped"), 0, FpeHarped, XpsHarped, Yh1);
   m_HaulingStabilityProblem.AddFpe(_T("Harped"), Xh1, FpeHarped, XpsHarped, Yh1);
   m_HaulingStabilityProblem.AddFpe(_T("Harped"), Xh2, FpeHarped, XpsHarped, Yh2);
   m_HaulingStabilityProblem.AddFpe(_T("Harped"), Xh3, FpeHarped, XpsHarped, Yh3);
   m_HaulingStabilityProblem.AddFpe(_T("Harped"), Xh4, FpeHarped, XpsHarped, Yh4);
   m_HaulingStabilityProblem.AddFpe(_T("Harped"), L, FpeHarped, XpsHarped, Yh4);

   m_HaulingStabilityProblem.AddFpe(_T("Temporary"), 0, FpeTemp, XpsTemp, YpsTemp);
   m_HaulingStabilityProblem.AddFpe(_T("Temporary"), Xh1, FpeTemp, XpsTemp, YpsTemp);
   m_HaulingStabilityProblem.AddFpe(_T("Temporary"), Xh2, FpeTemp, XpsTemp, YpsTemp);
   m_HaulingStabilityProblem.AddFpe(_T("Temporary"), Xh3, FpeTemp, XpsTemp, YpsTemp);
   m_HaulingStabilityProblem.AddFpe(_T("Temporary"), Xh4, FpeTemp, XpsTemp, YpsTemp);
   m_HaulingStabilityProblem.AddFpe(_T("Temporary"), L, FpeTemp, XpsTemp, YpsTemp);
}

void CPGStableModel::ResolveExactHaulingStrandLocations() const
{
   Float64 Lg = m_Girder[m_GirderType].GetGirderLength();
   m_HaulingStabilityProblem.ClearFpe();
   for (const auto& fpe : m_Strands[m_GirderType][ModelType::Hauling].m_vFpe)
   {
      if ( ::InRange(0.0,fpe.X,Lg) )
      {
         Float64 Xs, Ys, Xh, Yh, Xt, Yt;
         GetStrandLocations(fpe, &m_Girder[m_GirderType], &Xs, &Ys, &Xh, &Yh, &Xt, &Yt);
         m_HaulingStabilityProblem.AddFpe(_T("Straight"), fpe.X, fpe.FpeStraight, Xs, Ys);
         m_HaulingStabilityProblem.AddFpe(_T("Harped"), fpe.X, fpe.FpeHarped, Xh, Yh);
         m_HaulingStabilityProblem.AddFpe(_T("Temporary"), fpe.X, fpe.FpeTemp, Xt, Yt);
      }
   }
}

void CPGStableModel::ResolveOneEndSeatedStrandLocations() const
{
   if (m_Strands[m_GirderType][ModelType::OneEndSeated].strandMethod == CPGStableStrands::Simplified)
   {
      ResolveSimplifedOneEndSeatedStrandLocations();
   }
   else
   {
      ResolveExactOneEndSeatedStrandLocations();
   }
}
void CPGStableModel::ResolveSimplifedOneEndSeatedStrandLocations() const
{
   Float64 L = m_Girder[m_GirderType].GetGirderLength();
   m_OneEndSeatedStabilityProblem.AdjustForXferLength(true);
   m_OneEndSeatedStabilityProblem.SetXferLength(m_Strands[m_GirderType][ModelType::OneEndSeated].XferLength, L);

   Float64 XpsStraight, YpsStraight, XpsHarped, Xh1, Yh1, Xh2, Yh2, Xh3, Yh3, Xh4, Yh4, XpsTemp, YpsTemp;
   GetSimplifiedStrandLocations(&m_Strands[m_GirderType][ModelType::OneEndSeated], &m_Girder[m_GirderType], &XpsStraight, &YpsStraight, &XpsHarped, &Xh1, &Yh1, &Xh2, &Yh2, &Xh3, &Yh3, &Xh4, &Yh4, &XpsTemp, &YpsTemp);

   m_OneEndSeatedStabilityProblem.ClearFpe();

   Float64 FpeStraight = m_Strands[m_GirderType][ModelType::OneEndSeated].FpeStraight;
   Float64 FpeHarped = m_Strands[m_GirderType][ModelType::OneEndSeated].FpeHarped;
   Float64 FpeTemp = m_Strands[m_GirderType][ModelType::OneEndSeated].FpeTemp;

   m_OneEndSeatedStabilityProblem.AddFpe(_T("Straight"), 0, FpeStraight, XpsStraight, YpsStraight);
   m_OneEndSeatedStabilityProblem.AddFpe(_T("Straight"), Xh1, FpeStraight, XpsStraight, YpsStraight);
   m_OneEndSeatedStabilityProblem.AddFpe(_T("Straight"), Xh2, FpeStraight, XpsStraight, YpsStraight);
   m_OneEndSeatedStabilityProblem.AddFpe(_T("Straight"), Xh3, FpeStraight, XpsStraight, YpsStraight);
   m_OneEndSeatedStabilityProblem.AddFpe(_T("Straight"), Xh4, FpeStraight, XpsStraight, YpsStraight);
   m_OneEndSeatedStabilityProblem.AddFpe(_T("Straight"), L, FpeStraight, XpsStraight, YpsStraight);

   m_OneEndSeatedStabilityProblem.AddFpe(_T("Harped"), 0, FpeHarped, XpsHarped, Yh1);
   m_OneEndSeatedStabilityProblem.AddFpe(_T("Harped"), Xh1, FpeHarped, XpsHarped, Yh1);
   m_OneEndSeatedStabilityProblem.AddFpe(_T("Harped"), Xh2, FpeHarped, XpsHarped, Yh2);
   m_OneEndSeatedStabilityProblem.AddFpe(_T("Harped"), Xh3, FpeHarped, XpsHarped, Yh3);
   m_OneEndSeatedStabilityProblem.AddFpe(_T("Harped"), Xh4, FpeHarped, XpsHarped, Yh4);
   m_OneEndSeatedStabilityProblem.AddFpe(_T("Harped"), L, FpeHarped, XpsHarped, Yh4);

   m_OneEndSeatedStabilityProblem.AddFpe(_T("Temporary"), 0, FpeTemp, XpsTemp, YpsTemp);
   m_OneEndSeatedStabilityProblem.AddFpe(_T("Temporary"), Xh1, FpeTemp, XpsTemp, YpsTemp);
   m_OneEndSeatedStabilityProblem.AddFpe(_T("Temporary"), Xh2, FpeTemp, XpsTemp, YpsTemp);
   m_OneEndSeatedStabilityProblem.AddFpe(_T("Temporary"), Xh3, FpeTemp, XpsTemp, YpsTemp);
   m_OneEndSeatedStabilityProblem.AddFpe(_T("Temporary"), Xh4, FpeTemp, XpsTemp, YpsTemp);
   m_OneEndSeatedStabilityProblem.AddFpe(_T("Temporary"), L, FpeTemp, XpsTemp, YpsTemp);
}

void CPGStableModel::ResolveExactOneEndSeatedStrandLocations() const
{
   Float64 Lg = m_Girder[m_GirderType].GetGirderLength();
   m_OneEndSeatedStabilityProblem.ClearFpe();
   for (const auto& fpe : m_Strands[m_GirderType][ModelType::OneEndSeated].m_vFpe)
   {
      if (::InRange(0.0, fpe.X, Lg))
      {
         Float64 Xs, Ys, Xh, Yh, Xt, Yt;
         GetStrandLocations(fpe, &m_Girder[m_GirderType], &Xs, &Ys, &Xh, &Yh, &Xt, &Yt);
         m_OneEndSeatedStabilityProblem.AddFpe(_T("Straight"), fpe.X, fpe.FpeStraight, Xs, Ys);
         m_OneEndSeatedStabilityProblem.AddFpe(_T("Harped"), fpe.X, fpe.FpeHarped, Xh, Yh);
         m_OneEndSeatedStabilityProblem.AddFpe(_T("Temporary"), fpe.X, fpe.FpeTemp, Xt, Yt);
      }
   }
}

void CPGStableModel::MapSimplifiedToExactStrandLocations(CPGStableStrands* pStrands)
{
   ATLASSERT(pStrands->strandMethod == CPGStableStrands::Simplified);

   // this call gets the strand location information for the stability engine.
   // we only want to create input parameters so we only want the X values for the harped strands
   Float64 XpsStraight,YpsStraight,XpsHarped,Xh1,Yh1,Xh2,Yh2,Xh3,Yh3,Xh4,Yh4,XpsTemp,YpsTemp;
   GetSimplifiedStrandLocations(pStrands,&m_Girder[m_GirderType],&XpsStraight,&YpsStraight,&XpsHarped,&Xh1,&Yh1,&Xh2,&Yh2,&Xh3,&Yh3,&Xh4,&Yh4,&XpsTemp,&YpsTemp);

   Float64 L = m_Girder[m_GirderType].GetGirderLength();

   pStrands->m_vFpe.clear();
   pStrands->m_vFpe.insert(CPGStableFpe(0,  pStrands->FpeStraight, XpsStraight, pStrands->Ys,pStrands->YsMeasure, pStrands->FpeHarped, XpsHarped, pStrands->Yh1,pStrands->Yh1Measure, pStrands->FpeTemp, XpsTemp, pStrands->Yt,pStrands->YtMeasure));
   pStrands->m_vFpe.insert(CPGStableFpe(Xh1,pStrands->FpeStraight, XpsStraight, pStrands->Ys,pStrands->YsMeasure, pStrands->FpeHarped, XpsHarped, pStrands->Yh1,pStrands->Yh1Measure, pStrands->FpeTemp, XpsTemp, pStrands->Yt,pStrands->YtMeasure));
   pStrands->m_vFpe.insert(CPGStableFpe(Xh2,pStrands->FpeStraight, XpsStraight, pStrands->Ys,pStrands->YsMeasure, pStrands->FpeHarped, XpsHarped, pStrands->Yh2,pStrands->Yh2Measure, pStrands->FpeTemp, XpsTemp, pStrands->Yt,pStrands->YtMeasure));
   pStrands->m_vFpe.insert(CPGStableFpe(Xh3,pStrands->FpeStraight, XpsStraight, pStrands->Ys,pStrands->YsMeasure, pStrands->FpeHarped, XpsHarped, pStrands->Yh3,pStrands->Yh3Measure, pStrands->FpeTemp, XpsTemp, pStrands->Yt,pStrands->YtMeasure));
   pStrands->m_vFpe.insert(CPGStableFpe(Xh4,pStrands->FpeStraight, XpsStraight, pStrands->Ys,pStrands->YsMeasure, pStrands->FpeHarped, XpsHarped, pStrands->Yh4,pStrands->Yh4Measure, pStrands->FpeTemp, XpsTemp, pStrands->Yt,pStrands->YtMeasure));
   pStrands->m_vFpe.insert(CPGStableFpe(L,  pStrands->FpeStraight, XpsStraight, pStrands->Ys,pStrands->YsMeasure, pStrands->FpeHarped, XpsHarped, pStrands->Yh4,pStrands->Yh4Measure, pStrands->FpeTemp, XpsTemp, pStrands->Yt,pStrands->YtMeasure));
}

void CPGStableModel::GetSimplifiedStrandLocations(const CPGStableStrands* pStrands,const WBFL::Stability::Girder* pGirder,Float64* pXpsStraight,Float64* pYpsStraight,Float64* pXpsHarped,Float64* pXh1,Float64* pYh1,Float64* pXh2,Float64* pYh2,Float64* pXh3,Float64* pYh3,Float64* pXh4,Float64* pYh4,Float64* pXpsTemp,Float64* pYpsTemp) const
{
   Float64 L = pGirder->GetGirderLength();

   Float64 Ag,Ixx,Iyy,Ixy,Xleft,Ytop,Hg,Wtf,Wbf;
   pGirder->GetSectionProperties(0,WBFL::Stability::Section::Start,&Ag,&Ixx,&Iyy,&Ixy,&Xleft,&Ytop,&Hg,&Wtf,&Wbf);

   Float64 XpsStraight = Xleft + pStrands->ex;
   Float64 XpsHarped = Xleft + pStrands->ex;
   Float64 XpsTemp = Xleft + pStrands->ex;

   Float64 YpsStraight = (pStrands->YsMeasure == TOP ? -pStrands->Ys : pStrands->Ys - Hg);
   Float64 YpsTemp = (pStrands->YtMeasure == TOP ? -pStrands->Yt : pStrands->Yt - Hg);
   Float64 Xh1,Yh1,Xh2,Yh2,Xh3,Yh3,Xh4,Yh4;
   if ( pStrands->Xh1Measure == FRACTION )
   {
      Xh1 = L*(pStrands->Xh1);
   }
   else
   {
      Xh1 = pStrands->Xh1;
   }

   pGirder->GetSectionProperties(Xh1,&Ag,&Ixx,&Iyy,&Ixy,&Xleft,&Ytop,&Hg,&Wtf,&Wbf);
   if ( pStrands->Yh1Measure == TOP )
   {
      Yh1 = -pStrands->Yh1;
   }
   else
   {
      Yh1 = pStrands->Yh1 - Hg;
   }

   if ( pStrands->Xh2Measure == FRACTION )
   {
      Xh2 = L*(pStrands->Xh2);
   }
   else
   {
      Xh2 = pStrands->Xh2;
   }

   pGirder->GetSectionProperties(Xh2,&Ag,&Ixx,&Iyy,&Ixy,&Xleft,&Ytop,&Hg,&Wtf,&Wbf);
   if ( pStrands->Yh2Measure == TOP )
   {
      Yh2 = -pStrands->Yh2;
   }
   else
   {
      Yh2 = pStrands->Yh2 - Hg;
   }

   if ( pStrands->Xh3Measure == FRACTION )
   {
      Xh3 = L*(pStrands->Xh3);
   }
   else
   {
      Xh3 = pStrands->Xh3;
   }

   pGirder->GetSectionProperties(Xh3,&Ag,&Ixx,&Iyy,&Ixy,&Xleft,&Ytop,&Hg,&Wtf,&Wbf);
   if ( pStrands->Yh3Measure == TOP )
   {
      Yh3 = -pStrands->Yh3;
   }
   else
   {
      Yh3 = pStrands->Yh3 - Hg;
   }

   if ( pStrands->Xh4Measure == FRACTION )
   {
      Xh4 = L*(pStrands->Xh4);
   }
   else
   {
      Xh4 = pStrands->Xh4;
   }

   pGirder->GetSectionProperties(Xh4,&Ag,&Ixx,&Iyy,&Ixy,&Xleft,&Ytop,&Hg,&Wtf,&Wbf);
   if ( pStrands->Yh4Measure == TOP )
   {
      Yh4 = -pStrands->Yh4;
   }
   else
   {
      Yh4 = pStrands->Yh4 - Hg;
   }
   
   *pXpsStraight = XpsStraight;
   *pYpsStraight = YpsStraight;
   *pXpsHarped = XpsHarped;
   *pXh1 = Xh1;
   *pYh1 = Yh1;
   *pXh2 = Xh2;
   *pYh2 = Yh2;
   *pXh3 = Xh3;
   *pYh3 = Yh3;
   *pXh4 = Xh4;
   *pYh4 = Yh4;
   *pXpsTemp = XpsTemp;
   *pYpsTemp = YpsTemp;
}

Float64 CPGStableModel::GetHarpedStrandLocation(Float64 X,Float64 X1,Float64 Y1,Float64 X2,Float64 Y2,Float64 X3,Float64 Y3,Float64 X4,Float64 Y4) const
{
   if ( ::InRange(0.0,X,X1) )
   {
      return Y1;
   }
   else if ( ::InRange(X1,X,X2) )
   {
      return ::LinInterp(X - X1,Y1,Y2,X2-X1);
   }
   else if ( ::InRange(X2,X,X3) )
   {
      return ::LinInterp(X - X2,Y2,Y3,X3-X2);
   }
   else if ( ::InRange(X3,X,X4) )
   {
      return ::LinInterp(X - X3,Y3,Y4,X4-X3);
   }
   else
   {
      return Y4;
   }
}

bool CPGStableModel::SetConcreteType(WBFL::Materials::ConcreteType type)
{
   if (m_LiftingStabilityProblem.GetConcrete().GetType() != type)
   {
      m_LiftingStabilityProblem.GetConcrete().SetType(type);
      m_HaulingStabilityProblem.GetConcrete().SetType(type);
      m_OneEndSeatedStabilityProblem.GetConcrete().SetType(type);
      return true;
   }
   return false;
}

WBFL::Materials::ConcreteType CPGStableModel::GetConcreteType() const
{
   return m_LiftingStabilityProblem.GetConcrete().GetType();
}

bool CPGStableModel::SetDensity(Float64 density)
{
   if ( !::IsEqual(m_LiftingStabilityProblem.GetConcrete().GetDensity(),density) )
   {
      ATLASSERT(!::IsEqual(m_HaulingStabilityProblem.GetConcrete().GetDensity(),density));
      m_LiftingStabilityProblem.GetConcrete().SetDensity(density);
      m_HaulingStabilityProblem.GetConcrete().SetDensity(density);
      m_OneEndSeatedStabilityProblem.GetConcrete().SetDensity(density);
      return true;
   }
   return false;
}

Float64 CPGStableModel::GetDensity() const
{
   ATLASSERT(::IsEqual(m_LiftingStabilityProblem.GetConcrete().GetDensity(),m_HaulingStabilityProblem.GetConcrete().GetDensity()));
   return m_LiftingStabilityProblem.GetConcrete().GetDensity();
}

bool CPGStableModel::SetDensityWithRebar(Float64 density)
{
   if ( !::IsEqual(m_LiftingStabilityProblem.GetConcrete().GetDensityForWeight(),density) )
   {
      ATLASSERT(!::IsEqual(m_HaulingStabilityProblem.GetConcrete().GetDensityForWeight(),density));
      m_LiftingStabilityProblem.GetConcrete().SetDensityForWeight(density);
      m_HaulingStabilityProblem.GetConcrete().SetDensityForWeight(density);
      m_OneEndSeatedStabilityProblem.GetConcrete().SetDensityForWeight(density);
      return true;
   }
   return false;
}

Float64 CPGStableModel::GetDensityWithRebar() const
{
   ATLASSERT(::IsEqual(m_LiftingStabilityProblem.GetConcrete().GetDensityForWeight(),m_HaulingStabilityProblem.GetConcrete().GetDensityForWeight()));
   return m_LiftingStabilityProblem.GetConcrete().GetDensityForWeight();
}

bool CPGStableModel::SetGirderType(GirderType girderType)
{
   if ( m_GirderType != girderType )
   {
      m_GirderType = girderType;
      return true;
   }
   return false;
}

GirderType CPGStableModel::GetGirderType() const
{
   return m_GirderType;
}

bool CPGStableModel::SetStressPointType(int stressPointType)
{
   if (m_StressPointType != stressPointType)
   {
      m_StressPointType = stressPointType;
      return true;
   }
   return false;
}

int CPGStableModel::GetStressPointType() const
{
   return m_StressPointType;
}

bool CPGStableModel::SetStrands(GirderType girderType,ModelType modelType,const CPGStableStrands& strands)
{
   if ( m_Strands[girderType][modelType] != strands )
   {
      m_Strands[girderType][modelType] = strands;
      if ( m_Strands[girderType][modelType].strandMethod == CPGStableStrands::Simplified )
      {
         MapSimplifiedToExactStrandLocations(&m_Strands[girderType][modelType]);
      }
      return true;
   }
   return false;
}

const CPGStableStrands& CPGStableModel::GetStrands(GirderType girderType,ModelType modelType) const
{
   return m_Strands[girderType][modelType];
}

bool CPGStableModel::SetGirder(GirderType girderType,const WBFL::Stability::Girder& girder)
{
   if ( m_Girder[girderType] != girder )
   {
      m_Girder[girderType] = girder;
      return true;
   }
   return false;
}

const WBFL::Stability::Girder& CPGStableModel::GetGirder(GirderType girderType) const
{
   return m_Girder[girderType];
}

bool CPGStableModel::SetLiftingStabilityProblem(const WBFL::Stability::LiftingStabilityProblem& problem)
{
   if ( m_LiftingStabilityProblem != problem )
   {
      m_LiftingStabilityProblem = problem;
      return true;
   }
   return false;
}

const WBFL::Stability::LiftingStabilityProblem& CPGStableModel::GetLiftingStabilityProblem() const
{
   return m_LiftingStabilityProblem;
}

bool CPGStableModel::SetHaulingStabilityProblem(const WBFL::Stability::HaulingStabilityProblem& problem)
{
   if ( m_HaulingStabilityProblem != problem )
   {
      m_HaulingStabilityProblem = problem;
      return true;
   }
   return false;
}

const WBFL::Stability::HaulingStabilityProblem& CPGStableModel::GetHaulingStabilityProblem() const
{
   return m_HaulingStabilityProblem;
}

bool CPGStableModel::SetOneEndSeatedStabilityProblem(const WBFL::Stability::OneEndSeatedStabilityProblem& problem)
{
   if (m_OneEndSeatedStabilityProblem != problem)
   {
      m_OneEndSeatedStabilityProblem = problem;
      return true;
   }
   return false;
}

const WBFL::Stability::OneEndSeatedStabilityProblem& CPGStableModel::GetOneEndSeatedStabilityProblem() const
{
   return m_OneEndSeatedStabilityProblem;
}

bool CPGStableModel::SetLiftingCriteria(const CPGStableLiftingCriteria& criteria)
{
   if ( m_LiftingCriteria != criteria )
   {
      m_LiftingCriteria = criteria;
      return true;
   }
   return false;
}

const CPGStableLiftingCriteria& CPGStableModel::GetLiftingCriteria() const
{
   return m_LiftingCriteria;
}

bool CPGStableModel::SetHaulingCriteria(const CPGStableHaulingCriteria& criteria)
{
   if ( m_HaulingCriteria != criteria )
   {
      m_HaulingCriteria = criteria;
      return true;
   }
   return false;
}

const CPGStableHaulingCriteria& CPGStableModel::GetHaulingCriteria() const
{
   return m_HaulingCriteria;
}

bool CPGStableModel::SetOneEndSeatedCriteria(const CPGStableOneEndSeatedCriteria& criteria)
{
   if (m_OneEndSeatedCriteria != criteria)
   {
      m_OneEndSeatedCriteria = criteria;
      return true;
   }
   return false;
}

const CPGStableOneEndSeatedCriteria& CPGStableModel::GetOneEndSeatedCriteria() const
{
   return m_OneEndSeatedCriteria;
}

Float64 CPGStableModel::GetK1() const
{
   return m_K1;
}

bool CPGStableModel::SetK1(Float64 k1)
{
   if ( !IsEqual(m_K1,k1) )
   {
      m_K1 = k1;
      return true;
   }
   return false;
}

Float64 CPGStableModel::GetK2() const
{
   return m_K2;
}

bool CPGStableModel::SetK2(Float64 k2)
{
   if ( !IsEqual(m_K2,k2) )
   {
      m_K2 = k2;
      return true;
   }
   return false;
}


void CPGStableModel::GetLiftingMaterials(Float64* pFci,bool* pbComputeEci,Float64* pFrCoefficient) const
{
   *pFci = m_LiftingStabilityProblem.GetConcrete().GetFc();
   *pbComputeEci = m_bComputeEci;
   *pFrCoefficient = m_LiftingFrCoefficient;
}

bool CPGStableModel::SetLiftingMaterials(Float64 fci,bool bComputeEci,Float64 frCoefficient)
{
   if ( !IsEqual(m_LiftingStabilityProblem.GetConcrete().GetFc(),fci) || m_bComputeEci != bComputeEci || !IsEqual(m_LiftingFrCoefficient,frCoefficient) )
   {
      m_LiftingStabilityProblem.GetConcrete().SetFc(fci);
      m_bComputeEci = bComputeEci;
      m_LiftingFrCoefficient = frCoefficient;
      return true;
   }
   return false;
}

void CPGStableModel::GetHaulingMaterials(Float64* pFc,bool* pbComputeEc,Float64* pFrCoefficient) const
{
   *pFc = m_HaulingStabilityProblem.GetConcrete().GetFc();
   *pbComputeEc = m_bComputeEc;
   *pFrCoefficient = m_HaulingFrCoefficient;
}

bool CPGStableModel::SetHaulingMaterials(Float64 fc,bool bComputeEc,Float64 frCoefficient)
{
   if ( !IsEqual(m_HaulingStabilityProblem.GetConcrete().GetFc(),fc) || m_bComputeEc != bComputeEc || !IsEqual(m_HaulingFrCoefficient,frCoefficient) )
   {
      m_HaulingStabilityProblem.GetConcrete().SetFc(fc);
      m_bComputeEc = bComputeEc;
      m_HaulingFrCoefficient = frCoefficient;
      return true;
   }
   return false;
}

void CPGStableModel::GetOneEndSeatedMaterials(Float64* pFc, bool* pbComputeEc, Float64* pFrCoefficient) const
{
   *pFc = m_OneEndSeatedStabilityProblem.GetConcrete().GetFc();
   *pbComputeEc = m_bComputeEcOneEndSeated;
   *pFrCoefficient = m_OneEndSeatedFrCoefficient;
}

bool CPGStableModel::SetOneEndSeatedMaterials(Float64 fc, bool bComputeEc, Float64 frCoefficient)
{
   if (!IsEqual(m_OneEndSeatedStabilityProblem.GetConcrete().GetFc(), fc) || m_bComputeEcOneEndSeated != bComputeEc || !IsEqual(m_OneEndSeatedFrCoefficient, frCoefficient))
   {
      m_OneEndSeatedStabilityProblem.GetConcrete().SetFc(fc);
      m_bComputeEcOneEndSeated = bComputeEc;
      m_OneEndSeatedFrCoefficient = frCoefficient;
      return true;
   }
   return false;
}

Float64 CPGStableModel::GetHeightOfGirderBottomAboveRoadway() const
{
   return m_Hgb;
}

bool CPGStableModel::SetHeightOfGirderBottomAboveRoadway(Float64 Hgb)
{
   if ( !IsEqual(m_Hgb,Hgb) )
   {
      m_Hgb = Hgb;
      return true;
   }
   return false;
}

void CPGStableModel::ResolveStrandLocations(const CPGStableStrands& strands,const WBFL::Stability::Girder& girder,Float64* pXs,Float64* pYs,Float64* pXh,Float64* pXh1,Float64* pYh1,Float64* pXh2,Float64* pYh2,Float64* pXh3,Float64* pYh3,Float64* pXh4,Float64* pYh4,Float64* pXt,Float64* pYt)
{
   GetSimplifiedStrandLocations(&strands,&girder,pXs,pYs,pXh,pXh1,pYh1,pXh2,pYh2,pXh3,pYh3,pXh4,pYh4,pXt,pYt);
}

void CPGStableModel::GetStrandProfiles(const CPGStableStrands& strands,const WBFL::Stability::Girder& girder,std::vector<std::pair<Float64,Float64>>* pvStraight,std::vector<std::pair<Float64,Float64>>* pvHarped,std::vector<std::pair<Float64,Float64>>* pvTemp) const
{
   if ( strands.strandMethod == CPGStableStrands::Simplified )
   {
      Float64 Xs,Ys,Xh,Xh1,Yh1,Xh2,Yh2,Xh3,Yh3,Xh4,Yh4,Xt,Yt;
      GetSimplifiedStrandLocations(&strands,&girder,&Xs,&Ys,&Xh,&Xh1,&Yh1,&Xh2,&Yh2,&Xh3,&Yh3,&Xh4,&Yh4,&Xt,&Yt);

      Float64 Lg = girder.GetGirderLength();

      pvStraight->clear();
      pvStraight->emplace_back(0, Ys);
      pvStraight->emplace_back(Lg,Ys);

      pvHarped->clear();
      pvHarped->emplace_back(0,Yh1);
      pvHarped->emplace_back(Xh1,Yh1);
      pvHarped->emplace_back(Xh2,Yh2);
      pvHarped->emplace_back(Xh3,Yh3);
      pvHarped->emplace_back(Xh4,Yh4);
      pvHarped->emplace_back(Lg, Yh4);

      pvTemp->clear();
      pvTemp->emplace_back(0, Yt);
      pvTemp->emplace_back(Lg,Yt);
   }
   else
   {
      Float64 Lg = girder.GetGirderLength();

      pvStraight->clear();
      pvHarped->clear();
      pvTemp->clear();

      Float64 Xs,Ys,Xh,Yh,Xt,Yt;
      const CPGStableFpe& firstFpe = *strands.m_vFpe.begin();
      GetStrandLocations(firstFpe,&girder,&Xs,&Ys,&Xh,&Yh,&Xt,&Yt);
      pvStraight->emplace_back(0,Ys);
      pvHarped->emplace_back(0,Yh);
      pvTemp->emplace_back(0,Yt);

      for (const auto& fpe : strands.m_vFpe)
      {
         if ( ::InRange(0.0,fpe.X,Lg) )
         {
            // ignore all fpe values that aren't on the girder
            GetStrandLocations(fpe,&girder,&Xs,&Ys,&Xh,&Yh,&Xt,&Yt);
            pvStraight->emplace_back(fpe.X,Ys);
            pvHarped->emplace_back(fpe.X,Yh);
            pvTemp->emplace_back(fpe.X,Yt);
         }
      }

      const CPGStableFpe& lastFpe = *strands.m_vFpe.rbegin();
      if (::InRange(0.0,lastFpe.X,Lg) )
      {
         GetStrandLocations(lastFpe,&girder,&Xs,&Ys,&Xh,&Yh,&Xt,&Yt);
         pvStraight->emplace_back(Lg,Ys);
         pvHarped->emplace_back(Lg,Yh);
         pvTemp->emplace_back(Lg,Yt);
      }
   }
}

void CPGStableModel::GetStrandLocations(const CPGStableFpe& fpe,const WBFL::Stability::Girder* pGirder,Float64* pXs,Float64* pYs,Float64* pXh,Float64* pYh,Float64* pXt,Float64* pYt) const
{
   Float64 Ag,Ixx,Iyy,Ixy,Xleft,Ytop,Hg,Wtf,Wbf;
   pGirder->GetSectionProperties(fpe.X,&Ag,&Ixx,&Iyy,&Ixy,&Xleft,&Ytop,&Hg,&Wtf,&Wbf);

   if ( fpe.YpsStraightMeasure == TOP )
   {
      *pYs = -fpe.YpsStraight;
   }
   else
   {
      *pYs = fpe.YpsStraight - Hg;
   }
   *pXs = fpe.XpsStraight;

   if ( fpe.YpsHarpedMeasure == TOP )
   {
      *pYh = -fpe.YpsHarped;
   }
   else
   {
      *pYh = fpe.YpsHarped - Hg;
   }
   *pXh = fpe.XpsHarped;

   if ( fpe.YpsTempMeasure == TOP )
   {
      *pYt = -fpe.YpsTemp;
   }
   else
   {
      *pYt = fpe.YpsTemp - Hg;
   }
   *pXt = fpe.XpsTemp;
}

HRESULT CPGStableModel::Save(IStructuredSave* pStrSave)
{
   HRESULT hr = pStrSave->BeginUnit(_T("PGStable"),3.0);
   if ( FAILED(hr) )
      return hr;

   CEAFApp* pApp = EAFGetApp();
   hr = pStrSave->put_Property(_T("Units"),CComVariant(pApp->GetUnitsMode()));

   pStrSave->put_Property(_T("GirderType"),CComVariant(m_GirderType));
   pStrSave->put_Property(_T("StressPointType"), CComVariant(m_StressPointType)); // added in version 2
   pStrSave->put_Property(_T("DragCoefficient"),CComVariant(m_Girder[m_GirderType].GetDragCoefficient()));
   pStrSave->put_Property(_T("Precamber"), CComVariant(m_Girder[m_GirderType].GetPrecamber())); // added in version 2

   pStrSave->BeginUnit(_T("Girder"),2.0);
   pStrSave->BeginUnit(_T("Sections"),1.0);
   IndexType nSections = m_Girder[m_GirderType].GetSectionCount();
   for ( IndexType sectIdx = 0; sectIdx < nSections; sectIdx++ )
   {
      pStrSave->BeginUnit(_T("Section"),1.0);

      Float64 L = m_Girder[m_GirderType].GetSectionLength(sectIdx);
      pStrSave->put_Property(_T("L"),CComVariant(L));

      Float64 Ag,Ixx,Iyy,Ixy,Xleft,Ytop,Hg,Wtf,Wbf;
      m_Girder[m_GirderType].GetSectionProperties(sectIdx,WBFL::Stability::Section::Start,&Ag,&Ixx,&Iyy,&Ixy,&Xleft,&Ytop,&Hg,&Wtf,&Wbf);
      pStrSave->BeginUnit(_T("StartFace"),2.0);
      pStrSave->put_Property(_T("Ag"),CComVariant(Ag));
      pStrSave->put_Property(_T("Ix"),CComVariant(Ixx));
      pStrSave->put_Property(_T("Iy"), CComVariant(Iyy));
      pStrSave->put_Property(_T("Ixy"), CComVariant(Ixy)); // added in version 2
      pStrSave->put_Property(_T("Xleft"), CComVariant(Xleft)); // added in version 2
      pStrSave->put_Property(_T("Ytop"), CComVariant(Ytop)); // changed from "Yt" to "Ytop" in version 2
      pStrSave->put_Property(_T("Hg"),CComVariant(Hg));
      pStrSave->put_Property(_T("Wtf"),CComVariant(Wtf));
      pStrSave->put_Property(_T("Wbf"),CComVariant(Wbf));

      if (m_StressPointType == DEFINE_STRESS_POINTS)
      {
         // added in version 2
         pStrSave->BeginUnit(_T("StressPoints"), 1.0);

         WBFL::Stability::Point pntTL, pntTR, pntBL, pntBR;
         m_Girder[m_GirderType].GetStressPoints(sectIdx, WBFL::Stability::Section::Start, &pntTL, &pntTR, &pntBL, &pntBR);
         pStrSave->BeginUnit(_T("TopLeft"), 1.0);
         pStrSave->put_Property(_T("X"), CComVariant(pntTL.X()));
         pStrSave->put_Property(_T("Y"), CComVariant(pntTL.Y()));
         pStrSave->EndUnit(); // TopLeft

         pStrSave->BeginUnit(_T("TopRight"), 1.0);
         pStrSave->put_Property(_T("X"), CComVariant(pntTR.X()));
         pStrSave->put_Property(_T("Y"), CComVariant(pntTR.Y()));
         pStrSave->EndUnit(); // TopRight

         pStrSave->BeginUnit(_T("BottomLeft"), 1.0);
         pStrSave->put_Property(_T("X"), CComVariant(pntBL.X()));
         pStrSave->put_Property(_T("Y"), CComVariant(pntBL.Y()));
         pStrSave->EndUnit(); // BottomLeft

         pStrSave->BeginUnit(_T("BottomRight"), 1.0);
         pStrSave->put_Property(_T("X"), CComVariant(pntBR.X()));
         pStrSave->put_Property(_T("Y"), CComVariant(pntBR.Y()));
         pStrSave->EndUnit(); // BottomRight

         pStrSave->EndUnit(); // StressPoints
      }

      pStrSave->EndUnit(); // LeftFace

      m_Girder[m_GirderType].GetSectionProperties(sectIdx,WBFL::Stability::Section::End,&Ag,&Ixx,&Iyy,&Ixy,&Xleft,&Ytop,&Hg,&Wtf,&Wbf);
      pStrSave->BeginUnit(_T("EndFace"),2.0);
      pStrSave->put_Property(_T("Ag"),CComVariant(Ag));
      pStrSave->put_Property(_T("Ix"),CComVariant(Ixx));
      pStrSave->put_Property(_T("Iy"), CComVariant(Iyy));
      pStrSave->put_Property(_T("Ixy"), CComVariant(Ixy)); // added in version 2
      pStrSave->put_Property(_T("Xleft"), CComVariant(Xleft)); // added in version 2
      pStrSave->put_Property(_T("Ytop"), CComVariant(Ytop)); // changed from "Yt" to "Ytop" in version 2
      pStrSave->put_Property(_T("Hg"),CComVariant(Hg));
      pStrSave->put_Property(_T("Wtf"),CComVariant(Wtf));
      pStrSave->put_Property(_T("Wbf"),CComVariant(Wbf));

      if (m_StressPointType == DEFINE_STRESS_POINTS)
      {
         // added in version 2
         pStrSave->BeginUnit(_T("StressPoints"), 1.0);

         WBFL::Stability::Point pntTL, pntTR, pntBL, pntBR;
         m_Girder[m_GirderType].GetStressPoints(sectIdx, WBFL::Stability::Section::End, &pntTL, &pntTR, &pntBL, &pntBR);
         pStrSave->BeginUnit(_T("TopLeft"), 1.0);
         pStrSave->put_Property(_T("X"), CComVariant(pntTL.X()));
         pStrSave->put_Property(_T("Y"), CComVariant(pntTL.Y()));
         pStrSave->EndUnit(); // TopLeft

         pStrSave->BeginUnit(_T("TopRight"), 1.0);
         pStrSave->put_Property(_T("X"), CComVariant(pntTR.X()));
         pStrSave->put_Property(_T("Y"), CComVariant(pntTR.Y()));
         pStrSave->EndUnit(); // TopRight

         pStrSave->BeginUnit(_T("BottomLeft"), 1.0);
         pStrSave->put_Property(_T("X"), CComVariant(pntBL.X()));
         pStrSave->put_Property(_T("Y"), CComVariant(pntBL.Y()));
         pStrSave->EndUnit(); // BottomLeft

         pStrSave->BeginUnit(_T("BottomRight"), 1.0);
         pStrSave->put_Property(_T("X"), CComVariant(pntBR.X()));
         pStrSave->put_Property(_T("Y"), CComVariant(pntBR.Y()));
         pStrSave->EndUnit(); // BottomRight

         pStrSave->EndUnit(); // StressPoints
      }

      pStrSave->EndUnit(); // RightFace

      pStrSave->EndUnit(); // Section
   }
   pStrSave->EndUnit(); // Sections

   pStrSave->put_Property(_T("DensityWithRebar"),CComVariant(GetDensityWithRebar()));
   pStrSave->put_Property(_T("Density"),CComVariant(GetDensity()));
   pStrSave->put_Property(_T("K1"),CComVariant(m_K1));
   pStrSave->put_Property(_T("K2"),CComVariant(m_K2));
   pStrSave->put_Property(_T("ConcreteType"), CComVariant((long)GetConcreteType())); // added in version 2 of Girder datablock

   std::vector<std::pair<Float64,Float64>> vLoads = m_Girder[m_GirderType].GetAdditionalLoads();
   if ( 0 < vLoads.size() )
   {
      pStrSave->BeginUnit(_T("AdditionalLoads"),1.0);
      std::vector<std::pair<Float64,Float64>>::iterator iter(vLoads.begin());
      std::vector<std::pair<Float64,Float64>>::iterator end(vLoads.end());
      for ( ; iter != end; iter++ )
      {
         pStrSave->BeginUnit(_T("AdditionalLoad"),1.0);
         pStrSave->put_Property(_T("X"),CComVariant(iter->first));
         pStrSave->put_Property(_T("P"),CComVariant(iter->second));
         pStrSave->EndUnit(); // AdditionalLoad
      }
      pStrSave->EndUnit(); // AdditionalLoads
   }
   pStrSave->EndUnit(); // Girder


   {
   pStrSave->BeginUnit(_T("LiftingProblem"),6.0);
   const WBFL::Stability::LiftingStabilityProblem& liftingProblem = GetLiftingStabilityProblem();
   m_Strands[m_GirderType][ModelType::Lifting].Save(pStrSave);

   Float64 camber = liftingProblem.GetCamber();
   //pStrSave->put_Property(_T("DirectCamber"),CComVariant(bDirectCamber)); // removed in version 4
   pStrSave->put_Property(_T("Camber"),CComVariant(camber));

   // added version 3
   Float64 lateralCamber = liftingProblem.GetLateralCamber();
   pStrSave->put_Property(_T("LateralCamber"), CComVariant(lateralCamber));

   Float64 Ll,Lr;
   liftingProblem.GetSupportLocations(&Ll,&Lr);
   ATLASSERT(IsEqual(Ll,Lr));
   pStrSave->put_Property(_T("LiftPoint"),CComVariant(Ll));

   Float64 Yra = liftingProblem.GetYRollAxis();
   pStrSave->put_Property(_T("Yra"),CComVariant(Yra));

   Float64 SweepTolerance = liftingProblem.GetSweepTolerance();
   pStrSave->put_Property(_T("SweepTolerance"),CComVariant(SweepTolerance));

   Float64 SweepGrowth = liftingProblem.GetSweepGrowth();
   pStrSave->put_Property(_T("SweepGrowth"), CComVariant(SweepGrowth)); // added in version 5

   Float64 SupportPlacementTolerance = liftingProblem.GetSupportPlacementTolerance();
   pStrSave->put_Property(_T("SupportPlacementTolerance"),CComVariant(SupportPlacementTolerance));

   Float64 imup,imdn;
   liftingProblem.GetImpact(&imup,&imdn);
   pStrSave->put_Property(_T("ImpactUp"),CComVariant(imup));
   pStrSave->put_Property(_T("ImpactDown"),CComVariant(imdn));

   // changed in version 2.0
   //bool bPlumbGirderStresses = liftingProblem.EvaluateStressesForPlumbGirder();
   //pStrSave->put_Property(_T("PlumbGirderStresses"),CComVariant(bPlumbGirderStresses));
   // removed in version 4
   //bool bEvaluateStressesAtEquilibriumAngle = liftingProblem.EvaluateStressesAtEquilibriumAngle();
   //pStrSave->put_Property(_T("EvaluateStressesAtEquilibriumAngle"), CComVariant(bEvaluateStressesAtEquilibriumAngle));

   WBFL::Stability::WindLoadType windLoadType;
   Float64 windLoad;
   liftingProblem.GetWindLoading(&windLoadType,&windLoad);
   pStrSave->put_Property(_T("WindLoadType"),CComVariant(+windLoadType));
   pStrSave->put_Property(_T("WindLoad"),CComVariant(windLoad));

   // added in version 6 of this datablock
   Float64 eb, wb;
   liftingProblem.GetAppurtenanceLoading(&eb, &wb);
   pStrSave->put_Property(_T("BracketEccentricity"), CComVariant(eb));
   pStrSave->put_Property(_T("BracketWeight"), CComVariant(wb));

   Float64 liftAngle = liftingProblem.GetLiftAngle();
   pStrSave->put_Property(_T("LiftAngle"),CComVariant(liftAngle));

   const WBFL::Materials::SimpleConcrete& concrete = liftingProblem.GetConcrete();
   pStrSave->put_Property(_T("FrCoefficient"),CComVariant(m_LiftingFrCoefficient));
   pStrSave->put_Property(_T("Fci"),CComVariant(concrete.GetFc()));
   pStrSave->put_Property(_T("ComputeEci"),CComVariant(m_bComputeEci));
   pStrSave->put_Property(_T("Eci"),CComVariant(concrete.GetE()));
   pStrSave->EndUnit(); // LiftingProblem
   }

   {
   pStrSave->BeginUnit(_T("HaulingProblem"),6.0);
   m_Strands[m_GirderType][ModelType::Hauling].Save(pStrSave);

   const WBFL::Stability::HaulingStabilityProblem& haulingProblem = GetHaulingStabilityProblem();

   Float64 camber = haulingProblem.GetCamber();
   //pStrSave->put_Property(_T("DirectCamber"),CComVariant(bDirectCamber)); // removed in version 4
   pStrSave->put_Property(_T("Camber"),CComVariant(camber));

   // added version 3
   Float64 lateralCamber = haulingProblem.GetLateralCamber();
   pStrSave->put_Property(_T("LateralCamber"), CComVariant(lateralCamber));

   Float64 Ll,Lr;
   haulingProblem.GetSupportLocations(&Ll,&Lr);
   pStrSave->put_Property(_T("LeftBunkPoint"),CComVariant(Ll));
   pStrSave->put_Property(_T("RightBunkPoint"),CComVariant(Lr));

   pStrSave->put_Property(_T("Hgb"),CComVariant(m_Hgb));

   Float64 SweepTolerance = haulingProblem.GetSweepTolerance();
   pStrSave->put_Property(_T("SweepTolerance"),CComVariant(SweepTolerance));

   Float64 SweepGrowth = haulingProblem.GetSweepGrowth();
   pStrSave->put_Property(_T("SweepGrowth"), CComVariant(SweepGrowth)); // added in version 5

   Float64 SupportPlacementTolerance = haulingProblem.GetSupportPlacementTolerance();
   pStrSave->put_Property(_T("SupportPlacementTolerance"),CComVariant(SupportPlacementTolerance));

   Float64 imup,imdn;
   haulingProblem.GetImpact(&imup,&imdn);
   pStrSave->put_Property(_T("ImpactUp"),CComVariant(imup));
   pStrSave->put_Property(_T("ImpactDown"),CComVariant(imdn));

   WBFL::Stability::HaulingImpact impactUsage = haulingProblem.GetImpactUsage();
   pStrSave->put_Property(_T("ImpactUsage"),CComVariant(+impactUsage));

   // added in version 2, removed in version 4
   //bool bEvaluateStressesAtEquilibriumAngle = haulingProblem.EvaluateStressesAtEquilibriumAngle(WBFL::Stability::CrownSlope);
   //pStrSave->put_Property(_T("EvaluateStressesAtEquilibriumAngle"), CComVariant(bEvaluateStressesAtEquilibriumAngle));

   WBFL::Stability::WindLoadType windLoadType;
   Float64 windLoad;
   haulingProblem.GetWindLoading(&windLoadType,&windLoad);
   pStrSave->put_Property(_T("WindLoadType"),CComVariant(+windLoadType));
   pStrSave->put_Property(_T("WindLoad"),CComVariant(windLoad));

   // added in version 6 of this datablock
   Float64 eb, wb;
   haulingProblem.GetAppurtenanceLoading(&eb, &wb);
   pStrSave->put_Property(_T("BracketEccentricity"), CComVariant(eb));
   pStrSave->put_Property(_T("BracketWeight"), CComVariant(wb));

   Float64 Ktheta = haulingProblem.GetRotationalStiffness();
   pStrSave->put_Property(_T("Ktheta"),CComVariant(Ktheta));
   Float64 Wcc = haulingProblem.GetSupportWidth();
   pStrSave->put_Property(_T("Wcc"),CComVariant(Wcc));
   Float64 Hrc = haulingProblem.GetHeightOfRollAxis();
   pStrSave->put_Property(_T("Hrc"),CComVariant(Hrc));
   Float64 CrownSlope = haulingProblem.GetSupportSlope();
   pStrSave->put_Property(_T("CrownSlope"),CComVariant(CrownSlope));
   Float64 Superelevation = haulingProblem.GetSuperelevation();
   pStrSave->put_Property(_T("Superelevation"),CComVariant(Superelevation));
   Float64 Velocity = haulingProblem.GetVelocity();
   pStrSave->put_Property(_T("Velocity"),CComVariant(Velocity));
   Float64 Radius = haulingProblem.GetTurningRadius();
   pStrSave->put_Property(_T("Radius"),CComVariant(Radius));
   WBFL::Stability::CFType cfType = haulingProblem.GetCentrifugalForceType();
   pStrSave->put_Property(_T("CFType"),CComVariant(+cfType));

   const WBFL::Materials::SimpleConcrete& concrete = haulingProblem.GetConcrete();
   pStrSave->put_Property(_T("FrCoefficient"),CComVariant(m_HaulingFrCoefficient));
   pStrSave->put_Property(_T("Fc"),CComVariant(concrete.GetFc()));
   pStrSave->put_Property(_T("ComputeEc"),CComVariant(m_bComputeEc));
   pStrSave->put_Property(_T("Ec"),CComVariant(concrete.GetE()));
   pStrSave->EndUnit(); // HaulingProblem
   }

   {
      // added in version 3 of the parent datablock
      pStrSave->BeginUnit(_T("OneEndSeatedProblem"), 1.0);
      m_Strands[m_GirderType][ModelType::OneEndSeated].Save(pStrSave);

      const WBFL::Stability::OneEndSeatedStabilityProblem& oneEndSeatedProblem = GetOneEndSeatedStabilityProblem();

      Float64 camber = oneEndSeatedProblem.GetCamber();
      pStrSave->put_Property(_T("Camber"), CComVariant(camber));

      Float64 lateralCamber = oneEndSeatedProblem.GetLateralCamber();
      pStrSave->put_Property(_T("LateralCamber"), CComVariant(lateralCamber));

      Float64 Ll, Lr;
      oneEndSeatedProblem.GetSupportLocations(&Ll, &Lr);
      pStrSave->put_Property(_T("LeftBunkPoint"), CComVariant(Ll));
      pStrSave->put_Property(_T("RightBunkPoint"), CComVariant(Lr));

      pStrSave->put_Property(_T("Hgb"), CComVariant(m_Hgb));

      Float64 SweepTolerance = oneEndSeatedProblem.GetSweepTolerance();
      pStrSave->put_Property(_T("SweepTolerance"), CComVariant(SweepTolerance));

      Float64 SweepGrowth = oneEndSeatedProblem.GetSweepGrowth();
      pStrSave->put_Property(_T("SweepGrowth"), CComVariant(SweepGrowth));

      Float64 SupportPlacementTolerance = oneEndSeatedProblem.GetSupportPlacementTolerance();
      pStrSave->put_Property(_T("SupportPlacementTolerance"), CComVariant(SupportPlacementTolerance));

      Float64 YrollLift = oneEndSeatedProblem.GetYRollLiftEnd();
      pStrSave->put_Property(_T("YrollLiftEnd"), CComVariant(YrollLift));
      Float64 liftTolerance = oneEndSeatedProblem.GetLiftPlacementTolerance();
      pStrSave->put_Property(_T("LiftPlacementTolerance"), CComVariant(liftTolerance));

      Float64 imup, imdn;
      oneEndSeatedProblem.GetImpact(&imup, &imdn);
      pStrSave->put_Property(_T("ImpactUp"), CComVariant(imup));
      pStrSave->put_Property(_T("ImpactDown"), CComVariant(imdn));

      WBFL::Stability::WindLoadType windLoadType;
      Float64 windLoad;
      oneEndSeatedProblem.GetWindLoading(&windLoadType, &windLoad);
      pStrSave->put_Property(_T("WindLoadType"), CComVariant(+windLoadType));
      pStrSave->put_Property(_T("WindLoad"), CComVariant(windLoad));

      Float64 eb, wb;
      oneEndSeatedProblem.GetAppurtenanceLoading(&eb, &wb);
      pStrSave->put_Property(_T("BracketEccentricity"), CComVariant(eb));
      pStrSave->put_Property(_T("BracketWeight"), CComVariant(wb));

      Float64 Ktheta = oneEndSeatedProblem.GetRotationalStiffness();
      pStrSave->put_Property(_T("Ktheta"), CComVariant(Ktheta));
      Float64 Kadjust = oneEndSeatedProblem.GetRotationalStiffnessAdjustmentFactor();
      pStrSave->put_Property(_T("Kadjust"), CComVariant(Kadjust));
      Float64 Wcc = oneEndSeatedProblem.GetSupportWidth();
      pStrSave->put_Property(_T("Wcc"), CComVariant(Wcc));
      Float64 Hrc = oneEndSeatedProblem.GetHeightOfRollAxis();
      pStrSave->put_Property(_T("Hrc"), CComVariant(Hrc));
      Float64 CrownSlope = oneEndSeatedProblem.GetSupportSlope();
      pStrSave->put_Property(_T("CrownSlope"), CComVariant(CrownSlope));

      const WBFL::Materials::SimpleConcrete& concrete = oneEndSeatedProblem.GetConcrete();
      pStrSave->put_Property(_T("FrCoefficient"), CComVariant(m_OneEndSeatedFrCoefficient));
      pStrSave->put_Property(_T("Fc"), CComVariant(concrete.GetFc()));
      pStrSave->put_Property(_T("ComputeEc"), CComVariant(m_bComputeEcOneEndSeated));
      pStrSave->put_Property(_T("Ec"), CComVariant(concrete.GetE()));
      pStrSave->EndUnit(); // OneEndSeatedProblem
   }

   pStrSave->BeginUnit(_T("LiftingCriteria"),1.0);
   m_LiftingCriteria.Save(pStrSave);
   pStrSave->EndUnit(); // LiftingCriteria

   pStrSave->BeginUnit(_T("HaulingCriteria"),1.0);
   m_HaulingCriteria.Save(pStrSave);
   pStrSave->EndUnit(); // HaulingCriteria

   // added in version 3 of the PGStable data block
   pStrSave->BeginUnit(_T("OneEndSeatedCriteria"), 1.0);
   m_OneEndSeatedCriteria.Save(pStrSave);
   pStrSave->EndUnit(); // OneEndSeatedCriteria


   hr = pStrSave->EndUnit();
   if ( FAILED(hr) )
      return hr;

   return S_OK;
}

HRESULT CPGStableModel::Load(IStructuredLoad* pStrLoad)
{
   CHRException hr;

   try
   {
      hr = pStrLoad->BeginUnit(_T("PGStable"));

      Float64 version;
      pStrLoad->get_Version(&version);

      CComVariant var;

      var.vt = VT_I4;
      hr = pStrLoad->get_Property(_T("Units"),&var);
      CEAFApp* pApp = EAFGetApp();
      pApp->SetUnitsMode(eafTypes::UnitMode(var.lVal));

      var.vt = VT_I4;
      hr = pStrLoad->get_Property(_T("GirderType"),&var);
      m_GirderType = (GirderType)var.lVal;

      if (1 < version)
      {
         // Added in version 2
         var.vt = VT_I4;
         hr = pStrLoad->get_Property(_T("StressPointType"), &var);
         m_StressPointType = var.intVal;
      }
      else
      {
         m_StressPointType = COMPUTE_STRESS_POINTS;
      }


      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("DragCoefficient"),&var);
      m_Girder[m_GirderType].SetDragCoefficient(var.dblVal);

      if (1 < version)
      {
         // added in version 2
         var.vt = VT_R8;
         hr = pStrLoad->get_Property(_T("Precamber"), &var);
         m_Girder[m_GirderType].SetPrecamber(var.dblVal);
      }

      hr = pStrLoad->BeginUnit(_T("Girder"));
      Float64 girder_datablock_version;
      pStrLoad->get_Version(&girder_datablock_version);

      hr = pStrLoad->BeginUnit(_T("Sections"));
      m_Girder[m_GirderType].ClearSections();
      while (SUCCEEDED(pStrLoad->BeginUnit(_T("Section"))) )
      {
         var.vt = VT_R8;
         hr = pStrLoad->get_Property(_T("L"),&var);
         Float64 L = var.dblVal;

         Float64 Ag[2],Ixx[2],Iyy[2],Ixy[2],Xleft[2],Ytop[2],Hg[2],Wtf[2],Wbf[2];
         WBFL::Stability::Point pntTL[2], pntTR[2], pntBL[2], pntBR[2];

         Ixy[0] = 0;
         Ixy[1] = 0;
         
         hr = pStrLoad->BeginUnit(_T("StartFace"));

         Float64 face_version;
         pStrLoad->get_Version(&face_version);

         hr = pStrLoad->get_Property(_T("Ag"),&var);
         Ag[+WBFL::Stability::Section::Start] = var.dblVal;

         hr = pStrLoad->get_Property(_T("Ix"),&var);
         Ixx[+WBFL::Stability::Section::Start] = var.dblVal;

         hr = pStrLoad->get_Property(_T("Iy"),&var);
         Iyy[+WBFL::Stability::Section::Start] = var.dblVal;

         if (1 < face_version)
         {
            // added in version 2
            hr = pStrLoad->get_Property(_T("Ixy"), &var);
            Ixy[+WBFL::Stability::Section::Start] = var.dblVal;
         }

         bool bApproximateXleft = false;
         if (face_version < 2)
         {
            bApproximateXleft = true;
            Xleft[+WBFL::Stability::Section::Start] = 0.0;

            hr = pStrLoad->get_Property(_T("Yt"), &var);
            Ytop[+WBFL::Stability::Section::Start] = var.dblVal;
         }
         else
         {
            hr = pStrLoad->get_Property(_T("Xleft"), &var);
            Xleft[+WBFL::Stability::Section::Start] = var.dblVal;

            hr = pStrLoad->get_Property(_T("Ytop"), &var);
            Ytop[+WBFL::Stability::Section::Start] = var.dblVal;
         }

         hr = pStrLoad->get_Property(_T("Hg"),&var);
         Hg[+WBFL::Stability::Section::Start] = var.dblVal;

         hr = pStrLoad->get_Property(_T("Wtf"),&var);
         Wtf[+WBFL::Stability::Section::Start] = var.dblVal;

         if (bApproximateXleft)
         {
            Xleft[+WBFL::Stability::Section::Start] = Wtf[+WBFL::Stability::Section::Start] / 2;
         }

         hr = pStrLoad->get_Property(_T("Wbf"),&var);
         Wbf[+WBFL::Stability::Section::Start] = var.dblVal;


         if (1 < face_version && m_StressPointType == DEFINE_STRESS_POINTS)
         {
            // added in version 2

            WBFL::Stability::Section section = WBFL::Stability::Section::Start;
            CComVariant varX, varY;
            varX.vt = VT_R8;
            varY.vt = VT_R8;
            pStrLoad->BeginUnit(_T("StressPoints"));
            pStrLoad->BeginUnit(_T("TopLeft"));
            pStrLoad->get_Property(_T("X"), &varX);
            pStrLoad->get_Property(_T("Y"), &varY);
            pntTL[+section].X() = varX.dblVal;
            pntTL[+section].Y() = varY.dblVal;
            pStrLoad->EndUnit(); // TopLeft

            pStrLoad->BeginUnit(_T("TopRight"));
            pStrLoad->get_Property(_T("X"), &varX);
            pStrLoad->get_Property(_T("Y"), &varY);
            pntTR[+section].X() = varX.dblVal;
            pntTR[+section].Y() = varY.dblVal;
            pStrLoad->EndUnit(); // TopRight

            pStrLoad->BeginUnit(_T("BottomLeft"));
            pStrLoad->get_Property(_T("X"), &varX);
            pStrLoad->get_Property(_T("Y"), &varY);
            pntBL[+section].X() = varX.dblVal;
            pntBL[+section].Y() = varY.dblVal;
            pStrLoad->EndUnit(); // BottomLeft

            pStrLoad->BeginUnit(_T("BottomRight"));
            pStrLoad->get_Property(_T("X"), &varX);
            pStrLoad->get_Property(_T("Y"), &varY);
            pntBR[+section].X() = varX.dblVal;
            pntBR[+section].Y() = varY.dblVal;
            pStrLoad->EndUnit(); // BottomRight

            pStrLoad->EndUnit(); // StressPoints
         }
         
         hr = pStrLoad->EndUnit(); // StartFace

         //////////////////////////////////////////////////////////////////////////

         hr = pStrLoad->BeginUnit(_T("EndFace"));
         pStrLoad->get_Version(&face_version);

         hr = pStrLoad->get_Property(_T("Ag"),&var);
         Ag[+WBFL::Stability::Section::End] = var.dblVal;

         hr = pStrLoad->get_Property(_T("Ix"),&var);
         Ixx[+WBFL::Stability::Section::End] = var.dblVal;

         hr = pStrLoad->get_Property(_T("Iy"),&var);
         Iyy[+WBFL::Stability::Section::End] = var.dblVal;

         if (1 < face_version)
         {
            // added in version 2
            hr = pStrLoad->get_Property(_T("Ixy"), &var);
            Ixy[+WBFL::Stability::Section::End] = var.dblVal;
         }

         bApproximateXleft = false;
         if (face_version < 2)
         {
            bApproximateXleft = true;
            Xleft[+WBFL::Stability::Section::End] = 0.0;

            hr = pStrLoad->get_Property(_T("Yt"), &var);
            Ytop[+WBFL::Stability::Section::End] = var.dblVal;
         }
         else
         {
            hr = pStrLoad->get_Property(_T("Xleft"), &var);
            Xleft[+WBFL::Stability::Section::End] = var.dblVal;

            hr = pStrLoad->get_Property(_T("Ytop"), &var);
            Ytop[+WBFL::Stability::Section::End] = var.dblVal;
         }

         hr = pStrLoad->get_Property(_T("Hg"),&var);
         Hg[+WBFL::Stability::Section::End] = var.dblVal;

         hr = pStrLoad->get_Property(_T("Wtf"),&var);
         Wtf[+WBFL::Stability::Section::End] = var.dblVal;

         if (bApproximateXleft)
         {
            Xleft[+WBFL::Stability::Section::End] = Wtf[+WBFL::Stability::Section::End] / 2;
         }

         hr = pStrLoad->get_Property(_T("Wbf"),&var);
         Wbf[+WBFL::Stability::Section::End] = var.dblVal;

         if (1 < face_version && m_StressPointType == DEFINE_STRESS_POINTS)
         {
            // added in version 2

            WBFL::Stability::Section section = WBFL::Stability::Section::End;
            CComVariant varX, varY;
            varX.vt = VT_R8;
            varY.vt = VT_R8;
            pStrLoad->BeginUnit(_T("StressPoints"));
            pStrLoad->BeginUnit(_T("TopLeft"));
            pStrLoad->get_Property(_T("X"), &varX);
            pStrLoad->get_Property(_T("Y"), &varY);
            pntTL[+section].X() = varX.dblVal;
            pntTL[+section].Y() = varY.dblVal;
            pStrLoad->EndUnit(); // TopLeft

            pStrLoad->BeginUnit(_T("TopRight"));
            pStrLoad->get_Property(_T("X"), &varX);
            pStrLoad->get_Property(_T("Y"), &varY);
            pntTR[+section].X() = varX.dblVal;
            pntTR[+section].Y() = varY.dblVal;
            pStrLoad->EndUnit(); // TopRight

            pStrLoad->BeginUnit(_T("BottomLeft"));
            pStrLoad->get_Property(_T("X"), &varX);
            pStrLoad->get_Property(_T("Y"), &varY);
            pntBL[+section].X() = varX.dblVal;
            pntBL[+section].Y() = varY.dblVal;
            pStrLoad->EndUnit(); // BottomLeft

            pStrLoad->BeginUnit(_T("BottomRight"));
            pStrLoad->get_Property(_T("X"), &varX);
            pStrLoad->get_Property(_T("Y"), &varY);
            pntBR[+section].X() = varX.dblVal;
            pntBR[+section].Y() = varY.dblVal;
            pStrLoad->EndUnit(); // BottomRight

            pStrLoad->EndUnit(); // StressPoints
         }

         hr = pStrLoad->EndUnit(); // EndFace

         IndexType sectIdx = m_Girder[m_GirderType].AddSection(L,Ag[+WBFL::Stability::Section::Start],
                                                            Ixx[+WBFL::Stability::Section::Start],
                                                            Iyy[+WBFL::Stability::Section::Start],
                                                            Ixy[+WBFL::Stability::Section::Start],
                                                            Xleft[+WBFL::Stability::Section::Start],
                                                            Ytop[+WBFL::Stability::Section::Start],
                                                            Hg[+WBFL::Stability::Section::Start],
                                                            Wtf[+WBFL::Stability::Section::Start],
                                                            Wbf[+WBFL::Stability::Section::Start],
                                                            Ag[+WBFL::Stability::Section::End],
                                                            Ixx[+WBFL::Stability::Section::End],
                                                            Iyy[+WBFL::Stability::Section::End],
                                                            Ixy[+WBFL::Stability::Section::End],
                                                            Xleft[+WBFL::Stability::Section::End],
                                                            Ytop[+WBFL::Stability::Section::End],
                                                            Hg[+WBFL::Stability::Section::End],
                                                            Wtf[+WBFL::Stability::Section::End],
                                                            Wbf[+WBFL::Stability::Section::End]);

         if (m_StressPointType == DEFINE_STRESS_POINTS)
         {
            m_Girder[m_GirderType].SetStressPoints(sectIdx, pntTL[+WBFL::Stability::Section::Start], pntTR[+WBFL::Stability::Section::Start], pntBL[+WBFL::Stability::Section::Start], pntBR[+WBFL::Stability::Section::Start], pntTL[+WBFL::Stability::Section::End], pntTR[+WBFL::Stability::Section::End], pntBL[+WBFL::Stability::Section::End], pntBR[+WBFL::Stability::Section::End]);
         }


         hr = pStrLoad->EndUnit(); // Section
      }
      hr = pStrLoad->EndUnit(); // Sections


      hr = pStrLoad->get_Property(_T("DensityWithRebar"),&var);
      SetDensityWithRebar(var.dblVal);

      hr = pStrLoad->get_Property(_T("Density"),&var);
      SetDensity(var.dblVal);

      hr = pStrLoad->get_Property(_T("K1"),&var);
      m_K1 = var.dblVal;

      hr = pStrLoad->get_Property(_T("K2"),&var);
      m_K2 = var.dblVal;

      if (1 < girder_datablock_version) // added in version 2
      {
         CComVariant concrete_var;
         concrete_var.vt = VT_I8;
         hr = pStrLoad->get_Property(_T("ConcreteType"), &concrete_var);
         SetConcreteType((WBFL::Materials::ConcreteType)concrete_var.lVal);
      }

      m_Girder[m_GirderType].ClearPointLoads();
      HRESULT hrAdditionalLoads = pStrLoad->BeginUnit(_T("AdditionalLoads"));
      if ( SUCCEEDED(hrAdditionalLoads) )
      {
         while( SUCCEEDED(pStrLoad->BeginUnit(_T("AdditionalLoad"))) )
         {
            Float64 X,P;
            hr = pStrLoad->get_Property(_T("X"),&var);
            X = var.dblVal;

            hr = pStrLoad->get_Property(_T("P"),&var);
            P = var.dblVal;

            m_Girder[m_GirderType].AddPointLoad(X,P);
            hr = pStrLoad->EndUnit(); // AdditionalLoad
         }
         hr = pStrLoad->EndUnit(); // AdditionalLoads
      }

      hr = pStrLoad->EndUnit(); // Girder


      {
      hr = pStrLoad->BeginUnit(_T("LiftingProblem"));

      Float64 version;
      pStrLoad->get_Version(&version);
      hr = m_Strands[m_GirderType][ModelType::Lifting].Load(pStrLoad);

      if (version < 4)
      {
         // removed in version 4
         bool bDirectCamber;
         var.vt = VT_BOOL;
         hr = pStrLoad->get_Property(_T("DirectCamber"), &var);
         bDirectCamber = (var.boolVal == VARIANT_TRUE ? true : false);
      }

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("Camber"),&var);
      Float64 camber;
      camber = var.dblVal;

      m_LiftingStabilityProblem.SetCamber(camber);

      if (2 < version)
      {
         // added in version 3
         var.vt = VT_R8;
         hr = pStrLoad->get_Property(_T("LateralCamber"), &var);
         m_LiftingStabilityProblem.SetLateralCamber(var.dblVal);
      }

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("LiftPoint"),&var);
      m_LiftingStabilityProblem.SetSupportLocations(var.dblVal,var.dblVal);

      hr = pStrLoad->get_Property(_T("Yra"),&var);
      m_LiftingStabilityProblem.SetYRollAxis(var.dblVal);

      hr = pStrLoad->get_Property(_T("SweepTolerance"),&var);
      m_LiftingStabilityProblem.SetSweepTolerance(var.dblVal);

      if (4 < version)
      {
         // added in version 5
         hr = pStrLoad->get_Property(_T("SweepGrowth"), &var);
         m_LiftingStabilityProblem.SetSweepGrowth(var.dblVal);
      }

      hr = pStrLoad->get_Property(_T("SupportPlacementTolerance"),&var);
      m_LiftingStabilityProblem.SetSupportPlacementTolerance(var.dblVal);


      Float64 imup,imdn;
      hr = pStrLoad->get_Property(_T("ImpactUp"),&var);
      imup = var.dblVal;
      hr = pStrLoad->get_Property(_T("ImpactDown"),&var);
      imdn = var.dblVal;
      m_LiftingStabilityProblem.SetImpact(imup,imdn);

      // this changed in version 2
      var.vt = VT_BOOL;
      if (version < 2)
      {
         // before version 2... need to change the scense of the boolean to match its current meaning
         hr = pStrLoad->get_Property(_T("PlumbGirderStresses"), &var);
         var.boolVal = (var.boolVal == VARIANT_TRUE ? VARIANT_FALSE : VARIANT_TRUE);
      }
      else if (version < 4)
      {
         // new in version 2, removed in version 4
         hr = pStrLoad->get_Property(_T("EvaluateStressesAtEquilibriumAngle"), &var);
      }
      //m_LiftingStabilityProblem.EvaluateStressesAtEquilibriumAngle(var.boolVal == VARIANT_TRUE ? true : false); // removed in version 4

      var.vt = VT_I4;
      hr = pStrLoad->get_Property(_T("WindLoadType"),&var);
      WBFL::Stability::WindLoadType windLoadType = (WBFL::Stability::WindLoadType)var.lVal;

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("WindLoad"),&var);
      Float64 windLoad = var.dblVal;
      m_LiftingStabilityProblem.SetWindLoading(windLoadType,windLoad);

      if (5 < version)
      {
         // added in version 6 of this datablock
         var.vt = VT_R8;
         hr = pStrLoad->get_Property(_T("BracketEccentricity"), &var);
         Float64 eb = var.dblVal;
         hr = pStrLoad->get_Property(_T("BracketWeight"), &var);
         Float64 wb = var.dblVal;
         m_LiftingStabilityProblem.SetAppurtenanceLoading(eb, wb);
      }

      hr = pStrLoad->get_Property(_T("LiftAngle"),&var);
      m_LiftingStabilityProblem.SetLiftAngle(var.dblVal);

      hr = pStrLoad->get_Property(_T("FrCoefficient"),&var);
      m_LiftingFrCoefficient = var.dblVal;

      hr = pStrLoad->get_Property(_T("Fci"),&var);
      Float64 fci = var.dblVal;
      
      var.vt = VT_BOOL;
      hr = pStrLoad->get_Property(_T("ComputeEci"),&var);
      m_bComputeEci = (var.boolVal == VARIANT_TRUE ? true : false);

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("Eci"),&var);
      Float64 Eci = var.dblVal;

      m_LiftingStabilityProblem.GetConcrete().SetFc(fci);
      m_LiftingStabilityProblem.GetConcrete().SetE(Eci);
      hr = pStrLoad->EndUnit(); // LiftingProblem
      }

      /////////////////////////////////////////////////////////////////

      {
      hr = pStrLoad->BeginUnit(_T("HaulingProblem"));

      Float64 version;
      pStrLoad->get_Version(&version);
      hr = m_Strands[m_GirderType][ModelType::Hauling].Load(pStrLoad);

      if (version < 4)
      {
         // removed in version 4
         bool bDirectCamber;
         var.vt = VT_BOOL;
         hr = pStrLoad->get_Property(_T("DirectCamber"), &var);
         bDirectCamber = (var.boolVal == VARIANT_TRUE ? true : false);
      }

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("Camber"),&var);
      Float64 camber;
      camber = var.dblVal;

      m_HaulingStabilityProblem.SetCamber(camber);

      if (2 < version)
      {
         // added in version 3
         var.vt = VT_R8;
         hr = pStrLoad->get_Property(_T("LateralCamber"), &var);
         m_HaulingStabilityProblem.SetLateralCamber(var.dblVal);
      }

      Float64 Ll,Lr;
      ATLASSERT(IsEqual(Ll,Lr));
      hr = pStrLoad->get_Property(_T("LeftBunkPoint"),&var);
      Ll = var.dblVal;
      hr = pStrLoad->get_Property(_T("RightBunkPoint"),&var);
      Lr = var.dblVal;
      m_HaulingStabilityProblem.SetSupportLocations(Ll,Lr);

      hr = pStrLoad->get_Property(_T("Hgb"),&var);
      m_Hgb = var.dblVal;

      hr = pStrLoad->get_Property(_T("SweepTolerance"),&var);
      m_HaulingStabilityProblem.SetSweepTolerance(var.dblVal);

      if (4 < version)
      {
         // added in version 5
         hr = pStrLoad->get_Property(_T("SweepGrowth"), &var);
         m_HaulingStabilityProblem.SetSweepGrowth(var.dblVal);
      }

      hr = pStrLoad->get_Property(_T("SupportPlacementTolerance"),&var);
      m_HaulingStabilityProblem.SetSupportPlacementTolerance(var.dblVal);

      Float64 imup,imdn;
      hr = pStrLoad->get_Property(_T("ImpactUp"),&var);
      imup = var.dblVal;
      hr = pStrLoad->get_Property(_T("ImpactDown"),&var);
      imdn = var.dblVal;
      m_HaulingStabilityProblem.SetImpact(imup,imdn);

      var.vt = VT_I4;
      hr = pStrLoad->get_Property(_T("ImpactUsage"),&var);
      m_HaulingStabilityProblem.SetImpactUsage((WBFL::Stability::HaulingImpact)var.lVal);

      if (1 < version && version < 4)
      {
         // added in version 2, removed in version 4
         hr = pStrLoad->get_Property(_T("EvaluateStressesAtEquilibriumAngle"), &var);
         //m_HaulingStabilityProblem.EvaluateStressesAtEquilibriumAngle(WBFL::Stability::CrownSlope,var.boolVal == VARIANT_TRUE ? true : false);
      }

      var.vt = VT_I4;
      hr = pStrLoad->get_Property(_T("WindLoadType"),&var);
      WBFL::Stability::WindLoadType windLoadType = (WBFL::Stability::WindLoadType)var.lVal;

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("WindLoad"),&var);
      Float64 windLoad = var.dblVal;
      m_HaulingStabilityProblem.SetWindLoading(windLoadType,windLoad);

      if (5 < version)
      {
         // added in version 6 of this datablock
         var.vt = VT_R8;
         hr = pStrLoad->get_Property(_T("BracketEccentricity"), &var);
         Float64 eb = var.dblVal;
         hr = pStrLoad->get_Property(_T("BracketWeight"), &var);
         Float64 wb = var.dblVal;
         m_HaulingStabilityProblem.SetAppurtenanceLoading(eb, wb);
      }

      hr = pStrLoad->get_Property(_T("Ktheta"),&var);
      m_HaulingStabilityProblem.SetRotationalStiffness(var.dblVal);

      hr = pStrLoad->get_Property(_T("Wcc"),&var);
      m_HaulingStabilityProblem.SetSupportWidth(var.dblVal);

      hr = pStrLoad->get_Property(_T("Hrc"),&var);
      m_HaulingStabilityProblem.SetHeightOfRollAxis(var.dblVal);

      hr = pStrLoad->get_Property(_T("CrownSlope"),&var);
      m_HaulingStabilityProblem.SetSupportSlope(var.dblVal);

      hr = pStrLoad->get_Property(_T("Superelevation"),&var);
      m_HaulingStabilityProblem.SetSuperelevation(var.dblVal);

      hr = pStrLoad->get_Property(_T("Velocity"),&var);
      m_HaulingStabilityProblem.SetVelocity(var.dblVal);

      hr = pStrLoad->get_Property(_T("Radius"),&var);
      m_HaulingStabilityProblem.SetTurningRadius(var.dblVal);

      var.vt = VT_I4;
      hr = pStrLoad->get_Property(_T("CFType"),&var);
      m_HaulingStabilityProblem.SetCentrifugalForceType((WBFL::Stability::CFType)var.lVal);

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("FrCoefficient"),&var);
      m_HaulingFrCoefficient = var.dblVal;

      hr = pStrLoad->get_Property(_T("Fc"),&var);
      Float64 fc = var.dblVal;

      var.vt = VT_BOOL;
      hr = pStrLoad->get_Property(_T("ComputeEc"),&var);
      m_bComputeEc = (var.boolVal == VARIANT_TRUE ? true : false);

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("Ec"),&var);
      Float64 Ec = var.dblVal;

      m_HaulingStabilityProblem.GetConcrete().SetFc(fc);
      m_HaulingStabilityProblem.GetConcrete().SetE(Ec);

      hr = pStrLoad->EndUnit(); // HaulingProblem
      }
      
      /////////////////////////////////////////////////////////////////
      // added in version 3
      if(2 < version)
      {
         hr = pStrLoad->BeginUnit(_T("OneEndSeatedProblem"));

         hr = m_Strands[m_GirderType][ModelType::OneEndSeated].Load(pStrLoad);

         var.vt = VT_R8;
         hr = pStrLoad->get_Property(_T("Camber"), &var);
         Float64 camber;
         camber = var.dblVal;

         m_OneEndSeatedStabilityProblem.SetCamber(camber);

         var.vt = VT_R8;
         hr = pStrLoad->get_Property(_T("LateralCamber"), &var);
         m_OneEndSeatedStabilityProblem.SetLateralCamber(var.dblVal);

         Float64 Ll, Lr;
         ATLASSERT(IsEqual(Ll, Lr));
         hr = pStrLoad->get_Property(_T("LeftBunkPoint"), &var);
         Ll = var.dblVal;
         hr = pStrLoad->get_Property(_T("RightBunkPoint"), &var);
         Lr = var.dblVal;
         m_OneEndSeatedStabilityProblem.SetSupportLocations(Ll, Lr);

         hr = pStrLoad->get_Property(_T("Hgb"), &var);
         m_Hgb = var.dblVal;

         hr = pStrLoad->get_Property(_T("SweepTolerance"), &var);
         m_OneEndSeatedStabilityProblem.SetSweepTolerance(var.dblVal);

         hr = pStrLoad->get_Property(_T("SweepGrowth"), &var);
         m_OneEndSeatedStabilityProblem.SetSweepGrowth(var.dblVal);

         hr = pStrLoad->get_Property(_T("SupportPlacementTolerance"), &var);
         m_OneEndSeatedStabilityProblem.SetSupportPlacementTolerance(var.dblVal);

         hr = pStrLoad->get_Property(_T("YrollLiftEnd"), &var);
         m_OneEndSeatedStabilityProblem.SetYRollLiftEnd(var.dblVal);

         hr = pStrLoad->get_Property(_T("LiftPlacementTolerance"), &var);
         m_OneEndSeatedStabilityProblem.SetLiftPlacementTolerance(var.dblVal);

         Float64 imup, imdn;
         hr = pStrLoad->get_Property(_T("ImpactUp"), &var);
         imup = var.dblVal;
         hr = pStrLoad->get_Property(_T("ImpactDown"), &var);
         imdn = var.dblVal;
         m_OneEndSeatedStabilityProblem.SetImpact(imup, imdn);

         var.vt = VT_I4;
         hr = pStrLoad->get_Property(_T("WindLoadType"), &var);
         WBFL::Stability::WindLoadType windLoadType = (WBFL::Stability::WindLoadType)var.lVal;

         var.vt = VT_R8;
         hr = pStrLoad->get_Property(_T("WindLoad"), &var);
         Float64 windLoad = var.dblVal;
         m_OneEndSeatedStabilityProblem.SetWindLoading(windLoadType, windLoad);

         var.vt = VT_R8;
         hr = pStrLoad->get_Property(_T("BracketEccentricity"), &var);
         Float64 eb = var.dblVal;
         hr = pStrLoad->get_Property(_T("BracketWeight"), &var);
         Float64 wb = var.dblVal;
         m_OneEndSeatedStabilityProblem.SetAppurtenanceLoading(eb, wb);

         hr = pStrLoad->get_Property(_T("Ktheta"), &var);
         m_OneEndSeatedStabilityProblem.SetRotationalStiffness(var.dblVal);

         hr = pStrLoad->get_Property(_T("Kadjust"), &var);
         m_OneEndSeatedStabilityProblem.SetRotationalStiffnessAdjustmentFactor(var.dblVal);

         hr = pStrLoad->get_Property(_T("Wcc"), &var);
         m_OneEndSeatedStabilityProblem.SetSupportWidth(var.dblVal);

         hr = pStrLoad->get_Property(_T("Hrc"), &var);
         m_OneEndSeatedStabilityProblem.SetHeightOfRollAxis(var.dblVal);

         hr = pStrLoad->get_Property(_T("CrownSlope"), &var);
         m_OneEndSeatedStabilityProblem.SetSupportSlope(var.dblVal);

         var.vt = VT_R8;
         hr = pStrLoad->get_Property(_T("FrCoefficient"), &var);
         m_OneEndSeatedFrCoefficient = var.dblVal;

         hr = pStrLoad->get_Property(_T("Fc"), &var);
         Float64 fc = var.dblVal;

         var.vt = VT_BOOL;
         hr = pStrLoad->get_Property(_T("ComputeEc"), &var);
         m_bComputeEcOneEndSeated = (var.boolVal == VARIANT_TRUE ? true : false);

         var.vt = VT_R8;
         hr = pStrLoad->get_Property(_T("Ec"), &var);
         Float64 Ec = var.dblVal;

         m_OneEndSeatedStabilityProblem.GetConcrete().SetFc(fc);
         m_OneEndSeatedStabilityProblem.GetConcrete().SetE(Ec);

         hr = pStrLoad->EndUnit(); // OneEndSeatedProblem
      }



      hr = pStrLoad->BeginUnit(_T("LiftingCriteria"));
      hr = m_LiftingCriteria.Load(pStrLoad);
      hr = pStrLoad->EndUnit(); // LiftingCriteria
      
      hr = pStrLoad->BeginUnit(_T("HaulingCriteria"));
      hr = m_HaulingCriteria.Load(pStrLoad);
      hr = pStrLoad->EndUnit(); // HaulingCriteria

      if (2 < version)
      {
         // added in version 3
         hr = pStrLoad->BeginUnit(_T("OneEndSeatedCriteria"));
         hr = m_OneEndSeatedCriteria.Load(pStrLoad);
         hr = pStrLoad->EndUnit(); // OneEndSeatedCriteria
      }

      hr = pStrLoad->EndUnit();
   }
   catch(...)
   {
      THROW_LOAD(InvalidFileFormat,pStrLoad);
   }

   if ( m_GirderType == GirderType::Prismatic )
   {
      m_Girder[GirderType::Nonprismatic] = m_Girder[GirderType::Prismatic];
      m_Strands[GirderType::Nonprismatic][ModelType::Lifting] = m_Strands[GirderType::Prismatic][ModelType::Lifting];
      m_Strands[GirderType::Nonprismatic][ModelType::Hauling] = m_Strands[GirderType::Prismatic][ModelType::Hauling];
   }
   else
   {
      m_Girder[GirderType::Prismatic] = m_Girder[GirderType::Nonprismatic];

      Float64 L = m_Girder[GirderType::Nonprismatic].GetSectionLength(0);
      Float64 Ag,Ixx,Iyy,Ixy,Xleft,Ytop,Hg,Wtf,Wbf;
      m_Girder[GirderType::Nonprismatic].GetSectionProperties(0,WBFL::Stability::Section::Start,&Ag,&Ixx,&Iyy,&Ixy,&Xleft,&Ytop,&Hg,&Wtf,&Wbf);

      m_Girder[GirderType::Prismatic].ClearSections();
      m_Girder[GirderType::Prismatic].AddSection(L,Ag,Ixx,Iyy,Ixy,Xleft,Ytop,Hg,Wtf,Wbf);
   }

   return S_OK;
}
