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

// CM3CDoc document
#include "BEToolboxDoc.h"
#include <WBFLTools.h>
#include <WBFLGeometry.h>
#include <WBFLUnitServer.h>
#include <WBFLRCCapacity.h>
#include <ReportManager\ReportManager.h>

#include "M3CProblemParameters.h"

class CM3CDoc : public CBEToolboxDoc
{
	DECLARE_DYNCREATE(CM3CDoc)

public:
	CM3CDoc();
	virtual ~CM3CDoc();

   virtual CString GetToolbarSectionName() override;

   void SetProblemParameters(const CM3CProblemParameters& parameters, BOOL bUpdate = TRUE);
   const CM3CProblemParameters& GetProblemParameters() const;

   void GetMomentCurvature(IMomentCurvatureSolution** ppSolution) const;

#ifdef _DEBUG
	virtual void AssertValid() const override;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const override;
#endif
#endif

protected:
   /// called when a document is created (New or Open)
   virtual BOOL Init() override;

   // Called by the framework when the document is to be loaded and saved
   virtual HRESULT WriteTheDocument(IStructuredSave* pStrSave) override;
   virtual HRESULT LoadTheDocument(IStructuredLoad* pStrLoad) override;

   virtual void LoadDocumentSettings() override;
   virtual void SaveDocumentSettings() override;

   virtual CString GetDocumentationRootLocation() override;

   afx_msg void OnRefreshReport();
   afx_msg void OnHelpFinder();
	DECLARE_MESSAGE_MAP()

   CM3CProblemParameters m_ProblemParams;

public:

   // over-ride default behavior by destroying column
   virtual void OnCloseDocument();

   mutable bool m_bIsSolutionValid;
   mutable CComPtr<IMomentCurvatureSolution> m_Solution;

   void BuildConcreteModel(IStressStrain** ppConcrete) const;
   void BuildRebarModel(IStressStrain** ppSteel) const;
   void BuildColumnSection(IGeneralSection** ppSection) const;
   CReportBuilderManager m_RptMgr;
};
