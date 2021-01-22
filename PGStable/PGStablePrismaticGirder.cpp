///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2020  Washington State Department of Transportation
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
#include "..\resource.h"
#include "PGStableDoc.h"
#include "PGStablePrismaticGirder.h"
#include "PGStableDDX.h"
#include <MFCTools\MFCTools.h>

#include <IFace\BeamFactory.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void DDX_Girder(CDataExchange* pDX,stbGirder& girder)
{
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   ATLASSERT(girder.GetSectionCount() == 1); // prismatic girders have only one section
   IndexType sectIdx = 0;
   Float64 Ag,Ixx,Iyy,Ixy,Xleft,Ytop,Hg,Wtf,Wbf;
   girder.GetSectionProperties(sectIdx,stbTypes::Start,&Ag,&Ixx,&Iyy,&Ixy,&Xleft,&Ytop,&Hg,&Wtf,&Wbf);

   Float64 L = girder.GetSectionLength(sectIdx);

   Float64 precamber = girder.GetPrecamber();

   Ytop *= -1; //this is in girder section coordinates, convert to Y positive down coordinates

   DDX_UnitValueAndTag(pDX,IDC_HG,IDC_HG_UNIT, Hg, pDispUnits->ComponentDim);
   DDV_UnitValueGreaterThanZero(pDX,IDC_HG,Hg,pDispUnits->ComponentDim);

   DDX_UnitValueAndTag(pDX,IDC_WTF,IDC_WTF_UNIT, Wtf, pDispUnits->ComponentDim);
   DDV_UnitValueGreaterThanZero(pDX,IDC_WTF, Wtf, pDispUnits->ComponentDim);

   DDX_UnitValueAndTag(pDX,IDC_WBF,IDC_WBF_UNIT, Wbf, pDispUnits->ComponentDim);
   DDV_UnitValueGreaterThanZero(pDX,IDC_WBF, Wbf, pDispUnits->ComponentDim);
   
   DDX_UnitValueAndTag(pDX,IDC_AREA,IDC_AREA_UNIT,Ag,pDispUnits->Area);
   DDV_UnitValueGreaterThanZero(pDX,IDC_AREA,Ag,pDispUnits->Area);

   DDX_UnitValueAndTag(pDX,IDC_IX,IDC_IX_UNIT,Ixx,pDispUnits->MomentOfInertia);
   DDV_UnitValueGreaterThanZero(pDX,IDC_IX,Ixx,pDispUnits->MomentOfInertia);

   DDX_UnitValueAndTag(pDX,IDC_IY,IDC_IY_UNIT,Iyy,pDispUnits->MomentOfInertia);
   DDV_UnitValueGreaterThanZero(pDX,IDC_IY,Iyy,pDispUnits->MomentOfInertia);

   DDX_UnitValueAndTag(pDX, IDC_IXY, IDC_IXY_UNIT, Ixy, pDispUnits->MomentOfInertia);
   //DDV_UnitValueGreaterThanZero(pDX, IDC_IXY, Ixy, pDispUnits->MomentOfInertia); // can by +, -, 0

   DDX_UnitValueAndTag(pDX, IDC_YTOP, IDC_YTOP_UNIT, Ytop, pDispUnits->ComponentDim);
   DDV_UnitValueGreaterThanZero(pDX, IDC_YTOP, Ytop, pDispUnits->ComponentDim);

   DDX_UnitValueAndTag(pDX, IDC_XLEFT, IDC_XLEFT_UNIT, Xleft, pDispUnits->ComponentDim);
   DDV_UnitValueGreaterThanZero(pDX, IDC_XLEFT, Xleft, pDispUnits->ComponentDim);

   DDX_UnitValueAndTag(pDX, IDC_PRECAMBER, IDC_PRECAMBER_UNIT, precamber, pDispUnits->ComponentDim);

   DDX_UnitValueAndTag(pDX,IDC_L,IDC_L_UNIT,L,pDispUnits->SpanLength);
   DDV_UnitValueGreaterThanZero(pDX,IDC_L,L,pDispUnits->SpanLength);

   if ( pDX->m_bSaveAndValidate )
   {
      Ytop *= -1;
      girder.ClearSections();
      girder.AddSection(L,Ag,Ixx,Iyy,Ixy,Xleft,Ytop,Hg,Wtf,Wbf);
      girder.SetPrecamber(precamber);
   }
}

void DDX_PrismaticGirderStrands(CDataExchange* pDX,CPGStableStrands& strands)
{
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   DDX_CBEnum(pDX,IDC_PS_METHOD,strands.strandMethod);

   DDX_UnitValueAndTag(pDX, IDC_EX, IDC_EX_UNIT, strands.ex, pDispUnits->ComponentDim);

   DDX_UnitValueAndTag(pDX,IDC_XFER_LENGTH,IDC_XFER_LENGTH_UNIT,strands.XferLength,pDispUnits->ComponentDim);
   DDV_UnitValueZeroOrMore(pDX,IDC_XFER_LENGTH,strands.XferLength,pDispUnits->ComponentDim);

   strands.YsMeasure = BOTTOM;
   strands.YtMeasure = TOP;

   strands.Xh1 = 0;
   strands.Xh1Measure = FRACTION;
   strands.Yh1Measure = TOP;

   strands.Xh2Measure = FRACTION;
   strands.Yh2Measure = BOTTOM;

   DDX_UnitValueAndTag(pDX,IDC_YS,IDC_YS_UNIT,strands.Ys,pDispUnits->ComponentDim);
   DDV_UnitValueZeroOrMore(pDX,IDC_YS,strands.Ys,pDispUnits->ComponentDim);

   DDX_UnitValueAndTag(pDX,IDC_YTEMP,IDC_YTEMP_UNIT,strands.Yt,pDispUnits->ComponentDim);
   DDV_UnitValueZeroOrMore(pDX,IDC_YTEMP,strands.Yt,pDispUnits->ComponentDim);

   DDX_UnitValueAndTag(pDX,IDC_YH1,IDC_YH1_UNIT,strands.Yh1,pDispUnits->ComponentDim);
   DDV_UnitValueZeroOrMore(pDX,IDC_YH1,strands.Yh1,pDispUnits->ComponentDim);

   DDX_UnitValueAndTag(pDX,IDC_YH2,IDC_YH2_UNIT,strands.Yh2,pDispUnits->ComponentDim);
   DDV_UnitValueZeroOrMore(pDX,IDC_YH2,strands.Yh2,pDispUnits->ComponentDim);

   DDX_Text(pDX,IDC_HP,strands.Xh2);
   DDV_MinMaxDouble(pDX,strands.Xh2,0.0,0.5);

   if ( pDX->m_bSaveAndValidate )
   {
      strands.Xh3 = 1.0 - strands.Xh2;
      strands.Xh3Measure = FRACTION;
      strands.Yh3 = strands.Yh2;
      strands.Yh3Measure = strands.Yh2Measure;

      strands.Xh4 = 1.0;
      strands.Xh4Measure = FRACTION;
      strands.Yh4 = strands.Yh1;
      strands.Yh4Measure = strands.Yh1Measure;
   }
}

// CPGStablePrismaticGirder dialog

IMPLEMENT_DYNAMIC(CPGStablePrismaticGirder, CDialog)

CPGStablePrismaticGirder::CPGStablePrismaticGirder(CWnd* pParent /*=nullptr*/)
	: CDialog(CPGStablePrismaticGirder::IDD, pParent)
{
}

CPGStablePrismaticGirder::~CPGStablePrismaticGirder()
{
}

void CPGStablePrismaticGirder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
   {
      AFX_MANAGE_STATE(AfxGetStaticModuleState());
      DDX_MetaFileStatic(pDX, IDC_STRAND_LAYOUT, m_StrandLayout, AfxGetResourceHandle(), _T("PrismaticStrandLayout"), _T("Metafile") );
   }
   DDX_Control(pDX,IDC_GIRDER,m_ctrlGirder);

   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   CView* pParent = (CView*)GetParent();
   CPGStableDoc* pDoc = (CPGStableDoc*)pParent->GetDocument();

   CString strGirder = pDoc->GetGirder();
   DDX_CBString(pDX,IDC_GIRDER_LIST,strGirder);

   stbGirder girder = pDoc->GetGirder(PRISMATIC);
   CPGStableStrands lifting_strands = pDoc->GetStrands(PRISMATIC, LIFTING);
   CPGStableStrands hauling_strands = pDoc->GetStrands(PRISMATIC, HAULING);


   Float64 Cd = girder.GetDragCoefficient();
   DDX_Text(pDX,IDC_DRAG_COEFFICIENT,Cd);
   DDV_GreaterThanZero(pDX,IDC_DRAG_COEFFICIENT,Cd);

   DDX_Girder(pDX,girder);
   DDX_PrismaticGirderStrands(pDX, lifting_strands);
   DDX_PrismaticGirderStrands(pDX, hauling_strands);

   int stressPointType = pDoc->GetStressPointType();
   DDX_Radio(pDX, IDC_COMPUTE_STRESS_POINTS, stressPointType);

   if (pDX->m_bSaveAndValidate)
   {
      m_pStressPointGrid->GetTopLeft(&m_pntTL.X(), &m_pntTL.Y());
      m_pStressPointGrid->GetTopRight(&m_pntTR.X(), &m_pntTR.Y());
      m_pStressPointGrid->GetBottomLeft(&m_pntBL.X(), &m_pntBL.Y());
      m_pStressPointGrid->GetBottomRight(&m_pntBR.X(), &m_pntBR.Y());
   }
   else
   {
      m_pStressPointGrid->SetTopLeft(m_pntTL.X(),m_pntTL.Y());
      m_pStressPointGrid->SetTopRight(m_pntTR.X(), m_pntTR.Y());
      m_pStressPointGrid->SetBottomLeft(m_pntBL.X(), m_pntBL.Y());
      m_pStressPointGrid->SetBottomRight(m_pntBR.X(), m_pntBR.Y());
   }

   Float64 densityWithRebar = pDoc->GetDensityWithRebar();
   Float64 density = pDoc->GetDensity();

   DDX_UnitValueAndTag(pDX,IDC_DENSITY,IDC_DENSITY_UNIT,density,pDispUnits->Density);
   DDV_UnitValueGreaterThanZero(pDX,IDC_DENSITY,density,pDispUnits->Density);
   DDX_UnitValueAndTag(pDX,IDC_DENSITY_WITH_REBAR,IDC_DENSITY_WITH_REBAR_UNIT,densityWithRebar,pDispUnits->Density);
   DDV_UnitValueGreaterThanZero(pDX,IDC_DENSITY_WITH_REBAR,densityWithRebar,pDispUnits->Density);

   matConcrete::Type concrete_type = pDoc->GetConcreteType();
   DDX_RadioEnum(pDX, IDC_NWC, concrete_type);

   std::vector<std::pair<Float64,Float64>> vLoads = girder.GetAdditionalLoads();
   DDX_PointLoadGrid(pDX,m_pPointLoadGrid.get(),vLoads);


   if ( pDX->m_bSaveAndValidate )
   {
      girder.SetAdditionalLoads(vLoads);

      girder.SetDragCoefficient(Cd);

      pDoc->SetStressPointType(stressPointType);
      if (stressPointType == DEFINE_STRESS_POINTS)
      {
         girder.SetStressPoints(0, m_pntTL, m_pntTR, m_pntBL, m_pntBR);
      }

      pDoc->SetGirder(PRISMATIC,girder);
      pDoc->SetStrands(PRISMATIC, LIFTING, lifting_strands);
      pDoc->SetStrands(PRISMATIC, HAULING, hauling_strands);

      pDoc->SetDensity(density);
      pDoc->SetDensityWithRebar(densityWithRebar);

      pDoc->SetConcreteType(concrete_type);

      pDoc->SetGirder(strGirder);
   }

   if ( !pDX->m_bSaveAndValidate )
   {
      m_ctrlGirder.Invalidate();
      m_ctrlGirder.UpdateWindow();
   }
}


BEGIN_MESSAGE_MAP(CPGStablePrismaticGirder, CDialog)
   ON_BN_CLICKED(IDC_ADD, &CPGStablePrismaticGirder::OnAddPointLoad)
   ON_BN_CLICKED(IDC_REMOVE, &CPGStablePrismaticGirder::OnRemovePointLoad)
   ON_UPDATE_COMMAND_UI(IDC_REMOVE,&CPGStablePrismaticGirder::OnUpdateRemove)
	ON_MESSAGE(WM_HELP, OnCommandHelp)
   ON_CBN_SELCHANGE(IDC_PS_METHOD, &CPGStablePrismaticGirder::OnPSMethodChanged)
   ON_CBN_SELCHANGE(IDC_GIRDER_LIST, &CPGStablePrismaticGirder::OnGirderChanged)
//   ON_WM_KILLFOCUS()
   ON_BN_CLICKED(IDC_COMPUTE_STRESS_POINTS, &CPGStablePrismaticGirder::OnBnClickedComputeStressPoints)
   ON_BN_CLICKED(IDC_DEFINE_STRESS_POINTS, &CPGStablePrismaticGirder::OnBnClickedDefineStressPoints)
   ON_EN_CHANGE(IDC_HG, &CPGStablePrismaticGirder::OnChangedStressPointDimension)
   ON_EN_CHANGE(IDC_WTF, &CPGStablePrismaticGirder::OnChangedStressPointDimension)
   ON_EN_CHANGE(IDC_WBF, &CPGStablePrismaticGirder::OnChangedStressPointDimension)
   ON_EN_CHANGE(IDC_YTOP, &CPGStablePrismaticGirder::OnChangedStressPointDimension)
   ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


LRESULT CPGStablePrismaticGirder::OnCommandHelp(WPARAM, LPARAM lParam)
{
   EAFHelp( EAFGetDocument()->GetDocumentationSetName(), IDH_PGSTABLE_PRISMATIC_GIRDER_VIEW );
   return TRUE;
}

BOOL CPGStablePrismaticGirder::OnInitDialog()
{
   {
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   m_pStressPointGrid = std::make_unique<CPGStableStressPointGrid>();
   m_pStressPointGrid->SubclassDlgItem(IDC_STRESS_POINT_GRID, this);
   m_pStressPointGrid->CustomInit();

   m_pPointLoadGrid = std::make_unique<CPGStablePointLoadGrid>();
	m_pPointLoadGrid->SubclassDlgItem(IDC_POINT_LOAD_GRID, this);
   m_pPointLoadGrid->CustomInit();

   m_ctrlGirder.SubclassDlgItem(IDC_GIRDER,this);
   m_ctrlGirder.CustomInit();
   }

   CComboBox* pcbMethod = (CComboBox*)GetDlgItem(IDC_PS_METHOD);
   pcbMethod->SetItemData(pcbMethod->AddString(_T("Simplified")),(DWORD_PTR)CPGStableStrands::Simplified);
   pcbMethod->SetItemData(pcbMethod->AddString(_T("Detailed")),(DWORD_PTR)CPGStableStrands::Detailed);

   CPGStableDoc* pDoc = (CPGStableDoc*)EAFGetDocument();
   CComboBox* pcbGirders = (CComboBox*)GetDlgItem(IDC_GIRDER_LIST);
   pcbGirders->AddString(gs_strGirder);
   const GirderLibrary* pLib = pDoc->GetGirderLibrary();
   libKeyListType keyList;
   pLib->KeyList(keyList);
   for (const auto& key : keyList)
   {
      // fill combo box with only the names of prismatic girders
      const GirderLibraryEntry* pGirderEntry = (const GirderLibraryEntry*)(pLib->GetEntry(key.c_str()));
      if ( pDoc->IsPermittedGirderEntry(pGirderEntry) )
      {
         pcbGirders->AddString(key.c_str());
      }
   }

   const stbGirder& girder = pDoc->GetGirder(PRISMATIC);
   IndexType sectIdx = 0;
   Float64 Ag, Ixx, Iyy, Ixy, Xleft, Ytop, Hg, Wtf, Wbf;
   girder.GetSectionProperties(sectIdx, stbTypes::Start, &Ag, &Ixx, &Iyy, &Ixy, &Xleft, &Ytop, &Hg, &Wtf, &Wbf);
   Ytop *= -1; //this is in girder section coordinates, convert to Y positive down coordinates

   if (pDoc->GetStressPointType() == COMPUTE_STRESS_POINTS)
   {
      InitStressPoints(Wtf, Wbf, Ytop, Hg);
   }
   else
   {
      girder.GetStressPoints(sectIdx, stbTypes::Start, &m_pntTL, &m_pntTR, &m_pntBL, &m_pntBR);
   }
   m_pntTLCache = m_pntTL;
   m_pntTRCache = m_pntTR;
   m_pntBLCache = m_pntBL;
   m_pntBRCache = m_pntBR;

   CDialog::OnInitDialog();

   OnPSMethodChanged();
   OnGirderChanged();

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

void CPGStablePrismaticGirder::OnPSMethodChanged()
{
   // TODO: Add your control notification handler code here
   CComboBox* pcbMethod = (CComboBox*)GetDlgItem(IDC_PS_METHOD);
   int curSel = pcbMethod->GetCurSel();
   CPGStableStrands::StrandMethod method = (CPGStableStrands::StrandMethod)(pcbMethod->GetItemData(curSel));

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

   int show = (method == CPGStableStrands::Simplified ? SW_SHOW : SW_HIDE);

   GetDlgItem(IDC_STRAND_LAYOUT)->ShowWindow(show);
   GetDlgItem(IDC_GIRDER)->ShowWindow(method == CPGStableStrands::Simplified ? SW_HIDE : SW_SHOW);

   GetDlgItem(IDC_XFER_LENGTH_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_XFER_LENGTH)->ShowWindow(show);
   GetDlgItem(IDC_XFER_LENGTH_UNIT)->ShowWindow(show);

   GetDlgItem(IDC_STRAND_LAYOUT)->ShowWindow(show);

   GetDlgItem(IDC_YS_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_YS)->ShowWindow(show);
   GetDlgItem(IDC_YS_UNIT)->ShowWindow(show);

   GetDlgItem(IDC_YH1_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_YH1)->ShowWindow(show);
   GetDlgItem(IDC_YH1_UNIT)->ShowWindow(show);

   GetDlgItem(IDC_YH2_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_YH2)->ShowWindow(show);
   GetDlgItem(IDC_YH2_UNIT)->ShowWindow(show);

   GetDlgItem(IDC_HP_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_HP)->ShowWindow(show);
   GetDlgItem(IDC_HP_UNIT)->ShowWindow(show);

   GetDlgItem(IDC_YTEMP_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_YTEMP)->ShowWindow(show);
   GetDlgItem(IDC_YTEMP_UNIT)->ShowWindow(show);

   GetDlgItem(IDC_SS_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_HS_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_TS_LABEL)->ShowWindow(show);

   GetDlgItem(IDC_EX_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_EX)->ShowWindow(show);
   GetDlgItem(IDC_EX_UNIT)->ShowWindow(show);
}

std::vector<std::pair<Float64,Float64>> CPGStablePrismaticGirder::GetGirderProfile()
{
   std::vector<std::pair<Float64,Float64>> vProfile;

   CDataExchange dx(this,TRUE);

   CView* pParent = (CView*)GetParent();
   CPGStableDoc* pDoc = (CPGStableDoc*)pParent->GetDocument();

   stbGirder girder = pDoc->GetGirder(PRISMATIC);
   DDX_Girder(&dx,girder);

   IndexType sectIdx = 0;
   Float64 Ag,Ixx,Iyy,Ixy,Xleft,Ytop,Hg,Wtf,Wbf;
   girder.GetSectionProperties(sectIdx,stbTypes::Start,&Ag,&Ixx,&Iyy,&Ixy,&Xleft,&Ytop,&Hg,&Wtf,&Wbf);

   Float64 Lg = girder.GetGirderLength();
   vProfile.emplace_back(0,0);
   vProfile.emplace_back(Lg,0);
   vProfile.emplace_back(Lg,-Hg);
   vProfile.emplace_back(0,-Hg);
   return vProfile;
}

void CPGStablePrismaticGirder::GetStrandProfiles(std::vector<std::pair<Float64,Float64>>* pvStraight,std::vector<std::pair<Float64,Float64>>* pvHarped,std::vector<std::pair<Float64,Float64>>* pvTemp)
{
   std::vector<std::pair<Float64,Float64>> vProfile;

   CDataExchange dx(this,TRUE);

   CView* pParent = (CView*)GetParent();
   CPGStableDoc* pDoc = (CPGStableDoc*)pParent->GetDocument();

   stbGirder girder = pDoc->GetGirder(PRISMATIC);
   DDX_Girder(&dx,girder);

   CPGStableStrands strands = pDoc->GetStrands(NONPRISMATIC,LIFTING);
   DDX_PrismaticGirderStrands(&dx,strands);

   pDoc->GetStrandProfiles(strands,girder,pvStraight,pvHarped,pvTemp);
}

void CPGStablePrismaticGirder::OnGirderChanged()
{
   CString strGirder;
   CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_GIRDER_LIST);
   int curSel = pCB->GetCurSel();
   pCB->GetLBText(curSel,strGirder);

   BOOL bEnable = strGirder == gs_strGirder ? TRUE : FALSE;
   GetDlgItem(IDC_HG)->EnableWindow(bEnable);
   GetDlgItem(IDC_WTF)->EnableWindow(bEnable);
   GetDlgItem(IDC_WBF)->EnableWindow(bEnable);
   GetDlgItem(IDC_AREA)->EnableWindow(bEnable);
   GetDlgItem(IDC_IX)->EnableWindow(bEnable);
   GetDlgItem(IDC_IY)->EnableWindow(bEnable);
   GetDlgItem(IDC_IXY)->EnableWindow(bEnable);
   GetDlgItem(IDC_YTOP)->EnableWindow(bEnable);
   GetDlgItem(IDC_XLEFT)->EnableWindow(bEnable);
   GetDlgItem(IDC_DRAG_COEFFICIENT)->EnableWindow(bEnable);

   if ( !bEnable )
   {
      CPGStableDoc* pDoc = (CPGStableDoc*)EAFGetDocument();
      const GirderLibrary* pLib = pDoc->GetGirderLibrary();
      const GirderLibraryEntry* pEntry = (const GirderLibraryEntry*)(pLib->GetEntry(strGirder));

      Float64 Cd = pEntry->GetDragCoefficient();

      CComPtr<IBeamFactory> factory;
      pEntry->GetBeamFactory(&factory);

      IBeamFactory::Dimensions dimensions = pEntry->GetDimensions();
      CComPtr<IGirderSection> section;
      factory->CreateGirderSection(nullptr,INVALID_ID,dimensions,-1,-1,&section);

      CComPtr<IShape> pShape;
      section.QueryInterface(&pShape);
      ATLASSERT(pShape != nullptr);

      CComPtr<IShapeProperties> shapeProperties;
      pShape->get_ShapeProperties(&shapeProperties);

      Float64 wleft, wright;
      section->get_TopWidth(&wleft, &wright);
      Float64 Wtf = wleft + wright;
      section->get_BottomWidth(&wleft, &wright);
      Float64 Wbf = wleft + wright;
      Float64 Hg;
      section->get_OverallHeight(&Hg);

      Float64 Xleft,Ytop, Ag, Ixx, Iyy, Ixy;
      shapeProperties->get_Area(&Ag);
      shapeProperties->get_Ixx(&Ixx);
      shapeProperties->get_Iyy(&Iyy);
      shapeProperties->get_Ixy(&Ixy);
      shapeProperties->get_Ytop(&Ytop);
      shapeProperties->get_Xleft(&Xleft);

      CEAFApp* pApp = EAFGetApp();
      const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();
      CDataExchange dx(this,false);
      DDX_UnitValueAndTag(&dx,IDC_HG,IDC_HG_UNIT, Hg, pDispUnits->ComponentDim);
      DDX_UnitValueAndTag(&dx,IDC_WTF,IDC_WTF_UNIT, Wtf, pDispUnits->ComponentDim);
      DDX_UnitValueAndTag(&dx,IDC_WBF,IDC_WBF_UNIT, Wbf, pDispUnits->ComponentDim);
      DDX_UnitValueAndTag(&dx,IDC_AREA,IDC_AREA_UNIT,Ag,pDispUnits->Area);
      DDX_UnitValueAndTag(&dx,IDC_IX,IDC_IX_UNIT,Ixx,pDispUnits->MomentOfInertia);
      DDX_UnitValueAndTag(&dx, IDC_IY, IDC_IY_UNIT, Iyy, pDispUnits->MomentOfInertia);
      DDX_UnitValueAndTag(&dx, IDC_IXY, IDC_IXY_UNIT, Ixy, pDispUnits->MomentOfInertia);
      DDX_UnitValueAndTag(&dx, IDC_YTOP, IDC_YTOP_UNIT, Ytop, pDispUnits->ComponentDim);
      DDX_UnitValueAndTag(&dx, IDC_XLEFT, IDC_XLEFT_UNIT, Xleft, pDispUnits->ComponentDim);
      DDX_Text(&dx,IDC_DRAG_COEFFICIENT,Cd);

      InitStressPoints(Wtf, Wbf, Ytop, Hg);
   }

   UpdateStressPoints();
}

void CPGStablePrismaticGirder::InitStressPoints(Float64 Wtf, Float64 Wbf, Float64 Ytop, Float64 Hg)
{
   m_pntTL.X() = -Wtf / 2;
   m_pntTL.Y() = Ytop;

   m_pntTR.X() = Wtf / 2;
   m_pntTR.Y() = Ytop;

   m_pntBL.X() = -Wbf / 2;
   m_pntBL.Y() = Ytop - Hg;

   m_pntBR.X() = Wbf / 2;
   m_pntBR.Y() = Ytop - Hg;
}

void CPGStablePrismaticGirder::UpdateStressPoints()
{
   if (GetCheckedRadioButton(IDC_COMPUTE_STRESS_POINTS, IDC_DEFINE_STRESS_POINTS) == IDC_COMPUTE_STRESS_POINTS)
   {
      Float64 Wtf, Wbf, Ytop, Hg;
      CEAFApp* pApp = EAFGetApp();
      const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();
      CDataExchange dx(this, true);
      DDX_UnitValueAndTag(&dx, IDC_HG, IDC_HG_UNIT, Hg, pDispUnits->ComponentDim);
      DDX_UnitValueAndTag(&dx, IDC_WTF, IDC_WTF_UNIT, Wtf, pDispUnits->ComponentDim);
      DDX_UnitValueAndTag(&dx, IDC_WBF, IDC_WBF_UNIT, Wbf, pDispUnits->ComponentDim);
      DDX_UnitValueAndTag(&dx, IDC_YTOP, IDC_YTOP_UNIT, Ytop, pDispUnits->ComponentDim);
      InitStressPoints(Wtf, Wbf, Ytop, Hg);

      m_pStressPointGrid->Enable(TRUE); // must enable the grid or the values one change
      m_pStressPointGrid->SetTopLeft(m_pntTL.X(),m_pntTL.Y());
      m_pStressPointGrid->SetTopRight(m_pntTR.X(), m_pntTR.Y());
      m_pStressPointGrid->SetBottomLeft(m_pntBL.X(), m_pntBL.Y());
      m_pStressPointGrid->SetBottomRight(m_pntBR.X(), m_pntBR.Y());
      m_pStressPointGrid->Enable(FALSE); // restore the enabled state
   }
}

void CPGStablePrismaticGirder::OnBnClickedComputeStressPoints()
{
   // Cache the current user defined values
   m_pStressPointGrid->GetTopLeft(&m_pntTLCache.X(), &m_pntTLCache.Y());
   m_pStressPointGrid->GetTopRight(&m_pntTRCache.X(), &m_pntTRCache.Y());
   m_pStressPointGrid->GetBottomLeft(&m_pntBLCache.X(), &m_pntBLCache.Y());
   m_pStressPointGrid->GetBottomRight(&m_pntBRCache.X(), &m_pntBRCache.Y());

   // Restore the values computed from the cross section
   UpdateStressPoints();

   // disable the grid since it contains computed values
   m_pStressPointGrid->Enable(FALSE);
}

void CPGStablePrismaticGirder::OnBnClickedDefineStressPoints()
{
   // enable the grid so values can be changed
   m_pStressPointGrid->Enable(TRUE);

   // Restore the user defined stress points
   m_pStressPointGrid->SetTopLeft(m_pntTLCache.X(), m_pntTLCache.Y());
   m_pStressPointGrid->SetTopRight(m_pntTRCache.X(), m_pntTRCache.Y());
   m_pStressPointGrid->SetBottomLeft(m_pntBLCache.X(), m_pntBLCache.Y());
   m_pStressPointGrid->SetBottomRight(m_pntBRCache.X(), m_pntBRCache.Y());
}

void CPGStablePrismaticGirder::OnChangedStressPointDimension()
{
   UpdateStressPoints();
}

void CPGStablePrismaticGirder::OnShowWindow(BOOL bShow, UINT nStatus)
{
   __super::OnShowWindow(bShow, nStatus);

   // TODO: Add your message handler code here
   if (bShow)
   {
      CView* pParent = (CView*)GetParent();
      CPGStableDoc* pDoc = (CPGStableDoc*)pParent->GetDocument();
      m_pStressPointGrid->Enable(pDoc->GetStressPointType() == DEFINE_STRESS_POINTS); // enable grid if user defined stress points
   }
}


void CPGStablePrismaticGirder::OnCancel()
{
   // prevent [ESC] from closing window
   //__super::OnCancel();
}


void CPGStablePrismaticGirder::OnOK()
{
   // prevent [Enter] from closing window
   //__super::OnOK();
}

void CPGStablePrismaticGirder::OnUnitsChanged()
{
   UpdateData(FALSE);
   m_pStressPointGrid->OnUnitsChanged();
}
