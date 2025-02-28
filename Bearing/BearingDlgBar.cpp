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

// DlgBar.cpp : implementation file
//

#include "stdafx.h"
#include "..\resource.h"
#include <EAF\EAFApp.h>
#include "BearingDlgBar.h"
#include <MFCTools\CustomDDX.h>
#include <LibraryFw/Library.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CBearingDialogBar

IMPLEMENT_DYNAMIC(CBearingDialogBar, CDialogBar)

CBearingDialogBar::CBearingDialogBar()
    : m_length{ 0 },
    m_width{ 0 },
    m_cover{ 0 },
    m_layer{ 0 },
    m_shim{ 0 },
    m_n_layers{ 0 },
    m_Gmin{ 0 },
    m_Gmax{ 0 },
    m_Fy{ 0 },
    m_Fth{ 0 },
    m_DL{ 0 },
    m_LL{ 0 },
    m_rot_x{ 0 },
    m_rot_y{ 0 },
    m_rot_st{ 0 },
    m_rot_cy{ 0 },
    m_shear_def{ 0 },
    m_ext_plates{ false },
    m_specification{WBFL::LRFD::BDSManager::Edition::LastEdition}
{
}




CBearingDialogBar::~CBearingDialogBar()
{
}


BEGIN_MESSAGE_MAP(CBearingDialogBar, CDialogBar)
END_MESSAGE_MAP()



// CBearingDialogBar message handlers



void CBearingDialogBar::DoDataExchange(CDataExchange* pDX)
{
   CDialogBar::DoDataExchange(pDX);

   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   int i = (pApp->GetUnitsMode() == eafTypes::umUS ? 0 : 1);
   DDX_Radio(pDX, IDC_US, i);
   DDX_Radio(pDX, IDC_METHOD_A, m_method);
   CComboBox* pcbSpecification = (CComboBox*)GetDlgItem(IDC_SPECIFICATION2);
   if (pDX->m_bSaveAndValidate)
   {
       int menuIdx;
       DDX_CBIndex(pDX, IDC_SPECIFICATION2, menuIdx);
       int specIdx = pcbSpecification->GetItemData(menuIdx);
       m_specification = static_cast<WBFL::LRFD::BDSManager::Edition>(specIdx);
   }
   else
   {
       CBearingDoc* pDoc = (CBearingDoc*)((CFrameWnd*)GetParent())->GetActiveDocument();
       m_specification = static_cast<WBFL::LRFD::BDSManager::Edition>(pDoc->GetSpecification());
       int defaultIdx = static_cast<int>(m_specification) - static_cast<int>(WBFL::LRFD::BDSManager::Edition::FourthEditionWith2009Interims);
       pcbSpecification->SetCurSel(defaultIdx);
   }

   DDX_UnitValueAndTag(pDX, IDC_BLENGTH, IDC_BLENGTH_UNIT, m_length, pDispUnits->ComponentDim);
   DDV_GreaterThanZero(pDX, IDC_BLENGTH, m_length);
   DDX_UnitValueAndTag(pDX, IDC_BWIDTH, IDC_BWIDTH_UNIT, m_width, pDispUnits->ComponentDim);
   DDV_GreaterThanZero(pDX, IDC_BWIDTH, m_width);
   DDX_UnitValueAndTag(pDX, IDC_COVER, IDC_COVER_UNIT, m_cover, pDispUnits->ComponentDim);
   DDV_GreaterThanZero(pDX, IDC_COVER, m_cover);
   DDX_UnitValueAndTag(pDX, IDC_LAYER, IDC_LAYER_UNIT, m_layer, pDispUnits->ComponentDim);
   DDV_GreaterThanZero(pDX, IDC_LAYER, m_layer);
   DDX_UnitValueAndTag(pDX, IDC_SHIM, IDC_SHIM_UNIT, m_shim, pDispUnits->ComponentDim);
   DDV_UnitValueZeroOrMore(pDX, IDC_SHIM, m_shim, pDispUnits->ComponentDim);

   DDX_Text(pDX, IDC_N_LAYERS, m_n_layers);

   if (pDX->m_bSaveAndValidate)
   {
       CEdit* editWindow = (CEdit*)GetDlgItem(IDC_N_LAYERS);
       CString strValue;
       editWindow->GetWindowText(strValue);
       if (strValue.Find('.') != -1 || _ttoi(strValue) <= 0)
       {
           pDX->PrepareEditCtrl(IDC_N_LAYERS);
           AfxMessageBox(_T("Enter an integer greater than zero"), MB_OK);
           pDX->Fail();
       }
   }


   DDX_UnitValueAndTag(pDX, IDC_SHEAR_MOD_MIN, IDC_SHEAR_MOD_MIN_UNIT, m_Gmin, pDispUnits->ModE);
   DDV_GreaterThanZero(pDX, IDC_SHEAR_MOD_MIN, m_Gmin);
   DDX_UnitValueAndTag(pDX, IDC_SHEAR_MOD_MAX, IDC_SHEAR_MOD_MAX_UNIT, m_Gmax, pDispUnits->ModE);
   DDV_GreaterThanZero(pDX, IDC_SHEAR_MOD_MAX, m_Gmax);
   if (m_method == 0)
   {
       DDV_UnitValueLimitOrMore(pDX, IDC_SHEAR_MOD_MAX, m_Gmax, m_Gmin, pDispUnits->ModE);
   }
   DDX_UnitValueAndTag(pDX, IDC_YIELD, IDC_YIELD_UNIT, m_Fy, pDispUnits->ModE);
   DDV_UnitValueZeroOrMore(pDX, IDC_YIELD, m_Fy, pDispUnits->ModE);
   DDX_UnitValueAndTag(pDX, IDC_FATIGUE, IDC_FATIGUE_UNIT, m_Fth, pDispUnits->ModE);
   DDV_UnitValueZeroOrMore(pDX, IDC_FATIGUE, m_Fth, pDispUnits->ModE);
   DDX_UnitValueAndTag(pDX, IDC_DL, IDC_DL_UNIT, m_DL, pDispUnits->GeneralForce);
   DDX_UnitValueAndTag(pDX, IDC_LL, IDC_LL_UNIT, m_LL, pDispUnits->GeneralForce);
   DDX_Text(pDX,IDC_ROT_STATIC, m_rot_st);
   DDX_Text(pDX, IDC_ROT_CYCLIC, m_rot_cy);
   DDX_Text(pDX, IDC_ROT_X, m_rot_x);
   DDX_Text(pDX, IDC_ROT_Y, m_rot_y);
   DDX_UnitValueAndTag(pDX, IDC_SHEAR_DEF, IDC_SHEAR_DEF_UNIT, m_shear_def, pDispUnits->ComponentDim);
   DDX_Check_Bool(pDX, IDC_FIXED_Y, m_fixed_y);
   DDX_Check_Bool(pDX, IDC_FIXED_X, m_fixed_x);
   DDX_Check_Bool(pDX, IDC_EXTERNAL_PLATES, m_ext_plates);
}


void CBearingDialogBar::SetBearingParameters(CBearingDialogBar& dlgBar,
    const WBFL::EngTools::Bearing& brg, 
    const WBFL::EngTools::BearingLoads& brg_loads,
    const WBFL::EngTools::BearingCalculator& brg_calc)
{


    dlgBar.m_method = (brg_calc.GetAnalysisMethod()==WBFL::EngTools::BearingCalculator::AnalysisMethod::MethodA? 0:1);


    CBearingDoc* pDoc = (CBearingDoc*)((CFrameWnd*)GetParent())->GetActiveDocument();

    dlgBar.m_specification = pDoc->GetSpecification();
    dlgBar.m_length = brg.GetLength();
    dlgBar.m_width = brg.GetWidth();
    dlgBar.m_cover = brg.GetCoverThickness();
    dlgBar.m_layer = brg.GetIntermediateLayerThickness();
    dlgBar.m_shim = brg.GetSteelShimThickness();
    dlgBar.m_n_layers = brg.GetNumIntLayers();
    dlgBar.m_Gmin = brg.GetShearModulusMinimum();
    dlgBar.m_Gmax = brg.GetShearModulusMaximum();
    dlgBar.m_Fy = brg.GetYieldStrength();
    dlgBar.m_Fth = brg.GetFatigueThreshold();
    dlgBar.m_DL = brg_loads.GetDeadLoad();
    dlgBar.m_LL = brg_loads.GetLiveLoad();
    dlgBar.m_rot_x = brg_loads.GetRotationX();
    dlgBar.m_rot_y = brg_loads.GetRotationY();
    dlgBar.m_rot_st = brg_loads.GetStaticRotation();
    dlgBar.m_rot_cy = brg_loads.GetCyclicRotation();
    dlgBar.m_shear_def = brg_loads.GetShearDeformation();
    dlgBar.m_ext_plates = brg.UseExternalPlates();
    dlgBar.m_fixed_x = brg_loads.GetFixedTranslationX();
    dlgBar.m_fixed_y = brg_loads.GetFixedTranslationY();
}

void CBearingDialogBar::SetBearing(
    WBFL::EngTools::Bearing& brg,
    WBFL::EngTools::BearingLoads& brg_loads,
    WBFL::EngTools::BearingCalculator& brg_calc)
{
    CBearingDoc* pDoc = (CBearingDoc*)((CFrameWnd*)GetParent())->GetActiveDocument();
    pDoc->SetSpecification(m_specification);
    brg_calc.SetSpecification(m_specification);
    brg_calc.SetAnalysisMethod(static_cast<WBFL::EngTools::BearingCalculator::AnalysisMethod>(m_method));
    brg.SetLength(m_length);
    brg.SetWidth(m_width);
    brg.SetShearModulusMinimum(m_Gmin);
    brg.SetShearModulusMaximum(m_Gmax);
    brg.SetIntermediateLayerThickness(m_layer);
    brg.SetCoverThickness(m_cover);
    brg.SetSteelShimThickness(m_shim);
    brg.SetYieldStrength(m_Fy);
    brg.SetFatigueThreshold(m_Fth);
    brg.SetNumIntLayers(m_n_layers);
    brg.SetUseExternalPlates(m_ext_plates);
    brg_loads.SetDeadLoad(m_DL);
    brg_loads.SetLiveLoad(m_LL);
    brg_loads.SetShearDeformation(m_shear_def);
    brg_loads.SetRotationX(m_rot_x);
    brg_loads.SetRotationY(m_rot_y);
    brg_loads.SetStaticRotation(m_rot_st);
    brg_loads.SetCyclicRotation(m_rot_cy);
    brg_loads.SetFixedTranslationX(m_fixed_x);
    brg_loads.SetFixedTranslationY(m_fixed_y);
}

#if defined _DEBUG
void CBearingDialogBar::AssertValid() const
{
   AFX_MANAGE_STATE(AfxGetAppModuleState());
   CDialogBar::AssertValid();
}
#endif

BOOL CBearingDialogBar::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{

   if ( !CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID) )
      return FALSE;

   CComboBox* pcbSpecification = (CComboBox*)GetDlgItem(IDC_SPECIFICATION2);

   int menuIdx = 0;

   for (int specIdx = static_cast<int>(WBFL::LRFD::BDSManager::Edition::FourthEditionWith2009Interims); specIdx <=
       static_cast<int>(WBFL::LRFD::BDSManager::Edition::LastEdition)-1; ++specIdx)
   {
       WBFL::LRFD::BDSManager::Edition edition = static_cast<WBFL::LRFD::BDSManager::Edition>(specIdx);
       LPCTSTR editionStr = WBFL::LRFD::BDSManager::GetEditionAsString(edition);
       menuIdx = pcbSpecification->AddString(editionStr);
       pcbSpecification->SetItemData(menuIdx, specIdx);
   }

   pcbSpecification->SetCurSel(menuIdx);

   return TRUE;
}
