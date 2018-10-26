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

#include "PGStablePointLoadGrid.h"
#include "PGStableGirderSectionGrid.h"
#include "PGStableGirderControl.h"

// CPGStableNonprismaticGirder dialog

class CPGStableNonprismaticGirder : public CDialog
{
	DECLARE_DYNAMIC(CPGStableNonprismaticGirder)

public:
	CPGStableNonprismaticGirder(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPGStableNonprismaticGirder();

// Dialog Data
	enum { IDD = IDD_NONPRISMATIC_GIRDER };

   std::vector<std::pair<Float64,Float64>> GetGirderProfile();
   std::vector<std::pair<Float64,Float64>> GetStrandProfile(int strandType);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

   void FillComboBoxes(INT xIDC,INT yIDC);

   CPGStableGirderSectionGrid* m_pGirderSectionGrid;
   CPGStablePointLoadGrid* m_pPointLoadGrid;

   CPGStableGirderControl m_ctrlGirder;

	DECLARE_MESSAGE_MAP()

public:
   afx_msg void OnAddGirderSection();
   afx_msg void OnRemoveGirderSection();
   afx_msg void OnUpdateRemoveGirderSection(CCmdUI* pCmdUI);
   BOOL AreGirderSectionsSelected();

   afx_msg void OnAddPointLoad();
   afx_msg void OnRemovePointLoad();
   afx_msg void OnUpdateRemovePointLoad(CCmdUI* pCmdUI);
   BOOL AreLoadsSelected();

   void SetGirderLength(Float64 Lg);

   virtual BOOL OnInitDialog();
   afx_msg void OnChange();
};
