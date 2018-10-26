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

#include "stdafx.h"
#include "resource.h"
#include "PGStableDoc.h"
#include "PGStableTabView.h"
#include "PGStableChildFrame.h"

#include "PGStableGirderView.h"
#include "PGStableLiftingView.h"
#include "PGStableHaulingView.h"

// CPGStableView

IMPLEMENT_DYNCREATE(CPGStableTabView, CTabView)

CPGStableTabView::CPGStableTabView()
{
   m_pLastView = nullptr;
}

CPGStableTabView::~CPGStableTabView()
{
}

BEGIN_MESSAGE_MAP(CPGStableTabView, CTabView)
	ON_WM_CREATE()
END_MESSAGE_MAP()

#ifdef _DEBUG
void CPGStableTabView::AssertValid() const
{
   //AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CTabView::AssertValid();
}

#ifndef _WIN32_WCE
void CPGStableTabView::Dump(CDumpContext& dc) const
{
	CTabView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPGStableTabView message handlers

int CPGStableTabView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   int result = CTabView::OnCreate(lpCreateStruct);

   CMFCTabCtrl& tabCtrl = GetTabControl();
   tabCtrl.SetActiveTabBoldFont();


   // NOTE: One assert for each tab control is going to fire, plus one additional assert
   // These asserts are all related to the module state when the tab control does its
   // self validation. We have to set the modulue state to the AfxGetStaticModuleState
   // to create the form view controls we are using on the tabs. However the
   // tab control must be created in the AfxGetAppModuleState state. Because there
   // isn't any virtual functions we can intercept, we can't change to the app module state
   // before the tab control self-validates. This causes the assert. The only way
   // I can find to get around this issue it to create a complete copy of CTabView.
   // Doing so doesn't seem worth the effort.
   AddView(RUNTIME_CLASS(CPGStableGirderView),  _T("Girder")   );
   AddView(RUNTIME_CLASS(CPGStableLiftingView), _T("Lifting")  );
   AddView(RUNTIME_CLASS(CPGStableHaulingView), _T("Hauling")  );

   return result;
}

void CPGStableTabView::OnInitialUpdate()
{
   CTabView::OnInitialUpdate();
}

void CPGStableTabView::OnActivateView(CView* pView)
{
   if ( m_pLastView )
   {
      m_pLastView->OnDeactivateView();
   }

   if ( pView && pView->IsKindOf(RUNTIME_CLASS(CPGStableFormView)) )
   {
      CPGStableFormView* pPGSView = (CPGStableFormView*)pView;
      pPGSView->OnActivateView();
      m_pLastView = pPGSView;
   }
}

void CPGStableTabView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
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
}
