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

// DlgBar.cpp : implementation file
//

#include "stdafx.h"
#include "..\resource.h"
#include <EAF\EAFApp.h>
#include "BearingDlgBar.h"
#include "BearingDoc.h"
#include <MFCTools\CustomDDX.h>

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
    m_shear_def{ 0 }
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
   CBearingDoc* pDoc = (CBearingDoc*)((CFrameWnd*)GetParent())->GetActiveDocument();

   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   int i = (pApp->GetUnitsMode() == eafTypes::umUS ? 0 : 1);
   DDX_Radio(pDX, IDC_US, i);
   DDX_Radio(pDX, IDC_METHOD_A, m_method_a);
   DDX_UnitValueAndTag(pDX, IDC_BLENGTH, IDC_BLENGTH_UNIT, m_length, pDispUnits->ComponentDim);
   DDX_UnitValueAndTag(pDX, IDC_BWIDTH, IDC_BWIDTH_UNIT, m_width, pDispUnits->ComponentDim);
   DDX_UnitValueAndTag(pDX, IDC_COVER, IDC_COVER_UNIT, m_cover, pDispUnits->ComponentDim);
   DDX_UnitValueAndTag(pDX, IDC_LAYER, IDC_LAYER_UNIT, m_layer, pDispUnits->ComponentDim);
   DDX_UnitValueAndTag(pDX, IDC_SHIM, IDC_SHIM_UNIT, m_shim, pDispUnits->ComponentDim);
   DDX_Text(pDX, IDC_N_LAYERS, m_n_layers);
   DDX_UnitValueAndTag(pDX, IDC_SHEAR_MOD_MIN, IDC_SHEAR_MOD_MIN_UNIT, m_Gmin, pDispUnits->ModE);
   DDX_UnitValueAndTag(pDX, IDC_SHEAR_MOD_MAX, IDC_SHEAR_MOD_MAX_UNIT, m_Gmax, pDispUnits->ModE);
   DDX_UnitValueAndTag(pDX, IDC_YIELD, IDC_YIELD_UNIT, m_Fy, pDispUnits->ModE);
   DDX_UnitValueAndTag(pDX, IDC_FATIGUE, IDC_FATIGUE_UNIT, m_Fth, pDispUnits->ModE);
   DDX_UnitValueAndTag(pDX, IDC_DL, IDC_DL_UNIT, m_DL, pDispUnits->GeneralForce);
   DDX_UnitValueAndTag(pDX, IDC_LL, IDC_LL_UNIT, m_LL, pDispUnits->GeneralForce);
   DDX_Text(pDX,IDC_ROT_STATIC, m_rot_st);
   DDX_Text(pDX, IDC_ROT_CYCLIC, m_rot_cy);
   DDX_Text(pDX, IDC_ROT_X, m_rot_x);
   DDX_Text(pDX, IDC_ROT_Y, m_rot_y);
   DDX_UnitValueAndTag(pDX, IDC_SHEAR_DEF, IDC_SHEAR_DEF_UNIT, m_shear_def, pDispUnits->ComponentDim);
   DDX_Radio(pDX, IDC_FIXED_X_YES, m_fixed_x);
   DDX_Radio(pDX, IDC_FIXED_Y_YES, m_fixed_y);
}


void CBearingDialogBar::SetBearingParameters(CBearingDialogBar& dlgBar,
    const WBFL::EngTools::Bearing& brg, 
    const WBFL::EngTools::BearingLoads& brg_loads,
    const WBFL::EngTools::BearingCalculator& brg_calc)
{

    if (brg_calc.GetAnalysisMethodA() == WBFL::EngTools::BearingCalculator::AnalysisMethodA::Yes)
    {
        dlgBar.m_method_a = 0;
    }
    else
    {
        dlgBar.m_method_a = 1;
    }


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

    if (brg_loads.GetFixedTranslationX() == WBFL::EngTools::BearingLoads::FixedTranslationX::No)
    {
        dlgBar.m_fixed_x = 1;
    }
    else
    {
        dlgBar.m_fixed_x = 0;
    }
    if (brg_loads.GetFixedTranslationY() == WBFL::EngTools::BearingLoads::FixedTranslationY::No)
    {
        dlgBar.m_fixed_y = 1;
    }
    else
    {
        dlgBar.m_fixed_y = 0;
    }

}

void CBearingDialogBar::SetBearing(
    WBFL::EngTools::Bearing& brg,
    WBFL::EngTools::BearingLoads& brg_loads,
    WBFL::EngTools::BearingCalculator& brg_calc)
{
    brg_calc.SetMethodA((WBFL::EngTools::BearingCalculator::AnalysisMethodA)m_method_a);
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
    brg_loads.SetDeadLoad(m_DL);
    brg_loads.SetLiveLoad(m_LL);
    brg_loads.SetShearDeformation(m_shear_def);
    brg_loads.SetRotationX(m_rot_x);
    brg_loads.SetRotationY(m_rot_y);
    brg_loads.SetStaticRotation(m_rot_st);
    brg_loads.SetCyclicRotation(m_rot_cy);
    brg_loads.SetFixedTranslationX((WBFL::EngTools::BearingLoads::FixedTranslationX)m_fixed_x);
    brg_loads.SetFixedTranslationY((WBFL::EngTools::BearingLoads::FixedTranslationY)m_fixed_y);
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
   // TODO: Add your specialized code here and/or call the base class

   if ( !CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID) )
      return FALSE;

   return TRUE;
}
