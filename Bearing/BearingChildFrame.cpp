///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2023  Washington State Department of Transportation
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
#include "BearingChildFrame.h"
#include "BearingDoc.h"
#include <BEToolbox.hh>
#include <EAF/EAFHelp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CBearingChildFrame, CEAFChildFrame)

CBearingChildFrame::CBearingChildFrame(void)
{
}

CBearingChildFrame::~CBearingChildFrame(void)
{
}

BEGIN_MESSAGE_MAP(CBearingChildFrame, CEAFChildFrame)
   ON_WM_CREATE()
   ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
	ON_MESSAGE(WM_HELP, OnCommandHelp)
END_MESSAGE_MAP()

LRESULT CBearingChildFrame::OnCommandHelp(WPARAM, LPARAM lParam)
{
   EAFHelp( EAFGetDocument()->GetDocumentationSetName(), IDH_BEARING );
   return TRUE;
}


const CBearingDialogBar& CBearingChildFrame::GetDialogBar() const
{
    return m_DlgBar;
}


void CBearingChildFrame::SetBearingParameters(const WBFL::EngTools::Bearing& brg,
    const WBFL::EngTools::BearingLoads& brg_loads)
{
    m_DlgBar.SetBearingParameters(m_DlgBar,brg,brg_loads);
    m_DlgBar.UpdateData(FALSE);
}






BOOL CBearingChildFrame::PreCreateWindow(CREATESTRUCT& cs)
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

int CBearingChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CEAFChildFrame::OnCreate(lpCreateStruct) == -1)
      return -1;

   {
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   if (!m_DlgBar.Create(this,IDD_BEARING,CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY, AFX_IDW_CONTROLBAR_FIRST))
   {
      TRACE0("Failed to create dialog bar\n");
      return -1;
   }

   EnableToolTips();
   m_DlgBar.EnableToolTips();

   SetIcon(AfxGetApp()->LoadIcon(IDR_BEARING),TRUE);
   }

   return 0;
}

void CBearingChildFrame::OnUpdate()
{
   m_DlgBar.UpdateData(TRUE);

   CBearingDoc* pDoc = (CBearingDoc*)GetActiveDocument();
   if ( pDoc )
   {
       WBFL::EngTools::Bearing brg;
       WBFL::EngTools::BearingLoads brg_loads;
       m_DlgBar.SetBearing(brg,brg_loads);
       pDoc->SetBearing(brg, brg_loads);
       pDoc->SetModifiedFlag();
       pDoc->UpdateAllViews(nullptr);
   }

   AfxMessageBox(_T("Dummy Dialog"));
}



void CBearingChildFrame::OnUSUnits()
{
    ASSERT(m_DlgBar.IsDlgButtonChecked(IDC_US) == TRUE);
    m_DlgBar.UpdateData(TRUE);
    SetUnitsMode(eafTypes::umUS);
    m_DlgBar.UpdateData(FALSE);
    OnUpdate();
}

void CBearingChildFrame::OnSIUnits()
{
    ASSERT(m_DlgBar.IsDlgButtonChecked(IDC_SI) == TRUE);
    m_DlgBar.UpdateData(TRUE);
    SetUnitsMode(eafTypes::umSI);
    m_DlgBar.UpdateData(FALSE);
    OnUpdate();
}

void CBearingChildFrame::SetUnitsMode(eafTypes::UnitMode um)
{
    CEAFApp* pApp = EAFGetApp();
    pApp->SetUnitsMode(um);
}





#if defined _DEBUG
void CBearingChildFrame::AssertValid() const
{
   CEAFChildFrame::AssertValid();
}
#endif

BOOL CBearingChildFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle , CWnd* pParentWnd , CCreateContext* pContext)
{
   // CEAFChildFrame::LoadFrame is going to load resources based on nIDResource
   // We have to set the module context to this module so the load happens
   // correctly
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   if ( !CEAFChildFrame::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext) )
   {
      return FALSE;
   }

   return TRUE;
}

void CBearingChildFrame::UpdateData(BOOL bUpdate)
{
   m_DlgBar.UpdateData(bUpdate);
}
