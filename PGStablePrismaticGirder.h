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

#include <MfcTools\MetaFileStatic.h>
#include "PGStableStressPointGrid.h"
#include "PGStablePointLoadGrid.h"
#include "PGStableGirderControl.h"

// CPGStablePrismaticGirder dialog

class CPGStablePrismaticGirder : public CDialog, public CPGStableGirderControlParent
{
	DECLARE_DYNAMIC(CPGStablePrismaticGirder)

public:
	CPGStablePrismaticGirder(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPGStablePrismaticGirder();

// Dialog Data
	enum { IDD = IDD_PRISMATIC_GIRDER };

   // CPGStableGirderControlParent
   virtual std::vector<std::pair<Float64,Float64>> GetGirderProfile() override;
   virtual void GetStrandProfiles(std::vector<std::pair<Float64,Float64>>* pvStraight,std::vector<std::pair<Float64,Float64>>* pvHarped,std::vector<std::pair<Float64,Float64>>* pvTemp) override;


protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support

   CMetaFileStatic m_StrandLayout;
   CPGStableGirderControl m_ctrlGirder;
   std::unique_ptr<CPGStablePointLoadGrid> m_pPointLoadGrid;
   std::unique_ptr<CPGStableStressPointGrid> m_pStressPointGrid;

   gpPoint2d m_pntTL, m_pntTR, m_pntBL, m_pntBR;
   gpPoint2d m_pntTLCache, m_pntTRCache, m_pntBLCache, m_pntBRCache;

   void InitStressPoints(Float64 Wtf,Float64 Wbf,Float64 Ytop,Float64 Hg);
   void UpdateStressPoints();

	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnAddPointLoad();
   afx_msg void OnRemovePointLoad();
   afx_msg void OnUpdateRemove(CCmdUI* pCmdUI);
   afx_msg LRESULT OnCommandHelp(WPARAM, LPARAM lParam);

   BOOL AreLoadsSelected();
   virtual BOOL OnInitDialog() override;
   afx_msg void OnPSMethodChanged();
   afx_msg void OnGirderChanged();
   afx_msg void OnBnClickedComputeStressPoints();
   afx_msg void OnBnClickedDefineStressPoints();
   afx_msg void OnChangedStressPointDimension();
   afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
   virtual void OnCancel();
   virtual void OnOK();

   void OnUnitsChanged();
};
