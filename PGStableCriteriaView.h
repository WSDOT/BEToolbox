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

#include "PGStableCriteria.h"


// CPGStableCriteriaView form view

class CPGStableCriteriaView : public CFormView
{
	DECLARE_DYNCREATE(CPGStableCriteriaView)

protected:
	CPGStableCriteriaView();           // protected constructor used by dynamic creation
	virtual ~CPGStableCriteriaView();

public:
	enum { IDD = IDD_PGSTABLECRITERIAVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

   CPGStableCriteria m_LiftingCriteria;
   CPGStableCriteria m_HaulingCriteria;

   virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,	DWORD dwRequestedStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,CCreateContext* pContext);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

	DECLARE_MESSAGE_MAP()
   virtual void OnActivateView(BOOL bActivate,CView* pActivateView,CView* pDeactivateView);

public:
   virtual void OnInitialUpdate();

   afx_msg void OnClickedLiftingTensionMax();
   afx_msg void OnClickedHaulingTensionMax();
};


