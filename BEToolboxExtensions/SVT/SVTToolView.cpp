///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2019  Washington State Department of Transportation
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

// SVTToolView.cpp : implementation file
//

#include "stdafx.h"
#include "SVTToolView.h"
#include "SVTToolDoc.h"

#include <EAF\EAFHints.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CSVTToolView

IMPLEMENT_DYNCREATE(CSVTToolView, CDisplayView)

CSVTToolView::CSVTToolView()
{

}

CSVTToolView::~CSVTToolView()
{
}

BEGIN_MESSAGE_MAP(CSVTToolView, CDisplayView)
   //ON_COMMAND(ID_FILE_PRINT_DIRECT,&CSVTToolView::OnFilePrint)
END_MESSAGE_MAP()

// CSVTToolView diagnostics

#ifdef _DEBUG
void CSVTToolView::AssertValid() const
{
   CDisplayView::AssertValid();
}

#ifndef _WIN32_WCE
void CSVTToolView::Dump(CDumpContext& dc) const
{
   CDisplayView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSVTToolView message handlers
#define DISPLAY_LIST_ID 100
void CSVTToolView::OnInitialUpdate()
{
   CSVTToolDoc* pDoc = (CSVTToolDoc*)GetDocument();

   m_pFrame = (CSVTToolChildFrame*)GetParentFrame();

   CComPtr<iDisplayMgr> dispMgr;
   GetDisplayMgr(&dispMgr);

   SetMappingMode(DManip::Isotropic);

   CComPtr<iDisplayList> dl;
   ::CoCreateInstance(CLSID_DisplayList, nullptr, CLSCTX_ALL, IID_iDisplayList, (void**)&dl);
   dl->SetID(DISPLAY_LIST_ID);
   dispMgr->AddDisplayList(dl);

   CDisplayView::OnInitialUpdate();
}

void CSVTToolView::OnUpdate(CView* pSender,LPARAM lHint,CObject* pHint)
{
   CDisplayView::OnUpdate(pSender, lHint, pHint);

   CComPtr<iDisplayMgr> dispMgr;
   GetDisplayMgr(&dispMgr);

   CDManipClientDC dc(this);

   CSVTToolDoc* pDoc = (CSVTToolDoc*)GetDocument();
   CComPtr<IShape> pShape;
   pDoc->GetShape(&pShape);

   dispMgr->ClearDisplayObjects();

   CComPtr<iDisplayList> display_list;
   dispMgr->FindDisplayList(DISPLAY_LIST_ID, &display_list);
   display_list->Clear();

   CComPtr<iPointDisplayObject> dispObj;
   dispObj.CoCreateInstance(CLSID_PointDisplayObject);

   CComPtr<iCompoundDrawPointStrategy> compound_strategy;
   compound_strategy.CoCreateInstance(CLSID_CompoundDrawPointStrategy);


   // the main girder shape
   CComPtr<iShapeDrawStrategy> shape_draw_strategy;
   shape_draw_strategy.CoCreateInstance(CLSID_ShapeDrawStrategy);
   shape_draw_strategy->SetShape(pShape);
   shape_draw_strategy->SetSolidLineColor(BLUE);
   compound_strategy->AddStrategy(shape_draw_strategy);

   // the finite difference grid
   std::vector<CComPtr<IRectangle>> vMesh = pDoc->GetMesh();
   for (auto mesh_element : vMesh)
   {
      CComQIPtr<IShape> shape(mesh_element);
      CComPtr<iShapeDrawStrategy> shape_draw_strategy;
      shape_draw_strategy.CoCreateInstance(CLSID_ShapeDrawStrategy);
      shape_draw_strategy->SetShape(shape);
      compound_strategy->AddStrategy(shape_draw_strategy);
   }


   dispObj->SetDrawingStrategy(compound_strategy);

   display_list->AddDisplayObject(dispObj);

   ScaleToFit();
}
