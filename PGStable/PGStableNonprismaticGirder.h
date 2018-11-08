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

#pragma once

#include "PGStablePointLoadGrid.h"
#include "PGStableGirderSectionGrid.h"
#include "PGStableGirderControl.h"

// CPGStableNonprismaticGirder dialog

class CPGStableNonprismaticGirder : public CDialog, public CPGStableGirderControlParent
{
	DECLARE_DYNAMIC(CPGStableNonprismaticGirder)

public:
	CPGStableNonprismaticGirder(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPGStableNonprismaticGirder();

// Dialog Data
	enum { IDD = IDD_NONPRISMATIC_GIRDER };

   // CPGStableGirderControlParent
   virtual std::vector<std::pair<Float64,Float64>> GetGirderProfile() override;
   virtual void GetStrandProfiles(std::vector<std::pair<Float64,Float64>>* pvStraight,std::vector<std::pair<Float64,Float64>>* pvHarped,std::vector<std::pair<Float64,Float64>>* pvTemp) override;

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support

   void FillComboBoxes(INT xIDC,INT yIDC);

   CPGStableGirderSectionGrid* m_pGirderSectionGrid;
   CPGStablePointLoadGrid* m_pPointLoadGrid;
   CPGStableGirderControl m_ctrlGirder;

   std::vector<StressPoints> m_StressPointCache;
   void InitStressPointCache(const stbGirder& girder);

	DECLARE_MESSAGE_MAP()

public:
   afx_msg LRESULT OnCommandHelp(WPARAM, LPARAM lParam);
   afx_msg void OnAddGirderSection();
   afx_msg void OnRemoveGirderSection();
   afx_msg void OnUpdateRemoveGirderSection(CCmdUI* pCmdUI);
   BOOL AreGirderSectionsSelected();

   afx_msg void OnAddPointLoad();
   afx_msg void OnRemovePointLoad();
   afx_msg void OnUpdateRemovePointLoad(CCmdUI* pCmdUI);
   afx_msg void OnPSMethodChanged();

   BOOL AreLoadsSelected();

   void SetGirderLength(Float64 Lg);

   virtual BOOL OnInitDialog() override;
   afx_msg void OnChange();
   afx_msg void OnBnClickedComputeStressPoints();
   afx_msg void OnBnClickedDefineStressPoints();
   afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
   virtual void OnCancel();
   virtual void OnOK();

   void OnUnitsChanged();
};
