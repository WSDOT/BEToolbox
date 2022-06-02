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

// CGenCompDoc document
#include "..\BEToolboxDoc.h"
#include <ReportManager\ReportManager.h>
#include <GenComp.h>

class CGenCompDoc : public CBEToolboxDoc
{
	DECLARE_DYNCREATE(CGenCompDoc)

public:
	CGenCompDoc();
	virtual ~CGenCompDoc();

   virtual CString GetToolbarSectionName() override;

   void Clear();

   void SetModularRatio(Float64 n);
   Float64 GetModularRatio() const;

   void ClearPrimaryPoints();
   void AddPrimaryPoint(Float64 x,Float64 y);
   void AddPrimaryPoints(const std::vector<std::pair<Float64,Float64>>& points);
   const std::vector<std::pair<Float64,Float64>>& GetPrimaryPoints();

   void ClearSecondaryPoints();
   void AddSecondaryPoint(Float64 x,Float64 y);
   void AddSecondaryPoints(const std::vector<std::pair<Float64,Float64>>& points);
   const std::vector<std::pair<Float64,Float64>>& GetSecondaryPoints();

   CComPtr<IPolyShape> GetPrimaryShape();
   CComPtr<ICompositeSection> GetCompositeSection();


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

   std::unique_ptr<GenComp> m_GenCompXML;
   CComPtr<IUnitServer> m_DocUnitServer;
   CComPtr<IUnitConvert> m_DocConvert;

   std::vector<std::pair<Float64,Float64>> m_PrimaryPoints;
   std::vector<std::pair<Float64,Float64>> m_SecondaryPoints;

   afx_msg void OnHelpFinder();
   DECLARE_MESSAGE_MAP()

public:

   CReportBuilderManager m_RptMgr;
};
