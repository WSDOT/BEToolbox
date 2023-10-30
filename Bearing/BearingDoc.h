///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2023  Washington State Department of Transportation
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

// CBearingDoc document
#include "..\BEToolboxDoc.h"
#include <WBFLTools.h>
#include <WBFLUnitServer.h>
#include <ReportManager\ReportManager.h>
#include <Math\PiecewiseFunction.h>
#include <EngTools\Bearing.h>
#include <EngTools\BearingLoads.h>
#include <EngTools\BearingCalculator.h>


class CBearingDoc : public CBEToolboxDoc
{
	DECLARE_DYNCREATE(CBearingDoc)

public:
	CBearingDoc();
	virtual ~CBearingDoc();

   virtual CString GetToolbarSectionName() override;

#ifdef _DEBUG
	virtual void AssertValid() const override;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const override;
#endif
#endif

protected:
   /// called when a document is created (New or Open)
   virtual BOOL Init(); 

   void InitWSDOT();
   void InitLRFD();
   void InitGuideSpec();

   // Called by the framework when the document is to be loaded and saved
   virtual HRESULT WriteTheDocument(IStructuredSave* pStrSave) override;
   virtual HRESULT LoadTheDocument(IStructuredLoad* pStrLoad) override;

   virtual void LoadDocumentSettings() override;
   virtual void SaveDocumentSettings() override;

   virtual CString GetDocumentationRootLocation() override;

   afx_msg void OnHelpFinder();
	DECLARE_MESSAGE_MAP()

public:
   virtual void OnCloseDocument() override;

   void SetBearing(const WBFL::EngTools::Bearing& brg, const WBFL::EngTools::BearingLoads& brg_loads, const WBFL::EngTools::BearingCalculator& brg_calc);
   const WBFL::EngTools::Bearing& GetBearing() const;
   const WBFL::EngTools::BearingLoads& GetBearingLoads() const;
   const WBFL::EngTools::BearingCalculator& GetBearingCalculator() const;

private:


	WBFL::EngTools::Bearing m_bearing;
	WBFL::EngTools::BearingLoads m_bearing_loads;
	WBFL::EngTools::BearingCalculator m_bearing_calculator;

};
