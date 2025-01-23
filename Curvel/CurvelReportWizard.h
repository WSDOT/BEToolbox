///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2025  Washington State Department of Transportation
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

#pragma once

#include "CurvelReportWizardStep1.h"
#include "CurvelReportWizardStep2.h"
#include "CurvelReportWizardStep3.h"
#include "CurvelReportSpecification.h"

// CCurvelReportWizard

class CCurvelReportWizard : public CPropertySheet
{
	DECLARE_DYNAMIC(CCurvelReportWizard)

public:
   CCurvelReportWizard(const std::shared_ptr<CCurvelReportSpecification>& pRptSpec, CWnd* pParentWnd = nullptr, UINT iSelectPage = 0);
	virtual ~CCurvelReportWizard();

   LRESULT GetNextPage();
   LRESULT GetBackPage();

   void UpdateReportSpecification(std::shared_ptr<CCurvelReportSpecification>& pRptSpec);

protected:
	DECLARE_MESSAGE_MAP()

   void Init();

   bool IsLastPage();
   bool IsFirstPage();

   std::shared_ptr<const CCurvelReportSpecification> m_pRptSpec;

   std::vector<LRESULT> m_Pages;
   std::vector<LRESULT>::iterator m_CurrentPage;

   friend CCurvelReportWizardStep1;
   friend CCurvelReportWizardStep2;
   friend CCurvelReportWizardStep3;

public:
   CCurvelReportWizardStep1 m_Step1;
   CCurvelReportWizardStep2 m_Step2;
   CCurvelReportWizardStep3 m_Step3;
   
   virtual BOOL OnInitDialog();
};


