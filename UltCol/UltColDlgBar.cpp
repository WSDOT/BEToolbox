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
#include "UltColDlgBar.h"
#include <MFCTools\CustomDDX.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CUltColDlgBar

IMPLEMENT_DYNAMIC(CUltColDlgBar, CDialogBar)

CUltColDlgBar::CUltColDlgBar()
{
}

CUltColDlgBar::~CUltColDlgBar()
{
}


BEGIN_MESSAGE_MAP(CUltColDlgBar, CDialogBar)
END_MESSAGE_MAP()



// CUltColDlgBar message handlers



void CUltColDlgBar::DoDataExchange(CDataExchange* pDX)
{
   CDialogBar::DoDataExchange(pDX);

   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   int i = (pApp->GetUnitsMode() == eafTypes::umUS ? 0 : 1);
   DDX_Radio(pDX,IDC_US,i);

   DDX_UnitValueAndTag(pDX,IDC_DIAMETER,IDC_DIAMETER_UNIT,m_Diameter,pDispUnits->ComponentDim);
   DDX_UnitValueAndTag(pDX,IDC_FC,      IDC_FC_UNIT,      m_Fc,      pDispUnits->Stress);
   DDX_UnitValueAndTag(pDX,IDC_COVER,   IDC_COVER_UNIT,   m_Cover,   pDispUnits->ComponentDim);
   DDX_UnitValueAndTag(pDX,IDC_AS,      IDC_AS_UNIT,      m_As,      pDispUnits->Area);
   DDX_UnitValueAndTag(pDX,IDC_ES,      IDC_ES_UNIT,      m_Es,      pDispUnits->Stress);
   DDX_UnitValueAndTag(pDX,IDC_FY,      IDC_FY_UNIT,      m_Fy,      pDispUnits->Stress);
   DDX_Text(pDX, IDC_ECL, m_ecl);
   DDX_Text(pDX, IDC_ETL, m_etl);
}
