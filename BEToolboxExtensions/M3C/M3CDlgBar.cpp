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

// DlgBar.cpp : implementation file
//

#include "stdafx.h"
#include "..\resource.h"
#include <EAF\EAFApp.h>
#include "M3CDlgBar.h"
#include <MFCTools\CustomDDX.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CM3CDlgBar

IMPLEMENT_DYNAMIC(CM3CDlgBar, CDialogBar)

CM3CDlgBar::CM3CDlgBar()
{
}

CM3CDlgBar::~CM3CDlgBar()
{
}


BEGIN_MESSAGE_MAP(CM3CDlgBar, CDialogBar)
END_MESSAGE_MAP()



// CM3CDlgBar message handlers

BOOL CM3CDlgBar::Create(CWnd* pParentWnd, LPCTSTR lpszTemplateName, UINT nStyle, UINT nID)
{
   BOOL bResult = CDialogBar::Create(pParentWnd, lpszTemplateName, nStyle, nID);
   if (bResult)
   {
      FillStrandList();
   }

   return bResult;
}

BOOL CM3CDlgBar::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
   return CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
}

void CM3CDlgBar::FillStrandList()
{
   CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_TENDON_TYPE);

   lrfdStrandIter iter(matPsStrand::Gr1860, matPsStrand::LowRelaxation);
   lrfdStrandPool* pPool = lrfdStrandPool::GetInstance();
   for (iter.Begin(); iter; iter.Next())
   {
      const matPsStrand* pStrand = iter.GetCurrentStrand();
      int idx = pCB->AddString(pStrand->GetName().c_str());

      Int32 key = pPool->GetStrandKey(pStrand);
      pCB->SetItemData(idx, key);
   }
}

void CM3CDlgBar::DoDataExchange(CDataExchange* pDX)
{
   CDialogBar::DoDataExchange(pDX);

   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   // Column Section
   DDX_UnitValueAndTag(pDX,IDC_DIAMETER,IDC_DIAMETER_UNIT,m_ProblemParams.D,pDispUnits->ComponentDim);
   
   // Confined Concrete
   DDX_UnitValueAndTag(pDX,IDC_FCO,      IDC_FCO_UNIT, m_ProblemParams.fco,      pDispUnits->Stress);
   DDX_Text(pDX, IDC_ECO, m_ProblemParams.eco);

   // Spiral Reinforcement
   DDX_UnitValueAndTag(pDX, IDC_ASPIRAL, IDC_ASPIRAL_UNIT, m_ProblemParams.AsSpiral, pDispUnits->Area);
   DDX_UnitValueAndTag(pDX, IDC_DB, IDC_DB_UNIT, m_ProblemParams.db, pDispUnits->ComponentDim);
   DDX_UnitValueAndTag(pDX, IDC_S, IDC_S_UNIT, m_ProblemParams.S, pDispUnits->ComponentDim);
   DDX_UnitValueAndTag(pDX, IDC_FY_SPIRAL, IDC_FY_SPIRAL_UNIT, m_ProblemParams.FySpiral, pDispUnits->Stress);
   DDX_Text(pDX, IDC_ESU, m_ProblemParams.esu);

   // Longitudinal Reinforcement
   DDX_UnitValueAndTag(pDX, IDC_FY, IDC_FY_UNIT, m_ProblemParams.fy, pDispUnits->Stress);
   DDX_UnitValueAndTag(pDX, IDC_FU, IDC_FU_UNIT, m_ProblemParams.fu, pDispUnits->Stress);
   DDX_UnitValueAndTag(pDX, IDC_ES, IDC_ES_UNIT, m_ProblemParams.Es, pDispUnits->ModE);
   DDX_Text(pDX, IDC_ESH, m_ProblemParams.esh);
   DDX_Text(pDX, IDC_EFR, m_ProblemParams.efr);
   DDX_UnitValueAndTag(pDX, IDC_COVER, IDC_COVER_UNIT, m_ProblemParams.Cover, pDispUnits->ComponentDim);

   // Bonded Bars
   CString strTag;
   strTag.Format(_T("%s/bar"), pDispUnits->Area.UnitOfMeasure.UnitTag().c_str());

   DDX_UnitValue(pDX,IDC_BONDED_BAR_AS, m_ProblemParams.Bonded_Rebar_As_per_bar,      pDispUnits->Area);
   DDX_Text(pDX, IDC_BONDED_BAR_AS_UNIT, strTag);
   DDX_Text(pDX, IDC_BONDED_BAR_N, m_ProblemParams.Bonded_Rebar_nBars);

   // Unbonded Bars
   DDX_UnitValue(pDX, IDC_UNBONDED_BAR_AS, m_ProblemParams.Unbonded_Rebar_As_per_bar, pDispUnits->Area);
   DDX_Text(pDX, IDC_UNBONDED_BAR_AS_UNIT, strTag);
   DDX_Text(pDX, IDC_UNBONDED_BAR_N, m_ProblemParams.Unbonded_Rebar_nBars);
   DDX_UnitValueAndTag(pDX, IDC_UNBONDED_BAR_LU, IDC_UNBONDED_BAR_LU_UNIT, m_ProblemParams.Unbonded_Rebar_Lu, pDispUnits->XSectionDim);

   // Unbonded Tendons
   lrfdStrandPool* pPool = lrfdStrandPool::GetInstance();
   Int32 strandKey = pPool->GetStrandKey(m_ProblemParams.pStrand);
   DDX_CBItemData(pDX, IDC_TENDON_TYPE, strandKey);
   if (pDX->m_bSaveAndValidate)
   {
      m_ProblemParams.pStrand = pPool->GetStrand(strandKey);
   }
   DDX_UnitValueAndTag(pDX, IDC_TENDON_DIAMETER, IDC_TENDON_DIAMETER_UNIT, m_ProblemParams.Tendon_Ring_Diameter, pDispUnits->ComponentDim);
   DDX_Text(pDX, IDC_TENDON_N, m_ProblemParams.Tendon_nStrands);
   DDX_UnitValueAndTag(pDX, IDC_FPE, IDC_FPE_UNIT, m_ProblemParams.Tendon_fpe, pDispUnits->Stress);
   DDX_UnitValueAndTag(pDX, IDC_TENDON_LU, IDC_TENDON_LU_UNIT, m_ProblemParams.Tendon_Lu, pDispUnits->XSectionDim);


   // add analysis controls... P, NA, etc
   DDX_UnitValueAndTag(pDX, IDC_P, IDC_P_UNIT, m_ProblemParams.P, pDispUnits->GeneralForce);
   DDX_UnitValueAndTag(pDX, IDC_INIT_STEP, IDC_INIT_STEP_UNIT,m_ProblemParams.initialStep,pDispUnits->Curvature);
   DDX_Text(pDX, IDC_SLICES, m_ProblemParams.nSlices);
}
