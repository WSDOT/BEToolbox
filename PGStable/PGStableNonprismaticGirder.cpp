///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright � 1999-2025  Washington State Department of Transportation
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

// NonprismaticGirder.cpp : implementation file
//

#include "stdafx.h"
#include "..\resource.h"
#include "PGStableDoc.h"
#include "PGStableNonprismaticGirder.h"
#include "PGStableDDX.h"
#include "PGStableGirderControl.h"
#include <MFCTools\MFCTools.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void DDX_GirderSectionGrid(CDataExchange* pDX,CPGStableGirderSectionGrid* pGrid,WBFL::Stability::Girder& girder)
{
   if ( pDX->m_bSaveAndValidate )
   {
      pGrid->GetGirderSections(girder);
   }
   else
   {
      pGrid->FillGrid(girder);
   }
}

void DDX_Strands(CDataExchange* pDX,CPGStableStrands& strands)
{
   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   DDX_CBEnum(pDX,IDC_PS_METHOD,strands.strandMethod);

   DDX_UnitValueAndTag(pDX,IDC_XFER_LENGTH,IDC_XFER_LENGTH_UNIT,strands.XferLength,pDispUnits->ComponentDim);
   DDV_UnitValueZeroOrMore(pDX,IDC_XFER_LENGTH,strands.XferLength,pDispUnits->ComponentDim);

   DDX_UnitValueAndTag(pDX, IDC_EX, IDC_EX_UNIT, strands.ex, pDispUnits->ComponentDim);

   DDX_CBItemData(pDX,IDC_YS_MEASURE,strands.YsMeasure);
   DDX_UnitValueAndTag(pDX,IDC_YS,IDC_YS_UNIT,strands.Ys,pDispUnits->ComponentDim);
   DDV_UnitValueZeroOrMore(pDX,IDC_YS,strands.Ys,pDispUnits->ComponentDim);

   DDX_CBItemData(pDX,IDC_YT_MEASURE,strands.YtMeasure);
   DDX_UnitValueAndTag(pDX,IDC_YTEMP,IDC_YTEMP_UNIT,strands.Yt,pDispUnits->ComponentDim);
   DDV_UnitValueZeroOrMore(pDX,IDC_YTEMP,strands.Yt,pDispUnits->ComponentDim);

   DDX_CBItemData(pDX,IDC_XH1_UNIT,strands.Xh1Measure);
   if ( strands.Xh1Measure == DISTANCE )
   {
      DDX_UnitValue(pDX,IDC_XH1,strands.Xh1,pDispUnits->SpanLength);
   }
   else
   {
      DDX_Percentage(pDX,IDC_XH1,strands.Xh1);
      DDV_MinMaxDouble(pDX,100*strands.Xh1,0,100.0);
   }
   DDX_CBItemData(pDX,IDC_YH1_MEASURE,strands.Yh1Measure);
   DDX_UnitValueAndTag(pDX,IDC_YH1,IDC_YH1_UNIT,strands.Yh1,pDispUnits->ComponentDim);
   DDV_UnitValueZeroOrMore(pDX,IDC_YH1,strands.Yh1,pDispUnits->ComponentDim);

   DDX_CBItemData(pDX,IDC_XH2_UNIT,strands.Xh2Measure);
   if ( strands.Xh2Measure == DISTANCE )
   {
      DDX_UnitValue(pDX,IDC_XH2,strands.Xh2,pDispUnits->SpanLength);
   }
   else
   {
      DDX_Percentage(pDX,IDC_XH2,strands.Xh2);
      DDV_MinMaxDouble(pDX,100*strands.Xh2,0,100.0);
   }
   DDX_CBItemData(pDX,IDC_YH2_MEASURE,strands.Yh2Measure);
   DDX_UnitValueAndTag(pDX,IDC_YH2,IDC_YH2_UNIT,strands.Yh2,pDispUnits->ComponentDim);
   DDV_UnitValueZeroOrMore(pDX,IDC_YH2,strands.Yh2,pDispUnits->ComponentDim);

   DDX_CBItemData(pDX,IDC_XH3_UNIT,strands.Xh3Measure);
   if ( strands.Xh3Measure == DISTANCE )
   {
      DDX_UnitValue(pDX,IDC_XH3,strands.Xh3,pDispUnits->SpanLength);
   }
   else
   {
      DDX_Percentage(pDX,IDC_XH3,strands.Xh3);
      DDV_MinMaxDouble(pDX,100*strands.Xh3,0,100.0);
   }
   DDX_CBItemData(pDX,IDC_YH3_MEASURE,strands.Yh3Measure);
   DDX_UnitValueAndTag(pDX,IDC_YH3,IDC_YH3_UNIT,strands.Yh3,pDispUnits->ComponentDim);
   DDV_UnitValueZeroOrMore(pDX,IDC_YH3,strands.Yh3,pDispUnits->ComponentDim);

   DDX_CBItemData(pDX,IDC_XH4_UNIT,strands.Xh4Measure);
   if ( strands.Xh4Measure == DISTANCE )
   {
      DDX_UnitValue(pDX,IDC_XH4,strands.Xh4,pDispUnits->SpanLength);
   }
   else
   {
      DDX_Percentage(pDX,IDC_XH4,strands.Xh4);
      DDV_MinMaxDouble(pDX,100*strands.Xh4,0,100.0);
   }
   DDX_CBItemData(pDX,IDC_YH4_MEASURE,strands.Yh4Measure);
   DDX_UnitValueAndTag(pDX,IDC_YH4,IDC_YH4_UNIT,strands.Yh4,pDispUnits->ComponentDim);
   DDV_UnitValueZeroOrMore(pDX,IDC_YH4,strands.Yh4,pDispUnits->ComponentDim);
}

// CPGStableNonprismaticGirder dialog

IMPLEMENT_DYNAMIC(CPGStableNonprismaticGirder, CDialog)

CPGStableNonprismaticGirder::CPGStableNonprismaticGirder(CWnd* pParent /*=nullptr*/)
	: CDialog(CPGStableNonprismaticGirder::IDD, pParent)
{
   m_pPointLoadGrid = nullptr;
   m_pGirderSectionGrid = nullptr;
}

CPGStableNonprismaticGirder::~CPGStableNonprismaticGirder()
{
   delete m_pPointLoadGrid;
   delete m_pGirderSectionGrid;
}

void CPGStableNonprismaticGirder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

   DDX_Control(pDX,IDC_GIRDER,m_ctrlGirder);

   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   CView* pParent = (CView*)GetParent();
   CPGStableDoc* pDoc = (CPGStableDoc*)pParent->GetDocument();

   WBFL::Stability::Girder girder = pDoc->GetGirder(GirderType::Nonprismatic);
   CPGStableStrands strands = pDoc->GetStrands(GirderType::Nonprismatic, ModelType::Lifting);

   DDX_GirderSectionGrid(pDX,m_pGirderSectionGrid,girder);
   DDX_Strands(pDX,strands);

   int stressPointType = pDoc->GetStressPointType();
   DDX_Radio(pDX, IDC_COMPUTE_STRESS_POINTS, stressPointType);

   Float64 precamber = girder.GetPrecamber();
   DDX_UnitValueAndTag(pDX, IDC_PRECAMBER, IDC_PRECAMBER_UNIT, precamber, pDispUnits->ComponentDim);

   Float64 Cd = girder.GetDragCoefficient();
   DDX_Text(pDX,IDC_DRAG_COEFFICIENT,Cd);
   DDV_GreaterThanZero(pDX,IDC_DRAG_COEFFICIENT,Cd);

   Float64 densityWithRebar = pDoc->GetDensityWithRebar();
   Float64 density = pDoc->GetDensity();

   DDX_UnitValueAndTag(pDX,IDC_DENSITY,IDC_DENSITY_UNIT,density,pDispUnits->Density);
   DDV_UnitValueGreaterThanZero(pDX,IDC_DENSITY,density,pDispUnits->Density);
   DDX_UnitValueAndTag(pDX,IDC_DENSITY_WITH_REBAR,IDC_DENSITY_WITH_REBAR_UNIT,densityWithRebar,pDispUnits->Density);
   DDV_UnitValueGreaterThanZero(pDX,IDC_DENSITY_WITH_REBAR,densityWithRebar,pDispUnits->Density);

   auto concrete_type = pDoc->GetConcreteType();
   DDX_RadioEnum(pDX, IDC_NWC, concrete_type);

   Float64 K1 = pDoc->GetK1();
   Float64 K2 = pDoc->GetK2();
   DDX_Text(pDX, IDC_K1, K1);
   DDX_Text(pDX, IDC_K2, K2);

   std::vector<std::pair<Float64,Float64>> vLoads = girder.GetAdditionalLoads();
   DDX_PointLoadGrid(pDX,m_pPointLoadGrid,vLoads);

   if ( pDX->m_bSaveAndValidate )
   {
      girder.SetAdditionalLoads(vLoads);

      girder.SetDragCoefficient(Cd);

      girder.SetPrecamber(precamber);

      pDoc->SetStressPointType(stressPointType);
      if (stressPointType == DEFINE_STRESS_POINTS)
      {
         std::vector<StressPoints> vStressPoints = m_pGirderSectionGrid->GetStressPoints();
         IndexType sectIdx = 0;
         for (const auto& sp : vStressPoints)
         {
            girder.SetStressPoints(sectIdx, sp.pntTL[+WBFL::Stability::Section::Start], sp.pntTR[+WBFL::Stability::Section::Start], sp.pntBL[+WBFL::Stability::Section::Start], sp.pntBR[+WBFL::Stability::Section::Start], sp.pntTL[+WBFL::Stability::Section::End], sp.pntTR[+WBFL::Stability::Section::End], sp.pntBL[+WBFL::Stability::Section::End], sp.pntBR[+WBFL::Stability::Section::End]);
            sectIdx++;
         }
      }

      pDoc->SetGirder(GirderType::Nonprismatic,girder);

      pDoc->SetDensity(density);
      pDoc->SetDensityWithRebar(densityWithRebar);

      pDoc->SetConcreteType(concrete_type);

      pDoc->SetK1(K1);
      pDoc->SetK2(K2);


      pDoc->SetStrands(GirderType::Nonprismatic, ModelType::Lifting,strands);

      CPGStableStrands haulingStrands = pDoc->GetStrands(GirderType::Nonprismatic, ModelType::Hauling);
      haulingStrands.strandMethod = strands.strandMethod;
      if ( strands.strandMethod == CPGStableStrands::Simplified )
      {
         // we were operating on the lifting prestress information
         // since we are using the simplified method the geometry
         // of the hauling prestress is the same as the lifting prestress
         // update the hauling prestress information here
         haulingStrands.XferLength = strands.XferLength;
         haulingStrands.Xh1 = strands.Xh1;
         haulingStrands.Xh1Measure = strands.Xh1Measure;
         haulingStrands.Xh2 = strands.Xh2;
         haulingStrands.Xh2Measure = strands.Xh2Measure;
         haulingStrands.Xh3 = strands.Xh3;
         haulingStrands.Xh3Measure = strands.Xh3Measure;
         haulingStrands.Xh4 = strands.Xh4;
         haulingStrands.Xh4Measure = strands.Xh4Measure;
         haulingStrands.Ys = strands.Ys;
         haulingStrands.YsMeasure = strands.YsMeasure;
         haulingStrands.Yt = strands.Yt;
         haulingStrands.YtMeasure = strands.YtMeasure;
      }
      pDoc->SetStrands(GirderType::Nonprismatic, ModelType::Hauling,haulingStrands);
      pDoc->SetStrands(GirderType::Nonprismatic, ModelType::OneEndSeated, haulingStrands); // assume strands are the same for this condition
   }

   if ( !pDX->m_bSaveAndValidate )
   {
      OnChange();
   }
}


BEGIN_MESSAGE_MAP(CPGStableNonprismaticGirder, CDialog)
   ON_BN_CLICKED(IDC_ADD_SECTION, &CPGStableNonprismaticGirder::OnAddGirderSection)
   ON_BN_CLICKED(IDC_REMOVE_SECTION, &CPGStableNonprismaticGirder::OnRemoveGirderSection)
   ON_UPDATE_COMMAND_UI(IDC_REMOVE_SECTION,&CPGStableNonprismaticGirder::OnUpdateRemoveGirderSection)

   ON_BN_CLICKED(IDC_ADD_LOAD, &CPGStableNonprismaticGirder::OnAddPointLoad)
   ON_BN_CLICKED(IDC_REMOVE_LOAD, &CPGStableNonprismaticGirder::OnRemovePointLoad)
   ON_UPDATE_COMMAND_UI(IDC_REMOVE_LOAD,&CPGStableNonprismaticGirder::OnUpdateRemovePointLoad)
   ON_EN_CHANGE(IDC_YS, &CPGStableNonprismaticGirder::OnChange)
   ON_EN_CHANGE(IDC_XH1, &CPGStableNonprismaticGirder::OnChange)
   ON_EN_CHANGE(IDC_XH2, &CPGStableNonprismaticGirder::OnChange)
   ON_EN_CHANGE(IDC_XH3, &CPGStableNonprismaticGirder::OnChange)
   ON_EN_CHANGE(IDC_XH4, &CPGStableNonprismaticGirder::OnChange)
   ON_EN_CHANGE(IDC_YH1, &CPGStableNonprismaticGirder::OnChange)
   ON_EN_CHANGE(IDC_YH2, &CPGStableNonprismaticGirder::OnChange)
   ON_EN_CHANGE(IDC_YH3, &CPGStableNonprismaticGirder::OnChange)
   ON_EN_CHANGE(IDC_YH4, &CPGStableNonprismaticGirder::OnChange)
   ON_EN_CHANGE(IDC_YTEMP, &CPGStableNonprismaticGirder::OnChange)
   ON_CBN_SELCHANGE(IDC_YS_MEASURE, &CPGStableNonprismaticGirder::OnChange)
   ON_CBN_SELCHANGE(IDC_XH1_UNIT, &CPGStableNonprismaticGirder::OnChange)
   ON_CBN_SELCHANGE(IDC_XH2_UNIT, &CPGStableNonprismaticGirder::OnChange)
   ON_CBN_SELCHANGE(IDC_XH3_UNIT, &CPGStableNonprismaticGirder::OnChange)
   ON_CBN_SELCHANGE(IDC_XH4_UNIT, &CPGStableNonprismaticGirder::OnChange)
   ON_CBN_SELCHANGE(IDC_YH1_MEASURE, &CPGStableNonprismaticGirder::OnChange)
   ON_CBN_SELCHANGE(IDC_YH2_MEASURE, &CPGStableNonprismaticGirder::OnChange)
   ON_CBN_SELCHANGE(IDC_YH3_MEASURE, &CPGStableNonprismaticGirder::OnChange)
   ON_CBN_SELCHANGE(IDC_YH4_MEASURE, &CPGStableNonprismaticGirder::OnChange)
   ON_CBN_SELCHANGE(IDC_YT_MEASURE, CPGStableNonprismaticGirder::OnChange)
   ON_CBN_SELCHANGE(IDC_PS_METHOD, &CPGStableNonprismaticGirder::OnPSMethodChanged)
	ON_MESSAGE(WM_HELP, OnCommandHelp)
   ON_BN_CLICKED(IDC_COMPUTE_STRESS_POINTS, &CPGStableNonprismaticGirder::OnBnClickedComputeStressPoints)
   ON_BN_CLICKED(IDC_DEFINE_STRESS_POINTS, &CPGStableNonprismaticGirder::OnBnClickedDefineStressPoints)
   ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

LRESULT CPGStableNonprismaticGirder::OnCommandHelp(WPARAM, LPARAM lParam)
{
   EAFHelp( EAFGetDocument()->GetDocumentationSetName(), IDH_PGSTABLE_NONPRISMATIC_GIRDER_VIEW );
   return TRUE;
}


BOOL CPGStableNonprismaticGirder::OnInitDialog()
{
   {
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   m_pPointLoadGrid = new CPGStablePointLoadGrid;
	m_pPointLoadGrid->SubclassDlgItem(IDC_POINT_LOAD_GRID, this);
   m_pPointLoadGrid->CustomInit();

   m_pGirderSectionGrid = new CPGStableGirderSectionGrid;
	m_pGirderSectionGrid->SubclassDlgItem(IDC_GIRDER_SECTION_GRID, this);
   m_pGirderSectionGrid->CustomInit();

   m_ctrlGirder.SubclassDlgItem(IDC_GIRDER,this);
   m_ctrlGirder.CustomInit();
   }

   FillComboBoxes(0,IDC_YS_MEASURE);
   FillComboBoxes(0,IDC_YT_MEASURE);
   FillComboBoxes(IDC_XH1_UNIT,IDC_YH1_MEASURE);
   FillComboBoxes(IDC_XH2_UNIT,IDC_YH2_MEASURE);
   FillComboBoxes(IDC_XH3_UNIT,IDC_YH3_MEASURE);
   FillComboBoxes(IDC_XH4_UNIT,IDC_YH4_MEASURE);

   CComboBox* pcbMethod = (CComboBox*)GetDlgItem(IDC_PS_METHOD);
   pcbMethod->SetItemData(pcbMethod->AddString(_T("Simplified")),(DWORD_PTR)CPGStableStrands::Simplified);
   pcbMethod->SetItemData(pcbMethod->AddString(_T("Detailed")),(DWORD_PTR)CPGStableStrands::Detailed);

   CView* pParent = (CView*)GetParent();
   CPGStableDoc* pDoc = (CPGStableDoc*)pParent->GetDocument();
   const WBFL::Stability::Girder& girder = pDoc->GetGirder(GirderType::Nonprismatic);
   InitStressPointCache(girder);

   CDialog::OnInitDialog();

   OnPSMethodChanged();

   m_pGirderSectionGrid->EnableStressPoints(pDoc->GetStressPointType() == DEFINE_STRESS_POINTS); // enable grid if user defined stress points
                                                                                                 
   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}

void CPGStableNonprismaticGirder::FillComboBoxes(INT xIDC,INT yIDC)
{
   if ( xIDC != 0 )
   {
      CEAFApp* pApp = EAFGetApp();
      const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

      CComboBox* pX = (CComboBox*)GetDlgItem(xIDC);
      int idx = pX->AddString(pDispUnits->SpanLength.UnitOfMeasure.UnitTag().c_str());
      pX->SetItemData(idx,DISTANCE);
      idx = pX->AddString(_T("% L"));
      pX->SetItemData(idx,FRACTION);
   }

   CComboBox* pY = (CComboBox*)GetDlgItem(yIDC);
   int idx = pY->AddString(_T("Top"));
   pY->SetItemData(idx,TOP);
   idx = pY->AddString(_T("Bottom"));
   pY->SetItemData(idx,BOTTOM);
}

// CPGStableNonprismaticGirder message handlers

void CPGStableNonprismaticGirder::OnAddGirderSection()
{
   m_pGirderSectionGrid->AddGirderSection();
}

void CPGStableNonprismaticGirder::OnRemoveGirderSection()
{
   m_pGirderSectionGrid->RemoveSelectedGirderSections();
}

void CPGStableNonprismaticGirder::OnUpdateRemoveGirderSection(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(AreGirderSectionsSelected() ? TRUE : FALSE);
}

BOOL CPGStableNonprismaticGirder::AreGirderSectionsSelected()
{
   if ( m_pGirderSectionGrid )
   {
      return m_pGirderSectionGrid->AreGirderSectionsSelected();
   }
   else
   {
      return FALSE;
   }
}


void CPGStableNonprismaticGirder::OnAddPointLoad()
{
   m_pPointLoadGrid->AddLoad();
}

void CPGStableNonprismaticGirder::OnRemovePointLoad()
{
   m_pPointLoadGrid->RemoveSelectedLoads();
}

void CPGStableNonprismaticGirder::OnUpdateRemovePointLoad(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(AreLoadsSelected() ? TRUE : FALSE);
}

BOOL CPGStableNonprismaticGirder::AreLoadsSelected()
{
   if ( m_pPointLoadGrid )
   {
      return m_pPointLoadGrid->AreLoadsSelected();
   }
   else
   {
      return FALSE;
   }
}

void CPGStableNonprismaticGirder::SetGirderLength(Float64 Lg)
{
   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();
   CString str;
   str.Format(_T("Girder Length: %s"),::FormatDimension(Lg,pDispUnits->SpanLength));
   GetDlgItem(IDC_L)->SetWindowText(str);
}

void CPGStableNonprismaticGirder::OnChange()
{
   m_ctrlGirder.Invalidate();
   m_ctrlGirder.UpdateWindow();
}

std::vector<std::pair<Float64,Float64>> CPGStableNonprismaticGirder::GetGirderProfile()
{
   return m_pGirderSectionGrid->GetGirderProfile();
}

void CPGStableNonprismaticGirder::GetStrandProfiles(std::vector<std::pair<Float64,Float64>>* pvStraight,std::vector<std::pair<Float64,Float64>>* pvHarped,std::vector<std::pair<Float64,Float64>>* pvTemp)
{
   std::vector<std::pair<Float64,Float64>> vProfile;

   CDataExchange dx(this,TRUE);
   WBFL::Stability::Girder girder;
   DDX_GirderSectionGrid(&dx,m_pGirderSectionGrid,girder);

   CView* pParent = (CView*)GetParent();
   CPGStableDoc* pDoc = (CPGStableDoc*)pParent->GetDocument();

   CPGStableStrands strands = pDoc->GetStrands(GirderType::Nonprismatic, ModelType::Lifting);
   DDX_Strands(&dx,strands);

   pDoc->GetStrandProfiles(strands,girder,pvStraight,pvHarped,pvTemp);
}

void CPGStableNonprismaticGirder::OnPSMethodChanged()
{
   // TODO: Add your control notification handler code here
   CComboBox* pcbMethod = (CComboBox*)GetDlgItem(IDC_PS_METHOD);
   int curSel = pcbMethod->GetCurSel();
   CPGStableStrands::StrandMethod method = (CPGStableStrands::StrandMethod)(pcbMethod->GetItemData(curSel));
   int show = (method == CPGStableStrands::Simplified ? SW_SHOW : SW_HIDE);

   CString strFpeNote;
   if ( method == CPGStableStrands::Simplified )
   {
      strFpeNote = _T("NOTE: Effective prestress force (Fpe) is defined on the Lifting and Hauling tabs");
   }
   else
   {
      strFpeNote = _T("NOTE: Magnitude and location of the effective prestress force (Fpe) is defined on the Lifting and Hauling tabs");
   }
   GetDlgItem(IDC_FPE_NOTE)->SetWindowText(strFpeNote);

   GetDlgItem(IDC_XFER_LENGTH_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_XFER_LENGTH)->ShowWindow(show);
   GetDlgItem(IDC_XFER_LENGTH_UNIT)->ShowWindow(show);

   //GetDlgItem(IDC_GIRDER)->ShowWindow(show);

   GetDlgItem(IDC_YS_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_YS)->ShowWindow(show);
   GetDlgItem(IDC_YS_UNIT)->ShowWindow(show);
   GetDlgItem(IDC_YS_MEASURE_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_YS_MEASURE)->ShowWindow(show);

   GetDlgItem(IDC_XH1_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_XH1)->ShowWindow(show);
   GetDlgItem(IDC_XH1_UNIT)->ShowWindow(show);

   GetDlgItem(IDC_YH1_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_YH1)->ShowWindow(show);
   GetDlgItem(IDC_YH1_UNIT)->ShowWindow(show);
   GetDlgItem(IDC_YH1_MEASURE)->ShowWindow(show);

   GetDlgItem(IDC_XH2_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_XH2)->ShowWindow(show);
   GetDlgItem(IDC_XH2_UNIT)->ShowWindow(show);

   GetDlgItem(IDC_YH2_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_YH2)->ShowWindow(show);
   GetDlgItem(IDC_YH2_UNIT)->ShowWindow(show);
   GetDlgItem(IDC_YH2_MEASURE)->ShowWindow(show);

   GetDlgItem(IDC_XH3_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_XH3)->ShowWindow(show);
   GetDlgItem(IDC_XH3_UNIT)->ShowWindow(show);

   GetDlgItem(IDC_YH3_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_YH3)->ShowWindow(show);
   GetDlgItem(IDC_YH3_UNIT)->ShowWindow(show);
   GetDlgItem(IDC_YH3_MEASURE)->ShowWindow(show);

   GetDlgItem(IDC_XH4_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_XH4)->ShowWindow(show);
   GetDlgItem(IDC_XH4_UNIT)->ShowWindow(show);

   GetDlgItem(IDC_YH4_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_YH4)->ShowWindow(show);
   GetDlgItem(IDC_YH4_UNIT)->ShowWindow(show);
   GetDlgItem(IDC_YH4_MEASURE)->ShowWindow(show);

   GetDlgItem(IDC_FACE_LABEL)->ShowWindow(show);

   GetDlgItem(IDC_YTEMP_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_YTEMP)->ShowWindow(show);
   GetDlgItem(IDC_YTEMP_UNIT)->ShowWindow(show);
   GetDlgItem(IDC_YT_MEASURE_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_YT_MEASURE)->ShowWindow(show);

   GetDlgItem(IDC_SS_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_HS_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_TS_LABEL)->ShowWindow(show);

   GetDlgItem(IDC_EX_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_EX)->ShowWindow(show);
   GetDlgItem(IDC_EX_UNIT)->ShowWindow(show);
}

void CPGStableNonprismaticGirder::InitStressPointCache(const WBFL::Stability::Girder& girder)
{
   m_StressPointCache.clear();
   IndexType nSections = girder.GetSectionCount();
   for (IndexType sectIdx = 0; sectIdx < nSections; sectIdx++)
   {
      StressPoints sp;
      girder.GetStressPoints(sectIdx, WBFL::Stability::Section::Start, &sp.pntTL[+WBFL::Stability::Section::Start], &sp.pntTR[+WBFL::Stability::Section::Start], &sp.pntBL[+WBFL::Stability::Section::Start], &sp.pntBR[+WBFL::Stability::Section::Start]);
      girder.GetStressPoints(sectIdx, WBFL::Stability::Section::End,   &sp.pntTL[+WBFL::Stability::Section::End],   &sp.pntTR[+WBFL::Stability::Section::End],   &sp.pntBL[+WBFL::Stability::Section::End],   &sp.pntBR[+WBFL::Stability::Section::End]);
      m_StressPointCache.push_back(sp);
   }
}

void CPGStableNonprismaticGirder::OnBnClickedComputeStressPoints()
{
   m_StressPointCache = m_pGirderSectionGrid->GetStressPoints(); // get the user defined values and cache them
   m_pGirderSectionGrid->ComputeStressPoints();
   m_pGirderSectionGrid->EnableStressPoints(FALSE);
}

void CPGStableNonprismaticGirder::OnBnClickedDefineStressPoints()
{
   m_pGirderSectionGrid->EnableStressPoints(TRUE);
   m_pGirderSectionGrid->SetStressPoints(m_StressPointCache); // restore the cached user defined values
}


void CPGStableNonprismaticGirder::OnShowWindow(BOOL bShow, UINT nStatus)
{
   __super::OnShowWindow(bShow, nStatus);

   if (bShow)
   {
      CView* pParent = (CView*)GetParent();
      CPGStableDoc* pDoc = (CPGStableDoc*)pParent->GetDocument();
      m_pGirderSectionGrid->EnableStressPoints(pDoc->GetStressPointType() == DEFINE_STRESS_POINTS); // enable grid if user defined stress points
   }
}


void CPGStableNonprismaticGirder::OnCancel()
{
   // prevent [ESC] from closing window
   //__super::OnCancel();
}


void CPGStableNonprismaticGirder::OnOK()
{
   // prevent [Enter] from closing window
   //__super::OnOK();
}

void CPGStableNonprismaticGirder::OnUnitsChanged()
{
   UpdateData(FALSE);
   m_pGirderSectionGrid->OnUnitsChanged();
}
