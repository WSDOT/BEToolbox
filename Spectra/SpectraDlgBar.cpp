///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2020  Washington State Department of Transportation
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

// DlgBar.cpp : implementation file
//

#include "stdafx.h"
#include "..\resource.h"
#include <EAF\EAFApp.h>
#include "SpectraDlgBar.h"
#include "SpectraDoc.h"
#include <MFCTools\CustomDDX.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CSpectraDialogBar

IMPLEMENT_DYNAMIC(CSpectraDialogBar, CDialogBar)

CSpectraDialogBar::CSpectraDialogBar()
{
}

CSpectraDialogBar::~CSpectraDialogBar()
{
}


BEGIN_MESSAGE_MAP(CSpectraDialogBar, CDialogBar)
END_MESSAGE_MAP()



// CSpectraDialogBar message handlers



void CSpectraDialogBar::DoDataExchange(CDataExchange* pDX)
{
   CDialogBar::DoDataExchange(pDX);

   CSpectraDoc* pDoc = (CSpectraDoc*)((CFrameWnd*)GetParent())->GetActiveDocument();

   Float64 lat,lng;
   SiteClass siteClass;
   SpecificationType specType;

   if ( !pDX->m_bSaveAndValidate )
   {
      pDoc->GetLocation(&lat,&lng);
      lng *= -1; // change sign because we are west
      siteClass = pDoc->GetSiteClass();
      specType = pDoc->GetSpecification();
   }

   DDX_Text(pDX,IDC_LATITUDE,lat);
   DDX_Text(pDX,IDC_LONGITUDE,lng);

   DDX_CBEnum(pDX,IDC_SITE_CLASS,siteClass);

   DDX_CBEnum(pDX, IDC_SPECIFICATION, specType);

   if ( pDX->m_bSaveAndValidate )
   {
      lng *= -1;
      pDoc->SetLocation(lat,lng);
      pDoc->SetSiteClass(siteClass);
      pDoc->SetSpecification(specType);
   }

   if ( !pDX->m_bSaveAndValidate )
   {
      UpdateSiteClassDescription();
   }
}

#if defined _DEBUG
void CSpectraDialogBar::AssertValid() const
{
   AFX_MANAGE_STATE(AfxGetAppModuleState());
   CDialogBar::AssertValid();
}
#endif

void CSpectraDialogBar::UpdateSiteClassDescription()
{
   CSpectraDoc* pDoc = (CSpectraDoc*)((CFrameWnd*)GetParent())->GetActiveDocument();
   CString strDesc = pDoc->GetSiteClassDescription(pDoc->GetSiteClass());
   GetDlgItem(IDC_DESCRIPTION)->SetWindowText(strDesc);
}

BOOL CSpectraDialogBar::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
   // TODO: Add your specialized code here and/or call the base class

   if ( !CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID) )
      return FALSE;

   CString strLatRange;
   strLatRange.Format(_T("(%5.1f° - %5.1f° N)"),MIN_LATITUDE,MAX_LATITUDE);
   GetDlgItem(IDC_LAT_RANGE)->SetWindowText(strLatRange);

   CString strLngRange;
   strLngRange.Format(_T("(%5.1f° - %5.1f° W)"),-MAX_LONGITUDE,-MIN_LONGITUDE);
   GetDlgItem(IDC_LNG_RANGE)->SetWindowText(strLngRange);

   CComboBox* pcbSiteClass = (CComboBox*)GetDlgItem(IDC_SITE_CLASS);
   pcbSiteClass->AddString(_T("A"));
   pcbSiteClass->AddString(_T("B"));
   pcbSiteClass->AddString(_T("C"));
   pcbSiteClass->AddString(_T("D"));
   pcbSiteClass->AddString(_T("E"));
   pcbSiteClass->AddString(_T("F"));

   pcbSiteClass->SetCurSel(1);

   CComboBox* pcbSpecification = (CComboBox*)GetDlgItem(IDC_SPECIFICATION);
   pcbSpecification->AddString(_T("WSDOT Bridge Design Manual"));
   pcbSpecification->AddString(_T("AASHTO LRFD Bridge Design Specifications"));
   pcbSpecification->AddString(_T("AASHTO Guide Specifications for LRFD Seismic Bridge Design"));
   pcbSpecification->SetCurSel(0);


   return TRUE;
}
