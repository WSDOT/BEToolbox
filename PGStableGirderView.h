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

// CPGStableGirderView form view
#include "PGStableFormView.h"
#include "PGStablePrismaticGirder.h"
#include "PGStableNonprismaticGirder.h"

class CPGStableGirderView : public CPGStableFormView
{
	DECLARE_DYNCREATE(CPGStableGirderView)

protected:
	CPGStableGirderView();           // protected constructor used by dynamic creation
	virtual ~CPGStableGirderView();

public:
	enum { IDD = IDD_PGSTABLEGIRDERVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const override;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const override;
#endif
#endif

   void DoDataExchange(CDataExchange* pDX);

   virtual void OnActivateView() override;
   virtual void OnDeactivateView() override;
   virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,	DWORD dwRequestedStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,CCreateContext* pContext) override;

protected:
   BOOL m_bViewInitialized; // set to FALSE until OnInitialUpdate completes. The child dialog windows are not created until OnInitialUpdate is called
   CPGStablePrismaticGirder m_Prismatic;
   CPGStableNonprismaticGirder m_Nonprismatic;

	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnSwapUI();
   afx_msg LRESULT OnCommandHelp(WPARAM, LPARAM lParam);
   virtual void OnInitialUpdate() override;
protected:
   virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/) override;
public:
   afx_msg void OnChange();
};


