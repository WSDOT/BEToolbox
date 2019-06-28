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

#include "StdAfx.h"
#include "..\resource.h"
#include "SVTToolChildFrame.h"
#include "SVTToolDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSVTToolChildFrame, CEAFChildFrame)

CSVTToolChildFrame::CSVTToolChildFrame(void)
{
}

CSVTToolChildFrame::~CSVTToolChildFrame(void)
{
}

BEGIN_MESSAGE_MAP(CSVTToolChildFrame, CEAFChildFrame)
   ON_WM_CREATE()
   ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNotify)
   //ON_MESSAGE(WM_HELP, OnCommandHelp)
   ON_CBN_SELCHANGE(IDC_GIRDERS, OnGirderChanged)
   ON_BN_CLICKED(IDC_COMPUTE, OnCompute)
END_MESSAGE_MAP()

//LRESULT CSVTToolChildFrame::OnCommandHelp(WPARAM, LPARAM lParam)
//{
//   EAFHelp( EAFGetDocument()->GetDocumentationSetName(), IDH_SVTTOOL );
//   return TRUE;
//}

BOOL CSVTToolChildFrame::PreCreateWindow(CREATESTRUCT& cs)
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

int CSVTToolChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CEAFChildFrame::OnCreate(lpCreateStruct) == -1)
      return -1;

   {
      AFX_MANAGE_STATE(AfxGetStaticModuleState());

      if (!m_DlgBar.Create(this, IDD_SVT, CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY, AFX_IDW_CONTROLBAR_FIRST))
      {
         TRACE0("Failed to create dialog bar\n");
         return -1;
      }
      EnableToolTips();
      m_DlgBar.EnableToolTips();
      SetIcon(AfxGetApp()->LoadIcon(IDR_SVT), TRUE);
   }

   CSVTToolDoc* pDoc = (CSVTToolDoc*)GetActiveDocument();
   if (pDoc)
   {
      CComboBox* pCB = (CComboBox*)m_DlgBar.GetDlgItem(IDC_GIRDERS);
      pCB->SetCurSel((int)pDoc->GetGirder());
   }


   return 0;
}


BOOL CSVTToolChildFrame::OnToolTipNotify(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
   TOOLTIPTEXT* pTTT = (TOOLTIPTEXT*)pNMHDR;
   HWND hwndTool = (HWND)pNMHDR->idFrom;
   if (pTTT->uFlags & TTF_IDISHWND)
   {
      // idFrom is actually HWND of tool
      UINT nID = ::GetDlgCtrlID(hwndTool);
      switch (nID)
      {
      case IDC_GIRDERS:
         _tcscpy_s(pTTT->szText, sizeof(pTTT->szText) / sizeof(TCHAR), _T("Select a girder"));
         break;

      case IDC_COMPUTE:
         _tcscpy_s(pTTT->szText, sizeof(pTTT->szText) / sizeof(TCHAR), _T("Compute the St. Venant Torsional Constant"));
         break;

      default:
         return FALSE;
      }

      return TRUE;
   }
   return FALSE;
}

#if defined _DEBUG
void CSVTToolChildFrame::AssertValid() const
{
   CEAFChildFrame::AssertValid();
}
#endif

BOOL CSVTToolChildFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle , CWnd* pParentWnd , CCreateContext* pContext)
{
   // CEAFChildFrame::LoadFrame is going to load resources based on nIDResource
   // We have to set the module context to this module so the load happens
   // correctly
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   if (!CEAFChildFrame::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
   {
      return FALSE;
   }

   return TRUE;
}

void CSVTToolChildFrame::OnGirderChanged()
{
   CComboBox* pCB = (CComboBox*)m_DlgBar.GetDlgItem(IDC_GIRDERS);
   auto nCurSel = pCB->GetCurSel();

   CSVTToolDoc* pDoc = (CSVTToolDoc*)GetActiveDocument();
   if (pDoc && nCurSel != CB_ERR)
   {
      pDoc->SetGirder((BeamShapeType)nCurSel);
   }
}

void CSVTToolChildFrame::OnCompute()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CWaitCursor cursor;

   CSVTToolDoc* pDoc = (CSVTToolDoc*)GetActiveDocument();
   Float64 J;
   IndexType nElements, nNodes;
   pDoc->GetTorsionalConstant(&J,&nElements,&nNodes);
   CString str;
   str.Format(_T("J = %.0f  in^4\n# Elements = %d\n# Points = %d"), J,nElements, nNodes);
   AfxMessageBox(str);
}
