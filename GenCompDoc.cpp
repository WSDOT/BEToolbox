///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2013  Washington State Department of Transportation
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

// GenCompDoc.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "GenCompDoc.h"
#include "GenCompTitlePageBuilder.h"
#include "GenCompChapterBuilder.h"
#include "GenCompChildFrame.h"
#include "BEToolboxStatusBar.h"

#include <EAF\EAFUtilities.h>
#include <EAF\EAFApp.h>

// CGenCompDoc

IMPLEMENT_DYNCREATE(CGenCompDoc, CBEToolboxDoc)

CGenCompDoc::CGenCompDoc()
{
   // The reporting sub-system doesn't use the WBFLUnitServer implementation. It uses the old regular C++
   // units sytem. That system is in kms units, so we will create a unit server here also in the kms system
   // so that the curvel data, after loading is in set of consistent base units we want.
   // If the report system could handle the WBFLUnitServer, the <ConsistentUnits> declaration in the
   // CurvelXML instance document would work throughout this program because Curvel works exclusively in
   // consistent units.
   m_DocUnitServer.CoCreateInstance(CLSID_UnitServer);
   m_DocUnitServer->SetBaseUnits(CComBSTR(unitSysUnitsMgr::GetMassUnit().UnitTag().c_str()),
                            CComBSTR(unitSysUnitsMgr::GetLengthUnit().UnitTag().c_str()),
                            CComBSTR(unitSysUnitsMgr::GetTimeUnit().UnitTag().c_str()),
                            CComBSTR(unitSysUnitsMgr::GetTemperatureUnit().UnitTag().c_str()),
                            CComBSTR(unitSysUnitsMgr::GetAngleUnit().UnitTag().c_str()));  
   m_DocUnitServer->QueryInterface(&m_DocConvert);


   CReportBuilder* pRptBuilder = new CReportBuilder(_T("GenComp"));

   boost::shared_ptr<CTitlePageBuilder> pTitlePageBuilder(new CGenCompTitlePageBuilder());
   pRptBuilder->AddTitlePageBuilder( pTitlePageBuilder );

   boost::shared_ptr<CChapterBuilder> pChBuilder( new CGenCompChapterBuilder(this) );
   pRptBuilder->AddChapterBuilder(pChBuilder);

   m_RptMgr.AddReportBuilder(pRptBuilder);
}

CGenCompDoc::~CGenCompDoc()
{
}


BEGIN_MESSAGE_MAP(CGenCompDoc, CBEToolboxDoc)
END_MESSAGE_MAP()


// CGenCompDoc diagnostics

#ifdef _DEBUG
void CGenCompDoc::AssertValid() const
{
	CBEToolboxDoc::AssertValid();
}

#ifndef _WIN32_WCE
void CGenCompDoc::Dump(CDumpContext& dc) const
{
	CBEToolboxDoc::Dump(dc);
}
#endif
#endif //_DEBUG

CString CGenCompDoc::GetToolbarSectionName()
{
   return _T("GenComp");
}

BOOL CGenCompDoc::OpenTheDocument(LPCTSTR lpszPathName)
{
   m_GenCompXML = CreateGenCompModel(lpszPathName,m_DocConvert);
   return m_GenCompXML.get() == NULL ? FALSE : TRUE;
}

BOOL CGenCompDoc::SaveTheDocument(LPCTSTR lpszPathName)
{
   try
   {
      std::ofstream file(lpszPathName);
      GenComp_(file,*m_GenCompXML);
   }
   catch(const xml_schema::exception& e)
   {
#pragma Reminder("UPDATE: provide better error handling")
      AfxMessageBox(_T("An error occured while saving the file"));
      return FALSE;
   }
   return TRUE;
}

void CGenCompDoc::OnOldFormat(LPCTSTR lpszPathName)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   AfxMessageBox(_T("Error: Invalid file format.\n\nThis is not a GenComp file or it is in a legacy format that is not supported"),MB_OK | MB_ICONEXCLAMATION);
}

void CGenCompDoc::Clear()
{
   ClearPrimaryPoints();
   ClearSecondaryPoints();
}

void CGenCompDoc::SetModularRatio(Float64 n)
{
   m_GenCompXML->ModularRatio(n);
}

Float64 CGenCompDoc::GetModularRatio() const
{
   return m_GenCompXML->ModularRatio();
}

void CGenCompDoc::ClearPrimaryPoints()
{
   m_GenCompXML->PrimaryShape().Point().clear();
}

void CGenCompDoc::AddPrimaryPoint(Float64 x,Float64 y)
{
   m_GenCompXML->PrimaryShape().Point().push_back(PointType(x,y));
}

void CGenCompDoc::AddPrimaryPoints(const std::vector<std::pair<Float64,Float64>>& newPoints)
{
   ShapeType::Point_sequence& points(m_GenCompXML->PrimaryShape().Point());
   std::vector<std::pair<Float64,Float64>>::const_iterator iter(newPoints.begin());
   std::vector<std::pair<Float64,Float64>>::const_iterator end(newPoints.end());
   for ( ; iter != end; iter++ )
   {
      points.push_back(PointType(iter->first,iter->second));
   }
}

const std::vector<std::pair<Float64,Float64>>& CGenCompDoc::GetPrimaryPoints()
{
   m_PrimaryPoints.clear();
   ShapeType::Point_sequence& points(m_GenCompXML->PrimaryShape().Point());
   ShapeType::Point_sequence::iterator iter(points.begin());
   ShapeType::Point_sequence::iterator end(points.end());
   for ( ; iter != end; iter++ )
   {
      m_PrimaryPoints.push_back(std::make_pair<Float64,Float64>(iter->X(),iter->Y()));
   }
   return m_PrimaryPoints;
}

void CGenCompDoc::ClearSecondaryPoints()
{
   m_GenCompXML->SecondaryShape().Point().clear();
}

void CGenCompDoc::AddSecondaryPoint(Float64 x,Float64 y)
{
   m_GenCompXML->SecondaryShape().Point().push_back(PointType(x,y));
}

void CGenCompDoc::AddSecondaryPoints(const std::vector<std::pair<Float64,Float64>>& newPoints)
{
   ShapeType::Point_sequence& points(m_GenCompXML->SecondaryShape().Point());
   std::vector<std::pair<Float64,Float64>>::const_iterator iter(newPoints.begin());
   std::vector<std::pair<Float64,Float64>>::const_iterator end(newPoints.end());
   for ( ; iter != end; iter++ )
   {
      points.push_back(PointType(iter->first,iter->second));
   }
}

const std::vector<std::pair<Float64,Float64>>& CGenCompDoc::GetSecondaryPoints()
{
   m_SecondaryPoints.clear();
   ShapeType::Point_sequence& points(m_GenCompXML->SecondaryShape().Point());
   ShapeType::Point_sequence::iterator iter(points.begin());
   ShapeType::Point_sequence::iterator end(points.end());
   for ( ; iter != end; iter++ )
   {
      m_SecondaryPoints.push_back(std::make_pair<Float64,Float64>(iter->X(),iter->Y()));
   }
   return m_SecondaryPoints;
}

CComPtr<IPolyShape> CGenCompDoc::GetPrimaryShape()
{
   CComPtr<IPolyShape> polyShape;
   polyShape.CoCreateInstance(CLSID_PolyShape);

   ShapeType::Point_sequence& points(m_GenCompXML->PrimaryShape().Point());
   ShapeType::Point_sequence::iterator iter(points.begin());
   ShapeType::Point_sequence::iterator end(points.end());
   for ( ; iter != end; iter++ )
   {
      polyShape->AddPoint(iter->X(),iter->Y());
   }

   return polyShape;
}

CComPtr<ICompositeSection> CGenCompDoc::GetCompositeSection()
{
   CComPtr<IPolyShape> primaryShape(GetPrimaryShape());
   CComPtr<IPolyShape> secondaryShape;
   secondaryShape.CoCreateInstance(CLSID_PolyShape);

   ShapeType::Point_sequence& points(m_GenCompXML->SecondaryShape().Point());
   ShapeType::Point_sequence::iterator iter(points.begin());
   ShapeType::Point_sequence::iterator end(points.end());
   for ( ; iter != end; iter++ )
   {
      secondaryShape->AddPoint(iter->X(),iter->Y());
   }

   CComPtr<ICompositeSection> section;
   section.CoCreateInstance(CLSID_CompositeSection);

   CComQIPtr<IShape> shape1(primaryShape);
   section->AddSection(shape1,1.0,1.0,VARIANT_FALSE,VARIANT_TRUE);

   CComQIPtr<IShape> shape2(secondaryShape);
   section->AddSection(shape2,m_GenCompXML->ModularRatio(),1.0,VARIANT_FALSE,VARIANT_TRUE);

   return section;
}
