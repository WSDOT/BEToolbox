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

// PGStableHaulingView.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PGStableHaulingView.h"
#include "PGStableDoc.h"
#include "PGStableEffectivePrestressDlg.h"
#include <MFCTools\MFCTools.h>


// CPGStableHaulingView

IMPLEMENT_DYNCREATE(CPGStableHaulingView, CFormView)

CPGStableHaulingView::CPGStableHaulingView()
	: CFormView(CPGStableHaulingView::IDD)
{
   m_FpeType = CONSTANT_FPE;
}

CPGStableHaulingView::~CPGStableHaulingView()
{
}

void CPGStableHaulingView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

   DDX_Control(pDX, IDC_EC, m_ctrlEc);
   DDX_Control(pDX, IDC_FC, m_ctrlFc);
   DDX_Control(pDX, IDC_K1, m_ctrlK1);
   DDX_Control(pDX, IDC_K2, m_ctrlK2);

   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();

   Float64 fc, frCoefficient;
   bool bComputeEc;
   pDoc->GetHaulingMaterials(&fc,&bComputeEc,&frCoefficient);
   bComputeEc = !bComputeEc;

   stbHaulingStabilityProblem problem = pDoc->GetHaulingStabilityProblem();

   Float64 Ll,Lr;
   problem.GetSupportLocations(&Ll,&Lr);

   DDX_Radio(pDX,IDC_CONSTANT_FPE,m_FpeType);

   Float64 Fs,Fh,Ft;
   GetMaxFpe(&Fs,&Fh,&Ft);
   DDX_UnitValueAndTag(pDX,IDC_FPE_STRAIGHT,IDC_FPE_STRAIGHT_UNIT,Fs,pDispUnits->GeneralForce);
   DDX_UnitValueAndTag(pDX,IDC_FPE_HARPED,IDC_FPE_HARPED_UNIT,Fh,pDispUnits->GeneralForce);
   DDX_UnitValueAndTag(pDX,IDC_FPE_TEMP,IDC_FPE_TEMP_UNIT,Ft,pDispUnits->GeneralForce);


   DDX_UnitValueAndTag(pDX,IDC_FC,IDC_FC_UNIT,fc,pDispUnits->Stress);
   Float64 Ec = problem.GetEc(); // this is the computed value
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

   bool bImpact = !problem.ApplyImpactToTiltedGirder();
   DDX_Check_Bool(pDX,IDC_RESTRICT_IMPACT,bImpact);

   Float64 windPressure = problem.GetWindPressure();

   Float64 velocity = problem.GetVelocity();
   Float64 radius = problem.GetTurningRadius();


   bool bDirectCamber;
   Float64 camber;
   Float64 camberOffset;
   problem.GetCamber(&bDirectCamber,&camber);
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

   DDX_UnitValueAndTag(pDX,IDC_VELOCITY,IDC_VELOCITY_UNIT,velocity,pDispUnits->Velocity);
   DDX_UnitValueAndTag(pDX,IDC_RADIUS,IDC_RADIUS_UNIT,radius,pDispUnits->AlignmentLength);

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
         GetDlgItem(IDC_SWEEP_TOLERANCE_LABEL)->SetWindowText(_T("Sweep Tolerance"));
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
         GetDlgItem(IDC_SWEEP_TOLERANCE_LABEL)->SetWindowText(_T("Sweep Tolerance 1/"));
         GetDlgItem(IDC_SWEEP_TOLERANCE_UNIT)->SetWindowText(_T("in/10 ft"));
      }
   }

   DDX_UnitValueAndTag(pDX,IDC_SUPPORT_PLACEMENT_TOLERANCE,IDC_SUPPORT_PLACEMENT_TOLERANCE_UNIT,supportPlacementTolerance,pDispUnits->ComponentDim);

   DDX_UnitValueAndTag(pDX,IDC_WIND_PRESSURE,IDC_WIND_PRESSURE_UNIT,windPressure,pDispUnits->WindPressure);

   Float64 Hgb = pDoc->GetHeightOfGirderBottomAboveRoadway();
   DDX_UnitValueAndTag(pDX,IDC_HGB,IDC_HGB_UNIT,Hgb,pDispUnits->ComponentDim);

   Float64 Hrc = problem.GetHeightOfRollAxisAboveRoadway();
   DDX_UnitValueAndTag(pDX,IDC_HRC,IDC_HRC_UNIT,Hrc,pDispUnits->ComponentDim);

   Float64 superelevation = problem.GetSuperelevation();
   DDX_Text(pDX,IDC_SUPERELEVATION,superelevation);

   Float64 Wcc = problem.GetWheelLineSpacing();
   DDX_UnitValueAndTag(pDX,IDC_WCC,IDC_WCC_UNIT,Wcc,pDispUnits->ComponentDim);

   Float64 Ktheta = problem.GetTruckRotationalStiffness();
   DDX_UnitValueAndTag(pDX,IDC_KTHETA,IDC_KTHETA_UNIT,Ktheta,pDispUnits->MomentPerAngle);

   if ( pDX->m_bSaveAndValidate )
   {
      pDoc->SetHaulingFpeType(m_FpeType);
      if ( m_FpeType == CONSTANT_FPE )
      {
         problem.ClearFpe();
         problem.AddFpe(0,Fs,Fh,Ft);
      }
      else
      {
         problem.ClearFpe();
         BOOST_FOREACH(stbFpe& fpe,m_Fpe)
         {
            problem.AddFpe(fpe.X,fpe.fpeStraight,fpe.fpeHarped,fpe.fpeTemporary);
         }
      }

      problem.SetEc(Ec);
      pDoc->SetK1(K1);
      pDoc->SetK2(K2);

      problem.SetSweepTolerance(sweepTolerance);
      problem.SetSupportPlacementTolerance(supportPlacementTolerance);

      problem.SetSupportLocations(Ll,Lr);

      problem.SetImpact(impactUp,impactDown);
      problem.ApplyImpactToTiltedGirder(!bImpact);

      problem.SetWindPressure(windPressure);
      problem.SetVelocity(velocity);
      problem.SetTurningRadius(radius);

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

      pDoc->SetHaulingStabilityProblem(problem);
      pDoc->SetHaulingMaterials(fc,!bComputeEc,frCoefficient);
   }
}

BEGIN_MESSAGE_MAP(CPGStableHaulingView, CFormView)
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
   ON_EN_CHANGE(IDC_SUPERELEVATION, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_WIND_PRESSURE, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_VELOCITY, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_RADIUS, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_CAMBER_OFFSET, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_CAMBER, &CPGStableHaulingView::OnChange)
   ON_BN_CLICKED(IDC_CAMBER1, &CPGStableHaulingView::OnChange)
   ON_BN_CLICKED(IDC_CAMBER2, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_HGB, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_HRC, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_KTHETA, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_WCC, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_SWEEP_TOLERANCE, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_LATERAL_SWEEP_INCREMENT, &CPGStableHaulingView::OnChange)
   ON_EN_CHANGE(IDC_SUPPORT_PLACEMENT_TOLERANCE, &CPGStableHaulingView::OnChange)
   ON_BN_CLICKED(IDC_RESTRICT_IMPACT, &CPGStableHaulingView::OnChange)
   ON_BN_CLICKED(IDC_EDIT_FPE, &CPGStableHaulingView::OnEditFpe)
   ON_BN_CLICKED(IDC_CONSTANT_FPE, &CPGStableHaulingView::OnFpeType)
   ON_BN_CLICKED(IDC_VARIABLE_FPE, &CPGStableHaulingView::OnFpeType)
END_MESSAGE_MAP()


// CPGStableHaulingView diagnostics

#ifdef _DEBUG
void CPGStableHaulingView::AssertValid() const
{
   AFX_MANAGE_STATE(AfxGetAppModuleState());
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPGStableHaulingView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPGStableHaulingView message handlers

BOOL CPGStableHaulingView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,	DWORD dwRequestedStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,CCreateContext* pContext)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return CFormView::Create(lpszClassName,lpszWindowName,dwRequestedStyle,rect,pParentWnd,nID,pContext);
}

void CPGStableHaulingView::OnActivateView(BOOL bActivate,CView* pActivateView,CView* pDeactivateView)
{
   CFormView::OnActivateView(bActivate,pActivateView,pDeactivateView);
   UpdateData(!bActivate);

   if ( m_strUserEc == _T("") )
   {
      m_ctrlEc.GetWindowText(m_strUserEc);
   }

   OnUserEc();
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

   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   const CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();
   const stbGirder& girder = pDoc->GetGirder(pDoc->GetGirderType());
   const stbHaulingStabilityProblem& problem = pDoc->GetHaulingStabilityProblem();

   stbHaulingCheckArtifact artifact = pDoc->GetHaulingCheckArtifact();
   const stbHaulingResults& results = artifact.GetHaulingResults();
   const stbCriteria& criteria = artifact.GetCriteria();

   stbHaulingSectionResult sectionResult = results.vSectionResults[results.FScrAnalysisPointIndex];

   CString strImpact[] = {_T("no impact"),_T("impact up"),_T("impact down")};
   CString strCorner[] = {_T("top left"),_T("top right"),_T("bottom left"),_T("bottom right")};
   CString strDir[] = {_T("left"),_T("right")};

   CString strMinDirectStress;
   Float64 X = problem.GetAnalysisPoints().at(results.MinDirectStressAnalysisPointIndex);
   strMinDirectStress.Format(_T("The minimum stress in plumb girder occurs at %s (%fL), %s flange tip with %s, wind toward the %s, and centrifugal force toward the %s.\r\nAllowable Stress = %s, Stress = %s, Status: %s"),
      ::FormatDimension(X,pDispUnits->SpanLength),
      X/girder.GetGirderLength(),
      strCorner[results.MinDirectStressCorner],
      strImpact[results.MinDirectStressImpactDirection],
      strDir[results.MinDirectStressWindDirection],
      strDir[results.MinDirectStressCFDirection],
      ::FormatDimension(criteria.AllowableCompression,pDispUnits->Stress),
      ::FormatDimension(results.MinDirectStress,pDispUnits->Stress),
      (artifact.PassedDirectCompressionCheck() ? _T("Passed") : _T("Failed"))
      );

   CString strMaxDirectStress;
   X = problem.GetAnalysisPoints().at(results.MaxDirectStressAnalysisPointIndex);
   strMaxDirectStress.Format(_T("The maximum stress in plumb girder occurs at %s (%fL), %s flange tip with %s, wind toward the %s, and centrifugal force toward the %s.\r\nAllowable Stress = %s, Stress = %s, Status: %s"),
      ::FormatDimension(X,pDispUnits->SpanLength),
      X/girder.GetGirderLength(),
      strCorner[results.MaxDirectStressCorner],
      strImpact[results.MaxDirectStressImpactDirection],
      strDir[results.MaxDirectStressWindDirection],
      strDir[results.MaxDirectStressCFDirection],
      ::FormatDimension(criteria.AllowableTension,pDispUnits->Stress),
      ::FormatDimension(results.MaxDirectStress,pDispUnits->Stress),
      (artifact.PassedDirectTensionCheck() ? _T("Passed") : _T("Failed"))
      );

   CString strMinStress;
   X = problem.GetAnalysisPoints().at(results.MinStressAnalysisPointIndex);
   strMinStress.Format(_T("The minimum stress in tilted girder occurs at %s (%fL), %s flange tip with %s, wind toward the %s, and centrifugal force toward the %s.\r\nAllowable Stress = %s, Stress = %s, Status: %s"),
      ::FormatDimension(X,pDispUnits->SpanLength),
      X/girder.GetGirderLength(),
      strCorner[results.MinStressCorner],
      strImpact[results.MinStressImpactDirection],
      strDir[results.MinStressWindDirection],
      strDir[results.MinStressCFDirection],
      ::FormatDimension(criteria.AllowableCompression,pDispUnits->Stress),
      ::FormatDimension(results.MinStress,pDispUnits->Stress),
      (artifact.PassedCompressionCheck() ? _T("Passed") : _T("Failed"))
      );

   CString strMaxStress;
   X = problem.GetAnalysisPoints().at(results.MaxStressAnalysisPointIndex);
   strMaxStress.Format(_T("The maximum stress tilted girder occurs at %s (%fL), %s flange tip with %s, wind toward the %s, and centrifugal force toward the %s.\r\nAllowable Stress = %s, Stress = %s, Status: %s"),
      ::FormatDimension(X,pDispUnits->SpanLength),
      X/girder.GetGirderLength(),
      strCorner[results.MaxStressCorner],
      strImpact[results.MaxStressImpactDirection],
      strDir[results.MaxStressWindDirection],
      strDir[results.MaxStressCFDirection],
      ::FormatDimension(criteria.AllowableTension,pDispUnits->Stress),
      ::FormatDimension(results.MaxStress,pDispUnits->Stress),
      (artifact.PassedTensionCheck() ? _T("Passed") : _T("Failed"))
      );

   X = problem.GetAnalysisPoints().at(results.FScrAnalysisPointIndex);
   CString strMinFScr;
   strMinFScr.Format(_T("The minimum factor of safety against cracking occurs at %s (%fL), %s flange tip with %s, wind toward the %s, and centrifugal force toward the %s.\r\nFScr = %s"),
      ::FormatDimension(X,pDispUnits->SpanLength),
      X/girder.GetGirderLength(),
      strCorner[sectionResult.CrackedFlange[results.FScrImpactDirection][results.FScrWindDirection][results.FScrCFDirection]],
      strImpact[results.FScrImpactDirection],
      strDir[results.FScrWindDirection],
      strDir[results.FScrCFDirection],
      ::FormatScalar(results.MinFScr,pDispUnits->Scalar));

   CString strMinFSf;
   strMinFSf.Format(_T("The minimum factor of safety against failure occurs with %s, wind toward the %s, and the centrifugal force toward the %s.\r\nFSf = %s"),
      strImpact[results.FSfImpactDirection],
      strDir[results.FSfWindDirection],
      strDir[results.FSfCFDirection],
      ::FormatScalar(results.MinFsFailure,pDispUnits->Scalar));

   CString strMinFSro;
   strMinFSro.Format(_T("The minimum factor of safety against roll over occurs with %s, wind toward the %s, and the centrifugal force toward the %s.\r\nFSro = %s"),
      strImpact[results.FSroImpactDirection],
      strDir[results.FSroWindDirection],
      strDir[results.FSroCFDirection],
      ::FormatScalar(results.MinFsRollover,pDispUnits->Scalar));

   CString strResults;
   strResults.Format(_T("%s\r\n\r\n%s\r\n\r\n%s\r\n\r\n%s\r\n\r\n%s\r\n\r\n%s\r\n\r\n%s"),strMinDirectStress,strMaxDirectStress,strMinStress,strMaxStress,strMinFScr,strMinFSf,strMinFSro);
   CWnd* pWnd = GetDlgItem(IDC_RESULTS);
   pWnd->SetWindowText(strResults);

   BuildStressGraph();
   BuildFSCrackingGraph();
}

void CPGStableHaulingView::BuildStressGraph()
{
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   const CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();
   const stbGirder& girder = pDoc->GetGirder(pDoc->GetGirderType());
   const stbLiftingStabilityProblem& problem = pDoc->GetLiftingStabilityProblem();

   stbHaulingCheckArtifact artifact = pDoc->GetHaulingCheckArtifact();
   const stbHaulingResults& results = artifact.GetHaulingResults();
   const stbCriteria& criteria = artifact.GetCriteria();

   CString strImpact[] = {_T("no impact"),_T("impact up"),_T("impact down")};
   CString strCorner[] = {_T("top left"),_T("top right"),_T("bottom left"),_T("bottom right")};
   CString strDir[] = {_T("left"),_T("right")};

   IndexType impact = results.MaxDirectStressImpactDirection;
   IndexType wind   = results.MaxDirectStressWindDirection;
   IndexType cf     = results.MaxDirectStressCFDirection;

   arvPhysicalConverter* pXFormat = new LengthTool(pDispUnits->SpanLength);
   arvPhysicalConverter* pYFormat = new StressTool(pDispUnits->Stress);
   m_ctrlStressGraph.SetAxisFormatters(pXFormat,pYFormat);

   grGraphXY& graph = m_ctrlStressGraph.GetGraph();
   graph.SetTitle(_T("Stress in Plumb Girder"));

   CString strSubtitle;
   strSubtitle.Format(_T("%s, wind %s, centrifugal force %s"),strImpact[impact],strDir[wind],strDir[cf]);
   graph.SetSubtitle(strSubtitle);
   graph.SetGridPenStyle(GRAPH_GRID_PEN_STYLE, GRAPH_GRID_PEN_WEIGHT, GRAPH_GRID_COLOR);
   graph.SetClientAreaColor(GRAPH_BACKGROUND);
   CString strXAxisTitle;
   strXAxisTitle.Format(_T("X (%s)"),((LengthTool*)pXFormat)->UnitTag().c_str());
   graph.SetXAxisTitle(strXAxisTitle);
   CString strYAxisTitle;
   strYAxisTitle.Format(_T("Stress (%s)"),((StressTool*)pYFormat)->UnitTag().c_str());
   graph.SetYAxisTitle(strYAxisTitle);
   //graph.SetIsotropicAxes();
   graph.ClearData();
   IndexType dataSeries[4] = {graph.CreateDataSeries(_T("Top Left"),    PS_SOLID,1,RED),
                              graph.CreateDataSeries(_T("Top Right"),   PS_SOLID,1,GREEN),
                              graph.CreateDataSeries(_T("Bottom Left"), PS_SOLID,1,BLUE),
                              graph.CreateDataSeries(_T("Bottom Right"),PS_SOLID,1,BLACK) };


   BOOST_FOREACH(const stbHaulingSectionResult& sectionResult,results.vSectionResults)
   {
      Float64 X = pXFormat->Convert(sectionResult.X);
      for ( IndexType corner = 0; corner < 4; corner++ )
      {
         Float64 f = pYFormat->Convert(sectionResult.fDirect[impact][wind][cf][corner]);
         gpPoint2d pnt(X,f);
         graph.AddPoint(dataSeries[corner],pnt);
      }
   }

   IndexType compressionLimit = graph.CreateDataSeries(_T("Allow. Compression"),PS_SOLID,2,RED);
   graph.AddPoint(compressionLimit,gpPoint2d(0.0,pYFormat->Convert(criteria.AllowableCompression)));
   graph.AddPoint(compressionLimit,gpPoint2d(pXFormat->Convert(girder.GetGirderLength()),pYFormat->Convert(criteria.AllowableCompression)));

   IndexType tensionLimit = graph.CreateDataSeries(_T("Allow. Tension"),PS_SOLID,2,BLUE);
   graph.AddPoint(tensionLimit,gpPoint2d(0.0,pYFormat->Convert(criteria.AllowableTension)));
   graph.AddPoint(tensionLimit,gpPoint2d(pXFormat->Convert(girder.GetGirderLength()),pYFormat->Convert(criteria.AllowableTension)));

   m_ctrlStressGraph.Invalidate();
   m_ctrlStressGraph.UpdateWindow();
}

void CPGStableHaulingView::BuildFSCrackingGraph()
{
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   const CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();
   const stbGirder& girder = pDoc->GetGirder(pDoc->GetGirderType());
   const stbHaulingStabilityProblem& problem = pDoc->GetHaulingStabilityProblem();

   stbHaulingCheckArtifact artifact = pDoc->GetHaulingCheckArtifact();
   const stbHaulingResults& results = artifact.GetHaulingResults();
   const stbCriteria& criteria = artifact.GetCriteria();

   CString strImpact[] = {_T("no impact"),_T("impact up"),_T("impact down")};
   CString strCorner[] = {_T("top left"),_T("top right"),_T("bottom left"),_T("bottom right")};
   CString strDir[] = {_T("left"),_T("right")};

   IndexType impact = results.FScrImpactDirection;
   IndexType wind   = results.FScrWindDirection;
   IndexType cf     = results.FScrCFDirection;

   arvPhysicalConverter* pXFormat = new LengthTool(pDispUnits->SpanLength);
   arvPhysicalConverter* pYFormat = new ScalarTool(pDispUnits->Scalar);
   m_ctrlFSGraph.SetAxisFormatters(pXFormat,pYFormat);

   grGraphXY& graph = m_ctrlFSGraph.GetGraph();
   graph.SetTitle(_T("Factor of Safety Against Cracking"));

   CString strSubtitle;
   strSubtitle.Format(_T("%s, wind %s, centrigual force %s"),strImpact[impact],strDir[wind],strDir[cf]);
   graph.SetSubtitle(strSubtitle);
   graph.SetGridPenStyle(GRAPH_GRID_PEN_STYLE, GRAPH_GRID_PEN_WEIGHT, GRAPH_GRID_COLOR);
   graph.SetClientAreaColor(GRAPH_BACKGROUND);
   CString strXAxisTitle;
   strXAxisTitle.Format(_T("X (%s)"),((LengthTool*)pXFormat)->UnitTag().c_str());
   graph.SetXAxisTitle(strXAxisTitle);
   CString strYAxisTitle;
   strYAxisTitle.Format(_T("%s"),_T("FS"));
   graph.SetYAxisTitle(strYAxisTitle);
   //graph.SetIsotropicAxes();
   graph.ClearData();
   IndexType dataSeries = graph.CreateDataSeries(_T("FS"),    PS_SOLID,1,BLUE);


   BOOST_FOREACH(const stbHaulingSectionResult& sectionResult,results.vSectionResults)
   {
      Float64 X = pXFormat->Convert(sectionResult.X);
      Float64 FS = sectionResult.FScr[impact][wind][cf];
      gpPoint2d pnt(X,FS);
      graph.AddPoint(dataSeries,pnt);
   }

   IndexType fsSeries = graph.CreateDataSeries(_T("Min. FS"),PS_SOLID,2,RED);
   graph.AddPoint(fsSeries,gpPoint2d(0.0,criteria.MinFScr));
   graph.AddPoint(fsSeries,gpPoint2d(pXFormat->Convert(girder.GetGirderLength()),criteria.MinFScr));

   m_ctrlFSGraph.Invalidate();
   m_ctrlFSGraph.UpdateWindow();
}

void CPGStableHaulingView::OnFpeType()
{
   m_FpeType = (GetCheckedRadioButton(IDC_CONSTANT_FPE,IDC_VARIABLE_FPE) == IDC_CONSTANT_FPE ? CONSTANT_FPE : VARYING_FPE);
   BOOL bEnable = (m_FpeType == CONSTANT_FPE ? TRUE : FALSE);
   GetDlgItem(IDC_FPE_STRAIGHT)->EnableWindow(bEnable);
   GetDlgItem(IDC_FPE_HARPED)->EnableWindow(bEnable);
   GetDlgItem(IDC_FPE_TEMP)->EnableWindow(bEnable);
   GetDlgItem(IDC_EDIT_FPE)->EnableWindow(!bEnable);
}

void CPGStableHaulingView::OnEditFpe()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CPGStableEffectivePrestressDlg dlg;
   dlg.m_Fpe = m_Fpe;
   if ( dlg.DoModal() == IDOK )
   {
      m_Fpe = dlg.m_Fpe;

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
   m_FpeType = pDoc->GetHaulingFpeType();

   {
      AFX_MANAGE_STATE(AfxGetStaticModuleState());
      m_ctrlStressGraph.SubclassDlgItem(IDC_STRESS_GRAPH,this);
      m_ctrlStressGraph.CustomInit();

      m_ctrlFSGraph.SubclassDlgItem(IDC_FS_GRAPH,this);
      m_ctrlFSGraph.CustomInit();
   }

   CFormView::OnInitialUpdate();

   stbHaulingStabilityProblem problem = pDoc->GetHaulingStabilityProblem();
   IndexType nFpe = problem.GetFpeCount();;
   for ( IndexType fpeIdx = 0; fpeIdx < nFpe; fpeIdx++ )
   {
      stbFpe fpe;
      problem.GetFpe(fpeIdx,&fpe.X,&fpe.fpeStraight,&fpe.fpeHarped,&fpe.fpeTemporary);
      m_Fpe.insert(fpe);
   }


   OnFpeType();
}

void CPGStableHaulingView::GetMaxFpe(Float64* pFpeStraight,Float64* pFpeHarped,Float64* pFpeTemp)
{
   Float64 FpeStraight = 0;
   Float64 FpeHarped = 0;
   Float64 FpeTemporary = 0;
   BOOST_FOREACH(stbFpe& fpe,m_Fpe)
   {
      FpeStraight  = Max(FpeStraight,fpe.fpeStraight);
      FpeHarped    = Max(FpeHarped,fpe.fpeHarped);
      FpeTemporary = Max(FpeTemporary,fpe.fpeTemporary);
   }

   *pFpeStraight = FpeStraight;
   *pFpeHarped   = FpeHarped;
   *pFpeTemp     = FpeTemporary;
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