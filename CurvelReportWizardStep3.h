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

#include "CurvelIndividualStationGrid.h"
#include "CurvelStationRangeGrid.h"
#include "CurvelSkewLineGrid.h"

class CCurvelReportSpecification;

// CCurvelReportWizardStep3 dialog

class CCurvelReportWizardStep3 : public CPropertyPage
{
	DECLARE_DYNAMIC(CCurvelReportWizardStep3)

public:
	CCurvelReportWizardStep3();
	virtual ~CCurvelReportWizardStep3();

   void UpdateReportSpecification(CCurvelReportSpecification* pRptSpec);

// Dialog Data
	enum { IDD = IDD_CURVEL_WIZ_3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   virtual BOOL PreTranslateMessage(MSG* pMsg);

   void InitFromRptSpec();

   afx_msg void OnHelp();

   CCurvelIndividualStationGrid m_IndividualStationGrid;
   std::vector<IndividualStation> m_IndividualStations;

   CCurvelStationRangeGrid m_StationRangeGrid;
   std::vector<StationRange> m_StationRanges;

   CCurvelSkewLineGrid m_SkewLineGrid;
   std::vector<SkewLine> m_SkewLines;

	DECLARE_MESSAGE_MAP()
public:
   virtual LRESULT OnWizardNext();
   virtual LRESULT OnWizardBack();
   virtual BOOL OnInitDialog();
   virtual BOOL OnSetActive();
   virtual BOOL OnKillActive();

   afx_msg void OnAddIndividualStation();
   afx_msg void OnRemoveIndividualStation();
   afx_msg void OnUpdateRemoveIndividualStation(CCmdUI* pCmdUI);

   afx_msg void OnAddStationRange();
   afx_msg void OnRemoveStationRange();
   afx_msg void OnUpdateRemoveStationRange(CCmdUI* pCmdUI);

   afx_msg void OnAddSkewLine();
   afx_msg void OnRemoveSkewLine();
   afx_msg void OnUpdateRemoveSkewLine(CCmdUI* pCmdUI);
};
