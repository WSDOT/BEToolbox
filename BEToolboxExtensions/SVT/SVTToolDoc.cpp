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

// SVTToolDoc.cpp : implementation file
//

#include "stdafx.h"
#include "..\resource.h"
#include "SVTToolDoc.h"
#include "SVTToolChildFrame.h"
#include "BEToolboxStatusBar.h"
#include "SVTTool.hh"

#include <EAF\EAFUtilities.h>
#include <EAF\EAFApp.h>
#include <EAF\EAFHelp.h>

#include "FDMeshGenerator.h"
#include "PrandtlMembrane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CSVTToolDoc

IMPLEMENT_DYNCREATE(CSVTToolDoc, CBEToolboxDoc)

CSVTToolDoc::CSVTToolDoc()
{
   UIHints(FALSE); // not using UIHints feature
}

CSVTToolDoc::~CSVTToolDoc()
{
}


BEGIN_MESSAGE_MAP(CSVTToolDoc, CBEToolboxDoc)
   ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
END_MESSAGE_MAP()

void CSVTToolDoc::OnHelpFinder()
{
   EAFHelp(EAFGetDocument()->GetDocumentationSetName(),IDH_SVTTOOL);
}


// CSVTToolDoc diagnostics

#ifdef _DEBUG
void CSVTToolDoc::AssertValid() const
{
	CBEToolboxDoc::AssertValid();
}

#ifndef _WIN32_WCE
void CSVTToolDoc::Dump(CDumpContext& dc) const
{
	CBEToolboxDoc::Dump(dc);
}
#endif
#endif //_DEBUG

BOOL CSVTToolDoc::Init()
{
   if (!CBEToolboxDoc::Init())
   {
      return FALSE;
   }

   // initialize with some data
   m_Factory.CoCreateInstance(CLSID_BeamShapeFactory);
   m_Type = AASHTO_TypeI;
   m_Factory->CreateShape(m_Type, &m_pShape);
   m_Dmax = 0.25; // inch
   m_pMesh = std::make_unique<UniformFDMesh>();
   GenerateMesh(*(m_pMesh.get()));

   return TRUE;
}

void CSVTToolDoc::OnCloseDocument()
{
   CBEToolboxDoc::OnCloseDocument();
}

HRESULT CSVTToolDoc::WriteTheDocument(IStructuredSave* pStrSave)
{
   HRESULT hr = pStrSave->BeginUnit(_T("SVTTool"),1.0);
   if ( FAILED(hr) )
      return hr;

   // save that data for this tool here

   hr = pStrSave->EndUnit(); // SVTTool
   if ( FAILED(hr) )
      return hr;

   return S_OK;
}

HRESULT CSVTToolDoc::LoadTheDocument(IStructuredLoad* pStrLoad)
{
   HRESULT hr = pStrLoad->BeginUnit(_T("SVTTool"));
   if ( FAILED(hr) )
      return hr;

   // load that data for this tool here
   
   hr = pStrLoad->EndUnit(); // SVTTool
   if ( FAILED(hr) )
      return hr;

   return S_OK;
}

void CSVTToolDoc::LoadDocumentSettings()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CBEToolboxDoc::LoadDocumentSettings();
}

void CSVTToolDoc::SaveDocumentSettings()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CBEToolboxDoc::SaveDocumentSettings();
}

CString CSVTToolDoc::GetDocumentationRootLocation()
{
   // clearly a dummy URL, but you get the idea... this is the
   // URL where your documentation is located
   // be sure to include the last '/'
   return _T("http://www.somewhere.com/Documentation/");

   //CEAFApp* pApp = EAFGetApp();
   //return pApp->GetDocumentationRootLocation();
}

CString CSVTToolDoc::GetDocumentationSetName()
{
   // Return the name of your documentation set
   return _T("ExampleDocs");
   //return CBEToolboxDoc::GetDocumentationSetName(); // this returns BEToolbox
}

CString CSVTToolDoc::GetDocumentationURL()
{
   // Return the URL for documation for the application
   // Called by GetDocumenentLocation to form the complete
   // documentation URL. The typical format for documentation URLs is:
   // For an Online source
   //     GetDocumentationRootLocation() + m_pszExeName + major.minor version number
   // For a local source
   //     GetDocumentationRootLocation() + m_pszExename

   return GetDocumentationRootLocation();
}

CString CSVTToolDoc::GetToolbarSectionName()
{
   // This is the name that is used in the registry to
   // save/load the toolbar state
   return _T("SVTTool");
}

UINT CSVTToolDoc::GetToolbarID()
{
   // return the resource ID for this tool's toolbar
   return IDR_SVT;
}

void CSVTToolDoc::LoadToolbarResource()
{
   // Set the module state to this DLL before the BEToolbox application
   // loads the toolbar resource (identified by the return value of GetToolbarID())
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CBEToolboxDoc::LoadToolbarResource();
}

void CSVTToolDoc::SetGirder(BeamShapeType type)
{
   if (m_Type != type)
   {
      m_pShape.Release();
      m_Type = type;
      m_Factory->CreateShape(m_Type,&m_pShape);

      m_pMesh = std::make_unique<UniformFDMesh>();
      GenerateMesh(*(m_pMesh.get()));

      UpdateAllViews(nullptr);
   }
}

BeamShapeType CSVTToolDoc::GetGirder() const
{
   return m_Type;
}

void CSVTToolDoc::GetShape(IShape** ppShape)
{
   m_pShape.CopyTo(ppShape);
}

void CSVTToolDoc::SetMaxElementSize(Float64 dMax)
{
   m_Dmax = dMax;
}

Float64 CSVTToolDoc::GetMaxElementSize() const
{
   return m_Dmax;
}

void CSVTToolDoc::GetTorsionalConstant(Float64* pJ, IndexType* pnElements, IndexType* pnPoints)
{
   PrandtlMembrane membrane;
   *pJ = membrane.ComputeJ(*(m_pMesh.get()));
   *pnElements = m_pMesh->GetElementCount();
   *pnPoints = m_pMesh->GetInteriorNodeCount();
}

std::vector<CComPtr<IRectangle>> CSVTToolDoc::GetMesh()
{
   CComPtr<IRect2d> bbox;
   m_pShape->get_BoundingBox(&bbox);
   CComPtr<IPoint2d> pntTL;
   bbox->get_TopLeft(&pntTL);
   Float64 tlx, tly;
   pntTL->Location(&tlx, &tly);

   Float64 dx, dy;
   m_pMesh->GetElementSize(&dx, &dy);

   std::vector<CComPtr<IRectangle>> mesh_shapes;
   mesh_shapes.reserve(m_pMesh->GetElementCount());

   auto nRows = m_pMesh->GetElementRowCount();
   for (auto row = 0; row < nRows; row++)
   {
      Float64 cy = tly - row*dy -  dy/2;

      IndexType gridStartIdx, firstElementIdx, lastElementIdx;
      m_pMesh->GetElementRange(row, &gridStartIdx, &firstElementIdx, &lastElementIdx);
      Float64 x = gridStartIdx * dx;
      for (auto elementIdx = firstElementIdx; elementIdx <= lastElementIdx; elementIdx++)
      {
         Float64 cx = tlx + (gridStartIdx + elementIdx - firstElementIdx)*dx + dx/2;

         CComPtr<IRectangle> rect;
         rect.CoCreateInstance(CLSID_Rect);
         rect->put_Width(dx);
         rect->put_Height(dy);
         CComPtr<IPoint2d> pntHook;
         rect->get_HookPoint(&pntHook);
         pntHook->Move(cx, cy);

         mesh_shapes.push_back(rect);
      }
   }

   return mesh_shapes;
}

void CSVTToolDoc::GenerateMesh(UniformFDMesh& mesh)
{
   Float64 min_mesh_element_size = 0.25;
   FDMeshGenerator mesh_generator(min_mesh_element_size, min_mesh_element_size);

   mesh_generator.GenerateMesh(m_pShape, mesh);
}
