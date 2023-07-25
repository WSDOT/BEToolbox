///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2022  Washington State Department of Transportation
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
   //DDX_Radio(pDX, IDC_A, m_method);
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
