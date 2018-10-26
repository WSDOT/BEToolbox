///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2017  Washington State Department of Transportation
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

CPGStableModel::CPGStableModel()
{
   m_GirderType = PRISMATIC;

   Float64 Hg  = ::ConvertToSysUnits(72,unitMeasure::Inch);
   Float64 Wtf = ::ConvertToSysUnits(42,unitMeasure::Inch);
   Float64 Wbf = ::ConvertToSysUnits(26,unitMeasure::Inch);
   
   Float64 Ag = ::ConvertToSysUnits(767,unitMeasure::Inch2);
   Float64 Ix = ::ConvertToSysUnits(545894,unitMeasure::Inch4);
   Float64 Iy = ::ConvertToSysUnits(37634,unitMeasure::Inch4);
   Float64 Yt = ::ConvertToSysUnits(36.6-72,unitMeasure::Inch); // want neg because we are in section coordinates
   Float64 L = ::ConvertToSysUnits(100,unitMeasure::Feet);
   m_Girder[PRISMATIC].AddSection(L,Ag,Ix,Iy,Yt,Hg,Wtf,Wbf);

   m_Girder[NONPRISMATIC] = m_Girder[PRISMATIC];

   matConcreteEx liftingConcrete, haulingConcrete;

   Float64 density = ::ConvertToSysUnits(0.155,unitMeasure::KipPerFeet3); // without rebar (used to compute Ec)
   Float64 densityWithRebar = ::ConvertToSysUnits(0.160,unitMeasure::KipPerFeet3); // including allowance for rebar (used for computing dead load)
   liftingConcrete.SetDensity(density);
   haulingConcrete.SetDensity(density);
   liftingConcrete.SetDensityForWeight(densityWithRebar);
   haulingConcrete.SetDensityForWeight(densityWithRebar);


   m_LiftingFrCoefficient = ::ConvertToSysUnits(0.24,unitMeasure::SqrtKSI);
   m_HaulingFrCoefficient = ::ConvertToSysUnits(0.24,unitMeasure::SqrtKSI);
   m_bComputeEci = true;
   m_bComputeEc = true;
   Float64 fci = ::ConvertToSysUnits(5.5,unitMeasure::KSI);
   Float64 fc  = ::ConvertToSysUnits(7.0,unitMeasure::KSI);
   m_K1 = 1.0;
   m_K2 = 1.0;

   liftingConcrete.SetFc(fci);
   haulingConcrete.SetFc(fc);

   Float64 Eci = ::lrfdConcreteUtil::ModE(fci,density,false/*ignore LRFD range checks*/);
   Float64 Ec  = ::lrfdConcreteUtil::ModE(fc, density,false/*ignore LRFD range checks*/);

   liftingConcrete.SetE(Eci);
   haulingConcrete.SetE(Ec);

   liftingConcrete.SetFlexureFr(::ConvertToSysUnits(0.24*sqrt(fci),unitMeasure::KSI));
   haulingConcrete.SetFlexureFr(::ConvertToSysUnits(0.24*sqrt(fc),unitMeasure::KSI));

   m_LiftingStabilityProblem.SetConcrete(liftingConcrete);
   m_HaulingStabilityProblem.SetConcrete(haulingConcrete);

   m_LiftingStabilityProblem.SetCamber(true,::ConvertToSysUnits(0,unitMeasure::Inch));
   m_LiftingStabilityProblem.SetSweepTolerance(::ConvertToSysUnits(0.125,unitMeasure::Inch)/::ConvertToSysUnits(10.0,unitMeasure::Feet));
   m_LiftingStabilityProblem.SetSupportPlacementTolerance( ::ConvertToSysUnits(0.25,unitMeasure::Inch) );
   m_LiftingStabilityProblem.SetSupportLocations(::ConvertToSysUnits(3,unitMeasure::Feet),::ConvertToSysUnits(3,unitMeasure::Feet));
   m_LiftingStabilityProblem.SetLiftAngle(PI_OVER_2);
   m_LiftingStabilityProblem.SetYRollAxis(::ConvertToSysUnits(0.0,unitMeasure::Inch));
   m_LiftingStabilityProblem.SetImpact(0,0);
   m_LiftingStabilityProblem.EvaluateStressesAtEquilibriumAngle(true);

   m_HaulingStabilityProblem.SetCamber(true,::ConvertToSysUnits(0,unitMeasure::Inch));

   m_HaulingStabilityProblem.SetSupportLocations(::ConvertToSysUnits(5,unitMeasure::Feet),::ConvertToSysUnits(5,unitMeasure::Feet));
   m_HaulingStabilityProblem.SetSweepTolerance(::ConvertToSysUnits(0.125,unitMeasure::Inch)/::ConvertToSysUnits(10.0,unitMeasure::Feet));
   m_HaulingStabilityProblem.SetSupportPlacementTolerance(::ConvertToSysUnits(1.0,unitMeasure::Inch));
   m_HaulingStabilityProblem.SetTruckRotationalStiffness(::ConvertToSysUnits(40500.,unitMeasure::KipInchPerRadian));
   m_HaulingStabilityProblem.SetCrownSlope(0.02);
   m_HaulingStabilityProblem.SetSuperelevation(0.06);
   m_HaulingStabilityProblem.SetWheelLineSpacing(::ConvertToSysUnits(72.,unitMeasure::Inch));
   m_HaulingStabilityProblem.SetHeightOfRollAxisAboveRoadway(::ConvertToSysUnits(24.,unitMeasure::Inch));
   m_HaulingStabilityProblem.SetImpact(0,0);
   m_HaulingStabilityProblem.SetImpactUsage(stbTypes::NormalCrown);
   m_HaulingStabilityProblem.SetVelocity(0);
   m_HaulingStabilityProblem.SetTurningRadius(::ConvertToSysUnits(100,unitMeasure::Feet));
   m_HaulingStabilityProblem.EvaluateStressesAtEquilibriumAngle(stbTypes::CrownSlope, true);
   m_HaulingStabilityProblem.EvaluateStressesAtEquilibriumAngle(stbTypes::Superelevation, true);

   m_Hgb = ::ConvertToSysUnits(72.0,unitMeasure::Inch);

   m_LiftingCriteria.bMaxTension = true;


   m_DocUnitServer.CoCreateInstance(CLSID_UnitServer);
   m_DocUnitServer->SetBaseUnits(CComBSTR(unitSysUnitsMgr::GetMassUnit().UnitTag().c_str()),
                            CComBSTR(unitSysUnitsMgr::GetLengthUnit().UnitTag().c_str()),
                            CComBSTR(unitSysUnitsMgr::GetTimeUnit().UnitTag().c_str()),
                            CComBSTR(unitSysUnitsMgr::GetTemperatureUnit().UnitTag().c_str()),
                            CComBSTR(unitSysUnitsMgr::GetAngleUnit().UnitTag().c_str()));  
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


stbLiftingResults CPGStableModel::GetLiftingResults() const
{
   stbLiftingCheckArtifact artifact = GetLiftingCheckArtifact();
   return artifact.GetLiftingResults();
}

stbLiftingCheckArtifact CPGStableModel::GetLiftingCheckArtifact() const
{
   matConcreteEx concrete = m_LiftingStabilityProblem.GetConcrete();
   Float64 fci = concrete.GetFc();
   if ( m_bComputeEci )
   {
      Float64 density = concrete.GetDensity();
      Float64 Ec = lrfdConcreteUtil::ModE(fci,density,false/*ignore LRFD range checks*/);
      if ( lrfdVersionMgr::ThirdEditionWith2005Interims <= lrfdVersionMgr::GetVersion() )
      {
         Ec *= m_K1*m_K2;
      }
      concrete.SetE(Ec);
   }

   Float64 fr = ::lrfdConcreteUtil::ModRupture(fci,m_LiftingFrCoefficient);
   concrete.SetFlexureFr(fr);

   concrete.SetLambda(lrfdConcreteUtil::ComputeConcreteDensityModificationFactor(matConcrete::Normal,concrete.GetDensity(),false,0,0));

   m_LiftingStabilityProblem.SetConcrete(concrete);

   stbGirder* pGirder = &m_Girder[m_GirderType];
   Float64 L = pGirder->GetGirderLength();
   m_LiftingStabilityProblem.ClearAnalysisPoints();
   int n = 10;
   for ( int i = 0; i <= n; i++ )
   {
      Float64 X = i*L/n;
      m_LiftingStabilityProblem.AddAnalysisPoint(new stbAnalysisPoint(X));
   }

   // when the lifting cables are inclined, the moment diagram due to the horizontal component of the lift force
   // jumps from zero to some value at the lift point. We need to add analysis points just outside of the lift
   // point to capture the effect of the jump.
   bool bAddExtraLiftPointAnalysisPoints(IsEqual(m_LiftingStabilityProblem.GetLiftAngle(), PI_OVER_2) ? false : true);

   Float64 Ll, Lr;
   m_LiftingStabilityProblem.GetSupportLocations(&Ll,&Lr);
   Float64 offset = ::ConvertToSysUnits(0.001, unitMeasure::Feet);

   if (bAddExtraLiftPointAnalysisPoints)
   {
      m_LiftingStabilityProblem.AddAnalysisPoint(new stbAnalysisPoint(Ll - offset));
   }

   m_LiftingStabilityProblem.AddAnalysisPoint(new stbAnalysisPoint(Ll));
   m_LiftingStabilityProblem.AddAnalysisPoint(new stbAnalysisPoint(L - Lr));

   if (bAddExtraLiftPointAnalysisPoints)
   {
      m_LiftingStabilityProblem.AddAnalysisPoint(new stbAnalysisPoint(L - Lr + offset));
   }

   ResolveLiftingStrandLocations();

   // criteria
   m_LiftingCriteria.AllowableCompression = -m_LiftingCriteria.CompressionCoefficient*fci;
   m_LiftingCriteria.AllowableTension = m_LiftingCriteria.TensionCoefficient*sqrt(fci);
   if ( m_LiftingCriteria.bMaxTension )
   {
      m_LiftingCriteria.AllowableTension = Min(m_LiftingCriteria.AllowableTension,m_LiftingCriteria.MaxTension);
   }
   m_LiftingCriteria.AllowableTensionWithRebar = m_LiftingCriteria.TensionCoefficientWithRebar*sqrt(fci);
   m_LiftingCriteria.Lambda = concrete.GetLambda();

   stbStabilityEngineer stabilityEngineer(m_DocConvert);
   stbLiftingCheckArtifact artifact = stabilityEngineer.CheckLifting(pGirder,&m_LiftingStabilityProblem,m_LiftingCriteria);
   return artifact;
}

stbHaulingResults CPGStableModel::GetHaulingResults() const
{
   stbHaulingCheckArtifact artifact = GetHaulingCheckArtifact();
   return artifact.GetHaulingResults();
}

stbHaulingCheckArtifact CPGStableModel::GetHaulingCheckArtifact() const
{
   matConcreteEx concrete = m_HaulingStabilityProblem.GetConcrete();
   Float64 fc = concrete.GetFc();
   if ( m_bComputeEc )
   {
      Float64 density = concrete.GetDensity();
      Float64 Ec = ::lrfdConcreteUtil::ModE(fc,density,false/*ignore LRFD range checks*/);
      if ( lrfdVersionMgr::ThirdEditionWith2005Interims <= lrfdVersionMgr::GetVersion() )
      {
         Ec *= m_K1*m_K2;
      }

      concrete.SetE(Ec);
   }

   Float64 fr = ::lrfdConcreteUtil::ModRupture(fc,m_HaulingFrCoefficient);
   concrete.SetFlexureFr(fr);

   concrete.SetLambda(lrfdConcreteUtil::ComputeConcreteDensityModificationFactor(matConcrete::Normal,concrete.GetDensity(),false,0,0));

   m_HaulingStabilityProblem.SetConcrete(concrete);

   stbGirder* pGirder = &m_Girder[m_GirderType];
   Float64 L = pGirder->GetGirderLength();
   m_HaulingStabilityProblem.ClearAnalysisPoints();
   int n = 10;
   for ( int i = 0; i <= n; i++ )
   {
      Float64 X = i*L/n;
      m_HaulingStabilityProblem.AddAnalysisPoint(new stbAnalysisPoint(X));
   }

   Float64 Ll, Lr;
   m_HaulingStabilityProblem.GetSupportLocations(&Ll,&Lr);
   m_HaulingStabilityProblem.AddAnalysisPoint(new stbAnalysisPoint(Ll));
   m_HaulingStabilityProblem.AddAnalysisPoint(new stbAnalysisPoint(L-Lr));

   Float64 Ag,Ix,Iy,Yt,Hg,Wtop,Wbot;
   pGirder->GetSectionProperties(0,stbTypes::Start,&Ag,&Ix,&Iy,&Yt,&Hg,&Wtop,&Wbot);
   Float64 Yra = -(Hg + m_Hgb) + m_HaulingStabilityProblem.GetHeightOfRollAxisAboveRoadway();
   ATLASSERT(Yra < 0);
   m_HaulingStabilityProblem.SetYRollAxis(Yra); // location of roll axes relative to the top of girder

   ResolveHaulingStrandLocations();

   m_HaulingCriteria.AllowableCompression = -m_HaulingCriteria.CompressionCoefficient*fc;

   m_HaulingCriteria.AllowableTension[stbTypes::CrownSlope] = m_HaulingCriteria.TensionCoefficient[stbTypes::CrownSlope]*sqrt(fc);
   if ( m_HaulingCriteria.bMaxTension[stbTypes::CrownSlope] )
   {
      m_HaulingCriteria.AllowableTension[stbTypes::CrownSlope] = Min(m_HaulingCriteria.AllowableTension[stbTypes::CrownSlope],m_HaulingCriteria.MaxTension[stbTypes::CrownSlope]);
   }
   m_HaulingCriteria.AllowableTensionWithRebar[stbTypes::CrownSlope] = m_HaulingCriteria.TensionCoefficientWithRebar[stbTypes::CrownSlope]*sqrt(fc);


   m_HaulingCriteria.AllowableTension[stbTypes::MaxSuper] = concrete.GetFlexureFr();
   m_HaulingCriteria.TensionCoefficient[stbTypes::MaxSuper] = m_HaulingFrCoefficient;
   m_HaulingCriteria.bMaxTension[stbTypes::MaxSuper] = false;
   m_HaulingCriteria.AllowableTensionWithRebar[stbTypes::MaxSuper] = concrete.GetFlexureFr();
   m_HaulingCriteria.TensionCoefficientWithRebar[stbTypes::MaxSuper] = m_HaulingFrCoefficient;

   m_HaulingCriteria.Lambda = concrete.GetLambda();

   stbStabilityEngineer stabilityEngineer(m_DocConvert);
   stbHaulingCheckArtifact artifact = stabilityEngineer.CheckHauling(pGirder,&m_HaulingStabilityProblem,m_HaulingCriteria);
   return artifact;
}

void CPGStableModel::ResolveLiftingStrandLocations() const
{
   if ( m_Strands[m_GirderType][LIFTING].strandMethod == CPGStableStrands::Simplified )
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
   m_LiftingStabilityProblem.SetXferLength(m_Strands[m_GirderType][LIFTING].XferLength,L);

   Float64 YpsStraight,Xh1,Yh1,Xh2,Yh2,Xh3,Yh3,Xh4,Yh4,YpsTemp;
   GetSimplifiedStrandLocations(&m_Strands[m_GirderType][LIFTING],&m_Girder[m_GirderType],&YpsStraight,&Xh1,&Yh1,&Xh2,&Yh2,&Xh3,&Yh3,&Xh4,&Yh4,&YpsTemp);

   m_LiftingStabilityProblem.ClearFpe();

   Float64 FpeStraight = m_Strands[m_GirderType][LIFTING].FpeStraight;
   Float64 FpeHarped   = m_Strands[m_GirderType][LIFTING].FpeHarped;
   Float64 FpeTemp     = m_Strands[m_GirderType][LIFTING].FpeTemp;
   m_LiftingStabilityProblem.AddFpe(0,  FpeStraight,YpsStraight,FpeHarped,Yh1,FpeTemp,YpsTemp);
   m_LiftingStabilityProblem.AddFpe(Xh1,FpeStraight,YpsStraight,FpeHarped,Yh1,FpeTemp,YpsTemp);
   m_LiftingStabilityProblem.AddFpe(Xh2,FpeStraight,YpsStraight,FpeHarped,Yh2,FpeTemp,YpsTemp);
   m_LiftingStabilityProblem.AddFpe(Xh3,FpeStraight,YpsStraight,FpeHarped,Yh3,FpeTemp,YpsTemp);
   m_LiftingStabilityProblem.AddFpe(Xh4,FpeStraight,YpsStraight,FpeHarped,Yh4,FpeTemp,YpsTemp);
   m_LiftingStabilityProblem.AddFpe(L,  FpeStraight,YpsStraight,FpeHarped,Yh4,FpeTemp,YpsTemp);
}

void CPGStableModel::ResolveExactLiftingStrandLocations() const
{
   Float64 Lg = m_Girder[m_GirderType].GetGirderLength();
   m_LiftingStabilityProblem.ClearFpe();
   for (const auto& fpe : m_Strands[m_GirderType][LIFTING].m_vFpe)
   {
      if ( ::InRange(0.0,fpe.X,Lg) )
      {
         Float64 Ys,Yh,Yt;
         GetStrandLocations(fpe,&m_Girder[m_GirderType],&Ys,&Yh,&Yt);
         m_LiftingStabilityProblem.AddFpe(fpe.X,fpe.FpeStraight,Ys,fpe.FpeHarped,Yh,fpe.FpeTemp,Yt);
      }
   }
}

void CPGStableModel::ResolveHaulingStrandLocations() const
{
   if ( m_Strands[m_GirderType][HAULING].strandMethod == CPGStableStrands::Simplified )
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
   m_HaulingStabilityProblem.SetXferLength(m_Strands[m_GirderType][HAULING].XferLength,L);

   Float64 YpsStraight,Xh1,Yh1,Xh2,Yh2,Xh3,Yh3,Xh4,Yh4,YpsTemp;
   GetSimplifiedStrandLocations(&m_Strands[m_GirderType][HAULING],&m_Girder[m_GirderType],&YpsStraight,&Xh1,&Yh1,&Xh2,&Yh2,&Xh3,&Yh3,&Xh4,&Yh4,&YpsTemp);

   m_HaulingStabilityProblem.ClearFpe();

   Float64 FpeStraight = m_Strands[m_GirderType][HAULING].FpeStraight;
   Float64 FpeHarped   = m_Strands[m_GirderType][HAULING].FpeHarped;
   Float64 FpeTemp     = m_Strands[m_GirderType][HAULING].FpeTemp;
   m_HaulingStabilityProblem.AddFpe(0,  FpeStraight,YpsStraight,FpeHarped,Yh1,FpeTemp,YpsTemp);
   m_HaulingStabilityProblem.AddFpe(Xh1,FpeStraight,YpsStraight,FpeHarped,Yh1,FpeTemp,YpsTemp);
   m_HaulingStabilityProblem.AddFpe(Xh2,FpeStraight,YpsStraight,FpeHarped,Yh2,FpeTemp,YpsTemp);
   m_HaulingStabilityProblem.AddFpe(Xh3,FpeStraight,YpsStraight,FpeHarped,Yh3,FpeTemp,YpsTemp);
   m_HaulingStabilityProblem.AddFpe(Xh4,FpeStraight,YpsStraight,FpeHarped,Yh4,FpeTemp,YpsTemp);
   m_HaulingStabilityProblem.AddFpe(L,  FpeStraight,YpsStraight,FpeHarped,Yh4,FpeTemp,YpsTemp);
}

void CPGStableModel::ResolveExactHaulingStrandLocations() const
{
   Float64 Lg = m_Girder[m_GirderType].GetGirderLength();
   m_HaulingStabilityProblem.ClearFpe();
   for (const auto& fpe : m_Strands[m_GirderType][HAULING].m_vFpe)
   {
      if ( ::InRange(0.0,fpe.X,Lg) )
      {
         Float64 Ys,Yh,Yt;
         GetStrandLocations(fpe,&m_Girder[m_GirderType],&Ys,&Yh,&Yt);
         m_HaulingStabilityProblem.AddFpe(fpe.X,fpe.FpeStraight,Ys,fpe.FpeHarped,Yh,fpe.FpeTemp,Yt);
      }
   }
}

void CPGStableModel::MapSimplifiedToExactStrandLocations(CPGStableStrands* pStrands)
{
   ATLASSERT(pStrands->strandMethod == CPGStableStrands::Simplified);

   // this call gets the strand location information for the stability engine.
   // we only want to create input parameters so we only want the X values for the harped strands
   Float64 YpsStraight,Xh1,Yh1,Xh2,Yh2,Xh3,Yh3,Xh4,Yh4,YpsTemp;
   GetSimplifiedStrandLocations(pStrands,&m_Girder[m_GirderType],&YpsStraight,&Xh1,&Yh1,&Xh2,&Yh2,&Xh3,&Yh3,&Xh4,&Yh4,&YpsTemp);

   Float64 L = m_Girder[m_GirderType].GetGirderLength();

   pStrands->m_vFpe.clear();
   pStrands->m_vFpe.insert(CPGStableFpe(0,  pStrands->FpeStraight,pStrands->Ys,pStrands->YsMeasure, pStrands->FpeHarped,pStrands->Yh1,pStrands->Yh1Measure, pStrands->FpeTemp,pStrands->Yt,pStrands->YtMeasure));
   pStrands->m_vFpe.insert(CPGStableFpe(Xh1,pStrands->FpeStraight,pStrands->Ys,pStrands->YsMeasure, pStrands->FpeHarped,pStrands->Yh1,pStrands->Yh1Measure, pStrands->FpeTemp,pStrands->Yt,pStrands->YtMeasure));
   pStrands->m_vFpe.insert(CPGStableFpe(Xh2,pStrands->FpeStraight,pStrands->Ys,pStrands->YsMeasure, pStrands->FpeHarped,pStrands->Yh2,pStrands->Yh2Measure, pStrands->FpeTemp,pStrands->Yt,pStrands->YtMeasure));
   pStrands->m_vFpe.insert(CPGStableFpe(Xh3,pStrands->FpeStraight,pStrands->Ys,pStrands->YsMeasure, pStrands->FpeHarped,pStrands->Yh3,pStrands->Yh3Measure, pStrands->FpeTemp,pStrands->Yt,pStrands->YtMeasure));
   pStrands->m_vFpe.insert(CPGStableFpe(Xh4,pStrands->FpeStraight,pStrands->Ys,pStrands->YsMeasure, pStrands->FpeHarped,pStrands->Yh4,pStrands->Yh4Measure, pStrands->FpeTemp,pStrands->Yt,pStrands->YtMeasure));
   pStrands->m_vFpe.insert(CPGStableFpe(L,  pStrands->FpeStraight,pStrands->Ys,pStrands->YsMeasure, pStrands->FpeHarped,pStrands->Yh4,pStrands->Yh4Measure, pStrands->FpeTemp,pStrands->Yt,pStrands->YtMeasure));
}

void CPGStableModel::GetSimplifiedStrandLocations(const CPGStableStrands* pStrands,const stbGirder* pGirder,Float64* pYpsStraight,Float64* pXh1,Float64* pYh1,Float64* pXh2,Float64* pYh2,Float64* pXh3,Float64* pYh3,Float64* pXh4,Float64* pYh4,Float64* pYpsTemp) const
{
   Float64 L = pGirder->GetGirderLength();

   Float64 Ag,Ix,Iy,Yt,Hg,Wtf,Wbf;
   pGirder->GetSectionProperties(0,stbTypes::Start,&Ag,&Ix,&Iy,&Yt,&Hg,&Wtf,&Wbf);

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
   pGirder->GetSectionProperties(Xh1,&Ag,&Ix,&Iy,&Yt,&Hg,&Wtf,&Wbf);
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
   pGirder->GetSectionProperties(Xh2,&Ag,&Ix,&Iy,&Yt,&Hg,&Wtf,&Wbf);
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
   pGirder->GetSectionProperties(Xh3,&Ag,&Ix,&Iy,&Yt,&Hg,&Wtf,&Wbf);
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
   pGirder->GetSectionProperties(Xh4,&Ag,&Ix,&Iy,&Yt,&Hg,&Wtf,&Wbf);
   if ( pStrands->Yh4Measure == TOP )
   {
      Yh4 = -pStrands->Yh4;
   }
   else
   {
      Yh4 = pStrands->Yh4 - Hg;
   }
   
   *pYpsStraight = YpsStraight;
   *pXh1 = Xh1;
   *pYh1 = Yh1;
   *pXh2 = Xh2;
   *pYh2 = Yh2;
   *pXh3 = Xh3;
   *pYh3 = Yh3;
   *pXh4 = Xh4;
   *pYh4 = Yh4;
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

bool CPGStableModel::SetDensity(Float64 density)
{
   if ( !::IsEqual(m_LiftingStabilityProblem.GetConcrete().GetDensity(),density) )
   {
      ATLASSERT(!::IsEqual(m_HaulingStabilityProblem.GetConcrete().GetDensity(),density));
      m_LiftingStabilityProblem.GetConcrete().SetDensity(density);
      m_HaulingStabilityProblem.GetConcrete().SetDensity(density);
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
      return true;
   }
   return false;
}

Float64 CPGStableModel::GetDensityWithRebar() const
{
   ATLASSERT(::IsEqual(m_LiftingStabilityProblem.GetConcrete().GetDensityForWeight(),m_HaulingStabilityProblem.GetConcrete().GetDensityForWeight()));
   return m_LiftingStabilityProblem.GetConcrete().GetDensityForWeight();
}

bool CPGStableModel::SetGirderType(int girderType)
{
   if ( m_GirderType != girderType )
   {
      m_GirderType = girderType;
      return true;
   }
   return false;
}

int CPGStableModel::GetGirderType() const
{
   return m_GirderType;
}

bool CPGStableModel::SetStrands(int girderType,int modelType,const CPGStableStrands& strands)
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

const CPGStableStrands& CPGStableModel::GetStrands(int girderType,int modelType) const
{
   return m_Strands[girderType][modelType];
}

bool CPGStableModel::SetGirder(int girderType,const stbGirder& girder)
{
   if ( m_Girder[girderType] != girder )
   {
      m_Girder[girderType] = girder;
      return true;
   }
   return false;
}

const stbGirder& CPGStableModel::GetGirder(int girderType) const
{
   return m_Girder[girderType];
}

bool CPGStableModel::SetLiftingStabilityProblem(const stbLiftingStabilityProblem& problem)
{
   if ( m_LiftingStabilityProblem != problem )
   {
      m_LiftingStabilityProblem = problem;
      return true;
   }
   return false;
}

const stbLiftingStabilityProblem& CPGStableModel::GetLiftingStabilityProblem() const
{
   return m_LiftingStabilityProblem;
}

bool CPGStableModel::SetHaulingStabilityProblem(const stbHaulingStabilityProblem& problem)
{
   if ( m_HaulingStabilityProblem != problem )
   {
      m_HaulingStabilityProblem = problem;
      return true;
   }
   return false;
}

const stbHaulingStabilityProblem& CPGStableModel::GetHaulingStabilityProblem() const
{
   return m_HaulingStabilityProblem;
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

void CPGStableModel::ResolveStrandLocations(const CPGStableStrands& strands,const stbGirder& girder,Float64* pYs,Float64* pXh1,Float64* pYh1,Float64* pXh2,Float64* pYh2,Float64* pXh3,Float64* pYh3,Float64* pXh4,Float64* pYh4,Float64* pYt)
{
   GetSimplifiedStrandLocations(&strands,&girder,pYs,pXh1,pYh1,pXh2,pYh2,pXh3,pYh3,pXh4,pYh4,pYt);
}

void CPGStableModel::GetStrandProfiles(const CPGStableStrands& strands,const stbGirder& girder,std::vector<std::pair<Float64,Float64>>* pvStraight,std::vector<std::pair<Float64,Float64>>* pvHarped,std::vector<std::pair<Float64,Float64>>* pvTemp) const
{
   if ( strands.strandMethod == CPGStableStrands::Simplified )
   {
      Float64 Ys,Yt,Xh1,Yh1,Xh2,Yh2,Xh3,Yh3,Xh4,Yh4;
      GetSimplifiedStrandLocations(&strands,&girder,&Ys,&Xh1,&Yh1,&Xh2,&Yh2,&Xh3,&Yh3,&Xh4,&Yh4,&Yt);

      Float64 Lg = girder.GetGirderLength();

      pvStraight->clear();
      pvStraight->push_back(std::make_pair(0, Ys));
      pvStraight->push_back(std::make_pair(Lg,Ys));

      pvHarped->clear();
      pvHarped->push_back(std::make_pair(0,Yh1));
      pvHarped->push_back(std::make_pair(Xh1,Yh1));
      pvHarped->push_back(std::make_pair(Xh2,Yh2));
      pvHarped->push_back(std::make_pair(Xh3,Yh3));
      pvHarped->push_back(std::make_pair(Xh4,Yh4));
      pvHarped->push_back(std::make_pair(Lg,Yh4));

      pvTemp->clear();
      pvTemp->push_back(std::make_pair(0, Yt));
      pvTemp->push_back(std::make_pair(Lg,Yt));
   }
   else
   {
      Float64 Lg = girder.GetGirderLength();

      pvStraight->clear();
      pvHarped->clear();
      pvTemp->clear();

      Float64 Ys,Yh,Yt;
      const CPGStableFpe& firstFpe = *strands.m_vFpe.begin();
      GetStrandLocations(firstFpe,&girder,&Ys,&Yh,&Yt);
      pvStraight->push_back(std::make_pair(0,Ys));
      pvHarped->push_back(std::make_pair(0,Yh));
      pvTemp->push_back(std::make_pair(0,Yt));

      for (const auto& fpe : strands.m_vFpe)
      {
         if ( ::InRange(0.0,fpe.X,Lg) )
         {
            // ignore all fpe values that aren't on the girder
            GetStrandLocations(fpe,&girder,&Ys,&Yh,&Yt);
            pvStraight->push_back(std::make_pair(fpe.X,Ys));
            pvHarped->push_back(std::make_pair(fpe.X,Yh));
            pvTemp->push_back(std::make_pair(fpe.X,Yt));
         }
      }

      const CPGStableFpe& lastFpe = *strands.m_vFpe.rbegin();
      if (::InRange(0.0,lastFpe.X,Lg) )
      {
         GetStrandLocations(lastFpe,&girder,&Ys,&Yh,&Yt);
         pvStraight->push_back(std::make_pair(Lg,Ys));
         pvHarped->push_back(std::make_pair(Lg,Yh));
         pvTemp->push_back(std::make_pair(Lg,Yt));
      }
   }
}

void CPGStableModel::GetStrandLocations(const CPGStableFpe& fpe,const stbGirder* pGirder,Float64* pYs,Float64* pYh,Float64* pYt) const
{
   Float64 Ag,Ix,Iy,Yt,Hg,Wtf,Wbf;
   pGirder->GetSectionProperties(fpe.X,&Ag,&Ix,&Iy,&Yt,&Hg,&Wtf,&Wbf);

   if ( fpe.YpsStraightMeasure == TOP )
   {
      *pYs = -fpe.YpsStraight;
   }
   else
   {
      *pYs = fpe.YpsStraight - Hg;
   }

   if ( fpe.YpsHarpedMeasure == TOP )
   {
      *pYh = -fpe.YpsHarped;
   }
   else
   {
      *pYh = fpe.YpsHarped - Hg;
   }

   if ( fpe.YpsTempMeasure == TOP )
   {
      *pYt = -fpe.YpsTemp;
   }
   else
   {
      *pYt = fpe.YpsTemp - Hg;
   }
}

HRESULT CPGStableModel::Save(IStructuredSave* pStrSave)
{
   HRESULT hr = pStrSave->BeginUnit(_T("PGStable"),1.0);
   if ( FAILED(hr) )
      return hr;

   CEAFApp* pApp = EAFGetApp();
   hr = pStrSave->put_Property(_T("Units"),CComVariant(pApp->GetUnitsMode()));

   pStrSave->put_Property(_T("GirderType"),CComVariant(m_GirderType));
   pStrSave->put_Property(_T("DragCoefficient"),CComVariant(m_Girder[m_GirderType].GetDragCoefficient()));

   pStrSave->BeginUnit(_T("Girder"),1.0);
   pStrSave->BeginUnit(_T("Sections"),1.0);
   IndexType nSections = m_Girder[m_GirderType].GetSectionCount();
   for ( IndexType sectIdx = 0; sectIdx < nSections; sectIdx++ )
   {
      pStrSave->BeginUnit(_T("Section"),1.0);

      Float64 L = m_Girder[m_GirderType].GetSectionLength(sectIdx);
      pStrSave->put_Property(_T("L"),CComVariant(L));

      Float64 Ag,Ix,Iy,Yt,Hg,Wtf,Wbf;
      m_Girder[m_GirderType].GetSectionProperties(sectIdx,stbTypes::Start,&Ag,&Ix,&Iy,&Yt,&Hg,&Wtf,&Wbf);
      pStrSave->BeginUnit(_T("StartFace"),1.0);
      pStrSave->put_Property(_T("Ag"),CComVariant(Ag));
      pStrSave->put_Property(_T("Ix"),CComVariant(Ix));
      pStrSave->put_Property(_T("Iy"),CComVariant(Iy));
      pStrSave->put_Property(_T("Yt"),CComVariant(Yt));
      pStrSave->put_Property(_T("Hg"),CComVariant(Hg));
      pStrSave->put_Property(_T("Wtf"),CComVariant(Wtf));
      pStrSave->put_Property(_T("Wbf"),CComVariant(Wbf));
      pStrSave->EndUnit(); // LeftFace

      m_Girder[m_GirderType].GetSectionProperties(sectIdx,stbTypes::End,&Ag,&Ix,&Iy,&Yt,&Hg,&Wtf,&Wbf);
      pStrSave->BeginUnit(_T("EndFace"),1.0);
      pStrSave->put_Property(_T("Ag"),CComVariant(Ag));
      pStrSave->put_Property(_T("Ix"),CComVariant(Ix));
      pStrSave->put_Property(_T("Iy"),CComVariant(Iy));
      pStrSave->put_Property(_T("Yt"),CComVariant(Yt));
      pStrSave->put_Property(_T("Hg"),CComVariant(Hg));
      pStrSave->put_Property(_T("Wtf"),CComVariant(Wtf));
      pStrSave->put_Property(_T("Wbf"),CComVariant(Wbf));
      pStrSave->EndUnit(); // RightFace

      pStrSave->EndUnit(); // Section
   }
   pStrSave->EndUnit(); // Sections

   pStrSave->put_Property(_T("DensityWithRebar"),CComVariant(GetDensityWithRebar()));
   pStrSave->put_Property(_T("Density"),CComVariant(GetDensity()));
   pStrSave->put_Property(_T("K1"),CComVariant(m_K1));
   pStrSave->put_Property(_T("K2"),CComVariant(m_K2));

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
   pStrSave->BeginUnit(_T("LiftingProblem"),2.0);
   const stbLiftingStabilityProblem& liftingProblem = GetLiftingStabilityProblem();
   m_Strands[m_GirderType][LIFTING].Save(pStrSave);

   bool bDirectCamber;
   Float64 camber;
   liftingProblem.GetCamber(&bDirectCamber,&camber);
   pStrSave->put_Property(_T("DirectCamber"),CComVariant(bDirectCamber));
   pStrSave->put_Property(_T("Camber"),CComVariant(camber));

   Float64 Ll,Lr;
   liftingProblem.GetSupportLocations(&Ll,&Lr);
   ATLASSERT(IsEqual(Ll,Lr));
   pStrSave->put_Property(_T("LiftPoint"),CComVariant(Ll));

   Float64 Yra = liftingProblem.GetYRollAxis();
   pStrSave->put_Property(_T("Yra"),CComVariant(Yra));

   Float64 SweepTolerance = liftingProblem.GetSweepTolerance();
   pStrSave->put_Property(_T("SweepTolerance"),CComVariant(SweepTolerance));

   Float64 SupportPlacementTolerance = liftingProblem.GetSupportPlacementTolerance();
   pStrSave->put_Property(_T("SupportPlacementTolerance"),CComVariant(SupportPlacementTolerance));

   Float64 imup,imdn;
   liftingProblem.GetImpact(&imup,&imdn);
   pStrSave->put_Property(_T("ImpactUp"),CComVariant(imup));
   pStrSave->put_Property(_T("ImpactDown"),CComVariant(imdn));

   // changed in version 2.0
   //bool bPlumbGirderStresses = liftingProblem.EvaluateStressesForPlumbGirder();
   //pStrSave->put_Property(_T("PlumbGirderStresses"),CComVariant(bPlumbGirderStresses));
   bool bEvaluateStressesAtEquilibriumAngle = liftingProblem.EvaluateStressesAtEquilibriumAngle();
   pStrSave->put_Property(_T("EvaluateStressesAtEquilibriumAngle"), CComVariant(bEvaluateStressesAtEquilibriumAngle));

   stbTypes::WindType windLoadType;
   Float64 windLoad;
   liftingProblem.GetWindLoading(&windLoadType,&windLoad);
   pStrSave->put_Property(_T("WindLoadType"),CComVariant(windLoadType));
   pStrSave->put_Property(_T("WindLoad"),CComVariant(windLoad));

   Float64 liftAngle = liftingProblem.GetLiftAngle();
   pStrSave->put_Property(_T("LiftAngle"),CComVariant(liftAngle));

   const matConcreteEx& concrete = liftingProblem.GetConcrete();
   pStrSave->put_Property(_T("FrCoefficient"),CComVariant(m_LiftingFrCoefficient));
   pStrSave->put_Property(_T("Fci"),CComVariant(concrete.GetFc()));
   pStrSave->put_Property(_T("ComputeEci"),CComVariant(m_bComputeEci));
   pStrSave->put_Property(_T("Eci"),CComVariant(concrete.GetE()));
   pStrSave->EndUnit(); // LiftingProblem
   }

   {
   pStrSave->BeginUnit(_T("HaulingProblem"),2.0);
   m_Strands[m_GirderType][HAULING].Save(pStrSave);

   const stbHaulingStabilityProblem& haulingProblem = GetHaulingStabilityProblem();

   bool bDirectCamber;
   Float64 camber;
   haulingProblem.GetCamber(&bDirectCamber,&camber);
   pStrSave->put_Property(_T("DirectCamber"),CComVariant(bDirectCamber));
   pStrSave->put_Property(_T("Camber"),CComVariant(camber));

   Float64 Ll,Lr;
   haulingProblem.GetSupportLocations(&Ll,&Lr);
   ATLASSERT(IsEqual(Ll,Lr));
   pStrSave->put_Property(_T("LeftBunkPoint"),CComVariant(Ll));
   pStrSave->put_Property(_T("RightBunkPoint"),CComVariant(Lr));

   pStrSave->put_Property(_T("Hgb"),CComVariant(m_Hgb));

   Float64 SweepTolerance = haulingProblem.GetSweepTolerance();
   pStrSave->put_Property(_T("SweepTolerance"),CComVariant(SweepTolerance));

   Float64 SupportPlacementTolerance = haulingProblem.GetSupportPlacementTolerance();
   pStrSave->put_Property(_T("SupportPlacementTolerance"),CComVariant(SupportPlacementTolerance));

   Float64 imup,imdn;
   haulingProblem.GetImpact(&imup,&imdn);
   pStrSave->put_Property(_T("ImpactUp"),CComVariant(imup));
   pStrSave->put_Property(_T("ImpactDown"),CComVariant(imdn));

   stbTypes::HaulingImpact impactUsage = haulingProblem.GetImpactUsage();
   pStrSave->put_Property(_T("ImpactUsage"),CComVariant(impactUsage));

   // added in version 2
   bool bEvaluateStressesAtEquilibriumAngle = haulingProblem.EvaluateStressesAtEquilibriumAngle(stbTypes::CrownSlope);
   pStrSave->put_Property(_T("EvaluateStressesAtEquilibriumAngle"), CComVariant(bEvaluateStressesAtEquilibriumAngle));

   stbTypes::WindType windLoadType;
   Float64 windLoad;
   haulingProblem.GetWindLoading(&windLoadType,&windLoad);
   pStrSave->put_Property(_T("WindLoadType"),CComVariant(windLoadType));
   pStrSave->put_Property(_T("WindLoad"),CComVariant(windLoad));

   Float64 Ktheta = haulingProblem.GetTruckRotationalStiffness();
   pStrSave->put_Property(_T("Ktheta"),CComVariant(Ktheta));
   Float64 Wcc = haulingProblem.GetWheelLineSpacing();
   pStrSave->put_Property(_T("Wcc"),CComVariant(Wcc));
   Float64 Hrc = haulingProblem.GetHeightOfRollAxisAboveRoadway();
   pStrSave->put_Property(_T("Hrc"),CComVariant(Hrc));
   Float64 CrownSlope = haulingProblem.GetCrownSlope();
   pStrSave->put_Property(_T("CrownSlope"),CComVariant(CrownSlope));
   Float64 Superelevation = haulingProblem.GetSuperelevation();
   pStrSave->put_Property(_T("Superelevation"),CComVariant(Superelevation));
   Float64 Velocity = haulingProblem.GetVelocity();
   pStrSave->put_Property(_T("Velocity"),CComVariant(Velocity));
   Float64 Radius = haulingProblem.GetTurningRadius();
   pStrSave->put_Property(_T("Radius"),CComVariant(Radius));
   stbTypes::CFType cfType = haulingProblem.GetCentrifugalForceType();
   pStrSave->put_Property(_T("CFType"),CComVariant(cfType));

   const matConcreteEx& concrete = haulingProblem.GetConcrete();
   pStrSave->put_Property(_T("FrCoefficient"),CComVariant(m_HaulingFrCoefficient));
   pStrSave->put_Property(_T("Fc"),CComVariant(concrete.GetFc()));
   pStrSave->put_Property(_T("ComputeEc"),CComVariant(m_bComputeEc));
   pStrSave->put_Property(_T("Ec"),CComVariant(concrete.GetE()));
   pStrSave->EndUnit(); // HaulingProblem
   }

   pStrSave->BeginUnit(_T("LiftingCriteria"),1.0);
   m_LiftingCriteria.Save(pStrSave);
   pStrSave->EndUnit(); // LiftingCriteria

   pStrSave->BeginUnit(_T("HaulingCriteria"),1.0);
   m_HaulingCriteria.Save(pStrSave);
   pStrSave->EndUnit(); // HaulingCriteria

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

      CComVariant var;

      var.vt = VT_I4;
      hr = pStrLoad->get_Property(_T("Units"),&var);
      CEAFApp* pApp = EAFGetApp();
      pApp->SetUnitsMode(eafTypes::UnitMode(var.lVal));

      var.vt = VT_I4;
      hr = pStrLoad->get_Property(_T("GirderType"),&var);
      m_GirderType = var.lVal;

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("DragCoefficient"),&var);
      m_Girder[m_GirderType].SetDragCoefficient(var.dblVal);

      hr = pStrLoad->BeginUnit(_T("Girder"));
      hr = pStrLoad->BeginUnit(_T("Sections"));
      m_Girder[m_GirderType].ClearSections();
      while (SUCCEEDED(pStrLoad->BeginUnit(_T("Section"))) )
      {
         var.vt = VT_R8;
         hr = pStrLoad->get_Property(_T("L"),&var);
         Float64 L = var.dblVal;

         Float64 Ag[2],Ix[2],Iy[2],Yt[2],Hg[2],Wtf[2],Wbf[2];
         
         hr = pStrLoad->BeginUnit(_T("StartFace"));
         hr = pStrLoad->get_Property(_T("Ag"),&var);
         Ag[stbTypes::Start] = var.dblVal;

         hr = pStrLoad->get_Property(_T("Ix"),&var);
         Ix[stbTypes::Start] = var.dblVal;

         hr = pStrLoad->get_Property(_T("Iy"),&var);
         Iy[stbTypes::Start] = var.dblVal;

         hr = pStrLoad->get_Property(_T("Yt"),&var);
         Yt[stbTypes::Start] = var.dblVal;

         hr = pStrLoad->get_Property(_T("Hg"),&var);
         Hg[stbTypes::Start] = var.dblVal;

         hr = pStrLoad->get_Property(_T("Wtf"),&var);
         Wtf[stbTypes::Start] = var.dblVal;

         hr = pStrLoad->get_Property(_T("Wbf"),&var);
         Wbf[stbTypes::Start] = var.dblVal;
         hr = pStrLoad->EndUnit(); // StartFace

         hr = pStrLoad->BeginUnit(_T("EndFace"));
         hr = pStrLoad->get_Property(_T("Ag"),&var);
         Ag[stbTypes::End] = var.dblVal;

         hr = pStrLoad->get_Property(_T("Ix"),&var);
         Ix[stbTypes::End] = var.dblVal;

         hr = pStrLoad->get_Property(_T("Iy"),&var);
         Iy[stbTypes::End] = var.dblVal;

         hr = pStrLoad->get_Property(_T("Yt"),&var);
         Yt[stbTypes::End] = var.dblVal;

         hr = pStrLoad->get_Property(_T("Hg"),&var);
         Hg[stbTypes::End] = var.dblVal;

         hr = pStrLoad->get_Property(_T("Wtf"),&var);
         Wtf[stbTypes::End] = var.dblVal;

         hr = pStrLoad->get_Property(_T("Wbf"),&var);
         Wbf[stbTypes::End] = var.dblVal;
         hr = pStrLoad->EndUnit(); // EndFace

         m_Girder[m_GirderType].AddSection(L,Ag[stbTypes::Start],
                                             Ix[stbTypes::Start],
                                             Iy[stbTypes::Start],
                                             Yt[stbTypes::Start],
                                             Hg[stbTypes::Start],
                                             Wtf[stbTypes::Start],
                                             Wbf[stbTypes::Start],
                                             Ag[stbTypes::End],
                                             Ix[stbTypes::End],
                                             Iy[stbTypes::End],
                                             Yt[stbTypes::End],
                                             Hg[stbTypes::End],
                                             Wtf[stbTypes::End],
                                             Wbf[stbTypes::End]);

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

      hr = m_Strands[m_GirderType][LIFTING].Load(pStrLoad);

      bool bDirectCamber;
      Float64 camber;
      var.vt = VT_BOOL;
      hr = pStrLoad->get_Property(_T("DirectCamber"),&var);
      bDirectCamber = (var.boolVal == VARIANT_TRUE ? true : false);

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("Camber"),&var);
      camber = var.dblVal;

      m_LiftingStabilityProblem.SetCamber(bDirectCamber,camber);

      hr = pStrLoad->get_Property(_T("LiftPoint"),&var);
      m_LiftingStabilityProblem.SetSupportLocations(var.dblVal,var.dblVal);

      hr = pStrLoad->get_Property(_T("Yra"),&var);
      m_LiftingStabilityProblem.SetYRollAxis(var.dblVal);

      hr = pStrLoad->get_Property(_T("SweepTolerance"),&var);
      m_LiftingStabilityProblem.SetSweepTolerance(var.dblVal);

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
      else
      {
         // new in version 2
         hr = pStrLoad->get_Property(_T("EvaluateStressesAtEquilibriumAngle"), &var);
      }
      m_LiftingStabilityProblem.EvaluateStressesAtEquilibriumAngle(var.boolVal == VARIANT_TRUE ? true : false);

      var.vt = VT_I4;
      hr = pStrLoad->get_Property(_T("WindLoadType"),&var);
      stbTypes::WindType windLoadType = (stbTypes::WindType)var.lVal;

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("WindLoad"),&var);
      Float64 windLoad = var.dblVal;
      m_LiftingStabilityProblem.SetWindLoading(windLoadType,windLoad);

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

      {
      hr = pStrLoad->BeginUnit(_T("HaulingProblem"));

      Float64 version;
      pStrLoad->get_Version(&version);

      hr = m_Strands[m_GirderType][HAULING].Load(pStrLoad);

      bool bDirectCamber;
      Float64 camber;
      var.vt = VT_BOOL;
      hr = pStrLoad->get_Property(_T("DirectCamber"),&var);
      bDirectCamber = (var.boolVal == VARIANT_TRUE ? true : false);

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("Camber"),&var);
      camber = var.dblVal;

      m_HaulingStabilityProblem.SetCamber(bDirectCamber,camber);

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
      m_HaulingStabilityProblem.SetImpactUsage((stbTypes::HaulingImpact)var.lVal);

      if (1 < version)
      {
         // added in version 2
         hr = pStrLoad->get_Property(_T("EvaluateStressesAtEquilibriumAngle"), &var);
         m_HaulingStabilityProblem.EvaluateStressesAtEquilibriumAngle(stbTypes::CrownSlope,var.boolVal == VARIANT_TRUE ? true : false);
      }

      var.vt = VT_I4;
      hr = pStrLoad->get_Property(_T("WindLoadType"),&var);
      stbTypes::WindType windLoadType = (stbTypes::WindType)var.lVal;

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("WindLoad"),&var);
      Float64 windLoad = var.dblVal;
      m_HaulingStabilityProblem.SetWindLoading(windLoadType,windLoad);

      hr = pStrLoad->get_Property(_T("Ktheta"),&var);
      m_HaulingStabilityProblem.SetTruckRotationalStiffness(var.dblVal);

      hr = pStrLoad->get_Property(_T("Wcc"),&var);
      m_HaulingStabilityProblem.SetWheelLineSpacing(var.dblVal);

      hr = pStrLoad->get_Property(_T("Hrc"),&var);
      m_HaulingStabilityProblem.SetHeightOfRollAxisAboveRoadway(var.dblVal);

      hr = pStrLoad->get_Property(_T("CrownSlope"),&var);
      m_HaulingStabilityProblem.SetCrownSlope(var.dblVal);

      hr = pStrLoad->get_Property(_T("Superelevation"),&var);
      m_HaulingStabilityProblem.SetSuperelevation(var.dblVal);

      hr = pStrLoad->get_Property(_T("Velocity"),&var);
      m_HaulingStabilityProblem.SetVelocity(var.dblVal);

      hr = pStrLoad->get_Property(_T("Radius"),&var);
      m_HaulingStabilityProblem.SetTurningRadius(var.dblVal);

      var.vt = VT_I4;
      hr = pStrLoad->get_Property(_T("CFType"),&var);
      m_HaulingStabilityProblem.SetCentrifugalForceType((stbTypes::CFType)var.lVal);

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
      
      hr = pStrLoad->BeginUnit(_T("LiftingCriteria"));
      hr = m_LiftingCriteria.Load(pStrLoad);
      hr = pStrLoad->EndUnit(); // LiftingCriteria
      
      hr = pStrLoad->BeginUnit(_T("HaulingCriteria"));
      hr = m_HaulingCriteria.Load(pStrLoad);
      hr = pStrLoad->EndUnit(); // HaulingCriteria

      hr = pStrLoad->EndUnit();
   }
   catch(...)
   {
      THROW_LOAD(InvalidFileFormat,pStrLoad);
   }

   if ( m_GirderType == PRISMATIC )
   {
      m_Girder[NONPRISMATIC] = m_Girder[PRISMATIC];
      m_Strands[NONPRISMATIC][LIFTING] = m_Strands[PRISMATIC][LIFTING];
      m_Strands[NONPRISMATIC][HAULING] = m_Strands[PRISMATIC][HAULING];
   }
   else
   {
      m_Girder[PRISMATIC] = m_Girder[NONPRISMATIC];

      Float64 L = m_Girder[NONPRISMATIC].GetSectionLength(0);
      Float64 Ag,Ix,Iy,Yt,Hg,Wtf,Wbf;
      m_Girder[NONPRISMATIC].GetSectionProperties(0,stbTypes::Start,&Ag,&Ix,&Iy,&Yt,&Hg,&Wtf,&Wbf);

      m_Girder[PRISMATIC].ClearSections();
      m_Girder[PRISMATIC].AddSection(L,Ag,Ix,Iy,Yt,Hg,Wtf,Wbf);
   }

   return S_OK;
}
