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

// PGStableCriteriaView.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PGStableCriteriaView.h"
#include "PGStableDoc.h"
#include <MFCTools\CustomDDX.h>


// CPGStableCriteriaView

IMPLEMENT_DYNCREATE(CPGStableCriteriaView, CFormView)

CPGStableCriteriaView::CPGStableCriteriaView()
	: CFormView(CPGStableCriteriaView::IDD)
{

}

CPGStableCriteriaView::~CPGStableCriteriaView()
{
}

void CPGStableCriteriaView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   CString strLiftingTag(pApp->GetUnitsMode() == eafTypes::umUS ? _T("sqrt(f'ci (KSI))") : _T("sqrt(f'ci (MPa))"));
   CString strHaulingTag(pApp->GetUnitsMode() == eafTypes::umUS ? _T("sqrt(f'c (KSI))") : _T("sqrt(f'c (MPa))"));

   DDX_Text(pDX,IDC_LIFTING_FS_CRACKING,m_LiftingCriteria.MinFScr);
   DDX_Text(pDX,IDC_LIFTING_FS_FAILURE,m_LiftingCriteria.MinFSf);
   DDX_Text(pDX,IDC_LIFTING_COMPRESSION,m_LiftingCriteria.CompressionCoefficient);
   DDX_UnitValue(pDX,IDC_LIFTING_TENSION,m_LiftingCriteria.TensionCoefficient,pDispUnits->SqrtPressure);
   DDX_Text(pDX,IDC_LIFTING_TENSION_UNIT,strLiftingTag);
   DDX_Check_Bool(pDX,IDC_CHECK_LIFTING_TENSION_MAX,m_LiftingCriteria.bMaxTension);
   DDX_UnitValueAndTag(pDX,IDC_LIFTING_TENSION_MAX,IDC_LIFTING_TENSION_MAX_UNIT,m_LiftingCriteria.MaxTension,pDispUnits->Stress);
   DDX_UnitValue(pDX,IDC_LIFTING_TENSION_WITH_REBAR,m_LiftingCriteria.TensionCoefficientWithRebar,pDispUnits->SqrtPressure);
   DDX_Text(pDX,IDC_LIFTING_TENSION_WITH_REBAR_UNIT,strLiftingTag);

   DDX_Text(pDX,IDC_HAULING_FS_CRACKING,m_HaulingCriteria.MinFScr);
   DDX_Text(pDX,IDC_HAULING_FS_FAILURE,m_HaulingCriteria.MinFSf);
   DDX_Text(pDX,IDC_HAULING_COMPRESSION,m_HaulingCriteria.CompressionCoefficient);
   DDX_UnitValue(pDX,IDC_HAULING_TENSION,m_HaulingCriteria.TensionCoefficient,pDispUnits->SqrtPressure);
   DDX_Text(pDX,IDC_HAULING_TENSION_UNIT,strHaulingTag);
   DDX_Check_Bool(pDX,IDC_CHECK_HAULING_TENSION_MAX,m_HaulingCriteria.bMaxTension);
   DDX_UnitValueAndTag(pDX,IDC_HAULING_TENSION_MAX,IDC_HAULING_TENSION_MAX_UNIT,m_HaulingCriteria.MaxTension,pDispUnits->Stress);
   DDX_UnitValue(pDX,IDC_HAULING_TENSION_WITH_REBAR,m_HaulingCriteria.TensionCoefficientWithRebar,pDispUnits->SqrtPressure);
   DDX_Text(pDX,IDC_HAULING_TENSION_WITH_REBAR_UNIT,strHaulingTag);

   if ( pDX->m_bSaveAndValidate )
   {
      CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();
      pDoc->SetLiftingCriteria(m_LiftingCriteria);
      pDoc->SetHaulingCriteria(m_HaulingCriteria);
   }
}

BEGIN_MESSAGE_MAP(CPGStableCriteriaView, CFormView)
   ON_BN_CLICKED(IDC_CHECK_LIFTING_TENSION_MAX, &CPGStableCriteriaView::OnClickedLiftingTensionMax)
   ON_BN_CLICKED(IDC_CHECK_HAULING_TENSION_MAX, &CPGStableCriteriaView::OnClickedHaulingTensionMax)
END_MESSAGE_MAP()


// CPGStableCriteriaView diagnostics

#ifdef _DEBUG
void CPGStableCriteriaView::AssertValid() const
{
   AFX_MANAGE_STATE(AfxGetAppModuleState());
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPGStableCriteriaView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPGStableCriteriaView message handlers

BOOL CPGStableCriteriaView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,	DWORD dwRequestedStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,CCreateContext* pContext)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return CFormView::Create(lpszClassName,lpszWindowName,dwRequestedStyle,rect,pParentWnd,nID,pContext);
}

void CPGStableCriteriaView::OnActivateView(BOOL bActivate,CView* pActivateView,CView* pDeactivateView)
{
   CFormView::OnActivateView(bActivate,pActivateView,pDeactivateView);
   UpdateData(!bActivate);
}

void CPGStableCriteriaView::OnInitialUpdate()
{
   CPGStableDoc* pDoc = (CPGStableDoc*)GetDocument();
   m_LiftingCriteria = pDoc->GetLiftingCriteria();
   m_HaulingCriteria = pDoc->GetHaulingCriteria();

   CFormView::OnInitialUpdate();

   OnClickedLiftingTensionMax();
   OnClickedHaulingTensionMax();
}

void CPGStableCriteriaView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* /*pHint*/)
{
   if ( EAFGetActiveView() == this )
   {
      if ( lHint == EAF_HINT_UNITS_CHANGING )
      {
         UpdateData(TRUE);
      }
      else if ( lHint == EAF_HINT_UNITS_CHANGED )
      {
         UpdateData(FALSE);
      }
   }
}

void CPGStableCriteriaView::OnClickedLiftingTensionMax()
{
   BOOL bEnable = IsDlgButtonChecked(IDC_CHECK_LIFTING_TENSION_MAX) == BST_CHECKED ? TRUE : FALSE;
   GetDlgItem(IDC_LIFTING_TENSION_MAX)->EnableWindow(bEnable);
}

void CPGStableCriteriaView::OnClickedHaulingTensionMax()
{
   BOOL bEnable = IsDlgButtonChecked(IDC_CHECK_HAULING_TENSION_MAX) == BST_CHECKED ? TRUE : FALSE;
   GetDlgItem(IDC_HAULING_TENSION_MAX)->EnableWindow(bEnable);
}
