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

#include "stdafx.h"
#include "..\resource.h"
#include "GirCompChildFrame.h"
#include "GirCompDoc.h"

IMPLEMENT_DYNCREATE(CGirCompChildFrame, CEAFChildFrame)

CGirCompChildFrame::CGirCompChildFrame(void)
{
}

CGirCompChildFrame::~CGirCompChildFrame(void)
{
}

BEGIN_MESSAGE_MAP(CGirCompChildFrame, CEAFChildFrame)
   ON_WM_CREATE()
   ON_BN_CLICKED(IDC_UPDATE,OnUpdate)
   ON_BN_CLICKED(IDC_US,OnUSUnits)
   ON_BN_CLICKED(IDC_SI,OnSIUnits)
   ON_BN_CLICKED(IDC_ADD,OnAdd)
   ON_BN_CLICKED(IDC_REMOVE,OnRemove)
   ON_UPDATE_COMMAND_UI(IDC_REMOVE,OnUpdateRemove)
	ON_MESSAGE(WM_HELP, OnCommandHelp)
END_MESSAGE_MAP()

LRESULT CGirCompChildFrame::OnCommandHelp(WPARAM, LPARAM lParam)
{
   EAFHelp( EAFGetDocument()->GetDocumentationSetName(), IDH_GIRCOMP );
   return TRUE;
}

BOOL CGirCompChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
   // force this window to be maximized (not sure why WS_VISIBLE is required)
   cs.style |= WS_MAXIMIZE | WS_VISIBLE;
   
   // I have no idea why, but creating this window fail the second time a BEToolbox document
   // is created in a single BridgeLink run. For some reason, MFC determines the wrong
   // class name. lpszClass is nullptr the first time this window is created successfully, so
   // we'll force it to nullptr here so it works on subsequent creation attempts
   cs.lpszClass = nullptr;

   return CEAFChildFrame::PreCreateWindow(cs);
}

int CGirCompChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CEAFChildFrame::OnCreate(lpCreateStruct) == -1)
      return -1;

   {
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   if (!m_LeftDlgBar.Create(this,IDD_GIRCOMP_LEFT,CBRS_LEFT | CBRS_TOOLTIPS | CBRS_FLYBY, AFX_IDW_CONTROLBAR_FIRST))
   {
      TRACE0("Failed to create dialog bar\n");
      return -1;
   }

   if (!m_TopDlgBar.Create(this,IDD_GIRCOMP_TOP,CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY, AFX_IDW_CONTROLBAR_FIRST+1))
   {
      TRACE0("Failed to create dialog bar\n");
      return -1;
   }

   EnableToolTips();
   m_TopDlgBar.EnableToolTips();

   SetIcon(AfxGetApp()->LoadIcon(IDR_GIRCOMP),TRUE);
   }

   return 0;
}

void CGirCompChildFrame::OnUpdate()
{
   m_TopDlgBar.UpdateData(TRUE);
   m_LeftDlgBar.UpdateData(TRUE);

   CGirCompDoc* pDoc = (CGirCompDoc*)GetActiveDocument();
   if ( pDoc )
   {
      pDoc->SetModifiedFlag();
      pDoc->UpdateAllViews(nullptr);
   }
}

void CGirCompChildFrame::OnUSUnits()
{
   ASSERT(m_TopDlgBar.IsDlgButtonChecked(IDC_US) == 1 );
   SetUnitsMode(WBFL::EAF::UnitMode::US);
}

void CGirCompChildFrame::OnSIUnits()
{
   ASSERT(m_TopDlgBar.IsDlgButtonChecked(IDC_SI) == 1 );
   SetUnitsMode(WBFL::EAF::UnitMode::SI);
}

void CGirCompChildFrame::OnAdd()
{
   m_TopDlgBar.AddProblem();
}

void CGirCompChildFrame::OnRemove()
{
   m_TopDlgBar.RemoveSelectedProblems();
}

void CGirCompChildFrame::OnUpdateRemove(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(m_TopDlgBar.AreProblemsSelected() ? TRUE : FALSE);
}

void CGirCompChildFrame::SetUnitsMode(WBFL::EAF::UnitMode um)
{
   CEAFApp* pApp = EAFGetApp();
   if ( pApp->GetUnitsMode() != um )
   {
      UpdateData(TRUE); // update the data
      // before changing the units mode
      pApp->SetUnitsMode(um);
      UpdateUnitsDisplayMode();

      CGirCompDoc* pDoc = (CGirCompDoc*)GetActiveDocument();
      if ( pDoc )
      {
         pDoc->SetModifiedFlag();
      }
   }
}

void CGirCompChildFrame::UpdateUnitsDisplayMode()
{
   m_TopDlgBar.OnUnitsModeChanged();
}

#if defined _DEBUG
void CGirCompChildFrame::AssertValid() const
{
   CEAFChildFrame::AssertValid();
}
#endif

BOOL CGirCompChildFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle , CWnd* pParentWnd , CCreateContext* pContext)
{
   // CEAFChildFrame::LoadFrame is going to load resources based on nIDResource
   // We have to set the module context to this module so the load happens
   // correctly
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   if ( !CEAFChildFrame::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext) )
      return FALSE;

   return TRUE;
}

void CGirCompChildFrame::UpdateData(BOOL bUpdate)
{
   m_TopDlgBar.UpdateData(bUpdate);
   m_LeftDlgBar.UpdateData(bUpdate);
}