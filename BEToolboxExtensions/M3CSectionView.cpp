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
// M3CSectionView.cpp : implementation file
//

#include "stdafx.h"
#include "M3CSectionView.h"
#include "M3CDoc.h"
#include "BEToolboxColors.h"

static const COLORREF COLUMN_COLOR = GREY70;
static const COLORREF BONDED_REBAR_COLOR = GREEN;
static const COLORREF UNBONDED_REBAR_COLOR = PURPLE;
static const COLORREF TENSION_COLOR = BLUE;


#define COLUMN_DISPLAY_LIST 0
#define BONDED_REBAR_DISPLAY_LIST 1
#define UNBONDED_REBAR_DISPLAY_LIST 2
#define TENDON_DISPLAY_LIST 3

// CM3CSectionView

IMPLEMENT_DYNCREATE(CM3CSectionView, CDisplayView)

CM3CSectionView::CM3CSectionView()
{

}

CM3CSectionView::~CM3CSectionView()
{
}

BEGIN_MESSAGE_MAP(CM3CSectionView, CDisplayView)
   ON_WM_SIZE()
END_MESSAGE_MAP()

void CM3CSectionView::DoPrint(CDC* pDC, CPrintInfo* pInfo)
{
   OnBeginPrinting(pDC, pInfo, pInfo->m_rectDraw);
   OnPrepareDC(pDC);
   ScaleToFit();
   OnDraw(pDC);
   OnEndPrinting(pDC, pInfo);
}

void CM3CSectionView::BuildDisplayLists()
{
   CComPtr<iDisplayMgr> dispMgr;
   GetDisplayMgr(&dispMgr);

   CDisplayView::SetMappingMode(DManip::Isotropic);

   // Setup display lists
   CComPtr<iDisplayList> tendon_list;
   ::CoCreateInstance(CLSID_DisplayList, nullptr, CLSCTX_ALL, IID_iDisplayList, (void**)&tendon_list);
   tendon_list->SetID(TENDON_DISPLAY_LIST);
   dispMgr->AddDisplayList(tendon_list);

   CComPtr<iDisplayList> unbonded_rebar_list;
   ::CoCreateInstance(CLSID_DisplayList, nullptr, CLSCTX_ALL, IID_iDisplayList, (void**)&unbonded_rebar_list);
   unbonded_rebar_list->SetID(UNBONDED_REBAR_DISPLAY_LIST);
   dispMgr->AddDisplayList(unbonded_rebar_list);

   CComPtr<iDisplayList> bonded_rebar_list;
   ::CoCreateInstance(CLSID_DisplayList, nullptr, CLSCTX_ALL, IID_iDisplayList, (void**)&bonded_rebar_list);
   bonded_rebar_list->SetID(BONDED_REBAR_DISPLAY_LIST);
   dispMgr->AddDisplayList(bonded_rebar_list);

   CComPtr<iDisplayList> column_list;
   ::CoCreateInstance(CLSID_DisplayList, nullptr, CLSCTX_ALL, IID_iDisplayList, (void**)&column_list);
   column_list->SetID(COLUMN_DISPLAY_LIST);
   dispMgr->AddDisplayList(column_list);

   // OnInitialUpdate eventually calls OnUpdate... OnUpdate calls the
   // following two methods so there isn't any need to call them here
   //UpdateDisplayObjects();
   //UpdateDrawingScale();
}



void CM3CSectionView::OnInitialUpdate()
{
   EnableToolTips();
   BuildDisplayLists();
   CDisplayView::OnInitialUpdate();
   UpdateDisplayObjects();
   UpdateDrawingScale();
}

void CM3CSectionView::OnUpdate(CView* pView, LPARAM lHint, CObject* pHint)
{
   CDisplayView::OnUpdate(pView, lHint, pHint);
   UpdateDisplayObjects();
   UpdateDrawingScale();
}

void CM3CSectionView::UpdateDrawingScale()
{
   CDManipClientDC dc(this);
   ScaleToFit(false); // don't force a redraw
}

void CM3CSectionView::UpdateDisplayObjects()
{
   // get the display manager
   CComPtr<iDisplayMgr> dispMgr;
   GetDisplayMgr(&dispMgr);

   // clean out all the display objects
   dispMgr->ClearDisplayObjects();

   CComPtr<iDisplayList> dlColumn, dlBondedRebar, dlUnbondedRebar, dlTendon;
   dispMgr->FindDisplayList(COLUMN_DISPLAY_LIST, &dlColumn);
   dispMgr->FindDisplayList(BONDED_REBAR_DISPLAY_LIST, &dlBondedRebar);
   dispMgr->FindDisplayList(UNBONDED_REBAR_DISPLAY_LIST, &dlUnbondedRebar);
   dispMgr->FindDisplayList(TENDON_DISPLAY_LIST, &dlTendon);

   CM3CDoc* pDoc = (CM3CDoc*)GetDocument();

   CComPtr<IGeneralSection> section;
   pDoc->GetSection(&section);

   const auto& params = pDoc->GetProblemParameters();

   IndexType startColumnIdx = 0;
   IndexType endColumnIdx = 0;
   IndexType startBondedRebarIdx = INVALID_INDEX;
   IndexType endBondedRebarIdx = INVALID_INDEX;
   IndexType startUnbondedRebarIdx = INVALID_INDEX;
   IndexType endUnbondedRebarIdx = INVALID_INDEX;
   IndexType startTendonIdx = INVALID_INDEX;
   IndexType endTendonIdx = INVALID_INDEX;

   if (0 < params.Bonded_Rebar_nBars)
   {
      startBondedRebarIdx = endColumnIdx + 1;
      endBondedRebarIdx = startBondedRebarIdx + params.Bonded_Rebar_nBars - 1;
   }

   if (0 < params.Unbonded_Rebar_nBars)
   {
      startUnbondedRebarIdx = (endBondedRebarIdx == INVALID_INDEX ? endColumnIdx : endBondedRebarIdx) + 1;
      endUnbondedRebarIdx = startUnbondedRebarIdx + params.Unbonded_Rebar_nBars - 1;
   }

   if (0 < params.Tendon_nStrands)
   {
      startTendonIdx = (endUnbondedRebarIdx == INVALID_INDEX ? (endBondedRebarIdx == INVALID_INDEX ? endColumnIdx : endBondedRebarIdx) : endUnbondedRebarIdx) + 1;
   }


   CollectionIndexType nShapes;
   section->get_ShapeCount(&nShapes);
   for (CollectionIndexType shapeIdx = 0; shapeIdx < nShapes; shapeIdx++)
   {
      CComPtr<IShape> shape;
      section->get_Shape(shapeIdx, &shape);

      CComQIPtr<IGenericShape> generic_shape(shape);
      if (generic_shape)
      {
         CComPtr<IPoint2d> pnt;
         generic_shape->get_Centroid(&pnt);

         // assume shape can be modeled with a circle
         Float64 radius;
         Float64 P;
         shape->get_Perimeter(&P);
         if (IsZero(P))
         {
            // A = PI(radius)^2
            Float64 A;
            generic_shape->get_Area(&A);
            radius = sqrt(A / M_PI);
         }
         else
         {
            // P = 2*PI*radius
            radius = P / TWO_PI;
         }

         if (!IsZero(radius))
         {
            CComPtr<ICircle> circle;
            circle.CoCreateInstance(CLSID_Circle);
            circle->put_Radius(radius);
            circle->putref_Center(pnt);

            shape.Release();
            circle.QueryInterface(&shape);
         }
      }

      CComPtr<iPointDisplayObject> doPnt;
      ::CoCreateInstance(CLSID_PointDisplayObject, nullptr, CLSCTX_ALL, IID_iPointDisplayObject, (void**)&doPnt);
      doPnt->SetID(shapeIdx);

      CComPtr<iShapeDrawStrategy> strategy;
      ::CoCreateInstance(CLSID_ShapeDrawStrategy, nullptr, CLSCTX_ALL, IID_iShapeDrawStrategy, (void**)&strategy);
      doPnt->SetDrawingStrategy(strategy);

      strategy->SetShape(shape);
      strategy->DoFill(true);

      if (startColumnIdx <= shapeIdx && shapeIdx <= endColumnIdx )
      {
         strategy->SetSolidLineColor(COLUMN_COLOR);
         strategy->SetSolidFillColor(COLUMN_COLOR);
         dlColumn->AddDisplayObject(doPnt);
      }
      else if (startBondedRebarIdx <= shapeIdx && shapeIdx <= endBondedRebarIdx)
      {
         strategy->SetSolidLineColor(BONDED_REBAR_COLOR);
         strategy->SetSolidFillColor(BONDED_REBAR_COLOR);
         dlBondedRebar->AddDisplayObject(doPnt);
      }
      else if (startUnbondedRebarIdx <= shapeIdx && shapeIdx <= endUnbondedRebarIdx)
      {
         strategy->SetSolidLineColor(UNBONDED_REBAR_COLOR);
         strategy->SetSolidFillColor(UNBONDED_REBAR_COLOR);
         dlUnbondedRebar->AddDisplayObject(doPnt);
      }
      else if (startTendonIdx <= shapeIdx && shapeIdx <= endTendonIdx)
      {
         strategy->SetSolidLineColor(TENSION_COLOR);
         strategy->SetSolidFillColor(TENSION_COLOR);
         dlTendon->AddDisplayObject(doPnt);
      }
      else
      {
         ATLASSERT(false);
      }
   }
}

// CM3CSectionView diagnostics

#ifdef _DEBUG
void CM3CSectionView::AssertValid() const
{
   CDisplayView::AssertValid();
}

#ifndef _WIN32_WCE
void CM3CSectionView::Dump(CDumpContext& dc) const
{
   CDisplayView::Dump(dc);
}
#endif
#endif //_DEBUG

// CM3CSectionView message handlers


void CM3CSectionView::OnSize(UINT nType, int cx, int cy)
{
   CDisplayView::OnSize(nType, cx, cy);

   CRect rect;
   GetClientRect(&rect);
   rect.DeflateRect(5, 5, 5, 5);

   CSize size = rect.Size();
   size.cx = Max(0L, size.cx);
   size.cy = Max(0L, size.cy);

   SetLogicalViewRect(MM_TEXT, rect);

   SetScrollSizes(MM_TEXT, size, CScrollView::sizeDefault, CScrollView::sizeDefault);

   UpdateDrawingScale();
   Invalidate();
   UpdateWindow();
}
