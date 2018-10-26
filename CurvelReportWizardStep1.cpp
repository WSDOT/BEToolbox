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

// CurvelReportWizardPage1.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "CurvelReportWizardStep1.h"
#include <EAF\EAFApp.h>
#include <EAF\EAFDocument.h>

#include "CurvelReportWizard.h"

// CCurvelReportWizardStep1 dialog

IMPLEMENT_DYNAMIC(CCurvelReportWizardStep1, CPropertyPage)

CCurvelReportWizardStep1::CCurvelReportWizardStep1()
	: CPropertyPage(CCurvelReportWizardStep1::IDD)
{
   m_psp.dwFlags |= PSP_DEFAULT | PSP_HASHELP | PSP_USEHEADERTITLE/* | PSP_USEHEADERSUBTITLE*/;
   m_psp.pszHeaderTitle    = _T("Define vertical curve");
   //m_psp.pszHeaderSubTitle = _T("Elevations can be computed at individual stations, increments, and on skews");
}

CCurvelReportWizardStep1::~CCurvelReportWizardStep1()
{
}

void CCurvelReportWizardStep1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDisplayUnits = pApp->GetDisplayUnits();

   DDX_Percentage(pDX,IDC_G1,m_G1);
   DDX_Percentage(pDX,IDC_G2,m_G2);
   DDX_Station(pDX,IDC_PVI_STATION,m_PVIStation,pDisplayUnits->StationFormat);
   DDX_UnitValueAndTag(pDX,IDC_PVI_ELEVATION,IDC_PVI_ELEVATION_UNIT,m_PVIElevation,pDisplayUnits->AlignmentLength);
   DDX_UnitValueAndTag(pDX,IDC_LENGTH,IDC_LENGTH_UNIT,m_Length,pDisplayUnits->AlignmentLength);
   DDV_UnitValueZeroOrMore(pDX, IDC_LENGTH, m_Length, pDisplayUnits->AlignmentLength);


   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   DDX_MetaFileStatic(pDX, IDC_IMAGE, m_Image, AfxGetResourceHandle(), _T("VertCurve"), _T("Metafile") );
}


BEGIN_MESSAGE_MAP(CCurvelReportWizardStep1, CPropertyPage)
   ON_COMMAND(ID_HELP,OnHelp)
END_MESSAGE_MAP()


// CCurvelReportWizardStep1 message handlers

LRESULT CCurvelReportWizardStep1::OnWizardNext()
{
   if ( !UpdateData() )
      return -1;

   CCurvelReportWizard* pParent = (CCurvelReportWizard*)GetParent();
   pParent->m_CurrentPage = pParent->m_Pages.begin();
   return pParent->GetNextPage();
   //return CPropertyPage::OnWizardNext();
}

BOOL CCurvelReportWizardStep1::OnKillActive()
{
   return TRUE;
   //return CPropertyPage::OnKillActive();
}

BOOL CCurvelReportWizardStep1::OnSetActive()
{
   CPropertySheet* pParent = (CPropertySheet*)GetParent();
   pParent->SetWizardButtons(PSWIZB_NEXT);

   return CPropertyPage::OnSetActive();
}

BOOL CCurvelReportWizardStep1::OnInitDialog()
{
   // Put our logo on the dialog
   HICON hIcon = (HICON)LoadImage(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_CURVEL),IMAGE_ICON,0,0,LR_DEFAULTSIZE);
   SetIcon(hIcon,FALSE);

   CCurvelReportWizard* pParent = (CCurvelReportWizard*)GetParent();
   if ( pParent->m_pRptSpec )
      InitFromRptSpec();

   CPropertyPage::OnInitDialog();

   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}

void CCurvelReportWizardStep1::InitFromRptSpec()
{
   CCurvelReportWizard* pParent = (CCurvelReportWizard*)GetParent();

   pParent->m_pRptSpec->GetVerticalCurveParameters(&m_G1,&m_G2,&m_PVIStation,&m_PVIElevation,&m_Length);
}

void CCurvelReportWizardStep1::UpdateReportSpecification(std::shared_ptr<CCurvelReportSpecification>& pRptSpec)
{
   pRptSpec->SetVerticalCurveParameters(m_G1,m_G2,m_PVIStation,m_PVIElevation,m_Length);
}

void CCurvelReportWizardStep1::OnHelp()
{
   EAFHelp(EAFGetDocument()->GetDocumentationSetName(), IDH_CURVEL_STEP1);
}
