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


// CM3CReportView view

#include <EAF\EAFReportView.h>

class CM3CReportView : public CEAFReportView
{
	DECLARE_DYNAMIC(CM3CReportView)

protected:
   CM3CReportView();           // protected constructor used by dynamic creation
	virtual ~CM3CReportView();

#ifdef _DEBUG
	virtual void AssertValid() const override;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const override;
#endif
#endif

   virtual BOOL CanEditReport() override { return FALSE; }

   virtual LPCTSTR GetReportName() const = 0;

protected:
	DECLARE_MESSAGE_MAP()

public:
   virtual void OnInitialUpdate() override;
};

class CM3CAnalysisReportView : public CM3CReportView
{
   DECLARE_DYNCREATE(CM3CAnalysisReportView)

protected:
   virtual LPCTSTR GetReportName() const override { return _T("M3C"); }
   DECLARE_MESSAGE_MAP()
};

class CM3CMaterialDetailsReportView : public CM3CReportView
{
   DECLARE_DYNCREATE(CM3CMaterialDetailsReportView)

protected:
   virtual LPCTSTR GetReportName() const override { return _T("M3CMaterialsDetails"); }
   DECLARE_MESSAGE_MAP()
};

class CM3CAnalysisDetailsReportView : public CM3CReportView
{
   DECLARE_DYNCREATE(CM3CAnalysisDetailsReportView)

public:
   void CurvatureChanged(IndexType idx);

protected:
   virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) override;
   virtual BOOL CanEditReport() override { return TRUE; }
   virtual LPCTSTR GetReportName() const override { return _T("M3CAnalysisDetails"); }
   virtual CWnd* CreateEditButton() override;
   void UpdateCurvatureList();
   DECLARE_MESSAGE_MAP()
};

class CCurvatureList : public CComboBox
{
   DECLARE_DYNAMIC(CCurvatureList);
public:
   CCurvatureList();
   virtual ~CCurvatureList();
   void Register(CM3CAnalysisDetailsReportView* pView);

protected:
   DECLARE_MESSAGE_MAP();

   afx_msg void OnSelChange();

   CM3CAnalysisDetailsReportView* m_pListener;
};
