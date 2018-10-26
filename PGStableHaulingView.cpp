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

// PGStableHaulingView.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PGStableDoc.h"
#include "PGStableHaulingView.h"
#include "PGStableEffectivePrestressDlg.h"
#include <MFCTools\MFCTools.h>


// CPGStableHaulingView

IMPLEMENT_DYNCREATE(CPGStableHaulingView, CPGStableFormView)

CPGStableHaulingView::CPGStableHaulingView()
	: CPGStableFormView(CPGStableHaulingView::IDD)
{
}

CPGStableHaulingView::~CPGStableHaulingView()
{
}

void CPGStableHaulingView::DoDataExchange(CDataExchange* pDX)
{
	CPGStableFormView::DoDataExchange(pDX);

   DDX_Control(pDX, IDC_EC, m_ctrlEc);
   DDX_Control(pDX, IDC_FC, m_ctrlFc);
   DDX_Control(pDX, IDC_K1, m_ctrlK1);
   DDX_Control(pDX, IDC_K2, m_ctrlK2);

   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();

   CString strHaulTruck = pDoc->GetHaulTruck();
   DDX_CBString(pDX,IDC_HAUL_TRUCK,strHaulTruck);

   Float64 fc, frCoefficient;
   bool bComputeEc;
   pDoc->GetHaulingMaterials(&fc,&bComputeEc,&frCoefficient);
   bComputeEc = !bComputeEc;

   stbHaulingStabilityProblem problem = pDoc->GetHaulingStabilityProblem();

   stbTypes::HaulingImpact impactUsage = problem.GetImpactUsage();
   DDX_CBItemData(pDX,IDC_IMPACT_USAGE,impactUsage);

   stbTypes::WindType windLoadType;
   Float64 windLoad;
   problem.GetWindLoading(&windLoadType,&windLoad);
   DDX_CBEnum(pDX,IDC_WIND_TYPE,windLoadType);
   if ( windLoadType == stbTypes::Speed )
   {
      DDX_UnitValueAndTag(pDX,IDC_WIND_PRESSURE,IDC_WIND_PRESSURE_UNIT,windLoad,pDispUnits->Velocity);
   }
   else
   {
      DDX_UnitValueAndTag(pDX,IDC_WIND_PRESSURE,IDC_WIND_PRESSURE_UNIT,windLoad,pDispUnits->WindPressure);
   }

   Float64 Ll,Lr;
   problem.GetSupportLocations(&Ll,&Lr);

   Float64 Fs,Fh,Ft;
   GetMaxFpe(&Fs,&Fh,&Ft);
   DDX_UnitValueAndTag(pDX,IDC_FPE_STRAIGHT,IDC_FPE_STRAIGHT_UNIT,Fs,pDispUnits->GeneralForce);
   DDX_UnitValueAndTag(pDX,IDC_FPE_HARPED,IDC_FPE_HARPED_UNIT,Fh,pDispUnits->GeneralForce);
   DDX_UnitValueAndTag(pDX,IDC_FPE_TEMP,IDC_FPE_TEMP_UNIT,Ft,pDispUnits->GeneralForce);


   DDX_UnitValueAndTag(pDX,IDC_FC,IDC_FC_UNIT,fc,pDispUnits->Stress);
   Float64 Ec = problem.GetConcrete().GetE(); // this is the computed value
   DDX_Check_Bool(pDX,IDC_COMPUTE_EC,bComputeEc);
   DDX_UnitValueAndTag(pDX,IDC_EC,IDC_EC_UNIT,Ec,pDispUnits->ModE);

   Float64 K1 = pDoc->GetK1();
   Float64 K2 = pDoc->GetK2();
   DDX_Text(pDX,IDC_K1,K1);
   DDX_Text(pDX,IDC_K2,K2);

   Float64 sweepTolerance = problem.GetSweepTolerance();
   Float64 supportPlacementTolerance = problem.GetSupportPlacementTolerance();

   Float64 impactUp, impactDown;
   problem.GetImpact(&impactUp,&impactDown);
   DDX_Percentage(pDX,IDC_IMPACT_UP,impactUp);
   DDX_Percentage(pDX,IDC_IMPACT_DOWN,impactDown);

   bool bDirectCamber;
   Float64 camber;
   Float64 camberOffset;
   problem.GetCamber(&bDirectCamber,&camber);
   Float64 camberMultiplier = problem.GetCamberMultiplier();
   int camber_method;
   if ( bDirectCamber )
   {
      camber_method = 1;
      camberOffset = 0.02;
   }
   else
   {
      camber_method = 0;
      camberOffset = camber;
      camber = ::ConvertToSysUnits(1.0,unitMeasure::Inch);
   }
   DDX_Radio(pDX,IDC_CAMBER1,camber_method);
   DDX_Percentage(pDX,IDC_CAMBER_OFFSET,camberOffset);
   DDX_UnitValueAndTag(pDX,IDC_CAMBER,IDC_CAMBER_UNIT,camber,pDispUnits->ComponentDim);
   DDX_Text(pDX, IDC_CAMBER_MULTIPLIER, camberMultiplier);

   DDX_UnitValueAndTag(pDX,IDC_FR_COEFFICIENT,IDC_FR_COEFFICIENT_UNIT,frCoefficient,pDispUnits->SqrtPressure);
   CString tag;
   if ( lrfdVersionMgr::GetVersion() < lrfdVersionMgr::SeventhEditionWith2016Interims )
   {
      tag = pApp->GetUnitsMode() == eafTypes::umSI ? _T("sqrt(f'c (MPa))") : _T("sqrt(f'c (KSI))");
   }
   else
   {
      tag = pApp->GetUnitsMode() == eafTypes::umSI ? _T("(lambda)sqrt(f'c (MPa))") : _T("(lambda)sqrt(f'c (KSI))");
   }
   DDX_Text(pDX,IDC_FR_COEFFICIENT_UNIT,tag);

   DDX_UnitValueAndTag(pDX,IDC_LEFT_BUNK,IDC_LEFT_BUNK_UNIT,Ll,pDispUnits->SpanLength);
   DDX_UnitValueAndTag(pDX,IDC_RIGHT_BUNK,IDC_RIGHT_BUNK_UNIT,Lr,pDispUnits->SpanLength);

   Float64 velocity = problem.GetVelocity();
   Float64 radius = problem.GetTurningRadius();
   stbTypes::CFType cfType = problem.GetCentrifugalForceType();
   DDX_UnitValueAndTag(pDX,IDC_VELOCITY,IDC_VELOCITY_UNIT,velocity,pDispUnits->Velocity);
   DDX_UnitValueAndTag(pDX,IDC_RADIUS,IDC_RADIUS_UNIT,radius,pDispUnits->AlignmentLength);
   DDX_CBEnum(pDX,IDC_CF_TYPE,cfType);

   if ( pApp->GetUnitsMode() == eafTypes::umSI )
   {
      sweepTolerance *= 1000;
      DDX_Text(pDX,IDC_SWEEP_TOLERANCE,sweepTolerance);
      if ( pDX->m_bSaveAndValidate )
      {
         sweepTolerance /= 1000;
      }
      else
      {
         GetDlgItem(IDC_SWEEP_TOLERANCE_LABEL)->SetWindowText(_T(""));
         GetDlgItem(IDC_SWEEP_TOLERANCE_UNIT)->SetWindowText(_T("mm/m"));
      }
   }
   else
   {
      INT x = (INT)::RoundOff((1.0/(sweepTolerance*120.0)),1.0);
      DDX_Text(pDX,IDC_SWEEP_TOLERANCE,x);

      if ( pDX->m_bSaveAndValidate )
      {
         sweepTolerance = 1.0/(x*120.0);
      }
      else
      {
         GetDlgItem(IDC_SWEEP_TOLERANCE_LABEL)->SetWindowText(_T("1/"));
         GetDlgItem(IDC_SWEEP_TOLERANCE_UNIT)->SetWindowText(_T("in/10 ft"));
      }
   }

   DDX_UnitValueAndTag(pDX,IDC_SUPPORT_PLACEMENT_TOLERANCE,IDC_SUPPORT_PLACEMENT_TOLERANCE_UNIT,supportPlacementTolerance,pDispUnits->ComponentDim);

   Float64 Hgb = pDoc->GetHeightOfGirderBottomAboveRoadway();
   DDX_UnitValueAndTag(pDX,IDC_HGB,IDC_HGB_UNIT,Hgb,pDispUnits->ComponentDim);

   Float64 Hrc = problem.GetHeightOfRollAxisAboveRoadway();
   DDX_UnitValueAndTag(pDX,IDC_HRC,IDC_HRC_UNIT,Hrc,pDispUnits->ComponentDim);

   CString slope_unit(pApp->GetUnitsMode() == eafTypes::umSI ? _T("m/m") : _T("ft/ft"));

   Float64 crown_slope = problem.GetCrownSlope();
   DDX_Text(pDX,IDC_CROWN_SLOPE,crown_slope);
   DDX_Text(pDX,IDC_CROWN_SLOPE_UNIT,slope_unit);

   Float64 superelevation = problem.GetSuperelevation();
   DDX_Text(pDX,IDC_SUPERELEVATION,superelevation);
   DDX_Text(pDX,IDC_SUPERELEVATION_UNIT,slope_unit);

   Float64 Wcc = problem.GetWheelLineSpacing();
   DDX_UnitValueAndTag(pDX,IDC_WCC,IDC_WCC_UNIT,Wcc,pDispUnits->ComponentDim);

   Float64 Ktheta = problem.GetTruckRotationalStiffness();
   DDX_UnitValueAndTag(pDX,IDC_KTHETA,IDC_KTHETA_UNIT,Ktheta,pDispUnits->MomentPerAngle);


   CString strHaulingTag(pApp->GetUnitsMode() == eafTypes::umUS ? _T("sqrt(f'c (KSI))") : _T("sqrt(f'c (MPa))"));

   DDX_Text(pDX,IDC_HAULING_FS_CRACKING,m_HaulingCriteria.MinFScr);
   DDX_Text(pDX,IDC_HAULING_FS_FAILURE,m_HaulingCriteria.MinFSf);
   DDX_UnitValueAndTag(pDX,IDC_MAX_BUNK,IDC_MAX_BUNK_UNIT,m_HaulingCriteria.MaxClearSpan,pDispUnits->SpanLength);
   DDX_UnitValueAndTag(pDX,IDC_LEADING_OVERHANG,IDC_LEADING_OVERHANG_UNIT,m_HaulingCriteria.MaxLeadingOverhang,pDispUnits->SpanLength);
   DDX_UnitValueAndTag(pDX,IDC_MAX_GIRDER_WEIGHT,IDC_MAX_GIRDER_WEIGHT_UNIT,m_HaulingCriteria.MaxGirderWeight,pDispUnits->GeneralForce);
   DDX_Text(pDX,IDC_HAULING_COMPRESSION,m_HaulingCriteria.CompressionCoefficient);

   DDX_UnitValue(pDX,IDC_HAULING_TENSION_CROWN,m_HaulingCriteria.TensionCoefficient[stbTypes::CrownSlope],pDispUnits->SqrtPressure);
   DDX_Text(pDX,IDC_HAULING_TENSION_CROWN_UNIT,strHaulingTag);
   DDX_Check_Bool(pDX,IDC_CHECK_HAULING_TENSION_MAX_CROWN,m_HaulingCriteria.bMaxTension[stbTypes::CrownSlope]);
   DDX_UnitValueAndTag(pDX,IDC_HAULING_TENSION_MAX_CROWN,IDC_HAULING_TENSION_MAX_CROWN_UNIT,m_HaulingCriteria.MaxTension[stbTypes::CrownSlope],pDispUnits->Stress);
   DDX_UnitValue(pDX,IDC_HAULING_TENSION_WITH_REBAR_CROWN,m_HaulingCriteria.TensionCoefficientWithRebar[stbTypes::CrownSlope],pDispUnits->SqrtPressure);
   DDX_Text(pDX,IDC_HAULING_TENSION_WITH_REBAR_CROWN_UNIT,strHaulingTag);

   DDX_UnitValue(pDX,IDC_HAULING_TENSION_SUPER,m_HaulingCriteria.TensionCoefficient[stbTypes::MaxSuper],pDispUnits->SqrtPressure);
   DDX_Text(pDX,IDC_HAULING_TENSION_SUPER_UNIT,strHaulingTag);
   DDX_Check_Bool(pDX,IDC_CHECK_HAULING_TENSION_MAX_SUPER,m_HaulingCriteria.bMaxTension[stbTypes::MaxSuper]);
   DDX_UnitValueAndTag(pDX,IDC_HAULING_TENSION_MAX_SUPER,IDC_HAULING_TENSION_MAX_SUPER_UNIT,m_HaulingCriteria.MaxTension[stbTypes::MaxSuper],pDispUnits->Stress);
   DDX_UnitValue(pDX,IDC_HAULING_TENSION_WITH_REBAR_SUPER,m_HaulingCriteria.TensionCoefficientWithRebar[stbTypes::MaxSuper],pDispUnits->SqrtPressure);
   DDX_Text(pDX,IDC_HAULING_TENSION_WITH_REBAR_SUPER_UNIT,strHaulingTag);

   bool bEvaluateStressesAtEquilibriumAngle = problem.EvaluateStressesAtEquilibriumAngle();
   DDX_CBItemData(pDX, IDC_STRESSES, bEvaluateStressesAtEquilibriumAngle);

   if ( pDX->m_bSaveAndValidate )
   {
      problem.GetConcrete().SetE(Ec);
      pDoc->SetK1(K1);
      pDoc->SetK2(K2);

      problem.SetSweepTolerance(sweepTolerance);
      problem.SetSupportPlacementTolerance(supportPlacementTolerance);

      problem.SetSupportLocations(Ll,Lr);

      problem.SetImpact(impactUp,impactDown);
      problem.SetImpactUsage(impactUsage);

      problem.SetWindLoading(windLoadType,windLoad);

      problem.SetVelocity(velocity);
      problem.SetTurningRadius(radius);
      problem.SetCentrifugalForceType(cfType);

      problem.SetCrownSlope(crown_slope);
      problem.SetSuperelevation(superelevation);
      problem.SetWheelLineSpacing(Wcc);
      problem.SetTruckRotationalStiffness(Ktheta);
      problem.SetHeightOfRollAxisAboveRoadway(Hrc);

      pDoc->SetHeightOfGirderBottomAboveRoadway(Hgb);

      if ( camber_method == 0 )
      {
         bDirectCamber = false;
         camber = camberOffset;
      }
      else
      {
         bDirectCamber = true;
      }
      problem.SetCamber(bDirectCamber,camber);
      problem.SetCamberMultiplier(camberMultiplier);

      problem.EvaluateStressesAtEquilibriumAngle(bEvaluateStressesAtEquilibriumAngle);

      pDoc->SetHaulingStabilityProblem(problem);
      pDoc->SetHaulingMaterials(fc,!bComputeEc,frCoefficient);

      pDoc->SetHaulingCriteria(m_HaulingCriteria);

      if ( m_Strands.strandMethod == CPGStableStrands::Simplified )
      {
         m_Strands.FpeStraight = Fs;
         m_Strands.FpeHarped   = Fh;
         m_Strands.FpeTemp     = Ft;
      }
      pDoc->SetStrands(pDoc->GetGirderType(),HAULING,m_Strands);

      pDoc->SetHaulTruck(strHaulTruck);

   }
}

BEGIN_MESSAGE_MAP(CPGStableHaulingView, CPGStableFormView)
   ON_BN_CLICKED(IDC_COMPUTE_EC, &CPGStableHaulingView::OnUserEc)
	ON_EN_CHANGE(IDC_FC, &CPGStableHaulingView::OnChangeFc)
   ON_EN_CHANGE(IDC_K1, &CPGStableHaulingView::OnChangeFc)
   ON_EN_CHANGE(IDC_K2, &CPGStableHaulingView::OnChangeFc)
   ON_EN_CHANGE(IDC_FPE_STRAIGHT, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_FPE_HARPED, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_FPE_TEMP, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_EC, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_FR_COEFFICIENT, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_LEFT_BUNK, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_RIGHT_BUNK, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_IMPACT_UP, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_IMPACT_DOWN, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_CROWN_SLOPE, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_SUPERELEVATION, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_WIND_PRESSURE, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_VELOCITY, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_RADIUS, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_CAMBER_OFFSET, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_CAMBER, &CPGStableHaulingView::OnChange)
   ON_BN_CLICKED(IDC_CAMBER1, &CPGStableHaulingView::OnChange)
   ON_BN_CLICKED(IDC_CAMBER2, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_CAMBER_MULTIPLIER, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_HGB, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_HRC, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_KTHETA, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_WCC, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_SWEEP_TOLERANCE, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_LATERAL_SWEEP_INCREMENT, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_SUPPORT_PLACEMENT_TOLERANCE, &CPGStableHaulingView::OnChange)
   ON_BN_CLICKED(IDC_EDIT_FPE, &CPGStableHaulingView::OnEditFpe)
   ON_CBN_SELCHANGE(IDC_CF_TYPE,&CPGStableHaulingView::OnChange)
   ON_COMMAND(ID_FILE_PRINT,&CPGStableHaulingView::OnPrint)
   ON_COMMAND(ID_FILE_PRINT_DIRECT,&CPGStableHaulingView::OnPrintDirect)
   ON_BN_CLICKED(IDC_CHECK_HAULING_TENSION_MAX_CROWN, &CPGStableHaulingView::OnClickedHaulingTensionMaxCrown)
   ON_BN_CLICKED(IDC_CHECK_HAULING_TENSION_MAX_SUPER, &CPGStableHaulingView::OnClickedHaulingTensionMaxSuper)
   ON_WM_SIZE()
   ON_CBN_SELCHANGE(IDC_WIND_TYPE, &CPGStableHaulingView::OnWindTypeChanged)
   ON_CBN_SELCHANGE(IDC_IMPACT_USAGE, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_HAULING_COMPRESSION,&CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_HAULING_TENSION_CROWN,&CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_HAULING_TENSION_MAX_CROWN,&CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_HAULING_TENSION_WITH_REBAR_CROWN,&CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_HAULING_TENSION_SUPER,&CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_HAULING_TENSION_MAX_SUPER,&CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_HAULING_TENSION_WITH_REBAR_SUPER,&CPGStableHaulingView::OnChange)
	ON_MESSAGE(WM_HELP, OnCommandHelp)
   ON_CBN_SELCHANGE(IDC_HAUL_TRUCK, &CPGStableHaulingView::OnHaulTruckChanged)
   ON_CBN_SELCHANGE(IDC_STRESSES, &CPGStableHaulingView::OnChange)
   ON_COMMAND_RANGE(CCS_CMENU_BASE, CCS_CMENU_MAX, OnCmenuSelected)
END_MESSAGE_MAP()

LRESULT CPGStableHaulingView::OnCommandHelp(WPARAM, LPARAM lParam)
{
   EAFHelp( EAFGetDocument()->GetDocumentationSetName(), IDH_PGSTABLE_HAULING_VIEW );
   return TRUE;
}

void CPGStableHaulingView::OnCmenuSelected(UINT id)
{
  UINT cmd = id-CCS_CMENU_BASE ;

  switch(cmd)
  {
  //case CCS_RB_EDIT:
  //   EditReport();
  //   break;

  case CCS_RB_FIND:
     m_pBrowser->Find();
     break;

  case CCS_RB_SELECT_ALL:
     m_pBrowser->SelectAll();
     break;
  case CCS_RB_PRINT:
     m_pBrowser->Print(true);
     break;

  case CCS_RB_REFRESH:
     m_pBrowser->Refresh();
     break;

  case CCS_RB_VIEW_SOURCE:
     m_pBrowser->ViewSource();
     break;

  case CCS_RB_VIEW_BACK:
     m_pBrowser->Back();
     break;

  case CCS_RB_VIEW_FORWARD:
     m_pBrowser->Forward();
     break;

  default:
     // must be a toc anchor
     ATLASSERT(CCS_RB_TOC <= cmd);
     m_pBrowser->NavigateAnchor(cmd-CCS_RB_TOC);
  }
}


// CPGStableHaulingView diagnostics

#ifdef _DEBUG
void CPGStableHaulingView::AssertValid() const
{
   AFX_MANAGE_STATE(AfxGetAppModuleState());
	CPGStableFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPGStableHaulingView::Dump(CDumpContext& dc) const
{
	CPGStableFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPGStableHaulingView message handlers

BOOL CPGStableHaulingView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,	DWORD dwRequestedStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,CCreateContext* pContext)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return CPGStableFormView::Create(lpszClassName,lpszWindowName,dwRequestedStyle,rect,pParentWnd,nID,pContext);
}

void CPGStableHaulingView::OnActivateView()
{
   UpdateData(FALSE);

   if ( m_strUserEc == _T("") )
   {
      m_ctrlEc.GetWindowText(m_strUserEc);
   }

   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();
   m_Strands = pDoc->GetStrands(pDoc->GetGirderType(),HAULING);
   UpdateFpeControls();
   UpdateCriteriaControls();
   OnHaulTruckChanged();


   OnUserEc();

   RefreshReport();
}

void CPGStableHaulingView::OnDeactivateView()
{
   UpdateData();
}

void CPGStableHaulingView::OnUserEc()
{
   BOOL bEnable = ((CButton*)GetDlgItem(IDC_COMPUTE_EC))->GetCheck();
   GetDlgItem(IDC_EC)->EnableWindow(bEnable);
   GetDlgItem(IDC_EC_UNIT)->EnableWindow(bEnable);
   GetDlgItem(IDC_K1)->EnableWindow(!bEnable);
   GetDlgItem(IDC_K2)->EnableWindow(!bEnable);

   if (bEnable==FALSE)
   {
      m_ctrlEc.GetWindowText(m_strUserEc);
      UpdateEc();
   }
   else
   {
      m_ctrlEc.SetWindowText(m_strUserEc);
   }
}

void CPGStableHaulingView::UpdateEc()
{
   if ( IsDlgButtonChecked(IDC_COMPUTE_EC) )
   {
      return;
   }

    // need to manually parse strength and density values
   CString strFc, strDensity, strK1, strK2;
   m_ctrlFc.GetWindowText(strFc);
   m_ctrlK1.GetWindowText(strK1);
   m_ctrlK2.GetWindowText(strK2);

   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();

   strDensity.Format(_T("%s"),FormatDimension(pDoc->GetDensity(),pDispUnits->Density,false));

   CString strEc = pDoc->UpdateEc(strFc,strDensity,strK1,strK2);
   m_ctrlEc.SetWindowText(strEc);
}

void CPGStableHaulingView::OnChangeFc() 
{
   UpdateEc();
   OnChange();
}

void CPGStableHaulingView::OnChange()
{
   UpdateData();
   RefreshReport();
}

void CPGStableHaulingView::UpdateFpeControls()
{
   BOOL bEnable = (m_Strands.strandMethod == CPGStableStrands::Simplified ? TRUE : FALSE);
   GetDlgItem(IDC_FPE_STRAIGHT)->EnableWindow(bEnable);
   GetDlgItem(IDC_FPE_HARPED)->EnableWindow(bEnable);
   GetDlgItem(IDC_FPE_TEMP)->EnableWindow(bEnable);

   GetDlgItem(IDC_EDIT_FPE)->ShowWindow(bEnable ? SW_HIDE : SW_SHOW);
}

void CPGStableHaulingView::UpdateCriteriaControls()
{
   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();
   BOOL bEnable = pDoc->GetCriteria() == gs_strCriteria ? TRUE : FALSE;
   GetDlgItem(IDC_IMPACT_UP)->EnableWindow(bEnable);
   GetDlgItem(IDC_IMPACT_DOWN)->EnableWindow(bEnable);
   GetDlgItem(IDC_IMPACT_USAGE)->EnableWindow(bEnable);
   GetDlgItem(IDC_CROWN_SLOPE)->EnableWindow(bEnable);
   GetDlgItem(IDC_SUPERELEVATION)->EnableWindow(bEnable);
   GetDlgItem(IDC_WIND_TYPE)->EnableWindow(bEnable);
   GetDlgItem(IDC_WIND_PRESSURE)->EnableWindow(bEnable);
   GetDlgItem(IDC_CF_TYPE)->EnableWindow(bEnable);
   GetDlgItem(IDC_VELOCITY)->EnableWindow(bEnable);
   GetDlgItem(IDC_RADIUS)->EnableWindow(bEnable);
   GetDlgItem(IDC_CAMBER1)->EnableWindow(bEnable);
   GetDlgItem(IDC_CAMBER2)->EnableWindow(bEnable);
   GetDlgItem(IDC_CAMBER_OFFSET)->EnableWindow(bEnable);
   if (GetCheckedRadioButton(IDC_CAMBER1,IDC_CAMBER2) == IDC_CAMBER1 )
   {
      GetDlgItem(IDC_CAMBER)->EnableWindow(bEnable == FALSE ? FALSE : TRUE); // disable if we are disabling controls and camber is by the % method
   }
   else
   {
      GetDlgItem(IDC_CAMBER)->EnableWindow(TRUE); // if camber is a direct input value, always enable
   }
   GetDlgItem(IDC_CAMBER_MULTIPLIER)->EnableWindow(bEnable);
   GetDlgItem(IDC_SWEEP_TOLERANCE)->EnableWindow(bEnable);
   GetDlgItem(IDC_SUPPORT_PLACEMENT_TOLERANCE)->EnableWindow(bEnable);
   GetDlgItem(IDC_HAULING_FS_CRACKING)->EnableWindow(bEnable);
   GetDlgItem(IDC_HAULING_FS_FAILURE)->EnableWindow(bEnable);
   GetDlgItem(IDC_HAULING_COMPRESSION)->EnableWindow(bEnable);
   GetDlgItem(IDC_HAULING_TENSION_CROWN)->EnableWindow(bEnable);
   GetDlgItem(IDC_CHECK_HAULING_TENSION_MAX_CROWN)->EnableWindow(bEnable);
   GetDlgItem(IDC_HAULING_TENSION_MAX_CROWN)->EnableWindow(bEnable);
   GetDlgItem(IDC_HAULING_TENSION_WITH_REBAR_CROWN)->EnableWindow(bEnable);
   GetDlgItem(IDC_HAULING_TENSION_SUPER)->EnableWindow(bEnable);
   GetDlgItem(IDC_CHECK_HAULING_TENSION_MAX_SUPER)->EnableWindow(bEnable);
   GetDlgItem(IDC_HAULING_TENSION_MAX_SUPER)->EnableWindow(bEnable);
   GetDlgItem(IDC_HAULING_TENSION_WITH_REBAR_SUPER)->EnableWindow(bEnable);
   GetDlgItem(IDC_STRESSES)->EnableWindow(bEnable);
}

void CPGStableHaulingView::OnEditFpe()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CPGStableEffectivePrestressDlg dlg;
   dlg.m_Strands = m_Strands;
   if ( dlg.DoModal() == IDOK )
   {
      m_Strands = dlg.m_Strands;

      Float64 Fs,Fh,Ft;
      GetMaxFpe(&Fs,&Fh,&Ft);
      CDataExchange dx(this,FALSE);
      CEAFApp* pApp = EAFGetApp();
      const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();
      DDX_UnitValueAndTag(&dx,IDC_FPE_STRAIGHT,IDC_FPE_STRAIGHT_UNIT,Fs,pDispUnits->GeneralForce);
      DDX_UnitValueAndTag(&dx,IDC_FPE_HARPED,IDC_FPE_HARPED_UNIT,Fh,pDispUnits->GeneralForce);
      DDX_UnitValueAndTag(&dx,IDC_FPE_TEMP,IDC_FPE_TEMP_UNIT,Ft,pDispUnits->GeneralForce);
   }
}

void CPGStableHaulingView::OnInitialUpdate()
{
   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();

   const HaulTruckLibrary* pLib = pDoc->GetHaulTruckLibrary();
   libKeyListType keyList;
   pLib->KeyList(keyList);
   CComboBox* pcbHaulTruck = (CComboBox*)GetDlgItem(IDC_HAUL_TRUCK);
   pcbHaulTruck->AddString(gs_strHaulTruck);
   for (const auto& key : keyList)
   {
      pcbHaulTruck->AddString(key.c_str());
   }

   m_Strands = pDoc->GetStrands(pDoc->GetGirderType(),HAULING);
   m_HaulingCriteria = pDoc->GetHaulingCriteria();

   CWnd* pWnd = GetDlgItem(IDC_BROWSER);
   pWnd->ShowWindow(SW_HIDE);

   std::shared_ptr<CReportBuilder> pRptBuilder = pDoc->m_RptMgr.GetReportBuilder(_T("Hauling"));
   CReportDescription rptDesc = pRptBuilder->GetReportDescription();

   std::shared_ptr<CReportSpecificationBuilder> pRptSpecBuilder = pRptBuilder->GetReportSpecificationBuilder();
   m_pRptSpec = pRptSpecBuilder->CreateDefaultReportSpec(rptDesc);

   m_pBrowser = pDoc->m_RptMgr.CreateReportBrowser(GetSafeHwnd(),m_pRptSpec, std::shared_ptr<CReportSpecificationBuilder>());

   m_pBrowser->GetBrowserWnd()->ModifyStyle(0,WS_BORDER);

   CComboBox* pcbImpactUsage = (CComboBox*)GetDlgItem(IDC_IMPACT_USAGE);
   pcbImpactUsage->SetItemData(pcbImpactUsage->AddString(_T("Normal Crown Slope and Max. Superelevation Cases")),(DWORD_PTR)stbTypes::Both);
   pcbImpactUsage->SetItemData(pcbImpactUsage->AddString(_T("Normal Crown Slope Case Only")),(DWORD_PTR)stbTypes::NormalCrown);
   pcbImpactUsage->SetItemData(pcbImpactUsage->AddString(_T("Max. Superelevation Case Only")),(DWORD_PTR)stbTypes::MaxSuper);

   CComboBox* pcbWindType = (CComboBox*)GetDlgItem(IDC_WIND_TYPE);
   pcbWindType->SetItemData(pcbWindType->AddString(_T("Wind Speed")),(DWORD_PTR)stbTypes::Speed);
   pcbWindType->SetItemData(pcbWindType->AddString(_T("Wind Pressure")),(DWORD_PTR)stbTypes::Pressure);

   CComboBox* pcbCFType = (CComboBox*)GetDlgItem(IDC_CF_TYPE);
   pcbCFType->SetItemData(pcbCFType->AddString(_T("Adverse")),(DWORD_PTR)stbTypes::Adverse);
   pcbCFType->SetItemData(pcbCFType->AddString(_T("Favorable")),(DWORD_PTR)stbTypes::Favorable);

   CComboBox* pcbStresses = (CComboBox*)GetDlgItem(IDC_STRESSES);
   pcbStresses->SetItemData(pcbStresses->AddString(_T("Include girder stability equilibrium angle in stress calculations")), (DWORD_PTR)true);
   pcbStresses->SetItemData(pcbStresses->AddString(_T("Ignore girder stability equilibrium angle in stress calculations")), (DWORD_PTR)false);

   CPGStableFormView::OnInitialUpdate();

   UpdateFpeControls();
   OnHaulTruckChanged();

   OnClickedHaulingTensionMaxCrown();
   OnClickedHaulingTensionMaxSuper();
}

void CPGStableHaulingView::GetMaxFpe(Float64* pFpeStraight,Float64* pFpeHarped,Float64* pFpeTemp)
{
   if ( m_Strands.strandMethod == CPGStableStrands::Simplified )
   {
      *pFpeStraight = m_Strands.FpeStraight;
      *pFpeHarped   = m_Strands.FpeHarped;
      *pFpeTemp     = m_Strands.FpeTemp;
   }
   else
   {
      Float64 FpeStraight = 0;
      Float64 FpeHarped = 0;
      Float64 FpeTemporary = 0;
      for (const auto& fpe : m_Strands.m_vFpe)
      {
         FpeStraight  = Max(FpeStraight,fpe.FpeStraight);
         FpeHarped    = Max(FpeHarped,fpe.FpeHarped);
         FpeTemporary = Max(FpeTemporary,fpe.FpeTemp);
      }

      *pFpeStraight = FpeStraight;
      *pFpeHarped   = FpeHarped;
      *pFpeTemp     = FpeTemporary;
   }
}

void CPGStableHaulingView::RefreshReport()
{
   if ( m_pRptSpec == nullptr )
      return;

   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();

   // refresh the report
   m_pRptSpec = m_pBrowser->GetReportSpecification();
   std::shared_ptr<CReportBuilder> pBuilder = pDoc->m_RptMgr.GetReportBuilder( m_pRptSpec->GetReportName() );
   std::shared_ptr<rptReport> pReport = pBuilder->CreateReport( m_pRptSpec );
   m_pBrowser->UpdateReport( pReport, true );

}

void CPGStableHaulingView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* /*pHint*/)
{
   if ( EAFGetActiveView() == this )
   {
      if ( lHint == EAF_HINT_UNITS_CHANGING )
      {
         UpdateData(TRUE);
      }
      else if ( lHint == EAF_HINT_UNITS_CHANGED )
      {
         UpdateData(FALSE);
         OnChange();
      }
   }
}

void CPGStableHaulingView::OnClickedHaulingTensionMaxCrown()
{
   BOOL bEnable = IsDlgButtonChecked(IDC_CHECK_HAULING_TENSION_MAX_CROWN) == BST_CHECKED ? TRUE : FALSE;
   GetDlgItem(IDC_HAULING_TENSION_MAX_CROWN)->EnableWindow(bEnable);
}

void CPGStableHaulingView::OnClickedHaulingTensionMaxSuper()
{
   BOOL bEnable = IsDlgButtonChecked(IDC_CHECK_HAULING_TENSION_MAX_SUPER) == BST_CHECKED ? TRUE : FALSE;
   GetDlgItem(IDC_HAULING_TENSION_MAX_SUPER)->EnableWindow(bEnable);
}

void CPGStableHaulingView::OnSize(UINT nType, int cx, int cy)
{
   CPGStableFormView::OnSize(nType, cx, cy);

   if ( m_pBrowser )
   {
      // Convert a 7du x 7du rect into pixels
      CRect sizeRect(0,0,7,7);
      MapDialogRect(GetSafeHwnd(),&sizeRect);

      CRect clientRect;
      GetClientRect(&clientRect);

      CWnd* pWnd = GetDlgItem(IDC_BROWSER);
      CRect placeholderRect;
      pWnd->GetWindowRect(&placeholderRect);
      ScreenToClient(&placeholderRect);

      CRect browserRect;
      browserRect.left = placeholderRect.left;
      browserRect.top  = placeholderRect.top;
      browserRect.right = clientRect.right - sizeRect.Width();
      browserRect.bottom = clientRect.bottom - sizeRect.Height();

      m_pBrowser->Move(browserRect.TopLeft());
      m_pBrowser->Size(browserRect.Size() );

      Invalidate();
   }
}

void CPGStableHaulingView::OnPrint() 
{
   m_pBrowser->Print(true);
}

void CPGStableHaulingView::OnPrintDirect() 
{
   m_pBrowser->Print(false);
}

void CPGStableHaulingView::OnWindTypeChanged()
{
   CComboBox* pcbWindType = (CComboBox*)GetDlgItem(IDC_WIND_TYPE);
   int curSel = pcbWindType->GetCurSel();
   stbTypes::WindType windType = (stbTypes::WindType)pcbWindType->GetItemData(curSel);
   CDataExchange dx(this,false);
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();
   if ( windType == stbTypes::Speed )
   {
      DDX_Tag(&dx,IDC_WIND_PRESSURE_UNIT,pDispUnits->Velocity);
   }
   else
   {
      DDX_Tag(&dx,IDC_WIND_PRESSURE_UNIT,pDispUnits->WindPressure);
   }
   OnChange();
}

void CPGStableHaulingView::OnHaulTruckChanged()
{
   CString strHaulTruck;
   CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_HAUL_TRUCK);
   int curSel = pCB->GetCurSel();
   pCB->GetLBText(curSel,strHaulTruck);

   BOOL bEnable = strHaulTruck == gs_strHaulTruck ? TRUE : FALSE;
   GetDlgItem(IDC_HGB)->EnableWindow(bEnable);
   GetDlgItem(IDC_HRC)->EnableWindow(bEnable);
   GetDlgItem(IDC_KTHETA)->EnableWindow(bEnable);
   GetDlgItem(IDC_WCC)->EnableWindow(bEnable);
   GetDlgItem(IDC_MAX_BUNK)->EnableWindow(bEnable);
   GetDlgItem(IDC_LEADING_OVERHANG)->EnableWindow(bEnable);
   GetDlgItem(IDC_MAX_GIRDER_WEIGHT)->EnableWindow(bEnable);

   if ( !bEnable )
   {
      // get the data for the selected haul truck
      CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();
      const HaulTruckLibrary* pLib = pDoc->GetHaulTruckLibrary();
      const HaulTruckLibraryEntry* pEntry = (const HaulTruckLibraryEntry*)(pLib->GetEntry(strHaulTruck));

      Float64 Hgb = pEntry->GetBottomOfGirderHeight();
      Float64 Hrc = pEntry->GetRollCenterHeight();
      Float64 Wcc = pEntry->GetAxleWidth();
      Float64 Ktheta = pEntry->GetRollStiffness();
      Float64 Lmax = pEntry->GetMaxDistanceBetweenBunkPoints();
      Float64 MaxOH = pEntry->GetMaximumLeadingOverhang();
      Float64 Wmax = pEntry->GetMaxGirderWeight();


      CEAFApp* pApp = EAFGetApp();
      const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();
      CDataExchange dx(this,FALSE);
      DDX_UnitValueAndTag(&dx,IDC_HGB,IDC_HGB_UNIT,Hgb,pDispUnits->ComponentDim);
      DDX_UnitValueAndTag(&dx,IDC_HRC,IDC_HRC_UNIT,Hrc,pDispUnits->ComponentDim);
      DDX_UnitValueAndTag(&dx,IDC_WCC,IDC_WCC_UNIT,Wcc,pDispUnits->ComponentDim);
      DDX_UnitValueAndTag(&dx,IDC_KTHETA,IDC_KTHETA_UNIT,Ktheta,pDispUnits->MomentPerAngle);
      DDX_UnitValueAndTag(&dx,IDC_MAX_BUNK,IDC_MAX_BUNK_UNIT,Lmax,pDispUnits->SpanLength);
      DDX_UnitValueAndTag(&dx,IDC_LEADING_OVERHANG,IDC_LEADING_OVERHANG_UNIT,MaxOH,pDispUnits->SpanLength);
      DDX_UnitValueAndTag(&dx,IDC_MAX_GIRDER_WEIGHT,IDC_MAX_GIRDER_WEIGHT_UNIT,Wmax,pDispUnits->GeneralForce);

      OnChange();
   }
}
