///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2015  Washington State Department of Transportation
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
#include "BEToolboxDoc.h"
#include "CurvelReportSpecification.h"

class CCurvelDoc : public CBEToolboxDoc
{
	DECLARE_DYNCREATE(CCurvelDoc)

public:
	CCurvelDoc();
	virtual ~CCurvelDoc();

   virtual CString GetToolbarSectionName();

   CReportBuilderManager* GetReportBuilderManager();
   boost::shared_ptr<CReportSpecificationBuilder> GetReportSpecificationBuilder();
   boost::shared_ptr<CReportSpecification> GetDefaultReportSpecification();

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
   /// called when a document is created (New or Open)
   virtual BOOL Init(); 

   // Called by the framework when the document is to be loaded and saved
   virtual HRESULT WriteTheDocument(IStructuredSave* pStrSave);
   virtual HRESULT LoadTheDocument(IStructuredLoad* pStrLoad);

   BOOL OpenOldFormat(LPCTSTR lpszPathName);

   DECLARE_MESSAGE_MAP()
public:

   // over-ride default behavior by destroying column
   virtual void OnCloseDocument();

private:
   CReportBuilderManager m_RptMgr;

   boost::shared_ptr<CReportSpecificationBuilder> m_pRptSpecBuilder;
   boost::shared_ptr<CCurvelReportSpecification> m_pDefaultRptSpec; // this is the default spec that is
   // used to get things started... after the report is displayed, the view holds the real repost spec
};
