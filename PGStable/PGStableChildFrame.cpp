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
#include "..\resource.h"
#include "PGStableChildFrame.h"
#include "PGStableDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CPGStableChildFrame, CEAFChildFrame)

CPGStableChildFrame::CPGStableChildFrame(void)
{
}

CPGStableChildFrame::~CPGStableChildFrame(void)
{
}

BEGIN_MESSAGE_MAP(CPGStableChildFrame, CEAFChildFrame)
   ON_WM_CREATE()
   ON_BN_CLICKED(EAFID_UNITS_US,OnUSUnits)
   ON_BN_CLICKED(EAFID_UNITS_SI,OnSIUnits)
   ON_NOTIFY_EX(TTN_NEEDTEXT,0,OnToolTipNotify)
   ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
END_MESSAGE_MAP()

void CPGStableChildFrame::OnHelpFinder()
{
   EAFHelp(EAFGetDocument()->GetDocumentationSetName(),IDH_PGSTABLE);
}

BOOL CPGStableChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
   // force this window to be maximized (not sure why WS_VISIBLE is required)
   cs.style |= WS_MAXIMIZE | WS_VISIBLE;
   cs.style &= ~WS_MAXIMIZEBOX; // we don't want a maximize box
   cs.style &= ~WS_MINIMIZEBOX; // we don't want a minimize box
   cs.style &= ~WS_SYSMENU; // we don't want a system menu

   // I have no idea why, but creating this window fails the second time a BEToolbox document
   // is created in a single BridgeLink run. For some reason, MFC determines the wrong
   // class name. lpszClass is nullptr the first time this window is created successfully, so
   // we'll force it to nullptr here so it works on subsequent creation attempts
   cs.lpszClass = nullptr;

   return CEAFChildFrame::PreCreateWindow(cs);
}

int CPGStableChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CEAFChildFrame::OnCreate(lpCreateStruct) == -1)
      return -1;

   EnableToolTips();

   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   SetIcon(AfxGetApp()->LoadIcon(IDR_PGSTABLE),TRUE);

   return 0;
}

void CPGStableChildFrame::OnUpdate(LPARAM hint)
{
   CPGStableDoc* pDoc = (CPGStableDoc*)GetActiveDocument();
   if ( pDoc )
   {
      pDoc->SetModifiedFlag();
      pDoc->UpdateAllViews(nullptr,hint,nullptr);
   }
}

void CPGStableChildFrame::OnUSUnits()
{
   SetUnitsMode(eafTypes::umUS);
}

void CPGStableChildFrame::OnSIUnits()
{
   SetUnitsMode(eafTypes::umSI);
}

void CPGStableChildFrame::SetUnitsMode(eafTypes::UnitMode um)
{
   CEAFApp* pApp = EAFGetApp();
   pApp->SetUnitsMode(um);
}

BOOL CPGStableChildFrame::OnToolTipNotify(UINT id,NMHDR* pNMHDR, LRESULT* pResult)
{
   TOOLTIPTEXT* pTTT = (TOOLTIPTEXT*)pNMHDR;
   HWND hwndTool = (HWND)pNMHDR->idFrom;
   if ( pTTT->uFlags & TTF_IDISHWND )
   {
      // idFrom is actually HWND of tool
      UINT nID = ::GetDlgCtrlID(hwndTool);
      switch(nID)
      {
      case IDC_PRISMATIC:
         _tcscpy_s(pTTT->szText,sizeof(pTTT->szText)/sizeof(TCHAR),_T("Define a prismatic girder"));
         break;

      case IDC_NONPRISMATIC:
         _tcscpy_s(pTTT->szText,sizeof(pTTT->szText)/sizeof(TCHAR),_T("Define a non-prismatic girder"));
         break;

      default:
         return FALSE;
      }

      return TRUE;
   }
   return FALSE;
}

#if defined _DEBUG
void CPGStableChildFrame::AssertValid() const
{
   CEAFChildFrame::AssertValid();
}
#endif

BOOL CPGStableChildFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle , CWnd* pParentWnd , CCreateContext* pContext)
{
   // CEAFChildFrame::LoadFrame is going to load resources based on nIDResource
   // We have to set the module context to this module so the load happens
   // correctly
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return CEAFChildFrame::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext);
}
