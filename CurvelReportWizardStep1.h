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

#pragma once


// CCurvelReportWizardStep1 dialog
class CCurvelReportSpecification;

class CCurvelReportWizardStep1 : public CPropertyPage
{
	DECLARE_DYNAMIC(CCurvelReportWizardStep1)

public:
	CCurvelReportWizardStep1();
	virtual ~CCurvelReportWizardStep1();

   void UpdateReportSpecification(CCurvelReportSpecification* pRptSpec);

// Dialog Data
	enum { IDD = IDD_CURVEL_WIZ_1 };

   Float64 m_G1;
   Float64 m_G2;
   Float64 m_PVIStation;
   Float64 m_PVIElevation;
   Float64 m_Length;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

   void InitFromRptSpec();

   afx_msg void OnHelp();

   CMetaFileStatic m_Image;

	DECLARE_MESSAGE_MAP()
public:
   virtual LRESULT OnWizardNext();
   virtual BOOL OnSetActive();
   virtual BOOL OnInitDialog();
   virtual BOOL OnKillActive();
};
