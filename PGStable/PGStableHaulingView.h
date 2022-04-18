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

#include <set>
#include "afxcmn.h"
#include "PGStableStrands.h"
#include "PGStableFormView.h"



// CPGStableHaulingView form view

class CPGStableHaulingView : public CPGStableFormView
{
	DECLARE_DYNCREATE(CPGStableHaulingView)

protected:
	CPGStableHaulingView();           // protected constructor used by dynamic creation
	virtual ~CPGStableHaulingView();

	CEdit	   m_ctrlEc;
   CEdit    m_ctrlFc;
   CEdit    m_ctrlK1;
   CEdit    m_ctrlK2;
   CString m_strUserEc;
   void UpdateEc();


   void GetMaxFpe(Float64* pFpeStraight,Float64* pFpeHarped,Float64* pFpeTemp);

   CPGStableHaulingCriteria m_HaulingCriteria;

public:
	enum { IDD = IDD_PGSTABLEHAULINGVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const override;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const override;
#endif
#endif

   virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,	DWORD dwRequestedStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,CCreateContext* pContext) override;


protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
   virtual void OnActivateView() override;
   virtual void OnDeactivateView() override;

   std::shared_ptr<CReportSpecification> m_pRptSpec;
   std::shared_ptr<CReportBrowser> m_pBrowser; // this is the actual browser window that displays the report
   void RefreshReport();

   void UpdateFpeControls();
   void UpdateCriteriaControls();

public:
   afx_msg void OnUserEc();
   afx_msg void OnChangeFc();
   afx_msg void OnChange();
   afx_msg void OnEditFpe();
   afx_msg void OnPrint();
   afx_msg void OnPrintDirect();
   afx_msg void OnClickedHaulingTensionMaxCrown();
   afx_msg void OnClickedHaulingTensionMaxSuper();
   afx_msg LRESULT OnCommandHelp(WPARAM, LPARAM lParam);
   afx_msg void OnCmenuSelected(UINT id);
   virtual void OnInitialUpdate() override;

protected:
   virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/) override;
public:
   afx_msg void OnSize(UINT nType, int cx, int cy);
   afx_msg void OnWindTypeChanged();
   afx_msg void OnHaulTruckChanged();
};


