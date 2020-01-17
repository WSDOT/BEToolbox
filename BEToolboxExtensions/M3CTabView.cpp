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

#include "stdafx.h"
#include "resource.h"
#include "M3CDoc.h"
#include "M3CTabView.h"
#include "M3CChildFrame.h"
#include "M3CMainView.h"
#include "M3CReportView.h"

#include <EAF\EAFHints.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CM3CTabView

IMPLEMENT_DYNCREATE(CM3CTabView, CTabView)

CM3CTabView::CM3CTabView()
{
}

CM3CTabView::~CM3CTabView()
{
}

BEGIN_MESSAGE_MAP(CM3CTabView, CTabView)
	ON_WM_CREATE()
END_MESSAGE_MAP()

#ifdef _DEBUG
void CM3CTabView::AssertValid() const
{
   //AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CTabView::AssertValid();
}

#ifndef _WIN32_WCE
void CM3CTabView::Dump(CDumpContext& dc) const
{
	CTabView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPGStableTabView message handlers

int CM3CTabView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   int result = CTabView::OnCreate(lpCreateStruct);

   CMFCTabCtrl& tabCtrl = GetTabControl();
   tabCtrl.SetActiveTabBoldFont();

   AddView(RUNTIME_CLASS(CM3CMainView),  _T("Column Properties")   );
   AddView(RUNTIME_CLASS(CM3CAnalysisReportView), _T("Moment-Curvature"));
   AddView(RUNTIME_CLASS(CM3CMaterialDetailsReportView), _T("Material Model Details"));
   AddView(RUNTIME_CLASS(CM3CAnalysisDetailsReportView), _T("Analysis Details"));

   return result;
}

CM3CMainView* CM3CTabView::GetMainView()
{
   CView* pView = GetActiveView();
   int idx = FindTab(pView->GetSafeHwnd());
   SetActiveView(0);
   CView* pMainView = GetActiveView();
   ASSERT_KINDOF(CM3CMainView, pMainView);
   SetActiveView(idx);
   return (CM3CMainView*)pMainView;
}

void CM3CTabView::OnInitialUpdate()
{
   CTabView::OnInitialUpdate();

   CM3CDoc* pDoc = (CM3CDoc*)GetDocument();
   CM3CChildFrame* pFrame = (CM3CChildFrame*)GetParent();
   pFrame->SetProblemParameters(pDoc->GetProblemParameters());
}

void CM3CTabView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* /*pHint*/)
{
   // TODO: Add your specialized code here and/or call the base class
   Invalidate();
   UpdateWindow();

   CView* pView = GetActiveView();
   if ( pView )
   {
      pView->Invalidate();
      pView->UpdateWindow();
   }

   if (lHint == EAF_HINT_UNITS_CHANGING)
   {
      CM3CChildFrame* pFrame = (CM3CChildFrame*)GetParent();
      pFrame->OnUnitsChanging();
   }
   else if (lHint == EAF_HINT_UNITS_CHANGED)
   {
      CM3CChildFrame* pFrame = (CM3CChildFrame*)GetParent();
      pFrame->OnUnitsChanged();
   }
}
