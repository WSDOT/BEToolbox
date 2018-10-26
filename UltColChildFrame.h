///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2017  Washington State Department of Transportation
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
#include <EAF\EAFChildFrame.h>
#include <EAF\EAFTypes.h>
#include "UltColDlgBar.h"

class CUltColChildFrame :
   public CEAFChildFrame
{
public:
	DECLARE_DYNCREATE(CUltColChildFrame)

   void SetColumnParameters(Float64 diameter, Float64 fc, Float64 cover, Float64 As, Float64 Es, Float64 fy,Float64 ecl,Float64 etl);
   void SetUnitsMode(eafTypes::UnitMode um);

protected:
   CUltColChildFrame(void);
   ~CUltColChildFrame(void);

   afx_msg void OnUpdate();
   afx_msg void OnUSUnits();
   afx_msg void OnSIUnits();

   afx_msg BOOL OnToolTipNotify(UINT id,NMHDR* pNMHDR, LRESULT* pResult);
   
   DECLARE_MESSAGE_MAP()
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   afx_msg LRESULT OnCommandHelp(WPARAM, LPARAM lParam);

protected:
   CUltColDlgBar m_DlgBar;

public:
#if defined _DEBUG
   virtual void AssertValid() const override;
#endif // _DEBUG
protected:
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs) override;
public:

   // Must over-ride this method and call AFX_MANAGE_STATE(AfxGetStaticModuleState()) before
   // calling base class method
   virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr) override;
};
