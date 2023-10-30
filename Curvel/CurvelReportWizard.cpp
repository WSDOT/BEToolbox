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

// CurveReportWizard.cpp : implementation file
//

#include "stdafx.h"
#include "..\resource.h"
#include "CurvelReportWizard.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CCurvelReportWizard

IMPLEMENT_DYNAMIC(CCurvelReportWizard, CPropertySheet)

CCurvelReportWizard::CCurvelReportWizard(const std::shared_ptr<CCurvelReportSpecification>& pRptSpec, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(_T(""), pParentWnd, iSelectPage),
   m_pRptSpec(pRptSpec)
{
   Init();
}

CCurvelReportWizard::~CCurvelReportWizard()
{
}


BEGIN_MESSAGE_MAP(CCurvelReportWizard, CPropertySheet)
END_MESSAGE_MAP()


// CCurvelReportWizard message handlers
void CCurvelReportWizard::Init()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   AddPage(&m_Step1);
   AddPage(&m_Step2);
   AddPage(&m_Step3);

   m_Pages.push_back(CCurvelReportWizardStep1::IDD);
   m_Pages.push_back(CCurvelReportWizardStep2::IDD);
   m_Pages.push_back(CCurvelReportWizardStep3::IDD);
   m_CurrentPage = m_Pages.begin();

   m_psh.dwFlags |= PSH_WIZARD97 | PSH_HEADER | PSH_WATERMARK;
   m_psh.pszbmWatermark = MAKEINTRESOURCE(IDB_CURVEL_WATERMARK);
   m_psh.pszbmHeader = MAKEINTRESOURCE(IDB_CURVEL_HEADER);
   m_psh.hInstance = AfxGetInstanceHandle(); 

   SetWizardMode();
}

LRESULT CCurvelReportWizard::GetNextPage()
{
   m_CurrentPage++;

   if ( IsLastPage() )
   {
      SetWizardButtons(PSWIZB_BACK | PSWIZB_FINISH);
   }
   else
   {
      SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
   }

   return *m_CurrentPage;
}

LRESULT CCurvelReportWizard::GetBackPage()
{
   m_CurrentPage--;

   if ( IsFirstPage() )
   {
      SetWizardButtons(PSWIZB_NEXT);
   }
   else
   {
      SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
   }
   
   return *m_CurrentPage;
}

bool CCurvelReportWizard::IsLastPage()
{
   return ( *m_CurrentPage == m_Pages.back() ? true : false );
}

bool CCurvelReportWizard::IsFirstPage()
{
   return (*m_CurrentPage == m_Pages.front() ? true : false);
}

BOOL CCurvelReportWizard::OnInitDialog()
{
   BOOL bResult = CPropertySheet::OnInitDialog();

   return bResult;
}

void CCurvelReportWizard::UpdateReportSpecification(std::shared_ptr<CCurvelReportSpecification>& pRptSpec)
{
   m_Step1.UpdateReportSpecification(pRptSpec);
   m_Step2.UpdateReportSpecification(pRptSpec);
   m_Step3.UpdateReportSpecification(pRptSpec);
}
