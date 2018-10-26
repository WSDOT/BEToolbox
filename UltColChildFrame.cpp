///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2017  Washington State Department of Transportation
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
#include "UltColChildFrame.h"
#include "UltColDoc.h"

IMPLEMENT_DYNCREATE(CUltColChildFrame, CEAFChildFrame)

CUltColChildFrame::CUltColChildFrame(void)
{
}

CUltColChildFrame::~CUltColChildFrame(void)
{
}

BEGIN_MESSAGE_MAP(CUltColChildFrame, CEAFChildFrame)
   ON_WM_CREATE()
   ON_BN_CLICKED(IDC_UPDATE,OnUpdate)
   ON_BN_CLICKED(IDC_US,OnUSUnits)
   ON_BN_CLICKED(IDC_SI,OnSIUnits)
   ON_NOTIFY_EX(TTN_NEEDTEXT,0,OnToolTipNotify)
	ON_MESSAGE(WM_HELP, OnCommandHelp)
END_MESSAGE_MAP()

LRESULT CUltColChildFrame::OnCommandHelp(WPARAM, LPARAM lParam)
{
   EAFHelp( EAFGetDocument()->GetDocumentationSetName(), IDH_ULTCOL );
   return TRUE;
}

void CUltColChildFrame::SetColumnParameters(Float64 diameter, Float64 fc, Float64 cover, Float64 As, Float64 Es, Float64 fy,Float64 ecl,Float64 etl)
{
   m_DlgBar.m_Diameter = diameter;
   m_DlgBar.m_Fc = fc;
   m_DlgBar.m_Cover = cover;
   m_DlgBar.m_As = As;
   m_DlgBar.m_Es = Es;
   m_DlgBar.m_Fy = fy;
   m_DlgBar.m_ecl = ecl;
   m_DlgBar.m_etl = etl;

   m_DlgBar.UpdateData(FALSE);
}

BOOL CUltColChildFrame::PreCreateWindow(CREATESTRUCT& cs)
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

int CUltColChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CEAFChildFrame::OnCreate(lpCreateStruct) == -1)
      return -1;

   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   if (!m_DlgBar.Create(this,IDD_ULTCOL,CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY, AFX_IDW_CONTROLBAR_FIRST))
   {
      TRACE0("Failed to create dialog bar\n");
      return -1;
   }

   EnableToolTips();
   m_DlgBar.EnableToolTips();

   SetIcon(AfxGetApp()->LoadIcon(IDR_ULTCOL),TRUE);

   return 0;
}

void CUltColChildFrame::OnUpdate()
{
   m_DlgBar.UpdateData(TRUE);

   CUltColDoc* pDoc = (CUltColDoc*)GetActiveDocument();
   if ( pDoc )
   {
      BOOL bModified = FALSE;
      Float64 dia, fc, cover, as, es, fy, ecl, etl;
      pDoc->m_Column->get_As(&as);
      pDoc->m_Column->get_Cover(&cover);
      pDoc->m_Column->get_Diameter(&dia);
      pDoc->m_Column->get_Es(&es);
      pDoc->m_Column->get_fc(&fc);
      pDoc->m_Column->get_fy(&fy);
      ecl = pDoc->m_ecl;
      etl = pDoc->m_etl;
      if ( !IsEqual(dia,m_DlgBar.m_Diameter) ||
           !IsEqual(fc,m_DlgBar.m_Fc) ||
           !IsEqual(cover,m_DlgBar.m_Cover) ||
           !IsEqual(as,m_DlgBar.m_As) ||
           !IsEqual(es,m_DlgBar.m_Es) ||
           !IsEqual(fy,m_DlgBar.m_Fy) ||
           !IsEqual(ecl,m_DlgBar.m_ecl) ||
           !IsEqual(etl,m_DlgBar.m_etl))
      {
         bModified = TRUE;
      }

      pDoc->m_Column->put_Diameter( m_DlgBar.m_Diameter );
      pDoc->m_Column->put_fc(       m_DlgBar.m_Fc );
      pDoc->m_Column->put_Cover(    m_DlgBar.m_Cover );
      pDoc->m_Column->put_As(       m_DlgBar.m_As );
      pDoc->m_Column->put_Es(       m_DlgBar.m_Es );
      pDoc->m_Column->put_fy(       m_DlgBar.m_Fy );
      pDoc->m_ecl = m_DlgBar.m_ecl;
      pDoc->m_etl = m_DlgBar.m_etl;

      if ( pDoc->IsModified() || bModified )
         pDoc->SetModifiedFlag();

      pDoc->UpdateAllViews(nullptr);
   }
}

void CUltColChildFrame::OnUSUnits()
{
   ASSERT(m_DlgBar.IsDlgButtonChecked(IDC_US) == 1 );
   m_DlgBar.UpdateData(TRUE);
   SetUnitsMode(eafTypes::umUS);
   m_DlgBar.UpdateData(FALSE);
   OnUpdate();
}

void CUltColChildFrame::OnSIUnits()
{
   ASSERT(m_DlgBar.IsDlgButtonChecked(IDC_SI) == 1 );
   m_DlgBar.UpdateData(TRUE);
   SetUnitsMode(eafTypes::umSI);
   m_DlgBar.UpdateData(FALSE);
   OnUpdate();
}

void CUltColChildFrame::SetUnitsMode(eafTypes::UnitMode um)
{
   CEAFApp* pApp = EAFGetApp();
   pApp->SetUnitsMode(um);
}

BOOL CUltColChildFrame::OnToolTipNotify(UINT id,NMHDR* pNMHDR, LRESULT* pResult)
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
void CUltColChildFrame::AssertValid() const
{
   CEAFChildFrame::AssertValid();
}
#endif

BOOL CUltColChildFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle , CWnd* pParentWnd , CCreateContext* pContext)
{
   // CEAFChildFrame::LoadFrame is going to load resources based on nIDResource
   // We have to set the module context to this module so the load happens
   // correctly
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return CEAFChildFrame::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext);
}
