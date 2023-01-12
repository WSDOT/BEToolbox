///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2023  Washington State Department of Transportation
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

// CurvelReportWizardPage2.cpp : implementation file
//

#include "stdafx.h"
#include "..\resource.h"
#include "CurvelReportWizardStep2.h"
#include "CurvelReportWizard.h"
#include <EAF\EAFApp.h>
#include <EAF\EAFDocument.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



// CCurvelReportWizardStep2 dialog

IMPLEMENT_DYNAMIC(CCurvelReportWizardStep2, CPropertyPage)

CCurvelReportWizardStep2::CCurvelReportWizardStep2()
	: CPropertyPage(CCurvelReportWizardStep2::IDD)
{
   m_psp.dwFlags |= PSP_DEFAULT | PSP_HASHELP | PSP_USEHEADERTITLE/* | PSP_USEHEADERSUBTITLE*/;
   m_psp.pszHeaderTitle    = _T("Define roadway section and superelevation profile");
   //m_psp.pszHeaderSubTitle = _T("Elevations are computed at up to 4 points offset from the alignment");
}

CCurvelReportWizardStep2::~CCurvelReportWizardStep2()
{
}

void CCurvelReportWizardStep2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDisplayUnits = pApp->GetDisplayUnits();

   DDX_Check_Bool(pDX,IDC_CROWN_OPTION,m_bCorrectForSuperelevation);

   DDX_OffsetAndTag(pDX,IDC_CROWN_OFFSET,IDC_CROWN_OFFSET_UNIT,m_ProfileGradeOffset,pDisplayUnits->AlignmentLength);

   DDX_Station(pDX,IDC_STATION_1,m_SuperelevationPoint[0].Station,pDisplayUnits->StationFormat);
   DDX_Text(pDX,IDC_LEFT_SLOPE_1,m_SuperelevationPoint[0].LeftSlope);
   DDX_Text(pDX,IDC_RIGHT_SLOPE_1,m_SuperelevationPoint[0].RightSlope);

   DDX_Station(pDX,IDC_STATION_2,m_SuperelevationPoint[1].Station,pDisplayUnits->StationFormat);
   DDX_Text(pDX,IDC_LEFT_SLOPE_2,m_SuperelevationPoint[1].LeftSlope);
   DDX_Text(pDX,IDC_RIGHT_SLOPE_2,m_SuperelevationPoint[1].RightSlope);

   DDX_Station(pDX,IDC_STATION_3,m_SuperelevationPoint[2].Station,pDisplayUnits->StationFormat);
   DDX_Text(pDX,IDC_LEFT_SLOPE_3,m_SuperelevationPoint[2].LeftSlope);
   DDX_Text(pDX,IDC_RIGHT_SLOPE_3,m_SuperelevationPoint[2].RightSlope);

   if ( !pDX->m_bSaveAndValidate )
   {
      std::_tstring strUnitTag = pDisplayUnits->AlignmentLength.UnitOfMeasure.UnitTag();
      CString strSlopeUnit;
      strSlopeUnit.Format(_T("%s/%s"),strUnitTag.c_str(),strUnitTag.c_str());
      GetDlgItem(IDC_LEFT_SLOPE_UNIT_1)->SetWindowText(strSlopeUnit);
      GetDlgItem(IDC_RIGHT_SLOPE_UNIT_1)->SetWindowText(strSlopeUnit);
      GetDlgItem(IDC_LEFT_SLOPE_UNIT_2)->SetWindowText(strSlopeUnit);
      GetDlgItem(IDC_RIGHT_SLOPE_UNIT_2)->SetWindowText(strSlopeUnit);
      GetDlgItem(IDC_LEFT_SLOPE_UNIT_3)->SetWindowText(strSlopeUnit);
      GetDlgItem(IDC_RIGHT_SLOPE_UNIT_3)->SetWindowText(strSlopeUnit);
   }

   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   DDX_MetaFileStatic(pDX, IDC_IMAGE, m_Image, AfxGetResourceHandle(), _T("Superelevation"), _T("Metafile") );

   if ( pDX->m_bSaveAndValidate && m_bCorrectForSuperelevation)
   {
      if ( !(m_SuperelevationPoint[0].Station < m_SuperelevationPoint[1].Station && m_SuperelevationPoint[1].Station < m_SuperelevationPoint[2].Station) )
      {
         pDX->PrepareEditCtrl(IDC_STATION_1);
         AfxMessageBox(_T("Superelevation Profile Stations must be given in order of increasing station"));
         pDX->Fail();
      }
   }
}


BEGIN_MESSAGE_MAP(CCurvelReportWizardStep2, CPropertyPage)
   ON_COMMAND(ID_HELP,OnHelp)
   ON_BN_CLICKED(IDC_CROWN_OPTION, &CCurvelReportWizardStep2::OnBnClickedCrownOption)
END_MESSAGE_MAP()


// CCurvelReportWizardStep2 message handlers

LRESULT CCurvelReportWizardStep2::OnWizardNext()
{
   if ( !UpdateData() )
      return -1;

   CCurvelReportWizard* pParent = (CCurvelReportWizard*)GetParent();
   return pParent->GetNextPage();
   //return CPropertyPage::OnWizardNext();
}

LRESULT CCurvelReportWizardStep2::OnWizardBack()
{
   CCurvelReportWizard* pParent = (CCurvelReportWizard*)GetParent();
   return pParent->GetBackPage();

//   return CPropertyPage::OnWizardBack();
}

BOOL CCurvelReportWizardStep2::OnKillActive()
{
   return TRUE;
   //return CPropertyPage::OnKillActive();
}

BOOL CCurvelReportWizardStep2::OnInitDialog()
{
   // Put our logo on the dialog
   HICON hIcon = (HICON)LoadImage(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_CURVEL),IMAGE_ICON,0,0,LR_DEFAULTSIZE);
   SetIcon(hIcon,FALSE);

   CCurvelReportWizard* pParent = (CCurvelReportWizard*)GetParent();
   if ( pParent->m_pRptSpec )
      InitFromRptSpec();

   CPropertyPage::OnInitDialog();

   OnBnClickedCrownOption();

   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}

void CCurvelReportWizardStep2::InitFromRptSpec()
{
   CCurvelReportWizard* pParent = (CCurvelReportWizard*)GetParent();
   m_bCorrectForSuperelevation = pParent->m_pRptSpec->CorrectForSuperelevation();
   m_ProfileGradeOffset        = pParent->m_pRptSpec->GetProfileGradeOffset();
   m_SuperelevationPoint[0]    = pParent->m_pRptSpec->GetSuperelevationPoint(0);
   m_SuperelevationPoint[1]    = pParent->m_pRptSpec->GetSuperelevationPoint(1);
   m_SuperelevationPoint[2]    = pParent->m_pRptSpec->GetSuperelevationPoint(2);
}

bool CCurvelReportWizardStep2::CorrectForSuperelevation()
{
   return m_bCorrectForSuperelevation;
}

void CCurvelReportWizardStep2::UpdateReportSpecification(std::shared_ptr<CCurvelReportSpecification>& pRptSpec)
{
   pRptSpec->CorrectForSuperelevation(m_bCorrectForSuperelevation);
   if ( m_bCorrectForSuperelevation )
   {
      pRptSpec->SetProfileGradeOffset(m_ProfileGradeOffset);
      pRptSpec->SetSuperelevationPoint(0,m_SuperelevationPoint[0]);
      pRptSpec->SetSuperelevationPoint(1,m_SuperelevationPoint[1]);
      pRptSpec->SetSuperelevationPoint(2,m_SuperelevationPoint[2]);
   }
}

void CCurvelReportWizardStep2::OnHelp()
{
   EAFHelp(EAFGetDocument()->GetDocumentationSetName(), IDH_CURVEL_STEP2);
}

void CCurvelReportWizardStep2::OnBnClickedCrownOption()
{
   // TODO: Add your control notification handler code here
   BOOL bEnable = IsDlgButtonChecked(IDC_CROWN_OPTION);
   GetDlgItem(IDC_CROWN_OFFSET_LABEL)->EnableWindow(bEnable);
   GetDlgItem(IDC_CROWN_OFFSET)->EnableWindow(bEnable);
   GetDlgItem(IDC_CROWN_OFFSET_UNIT)->EnableWindow(bEnable);

   GetDlgItem(IDC_STATION_LABEL_1)->EnableWindow(bEnable);
   GetDlgItem(IDC_STATION_1)->EnableWindow(bEnable);
   GetDlgItem(IDC_LEFT_SLOPE_LABEL_1)->EnableWindow(bEnable);
   GetDlgItem(IDC_LEFT_SLOPE_1)->EnableWindow(bEnable);
   GetDlgItem(IDC_LEFT_SLOPE_UNIT_1)->EnableWindow(bEnable);
   GetDlgItem(IDC_RIGHT_SLOPE_LABEL_1)->EnableWindow(bEnable);
   GetDlgItem(IDC_RIGHT_SLOPE_1)->EnableWindow(bEnable);
   GetDlgItem(IDC_RIGHT_SLOPE_UNIT_1)->EnableWindow(bEnable);

   GetDlgItem(IDC_STATION_LABEL_2)->EnableWindow(bEnable);
   GetDlgItem(IDC_STATION_2)->EnableWindow(bEnable);
   GetDlgItem(IDC_LEFT_SLOPE_LABEL_2)->EnableWindow(bEnable);
   GetDlgItem(IDC_LEFT_SLOPE_2)->EnableWindow(bEnable);
   GetDlgItem(IDC_LEFT_SLOPE_UNIT_2)->EnableWindow(bEnable);
   GetDlgItem(IDC_RIGHT_SLOPE_LABEL_2)->EnableWindow(bEnable);
   GetDlgItem(IDC_RIGHT_SLOPE_2)->EnableWindow(bEnable);
   GetDlgItem(IDC_RIGHT_SLOPE_UNIT_2)->EnableWindow(bEnable);

   GetDlgItem(IDC_STATION_LABEL_3)->EnableWindow(bEnable);
   GetDlgItem(IDC_STATION_3)->EnableWindow(bEnable);
   GetDlgItem(IDC_LEFT_SLOPE_LABEL_3)->EnableWindow(bEnable);
   GetDlgItem(IDC_LEFT_SLOPE_3)->EnableWindow(bEnable);
   GetDlgItem(IDC_LEFT_SLOPE_UNIT_3)->EnableWindow(bEnable);
   GetDlgItem(IDC_RIGHT_SLOPE_LABEL_3)->EnableWindow(bEnable);
   GetDlgItem(IDC_RIGHT_SLOPE_3)->EnableWindow(bEnable);
   GetDlgItem(IDC_RIGHT_SLOPE_UNIT_3)->EnableWindow(bEnable);
}
