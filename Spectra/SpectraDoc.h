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

// CSpectraDoc document
#include "..\BEToolboxDoc.h"
#include <WBFLTools.h>
#include <WBFLUnitServer.h>
#include <ReportManager\ReportManager.h>
#include "ResponseSpectra.h"
#include <Math\PwLinearFunction2dUsingPoints.h>

class CSpectraDoc : public CBEToolboxDoc
{
	DECLARE_DYNCREATE(CSpectraDoc)

public:
	CSpectraDoc();
	virtual ~CSpectraDoc();

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

   CString GetResourcePath();
   bool LoadSpectralData();

   SpecificationType m_SpecType;
   Float64 m_Lat, m_Lng;
   SiteClass m_SiteClass;

   SpectralValues* m_pValues;
   std::vector<std::shared_ptr<mathPwLinearFunction2dUsingPoints>> m_ZeroPeriodSiteFactors[nSpecTypes];
   std::vector<std::shared_ptr<mathPwLinearFunction2dUsingPoints>> m_ShortPeriodSiteFactors[nSpecTypes];
   std::vector<std::shared_ptr<mathPwLinearFunction2dUsingPoints>> m_LongPeriodSiteFactors[nSpecTypes];

public:
   void SetSpecification(SpecificationType specType);
   SpecificationType GetSpecification() const;
   void SetLocation(Float64 lat,Float64 lng);
   void GetLocation(Float64* pLat,Float64* pLng) const;
   void SetSiteClass(SiteClass siteClass);
   SiteClass GetSiteClass() const;
   LPCTSTR GetSpecificationDescription(SpecificationType specType) const;
   LPCTSTR GetSiteClassDescription(SiteClass siteClass) const;
   LPCTSTR GetSpectraResultExplaination(Uint32 result) const;
   Uint32 GetResponseSpectra(SpecificationType specType, Float64 lat,Float64 lng,SiteClass siteClass,CResponseSpectra* pSpectra) const;

   const mathPwLinearFunction2dUsingPoints* GetZeroPeriodSiteFactors(SpecificationType specType, SiteClass siteClass) const;
   const mathPwLinearFunction2dUsingPoints* GetShortPeriodSiteFactors(SpecificationType specType, SiteClass siteClass) const;
   const mathPwLinearFunction2dUsingPoints* GetLongPeriodSiteFactors(SpecificationType specType, SiteClass siteClass) const;

   virtual void OnCloseDocument() override;

   CReportBuilderManager m_RptMgr;

protected:
   void GetSpectralValues(Float64 lat,Float64 lng,Float64* pS1,Float64* pSs,Float64* pPGA) const;
   Uint32 GetSiteFactors(SpecificationType specType, Float64 S1,Float64 Ss,Float64 PGA,SiteClass siteClass,Float64* pFa,Float64* pFv,Float64* pFpga) const;
};
