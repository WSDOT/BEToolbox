///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2022  Washington State Department of Transportation
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
#include "GirCompDlgBar.h"

class CGirCompChildFrame :
   public CEAFChildFrame
{
public:
	DECLARE_DYNCREATE(CGirCompChildFrame)

   void SetUnitsMode(eafTypes::UnitMode um);
   void UpdateUnitsDisplayMode();

   void UpdateData(BOOL bUpdate);


protected:
   CGirCompChildFrame(void);
   ~CGirCompChildFrame(void);

   afx_msg void OnUpdate();
   afx_msg void OnUSUnits();
   afx_msg void OnSIUnits();
   afx_msg void OnAdd();
   afx_msg void OnRemove();
   afx_msg void OnUpdateRemove(CCmdUI* pCmdUI);
   afx_msg LRESULT OnCommandHelp(WPARAM, LPARAM lParam);

   DECLARE_MESSAGE_MAP()
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

protected:
   CGirCompDialogTopBar m_TopDlgBar;
   CGirCompDialogLeftBar m_LeftDlgBar;

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
