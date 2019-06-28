///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2019  Washington State Department of Transportation
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

#include "BEToolboxDoc.h"
#include <WBFLGeometry.h>
#include "UniformFDMesh.h"

class CSVTToolDoc : public CBEToolboxDoc
{
	DECLARE_DYNCREATE(CSVTToolDoc)

public:
   CSVTToolDoc();
	virtual ~CSVTToolDoc();

   virtual CString GetToolbarSectionName() override;

#ifdef _DEBUG
	virtual void AssertValid() const override;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const override;
#endif
#endif

protected:
   // called when a document is created (New or Open)
   virtual BOOL Init() override; 

   // Called by the framework when the document is to be loaded and saved
   virtual HRESULT WriteTheDocument(IStructuredSave* pStrSave) override;
   virtual HRESULT LoadTheDocument(IStructuredLoad* pStrLoad) override;

   virtual void LoadDocumentSettings() override;
   virtual void SaveDocumentSettings() override;

   virtual CString GetDocumentationRootLocation() override;
   virtual CString GetDocumentationSetName() override;
   virtual CString GetDocumentationURL() override;

   afx_msg void OnHelpFinder();
	DECLARE_MESSAGE_MAP()
public:

   void SetGirder(BeamShapeType type);
   BeamShapeType GetGirder() const;

   void SetMaxElementSize(Float64 dMax);
   Float64 GetMaxElementSize() const;

   void GetShape(IShape** ppShape);

   void GetTorsionalConstant(Float64* pJ,IndexType* pnElements,IndexType* pnPoints);

   std::vector<CComPtr<IRectangle>> GetMesh();

   // over-ride default behavior by destroying column
   virtual void OnCloseDocument() override;

protected:
   virtual UINT GetToolbarID() override;
   virtual void LoadToolbarResource() override;

private:
   BeamShapeType m_Type;
   CComPtr<IShape> m_pShape;
   CComPtr<IBeamShapeFactory> m_Factory;
   std::unique_ptr<UniformFDMesh> m_pMesh;

   Float64 m_Dmax;

   void GenerateMesh(UniformFDMesh& mesh);
};
