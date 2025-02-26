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

// BearingDoc.cpp : implementation file
//

#include "stdafx.h"
#include "..\resource.h"
#include "BearingDoc.h"
#include "BearingTitlePageBuilder.h"
#include "BearingChapterBuilder.h"
#include "BearingChildFrame.h"
#include "..\BEToolboxStatusBar.h";



#include <EAF\EAFUtilities.h>
#include <EAF\EAFApp.h>
#include <BEToolbox.hh>
#include <EAF/EAFHelp.h>

using namespace WBFL::EngTools;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CBearingDoc

IMPLEMENT_DYNCREATE(CBearingDoc, CBEToolboxDoc)

CBearingDoc::CBearingDoc() : CBEToolboxDoc()
{
   std::shared_ptr<WBFL::Reporting::ReportBuilder> pRptBuilder(std::make_shared<WBFL::Reporting::ReportBuilder>(_T("Bearing")));

   pRptBuilder->EnableHeadingNumbers(true);

   GetReportManager()->AddReportBuilder(pRptBuilder);

   std::shared_ptr<WBFL::Reporting::TitlePageBuilder> pTitlePageBuilder(std::make_shared<CBearingTitlePageBuilder>());
   pRptBuilder->AddTitlePageBuilder( pTitlePageBuilder );

   std::shared_ptr<WBFL::Reporting::ChapterBuilder> pChBuilder(std::make_shared<CBearingChapterBuilder>(this) );
   pRptBuilder->AddChapterBuilder(pChBuilder);

   EnableUIHints(FALSE); // not using UIHints feature
}

CBearingDoc::~CBearingDoc()
{
}

CString CBearingDoc::GetToolbarSectionName()
{
   return _T("Bearing");
}

BEGIN_MESSAGE_MAP(CBearingDoc, CBEToolboxDoc)
   ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
END_MESSAGE_MAP()


void CBearingDoc::SetSpecification(WBFL::LRFD::BDSManager::Edition specification)
{
    if (m_specification != specification)
    {
        m_specification = specification;
        UpdateAllViews(nullptr);
        SetModifiedFlag();
    }
}


const WBFL::LRFD::BDSManager::Edition& CBearingDoc::GetSpecification() const
{
    return m_specification;
}

const WBFL::EngTools::BearingDesignCriteria& CBearingDoc::GetBearingDesignCriteria() const
{
    return m_criteria;
}

void CBearingDoc::SetBearing(const Bearing& brg, const BearingLoads& brg_loads, const BearingCalculator& brg_calc)
{
	m_bearing = brg;
	m_bearing_loads = brg_loads;
    m_bearing_calculator = brg_calc;
	UpdateAllViews(NULL);
}


const Bearing& CBearingDoc::GetBearing() const
{
    return m_bearing;
}


const BearingLoads& CBearingDoc::GetBearingLoads() const
{
    return m_bearing_loads;
}


const BearingCalculator& CBearingDoc::GetBearingCalculator() const
{
    return m_bearing_calculator;
}


void CBearingDoc::OnHelpFinder()
{
   EAFHelp(EAFGetDocument()->GetDocumentationSetName(),IDH_BEARING);
}


// CBearingDoc diagnostics

#ifdef _DEBUG
void CBearingDoc::AssertValid() const
{
	CBEToolboxDoc::AssertValid();
}

#ifndef _WIN32_WCE
void CBearingDoc::Dump(CDumpContext& dc) const
{
	CBEToolboxDoc::Dump(dc);
}
#endif
#endif //_DEBUG

BOOL CBearingDoc::Init()
{
   if ( !CBEToolboxDoc::Init() )
      return FALSE;

   // initialize with some defaults

   SetSpecification(WBFL::LRFD::BDSManager::Edition::LastEdition);

   m_bearing_calculator.SetAnalysisMethod(BearingCalculator::AnalysisMethod::MethodA);

   m_bearing.SetLength(WBFL::Units::ConvertToSysUnits(11.0, WBFL::Units::Measure::Inch));
   m_bearing.SetWidth(WBFL::Units::ConvertToSysUnits(27.0, WBFL::Units::Measure::Inch));
   m_bearing.SetShearModulusMinimum(WBFL::Units::ConvertToSysUnits(140, WBFL::Units::Measure::PSI));
   m_bearing.SetShearModulusMaximum(WBFL::Units::ConvertToSysUnits(190, WBFL::Units::Measure::PSI));
   m_bearing.SetIntermediateLayerThickness(WBFL::Units::ConvertToSysUnits(0.5, WBFL::Units::Measure::Inch));
   m_bearing.SetCoverThickness(WBFL::Units::ConvertToSysUnits(0.25, WBFL::Units::Measure::Inch));
   m_bearing.SetSteelShimThickness(WBFL::Units::ConvertToSysUnits(0.0747, WBFL::Units::Measure::Inch));
   m_bearing.SetYieldStrength(WBFL::Units::ConvertToSysUnits(36, WBFL::Units::Measure::KSI));
   m_bearing.SetFatigueThreshold(WBFL::Units::ConvertToSysUnits(24, WBFL::Units::Measure::KSI));
   m_bearing.SetNumIntLayers(4);
   m_bearing.SetDensityElastomer(WBFL::Units::ConvertToSysUnits(74.93, WBFL::Units::Measure::LbfPerFeet3));
   m_bearing.SetDensitySteel(WBFL::Units::ConvertToSysUnits(490.0, WBFL::Units::Measure::LbfPerFeet3));
   m_bearing.SetUseExternalPlates(false);

   m_bearing_loads.SetDeadLoad(WBFL::Units::ConvertToSysUnits(86.0, WBFL::Units::Measure::Kip));
   m_bearing_loads.SetLiveLoad(WBFL::Units::ConvertToSysUnits(47.0, WBFL::Units::Measure::Kip));
   m_bearing_loads.SetShearDeformation(WBFL::Units::ConvertToSysUnits(0.47, WBFL::Units::Measure::Inch));
   m_bearing_loads.SetRotationX(0.014);
   m_bearing_loads.SetRotationY(0.005);
   m_bearing_loads.SetStaticRotation(0.008);
   m_bearing_loads.SetCyclicRotation(0.006);
   m_bearing_loads.SetFixedTranslationX(true);
   m_bearing_loads.SetFixedTranslationY(false);

   return TRUE;
}

void CBearingDoc::OnCloseDocument()
{
   CBEToolboxDoc::OnCloseDocument();
}

HRESULT CBearingDoc::WriteTheDocument(IStructuredSave* pStrSave)
{
   HRESULT hr = pStrSave->BeginUnit(_T("Bearing"),1.2);
   if ( FAILED(hr) )
      return hr;


   CEAFApp* pApp = EAFGetApp();

   BearingCalculator::AnalysisMethod m_method = m_bearing_calculator.GetAnalysisMethod();
   bool fixed_translation_x = m_bearing_loads.GetFixedTranslationX();
   bool fixed_translation_y = m_bearing_loads.GetFixedTranslationY();
   Float64 length = m_bearing.GetLength();
   Float64 width = m_bearing.GetWidth();
   Float64 Gmin = m_bearing.GetShearModulusMinimum();
   Float64 Gmax = m_bearing.GetShearModulusMaximum();
   Float64 tlayer = m_bearing.GetIntermediateLayerThickness();
   Float64 tcover = m_bearing.GetCoverThickness();
   Float64 tshim = m_bearing.GetSteelShimThickness();
   Float64 fy = m_bearing.GetYieldStrength();
   Float64 fth = m_bearing.GetFatigueThreshold();
   IndexType n = m_bearing.GetNumIntLayers();
   Float64 ps = m_bearing.GetDensitySteel();
   Float64 pe = m_bearing.GetDensityElastomer();
   Float64 DL = m_bearing_loads.GetDeadLoad();
   Float64 LL = m_bearing_loads.GetLiveLoad();
   Float64 TL = m_bearing_loads.GetTotalLoad();
   Float64 rotx = m_bearing_loads.GetRotationX();
   Float64 roty = m_bearing_loads.GetRotationY();
   Float64 rot_static = m_bearing_loads.GetStaticRotation();
   Float64 rot_cyclic = m_bearing_loads.GetCyclicRotation();
   Float64 shear_def = m_bearing_loads.GetShearDeformation();

   // added in version 1.1
   bool use_external_plates = m_bearing.UseExternalPlates();

   // added in version 1.2
   WBFL::LRFD::BDSManager::Edition specification = GetSpecification();


   hr = pStrSave->put_Property(_T("Specification"), CComVariant((int)specification));
   if (FAILED(hr))
       return hr;

   hr = pStrSave->put_Property(_T("Units"), CComVariant(pApp->GetUnitsMode()));
   if (FAILED(hr))
       return hr;

   hr = pStrSave->put_Property(_T("Analysis_Method"), CComVariant((int)m_method));
   if (FAILED(hr))
       return hr;

   hr = pStrSave->put_Property(_T("Fixed_X_Translation"), CComVariant((bool)fixed_translation_x));
   if (FAILED(hr))
       return hr;

   hr = pStrSave->put_Property(_T("Fixed_Y_Translation"), CComVariant((bool)fixed_translation_y));
   if (FAILED(hr))
       return hr;

   hr = pStrSave->put_Property(_T("Use_External_Plates"), CComVariant((bool)use_external_plates));
   if (FAILED(hr))
       return hr;

   hr = pStrSave->put_Property(_T("Length"), CComVariant(length));
   if (FAILED(hr))
       return hr;

   hr = pStrSave->put_Property(_T("Width"), CComVariant(width));
   if (FAILED(hr))
       return hr;

   hr = pStrSave->put_Property(_T("Gmin"), CComVariant(Gmin));
   if (FAILED(hr))
       return hr;

   hr = pStrSave->put_Property(_T("Gmax"), CComVariant(Gmax));
   if (FAILED(hr))
       return hr;

   hr = pStrSave->put_Property(_T("tlayer"), CComVariant(tlayer));
   if (FAILED(hr))
       return hr;

   hr = pStrSave->put_Property(_T("tcover"), CComVariant(tcover));
   if (FAILED(hr))
       return hr;

   hr = pStrSave->put_Property(_T("tshim"), CComVariant(tshim));
   if (FAILED(hr))
       return hr;

   hr = pStrSave->put_Property(_T("fy"), CComVariant(fy));
   if (FAILED(hr))
       return hr;

   hr = pStrSave->put_Property(_T("fth"), CComVariant(fth));
   if (FAILED(hr))
       return hr;

   hr = pStrSave->put_Property(_T("n"), CComVariant(n));
   if (FAILED(hr))
       return hr;

   hr = pStrSave->put_Property(_T("ps"), CComVariant(ps));
   if (FAILED(hr))
       return hr;

   hr = pStrSave->put_Property(_T("pe"), CComVariant(pe));
   if (FAILED(hr))
       return hr;

   hr = pStrSave->put_Property(_T("DL"), CComVariant(DL));
   if (FAILED(hr))
       return hr;

   hr = pStrSave->put_Property(_T("LL"), CComVariant(LL));
   if (FAILED(hr))
       return hr;

   hr = pStrSave->put_Property(_T("rotx"), CComVariant(rotx));
   if (FAILED(hr))
       return hr;

   hr = pStrSave->put_Property(_T("roty"), CComVariant(roty));
   if (FAILED(hr))
       return hr;

   hr = pStrSave->put_Property(_T("rot_static"), CComVariant(rot_static));
   if (FAILED(hr))
       return hr;

   hr = pStrSave->put_Property(_T("rot_cyclic"), CComVariant(rot_cyclic));
   if (FAILED(hr))
       return hr;

   hr = pStrSave->put_Property(_T("shear_def"), CComVariant(shear_def));
   if (FAILED(hr))
       return hr;


   hr = pStrSave->EndUnit();
   if (FAILED(hr))
       return hr;


   return S_OK;


} // End Bearing Data






HRESULT CBearingDoc::LoadTheDocument(IStructuredLoad* pStrLoad)
{
   HRESULT hr = pStrLoad->BeginUnit(_T("Bearing"));
   if ( FAILED(hr) )
      return hr;

   Float64 version;
   pStrLoad->get_Version(&version);

   CComVariant var;

   CEAFApp* pApp = EAFGetApp();

   if (1.1 < version)
   {
       // added in version 1.2
       var.vt = VT_I4;
       hr = pStrLoad->get_Property(_T("Specification"), &var);
       if (FAILED(hr))
           return hr;
       m_specification = (WBFL::LRFD::BDSManager::Edition)(var.lVal);
   }

   var.vt = VT_I4;
   hr = pStrLoad->get_Property(_T("Units"), &var);
   if (FAILED(hr))
       return hr;
   pApp->SetUnitsMode(eafTypes::UnitMode(var.lVal));

   if (1.1 < version)
   {
       hr = pStrLoad->get_Property(_T("Analysis_Method"), &var);
       if (FAILED(hr))
           return hr;
       m_bearing_calculator.SetAnalysisMethod((BearingCalculator::AnalysisMethod)var.lVal);

   }
   else
   {
       hr = pStrLoad->get_Property(_T("Analysis_Method_A"), &var);
       if (FAILED(hr))
           return hr;
       m_bearing_calculator.SetAnalysisMethod((BearingCalculator::AnalysisMethod)var.lVal);
   }

   hr = pStrLoad->get_Property(_T("Fixed_X_Translation"), &var);
   if (FAILED(hr))
       return hr;
   m_bearing_loads.SetFixedTranslationX((bool)var.lVal);

   hr = pStrLoad->get_Property(_T("Fixed_Y_Translation"), &var);
   if (FAILED(hr))
       return hr;
   m_bearing_loads.SetFixedTranslationY((bool)var.lVal);


   pStrLoad->get_Version(&version);
   if (1 < version)
   {
        hr = pStrLoad->get_Property(_T("Use_External_Plates"), &var);
        if (FAILED(hr))
            return hr;
        m_bearing.SetUseExternalPlates(var.lVal);
   }

    var.vt = VT_R8;
    hr = pStrLoad->get_Property(_T("Length"), &var);
    if (FAILED(hr))
        return hr;
    m_bearing.SetLength(var.dblVal);

    hr = pStrLoad->get_Property(_T("Width"), &var);
    if (FAILED(hr))
        return hr;
    m_bearing.SetWidth(var.dblVal);

    hr = pStrLoad->get_Property(_T("Gmin"), &var);
    if (FAILED(hr))
        return hr;
    m_bearing.SetShearModulusMinimum(var.dblVal);

    hr = pStrLoad->get_Property(_T("Gmax"), &var);
    if (FAILED(hr))
        return hr;
    m_bearing.SetShearModulusMaximum(var.dblVal);

    hr = pStrLoad->get_Property(_T("tlayer"), &var);
    if (FAILED(hr))
        return hr;
    m_bearing.SetIntermediateLayerThickness(var.dblVal);

    hr = pStrLoad->get_Property(_T("tcover"), &var);
    if (FAILED(hr))
        return hr;
    m_bearing.SetCoverThickness(var.dblVal);

    hr = pStrLoad->get_Property(_T("tshim"), &var);
    if (FAILED(hr))
        return hr;
    m_bearing.SetSteelShimThickness(var.dblVal);

    hr = pStrLoad->get_Property(_T("fy"), &var);
    if (FAILED(hr))
        return hr;
    m_bearing.SetYieldStrength(var.dblVal);

    hr = pStrLoad->get_Property(_T("fth"), &var);
    if (FAILED(hr))
        return hr;
    m_bearing.SetFatigueThreshold(var.dblVal);

    var.vt = VT_I4;
    hr = pStrLoad->get_Property(_T("n"), &var);
    if (FAILED(hr))
        return hr;
    m_bearing.SetNumIntLayers(var.lVal);

    var.vt = VT_R8;
    hr = pStrLoad->get_Property(_T("ps"), &var);
    if (FAILED(hr))
        return hr;
    m_bearing.SetDensitySteel(var.dblVal);

    hr = pStrLoad->get_Property(_T("pe"), &var);
    if (FAILED(hr))
        return hr;
    m_bearing.SetDensityElastomer(var.dblVal);

    hr = pStrLoad->get_Property(_T("DL"), &var);
    if (FAILED(hr))
        return hr;
    m_bearing_loads.SetDeadLoad(var.dblVal);

    hr = pStrLoad->get_Property(_T("LL"), &var);
    if (FAILED(hr))
        return hr;
    m_bearing_loads.SetLiveLoad(var.dblVal);

    hr = pStrLoad->get_Property(_T("rotx"), &var);
    if (FAILED(hr))
        return hr;
    m_bearing_loads.SetRotationX(var.dblVal);

    hr = pStrLoad->get_Property(_T("roty"), &var);
    if (FAILED(hr))
        return hr;
    m_bearing_loads.SetRotationY(var.dblVal);

    hr = pStrLoad->get_Property(_T("rot_static"), &var);
    if (FAILED(hr))
        return hr;
    m_bearing_loads.SetStaticRotation(var.dblVal);

    hr = pStrLoad->get_Property(_T("rot_cyclic"), &var);
    if (FAILED(hr))
        return hr;
    m_bearing_loads.SetCyclicRotation(var.dblVal);

    hr = pStrLoad->get_Property(_T("shear_def"), &var);
    if (FAILED(hr))
        return hr;
    m_bearing_loads.SetShearDeformation(var.dblVal);

    hr = pStrLoad->EndUnit(); // Bearing
    if (FAILED(hr))
        return hr;

    return S_OK;
}

void CBearingDoc::LoadDocumentSettings()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   __super::LoadDocumentSettings();
}

void CBearingDoc::SaveDocumentSettings()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   __super::SaveDocumentSettings();
}

CString CBearingDoc::GetDocumentationRootLocation()
{
   CEAFApp* pApp = EAFGetApp();
   return pApp->GetDocumentationRootLocation();
}

