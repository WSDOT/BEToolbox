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

// CPGStableDoc document
#include "BEToolboxLib.h"
#include "BEToolboxDoc.h"
#include "PGStableModel.h"
#include "PGStablePluginCATID.h"

#include <ReportManager\ReportManager.h>
#include <PsgLib\LibraryManager.h>


static CString gs_strCriteria(_T("Define project criteria"));
static CString gs_strHaulTruck(_T("Define haul truck"));
static CString gs_strGirder(_T("Define girder properties"));

#define HINT_UPDATE_DATA 1

class BETCLASS CPGStableDoc : public CBEToolboxDoc
{
	DECLARE_DYNCREATE(CPGStableDoc)

public:
	CPGStableDoc();
	virtual ~CPGStableDoc();

   virtual CString GetToolbarSectionName();

   const CString& GetCriteria() const;
   void SetCriteria(LPCTSTR lpszCriteria);

   const CString& GetHaulTruck() const;
   void SetHaulTruck(LPCTSTR lpszHaulTruck);

   const CString& GetGirder() const;
   void SetGirder(LPCTSTR lpszGirder);

   void SetProjectProperties(const CString& strEngineer,const CString& strCompany,const CString& strJob,const CString& strComments);
   void GetProjectProperties(CString* pstrEngineer,CString* pstrCompany,CString* pstrJob,CString* pstrComments) const;

   void SetGirderType(int type);
   int GetGirderType() const;

   void SetStressPointType(int stressPointType);
   int GetStressPointType() const;

   const stbGirder& GetGirder(int type) const;
   void SetGirder(int type,const stbGirder& girder);

   const CPGStableStrands& GetStrands(int girderType,int modelType) const;
   void SetStrands(int girderType,int modelType,const CPGStableStrands& strands);

   const stbLiftingStabilityProblem& GetLiftingStabilityProblem() const;
   void SetLiftingStabilityProblem(const stbLiftingStabilityProblem& stabilityProblem);

   const stbHaulingStabilityProblem& GetHaulingStabilityProblem() const;
   void SetHaulingStabilityProblem(const stbHaulingStabilityProblem& stabilityProblem);

   const CPGStableLiftingCriteria& GetLiftingCriteria() const;
   void SetLiftingCriteria(const CPGStableLiftingCriteria& criteria);

   const CPGStableHaulingCriteria& GetHaulingCriteria() const;
   void SetHaulingCriteria(const CPGStableHaulingCriteria& criteria);

   Float64 GetDensity() const;
   void SetDensity(Float64 density);

   Float64 GetDensityWithRebar() const;
   void SetDensityWithRebar(Float64 density);

   Float64 GetK1() const;
   void SetK1(Float64 k1);

   Float64 GetK2() const;
   void SetK2(Float64 k2);

   void GetLiftingMaterials(Float64* pFci,bool* pbComputeEci,Float64* pFrCoefficient) const;
   void SetLiftingMaterials(Float64 fci,bool bComputeEci,Float64 frCoefficient);

   void GetHaulingMaterials(Float64* pFc,bool* pbComputeEc,Float64* pFrCoefficient) const;
   void SetHaulingMaterials(Float64 fc,bool bComputeEc,Float64 frCoefficient);

   void SetHeightOfGirderBottomAboveRoadway(Float64 Hgb);
   Float64 GetHeightOfGirderBottomAboveRoadway() const;

   CString UpdateEc(const CString& strFc,const CString& strDensity,const CString& strK1,const CString& strK2);

   void ResolveStrandLocations(const CPGStableStrands& strands,const stbGirder& girder,Float64* pYs,Float64* pXh1,Float64* pYh1,Float64* pXh2,Float64* pYh2,Float64* pXh3,Float64* pYh3,Float64* pXh4,Float64* pYh4,Float64* pYt);
   void GetStrandProfiles(const CPGStableStrands& strands,const stbGirder& girder,std::vector<std::pair<Float64,Float64>>* pvStraight,std::vector<std::pair<Float64,Float64>>* pvHarped,std::vector<std::pair<Float64,Float64>>* pvTemp);

   stbLiftingResults GetLiftingResults() const;
   stbHaulingResults GetHaulingResults() const;

   stbLiftingCheckArtifact GetLiftingCheckArtifact() const;
   stbHaulingCheckArtifact GetHaulingCheckArtifact() const;

   const SpecLibrary* GetSpecLibrary() const;
   const SpecLibraryEntry* GetSpecLibraryEntry() const;

   const HaulTruckLibrary* GetHaulTruckLibrary() const;
   const HaulTruckLibraryEntry* GetHaulTruckLibraryEntry() const;

   const GirderLibrary* GetGirderLibrary() const;
   const GirderLibraryEntry* GetGirderLibraryEntry() const;

   bool IsPermittedGirderEntry(const GirderLibraryEntry* pGirderEntry) const;

   CReportBuilderManager m_RptMgr;

#ifdef _DEBUG
	virtual void AssertValid() const override;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const override;
#endif
#endif

protected:
   /// called when a document is created (New or Open)
   virtual BOOL Init() override;

   // Plugins
   virtual CATID GetDocumentPluginCATID() override { return CATID_PGStablePlugin; };

   void LoadPGSLibrary();

   // Called by the framework when the document is to be loaded and saved
   virtual HRESULT WriteTheDocument(IStructuredSave* pStrSave) override;
   virtual HRESULT LoadTheDocument(IStructuredLoad* pStrLoad) override;

   virtual void LoadDocumentSettings() override;
   virtual void SaveDocumentSettings() override;

   virtual CString GetDocumentationRootLocation() override;
   virtual UINT GetToolbarID() override;

   CString m_strProjectCriteria;
   CString m_strHaulTruck;
   CString m_strGirder;

   CString m_strEngineer;
   CString m_strCompany;
   CString m_strJob;
   CString m_strComments;

   mutable CPGStableModel m_Model;

   psgLibraryManager m_LibMgr;

   afx_msg void OnHelpFinder();
	DECLARE_MESSAGE_MAP()
};
