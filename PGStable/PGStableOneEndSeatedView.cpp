///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2023  Washington State Department of Transportation
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

// PGStableOneEndSeatedView.cpp : implementation file
//

#include "stdafx.h"
#include "..\resource.h"
#include "PGStableDoc.h"
#include "PGStableOneEndSeatedView.h"
#include "PGStableEffectivePrestressDlg.h"
#include <MFCTools\MFCTools.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CPGStableOneEndSeatedView

IMPLEMENT_DYNCREATE(CPGStableOneEndSeatedView, CPGStableFormView)

CPGStableOneEndSeatedView::CPGStableOneEndSeatedView()
	: CPGStableFormView(CPGStableOneEndSeatedView::IDD)
{
}

CPGStableOneEndSeatedView::~CPGStableOneEndSeatedView()
{
}

void CPGStableOneEndSeatedView::DoDataExchange(CDataExchange* pDX)
{
	CPGStableFormView::DoDataExchange(pDX);

   DDX_Control(pDX, IDC_EC, m_ctrlEc);
   DDX_Control(pDX, IDC_FC, m_ctrlFc);

   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();

   CString strHaulTruck = pDoc->GetOneEndSeatedHaulTruck();
   DDX_CBString(pDX,IDC_HAUL_TRUCK,strHaulTruck);

   Float64 fc, frCoefficient;
   bool bComputeEc;
   pDoc->GetOneEndSeatedMaterials(&fc,&bComputeEc,&frCoefficient);
   bComputeEc = !bComputeEc;

   WBFL::Stability::OneEndSeatedStabilityProblem problem = pDoc->GetOneEndSeatedStabilityProblem();

   WBFL::Stability::WindLoadType windLoadType;
   Float64 windLoad;
   problem.GetWindLoading(&windLoadType,&windLoad);
   DDX_CBEnum(pDX,IDC_WIND_TYPE,windLoadType);
   if ( windLoadType == WBFL::Stability::WindLoadType::Speed )
   {
      DDX_UnitValueAndTag(pDX,IDC_WIND_PRESSURE,IDC_WIND_PRESSURE_UNIT,windLoad,pDispUnits->Velocity);
   }
   else
   {
      DDX_UnitValueAndTag(pDX,IDC_WIND_PRESSURE,IDC_WIND_PRESSURE_UNIT,windLoad,pDispUnits->WindPressure);
   }

   Float64 eb, Wb;
   problem.GetAppurtenanceLoading(&eb, &Wb);
   DDX_UnitValueAndTag(pDX, IDC_BRACKET_WEIGHT, IDC_BRACKET_WEIGHT_UNIT, Wb, pDispUnits->ForcePerLength);
   DDX_UnitValueAndTag(pDX, IDC_BRACKET_ECCENTRICITY, IDC_BRACKET_ECCENTRICITY_UNIT, eb, pDispUnits->ComponentDim);

   Float64 Fs,Fh,Ft;
   GetMaxFpe(&Fs,&Fh,&Ft);
   DDX_UnitValueAndTag(pDX,IDC_FPE_STRAIGHT,IDC_FPE_STRAIGHT_UNIT,Fs,pDispUnits->GeneralForce);
   DDX_UnitValueAndTag(pDX,IDC_FPE_HARPED,IDC_FPE_HARPED_UNIT,Fh,pDispUnits->GeneralForce);
   DDX_UnitValueAndTag(pDX,IDC_FPE_TEMP,IDC_FPE_TEMP_UNIT,Ft,pDispUnits->GeneralForce);


   DDX_UnitValueAndTag(pDX,IDC_FC,IDC_FC_UNIT,fc,pDispUnits->Stress);
   Float64 Ec = problem.GetConcrete().GetE(); // this is the computed value
   DDX_Check_Bool(pDX,IDC_COMPUTE_EC,bComputeEc);
   DDX_UnitValueAndTag(pDX,IDC_EC,IDC_EC_UNIT,Ec,pDispUnits->ModE);

   Float64 sweepTolerance = problem.GetSweepTolerance();
   Float64 sweepGrowth = problem.GetSweepGrowth();
   Float64 supportPlacementTolerance = problem.GetSupportPlacementTolerance();

   Float64 impactUp, impactDown;
   problem.GetImpact(&impactUp,&impactDown);
   DDX_Percentage(pDX,IDC_IMPACT_UP,impactUp);
   DDX_Percentage(pDX,IDC_IMPACT_DOWN,impactDown);

   Float64 camber = problem.GetCamber();
   Float64 camberMultiplier = problem.GetCamberMultiplier();

   DDX_UnitValueAndTag(pDX,IDC_CAMBER,IDC_CAMBER_UNIT,camber,pDispUnits->ComponentDim);
   DDX_Text(pDX, IDC_CAMBER_MULTIPLIER, camberMultiplier);

   Float64 lateralCamber = problem.GetLateralCamber();
   DDX_UnitValueAndTag(pDX, IDC_LATERAL_CAMBER, IDC_LATERAL_CAMBER_UNIT, lateralCamber, pDispUnits->ComponentDim);

   DDX_UnitValueAndTag(pDX,IDC_FR_COEFFICIENT,IDC_FR_COEFFICIENT_UNIT,frCoefficient,pDispUnits->SqrtPressure);
   CString tag;
   if ( WBFL::LRFD::BDSManager::GetEdition() < WBFL::LRFD::BDSManager::Edition::SeventhEditionWith2016Interims )
   {
      tag = pApp->GetUnitsMode() == eafTypes::umSI ? _T("sqrt(f'c (MPa))") : _T("sqrt(f'c (KSI))");
   }
   else
   {
      tag = pApp->GetUnitsMode() == eafTypes::umSI ? _T("(lambda)sqrt(f'c (MPa))") : _T("(lambda)sqrt(f'c (KSI))");
   }
   DDX_Text(pDX,IDC_FR_COEFFICIENT_UNIT,tag);

   Float64 Ll, Lr;
   problem.GetSupportLocations(&Ll, &Lr);
   DDX_UnitValueAndTag(pDX,IDC_LEFT_BUNK,IDC_LEFT_BUNK_UNIT,Ll,pDispUnits->SpanLength);
   DDX_UnitValueAndTag(pDX,IDC_RIGHT_BUNK,IDC_RIGHT_BUNK_UNIT,Lr,pDispUnits->SpanLength);

   Float64 ylift = problem.GetYRollLiftEnd();
   DDX_UnitValueAndTag(pDX, IDC_YLIFT, IDC_YLIFT_UNIT, ylift, pDispUnits->ComponentDim);

   Float64 elift = problem.GetLiftPlacementTolerance();
   DDX_UnitValueAndTag(pDX, IDC_LIFT_POINT_TOLERANCE, IDC_LIFT_POINT_TOLERANCE_UNIT, elift, pDispUnits->ComponentDim);

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

   DDX_UnitValueAndTag(pDX, IDC_SWEEP_GROWTH, IDC_SWEEP_GROWTH_UNIT, sweepGrowth, pDispUnits->ComponentDim);

   DDX_UnitValueAndTag(pDX,IDC_SUPPORT_PLACEMENT_TOLERANCE,IDC_SUPPORT_PLACEMENT_TOLERANCE_UNIT,supportPlacementTolerance,pDispUnits->ComponentDim);

   Float64 Hgb = pDoc->GetHeightOfGirderBottomAboveRoadway();
   DDX_UnitValueAndTag(pDX,IDC_HGB,IDC_HGB_UNIT,Hgb,pDispUnits->ComponentDim);

   Float64 Hrc = problem.GetHeightOfRollAxis();
   DDX_UnitValueAndTag(pDX,IDC_HRC,IDC_HRC_UNIT,Hrc,pDispUnits->ComponentDim);

   CString slope_unit(pApp->GetUnitsMode() == eafTypes::umSI ? _T("m/m") : _T("ft/ft"));

   Float64 crown_slope = problem.GetSupportSlope();
   DDX_Text(pDX,IDC_CROWN_SLOPE,crown_slope);
   DDX_Text(pDX,IDC_CROWN_SLOPE_UNIT,slope_unit);

   Float64 Wcc = problem.GetSupportWidth();
   DDX_UnitValueAndTag(pDX,IDC_WCC,IDC_WCC_UNIT,Wcc,pDispUnits->ComponentDim);

   Float64 Ktheta = problem.GetRotationalStiffness();
   DDX_UnitValueAndTag(pDX,IDC_KTHETA,IDC_KTHETA_UNIT,Ktheta,pDispUnits->MomentPerAngle);

   Float64 Kadjust = problem.GetRotationalStiffnessAdjustmentFactor();
   DDX_Percentage(pDX, IDC_FRACTION_OF_ROLL_STIFFNESS, Kadjust);

   DDX_Text(pDX,IDC_HAULING_FS_CRACKING,m_OneEndSeatedCriteria.MinFScr);
   DDX_Text(pDX,IDC_HAULING_FS_FAILURE,m_OneEndSeatedCriteria.MinFSf);
   DDX_Text(pDX, IDC_HAULING_GLOBAL_COMPRESSION, m_OneEndSeatedCriteria.CompressionCoefficient_GlobalStress);
   DDX_Text(pDX, IDC_HAULING_PEAK_COMPRESSION, m_OneEndSeatedCriteria.CompressionCoefficient_PeakStress);

   auto* pOneEndSeatedTensionStressLimit = dynamic_cast<WBFL::Stability::CCOneEndSeatedTensionStressLimit*>(m_OneEndSeatedCriteria.TensionStressLimit.get());

   DDX_UnitValue(pDX,IDC_HAULING_TENSION_CROWN, pOneEndSeatedTensionStressLimit->TensionCoefficient,pDispUnits->SqrtPressure);
   DDX_Text(pDX,IDC_HAULING_TENSION_CROWN_UNIT,tag);
   DDX_Check_Bool(pDX,IDC_CHECK_HAULING_TENSION_MAX_CROWN, pOneEndSeatedTensionStressLimit->bMaxTension);
   DDX_UnitValueAndTag(pDX,IDC_HAULING_TENSION_MAX_CROWN,IDC_HAULING_TENSION_MAX_CROWN_UNIT, pOneEndSeatedTensionStressLimit->MaxTension,pDispUnits->Stress);
   DDX_UnitValue(pDX,IDC_HAULING_TENSION_WITH_REBAR_CROWN, pOneEndSeatedTensionStressLimit->TensionCoefficientWithRebar,pDispUnits->SqrtPressure);
   DDX_Text(pDX,IDC_HAULING_TENSION_WITH_REBAR_CROWN_UNIT,tag);

   if ( pDX->m_bSaveAndValidate )
   {
      problem.GetConcrete().SetE(Ec);

      problem.SetSweepTolerance(sweepTolerance);
      problem.SetSweepGrowth(sweepGrowth);
      problem.SetSupportPlacementTolerance(supportPlacementTolerance);

      problem.SetSupportLocations(Ll,Lr);

      problem.SetYRollLiftEnd(ylift);
      problem.SetLiftPlacementTolerance(elift);

      problem.SetImpact(impactUp,impactDown);

      problem.SetWindLoading(windLoadType,windLoad);

      problem.SetAppurtenanceLoading(eb, Wb);

      problem.SetSupportSlope(crown_slope);
      problem.SetSupportWidth(Wcc);
      problem.SetRotationalStiffness(Ktheta);
      problem.SetHeightOfRollAxis(Hrc);
      problem.SetRotationalStiffnessAdjustmentFactor(Kadjust);

      pDoc->SetHeightOfGirderBottomAboveRoadway(Hgb);

      problem.SetCamber(camber);
      problem.SetCamberMultiplier(camberMultiplier);

      problem.SetLateralCamber(lateralCamber);
      problem.IncludeLateralRollAxisOffset(IsZero(lateralCamber) ? false : true);

      pDoc->SetOneEndSeatedStabilityProblem(problem);
      pDoc->SetOneEndSeatedMaterials(fc,!bComputeEc,frCoefficient);

      pDoc->SetOneEndSeatedCriteria(m_OneEndSeatedCriteria);

      auto strands = pDoc->GetStrands(pDoc->GetGirderType(), ModelType::OneEndSeated);
      if ( strands.strandMethod == CPGStableStrands::Simplified )
      {
         strands.FpeStraight = Fs;
         strands.FpeHarped   = Fh;
         strands.FpeTemp     = Ft;
      }
      pDoc->SetStrands(pDoc->GetGirderType(),ModelType::OneEndSeated, strands);

      pDoc->SetOneEndSeatedHaulTruck(strHaulTruck);

   }
}

BEGIN_MESSAGE_MAP(CPGStableOneEndSeatedView, CPGStableFormView)
   ON_BN_CLICKED(IDC_COMPUTE_EC, &CPGStableOneEndSeatedView::OnUserEc)
	ON_EN_CHANGE(IDC_FC, &CPGStableOneEndSeatedView::OnChangeFc)
   ON_EN_CHANGE(IDC_FPE_STRAIGHT, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_FPE_HARPED, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_FPE_TEMP, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_EC, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_FR_COEFFICIENT, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_LEFT_BUNK, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_RIGHT_BUNK, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_IMPACT_UP, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_IMPACT_DOWN, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_CROWN_SLOPE, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_SUPERELEVATION, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_WIND_PRESSURE, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_BRACKET_WEIGHT, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_BRACKET_ECCENTRICITY, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_VELOCITY, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_RADIUS, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_CAMBER, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_CAMBER_MULTIPLIER, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_HGB, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_HRC, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_KTHETA, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_WCC, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_SWEEP_TOLERANCE, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_SWEEP_GROWTH, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_LATERAL_SWEEP_INCREMENT, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_SUPPORT_PLACEMENT_TOLERANCE, &CPGStableOneEndSeatedView::OnChange)
   
   ON_EN_CHANGE(IDC_YLIFT, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_LIFT_POINT_TOLERANCE, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_LIFT_END_OFFSET, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_FRACTION_OF_ROLL_STIFFNESS, &CPGStableOneEndSeatedView::OnChange)

   ON_BN_CLICKED(IDC_EDIT_FPE, &CPGStableOneEndSeatedView::OnEditFpe)
   ON_CBN_SELCHANGE(IDC_CF_TYPE,&CPGStableOneEndSeatedView::OnChange)
   ON_COMMAND(ID_FILE_PRINT,&CPGStableOneEndSeatedView::OnPrint)
   ON_COMMAND(ID_FILE_PRINT_DIRECT,&CPGStableOneEndSeatedView::OnPrintDirect)
   ON_BN_CLICKED(IDC_CHECK_HAULING_TENSION_MAX_CROWN, &CPGStableOneEndSeatedView::OnClickedOneEndSeatedTensionMaxCrown)
   ON_WM_SIZE()
   ON_CBN_SELCHANGE(IDC_WIND_TYPE, &CPGStableOneEndSeatedView::OnWindTypeChanged)
   ON_CBN_SELCHANGE(IDC_IMPACT_USAGE, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_HAULING_TENSION_CROWN,&CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_HAULING_TENSION_MAX_CROWN,&CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_HAULING_TENSION_WITH_REBAR_CROWN,&CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_LATERAL_CAMBER, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_HAULING_FS_CRACKING, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_HAULING_FS_FAILURE, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_HAULING_GLOBAL_COMPRESSION, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_HAULING_PEAK_COMPRESSION, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_HAULING_TENSION_CROWN, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_HAULING_TENSION_MAX_CROWN, &CPGStableOneEndSeatedView::OnChange)
   ON_EN_CHANGE(IDC_HAULING_TENSION_WITH_REBAR_CROWN, &CPGStableOneEndSeatedView::OnChange)
   ON_BN_CLICKED(IDC_COPY, &CPGStableOneEndSeatedView::OnCopy)
   ON_MESSAGE(WM_HELP, OnCommandHelp)
   ON_CBN_SELCHANGE(IDC_HAUL_TRUCK, &CPGStableOneEndSeatedView::OnHaulTruckChanged)
   ON_COMMAND_RANGE(CCS_CMENU_BASE, CCS_CMENU_MAX, OnCmenuSelected)
END_MESSAGE_MAP()

LRESULT CPGStableOneEndSeatedView::OnCommandHelp(WPARAM, LPARAM lParam)
{
   EAFHelp( EAFGetDocument()->GetDocumentationSetName(), IDH_PGSTABLE_ONEENDSEATED_VIEW );
   return TRUE;
}

void CPGStableOneEndSeatedView::OnCmenuSelected(UINT id)
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

  case CCS_RB_COPY:
     m_pBrowser->Copy();
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


// CPGStableOneEndSeatedView diagnostics

#ifdef _DEBUG
void CPGStableOneEndSeatedView::AssertValid() const
{
   AFX_MANAGE_STATE(AfxGetAppModuleState());
	CPGStableFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPGStableOneEndSeatedView::Dump(CDumpContext& dc) const
{
	CPGStableFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPGStableOneEndSeatedView message handlers

BOOL CPGStableOneEndSeatedView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,	DWORD dwRequestedStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,CCreateContext* pContext)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return CPGStableFormView::Create(lpszClassName,lpszWindowName,dwRequestedStyle,rect,pParentWnd,nID,pContext);
}

void CPGStableOneEndSeatedView::OnActivateView()
{
   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();
   m_OneEndSeatedCriteria = pDoc->GetOneEndSeatedCriteria();

   UpdateData(FALSE);

   if ( m_strUserEc == _T("") )
   {
      m_ctrlEc.GetWindowText(m_strUserEc);
   }

   UpdateFpeControls();
   UpdateCriteriaControls();
   OnHaulTruckChanged();


   OnUserEc();

   RefreshReport();
}

void CPGStableOneEndSeatedView::OnDeactivateView()
{
   UpdateData();
}

void CPGStableOneEndSeatedView::OnUserEc()
{
   BOOL bEnable = ((CButton*)GetDlgItem(IDC_COMPUTE_EC))->GetCheck();
   GetDlgItem(IDC_EC)->EnableWindow(bEnable);
   GetDlgItem(IDC_EC_UNIT)->EnableWindow(bEnable);

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

void CPGStableOneEndSeatedView::UpdateEc()
{
   if ( IsDlgButtonChecked(IDC_COMPUTE_EC) )
   {
      return;
   }

   CString strFc;
   m_ctrlFc.GetWindowText(strFc);

   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();
   CString strEc = pDoc->UpdateEc(strFc);
   m_ctrlEc.SetWindowText(strEc);
}

void CPGStableOneEndSeatedView::OnChangeFc() 
{
   UpdateEc();
   OnChange();
}

void CPGStableOneEndSeatedView::OnChange()
{
   UpdateData();
   RefreshReport();
}

void CPGStableOneEndSeatedView::UpdateFpeControls()
{
   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();
   const auto& strands = pDoc->GetStrands(pDoc->GetGirderType(), ModelType::OneEndSeated);
   BOOL bEnable = (strands.strandMethod == CPGStableStrands::Simplified ? TRUE : FALSE);
   GetDlgItem(IDC_FPE_STRAIGHT)->EnableWindow(bEnable);
   GetDlgItem(IDC_FPE_HARPED)->EnableWindow(bEnable);
   GetDlgItem(IDC_FPE_TEMP)->EnableWindow(bEnable);

   GetDlgItem(IDC_EDIT_FPE)->ShowWindow(bEnable ? SW_HIDE : SW_SHOW);
}

void CPGStableOneEndSeatedView::UpdateCriteriaControls()
{
   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();
   BOOL bEnable = pDoc->GetCriteria() == gs_strCriteria ? TRUE : FALSE;
   GetDlgItem(IDC_IMPACT_UP)->EnableWindow(bEnable);
   GetDlgItem(IDC_IMPACT_DOWN)->EnableWindow(bEnable);
   GetDlgItem(IDC_CROWN_SLOPE)->EnableWindow(bEnable);
   GetDlgItem(IDC_WIND_TYPE)->EnableWindow(bEnable);
   GetDlgItem(IDC_WIND_PRESSURE)->EnableWindow(bEnable);
   GetDlgItem(IDC_CAMBER)->EnableWindow(TRUE); // if camber is a direct input value, always enable
   GetDlgItem(IDC_CAMBER_MULTIPLIER)->EnableWindow(bEnable);
   GetDlgItem(IDC_SWEEP_TOLERANCE)->EnableWindow(bEnable);
   GetDlgItem(IDC_SWEEP_GROWTH)->EnableWindow(bEnable);
   GetDlgItem(IDC_SUPPORT_PLACEMENT_TOLERANCE)->EnableWindow(bEnable);
   GetDlgItem(IDC_HAULING_FS_CRACKING)->EnableWindow(bEnable);
   GetDlgItem(IDC_HAULING_FS_FAILURE)->EnableWindow(bEnable);
   GetDlgItem(IDC_HAULING_GLOBAL_COMPRESSION)->EnableWindow(bEnable);
   GetDlgItem(IDC_HAULING_PEAK_COMPRESSION)->EnableWindow(bEnable);
   GetDlgItem(IDC_HAULING_TENSION_CROWN)->EnableWindow(bEnable);
   GetDlgItem(IDC_CHECK_HAULING_TENSION_MAX_CROWN)->EnableWindow(bEnable);
   GetDlgItem(IDC_HAULING_TENSION_MAX_CROWN)->EnableWindow(bEnable);
   GetDlgItem(IDC_HAULING_TENSION_WITH_REBAR_CROWN)->EnableWindow(bEnable);
}

void CPGStableOneEndSeatedView::OnEditFpe()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CPGStableEffectivePrestressDlg dlg;
   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();
   dlg.m_Strands = pDoc->GetStrands(pDoc->GetGirderType(), ModelType::OneEndSeated);
   if ( dlg.DoModal() == IDOK )
   {
      pDoc->SetStrands(pDoc->GetGirderType(), ModelType::OneEndSeated, dlg.m_Strands);

      Float64 Fs,Fh,Ft;
      GetMaxFpe(&Fs,&Fh,&Ft);
      CDataExchange dx(this,FALSE);
      CEAFApp* pApp = EAFGetApp();
      const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();
      DDX_UnitValueAndTag(&dx,IDC_FPE_STRAIGHT,IDC_FPE_STRAIGHT_UNIT,Fs,pDispUnits->GeneralForce);
      DDX_UnitValueAndTag(&dx,IDC_FPE_HARPED,IDC_FPE_HARPED_UNIT,Fh,pDispUnits->GeneralForce);
      DDX_UnitValueAndTag(&dx,IDC_FPE_TEMP,IDC_FPE_TEMP_UNIT,Ft,pDispUnits->GeneralForce);
   }
}

void CPGStableOneEndSeatedView::OnInitialUpdate()
{
   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();

   const HaulTruckLibrary* pLib = pDoc->GetHaulTruckLibrary();
   WBFL::Library::KeyListType keyList;
   pLib->KeyList(keyList);
   CComboBox* pcbHaulTruck = (CComboBox*)GetDlgItem(IDC_HAUL_TRUCK);
   pcbHaulTruck->AddString(gs_strHaulTruck);
   for (const auto& key : keyList)
   {
      pcbHaulTruck->AddString(key.c_str());
   }

   CWnd* pWnd = GetDlgItem(IDC_BROWSER);
   pWnd->ShowWindow(SW_HIDE);

   std::shared_ptr<WBFL::Reporting::ReportBuilder> pRptBuilder = pDoc->GetReportManager()->GetReportBuilder(_T("OneEndSeated"));
   WBFL::Reporting::ReportDescription rptDesc = pRptBuilder->GetReportDescription();

   std::shared_ptr<WBFL::Reporting::ReportSpecificationBuilder> pRptSpecBuilder = pRptBuilder->GetReportSpecificationBuilder();
   m_pRptSpec = pRptSpecBuilder->CreateDefaultReportSpec(rptDesc);

   m_pBrowser = pDoc->GetReportManager()->CreateReportBrowser(GetSafeHwnd(),m_pRptSpec, std::shared_ptr<const WBFL::Reporting::ReportSpecificationBuilder>());

   m_pBrowser->GetBrowserWnd()->ModifyStyle(0,WS_BORDER);

   CComboBox* pcbWindType = (CComboBox*)GetDlgItem(IDC_WIND_TYPE);
   pcbWindType->SetItemData(pcbWindType->AddString(_T("Wind Speed")),(DWORD_PTR)WBFL::Stability::WindLoadType::Speed);
   pcbWindType->SetItemData(pcbWindType->AddString(_T("Wind Pressure")),(DWORD_PTR)WBFL::Stability::WindLoadType::Pressure);

   CPGStableFormView::OnInitialUpdate();

   UpdateFpeControls();

   OnCopy();
}

void CPGStableOneEndSeatedView::GetMaxFpe(Float64* pFpeStraight,Float64* pFpeHarped,Float64* pFpeTemp)
{
   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();
   const auto& strands = pDoc->GetStrands(pDoc->GetGirderType(), ModelType::OneEndSeated);

   if (strands.strandMethod == CPGStableStrands::Simplified )
   {
      *pFpeStraight = strands.FpeStraight;
      *pFpeHarped   = strands.FpeHarped;
      *pFpeTemp     = strands.FpeTemp;
   }
   else
   {
      Float64 FpeStraight = 0;
      Float64 FpeHarped = 0;
      Float64 FpeTemporary = 0;
      for (const auto& fpe : strands.m_vFpe)
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

void CPGStableOneEndSeatedView::RefreshReport()
{
   if ( m_pRptSpec == nullptr )
      return;

   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();

   // refresh the report
   m_pRptSpec = m_pBrowser->GetReportSpecification();
   std::shared_ptr<WBFL::Reporting::ReportBuilder> pBuilder = pDoc->GetReportManager()->GetReportBuilder( m_pRptSpec->GetReportName() );
   std::shared_ptr<rptReport> pReport = pBuilder->CreateReport( m_pRptSpec );
   m_pBrowser->UpdateReport( pReport, true );

}

void CPGStableOneEndSeatedView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
   if ( EAFGetActiveView() == this || lHint == HINT_UPDATE_DATA)
   {
      if ( lHint == EAF_HINT_UNITS_CHANGING || lHint == HINT_UPDATE_DATA)
      {
         UpdateData(TRUE);
      }
      else if ( lHint == EAF_HINT_UNITS_CHANGED )
      {
         UpdateData(FALSE);
         OnChange();
      }
   }
   if (m_pBrowser) m_pBrowser->Refresh();
}

void CPGStableOneEndSeatedView::OnClickedOneEndSeatedTensionMaxCrown()
{
   BOOL bEnable = IsDlgButtonChecked(IDC_CHECK_HAULING_TENSION_MAX_CROWN) == BST_CHECKED ? TRUE : FALSE;
   GetDlgItem(IDC_HAULING_TENSION_MAX_CROWN)->EnableWindow(bEnable);
   OnChange();
}

void CPGStableOneEndSeatedView::OnSize(UINT nType, int cx, int cy)
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

void CPGStableOneEndSeatedView::OnPrint() 
{
   m_pBrowser->Print(true);
}

void CPGStableOneEndSeatedView::OnPrintDirect() 
{
   m_pBrowser->Print(false);
}

void CPGStableOneEndSeatedView::OnWindTypeChanged()
{
   CComboBox* pcbWindType = (CComboBox*)GetDlgItem(IDC_WIND_TYPE);
   int curSel = pcbWindType->GetCurSel();
   WBFL::Stability::WindLoadType windType = (WBFL::Stability::WindLoadType)pcbWindType->GetItemData(curSel);
   CDataExchange dx(this,false);
   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();
   if ( windType == WBFL::Stability::WindLoadType::Speed )
   {
      DDX_Tag(&dx,IDC_WIND_PRESSURE_UNIT,pDispUnits->Velocity);
   }
   else
   {
      DDX_Tag(&dx,IDC_WIND_PRESSURE_UNIT,pDispUnits->WindPressure);
   }
   OnChange();
}

void CPGStableOneEndSeatedView::OnHaulTruckChanged()
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


      CEAFApp* pApp = EAFGetApp();
      const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();
      CDataExchange dx(this,FALSE);
      DDX_UnitValueAndTag(&dx,IDC_HGB,IDC_HGB_UNIT,Hgb,pDispUnits->ComponentDim);
      DDX_UnitValueAndTag(&dx,IDC_HRC,IDC_HRC_UNIT,Hrc,pDispUnits->ComponentDim);
      DDX_UnitValueAndTag(&dx,IDC_WCC,IDC_WCC_UNIT,Wcc,pDispUnits->ComponentDim);
      DDX_UnitValueAndTag(&dx,IDC_KTHETA,IDC_KTHETA_UNIT,Ktheta,pDispUnits->MomentPerAngle);

      OnChange();
   }
}


void CPGStableOneEndSeatedView::OnCopy()
{
   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();
   const WBFL::Stability::LiftingStabilityProblem& lifting_problem = pDoc->GetLiftingStabilityProblem();
   const WBFL::Stability::HaulingStabilityProblem& hauling_problem = pDoc->GetHaulingStabilityProblem();

   auto problem = pDoc->GetOneEndSeatedStabilityProblem();

   // copy prestressing
   GirderType girderType = pDoc->GetGirderType();
   const CPGStableStrands& hauling_strands = pDoc->GetStrands(girderType, ModelType::Hauling);
   pDoc->SetStrands(girderType, ModelType::OneEndSeated, hauling_strands);

   // copy materials
   problem.SetConcrete(hauling_problem.GetConcrete());

   // copy wind loading
   WBFL::Stability::WindLoadType windType;
   Float64 windLoad;
   hauling_problem.GetWindLoading(&windType, &windLoad);
   problem.SetWindLoading(windType, windLoad);

   // copy overhang bracket
   Float64 eb, Wb;
   hauling_problem.GetAppurtenanceLoading(&eb, &Wb);
   problem.SetAppurtenanceLoading(eb, Wb);

   // copy camber
   problem.SetCamber(hauling_problem.GetCamber());
   problem.SetCamberMultiplier(hauling_problem.GetCamberMultiplier());
   problem.SetLateralCamber(hauling_problem.GetLateralCamber());
   problem.IncludeLateralRollAxisOffset(hauling_problem.IncludeLateralRollAxisOffset());

   // copy lift point
   problem.SetYRollLiftEnd(lifting_problem.GetYRollAxis());
   problem.SetLiftPlacementTolerance(lifting_problem.GetSupportPlacementTolerance());

   // copy haul truck
   pDoc->SetOneEndSeatedHaulTruck(pDoc->GetHaulTruck());
   problem.SetYRollAxis(hauling_problem.GetYRollAxis());
   problem.SetSupportPlacementTolerance(hauling_problem.GetSupportPlacementTolerance());
   problem.SetRotationalStiffness(hauling_problem.GetRotationalStiffness());
   problem.SetSupportWidth(hauling_problem.GetSupportWidth());
   problem.SetHeightOfRollAxis(hauling_problem.GetHeightOfRollAxis());
   
   // copy sweep
   problem.SetSweepTolerance(hauling_problem.GetSweepTolerance());
   problem.SetSweepGrowth(hauling_problem.GetSweepGrowth());

   // copy support locations - use the leading truck support assuming the trailing end is hanging from a crain
   Float64 Llift1, Llift2;
   lifting_problem.GetSupportLocations(&Llift1, &Llift2);
   Float64 Lt, Ll;
   hauling_problem.GetSupportLocations(&Lt, &Ll);
   problem.SetSupportLocations(Ll, Llift1);

   // copy criteria
   const CPGStableHaulingCriteria& hauling_criteria = pDoc->GetHaulingCriteria();
   CPGStableOneEndSeatedCriteria criteria = pDoc->GetOneEndSeatedCriteria();
   criteria.MinFScr = hauling_criteria.MinFScr;
   criteria.MinFSf = hauling_criteria.MinFSf;
   criteria.AllowableCompression_GlobalStress = hauling_criteria.AllowableCompression_GlobalStress;
   criteria.AllowableCompression_PeakStress = hauling_criteria.AllowableCompression_PeakStress;
   criteria.CompressionCoefficient_GlobalStress = hauling_criteria.CompressionCoefficient_GlobalStress;
   criteria.CompressionCoefficient_PeakStress = hauling_criteria.CompressionCoefficient_PeakStress;

   const auto* pHaulingTensionStressLimit = dynamic_cast<const WBFL::Stability::CCHaulingTensionStressLimit*>(hauling_criteria.TensionStressLimit.get());
   auto* pOneEndSeatedTensionStressLimit = dynamic_cast<WBFL::Stability::CCOneEndSeatedTensionStressLimit*>(criteria.TensionStressLimit.get());
   pOneEndSeatedTensionStressLimit->Lambda = pHaulingTensionStressLimit->Lambda;
   pOneEndSeatedTensionStressLimit->TensionCoefficient = pHaulingTensionStressLimit->TensionCoefficient[+WBFL::Stability::HaulingSlope::CrownSlope];
   pOneEndSeatedTensionStressLimit->bMaxTension = pHaulingTensionStressLimit->bMaxTension[+WBFL::Stability::HaulingSlope::CrownSlope];
   pOneEndSeatedTensionStressLimit->MaxTension = pHaulingTensionStressLimit->MaxTension[+WBFL::Stability::HaulingSlope::CrownSlope];
   pOneEndSeatedTensionStressLimit->AllowableTension = pHaulingTensionStressLimit->AllowableTension[+WBFL::Stability::HaulingSlope::CrownSlope];
   pOneEndSeatedTensionStressLimit->bWithRebarLimit = pHaulingTensionStressLimit->bWithRebarLimit[+WBFL::Stability::HaulingSlope::CrownSlope];
   pOneEndSeatedTensionStressLimit->TensionCoefficientWithRebar = pHaulingTensionStressLimit->TensionCoefficientWithRebar[+WBFL::Stability::HaulingSlope::CrownSlope];
   pOneEndSeatedTensionStressLimit->AllowableTensionWithRebar = pHaulingTensionStressLimit->AllowableTensionWithRebar[+WBFL::Stability::HaulingSlope::CrownSlope];

   pDoc->SetOneEndSeatedCriteria(criteria);

   pDoc->SetOneEndSeatedStabilityProblem(problem);
   UpdateData(FALSE);
   OnHaulTruckChanged();
   OnClickedOneEndSeatedTensionMaxCrown();
}