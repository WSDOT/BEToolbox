///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2016  Washington State Department of Transportation
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

// PGStableGirderView.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PGStableGirderView.h"
#include "PGStableDoc.h"


// CPGStableGirderView

IMPLEMENT_DYNCREATE(CPGStableGirderView, CFormView)

CPGStableGirderView::CPGStableGirderView()
	: CFormView(CPGStableGirderView::IDD)
{
   m_bViewInitialized = FALSE;
}

CPGStableGirderView::~CPGStableGirderView()
{
}

void CPGStableGirderView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();

   int prismatic = pDoc->GetGirderType();
   DDX_Radio(pDX,IDC_PRISMATIC,prismatic);
   if ( pDX->m_bSaveAndValidate )
   {
      pDoc->SetGirderType(prismatic);
   }
}

BEGIN_MESSAGE_MAP(CPGStableGirderView, CFormView)
   ON_BN_CLICKED(IDC_PRISMATIC, &CPGStableGirderView::OnSwapUI)
   ON_BN_CLICKED(IDC_NONPRISMATIC, &CPGStableGirderView::OnSwapUI)
END_MESSAGE_MAP()


// CPGStableGirderView diagnostics

#ifdef _DEBUG
void CPGStableGirderView::AssertValid() const
{
   AFX_MANAGE_STATE(AfxGetAppModuleState());
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPGStableGirderView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPGStableGirderView message handlers

BOOL CPGStableGirderView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,	DWORD dwRequestedStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,CCreateContext* pContext)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return CFormView::Create(lpszClassName,lpszWindowName,dwRequestedStyle,rect,pParentWnd,nID,pContext);
}

void CPGStableGirderView::OnActivateView(BOOL bActivate,CView* pActivateView,CView* pDeactivateView)
{
   CFormView::OnActivateView(bActivate,pActivateView,pDeactivateView);
   if ( m_bViewInitialized )
   {
      if ( GetCheckedRadioButton(IDC_PRISMATIC,IDC_NONPRISMATIC) == IDC_PRISMATIC )
      {
         m_Prismatic.UpdateData(!bActivate);
      }
      else
      {
         m_Nonprismatic.UpdateData(!bActivate);
      }
   }
}

void CPGStableGirderView::OnSwapUI()
{
   if ( m_bViewInitialized )
   {
      if ( GetCheckedRadioButton(IDC_PRISMATIC,IDC_NONPRISMATIC) == IDC_PRISMATIC )
      {
         m_Nonprismatic.UpdateData(TRUE);
         m_Nonprismatic.ShowWindow(SW_HIDE);

         m_Prismatic.ShowWindow(SW_SHOW);
         m_Prismatic.UpdateData(FALSE);
      }
      else
      {
         m_Prismatic.UpdateData(TRUE);
         m_Prismatic.ShowWindow(SW_HIDE);

         m_Nonprismatic.ShowWindow(SW_SHOW);
         m_Nonprismatic.UpdateData(FALSE);
      }
   }
}

void CPGStableGirderView::OnInitialUpdate()
{
   CFormView::OnInitialUpdate();

   // Embed dialogs. See http://www.codeproject.com/KB/dialog/embedded_dialog.aspx
   CWnd* pBox = GetDlgItem(IDC_STATIC_BOUNDS);
   pBox->ShowWindow(SW_HIDE);

   CRect boxRect;
   pBox->GetWindowRect(&boxRect);
   ScreenToClient(boxRect);

   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();
   int prismatic = pDoc->GetGirderType();

   VERIFY(m_Prismatic.Create(CPGStablePrismaticGirder::IDD, this));
   VERIFY(m_Prismatic.SetWindowPos( GetDlgItem(IDC_STATIC_BOUNDS), boxRect.left, boxRect.top, 0, 0, SWP_SHOWWINDOW|SWP_NOSIZE));
   m_Prismatic.ShowWindow(prismatic == PRISMATIC ? SW_SHOW : SW_HIDE);

   VERIFY(m_Nonprismatic.Create(CPGStableNonprismaticGirder::IDD, this));
   VERIFY(m_Nonprismatic.SetWindowPos( GetDlgItem(IDC_STATIC_BOUNDS), boxRect.left, boxRect.top, 0, 0, SWP_SHOWWINDOW|SWP_NOSIZE));
   m_Nonprismatic.ShowWindow(prismatic == PRISMATIC ? SW_HIDE : SW_SHOW);

   m_bViewInitialized = TRUE;
}

void CPGStableGirderView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* /*pHint*/)
{
   if ( EAFGetActiveView() == this && m_bViewInitialized )
   {
      if ( lHint == EAF_HINT_UNITS_CHANGING )
      {
         if ( GetCheckedRadioButton(IDC_PRISMATIC,IDC_NONPRISMATIC) == IDC_PRISMATIC )
         {
            m_Prismatic.UpdateData(TRUE);
         }
         else
         {
            m_Nonprismatic.UpdateData(TRUE);
         }
      }
      else if ( lHint == EAF_HINT_UNITS_CHANGED )
      {
         if ( GetCheckedRadioButton(IDC_PRISMATIC,IDC_NONPRISMATIC) == IDC_PRISMATIC )
         {
            m_Prismatic.UpdateData(FALSE);
         }
         else
         {
            m_Nonprismatic.UpdateData(FALSE);
         }
      }
   }
}
