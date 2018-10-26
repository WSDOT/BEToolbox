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

// CurvelReportWizardPage3.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "CurvelReportWizardStep3.h"
#include "CurvelReportWizard.h"

#include <EAF\EAFDocument.h>


// CCurvelReportWizardStep3 dialog

IMPLEMENT_DYNAMIC(CCurvelReportWizardStep3, CPropertyPage)

CCurvelReportWizardStep3::CCurvelReportWizardStep3()
	: CPropertyPage(CCurvelReportWizardStep3::IDD)
{
   m_psp.dwFlags |= PSP_DEFAULT | PSP_HASHELP | PSP_USEHEADERTITLE/* | PSP_USEHEADERSUBTITLE*/;
   m_psp.pszHeaderTitle    = _T("Define locations where elevations are computed");
   //m_psp.pszHeaderSubTitle = _T("Elevations can be computed at individual stations, increments, and on skews");
}

CCurvelReportWizardStep3::~CCurvelReportWizardStep3()
{
}

void CCurvelReportWizardStep3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

   DDV_GXGridWnd(pDX,&m_IndividualStationGrid);
   DDV_GXGridWnd(pDX,&m_StationRangeGrid);
   DDV_GXGridWnd(pDX,&m_SkewLineGrid);

   if ( pDX->m_bSaveAndValidate )
   {
      m_IndividualStations = m_IndividualStationGrid.GetStations();
      m_StationRanges = m_StationRangeGrid.GetStations();
      m_SkewLines = m_SkewLineGrid.GetSkewLines();
   }
   else
   {
      m_IndividualStationGrid.SetStations(m_IndividualStations);
      m_StationRangeGrid.SetStations(m_StationRanges);
      m_SkewLineGrid.SetSkewLines(m_SkewLines);
   }
}

BOOL CCurvelReportWizardStep3::PreTranslateMessage(MSG* pMsg)
{
   UpdateDialogControls(this,TRUE);
   return CPropertyPage::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CCurvelReportWizardStep3, CPropertyPage)
   ON_COMMAND(ID_HELP,OnHelp)
   ON_BN_CLICKED(IDC_INDIVIDUAL_STATION_ADD, &CCurvelReportWizardStep3::OnAddIndividualStation)
   ON_BN_CLICKED(IDC_INDIVIDUAL_STATION_REMOVE, &CCurvelReportWizardStep3::OnRemoveIndividualStation)
   ON_UPDATE_COMMAND_UI(IDC_INDIVIDUAL_STATION_REMOVE, &CCurvelReportWizardStep3::OnUpdateRemoveIndividualStation)

   ON_BN_CLICKED(IDC_STATION_RANGE_ADD, &CCurvelReportWizardStep3::OnAddStationRange)
   ON_BN_CLICKED(IDC_STATION_RANGE_REMOVE, &CCurvelReportWizardStep3::OnRemoveStationRange)
   ON_UPDATE_COMMAND_UI(IDC_STATION_RANGE_REMOVE, &CCurvelReportWizardStep3::OnUpdateRemoveStationRange)

   ON_BN_CLICKED(IDC_SKEW_LINE_ADD, &CCurvelReportWizardStep3::OnAddSkewLine)
   ON_BN_CLICKED(IDC_SKEW_LINE_REMOVE, &CCurvelReportWizardStep3::OnRemoveSkewLine)
   ON_UPDATE_COMMAND_UI(IDC_SKEW_LINE_REMOVE, &CCurvelReportWizardStep3::OnUpdateRemoveSkewLine)
END_MESSAGE_MAP()


// CCurvelReportWizardStep3 message handlers

LRESULT CCurvelReportWizardStep3::OnWizardNext()
{
   if ( !UpdateData() )
      return -1;

   CCurvelReportWizard* pParent = (CCurvelReportWizard*)GetParent();
   return pParent->GetNextPage();
   //return CPropertyPage::OnWizardNext();
}

LRESULT CCurvelReportWizardStep3::OnWizardBack()
{
   CCurvelReportWizard* pParent = (CCurvelReportWizard*)GetParent();
   return pParent->GetBackPage();

//   return CPropertyPage::OnWizardBack();
}

BOOL CCurvelReportWizardStep3::OnSetActive()
{
   CCurvelReportWizard* pParent = (CCurvelReportWizard*)GetParent();
   m_IndividualStationGrid.CorrectForSuperelevation(pParent->m_Step2.CorrectForSuperelevation());
   m_StationRangeGrid.CorrectForSuperelevation(pParent->m_Step2.CorrectForSuperelevation());
   m_SkewLineGrid.CorrectForSuperelevation(pParent->m_Step2.CorrectForSuperelevation());

   return CPropertyPage::OnSetActive();
}

BOOL CCurvelReportWizardStep3::OnKillActive()
{
   return TRUE;
   //return CPropertyPage::OnKillActive();
}

BOOL CCurvelReportWizardStep3::OnInitDialog()
{
   CCurvelReportWizard* pParent = (CCurvelReportWizard*)GetParent();

   m_IndividualStationGrid.SubclassDlgItem(IDC_INDIVIDUAL_STATIONS, this);
   m_IndividualStationGrid.CustomInit();

   m_StationRangeGrid.SubclassDlgItem(IDC_STATION_RANGE,this);
   m_StationRangeGrid.CustomInit();

   m_SkewLineGrid.SubclassDlgItem(IDC_SKEW_LINE,this);
   m_SkewLineGrid.CustomInit();

   // Put our logo on the dialog
   HICON hIcon = (HICON)LoadImage(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_CURVEL),IMAGE_ICON,0,0,LR_DEFAULTSIZE);
   SetIcon(hIcon,FALSE);

   if ( pParent->m_pRptSpec )
      InitFromRptSpec();

   CPropertyPage::OnInitDialog();


   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}

void CCurvelReportWizardStep3::InitFromRptSpec()
{
   CCurvelReportWizard* pParent = (CCurvelReportWizard*)GetParent();

   m_IndividualStations = pParent->m_pRptSpec->GetIndividualStations();
   m_StationRanges      = pParent->m_pRptSpec->GetStationRanges();
   m_SkewLines          = pParent->m_pRptSpec->GetSkewLines();
}

void CCurvelReportWizardStep3::UpdateReportSpecification(std::shared_ptr<CCurvelReportSpecification>& pRptSpec)
{
   pRptSpec->SetIndividualStations(m_IndividualStations);
   pRptSpec->SetStationRanges(m_StationRanges);
   pRptSpec->SetSkewLines(m_SkewLines);
}

void CCurvelReportWizardStep3::OnHelp()
{
   EAFHelp(EAFGetDocument()->GetDocumentationSetName(), IDH_CURVEL_STEP3);
}

void CCurvelReportWizardStep3::OnAddIndividualStation()
{
   m_IndividualStationGrid.AddStation();
}

void CCurvelReportWizardStep3::OnRemoveIndividualStation()
{
   m_IndividualStationGrid.RemoveSelectedStations();
}

void CCurvelReportWizardStep3::OnUpdateRemoveIndividualStation(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(m_IndividualStationGrid.AreStationsSelected() ? TRUE : FALSE);
}

void CCurvelReportWizardStep3::OnAddStationRange()
{
   m_StationRangeGrid.AddStation();
}

void CCurvelReportWizardStep3::OnRemoveStationRange()
{
   m_StationRangeGrid.RemoveSelectedStations();
}

void CCurvelReportWizardStep3::OnUpdateRemoveStationRange(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(m_StationRangeGrid.AreStationsSelected() ? TRUE : FALSE);
}

void CCurvelReportWizardStep3::OnAddSkewLine()
{
   m_SkewLineGrid.AddSkewLine();
}

void CCurvelReportWizardStep3::OnRemoveSkewLine()
{
   m_SkewLineGrid.RemoveSelectedSkewLines();
}

void CCurvelReportWizardStep3::OnUpdateRemoveSkewLine(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(m_SkewLineGrid.AreSkewLinesSelected() ? TRUE : FALSE);
}
