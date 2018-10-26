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

#include "M3CMainView.h"
#include "M3CSectioNView.h"

#include "M3CPrintJob.h"

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
   ON_BN_CLICKED(IDC_UPDATE,&CM3CChildFrame::OnUpdate)
   ON_NOTIFY_EX(TTN_NEEDTEXT,0,OnToolTipNotify)
	ON_MESSAGE(WM_HELP, OnCommandHelp)
   ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
   ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrintDirect)
END_MESSAGE_MAP()

LRESULT CM3CChildFrame::OnCommandHelp(WPARAM, LPARAM lParam)
{
   //EAFHelp( EAFGetDocument()->GetDocumentationSetName(), IDH_M3C );
   return TRUE;
}

void CM3CChildFrame::OnFilePrintDirect()
{
   DoFilePrint(true);
}

void CM3CChildFrame::OnFilePrint()
{
   DoFilePrint(false);
}

void CM3CChildFrame::DoFilePrint(bool bPrintDirect)
{
   CM3CPrintJob pj(this);
   pj.OnFilePrint(bPrintDirect);
}

void CM3CChildFrame::OnUnitsChanging()
{
   m_DlgBar.UpdateData(TRUE);
}

void CM3CChildFrame::OnUnitsChanged()
{
   m_DlgBar.UpdateData(FALSE);
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
   cs.style &= ~WS_MAXIMIZEBOX; // we don't want a maximize box
   cs.style &= ~WS_MINIMIZEBOX; // we don't want a minimize box
   cs.style &= ~WS_SYSMENU; // we don't want a system menu
   
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
         _tcscpy_s(pTTT->szText,sizeof(pTTT->szText)/sizeof(TCHAR),_T("Column diameter"));
         break;

      case IDC_FCO:
         _tcscpy_s(pTTT->szText, sizeof(pTTT->szText) / sizeof(TCHAR), _T("Unconfined concrete strength"));
         break;

      case IDC_ECO:
         _tcscpy_s(pTTT->szText, sizeof(pTTT->szText) / sizeof(TCHAR), _T("Crushing strain of unconfined concrete"));
         break;

      case IDC_ASPIRAL:
         _tcscpy_s(pTTT->szText, sizeof(pTTT->szText) / sizeof(TCHAR), _T("Area of spiral reinforcement"));
         break;

      case IDC_DB:
         _tcscpy_s(pTTT->szText, sizeof(pTTT->szText) / sizeof(TCHAR), _T("Diameter of spiral reinforcement"));
         break;

      case IDC_S:
         _tcscpy_s(pTTT->szText, sizeof(pTTT->szText) / sizeof(TCHAR), _T("Pitch of spiral reinforcement"));
         break;

      case IDC_FY_SPIRAL:
         _tcscpy_s(pTTT->szText, sizeof(pTTT->szText) / sizeof(TCHAR), _T("Yield Strengh of spiral reinforcement"));
         break;

      case IDC_ESU:
         _tcscpy_s(pTTT->szText, sizeof(pTTT->szText) / sizeof(TCHAR), _T("e su???"));
         break;

      case IDC_FY:
         _tcscpy_s(pTTT->szText, sizeof(pTTT->szText) / sizeof(TCHAR), _T("Yield Strengh of longitudinal reinforcement"));
         break;

      case IDC_FU:
         _tcscpy_s(pTTT->szText, sizeof(pTTT->szText) / sizeof(TCHAR), _T("Ultimate Strengh of longitudinal reinforcement"));
         break;

      case IDC_ES:
         _tcscpy_s(pTTT->szText, sizeof(pTTT->szText) / sizeof(TCHAR), _T("Modulus of Elasticity of longitudinal reinforcement"));
         break;

      case IDC_ESH:
         _tcscpy_s(pTTT->szText, sizeof(pTTT->szText) / sizeof(TCHAR), _T("Strain at which strain hardening begins"));
         break;

      case IDC_EFR:
         _tcscpy_s(pTTT->szText, sizeof(pTTT->szText) / sizeof(TCHAR), _T("Fracture strain"));
         break;

      case IDC_COVER:
         _tcscpy_s(pTTT->szText,sizeof(pTTT->szText)/sizeof(TCHAR),_T("Clear cover to spiral reinforcement"));
         break;

      case IDC_BONDED_BAR_AS:
      case IDC_UNBONDED_BAR_AS:
         _tcscpy_s(pTTT->szText, sizeof(pTTT->szText) / sizeof(TCHAR), _T("Area of a single reinforcing bar"));
         break;

      case IDC_BONDED_BAR_N:
      case IDC_UNBONDED_BAR_N:
         _tcscpy_s(pTTT->szText, sizeof(pTTT->szText) / sizeof(TCHAR), _T("Number of reinforcing bars"));
         break;

      case IDC_UNBONDED_BAR_LU:
      case IDC_TENDON_LU:
         _tcscpy_s(pTTT->szText, sizeof(pTTT->szText) / sizeof(TCHAR), _T("Unbonded length"));
         break;

      case IDC_TENDON_TYPE:
         _tcscpy_s(pTTT->szText, sizeof(pTTT->szText) / sizeof(TCHAR), _T("Select prestressing"));
         break;

      case IDC_TENDON_DIAMETER:
         _tcscpy_s(pTTT->szText, sizeof(pTTT->szText) / sizeof(TCHAR), _T("Diameter of tendon layout circle"));
         break;

      case IDC_TENDON_N:
         _tcscpy_s(pTTT->szText, sizeof(pTTT->szText) / sizeof(TCHAR), _T("Number of unbonded tendons"));
         break;

      case IDC_FPE:
         _tcscpy_s(pTTT->szText, sizeof(pTTT->szText) / sizeof(TCHAR), _T("Effective prestress in unbonded tendon"));
         break;

      case IDC_P:
         _tcscpy_s(pTTT->szText, sizeof(pTTT->szText) / sizeof(TCHAR), _T("Axial force"));
         break;

      case IDC_SLICES:
         _tcscpy_s(pTTT->szText, sizeof(pTTT->szText) / sizeof(TCHAR), _T("Number of slices for strain compatibility analysis"));
         break;

      case IDC_INIT_STEP:
         _tcscpy_s(pTTT->szText, sizeof(pTTT->szText) / sizeof(TCHAR), _T("Initial curvature step size"));
         break;

      case IDC_UPDATE:
         _tcscpy_s(pTTT->szText, sizeof(pTTT->szText) / sizeof(TCHAR), _T("Update results"));
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
