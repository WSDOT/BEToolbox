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

// PrismaticGirder.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PGStablePrismaticGirder.h"
#include "PGStableDoc.h"
#include "PGStableDDX.h"
#include <MFCTools\MFCTools.h>


// CPGStablePrismaticGirder dialog

IMPLEMENT_DYNAMIC(CPGStablePrismaticGirder, CDialog)

CPGStablePrismaticGirder::CPGStablePrismaticGirder(CWnd* pParent /*=NULL*/)
	: CDialog(CPGStablePrismaticGirder::IDD, pParent)
{
   m_pPointLoadGrid = NULL;
}

CPGStablePrismaticGirder::~CPGStablePrismaticGirder()
{
   delete m_pPointLoadGrid;
}

void CPGStablePrismaticGirder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

   {
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   DDX_MetaFileStatic(pDX, IDC_STRAND_LAYOUT, m_StrandLayout, AfxGetResourceHandle(), _T("PrismaticStrandLayout"), _T("Metafile") );
   }

   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   CView* pParent = (CView*)GetParent();
   CPGStableDoc* pDoc = (CPGStableDoc*)pParent->GetDocument();

   stbGirder girder = pDoc->GetGirder(PRISMATIC);
   CPGStableStrands strands = pDoc->GetStrands(PRISMATIC);

   ATLASSERT(girder.GetSectionCount() == 1); // prismatic girders have only one section
   IndexType sectIdx = 0;
   Float64 Ag,Ix,Iy,Yt,Hg,Wtf,Wbf;
   girder.GetSectionProperties(sectIdx,LEFT,&Ag,&Ix,&Iy,&Yt,&Hg,&Wtf,&Wbf);

   Float64 L = girder.GetSectionLength(sectIdx);

   Yt *= -1; //this is in girder section coordinates, convert to Y positive down coordinates

   DDX_UnitValueAndTag(pDX,IDC_HG,IDC_HG_UNIT, Hg, pDispUnits->ComponentDim);
   DDX_UnitValueAndTag(pDX,IDC_WTF,IDC_WTF_UNIT, Wtf, pDispUnits->ComponentDim);
   DDX_UnitValueAndTag(pDX,IDC_WBF,IDC_WBF_UNIT, Wbf, pDispUnits->ComponentDim);
   
   DDX_UnitValueAndTag(pDX,IDC_AREA,IDC_AREA_UNIT,Ag,pDispUnits->Area);
   DDX_UnitValueAndTag(pDX,IDC_IX,IDC_IX_UNIT,Ix,pDispUnits->MomentOfInertia);
   DDX_UnitValueAndTag(pDX,IDC_IY,IDC_IY_UNIT,Iy,pDispUnits->MomentOfInertia);
   DDX_UnitValueAndTag(pDX,IDC_YT,IDC_YT_UNIT,Yt,pDispUnits->ComponentDim);
   DDX_UnitValueAndTag(pDX,IDC_L,IDC_L_UNIT,L,pDispUnits->SpanLength);

   // strand locations are in girder section coordinates (0,0 at top center of girder)
   // locations are always going to be negative values, but we don't want the user to see this
   strands.YsMeasure = BOTTOM;
   strands.YtMeasure = TOP;

   strands.Xh1 = 0;
   strands.Xh1Measure = FRACTION;
   strands.Yh1Measure = TOP;

   strands.Xh2Measure = FRACTION;
   strands.Yh2Measure = BOTTOM;

   DDX_UnitValueAndTag(pDX,IDC_XFER_LENGTH,IDC_XFER_LENGTH_UNIT,strands.XferLength,pDispUnits->ComponentDim);

   DDX_UnitValueAndTag(pDX,IDC_YS,IDC_YS_UNIT,strands.Ys,pDispUnits->ComponentDim);
   DDX_UnitValueAndTag(pDX,IDC_YTEMP,IDC_YTEMP_UNIT,strands.Yt,pDispUnits->ComponentDim);

   DDX_UnitValueAndTag(pDX,IDC_YH1,IDC_YH1_UNIT,strands.Yh1,pDispUnits->ComponentDim);
   DDX_UnitValueAndTag(pDX,IDC_YH2,IDC_YH2_UNIT,strands.Yh2,pDispUnits->ComponentDim);

   DDX_Text(pDX,IDC_HP,strands.Xh2);

   Float64 densityWithRebar = girder.GetDensity();
   Float64 density = pDoc->GetDensity();

   DDX_UnitValueAndTag(pDX,IDC_DENSITY,IDC_DENSITY_UNIT,density,pDispUnits->Density);
   DDX_UnitValueAndTag(pDX,IDC_DENSITY_WITH_REBAR,IDC_DENSITY_WITH_REBAR_UNIT,densityWithRebar,pDispUnits->Density);

   std::vector<std::pair<Float64,Float64>> vLoads = girder.GetAdditionalLoads();
   DDX_PointLoadGrid(pDX,m_pPointLoadGrid,vLoads);

   if ( pDX->m_bSaveAndValidate )
   {
      Yt *= -1;

      strands.Xh3 = 1.0 - strands.Xh2;
      strands.Xh3Measure = FRACTION;
      strands.Yh3 = strands.Yh2;
      strands.Yh3Measure = strands.Yh2Measure;

      strands.Xh4 = 1.0;
      strands.Xh4Measure = FRACTION;
      strands.Yh4 = strands.Yh1;
      strands.Yh4Measure = strands.Yh1Measure;

#pragma Reminder("WORKING HERE - add validation")
      // most values cannot be zero
      // check vLoads... X must be between 0 and L
      // harp point must be between 0 and 0.5

      // UI assumes prismatic single piece so make right = left
      IndexType sectIdx = 0;
      girder.SetSectionProperties(sectIdx,L,Ag,Ix,Iy,Yt,Hg,Wtf,Wbf);

      girder.SetDensity(densityWithRebar);

      girder.SetAdditionalLoads(vLoads);

      pDoc->SetGirder(PRISMATIC,girder);
      pDoc->SetStrands(PRISMATIC,strands);
      pDoc->SetDensity(density);
   }
}


BEGIN_MESSAGE_MAP(CPGStablePrismaticGirder, CDialog)
   ON_BN_CLICKED(IDC_ADD, &CPGStablePrismaticGirder::OnAddPointLoad)
   ON_BN_CLICKED(IDC_REMOVE, &CPGStablePrismaticGirder::OnRemovePointLoad)
   ON_UPDATE_COMMAND_UI(IDC_REMOVE,&CPGStablePrismaticGirder::OnUpdateRemove)
END_MESSAGE_MAP()


BOOL CPGStablePrismaticGirder::OnInitDialog()
{
   {
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   m_pPointLoadGrid = new CPGStablePointLoadGrid;
	m_pPointLoadGrid->SubclassDlgItem(IDC_POINT_LOAD_GRID, this);
   m_pPointLoadGrid->CustomInit();
   }

   CDialog::OnInitDialog();

   // TODO:  Add extra initialization here

   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}

// CPGStablePrismaticGirder message handlers

void CPGStablePrismaticGirder::OnAddPointLoad()
{
   // TODO: Add your control notification handler code here
   m_pPointLoadGrid->AddLoad();
}

void CPGStablePrismaticGirder::OnRemovePointLoad()
{
   m_pPointLoadGrid->RemoveSelectedLoads();
}

void CPGStablePrismaticGirder::OnUpdateRemove(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(AreLoadsSelected() ? TRUE : FALSE);
}

BOOL CPGStablePrismaticGirder::AreLoadsSelected()
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
