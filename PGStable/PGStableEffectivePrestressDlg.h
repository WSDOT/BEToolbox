///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright � 1999-2023  Washington State Department of Transportation
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
#include "PGStableFpeGrid.h"
#include "PGStableStrands.h"

// CPGStableEffectivePrestressDlg dialog

class CPGStableEffectivePrestressDlg : public CDialog
{
	DECLARE_DYNAMIC(CPGStableEffectivePrestressDlg)

public:
	CPGStableEffectivePrestressDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPGStableEffectivePrestressDlg();

   CPGStableStrands m_Strands;

// Dialog Data
	enum { IDD = IDD_PGSTABLEEFFECTIVEPRESTRESSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support

   CPGStableFpeGrid m_Grid;

	DECLARE_MESSAGE_MAP()
public:
   virtual BOOL OnInitDialog() override;
   afx_msg void OnAdd();
   afx_msg void OnRemove();
};
