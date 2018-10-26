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

// CGenCompDoc document
#include "BEToolboxDoc.h"
#include <WBFLTools.h>
#include <WBFLUnitServer.h>
#include <ReportManager\ReportManager.h>
#include <WBFLSections.h>

class CGenCompDoc : public CBEToolboxDoc
{
	DECLARE_DYNCREATE(CGenCompDoc)

public:
	CGenCompDoc();
	virtual ~CGenCompDoc();

   virtual CString GetToolbarSectionName();

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

   std::vector<std::pair<Float64,Float64>> m_PrimaryPoints;
   std::vector<std::pair<Float64,Float64>> m_SecondaryPoints;
   Float64 m_ModularRatio;

   DECLARE_MESSAGE_MAP()
public:

   // over-ride default behavior by destroying column
   virtual void OnCloseDocument();

   CReportBuilderManager m_RptMgr;
};
