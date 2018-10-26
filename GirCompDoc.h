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

// CGirCompDoc document
#include "BEToolboxDoc.h"
#include <WBFLTools.h>
#include <WBFLGeometry.h>
#include <WBFLSections.h>
#include <WBFLUnitServer.h>
#include <ReportManager\ReportManager.h>

struct ROLLEDSECTION
{
   std::_tstring Name;
   Float64 Area;
   Float64 D;
   Float64 wFlange;
   Float64 I;

   ROLLEDSECTION(LPCTSTR name,Float64 a=0,Float64 d=0,Float64 wf=0,Float64 i=0):
   Name(name),Area(a),D(d),wFlange(wf),I(i) {}

   bool operator<(const ROLLEDSECTION& other) const { return Name < other.Name; }
};

struct GIRCOMPDIMENSIONS
{
   GIRCOMPDIMENSIONS();

   enum BeamType {Rolled,General,BuiltUp};
   BeamType Type;
   std::_tstring RolledSectionName; // key into the rolled section database

   // General Properties
   Float64 Area;
   Float64 MomentOfInertia;

   // Built-up Dimensions
   Float64 D;
   Float64 tWeb;
   Float64 wTopFlange;
   Float64 tTopFlange;
   Float64 wBotFlange;
   Float64 tBotFlange;
   
   // Slab
   Float64 G;
   Float64 tSlab;
   Float64 wSlab;
   Float64 N1, N2, N3;
};

class CGirCompDoc : public CBEToolboxDoc
{
	DECLARE_DYNCREATE(CGirCompDoc)

public:
	CGirCompDoc();
	virtual ~CGirCompDoc();

   virtual CString GetToolbarSectionName();

   IndexType GetProblemCount() const;
   void AddProblem(const GIRCOMPDIMENSIONS& problem);
   void AddProblems(const std::vector<GIRCOMPDIMENSIONS>& problems);
   void RemoveProblem(IndexType idx);
   const GIRCOMPDIMENSIONS& GetProblem(IndexType idx);
   void ClearProblems();

   void ComputeShapeProperties(IndexType idx,IShapeProperties** ppShapeProperties);

   const std::vector<ROLLEDSECTION>& GetRolledSections() const;

   void GetBuiltUpGirder(IndexType idx,IPlateGirder** ppGirder);
   void GetCompositeBeam(IndexType idx,int n,ICompositeBeam** ppCompBeam);

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

   std::vector<GIRCOMPDIMENSIONS> m_Problems;

   void ComputeRollSectionProperties(const GIRCOMPDIMENSIONS& dimensions,IShapeProperties** ppShapeProperties);
   void ComputeGeneralSectionProperties(const GIRCOMPDIMENSIONS& dimensions,IShapeProperties** ppShapeProperties);
   void ComputeBuiltUpSectionProperties(const GIRCOMPDIMENSIONS& dimensions,IShapeProperties** ppShapeProperties);

   std::vector<ROLLEDSECTION> m_RolledSections;
   void InitRolledSections();

	DECLARE_MESSAGE_MAP()
public:

   // over-ride default behavior by destroying column
   virtual void OnCloseDocument();

   CReportBuilderManager m_RptMgr;
};
