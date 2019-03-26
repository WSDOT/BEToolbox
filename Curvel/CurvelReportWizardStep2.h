///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2019  Washington State Department of Transportation
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

#include "CurvelReportSpecification.h"

// CCurvelReportWizardStep2 dialog

class CCurvelReportWizardStep2 : public CPropertyPage
{
	DECLARE_DYNAMIC(CCurvelReportWizardStep2)

public:
	CCurvelReportWizardStep2();
	virtual ~CCurvelReportWizardStep2();

   bool CorrectForSuperelevation();
   void UpdateReportSpecification(std::shared_ptr<CCurvelReportSpecification>& pRptSpec);

// Dialog Data
	enum { IDD = IDD_CURVEL_WIZ_2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support

   void InitFromRptSpec();

   afx_msg void OnHelp();

   bool m_bCorrectForSuperelevation;
   Float64 m_ProfileGradeOffset;
   SuperelevationProfilePoint m_SuperelevationPoint[3];

   CMetaFileStatic m_Image;

	DECLARE_MESSAGE_MAP()
public:
   virtual LRESULT OnWizardNext() override;
   virtual LRESULT OnWizardBack() override;
   virtual BOOL OnInitDialog() override;
   virtual BOOL OnKillActive() override;
   afx_msg void OnBnClickedCrownOption();
};
