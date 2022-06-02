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

// CCurvelDoc document
#include "..\BEToolboxDoc.h"
#include "CurvelReportSpecification.h"
#include <Curvel.h>

class CCurvelDoc : public CBEToolboxDoc
{
	DECLARE_DYNCREATE(CCurvelDoc)

public:
	CCurvelDoc();
	virtual ~CCurvelDoc();

   virtual CString GetToolbarSectionName() override;

   CReportBuilderManager* GetReportBuilderManager();
   std::shared_ptr<CReportSpecificationBuilder> GetReportSpecificationBuilder();
   std::shared_ptr<CReportSpecification> GetDefaultReportSpecification();

   Curvel* GetCurvelXML();

#ifdef _DEBUG
	virtual void AssertValid() const override;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const override;
#endif
#endif

	virtual BOOL OnNewDocument() override;

protected:
   virtual BOOL OpenTheDocument(LPCTSTR lpszPathName) override;
   virtual BOOL SaveTheDocument(LPCTSTR lpszPathName) override;

   virtual void LoadDocumentSettings() override;
   virtual void SaveDocumentSettings() override;

   virtual CString GetDocumentationRootLocation() override;

   virtual void OnOldFormat(LPCTSTR lpszPathName) override;

   std::unique_ptr<Curvel> m_CurvelXML;
   CComPtr<IUnitServer> m_DocUnitServer;
   CComPtr<IUnitConvert> m_DocConvert;

   afx_msg void OnHelpFinder();
   DECLARE_MESSAGE_MAP()

private:
   CReportBuilderManager m_RptMgr;

   std::shared_ptr<CReportSpecificationBuilder> m_pRptSpecBuilder;
   std::shared_ptr<CCurvelReportSpecification> m_pDefaultRptSpec; // this is the default spec that is
   // used to get things started... after the report is displayed, the view holds the real repost spec
};
