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

// NonprismaticGirder.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PGStableNonprismaticGirder.h"
#include "PGStableDoc.h"
#include "PGStableDDX.h"
#include "PGStableGirderControl.h"
#include <MFCTools\MFCTools.h>


void DDX_GirderSectionGrid(CDataExchange* pDX,CPGStableGirderSectionGrid* pGrid,stbGirder& girder)
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
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   DDX_UnitValueAndTag(pDX,IDC_XFER_LENGTH,IDC_XFER_LENGTH_UNIT,strands.XferLength,pDispUnits->ComponentDim);

   DDX_CBItemData(pDX,IDC_YS_MEASURE,strands.YsMeasure);
   DDX_UnitValueAndTag(pDX,IDC_YS,IDC_YS_UNIT,strands.Ys,pDispUnits->ComponentDim);

   DDX_CBItemData(pDX,IDC_YT_MEASURE,strands.YtMeasure);
   DDX_UnitValueAndTag(pDX,IDC_YTEMP,IDC_YTEMP_UNIT,strands.Yt,pDispUnits->ComponentDim);

   DDX_CBItemData(pDX,IDC_XH1_UNIT,strands.Xh1Measure);
   if ( strands.Xh1Measure == DISTANCE )
   {
      DDX_UnitValue(pDX,IDC_XH1,strands.Xh1,pDispUnits->SpanLength);
   }
   else
   {
      DDX_Percentage(pDX,IDC_XH1,strands.Xh1);
   }
   DDX_CBItemData(pDX,IDC_YH1_MEASURE,strands.Yh1Measure);
   DDX_UnitValueAndTag(pDX,IDC_YH1,IDC_YH1_UNIT,strands.Yh1,pDispUnits->ComponentDim);

   DDX_CBItemData(pDX,IDC_XH2_UNIT,strands.Xh2Measure);
   if ( strands.Xh2Measure == DISTANCE )
   {
      DDX_UnitValue(pDX,IDC_XH2,strands.Xh2,pDispUnits->SpanLength);
   }
   else
   {
      DDX_Percentage(pDX,IDC_XH2,strands.Xh2);
   }
   DDX_CBItemData(pDX,IDC_YH2_MEASURE,strands.Yh2Measure);
   DDX_UnitValueAndTag(pDX,IDC_YH2,IDC_YH2_UNIT,strands.Yh2,pDispUnits->ComponentDim);

   DDX_CBItemData(pDX,IDC_XH3_UNIT,strands.Xh3Measure);
   if ( strands.Xh3Measure == DISTANCE )
   {
      DDX_UnitValue(pDX,IDC_XH3,strands.Xh3,pDispUnits->SpanLength);
   }
   else
   {
      DDX_Percentage(pDX,IDC_XH3,strands.Xh3);
   }
   DDX_CBItemData(pDX,IDC_YH3_MEASURE,strands.Yh3Measure);
   DDX_UnitValueAndTag(pDX,IDC_YH3,IDC_YH3_UNIT,strands.Yh3,pDispUnits->ComponentDim);

   DDX_CBItemData(pDX,IDC_XH4_UNIT,strands.Xh4Measure);
   if ( strands.Xh4Measure == DISTANCE )
   {
      DDX_UnitValue(pDX,IDC_XH4,strands.Xh4,pDispUnits->SpanLength);
   }
   else
   {
      DDX_Percentage(pDX,IDC_XH4,strands.Xh4);
   }
   DDX_CBItemData(pDX,IDC_YH4_MEASURE,strands.Yh4Measure);
   DDX_UnitValueAndTag(pDX,IDC_YH4,IDC_YH4_UNIT,strands.Yh4,pDispUnits->ComponentDim);
}

// CPGStableNonprismaticGirder dialog

IMPLEMENT_DYNAMIC(CPGStableNonprismaticGirder, CDialog)

CPGStableNonprismaticGirder::CPGStableNonprismaticGirder(CWnd* pParent /*=NULL*/)
	: CDialog(CPGStableNonprismaticGirder::IDD, pParent)
{
   m_pPointLoadGrid = NULL;
   m_pGirderSectionGrid = NULL;
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
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   CView* pParent = (CView*)GetParent();
   CPGStableDoc* pDoc = (CPGStableDoc*)pParent->GetDocument();

   stbGirder girder = pDoc->GetGirder(NONPRISMATIC);
   DDX_GirderSectionGrid(pDX,m_pGirderSectionGrid,girder);

   CPGStableStrands strands = pDoc->GetStrands(NONPRISMATIC);
   DDX_Strands(pDX,strands);

   Float64 densityWithRebar = girder.GetDensity();
   Float64 density = pDoc->GetDensity();

   DDX_UnitValueAndTag(pDX,IDC_DENSITY,IDC_DENSITY_UNIT,density,pDispUnits->Density);
   DDX_UnitValueAndTag(pDX,IDC_DENSITY_WITH_REBAR,IDC_DENSITY_WITH_REBAR_UNIT,densityWithRebar,pDispUnits->Density);

   std::vector<std::pair<Float64,Float64>> vLoads = girder.GetAdditionalLoads();
   DDX_PointLoadGrid(pDX,m_pPointLoadGrid,vLoads);

   if ( pDX->m_bSaveAndValidate )
   {
#pragma Reminder("WORKING HERE - add validation")
      // most values cannot be zero
      // check vLoads... X must be between 0 and L
      // harp point must be between 0 and 0.5

      girder.SetDensity(densityWithRebar);

      girder.SetAdditionalLoads(vLoads);

      pDoc->SetGirder(NONPRISMATIC,girder);
      pDoc->SetStrands(NONPRISMATIC,strands);
      pDoc->SetDensity(density);
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
   ON_CBN_SELCHANGE(IDC_YT_MEASURE, &CPGStableNonprismaticGirder::OnChange)
END_MESSAGE_MAP()


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

   CDialog::OnInitDialog();

   // TODO:  Add extra initialization here

   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}

void CPGStableNonprismaticGirder::FillComboBoxes(INT xIDC,INT yIDC)
{
   if ( xIDC != 0 )
   {
      CEAFApp* pApp = EAFGetApp();
      const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

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
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();
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

std::vector<std::pair<Float64,Float64>> CPGStableNonprismaticGirder::GetStrandProfile(int strandType)
{
   std::vector<std::pair<Float64,Float64>> vProfile;

   CDataExchange dx(this,TRUE);
   stbGirder girder;
   DDX_GirderSectionGrid(&dx,m_pGirderSectionGrid,girder);

   CPGStableStrands strands;
   DDX_Strands(&dx,strands);

   CView* pParent = (CView*)GetParent();
   CPGStableDoc* pDoc = (CPGStableDoc*)pParent->GetDocument();

   pDoc->ResolveStrandLocations(&strands,&girder);

   if ( strandType == STRAIGHT )
   {
      Float64 Lg = m_pGirderSectionGrid->GetGirderLength();
      Float64 Ys = girder.GetStraightStrandLocation();
      vProfile.push_back(std::make_pair(0.0,Ys));
      vProfile.push_back(std::make_pair(Lg,Ys));
   }
   else if ( strandType == TEMPORARY )
   {
      Float64 Lg = m_pGirderSectionGrid->GetGirderLength();
      Float64 Yt = girder.GetTemporaryStrandLocation();
      vProfile.push_back(std::make_pair(0.0,Yt));
      vProfile.push_back(std::make_pair(Lg,Yt));
   }
   else
   {
      ATLASSERT(strandType == HARPED);
      Float64 Lg = m_pGirderSectionGrid->GetGirderLength();
      Float64 Xh1,Yh1,Xh2,Yh2,Xh3,Yh3,Xh4,Yh4;
      girder.GetHarpedStrandLocation(&Xh1,&Yh1,&Xh2,&Yh2,&Xh3,&Yh3,&Xh4,&Yh4);
      vProfile.push_back(std::make_pair(0.0,Yh1));
      vProfile.push_back(std::make_pair(Xh1,Yh1));
      vProfile.push_back(std::make_pair(Xh2,Yh2));
      vProfile.push_back(std::make_pair(Xh3,Yh3));
      vProfile.push_back(std::make_pair(Xh4,Yh4));
      vProfile.push_back(std::make_pair(Lg,Yh4));
   }
   return vProfile;
}
