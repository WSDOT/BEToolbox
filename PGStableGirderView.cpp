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

// PGStableGirderView.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PGStableGirderView.h"
#include "PGStableDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CPGStableGirderView

IMPLEMENT_DYNCREATE(CPGStableGirderView, CPGStableFormView)

CPGStableGirderView::CPGStableGirderView()
	: CPGStableFormView(CPGStableGirderView::IDD)
{
   m_bViewInitialized = FALSE;
}

CPGStableGirderView::~CPGStableGirderView()
{
}

void CPGStableGirderView::DoDataExchange(CDataExchange* pDX)
{
	CPGStableFormView::DoDataExchange(pDX);

   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();

   CString strSpec = pDoc->GetCriteria();
   DDX_CBString(pDX,IDC_SPEC,strSpec);

   CString strEngineer, strCompany, strJob, strComments;
   pDoc->GetProjectProperties(&strEngineer,&strCompany,&strJob,&strComments);

   DDX_Text(pDX,IDC_ENGINEER,strEngineer);
   DDX_Text(pDX,IDC_COMPANY,strCompany);
   DDX_Text(pDX,IDC_JOB,strJob);
   DDX_Text(pDX,IDC_COMMENTS,strComments);

   int girderType = pDoc->GetGirderType();
   DDX_Radio(pDX,IDC_PRISMATIC,girderType);
   if ( pDX->m_bSaveAndValidate )
   {
      pDoc->SetProjectProperties(strEngineer,strCompany,strJob,strComments);
      pDoc->SetGirderType(girderType);
      pDoc->SetCriteria(strSpec);
   }
}

BEGIN_MESSAGE_MAP(CPGStableGirderView, CPGStableFormView)
   ON_BN_CLICKED(IDC_PRISMATIC, &CPGStableGirderView::OnSwapUI)
   ON_BN_CLICKED(IDC_NONPRISMATIC, &CPGStableGirderView::OnSwapUI)
   ON_EN_CHANGE(IDC_ENGINEER, &CPGStableGirderView::OnChange)
   ON_EN_CHANGE(IDC_COMPANY, &CPGStableGirderView::OnChange)
   ON_EN_CHANGE(IDC_JOB, &CPGStableGirderView::OnChange)
   ON_EN_CHANGE(IDC_COMMENTS, &CPGStableGirderView::OnChange)
	ON_MESSAGE(WM_HELP, OnCommandHelp)
END_MESSAGE_MAP()


// CPGStableGirderView diagnostics

#ifdef _DEBUG
void CPGStableGirderView::AssertValid() const
{
   AFX_MANAGE_STATE(AfxGetAppModuleState());
	CPGStableFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPGStableGirderView::Dump(CDumpContext& dc) const
{
	CPGStableFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPGStableGirderView message handlers

BOOL CPGStableGirderView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,	DWORD dwRequestedStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,CCreateContext* pContext)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return CPGStableFormView::Create(lpszClassName,lpszWindowName,dwRequestedStyle,rect,pParentWnd,nID,pContext);
}

void CPGStableGirderView::OnActivateView()
{
   if ( m_bViewInitialized )
   {
      UpdateData(FALSE);
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

void CPGStableGirderView::OnDeactivateView()
{
   if ( m_bViewInitialized )
   {
      UpdateData(TRUE);
      if ( GetCheckedRadioButton(IDC_PRISMATIC,IDC_NONPRISMATIC) == IDC_PRISMATIC )
      {
         m_Prismatic.UpdateData(TRUE);
      }
      else
      {
         m_Nonprismatic.UpdateData(TRUE);
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

         m_Prismatic.UpdateData(FALSE);
         m_Prismatic.ShowWindow(SW_SHOW);
      }
      else
      {
         m_Prismatic.UpdateData(TRUE);
         m_Prismatic.ShowWindow(SW_HIDE);

         m_Nonprismatic.UpdateData(FALSE);
         m_Nonprismatic.ShowWindow(SW_SHOW);
      }
   }
}

LRESULT CPGStableGirderView::OnCommandHelp(WPARAM, LPARAM lParam)
{
   EAFHelp( EAFGetDocument()->GetDocumentationSetName(),GetCheckedRadioButton(IDC_PRISMATIC,IDC_NONPRISMATIC) == IDC_PRISMATIC ? IDH_PGSTABLE_PRISMATIC_GIRDER_VIEW : IDH_PGSTABLE_NONPRISMATIC_GIRDER_VIEW);
   return TRUE;
}

void CPGStableGirderView::OnInitialUpdate()
{
   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();

   const SpecLibrary* pSpecLib = pDoc->GetSpecLibrary();
   libKeyListType keyList;
   pSpecLib->KeyList(keyList);
   CComboBox* pcbSpec = (CComboBox*)GetDlgItem(IDC_SPEC);
   pcbSpec->AddString(gs_strCriteria);
   for (const auto& key : keyList)
   {
      pcbSpec->AddString(key.c_str());
   }


   CPGStableFormView::OnInitialUpdate();

   // Embed dialogs. See http://www.codeproject.com/KB/dialog/embedded_dialog.aspx
   CWnd* pBox = GetDlgItem(IDC_STATIC_BOUNDS);
   pBox->ShowWindow(SW_HIDE);

   CRect boxRect;
   pBox->GetWindowRect(&boxRect);
   ScreenToClient(boxRect);

   AFX_MANAGE_STATE(AfxGetStaticModuleState());

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
   if ( (EAFGetActiveView() == this && m_bViewInitialized) || lHint == HINT_UPDATE_DATA )
   {
      if ( lHint == EAF_HINT_UNITS_CHANGING || lHint == HINT_UPDATE_DATA)
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
            m_Prismatic.OnUnitsChanged();
         }
         else
         {
            m_Nonprismatic.OnUnitsChanged();
         }
      }
   }
}

void CPGStableGirderView::OnChange()
{
   // TODO:  If this is a RICHEDIT control, the control will not
   // send this notification unless you override the CFormView::OnInitDialog()
   // function and call CRichEditCtrl().SetEventMask()
   // with the ENM_CHANGE flag ORed into the mask.

   // TODO:  Add your control notification handler code here
   UpdateData(TRUE);
}
