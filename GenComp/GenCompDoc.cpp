///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2025  Washington State Department of Transportation
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
#include "..\resource.h"
#include "GenCompDoc.h"
#include "GenCompTitlePageBuilder.h"
#include "GenCompChapterBuilder.h"
#include "GenCompChildFrame.h"
#include "..\BEToolboxStatusBar.h"

#include <EAF\EAFUtilities.h>
#include <EAF\EAFApp.h>

// CGenCompDoc

IMPLEMENT_DYNCREATE(CGenCompDoc, CBEToolboxDoc)

CGenCompDoc::CGenCompDoc() : CBEToolboxDoc()
{
   // The reporting sub-system doesn't use the WBFLUnitServer implementation. It uses the old regular C++
   // units sytem. That system is in kms units, so we will create a unit server here also in the kms system
   // so that the curvel data, after loading is in set of consistent base units we want.
   // If the report system could handle the WBFLUnitServer, the <ConsistentUnits> declaration in the
   // CurvelXML instance document would work throughout this program because Curvel works exclusively in
   // consistent units.
   m_DocUnitServer.CoCreateInstance(CLSID_UnitServer);
   m_DocUnitServer->SetSystemUnits(CComBSTR(WBFL::Units::System::GetMassUnit().UnitTag().c_str()),
                            CComBSTR(WBFL::Units::System::GetLengthUnit().UnitTag().c_str()),
                            CComBSTR(WBFL::Units::System::GetTimeUnit().UnitTag().c_str()),
                            CComBSTR(WBFL::Units::System::GetTemperatureUnit().UnitTag().c_str()),
                            CComBSTR(WBFL::Units::System::GetAngleUnit().UnitTag().c_str()));  
   m_DocUnitServer->QueryInterface(&m_DocConvert);


   std::shared_ptr<WBFL::Reporting::ReportBuilder> pRptBuilder(std::make_shared<WBFL::Reporting::ReportBuilder>(_T("GenComp")));
   GetReportManager()->AddReportBuilder(pRptBuilder);

   std::shared_ptr<WBFL::Reporting::TitlePageBuilder> pTitlePageBuilder(std::make_shared<CGenCompTitlePageBuilder>());
   pRptBuilder->AddTitlePageBuilder( pTitlePageBuilder );

   std::shared_ptr<WBFL::Reporting::ChapterBuilder> pChBuilder(std::make_shared<CGenCompChapterBuilder>(this) );
   pRptBuilder->AddChapterBuilder(pChBuilder);

   EnableUIHints(FALSE); // not using UIHints feature
}

CGenCompDoc::~CGenCompDoc()
{
}


BEGIN_MESSAGE_MAP(CGenCompDoc, CBEToolboxDoc)
   ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
END_MESSAGE_MAP()

void CGenCompDoc::OnHelpFinder()
{
   EAFHelp(EAFGetDocument()->GetDocumentationSetName(),IDH_GENCOMP);
}


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

BOOL CGenCompDoc::OnNewDocument()
{
   if ( !CBEToolboxDoc::OnNewDocument() )
      return FALSE;

   m_GenCompXML = CreateGenCompModel();
   if ( m_GenCompXML.get() == nullptr )
      return FALSE;

   return TRUE;
}

BOOL CGenCompDoc::OpenTheDocument(LPCTSTR lpszPathName)
{
   m_GenCompXML = CreateGenCompModel(lpszPathName,m_DocUnitServer);
   return m_GenCompXML.get() == nullptr ? FALSE : TRUE;
}

BOOL CGenCompDoc::SaveTheDocument(LPCTSTR lpszPathName)
{
   try
   {
      std::ofstream file(lpszPathName);
      GenComp_(file,*m_GenCompXML);
   }
   catch(const xml_schema::exception& /*e*/)
   {
#pragma Reminder("UPDATE: provide better error handling")
      AfxMessageBox(_T("An error occured while saving the file"));
      return FALSE;
   }
   return TRUE;
}

void CGenCompDoc::LoadDocumentSettings()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   __super::LoadDocumentSettings();
}

void CGenCompDoc::SaveDocumentSettings()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   __super::SaveDocumentSettings();
}

CString CGenCompDoc::GetDocumentationRootLocation()
{
   CEAFApp* pApp = EAFGetApp();
   return pApp->GetDocumentationRootLocation();
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

std::vector<std::pair<Float64,Float64>> CGenCompDoc::GetPrimaryPoints() const
{
   std::vector<std::pair<Float64, Float64>> primary_points;
   ShapeType::Point_sequence& points(m_GenCompXML->PrimaryShape().Point());
   for ( const auto& point : points)
   {
      primary_points.emplace_back(point.X(),point.Y());
   }
   return primary_points;
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

std::vector<std::pair<Float64,Float64>> CGenCompDoc::GetSecondaryPoints() const
{
   std::vector<std::pair<Float64, Float64>> secondary_points;
   ShapeType::Point_sequence& points(m_GenCompXML->SecondaryShape().Point());
   for ( const auto& point : points)
   {
      secondary_points.emplace_back(point.X(),point.Y());
   }
   return secondary_points;
}

WBFL::Geometry::Polygon CGenCompDoc::GetPrimaryShape() const
{
   WBFL::Geometry::Polygon polygon;

   ShapeType::Point_sequence& points(m_GenCompXML->PrimaryShape().Point());
   ShapeType::Point_sequence::iterator iter(points.begin());
   ShapeType::Point_sequence::iterator end(points.end());
   for ( ; iter != end; iter++ )
   {
      polygon.AddPoint(iter->X(),iter->Y());
   }

   return polygon;
}

WBFL::Geometry::Section CGenCompDoc::GetCompositeSection() const
{
   auto primaryShape(GetPrimaryShape());
   WBFL::Geometry::Polygon secondaryShape;

   ShapeType::Point_sequence& points(m_GenCompXML->SecondaryShape().Point());
   ShapeType::Point_sequence::iterator iter(points.begin());
   ShapeType::Point_sequence::iterator end(points.end());
   for ( ; iter != end; iter++ )
   {
      secondaryShape.AddPoint(iter->X(),iter->Y());
   }

   WBFL::Geometry::Section section;
   section.AddComponent(primaryShape,   1.0, 1.0, 0.0, 0.0, WBFL::Geometry::SectionComponent::ComponentType::Structural);
   section.AddComponent(secondaryShape, m_GenCompXML->ModularRatio(), 1.0, 0.0, 0.0, WBFL::Geometry::SectionComponent::ComponentType::Structural);

   return section;
}
