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

// CPGStableDoc document
#include "BEToolboxDoc.h"
#include "PGStableModel.h"

#include <ReportManager\ReportManager.h>
#include <Stability\Stability.h>
//#include <PsgLib\LibraryManager.h>

class CPGStableDoc : public CBEToolboxDoc
{
	DECLARE_DYNCREATE(CPGStableDoc)

public:
	CPGStableDoc();
	virtual ~CPGStableDoc();

   virtual CString GetToolbarSectionName();

   void SetGirderType(int type);
   int GetGirderType() const;

   void SetLiftingFpeType(int type);
   int GetLiftingFpeType() const;

   void SetHaulingFpeType(int type);
   int GetHaulingFpeType() const;

   const stbGirder& GetGirder(int type) const;
   void SetGirder(int type,const stbGirder& girder);

   const CPGStableStrands& GetStrands(int type) const;
   void SetStrands(int type,const CPGStableStrands& strands);

   const stbLiftingStabilityProblem& GetLiftingStabilityProblem() const;
   void SetLiftingStabilityProblem(const stbLiftingStabilityProblem& stabilityProblem);

   const stbHaulingStabilityProblem& GetHaulingStabilityProblem() const;
   void SetHaulingStabilityProblem(const stbHaulingStabilityProblem& stabilityProblem);

   const CPGStableCriteria& GetLiftingCriteria() const;
   void SetLiftingCriteria(const CPGStableCriteria& criteria);

   const CPGStableCriteria& GetHaulingCriteria() const;
   void SetHaulingCriteria(const CPGStableCriteria& criteria);

   Float64 GetDensity() const;
   void SetDensity(Float64 density);

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
   void ResolveStrandLocations(const CPGStableStrands* pStrands,stbGirder* pGirder) const;

   stbLiftingResults GetLiftingResults() const;
   stbHaulingResults GetHaulingResults() const;

   stbLiftingCheckArtifact GetLiftingCheckArtifact() const;
   stbHaulingCheckArtifact GetHaulingCheckArtifact() const;

   //const SpecLibrary* GetSpecLibrary() const;

   CReportBuilderManager m_RptMgr;

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
   /// called when a document is created (New or Open)
   virtual BOOL Init(); 

   //void LoadPGSLibrary();

   // Called by the framework when the document is to be loaded and saved
   virtual HRESULT WriteTheDocument(IStructuredSave* pStrSave);
   virtual HRESULT LoadTheDocument(IStructuredLoad* pStrLoad);

   virtual void LoadDocumentSettings();
   virtual void SaveDocumentSettings();

   virtual CString GetDocumentationRootLocation();
   virtual UINT GetToolbarID();

   mutable CPGStableModel m_Model;

   //psgLibraryManager m_LibMgr;

   afx_msg void OnHelpFinder();
	DECLARE_MESSAGE_MAP()
};
