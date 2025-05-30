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

// PGStableLiftingView.cpp : implementation file
//

#include "stdafx.h"
#include "..\resource.h"
#include "PGStableDoc.h"
#include "PGStableLiftingView.h"
#include "PGStableEffectivePrestressDlg.h"
#include <MFCTools\MFCTools.h>

// CPGStableLiftingView

IMPLEMENT_DYNCREATE(CPGStableLiftingView, CPGStableFormView)

CPGStableLiftingView::CPGStableLiftingView()
	: CPGStableFormView(CPGStableLiftingView::IDD)
{
}

CPGStableLiftingView::~CPGStableLiftingView()
{
}

void CPGStableLiftingView::DoDataExchange(CDataExchange* pDX)
{
   CPGStableFormView::DoDataExchange(pDX);

   DDX_Control(pDX, IDC_EC, m_ctrlEc);
   DDX_Control(pDX, IDC_FC, m_ctrlFc);

   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();

   WBFL::Stability::LiftingStabilityProblem problem = pDoc->GetLiftingStabilityProblem();

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

   Float64 L;
   problem.GetSupportLocations(&L,&L);
   DDX_UnitValueAndTag(pDX,IDC_LIFT,IDC_LIFT_UNIT,L,pDispUnits->SpanLength);
   DDV_UnitValueZeroOrMore(pDX, IDC_LIFT_UNIT, L, pDispUnits->SpanLength);

   Float64 Fs,Fh,Ft;
   GetMaxFpe(&Fs,&Fh,&Ft);
   DDX_UnitValueAndTag(pDX,IDC_FPE_STRAIGHT,IDC_FPE_STRAIGHT_UNIT,Fs,pDispUnits->GeneralForce);
   DDX_UnitValueAndTag(pDX,IDC_FPE_HARPED,IDC_FPE_HARPED_UNIT,Fh,pDispUnits->GeneralForce);
   DDX_UnitValueAndTag(pDX,IDC_FPE_TEMP,IDC_FPE_TEMP_UNIT,Ft,pDispUnits->GeneralForce);

   Float64 sweepTolerance = problem.GetSweepTolerance();
   Float64 sweepGrowth = problem.GetSweepGrowth();
   Float64 supportPlacementTolerance = problem.GetSupportPlacementTolerance();

   if ( pApp->GetUnitsMode() == WBFL::EAF::UnitMode::SI )
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

   Float64 impactUp, impactDown;
   problem.GetImpact(&impactUp,&impactDown);
   DDX_Percentage(pDX,IDC_IMPACT_UP,impactUp);
   DDX_Percentage(pDX,IDC_IMPACT_DOWN,impactDown);

   Float64 liftAngle = problem.GetLiftAngle();
   DDX_UnitValueAndTag(pDX,IDC_LIFT_ANGLE,IDC_LIFT_ANGLE_UNIT,liftAngle,pDispUnits->Angle);

   Float64 Yra = problem.GetYRollAxis();
   DDX_UnitValueAndTag(pDX,IDC_YRA,IDC_YRA_UNIT,Yra,pDispUnits->ComponentDim);

   Float64 camber = problem.GetCamber();
   Float64 camberMultiplier = problem.GetCamberMultiplier();

   DDX_UnitValueAndTag(pDX,IDC_CAMBER,IDC_CAMBER_UNIT,camber,pDispUnits->ComponentDim);
   DDX_Text(pDX, IDC_CAMBER_MULTIPLIER, camberMultiplier);

   Float64 lateralCamber = problem.GetLateralCamber();
   DDX_UnitValueAndTag(pDX, IDC_LATERAL_CAMBER, IDC_LATERAL_CAMBER_UNIT, lateralCamber, pDispUnits->ComponentDim);


   Float64 fci, frCoefficient;
   bool bComputeEci;
   pDoc->GetLiftingMaterials(&fci,&bComputeEci,&frCoefficient);
   bComputeEci = !bComputeEci;
   Float64 Eci = problem.GetConcrete().GetE(); // this is the computed value
   DDX_UnitValueAndTag(pDX,IDC_FC,IDC_FC_UNIT,fci,pDispUnits->Stress);
   DDX_Check_Bool(pDX,IDC_COMPUTE_EC,bComputeEci);
   DDX_UnitValueAndTag(pDX,IDC_EC,IDC_EC_UNIT,Eci,pDispUnits->ModE);

   DDX_UnitValueAndTag(pDX,IDC_FR_COEFFICIENT,IDC_FR_COEFFICIENT_UNIT,frCoefficient,pDispUnits->SqrtPressure);
   CString tag;
   if ( WBFL::LRFD::BDSManager::GetEdition() < WBFL::LRFD::BDSManager::Edition::SeventhEditionWith2016Interims )
   {
      tag = pApp->GetUnitsMode() == WBFL::EAF::UnitMode::SI ? _T("sqrt(f'ci (MPa))") : _T("sqrt(f'ci (KSI))");
   }
   else
   {
      tag = pApp->GetUnitsMode() == WBFL::EAF::UnitMode::SI ? _T("(lambda)sqrt(f'ci (MPa))") : _T("(lambda)sqrt(f'ci (KSI))");
   }
   DDX_Text(pDX,IDC_FR_COEFFICIENT_UNIT,tag);

   DDX_Text(pDX,IDC_LIFTING_FS_CRACKING,m_LiftingCriteria.MinFScr);
   DDX_Text(pDX,IDC_LIFTING_FS_FAILURE,m_LiftingCriteria.MinFSf);
   
   DDX_Text(pDX, IDC_LIFTING_GLOBAL_COMPRESSION, m_LiftingCriteria.CompressionCoefficient_GlobalStress);
   DDX_Text(pDX, IDC_LIFTING_PEAK_COMPRESSION, m_LiftingCriteria.CompressionCoefficient_PeakStress);

   auto* pLiftingTensionStressLimit = dynamic_cast<WBFL::Stability::CCLiftingTensionStressLimit*>(m_LiftingCriteria.TensionStressLimit.get());

   DDX_UnitValue(pDX,IDC_LIFTING_TENSION, pLiftingTensionStressLimit->TensionCoefficient,pDispUnits->SqrtPressure);
   DDX_Text(pDX,IDC_LIFTING_TENSION_UNIT,tag);
   DDX_Check_Bool(pDX,IDC_CHECK_LIFTING_TENSION_MAX, pLiftingTensionStressLimit->bMaxTension);
   DDX_UnitValueAndTag(pDX,IDC_LIFTING_TENSION_MAX,IDC_LIFTING_TENSION_MAX_UNIT, pLiftingTensionStressLimit->MaxTension,pDispUnits->Stress);
   DDX_UnitValue(pDX,IDC_LIFTING_TENSION_WITH_REBAR, pLiftingTensionStressLimit->TensionCoefficientWithRebar,pDispUnits->SqrtPressure);
   DDX_Text(pDX,IDC_LIFTING_TENSION_WITH_REBAR_UNIT,tag);

   if ( pDX->m_bSaveAndValidate )
   {
      problem.GetConcrete().SetE(Eci);

      problem.SetSweepTolerance(sweepTolerance);
      problem.SetSweepGrowth(sweepGrowth);
      problem.SetSupportPlacementTolerance(supportPlacementTolerance);

      problem.SetSupportLocations(L,L);

      problem.SetImpact(impactUp,impactDown);

      problem.SetLiftAngle(liftAngle);

      problem.SetWindLoading(windLoadType,windLoad);

      problem.SetAppurtenanceLoading(eb, Wb);

      problem.SetYRollAxis(Yra);

      problem.SetCamber(camber);
      problem.SetCamberMultiplier(camberMultiplier);

      problem.SetLateralCamber(lateralCamber);
      problem.IncludeLateralRollAxisOffset(IsZero(lateralCamber) ? false : true);

      pDoc->SetLiftingStabilityProblem(problem);
      pDoc->SetLiftingMaterials(fci,!bComputeEci,frCoefficient);

      pDoc->SetLiftingCriteria(m_LiftingCriteria);

      auto strands = pDoc->GetStrands(pDoc->GetGirderType(), ModelType::Lifting);
      if (strands.strandMethod == CPGStableStrands::Simplified )
      {
         strands.FpeStraight = Fs;
         strands.FpeHarped   = Fh;
         strands.FpeTemp     = Ft;
      }
      pDoc->SetStrands(pDoc->GetGirderType(), ModelType::Lifting, strands);
   }
}

BEGIN_MESSAGE_MAP(CPGStableLiftingView, CPGStableFormView)
   ON_BN_CLICKED(IDC_COMPUTE_EC, &CPGStableLiftingView::OnUserEc)
	ON_EN_CHANGE(IDC_FC, &CPGStableLiftingView::OnChangeFc)
   ON_EN_CHANGE(IDC_FPE_STRAIGHT, &CPGStableLiftingView::OnChange)
   ON_EN_CHANGE(IDC_FPE_HARPED, &CPGStableLiftingView::OnChange)
   ON_EN_CHANGE(IDC_FPE_TEMP, &CPGStableLiftingView::OnChange)
   ON_EN_CHANGE(IDC_EC, &CPGStableLiftingView::OnChange)
   ON_EN_CHANGE(IDC_FR_COEFFICIENT, &CPGStableLiftingView::OnChange)
   ON_EN_CHANGE(IDC_LIFT, &CPGStableLiftingView::OnChange)
   ON_EN_CHANGE(IDC_IMPACT_UP, &CPGStableLiftingView::OnChange)
   ON_EN_CHANGE(IDC_IMPACT_DOWN, &CPGStableLiftingView::OnChange)
   ON_EN_CHANGE(IDC_LIFT_ANGLE, &CPGStableLiftingView::OnChange)
   ON_EN_CHANGE(IDC_WIND_PRESSURE, &CPGStableLiftingView::OnChange)
   ON_EN_CHANGE(IDC_BRACKET_WEIGHT, &CPGStableLiftingView::OnChange)
   ON_EN_CHANGE(IDC_BRACKET_ECCENTRICITY, &CPGStableLiftingView::OnChange)
   ON_EN_CHANGE(IDC_CAMBER, &CPGStableLiftingView::OnChange)
   ON_EN_CHANGE(IDC_CAMBER_MULTIPLIER, &CPGStableLiftingView::OnChange)
   ON_EN_CHANGE(IDC_YRA, &CPGStableLiftingView::OnChange)
   ON_EN_CHANGE(IDC_SWEEP_TOLERANCE, &CPGStableLiftingView::OnChange)
   ON_EN_CHANGE(IDC_SWEEP_GROWTH, &CPGStableLiftingView::OnChange)
   ON_EN_CHANGE(IDC_LATERAL_SWEEP_INCREMENT, &CPGStableLiftingView::OnChange)
   ON_EN_CHANGE(IDC_SUPPORT_PLACEMENT_TOLERANCE, &CPGStableLiftingView::OnChange)
   ON_EN_CHANGE(IDC_LATERAL_CAMBER, &CPGStableLiftingView::OnChange)
   ON_BN_CLICKED(IDC_EDIT_FPE, &CPGStableLiftingView::OnEditFpe)
   ON_COMMAND(ID_FILE_PRINT,&CPGStableLiftingView::OnPrint)
   ON_COMMAND(ID_FILE_PRINT_DIRECT,&CPGStableLiftingView::OnPrintDirect)
   ON_BN_CLICKED(IDC_CHECK_LIFTING_TENSION_MAX, &CPGStableLiftingView::OnClickedLiftingTensionMax)
   ON_WM_SIZE()
   ON_CBN_SELCHANGE(IDC_WIND_TYPE, &CPGStableLiftingView::OnWindTypeChanged)
	ON_MESSAGE(WM_HELP, OnCommandHelp)
   ON_COMMAND_RANGE(CCS_CMENU_BASE, CCS_CMENU_MAX, OnCmenuSelected)
   ON_EN_CHANGE(IDC_LIFTING_FS_CRACKING, &CPGStableLiftingView::OnChange)
   ON_EN_CHANGE(IDC_LIFTING_FS_FAILURE, &CPGStableLiftingView::OnChange)
   ON_EN_CHANGE(IDC_LIFTING_PEAK_COMPRESSION, &CPGStableLiftingView::OnChange)
   ON_EN_CHANGE(IDC_LIFTING_GLOBAL_COMPRESSION, &CPGStableLiftingView::OnChange)
   ON_EN_CHANGE(IDC_LIFTING_TENSION, &CPGStableLiftingView::OnChange)
   ON_EN_CHANGE(IDC_LIFTING_TENSION_MAX, &CPGStableLiftingView::OnChange)
   ON_EN_CHANGE(IDC_LIFTING_TENSION_WITH_REBAR, &CPGStableLiftingView::OnChange)
END_MESSAGE_MAP()


// CPGStableLiftingView diagnostics

#ifdef _DEBUG
void CPGStableLiftingView::AssertValid() const
{
   AFX_MANAGE_STATE(AfxGetAppModuleState());
	CPGStableFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPGStableLiftingView::Dump(CDumpContext& dc) const
{
	CPGStableFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPGStableLiftingView message handlers

BOOL CPGStableLiftingView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,	DWORD dwRequestedStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,CCreateContext* pContext)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return CPGStableFormView::Create(lpszClassName,lpszWindowName,dwRequestedStyle,rect,pParentWnd,nID,pContext);
}

void CPGStableLiftingView::OnActivateView()
{
   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();
   m_LiftingCriteria = pDoc->GetLiftingCriteria();

   UpdateData(FALSE);

   if ( m_strUserEc == _T("") )
   {
      m_ctrlEc.GetWindowText(m_strUserEc);
   }

   UpdateFpeControls();
   UpdateCriteriaControls();

   OnUserEc();

   RefreshReport();
}

void CPGStableLiftingView::OnDeactivateView()
{
   UpdateData();
}

void CPGStableLiftingView::OnUserEc()
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

void CPGStableLiftingView::UpdateEc()
{
   if ( IsDlgButtonChecked(IDC_COMPUTE_EC) )
   {
      return;
   }

    // need to manually parse strength
   CString strFc;
   m_ctrlFc.GetWindowText(strFc);

   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();
   CString strEc = pDoc->UpdateEc(strFc);
   m_ctrlEc.SetWindowText(strEc);
}

void CPGStableLiftingView::OnChangeFc() 
{
   UpdateEc();
   OnChange();
}

void CPGStableLiftingView::OnChange()
{
   UpdateData();
   RefreshReport();
}

void CPGStableLiftingView::UpdateFpeControls()
{
   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();
   const auto& strands = pDoc->GetStrands(pDoc->GetGirderType(), ModelType::Lifting);

   BOOL bEnable = (strands.strandMethod == CPGStableStrands::Simplified ? TRUE : FALSE);
   GetDlgItem(IDC_FPE_STRAIGHT)->EnableWindow(bEnable);
   GetDlgItem(IDC_FPE_HARPED)->EnableWindow(bEnable);
   GetDlgItem(IDC_FPE_TEMP)->EnableWindow(bEnable);

   GetDlgItem(IDC_EDIT_FPE)->ShowWindow(bEnable ? SW_HIDE : SW_SHOW);
}

void CPGStableLiftingView::UpdateCriteriaControls()
{
   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();
   BOOL bEnable = pDoc->GetCriteria() == gs_strCriteria ? TRUE : FALSE;
   GetDlgItem(IDC_IMPACT_UP)->EnableWindow(bEnable);
   GetDlgItem(IDC_IMPACT_DOWN)->EnableWindow(bEnable);
   GetDlgItem(IDC_LIFT_ANGLE)->EnableWindow(bEnable);
   GetDlgItem(IDC_WIND_TYPE)->EnableWindow(bEnable);
   GetDlgItem(IDC_WIND_PRESSURE)->EnableWindow(bEnable);
   GetDlgItem(IDC_CAMBER)->EnableWindow(TRUE); // if camber is a direct input value, always enable
   GetDlgItem(IDC_CAMBER_MULTIPLIER)->EnableWindow(bEnable);
   GetDlgItem(IDC_YRA)->EnableWindow(bEnable);
   GetDlgItem(IDC_SWEEP_TOLERANCE)->EnableWindow(bEnable);
   GetDlgItem(IDC_SUPPORT_PLACEMENT_TOLERANCE)->EnableWindow(bEnable);
   GetDlgItem(IDC_LIFTING_FS_CRACKING)->EnableWindow(bEnable);
   GetDlgItem(IDC_LIFTING_FS_FAILURE)->EnableWindow(bEnable);
   GetDlgItem(IDC_LIFTING_GLOBAL_COMPRESSION)->EnableWindow(bEnable);
   GetDlgItem(IDC_LIFTING_PEAK_COMPRESSION)->EnableWindow(bEnable);
   GetDlgItem(IDC_LIFTING_TENSION)->EnableWindow(bEnable);
   GetDlgItem(IDC_CHECK_LIFTING_TENSION_MAX)->EnableWindow(bEnable);
   GetDlgItem(IDC_LIFTING_TENSION_MAX)->EnableWindow(bEnable);
   GetDlgItem(IDC_LIFTING_TENSION_WITH_REBAR)->EnableWindow(bEnable);
}

void CPGStableLiftingView::OnEditFpe()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();
   CPGStableEffectivePrestressDlg dlg;
   dlg.m_Strands = pDoc->GetStrands(pDoc->GetGirderType(), ModelType::Lifting);
   if ( dlg.DoModal() == IDOK )
   {
      pDoc->SetStrands(pDoc->GetGirderType(), ModelType::Lifting,dlg.m_Strands);

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

void CPGStableLiftingView::OnInitialUpdate()
{
   // use exception safe older to prevent bad things to happen during initialization
   OnInitBoolHolder holder(m_bViewInitialized);

   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();

   CComboBox* pcbWindType = (CComboBox*)GetDlgItem(IDC_WIND_TYPE);
   pcbWindType->SetItemData(pcbWindType->AddString(_T("Wind Speed")),(DWORD_PTR)WBFL::Stability::WindLoadType::Speed);
   pcbWindType->SetItemData(pcbWindType->AddString(_T("Wind Pressure")),(DWORD_PTR)WBFL::Stability::WindLoadType::Pressure);

   CPGStableFormView::OnInitialUpdate();

   std::shared_ptr<WBFL::Reporting::ReportBuilder> pRptBuilder = pDoc->GetReportManager()->GetReportBuilder(_T("Lifting"));
   WBFL::Reporting::ReportDescription rptDesc = pRptBuilder->GetReportDescription();
   std::shared_ptr<WBFL::Reporting::ReportSpecificationBuilder> pRptSpecBuilder = pRptBuilder->GetReportSpecificationBuilder();
   m_pRptSpec = pRptSpecBuilder->CreateDefaultReportSpec(rptDesc);
   CWnd* pWnd = GetDlgItem(IDC_BROWSER);
   m_pBrowser = pDoc->GetReportManager()->CreateReportBrowser(pWnd->GetSafeHwnd(), WS_BORDER, m_pRptSpec, std::shared_ptr<const WBFL::Reporting::ReportSpecificationBuilder>());
   m_pBrowser->FitToParent();

   UpdateFpeControls();

   OnClickedLiftingTensionMax();
}

void CPGStableLiftingView::RefreshReport()
{
   if (!m_bViewInitialized)
   {
      return;
   }

   if ( m_pRptSpec == nullptr )
      return;

   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();

   // refresh the report
   m_pRptSpec = m_pBrowser->GetReportSpecification();
   std::shared_ptr<WBFL::Reporting::ReportBuilder> pBuilder = pDoc->GetReportManager()->GetReportBuilder( m_pRptSpec->GetReportName() );
   std::shared_ptr<rptReport> pReport = pBuilder->CreateReport( m_pRptSpec );
   m_pBrowser->UpdateReport( pReport, true );

}

BOOL CPGStableLiftingView::IsDataValid()
{
   try
   {
      CDataExchange DX(this,TRUE);
      DoDataExchange(&DX);
      return TRUE;
   }
   catch (...)
   {
      return FALSE;
   }
}

void CPGStableLiftingView::GetMaxFpe(Float64* pFpeStraight,Float64* pFpeHarped,Float64* pFpeTemp)
{
   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();
   const auto& strands = pDoc->GetStrands(pDoc->GetGirderType(), ModelType::Lifting);

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

void CPGStableLiftingView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* /*pHint*/)
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

void CPGStableLiftingView::OnClickedLiftingTensionMax()
{
   BOOL bEnable = IsDlgButtonChecked(IDC_CHECK_LIFTING_TENSION_MAX) == BST_CHECKED ? TRUE : FALSE;
   GetDlgItem(IDC_LIFTING_TENSION_MAX)->EnableWindow(bEnable);
   OnChange();
}

LRESULT CPGStableLiftingView::OnCommandHelp(WPARAM, LPARAM lParam)
{
   EAFHelp( EAFGetDocument()->GetDocumentationSetName(), IDH_PGSTABLE_LIFTING_VIEW );
   return TRUE;
}

void CPGStableLiftingView::OnCmenuSelected(UINT id)
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

void CPGStableLiftingView::OnSize(UINT nType, int cx, int cy)
{
   CPGStableFormView::OnSize(nType, cx, cy);

   if ( m_pBrowser )
   {
      // Convert a 7du x 7du rect into pixels
      CRect sizeRect(0, 0, 7, 7);
      MapDialogRect(GetSafeHwnd(), &sizeRect);

      CRect clientRect;
      GetClientRect(&clientRect);

      CWnd* pWnd = GetDlgItem(IDC_BROWSER);
      CRect placeholderRect;
      pWnd->GetWindowRect(&placeholderRect);
      ScreenToClient(&placeholderRect);

      CRect browserRect;
      browserRect.left = placeholderRect.left;
      browserRect.top = placeholderRect.top;
      browserRect.right = clientRect.right - sizeRect.Width();
      browserRect.bottom = clientRect.bottom - sizeRect.Height();
      pWnd->SetWindowPos(nullptr, browserRect.left, browserRect.top, browserRect.Width(), browserRect.Height(), SWP_NOZORDER | SWP_NOMOVE);

      m_pBrowser->FitToParent();
   }
}

void CPGStableLiftingView::OnPrint() 
{
   m_pBrowser->Print(true);
}

void CPGStableLiftingView::OnPrintDirect() 
{
   m_pBrowser->Print(false);
}

void CPGStableLiftingView::OnWindTypeChanged()
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
