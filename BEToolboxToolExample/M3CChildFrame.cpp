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

#include "StdAfx.h"
#include "resource.h"
#include "M3CChildFrame.h"
#include "M3CDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CM3CChildFrame, CEAFChildFrame)

CM3CChildFrame::CM3CChildFrame(void)
{
}

CM3CChildFrame::~CM3CChildFrame(void)
{
}

BEGIN_MESSAGE_MAP(CM3CChildFrame, CEAFChildFrame)
   ON_WM_CREATE()
   ON_BN_CLICKED(IDC_UPDATE,OnUpdate)
   ON_BN_CLICKED(IDC_US,OnUSUnits)
   ON_BN_CLICKED(IDC_SI,OnSIUnits)
   ON_NOTIFY_EX(TTN_NEEDTEXT,0,OnToolTipNotify)
	ON_MESSAGE(WM_HELP, OnCommandHelp)
END_MESSAGE_MAP()

LRESULT CM3CChildFrame::OnCommandHelp(WPARAM, LPARAM lParam)
{
   //EAFHelp( EAFGetDocument()->GetDocumentationSetName(), IDH_M3C );
   return TRUE;
}

void CM3CChildFrame::SetProblemParameters(const CM3CProblemParameters& params)
{
   m_DlgBar.m_ProblemParams = params;
   m_DlgBar.UpdateData(FALSE);
}

BOOL CM3CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
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

int CM3CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CEAFChildFrame::OnCreate(lpCreateStruct) == -1)
      return -1;

   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   if (!m_DlgBar.Create(this,IDD_M3C,CBRS_LEFT | CBRS_TOOLTIPS | CBRS_FLYBY, AFX_IDW_CONTROLBAR_FIRST))
   {
      TRACE0("Failed to create dialog bar\n");
      return -1;
   }

   EnableToolTips();
   m_DlgBar.EnableToolTips();

   SetIcon(AfxGetApp()->LoadIcon(IDR_M3C),TRUE);


   return 0;
}

void CM3CChildFrame::OnUpdate()
{
   CM3CDoc* pDoc = (CM3CDoc*)GetActiveDocument();
   if ( pDoc )
   {
      m_DlgBar.UpdateData(TRUE);
      pDoc->SetProblemParameters(m_DlgBar.m_ProblemParams);
   }
}

void CM3CChildFrame::OnUSUnits()
{
   ASSERT(m_DlgBar.IsDlgButtonChecked(IDC_US) == 1 );
   m_DlgBar.UpdateData(TRUE);
   SetUnitsMode(eafTypes::umUS);
   m_DlgBar.UpdateData(FALSE);
   OnUpdate();
}

void CM3CChildFrame::OnSIUnits()
{
   ASSERT(m_DlgBar.IsDlgButtonChecked(IDC_SI) == 1 );
   m_DlgBar.UpdateData(TRUE);
   SetUnitsMode(eafTypes::umSI);
   m_DlgBar.UpdateData(FALSE);
   OnUpdate();
}

void CM3CChildFrame::SetUnitsMode(eafTypes::UnitMode um)
{
   CEAFApp* pApp = EAFGetApp();
   pApp->SetUnitsMode(um);
}

BOOL CM3CChildFrame::OnToolTipNotify(UINT id,NMHDR* pNMHDR, LRESULT* pResult)
{
   TOOLTIPTEXT* pTTT = (TOOLTIPTEXT*)pNMHDR;
   HWND hwndTool = (HWND)pNMHDR->idFrom;
   if ( pTTT->uFlags & TTF_IDISHWND )
   {
      // idFrom is actually HWND of tool
      UINT nID = ::GetDlgCtrlID(hwndTool);
      switch(nID)
      {
      case IDC_DIAMETER:
         _tcscpy_s(pTTT->szText,sizeof(pTTT->szText)/sizeof(TCHAR),_T("Column Diameter"));
         break;

      case IDC_FC:
         _tcscpy_s(pTTT->szText,sizeof(pTTT->szText)/sizeof(TCHAR),_T("Concrete Strength"));
         break;

      case IDC_FY:
         _tcscpy_s(pTTT->szText,sizeof(pTTT->szText)/sizeof(TCHAR),_T("Yield Strengh of Reinforcement"));
         break;

      case IDC_ES:
         _tcscpy_s(pTTT->szText,sizeof(pTTT->szText)/sizeof(TCHAR),_T("Moduls of Elasticity of Reinforcement"));
         break;

      case IDC_AS:
         _tcscpy_s(pTTT->szText,sizeof(pTTT->szText)/sizeof(TCHAR),_T("Total Area of Reinforcement"));
         break;

      case IDC_COVER:
         _tcscpy_s(pTTT->szText,sizeof(pTTT->szText)/sizeof(TCHAR),_T("Cover to Center of Reinforcement"));
         break;

      case IDC_ECL:
         _tcscpy_s(pTTT->szText,sizeof(pTTT->szText)/sizeof(TCHAR),_T("Compression Strain Limit"));
         break;

      case IDC_ETL:
         _tcscpy_s(pTTT->szText,sizeof(pTTT->szText)/sizeof(TCHAR),_T("Tension Strain Limit"));
         break;

      case IDC_US:
         _tcscpy_s(pTTT->szText,sizeof(pTTT->szText)/sizeof(TCHAR),_T("Display US Units"));
         break;

      case IDC_SI:
         _tcscpy_s(pTTT->szText,sizeof(pTTT->szText)/sizeof(TCHAR),_T("Display SI Units"));
         break;

      default:
         return FALSE;
      }

      return TRUE;
   }
   return FALSE;
}

#if defined _DEBUG
void CM3CChildFrame::AssertValid() const
{
   CEAFChildFrame::AssertValid();
}
#endif

BOOL CM3CChildFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle , CWnd* pParentWnd , CCreateContext* pContext)
{
   // CEAFChildFrame::LoadFrame is going to load resources based on nIDResource
   // We have to set the module context to this module so the load happens
   // correctly
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return CEAFChildFrame::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext);
}
