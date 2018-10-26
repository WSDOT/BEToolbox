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

// PGStableModel.cpp : implementation file
//

#include "stdafx.h"
#include "PGStableModel.h"
#include <EAF\EAFApp.h>

CPGStableModel::CPGStableModel()
{
   m_GirderType = PRISMATIC;

#pragma Reminder("WORKING HERE - this is dummy code, just for getting started... move when done with it")
   Float64 Hg  = ::ConvertToSysUnits(72,unitMeasure::Inch);
   Float64 Wtf = ::ConvertToSysUnits(42,unitMeasure::Inch);
   Float64 Wbf = ::ConvertToSysUnits(26,unitMeasure::Inch);
   
   Float64 Ag = ::ConvertToSysUnits(767,unitMeasure::Inch2);
   Float64 Ix = ::ConvertToSysUnits(545894,unitMeasure::Inch4);
   Float64 Iy = ::ConvertToSysUnits(37634,unitMeasure::Inch4);
   Float64 Yt = ::ConvertToSysUnits(36.6-72,unitMeasure::Inch); // want neg because we are in section coordinates
   Float64 L = ::ConvertToSysUnits(136,unitMeasure::Feet);
   m_Girder[PRISMATIC].AddSection(L,Ag,Ix,Iy,Yt,Hg,Wtf,Wbf);

   m_Density = ::ConvertToSysUnits(0.150,unitMeasure::KipPerFeet3); // without rebar (used to compute Ec)
   m_Girder[PRISMATIC].SetDensity(::ConvertToSysUnits(0.155,unitMeasure::KipPerFeet3)); // including allowance for rebar (used for computing dead load)

   m_Girder[NONPRISMATIC] = m_Girder[PRISMATIC];

   m_LiftingFrCoefficient = ::ConvertToSysUnits(0.24,unitMeasure::SqrtKSI);
   m_HaulingFrCoefficient = ::ConvertToSysUnits(0.24,unitMeasure::SqrtKSI);
#pragma Reminder("WORKING HERE - set these to true... they are set to false to match the PCI document during development")
   m_bComputeEci = false;
   m_bComputeEc = false;
   m_Fci = ::ConvertToSysUnits(5.5,unitMeasure::KSI);
   m_Fc  = ::ConvertToSysUnits(7.0,unitMeasure::KSI);
   m_K1 = 1.0;
   m_K2 = 1.0;

   Float64 Eci = ::lrfdConcreteUtil::ModE(m_Fci,m_Density);
   Float64 Ec  = ::lrfdConcreteUtil::ModE(m_Fc, m_Density);

   // Example 6.1.1
   m_LiftingFpeType = CONSTANT_FPE;
   Float64 fpe = ::ConvertToSysUnits(1232.0,unitMeasure::Kip);
   m_LiftingStabilityProblem.AddFpe(0.0,0,fpe,0);
   m_LiftingStabilityProblem.SetCamber(true,::ConvertToSysUnits(2.92,unitMeasure::Inch));
   m_LiftingStabilityProblem.SetSweepTolerance(0.000520833333);
   m_LiftingStabilityProblem.SetSupportPlacementTolerance( ::ConvertToSysUnits(0.25,unitMeasure::Inch) );
   //m_LiftingStabilityProblem.SetFr(::ConvertToSysUnits(0.24*sqrt(5.5),unitMeasure::KSI));
   //m_LiftingStabilityProblem.SetEc( Eci );
   m_LiftingStabilityProblem.SetEc(::ConvertToSysUnits(4765.97,unitMeasure::KSI));
   m_LiftingStabilityProblem.SetSupportLocations(::ConvertToSysUnits(9,unitMeasure::Feet),::ConvertToSysUnits(9,unitMeasure::Feet));
   m_LiftingStabilityProblem.SetLiftAngle(PI_OVER_2);
   m_LiftingStabilityProblem.SetYRollAxis(::ConvertToSysUnits(0.0,unitMeasure::Inch));
   m_LiftingStabilityProblem.SetImpact(0,0);
   m_LiftingStabilityProblem.ApplyImpactToTiltedGirder(true);
   m_LiftingStabilityProblem.SetWindPressure(0.0);

   m_HaulingFpeType = CONSTANT_FPE;
   Float64 fpeHarp = ::ConvertToSysUnits(1195.47,unitMeasure::Kip);
   Float64 fpeTemp = ::ConvertToSysUnits(56.03,unitMeasure::Kip);
   m_HaulingStabilityProblem.AddFpe(0.0,0,fpeHarp,fpeTemp);
   m_HaulingStabilityProblem.SetCamber(true,::ConvertToSysUnits(2.92,unitMeasure::Inch));
   //m_HaulingStabilityProblem.SetFr(::ConvertToSysUnits(0.24*sqrt(7.0),unitMeasure::KSI));
   //m_HaulingStabilityProblem.SetEc(Ec);
   m_HaulingStabilityProblem.SetEc(::ConvertToSysUnits(5164.914,unitMeasure::KSI));

   m_HaulingStabilityProblem.SetSupportLocations(::ConvertToSysUnits(10,unitMeasure::Feet),::ConvertToSysUnits(10,unitMeasure::Feet));
   m_HaulingStabilityProblem.SetSweepTolerance(2*0.000520833333);
   m_HaulingStabilityProblem.SetSupportPlacementTolerance(::ConvertToSysUnits(1.0,unitMeasure::Inch));
//   m_HaulingStabilityProblem.SetYRollAxis(::ConvertToSysUnits(-48.0,unitMeasure::Inch) - Hg); // location of roll axes below top of girder
   m_HaulingStabilityProblem.SetTruckRotationalStiffness(::ConvertToSysUnits(40500.,unitMeasure::KipInchPerRadian));
   m_HaulingStabilityProblem.SetSuperelevation(0.06);
   m_HaulingStabilityProblem.SetWheelLineSpacing(::ConvertToSysUnits(72.,unitMeasure::Inch));
   m_HaulingStabilityProblem.SetHeightOfRollAxisAboveRoadway(::ConvertToSysUnits(24.,unitMeasure::Inch));
   m_HaulingStabilityProblem.SetImpact(0,0);
   m_HaulingStabilityProblem.ApplyImpactToTiltedGirder(false);
   m_HaulingStabilityProblem.SetWindPressure(0);
   m_HaulingStabilityProblem.SetVelocity(0);
   m_HaulingStabilityProblem.SetTurningRadius(::ConvertToSysUnits(120,unitMeasure::Feet));

   m_Hgb = ::ConvertToSysUnits(72.0,unitMeasure::Inch);

   m_LiftingCriteria.bMaxTension = true;
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
   m_LiftingStabilityProblem.SetFc(m_Fci);

   Float64 Ec;
   if ( m_bComputeEci )
   {
      Ec = ::lrfdConcreteUtil::ModE(m_Fci,m_Density);
      if ( lrfdVersionMgr::ThirdEditionWith2005Interims <= lrfdVersionMgr::GetVersion() )
      {
         Ec *= m_K1*m_K2;
      }
   }
   else
   {
      Ec = m_LiftingStabilityProblem.GetEc();
   }
   m_LiftingStabilityProblem.SetEc(Ec);

   Float64 fr = ::lrfdConcreteUtil::ModRupture(m_Fci,m_LiftingFrCoefficient);
   m_LiftingStabilityProblem.SetFr(fr);

   stbGirder* pGirder = &m_Girder[m_GirderType];
   Float64 L = pGirder->GetGirderLength();
   m_LiftingStabilityProblem.ClearAnalysisPoints();
   int n = 10;
   for ( int i = 0; i <= n; i++ )
   {
      Float64 X = i*L/n;
      m_LiftingStabilityProblem.AddAnalysisPoint(X);
   }

   Float64 Ll, Lr;
   m_LiftingStabilityProblem.GetSupportLocations(&Ll,&Lr);
   m_LiftingStabilityProblem.AddAnalysisPoint(Ll);
   m_LiftingStabilityProblem.AddAnalysisPoint(L-Lr);

   const CPGStableStrands* pStrands = &m_Strands[m_GirderType];
   ResolveStrandLocations(pStrands,pGirder);

   m_LiftingStabilityProblem.SetXferLength(pStrands->XferLength,L);

   // criteria
   m_LiftingCriteria.AllowableCompression = -m_LiftingCriteria.CompressionCoefficient*m_Fci;
   m_LiftingCriteria.AllowableTension = m_LiftingCriteria.TensionCoefficient*sqrt(m_Fci);
   if ( m_LiftingCriteria.bMaxTension )
   {
      m_LiftingCriteria.AllowableTension = Min(m_LiftingCriteria.AllowableTension,m_LiftingCriteria.MaxTension);
   }
   m_LiftingCriteria.AllowableTensionWithRebar = m_LiftingCriteria.TensionCoefficientWithRebar*sqrt(m_Fci);
   //m_LiftingCriteria.AllowableTensionTiltedGirder = m_LiftingCriteria.GetFr();

   stbStabilityEngineer stabilityEngineer;
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
   m_HaulingStabilityProblem.SetFc(m_Fc);

   Float64 Ec;
   if ( m_bComputeEc )
   {
      Ec = ::lrfdConcreteUtil::ModE(m_Fc,m_Density);
      if ( lrfdVersionMgr::ThirdEditionWith2005Interims <= lrfdVersionMgr::GetVersion() )
      {
         Ec *= m_K1*m_K2;
      }
   }
   else
   {
      Ec = m_HaulingStabilityProblem.GetEc();
   }
   m_HaulingStabilityProblem.SetEc(Ec);

   Float64 fr = ::lrfdConcreteUtil::ModRupture(m_Fc,m_HaulingFrCoefficient);
   m_HaulingStabilityProblem.SetFr(fr);

   stbGirder* pGirder = &m_Girder[m_GirderType];
   Float64 L = pGirder->GetGirderLength();
   m_HaulingStabilityProblem.ClearAnalysisPoints();
   int n = 10;
   for ( int i = 0; i <= n; i++ )
   {
      Float64 X = i*L/n;
      m_HaulingStabilityProblem.AddAnalysisPoint(X);
   }

   Float64 Ll, Lr;
   m_HaulingStabilityProblem.GetSupportLocations(&Ll,&Lr);
   m_HaulingStabilityProblem.AddAnalysisPoint(Ll);
   m_HaulingStabilityProblem.AddAnalysisPoint(L-Lr);

   Float64 Ag,Ix,Iy,Yt,Hg,Wtop,Wbot;
   pGirder->GetSectionProperties(0,LEFT,&Ag,&Ix,&Iy,&Yt,&Hg,&Wtop,&Wbot);
   Float64 Yra = -(Hg + m_Hgb) + m_HaulingStabilityProblem.GetHeightOfRollAxisAboveRoadway();
   ATLASSERT(Yra < 0);
   m_HaulingStabilityProblem.SetYRollAxis(Yra); // location of roll axes relative to the top of girder


   const CPGStableStrands* pStrands = &m_Strands[m_GirderType];
   ResolveStrandLocations(pStrands,pGirder);

   m_HaulingStabilityProblem.SetXferLength(pStrands->XferLength,L);

   m_HaulingCriteria.AllowableCompression = -m_HaulingCriteria.CompressionCoefficient*m_Fc;
   m_HaulingCriteria.AllowableTension = m_HaulingCriteria.TensionCoefficient*sqrt(m_Fc);
   if ( m_HaulingCriteria.bMaxTension )
   {
      m_HaulingCriteria.AllowableTension = Min(m_HaulingCriteria.AllowableTension,m_HaulingCriteria.MaxTension);
   }
   m_HaulingCriteria.AllowableTensionWithRebar = m_HaulingCriteria.TensionCoefficientWithRebar*sqrt(m_Fc);
   m_HaulingCriteria.AllowableTensionTiltedGirder = m_HaulingStabilityProblem.GetFr();

   stbStabilityEngineer stabilityEngineer;
   stbHaulingCheckArtifact artifact = stabilityEngineer.CheckHauling(pGirder,&m_HaulingStabilityProblem,m_HaulingCriteria);
   return artifact;
}

void CPGStableModel::ResolveStrandLocations(const CPGStableStrands* pStrands,stbGirder* pGirder) const
{
   Float64 L = pGirder->GetGirderLength();

   Float64 Ag,Ix,Iy,Yt,Hg,Wtf,Wbf;
   pGirder->GetSectionProperties(0,LEFT,&Ag,&Ix,&Iy,&Yt,&Hg,&Wtf,&Wbf);

   if ( pStrands->YsMeasure == TOP )
   {
      pGirder->SetStraightStrandLocation(-pStrands->Ys);
   }
   else
   {
      pGirder->SetStraightStrandLocation(pStrands->Ys - Hg);
   }

   if ( pStrands->YtMeasure == TOP )
   {
      pGirder->SetTemporaryStrandLocation(-pStrands->Yt);
   }
   else
   {
      pGirder->SetTemporaryStrandLocation(pStrands->Yt - Hg);
   }

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

   pGirder->SetHarpedStrandLocation(Xh1,Yh1,Xh2,Yh2,Xh3,Yh3,Xh4,Yh4);
}

HRESULT CPGStableModel::Save(IStructuredSave* pStrSave)
{
   HRESULT hr = pStrSave->BeginUnit(_T("PGStable"),1.0);
   if ( FAILED(hr) )
      return hr;

   CEAFApp* pApp = EAFGetApp();
   hr = pStrSave->put_Property(_T("Units"),CComVariant(pApp->GetUnitsMode()));

   pStrSave->put_Property(_T("GirderType"),CComVariant(m_GirderType));

   pStrSave->BeginUnit(_T("Girder"),1.0);
   pStrSave->BeginUnit(_T("Sections"),1.0);
   IndexType nSections = m_Girder[m_GirderType].GetSectionCount();
   for ( IndexType sectIdx = 0; sectIdx < nSections; sectIdx++ )
   {
      pStrSave->BeginUnit(_T("Section"),1.0);

      Float64 L = m_Girder[m_GirderType].GetSectionLength(sectIdx);
      pStrSave->put_Property(_T("L"),CComVariant(L));

      Float64 Ag,Ix,Iy,Yt,Hg,Wtf,Wbf;
      m_Girder[m_GirderType].GetSectionProperties(sectIdx,LEFT,&Ag,&Ix,&Iy,&Yt,&Hg,&Wtf,&Wbf);
      pStrSave->BeginUnit(_T("LeftFace"),1.0);
      pStrSave->put_Property(_T("Ag"),CComVariant(Ag));
      pStrSave->put_Property(_T("Ix"),CComVariant(Ix));
      pStrSave->put_Property(_T("Iy"),CComVariant(Iy));
      pStrSave->put_Property(_T("Yt"),CComVariant(Yt));
      pStrSave->put_Property(_T("Hg"),CComVariant(Hg));
      pStrSave->put_Property(_T("Wtf"),CComVariant(Wtf));
      pStrSave->put_Property(_T("Wbf"),CComVariant(Wbf));
      pStrSave->EndUnit(); // LeftFace

      m_Girder[m_GirderType].GetSectionProperties(sectIdx,RIGHT,&Ag,&Ix,&Iy,&Yt,&Hg,&Wtf,&Wbf);
      pStrSave->BeginUnit(_T("RightFace"),1.0);
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

   Float64 density = m_Girder[m_GirderType].GetDensity();
   pStrSave->put_Property(_T("DensityWithRebar"),CComVariant(density));
   pStrSave->put_Property(_T("Density"),CComVariant(m_Density));
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

   m_Strands[m_GirderType].Save(pStrSave);
   pStrSave->EndUnit(); // Girder


   {
   pStrSave->BeginUnit(_T("LiftingProblem"),1.0);
   pStrSave->put_Property(_T("FpeType"),CComVariant(m_LiftingFpeType));
   if ( m_LiftingFpeType == CONSTANT_FPE )
   {
      Float64 X,FpeStraight,FpeHarped,FpeTemp;
      m_LiftingStabilityProblem.GetFpe(0,&X,&FpeStraight,&FpeHarped,&FpeTemp);
      pStrSave->put_Property(_T("Fpe_Straight"),CComVariant(FpeStraight));
      pStrSave->put_Property(_T("Fpe_Harped"),CComVariant(FpeHarped));
      pStrSave->put_Property(_T("Fpe_Temporary"),CComVariant(FpeTemp));
   }
   else
   {
      pStrSave->BeginUnit(_T("EffectivePrestress"),1.0);
      IndexType nFpe = m_LiftingStabilityProblem.GetFpeCount();
      for ( IndexType fpeIdx = 0; fpeIdx < nFpe; fpeIdx++ )
      {
         pStrSave->BeginUnit(_T("Fpe"),1.0);
         Float64 X,FpeStraight,FpeHarped,FpeTemp;
         m_LiftingStabilityProblem.GetFpe(fpeIdx,&X,&FpeStraight,&FpeHarped,&FpeTemp);
         pStrSave->put_Property(_T("X"),CComVariant(X));
         pStrSave->put_Property(_T("Fpe_Straight"),CComVariant(FpeStraight));
         pStrSave->put_Property(_T("Fpe_Harped"),CComVariant(FpeHarped));
         pStrSave->put_Property(_T("Fpe_Temporary"),CComVariant(FpeTemp));
         pStrSave->EndUnit(); // Fpe
      }
      pStrSave->EndUnit(); // EffectivePrestress
   }

   bool bDirectCamber;
   Float64 camber;
   m_LiftingStabilityProblem.GetCamber(&bDirectCamber,&camber);
   pStrSave->put_Property(_T("DirectCamber"),CComVariant(bDirectCamber));
   pStrSave->put_Property(_T("Camber"),CComVariant(camber));

   Float64 Ll,Lr;
   m_LiftingStabilityProblem.GetSupportLocations(&Ll,&Lr);
   ATLASSERT(IsEqual(Ll,Lr));
   pStrSave->put_Property(_T("LiftPoint"),CComVariant(Ll));

   Float64 Yra = m_LiftingStabilityProblem.GetYRollAxis();
   pStrSave->put_Property(_T("Yra"),CComVariant(Yra));

   Float64 SweepTolerance = m_LiftingStabilityProblem.GetSweepTolerance();
   pStrSave->put_Property(_T("SweepTolerance"),CComVariant(SweepTolerance));

   Float64 SupportPlacementTolerance = m_LiftingStabilityProblem.GetSupportPlacementTolerance();
   pStrSave->put_Property(_T("SupportPlacementTolerance"),CComVariant(SupportPlacementTolerance));

   Float64 imup,imdn;
   m_LiftingStabilityProblem.GetImpact(&imup,&imdn);
   pStrSave->put_Property(_T("ImpactUp"),CComVariant(imup));
   pStrSave->put_Property(_T("ImpactDown"),CComVariant(imdn));

   bool bApplyImpact = m_LiftingStabilityProblem.ApplyImpactToTiltedGirder();
   pStrSave->put_Property(_T("ApplyImpactToTiltedGirder"),CComVariant(bApplyImpact));

   Float64 WindPressure = m_LiftingStabilityProblem.GetWindPressure();
   pStrSave->put_Property(_T("WindPressure"),CComVariant(WindPressure));

   Float64 liftAngle = m_LiftingStabilityProblem.GetLiftAngle();
   pStrSave->put_Property(_T("LiftAngle"),CComVariant(liftAngle));

   pStrSave->put_Property(_T("FrCoefficient"),CComVariant(m_LiftingFrCoefficient));
   pStrSave->put_Property(_T("Fci"),CComVariant(m_Fci));
   pStrSave->put_Property(_T("ComputeEci"),CComVariant(m_bComputeEci));
   Float64 Eci = m_LiftingStabilityProblem.GetEc();
   pStrSave->put_Property(_T("Eci"),CComVariant(Eci));
   pStrSave->EndUnit(); // LiftingProblem
   }

   {
   pStrSave->BeginUnit(_T("HaulingProblem"),1.0);
   pStrSave->put_Property(_T("FpeType"),CComVariant(m_HaulingFpeType));
   if ( m_HaulingFpeType == CONSTANT_FPE )
   {
      Float64 X,FpeStraight,FpeHarped,FpeTemp;
      m_HaulingStabilityProblem.GetFpe(0,&X,&FpeStraight,&FpeHarped,&FpeTemp);
      pStrSave->put_Property(_T("Fpe_Straight"),CComVariant(FpeStraight));
      pStrSave->put_Property(_T("Fpe_Harped"),CComVariant(FpeHarped));
      pStrSave->put_Property(_T("Fpe_Temporary"),CComVariant(FpeTemp));
   }
   else
   {
      pStrSave->BeginUnit(_T("EffectivePrestress"),1.0);
      IndexType nFpe = m_HaulingStabilityProblem.GetFpeCount();
      for ( IndexType fpeIdx = 0; fpeIdx < nFpe; fpeIdx++ )
      {
         pStrSave->BeginUnit(_T("Fpe"),1.0);
         Float64 X,FpeStraight,FpeHarped,FpeTemp;
         m_HaulingStabilityProblem.GetFpe(fpeIdx,&X,&FpeStraight,&FpeHarped,&FpeTemp);
         pStrSave->put_Property(_T("X"),CComVariant(X));
         pStrSave->put_Property(_T("Fpe_Straight"),CComVariant(FpeStraight));
         pStrSave->put_Property(_T("Fpe_Harped"),CComVariant(FpeHarped));
         pStrSave->put_Property(_T("Fpe_Temporary"),CComVariant(FpeTemp));
         pStrSave->EndUnit(); // Fpe
      }
      pStrSave->EndUnit(); // EffectivePrestress
   }

   bool bDirectCamber;
   Float64 camber;
   m_HaulingStabilityProblem.GetCamber(&bDirectCamber,&camber);
   pStrSave->put_Property(_T("DirectCamber"),CComVariant(bDirectCamber));
   pStrSave->put_Property(_T("Camber"),CComVariant(camber));

   Float64 Ll,Lr;
   m_HaulingStabilityProblem.GetSupportLocations(&Ll,&Lr);
   ATLASSERT(IsEqual(Ll,Lr));
   pStrSave->put_Property(_T("LeftBunkPoint"),CComVariant(Ll));
   pStrSave->put_Property(_T("RightBunkPoint"),CComVariant(Lr));

   pStrSave->put_Property(_T("Hgb"),CComVariant(m_Hgb));

   Float64 SweepTolerance = m_HaulingStabilityProblem.GetSweepTolerance();
   pStrSave->put_Property(_T("SweepTolerance"),CComVariant(SweepTolerance));

   Float64 SupportPlacementTolerance = m_HaulingStabilityProblem.GetSupportPlacementTolerance();
   pStrSave->put_Property(_T("SupportPlacementTolerance"),CComVariant(SupportPlacementTolerance));

   Float64 imup,imdn;
   m_HaulingStabilityProblem.GetImpact(&imup,&imdn);
   pStrSave->put_Property(_T("ImpactUp"),CComVariant(imup));
   pStrSave->put_Property(_T("ImpactDown"),CComVariant(imdn));

   bool bApplyImpact = m_HaulingStabilityProblem.ApplyImpactToTiltedGirder();
   pStrSave->put_Property(_T("ApplyImpactToTiltedGirder"),CComVariant(bApplyImpact));

   Float64 WindPressure = m_HaulingStabilityProblem.GetWindPressure();
   pStrSave->put_Property(_T("WindPressure"),CComVariant(WindPressure));

   Float64 Ktheta = m_HaulingStabilityProblem.GetTruckRotationalStiffness();
   pStrSave->put_Property(_T("Ktheta"),CComVariant(Ktheta));
   Float64 Wcc = m_HaulingStabilityProblem.GetWheelLineSpacing();
   pStrSave->put_Property(_T("Wcc"),CComVariant(Wcc));
   Float64 Hrc = m_HaulingStabilityProblem.GetHeightOfRollAxisAboveRoadway();
   pStrSave->put_Property(_T("Hrc"),CComVariant(Hrc));
   Float64 Superelevation = m_HaulingStabilityProblem.GetSuperelevation();
   pStrSave->put_Property(_T("Superelevation"),CComVariant(Superelevation));
   Float64 Velocity = m_HaulingStabilityProblem.GetVelocity();
   pStrSave->put_Property(_T("Velocity"),CComVariant(Velocity));
   Float64 Radius = m_HaulingStabilityProblem.GetTurningRadius();
   pStrSave->put_Property(_T("Radius"),CComVariant(Radius));

   pStrSave->put_Property(_T("FrCoefficient"),CComVariant(m_HaulingFrCoefficient));
   pStrSave->put_Property(_T("Fc"),CComVariant(m_Fc));
   pStrSave->put_Property(_T("ComputeEc"),CComVariant(m_bComputeEc));
   Float64 Ec = m_HaulingStabilityProblem.GetEc();
   pStrSave->put_Property(_T("Ec"),CComVariant(Ec));
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

      hr = pStrLoad->BeginUnit(_T("Girder"));
      hr = pStrLoad->BeginUnit(_T("Sections"));
      m_Girder[m_GirderType].ClearSections();
      while (SUCCEEDED(pStrLoad->BeginUnit(_T("Section"))) )
      {
         var.vt = VT_R8;
         hr = pStrLoad->get_Property(_T("L"),&var);
         Float64 L = var.dblVal;

         Float64 Ag[2],Ix[2],Iy[2],Yt[2],Hg[2],Wtf[2],Wbf[2];
         
         hr = pStrLoad->BeginUnit(_T("LeftFace"));
         hr = pStrLoad->get_Property(_T("Ag"),&var);
         Ag[LEFT] = var.dblVal;

         hr = pStrLoad->get_Property(_T("Ix"),&var);
         Ix[LEFT] = var.dblVal;

         hr = pStrLoad->get_Property(_T("Iy"),&var);
         Iy[LEFT] = var.dblVal;

         hr = pStrLoad->get_Property(_T("Yt"),&var);
         Yt[LEFT] = var.dblVal;

         hr = pStrLoad->get_Property(_T("Hg"),&var);
         Hg[LEFT] = var.dblVal;

         hr = pStrLoad->get_Property(_T("Wtf"),&var);
         Wtf[LEFT] = var.dblVal;

         hr = pStrLoad->get_Property(_T("Wbf"),&var);
         Wbf[LEFT] = var.dblVal;
         hr = pStrLoad->EndUnit(); // LeftFace

         hr = pStrLoad->BeginUnit(_T("RightFace"));
         hr = pStrLoad->get_Property(_T("Ag"),&var);
         Ag[RIGHT] = var.dblVal;

         hr = pStrLoad->get_Property(_T("Ix"),&var);
         Ix[RIGHT] = var.dblVal;

         hr = pStrLoad->get_Property(_T("Iy"),&var);
         Iy[RIGHT] = var.dblVal;

         hr = pStrLoad->get_Property(_T("Yt"),&var);
         Yt[RIGHT] = var.dblVal;

         hr = pStrLoad->get_Property(_T("Hg"),&var);
         Hg[RIGHT] = var.dblVal;

         hr = pStrLoad->get_Property(_T("Wtf"),&var);
         Wtf[RIGHT] = var.dblVal;

         hr = pStrLoad->get_Property(_T("Wbf"),&var);
         Wbf[RIGHT] = var.dblVal;
         hr = pStrLoad->EndUnit(); // RightFace

         m_Girder[m_GirderType].AddSection(L,Ag[LEFT],Ix[LEFT],Iy[LEFT],Yt[LEFT],Hg[LEFT],Wtf[LEFT],Wbf[LEFT],Ag[RIGHT],Ix[RIGHT],Iy[RIGHT],Yt[RIGHT],Hg[RIGHT],Wtf[RIGHT],Wbf[RIGHT]);

         hr = pStrLoad->EndUnit(); // Section
      }
      hr = pStrLoad->EndUnit(); // Sections

      hr = pStrLoad->get_Property(_T("DensityWithRebar"),&var);
      m_Girder[m_GirderType].SetDensity(var.dblVal);

      hr = pStrLoad->get_Property(_T("Density"),&var);
      m_Density = var.dblVal;

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

      m_Strands[m_GirderType].Load(pStrLoad);
      hr = pStrLoad->EndUnit(); // Girder


      {
      hr = pStrLoad->BeginUnit(_T("LiftingProblem"));

      var.vt = VT_I4;
      hr = pStrLoad->get_Property(_T("FpeType"),&var);
      m_LiftingFpeType = var.lVal;
      if ( m_LiftingFpeType == CONSTANT_FPE )
      {
         Float64 FpeStraight,FpeHarped,FpeTemp;
         var.vt = VT_R8;
         hr = pStrLoad->get_Property(_T("Fpe_Straight"),&var);
         FpeStraight = var.dblVal;
         hr = pStrLoad->get_Property(_T("Fpe_Harped"),&var);
         FpeHarped = var.dblVal;
         hr = pStrLoad->get_Property(_T("Fpe_Temporary"),&var);
         FpeTemp = var.dblVal;
         m_LiftingStabilityProblem.AddFpe(0,FpeStraight,FpeHarped,FpeTemp);
      }
      else
      {
         hr = pStrLoad->BeginUnit(_T("EffectivePrestress"));
         m_LiftingStabilityProblem.ClearFpe();
         while ( SUCCEEDED(pStrLoad->BeginUnit(_T("Fpe"))) )
         {
            var.vt = VT_R8;
            Float64 X,FpeStraight,FpeHarped,FpeTemp;
            hr = pStrLoad->get_Property(_T("X"),&var);
            X = var.dblVal;
            hr = pStrLoad->get_Property(_T("Fpe_Straight"),&var);
            FpeStraight = var.dblVal;
            hr = pStrLoad->get_Property(_T("Fpe_Harped"),&var);
            FpeHarped = var.dblVal;
            hr = pStrLoad->get_Property(_T("Fpe_Temporary"),&var);
            FpeTemp = var.dblVal;
            m_LiftingStabilityProblem.AddFpe(X,FpeStraight,FpeHarped,FpeTemp);
            hr = pStrLoad->EndUnit(); // Fpe
         }
         hr = pStrLoad->EndUnit(); // EffectivePrestress
      }

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

      var.vt = VT_BOOL;
      hr = pStrLoad->get_Property(_T("ApplyImpactToTiltedGirder"),&var);
      m_LiftingStabilityProblem.ApplyImpactToTiltedGirder(var.boolVal == VARIANT_TRUE ? true : false);

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("WindPressure"),&var);
      m_LiftingStabilityProblem.SetWindPressure(var.dblVal);

      hr = pStrLoad->get_Property(_T("LiftAngle"),&var);
      m_LiftingStabilityProblem.SetLiftAngle(var.dblVal);

      hr = pStrLoad->get_Property(_T("FrCoefficient"),&var);
      m_LiftingFrCoefficient = var.dblVal;

      hr = pStrLoad->get_Property(_T("Fci"),&var);
      m_Fci = var.dblVal;

      var.vt = VT_BOOL;
      hr = pStrLoad->get_Property(_T("ComputeEci"),&var);
      m_bComputeEci = (var.boolVal == VARIANT_TRUE ? true : false);

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("Eci"),&var);
      m_LiftingStabilityProblem.SetEc(var.dblVal);
      hr = pStrLoad->EndUnit(); // LiftingProblem
      }

      {
      hr = pStrLoad->BeginUnit(_T("HaulingProblem"));


      var.vt = VT_I4;
      hr = pStrLoad->get_Property(_T("FpeType"),&var);
      m_HaulingFpeType = var.lVal;
      if ( m_HaulingFpeType == CONSTANT_FPE )
      {
         Float64 FpeStraight,FpeHarped,FpeTemp;
         var.vt = VT_R8;
         hr = pStrLoad->get_Property(_T("Fpe_Straight"),&var);
         FpeStraight = var.dblVal;
         hr = pStrLoad->get_Property(_T("Fpe_Harped"),&var);
         FpeHarped = var.dblVal;
         hr = pStrLoad->get_Property(_T("Fpe_Temporary"),&var);
         FpeTemp = var.dblVal;
         m_HaulingStabilityProblem.AddFpe(0,FpeStraight,FpeHarped,FpeTemp);
      }
      else
      {
         hr = pStrLoad->BeginUnit(_T("EffectivePrestress"));
         m_HaulingStabilityProblem.ClearFpe();
         while ( SUCCEEDED(pStrLoad->BeginUnit(_T("Fpe"))) )
         {
            var.vt = VT_R8;
            Float64 X,FpeStraight,FpeHarped,FpeTemp;
            hr = pStrLoad->get_Property(_T("X"),&var);
            X = var.dblVal;
            hr = pStrLoad->get_Property(_T("Fpe_Straight"),&var);
            FpeStraight = var.dblVal;
            hr = pStrLoad->get_Property(_T("Fpe_Harped"),&var);
            FpeHarped = var.dblVal;
            hr = pStrLoad->get_Property(_T("Fpe_Temporary"),&var);
            FpeTemp = var.dblVal;
            m_HaulingStabilityProblem.AddFpe(X,FpeStraight,FpeHarped,FpeTemp);
            hr = pStrLoad->EndUnit(); // Fpe
         }
         hr = pStrLoad->EndUnit(); // EffectivePrestress
      }

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

      var.vt = VT_BOOL;
      hr = pStrLoad->get_Property(_T("ApplyImpactToTiltedGirder"),&var);
      m_HaulingStabilityProblem.ApplyImpactToTiltedGirder(var.boolVal == VARIANT_TRUE ? true : false);

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("WindPressure"),&var);
      m_HaulingStabilityProblem.SetWindPressure(var.dblVal);

      hr = pStrLoad->get_Property(_T("Ktheta"),&var);
      m_HaulingStabilityProblem.SetTruckRotationalStiffness(var.dblVal);

      hr = pStrLoad->get_Property(_T("Wcc"),&var);
      m_HaulingStabilityProblem.SetWheelLineSpacing(var.dblVal);

      hr = pStrLoad->get_Property(_T("Hrc"),&var);
      m_HaulingStabilityProblem.SetHeightOfRollAxisAboveRoadway(var.dblVal);

      hr = pStrLoad->get_Property(_T("Superelevation"),&var);
      m_HaulingStabilityProblem.SetSuperelevation(var.dblVal);

      hr = pStrLoad->get_Property(_T("Velocity"),&var);
      m_HaulingStabilityProblem.SetVelocity(var.dblVal);

      hr = pStrLoad->get_Property(_T("Radius"),&var);
      m_HaulingStabilityProblem.SetTurningRadius(var.dblVal);

      hr = pStrLoad->get_Property(_T("FrCoefficient"),&var);
      m_HaulingFrCoefficient = var.dblVal;

      hr = pStrLoad->get_Property(_T("Fc"),&var);
      m_Fc = var.dblVal;

      var.vt = VT_BOOL;
      hr = pStrLoad->get_Property(_T("ComputeEc"),&var);
      m_bComputeEc = (var.boolVal == VARIANT_TRUE ? true : false);

      var.vt = VT_R8;
      hr = pStrLoad->get_Property(_T("Ec"),&var);
      m_HaulingStabilityProblem.SetEc(var.dblVal);
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
      m_Strands[NONPRISMATIC] = m_Strands[PRISMATIC];
   }
   else
   {
      m_Girder[PRISMATIC] = m_Girder[NONPRISMATIC];

      Float64 L = m_Girder[NONPRISMATIC].GetSectionLength(0);
      Float64 Ag,Ix,Iy,Yt,Hg,Wtf,Wbf;
      m_Girder[NONPRISMATIC].GetSectionProperties(0,LEFT,&Ag,&Ix,&Iy,&Yt,&Hg,&Wtf,&Wbf);

      m_Girder[PRISMATIC].ClearSections();
      m_Girder[PRISMATIC].AddSection(L,Ag,Ix,Iy,Yt,Hg,Wtf,Wbf);

      m_Strands[PRISMATIC] = m_Strands[NONPRISMATIC];
      m_Strands[PRISMATIC].Xh1 = 0.0;
      m_Strands[PRISMATIC].Xh1Measure = FRACTION;

      m_Strands[PRISMATIC].Xh4 = 1.0;
      m_Strands[PRISMATIC].Xh4Measure = FRACTION;
      m_Strands[PRISMATIC].Yh4 = m_Strands[PRISMATIC].Yh1;
      m_Strands[PRISMATIC].Yh4Measure = m_Strands[PRISMATIC].Yh1Measure;

      Float64 hp;
      if ( m_Strands[NONPRISMATIC].Xh2Measure == DISTANCE )
      {
         hp = m_Strands[NONPRISMATIC].Xh2/m_Girder[NONPRISMATIC].GetGirderLength();
         if ( 0.5 < hp )
         {
            hp = 0.5;
         }
      }
      else
      {
         hp = m_Strands[NONPRISMATIC].Xh2;
      }
      m_Strands[PRISMATIC].Xh2 = hp;
      m_Strands[PRISMATIC].Xh2Measure = FRACTION;
      m_Strands[PRISMATIC].Xh3 = 1 - hp;
      m_Strands[PRISMATIC].Xh3Measure = FRACTION;
      m_Strands[PRISMATIC].Yh3 = m_Strands[PRISMATIC].Yh2;
      m_Strands[PRISMATIC].Yh3Measure = m_Strands[PRISMATIC].Yh2Measure;
   }

   return S_OK;
}

