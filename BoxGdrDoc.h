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

// CBoxGdrDoc document
#include "BEToolboxDoc.h"
#include <WBFLTools.h>
#include <WBFLGeometry.h>
#include <WBFLUnitServer.h>
#include <ReportManager\ReportManager.h>

struct BOXGDRDIMENSIONS
{
   Float64 D;
   Float64 T;
   IndexType N;
   Float64 W;
   Float64 ST;
   Float64 SB;
   Float64 FT;
   Float64 FB;
   Float64 EL;
   Float64 CL;
   Float64 BL;
   Float64 ER;
   Float64 CR;
   Float64 BR;
};

class CBoxGdrDoc : public CBEToolboxDoc
{
	DECLARE_DYNCREATE(CBoxGdrDoc)

public:
	CBoxGdrDoc();
	virtual ~CBoxGdrDoc();

   virtual CString GetToolbarSectionName();

   IndexType GetProblemCount() const;
   void AddProblem(const BOXGDRDIMENSIONS& problem);
   void AddProblems(const std::vector<BOXGDRDIMENSIONS>& problems);
   void RemoveProblem(IndexType idx);
   const BOXGDRDIMENSIONS& GetProblem(IndexType idx);
   void ClearProblems();

   void ComputeShapeProperties(IndexType idx,IShapeProperties** ppShapeProperties);

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

   virtual void LoadDocumentSettings();
   virtual void SaveDocumentSettings();

   virtual CString GetDocumentationRootLocation();

   virtual void OnOldFormat(LPCTSTR lpszPathName);

   std::vector<BOXGDRDIMENSIONS> m_Problems;

   afx_msg void OnHelpFinder();
	DECLARE_MESSAGE_MAP()
public:

   // over-ride default behavior by destroying column
   virtual void OnCloseDocument();

   CReportBuilderManager m_RptMgr;

private:
   CEAFToolBar* m_pMyToolBar;
};
