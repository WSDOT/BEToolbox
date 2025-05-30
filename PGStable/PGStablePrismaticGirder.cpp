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

// PrismaticGirder.cpp : implementation file
//

#include "stdafx.h"
#include "..\resource.h"
#include "PGStableDoc.h"
#include "PGStablePrismaticGirder.h"
#include "PGStableDDX.h"
#include <MFCTools\MFCTools.h>

#include <IFace\BeamFactory.h>

#include <PsgLib\UnitServer.h>

#include <Stability/Stability.h>
#include <psgLib/SpecificationCriteria.h>
#include <psgLib/PrestressedElementCriteria.h>

// inline functions to convert rebar data
inline BarSize GetBarSize(WBFL::Materials::Rebar::Size size)
{
   switch (size)
   {
   case WBFL::Materials::Rebar::Size::bs3:  return bs3;
   case WBFL::Materials::Rebar::Size::bs4:  return bs4;
   case WBFL::Materials::Rebar::Size::bs5:  return bs5;
   case WBFL::Materials::Rebar::Size::bs6:  return bs6;
   case WBFL::Materials::Rebar::Size::bs7:  return bs7;
   case WBFL::Materials::Rebar::Size::bs8:  return bs8;
   case WBFL::Materials::Rebar::Size::bs9:  return bs9;
   case WBFL::Materials::Rebar::Size::bs10: return bs10;
   case WBFL::Materials::Rebar::Size::bs11: return bs11;
   case WBFL::Materials::Rebar::Size::bs14: return bs14;
   case WBFL::Materials::Rebar::Size::bs18: return bs18;
   default:
      ATLASSERT(false); // should not get here
   }

   ATLASSERT(false); // should not get here
   return bs3;
}

inline RebarGrade GetRebarGrade(WBFL::Materials::Rebar::Grade grade)
{
   RebarGrade matGrade;
   switch (grade)
   {
   case WBFL::Materials::Rebar::Grade40: matGrade = Grade40; break;
   case WBFL::Materials::Rebar::Grade::Grade60: matGrade = Grade60; break;
   case WBFL::Materials::Rebar::Grade75: matGrade = Grade75; break;
   case WBFL::Materials::Rebar::Grade80: matGrade = Grade80; break;
   case WBFL::Materials::Rebar::Grade100: matGrade = Grade100; break;
   default:
      ATLASSERT(false);
   }

   return matGrade;
}

inline MaterialSpec GetRebarSpecification(WBFL::Materials::Rebar::Type type)
{
   return (type == WBFL::Materials::Rebar::Type::A615 ? msA615 : (type == WBFL::Materials::Rebar::Type::A706 ? msA706 : msA1035));
}


// Utility class to implement abstract data needs for alternate tensile stress requirement for pgstable models
class PGStableAlternateTensStressDataProvider : public WBFL::Stability::IAlternateTensStressDataProvider
{
public:
   PGStableAlternateTensStressDataProvider(IShape* pShape, Float64 L, Float64 Hg, const CPGStableLongitudinalRebarData& rebarData) :
      m_pShape(pShape),
      m_RebarData(rebarData),
      m_L(L),
      m_Hg(Hg),
      m_IsInit(false)
   {
   }

   // IAlternateTensStressDataProvider
   virtual HRESULT GetGirderShape(Float64 Xs, IShape** ppShape) override
   {
      if (m_pShape)
      {
         return m_pShape.CopyTo(ppShape);
      }
      else
      {
         ATLASSERT(0);
         *ppShape = nullptr;
         return E_FAIL;
      }
   }

   virtual HRESULT CreateRebarSection(Float64 Xs, IRebarSection** ppSection) override
   {
      Init();

      CComPtr<IRebarSection> pRebarSection;
      pRebarSection.CoCreateInstance(CLSID_RebarSection);

      MaterialSpec  matSpec = GetRebarSpecification(m_RebarData.BarType);
      RebarGrade    matGrade = GetRebarGrade(m_RebarData.BarGrade);

      for (const auto& barrow : m_RebarData.RebarRows)
      {
         BarSize       matSize = GetBarSize(barrow.BarSize);
         CComPtr<IRebar> rebar;
         m_RebarFactory->CreateRebar(matSpec, matGrade, matSize, m_UnitConvert, 0, &rebar);

         Float64 db;
         rebar->get_NominalDiameter(&db);

         Float64 topCover = barrow.Cover;

         // X location of left most bar
         Float64 barLoc = -1.0 * ((barrow.NumberOfBars - 1) * barrow.BarSpacing) / 2.0;
         for (int ibar = 0; ibar < barrow.NumberOfBars; ibar++)
         {
            CComPtr<IPoint2d> point;
            point.CoCreateInstance(CLSID_Point2d);
            point->Move(barLoc, m_Hg - topCover - db);

            Float64 lftDist = Xs;
            Float64 rgtDist = m_L - Xs;

            CComPtr<IRebarSectionItem> sectionItem;
            sectionItem.CoCreateInstance(CLSID_RebarSectionItem);
            sectionItem->Init(point, 1000.0, 1000.0, htNone, htNone, rebar);

            pRebarSection->Add(sectionItem);

            barLoc += barrow.BarSpacing;
         }
      }

      return pRebarSection.CopyTo(ppSection);
   }

private:
   void Init()
   {
      if (!m_IsInit)
      {
         // Create com objects we need for all rebar calls
         // Tebar factory. This does the work of creating rebar objects
         m_RebarFactory.CoCreateInstance(CLSID_RebarFactory);

         // Need a unit server object for units conversion in factory
         CComPtr<IUnitServer> unitServer;
         unitServer.CoCreateInstance(CLSID_UnitServer);
         HRESULT hr = ConfigureUnitServer(unitServer);
         ATLASSERT(SUCCEEDED(hr));

         unitServer->get_UnitConvert(&m_UnitConvert);

         m_IsInit = true;
      }
   }

private:
   PGStableAlternateTensStressDataProvider();
   CComPtr<IShape> m_pShape;
   CPGStableLongitudinalRebarData m_RebarData;
   Float64 m_L;
   Float64 m_Hg;
   bool m_IsInit;
   CComPtr<IRebarFactory> m_RebarFactory;
   CComPtr<IUnitConvert> m_UnitConvert;
};



void DDX_Girder(CDataExchange* pDX,WBFL::Stability::Girder& girder)
{
   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   ATLASSERT(girder.GetSectionCount() == 1); // prismatic girders have only one section
   IndexType sectIdx = 0;
   Float64 Ag,Ixx,Iyy,Ixy,Xleft,Ytop,Hg,Wtf,Wbf;
   girder.GetSectionProperties(sectIdx,WBFL::Stability::Section::Start,&Ag,&Ixx,&Iyy,&Ixy,&Xleft,&Ytop,&Hg,&Wtf,&Wbf);

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
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   DDX_CBEnum(pDX,IDC_PS_METHOD,strands.strandMethod);

   DDX_UnitValueAndTag(pDX, IDC_EX, IDC_EX_UNIT, strands.ex, pDispUnits->ComponentDim);

   DDX_UnitValueAndTag(pDX,IDC_XFER_LENGTH,IDC_XFER_LENGTH_UNIT,strands.XferLength,pDispUnits->ComponentDim);
   DDV_UnitValueZeroOrMore(pDX,IDC_XFER_LENGTH,strands.XferLength,pDispUnits->ComponentDim);

   strands.YsMeasure = BOTTOM_STRANDS;
   strands.YtMeasure = TOP_STRANDS;

   strands.Xh1 = 0;
   strands.Xh1Measure = FRACTION;
   strands.Yh1Measure = TOP_STRANDS;

   strands.Xh2Measure = FRACTION;
   strands.Yh2Measure = BOTTOM_STRANDS;

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
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   CView* pParent = (CView*)GetParent();
   CPGStableDoc* pDoc = (CPGStableDoc*)pParent->GetDocument();

   CString strGirder = pDoc->GetGirder();
   DDX_CBString(pDX,IDC_GIRDER_LIST,strGirder);

   WBFL::Stability::Girder girder = pDoc->GetGirder(GirderType::Prismatic);
   CPGStableStrands lifting_strands = pDoc->GetStrands(GirderType::Prismatic, ModelType::Lifting);
   CPGStableStrands hauling_strands = pDoc->GetStrands(GirderType::Prismatic, ModelType::Hauling);
   CPGStableStrands one_end_seated_strands = pDoc->GetStrands(GirderType::Prismatic, ModelType::OneEndSeated);


   Float64 Cd = girder.GetDragCoefficient();
   DDX_Text(pDX,IDC_DRAG_COEFFICIENT,Cd);
   DDV_GreaterThanZero(pDX,IDC_DRAG_COEFFICIENT,Cd);

   DDX_Control(pDX, IDC_MILD_STEEL_SELECTOR, m_cbRebar);

   CPGStableLongitudinalRebarData rebarData(pDoc->GetPGStableLongitudinalRebarData());
   DDX_UnitValueAndTag(pDX, IDC_COVER_LIMIT, IDC_COVER_LIMIT_UNIT, rebarData.MaxCoverToUseHigherTensionStressLimit, pDispUnits->ComponentDim);
   DDV_UnitValueZeroOrMore(pDX, IDC_COVER_LIMIT, rebarData.MaxCoverToUseHigherTensionStressLimit, pDispUnits->ComponentDim);
   DDX_RebarMaterial(pDX, IDC_MILD_STEEL_SELECTOR, rebarData.BarType, rebarData.BarGrade);

   if (pDX->m_bSaveAndValidate)
   {
      CPGStableLongitudinalRebarData rebarDataGrid;
      if (!m_pPGStableLongRebarGrid->GetRebarData(pDX, &rebarDataGrid))
      {
         pDX->PrepareCtrl(IDC_LONG_GRID);
         pDX->Fail();
      }

      rebarDataGrid.BarType = rebarData.BarType;
      rebarDataGrid.MaxCoverToUseHigherTensionStressLimit = rebarData.MaxCoverToUseHigherTensionStressLimit;
      pDoc->SetPGStableLongitudinalRebarData(rebarDataGrid);
   }
   else
   {
      m_pPGStableLongRebarGrid->FillGrid(rebarData);
      OnEnableDeleteRebarRow(false);
   }

   DDX_Girder(pDX,girder);
   DDX_PrismaticGirderStrands(pDX, lifting_strands);
   DDX_PrismaticGirderStrands(pDX, hauling_strands);
   DDX_PrismaticGirderStrands(pDX, one_end_seated_strands);

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

   auto concrete_type = pDoc->GetConcreteType();
   DDX_RadioEnum(pDX, IDC_NWC, concrete_type);

   Float64 K1 = pDoc->GetK1();
   Float64 K2 = pDoc->GetK2();
   DDX_Text(pDX, IDC_K1, K1);
   DDX_Text(pDX, IDC_K2, K2);

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

      pDoc->SetGirder(strGirder);

      // We can only compute alternative allowable tensile stress if we have a girder entry with a shape to integrate over. 
      const GirderLibraryEntry* pGdrEntry = pDoc->GetGirderLibraryEntry();
      if (pGdrEntry != nullptr)
      {
         auto pFactory = pGdrEntry->GetBeamFactory();
         GirderLibraryEntry::Dimensions dimensions = pGdrEntry->GetDimensions();

         CComPtr<IGirderSection> gdrSection;
         pFactory->CreateGirderSection(nullptr, INVALID_ID, dimensions, -1.0, -1.0, &gdrSection);

         CComPtr<IShape> pShape;
         gdrSection.QueryInterface(&pShape);

         // bounding box and height
         CComPtr<IRect2d> bbox;
         pShape->get_BoundingBox(&bbox);
         Float64 Hg;
         bbox->get_Height(&Hg);

         Float64 length = girder.GetGirderLength();
         CPGStableLongitudinalRebarData rebarData = pDoc->GetPGStableLongitudinalRebarData();

         std::shared_ptr<WBFL::Stability::IAlternateTensStressDataProvider> pATSP(std::make_shared<PGStableAlternateTensStressDataProvider>(pShape, length, Hg, rebarData));
         girder.SetAlternateTensStressDataProvider(pATSP);
      }
      else
      {
         girder.SetAlternateTensStressDataProvider(nullptr);
      }

      pDoc->SetGirder(GirderType::Prismatic,girder);
      pDoc->SetStrands(GirderType::Prismatic, ModelType::Lifting, lifting_strands);
      pDoc->SetStrands(GirderType::Prismatic, ModelType::Hauling, hauling_strands);
      pDoc->SetStrands(GirderType::Prismatic, ModelType::OneEndSeated, one_end_seated_strands);

      pDoc->SetDensity(density);
      pDoc->SetDensityWithRebar(densityWithRebar);

      pDoc->SetConcreteType(concrete_type);

      pDoc->SetK1(K1);
      pDoc->SetK2(K2);
   }

   if ( !pDX->m_bSaveAndValidate )
   {
      m_ctrlGirder.Invalidate();
      m_ctrlGirder.UpdateWindow();
      UpdateRebarControls();
   }
}


BEGIN_MESSAGE_MAP(CPGStablePrismaticGirder, CDialog)
   ON_BN_CLICKED(IDC_ADD, &CPGStablePrismaticGirder::OnAddPointLoad)
   ON_BN_CLICKED(IDC_REMOVE, &CPGStablePrismaticGirder::OnRemovePointLoad)
   ON_UPDATE_COMMAND_UI(IDC_REMOVE,&CPGStablePrismaticGirder::OnUpdateRemove)
	ON_MESSAGE(WM_HELP, OnCommandHelp)
   ON_CBN_SELCHANGE(IDC_PS_METHOD, &CPGStablePrismaticGirder::OnPSMethodChanged)
   ON_CBN_SELCHANGE(IDC_GIRDER_LIST, &CPGStablePrismaticGirder::OnGirderChanged)
   ON_CBN_SELCHANGE(IDC_MILD_STEEL_SELECTOR, &CPGStablePrismaticGirder::OnRebarMaterialChanged)
//   ON_WM_KILLFOCUS()
   ON_BN_CLICKED(IDC_COMPUTE_STRESS_POINTS, &CPGStablePrismaticGirder::OnBnClickedComputeStressPoints)
   ON_BN_CLICKED(IDC_DEFINE_STRESS_POINTS, &CPGStablePrismaticGirder::OnBnClickedDefineStressPoints)
   ON_BN_CLICKED(IDC_INSERT_REBAR_ROW, &CPGStablePrismaticGirder::OnBnClickedInsertRebarRow)
   ON_EN_CHANGE(IDC_HG, &CPGStablePrismaticGirder::OnChangedStressPointDimension)
   ON_EN_CHANGE(IDC_WTF, &CPGStablePrismaticGirder::OnChangedStressPointDimension)
   ON_EN_CHANGE(IDC_WBF, &CPGStablePrismaticGirder::OnChangedStressPointDimension)
   ON_EN_CHANGE(IDC_YTOP, &CPGStablePrismaticGirder::OnChangedStressPointDimension)
   ON_WM_SHOWWINDOW()
   ON_BN_CLICKED(IDC_REMOVE_REBAR_ROW, OnRemoveRebarRows)
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

   m_pPGStableLongRebarGrid = std::make_unique<CPGStableLongRebarGrid>();
   m_pPGStableLongRebarGrid->SubclassDlgItem(IDC_LONG_GRID, this);
   m_pPGStableLongRebarGrid->CustomInit();

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
   WBFL::Library::KeyListType keyList;
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

   const WBFL::Stability::Girder& girder = pDoc->GetGirder(GirderType::Prismatic);
   IndexType sectIdx = 0;
   Float64 Ag, Ixx, Iyy, Ixy, Xleft, Ytop, Hg, Wtf, Wbf;
   girder.GetSectionProperties(sectIdx, WBFL::Stability::Section::Start, &Ag, &Ixx, &Iyy, &Ixy, &Xleft, &Ytop, &Hg, &Wtf, &Wbf);
   Ytop *= -1; //this is in girder section coordinates, convert to Y positive down coordinates

   if (pDoc->GetStressPointType() == COMPUTE_STRESS_POINTS)
   {
      InitStressPoints(Wtf, Wbf, Ytop, Hg);
   }
   else
   {
      girder.GetStressPoints(sectIdx, WBFL::Stability::Section::Start, &m_pntTL, &m_pntTR, &m_pntBL, &m_pntBR);
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

   WBFL::Stability::Girder girder = pDoc->GetGirder(GirderType::Prismatic);
   DDX_Girder(&dx,girder);

   IndexType sectIdx = 0;
   Float64 Ag,Ixx,Iyy,Ixy,Xleft,Ytop,Hg,Wtf,Wbf;
   girder.GetSectionProperties(sectIdx,WBFL::Stability::Section::Start,&Ag,&Ixx,&Iyy,&Ixy,&Xleft,&Ytop,&Hg,&Wtf,&Wbf);

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

   WBFL::Stability::Girder girder = pDoc->GetGirder(GirderType::Prismatic);
   DDX_Girder(&dx,girder);

   CPGStableStrands strands = pDoc->GetStrands(GirderType::Nonprismatic,ModelType::Lifting);
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

      auto factory = pEntry->GetBeamFactory();

      auto dimensions = pEntry->GetDimensions();
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
      const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();
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
   UpdateRebarControls();
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
      const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();
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

void CPGStablePrismaticGirder::UpdateRebarControls()
{
   // Must use a girder library with a shape in order to model rebar
   CString strGirder;
   CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_GIRDER_LIST);
   int curSel = pCB->GetCurSel();
   pCB->GetLBText(curSel, strGirder);
   bool bShow = strGirder != gs_strGirder;

   CView* pParent = (CView*)GetParent();
   CPGStableDoc* pDoc = (CPGStableDoc*)pParent->GetDocument();

   bShow &= pDoc->GetGirderType() == Prismatic;

   int Show = bShow ? SW_SHOW : SW_HIDE;

   GetDlgItem(IDC_REINF_CB)->ShowWindow(Show);
   GetDlgItem(IDC_STATIC_REINFMAT)->ShowWindow(Show);
   GetDlgItem(IDC_MILD_STEEL_SELECTOR)->ShowWindow(Show);
   GetDlgItem(IDC_LONG_GRID)->ShowWindow(Show);
   GetDlgItem(IDC_INSERT_REBAR_ROW)->ShowWindow(Show);
   GetDlgItem(IDC_REMOVE_REBAR_ROW)->ShowWindow(Show);

   GetDlgItem(IDC_COVER_LIMIT)->ShowWindow(Show);
   GetDlgItem(IDC_COVER_LIMIT_UNIT)->ShowWindow(Show);
   GetDlgItem(IDC_COVER_LIMIT_TAG)->ShowWindow(Show);

   if (bShow)
   {
      // Move cover from spec entry into UI
      const SpecLibraryEntry* pSpec = pDoc->GetSpecLibraryEntry();
      BOOL bEnable = pSpec == nullptr ? TRUE : FALSE;
      if (!bEnable)
      {
         Float64 cover = pSpec->GetPrestressedElementCriteria().MaxCoverToUseHigherTensionStressLimit;

         CEAFApp* pApp = EAFGetApp();
         const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();
         CDataExchange DX(this, FALSE);
         DDX_UnitValueAndTag(&DX, IDC_COVER_LIMIT, IDC_COVER_LIMIT_UNIT, cover, pDispUnits->ComponentDim);
      }

      GetDlgItem(IDC_COVER_LIMIT)->EnableWindow(bEnable);
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

void CPGStablePrismaticGirder::OnBnClickedInsertRebarRow()
{
   WBFL::Materials::Rebar::Type type;
   WBFL::Materials::Rebar::Grade grade;
   CDataExchange dx(this, TRUE);
   DDX_RebarMaterial(&dx, IDC_MILD_STEEL_SELECTOR, type, grade);

   m_pPGStableLongRebarGrid->Insertrow(type, grade);
}

void CPGStablePrismaticGirder::OnRebarMaterialChanged()
{
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

BOOL CPGStablePrismaticGirder::IsDataValid()
{
   if (m_pPGStableLongRebarGrid)
   {
      return m_pPGStableLongRebarGrid->ValidateAllCells(FALSE);
   }

   return TRUE;
}

void CPGStablePrismaticGirder::OnRemoveRebarRows()
{
   m_pPGStableLongRebarGrid->Removerows();

   // selection is gone after row is deleted
   CWnd* pdel = GetDlgItem(IDC_REMOVE_REBAR_ROW);
   ASSERT(pdel);
   pdel->EnableWindow(FALSE);
}

void CPGStablePrismaticGirder::OnEnableDeleteRebarRow(bool canDelete)
{
   CWnd* pdel = GetDlgItem(IDC_REMOVE_REBAR_ROW);
   ASSERT(pdel);
   pdel->EnableWindow(canDelete);
}
