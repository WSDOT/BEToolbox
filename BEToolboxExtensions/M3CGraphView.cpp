///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2018  Washington State Department of Transportation
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
// M3CGraphView.cpp : implementation file
//

#include "stdafx.h"
#include "M3CGraphView.h"

#include <EAF\EAFApp.h>
#include <GraphicsLib\GraphicsLib.h>
#include <algorithm>
#include "M3CDoc.h"

#include "BEToolboxColors.h"

// CM3CView

IMPLEMENT_DYNAMIC(CM3CGraphView, CView)

CM3CGraphView::CM3CGraphView()
{
   m_bIsPrinting = false;

   m_Scalar.Width = 8;
   m_Scalar.Precision = 0;
   m_Scalar.Format = sysNumericFormatTool::Fixed;
}

CM3CGraphView::~CM3CGraphView()
{
}

BEGIN_MESSAGE_MAP(CM3CGraphView, CView)
END_MESSAGE_MAP()

// CM3CMainView drawing

void CM3CGraphView::DoPrint(CDC* pDC, CPrintInfo* pInfo)
{
   m_bIsPrinting = true;
   m_PrintingRect = pInfo->m_rectDraw;
   OnBeginPrinting(pDC, pInfo);
   OnPrepareDC(pDC);
   OnDraw(pDC);
   OnEndPrinting(pDC, pInfo);
   m_bIsPrinting = false;
}

CRect CM3CGraphView::GetDrawingRect()
{
   if (m_bIsPrinting)
   {
      return m_PrintingRect;
   }
   else
   {
      CRect rect;
      GetClientRect(&rect);
      return rect;
   }
}

void CM3CGraphView::OnDraw(CDC* pDC)
{
	CM3CDoc* pDoc = (CM3CDoc*)GetDocument();
	// TODO: add draw code here

   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   std::unique_ptr<arvPhysicalConverter> pStrainFormat = std::make_unique<ScalarTool>(m_Scalar);
   std::unique_ptr<arvPhysicalConverter> pStressFormat = std::make_unique<StressTool>(pDispUnits->Stress);
   grGraphXY graph(*pStrainFormat, *pStressFormat);

   graph.SetTitle(GetGraphTitle());
   graph.SetSubtitle(GetGraphSubtitle());
   graph.SetXAxisTitle(_T("Strain x 1000"));
   CString strYAxisTitle;
   strYAxisTitle.Format(_T("Stress (%s)"), pDispUnits->Stress.UnitOfMeasure.UnitTag().c_str());
   graph.SetYAxisTitle(strYAxisTitle);

   graph.SetClientAreaColor(GRAPH_BACKGROUND);
   graph.SetGridPenStyle(GRAPH_GRID_PEN_STYLE, GRAPH_GRID_PEN_WEIGHT, GRAPH_GRID_COLOR);

   CComPtr<IStressStrain> ss;
   GetMaterialModel(&ss);

   Float64 minStrain, maxStrain;
   ss->StrainLimits(&minStrain, &maxStrain);
   if (maxStrain == DBL_MAX)
      maxStrain = 0;

   std::vector<Float64> vStrain = linspace(minStrain, maxStrain, 30);
   std::vector<Float64> vSpecial = GetSpecialPoints();
   vStrain.insert(std::end(vStrain), std::begin(vSpecial), std::end(vSpecial));
   std::sort(std::begin(vStrain), std::end(vStrain));
   vStrain.erase(std::unique(std::begin(vStrain), std::end(vStrain)), std::end(vStrain));

   Float64 signX, signY;
   GetSign(&signX, &signY);
   
   IndexType idx = graph.CreateDataSeries(_T(""),PS_SOLID,1,BLUE);
   for ( auto strain : vStrain)
   {
      Float64 stress;
      ss->ComputeStress(strain, &stress);
      stress = ::ConvertFromSysUnits(stress, pDispUnits->Stress.UnitOfMeasure);
      gpPoint2d point(signX*strain*1000, signY*stress);
      graph.AddPoint(idx, point);
   }


   CRect rect = GetDrawingRect();
   graph.SetOutputRect(rect);
   graph.Draw(pDC->GetSafeHdc());
}


// CM3CGraphView diagnostics

#ifdef _DEBUG
void CM3CGraphView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CM3CGraphView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG

// CM3CView message handlers



IMPLEMENT_DYNCREATE(CM3CConcreteGraphView, CM3CGraphView)
void CM3CConcreteGraphView::GetMaterialModel(IStressStrain** ppMaterial)
{
   CM3CDoc* pDoc = (CM3CDoc*)GetDocument();
   pDoc->GetConcreteModel(ppMaterial);
}


IMPLEMENT_DYNCREATE(CM3CRebarGraphView, CM3CGraphView)
void CM3CRebarGraphView::GetMaterialModel(IStressStrain** ppMaterial)
{
   CM3CDoc* pDoc = (CM3CDoc*)GetDocument();
   pDoc->GetRebarModel(ppMaterial);
}

std::vector<Float64> CM3CRebarGraphView::GetSpecialPoints()
{
   std::vector<Float64> vStrains;
   vStrains.reserve(3);

   CComPtr<IStressStrain> ss;
   GetMaterialModel(&ss);
   CComQIPtr<IStrainHardenedRebarModel> rebar(ss);
   Float64 fy, Es;
   rebar->get_fy(&fy);
   rebar->get_Es(&Es);
   vStrains.push_back(fy / Es);
   vStrains.push_back(-fy / Es);

   Float64 e;
   rebar->get_esh(&e);
   vStrains.push_back(e);
   vStrains.push_back(-e);

   rebar->get_efr(&e);
   vStrains.push_back(e);
   vStrains.push_back(-e);

   return vStrains;
}


IMPLEMENT_DYNCREATE(CM3CStrandGraphView, CM3CGraphView)
void CM3CStrandGraphView::GetMaterialModel(IStressStrain** ppMaterial)
{
   CM3CDoc* pDoc = (CM3CDoc*)GetDocument();
   pDoc->GetStrandModel(ppMaterial);
}
