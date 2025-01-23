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
#include "GenCompDlgBar.h"
#include <MFCTools\CustomDDX.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CGenCompDialogBar

IMPLEMENT_DYNAMIC(CGenCompDialogBar, CDialogBar)

CGenCompDialogBar::CGenCompDialogBar()
{
   m_pPrimaryGrid = nullptr;
   m_pSecondaryGrid = nullptr;
}

CGenCompDialogBar::~CGenCompDialogBar()
{
   delete m_pPrimaryGrid;
   delete m_pSecondaryGrid;
}


BEGIN_MESSAGE_MAP(CGenCompDialogBar, CDialogBar)
END_MESSAGE_MAP()



// CGenCompDialogBar message handlers



void CGenCompDialogBar::DoDataExchange(CDataExchange* pDX)
{
   CDialogBar::DoDataExchange(pDX);

   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   int i = (pApp->GetUnitsMode() == eafTypes::umUS ? 0 : 1);
   DDX_Radio(pDX,IDC_US,i);

   CGenCompDoc* pDoc = (CGenCompDoc*)EAFGetDocument();
   Float64 modularRatio = pDoc->GetModularRatio();

   DDX_Text(pDX,IDC_N,modularRatio);

   pDoc->SetModularRatio(modularRatio);

   m_pPrimaryGrid->DoDataExchange(pDX);
   m_pSecondaryGrid->DoDataExchange(pDX);
}

BOOL CGenCompDialogBar::Create(CWnd* pParentWnd,UINT nIDTemplate,UINT nStyle,UINT nID)
{
   if ( CDialogBar::Create(pParentWnd,nIDTemplate,nStyle,nID) )
      InitGrids();

   return TRUE;
}

void CGenCompDialogBar::InitGrids()
{
   m_pPrimaryGrid = new CGenCompDimensionGrid;
   m_pSecondaryGrid = new CGenCompDimensionGrid;

	m_pPrimaryGrid->SubclassDlgItem(IDC_PRIMARY_SHAPE_GRID, this);
   m_pPrimaryGrid->CustomInit(true);

	m_pSecondaryGrid->SubclassDlgItem(IDC_SECONDARY_SHAPE_GRID, this);
   m_pSecondaryGrid->CustomInit(false);
}

void CGenCompDialogBar::AddPrimaryPoint()
{
   m_pPrimaryGrid->AddPoint();
}

void CGenCompDialogBar::RemoveSelectedPrimaryPoints()
{
   m_pPrimaryGrid->RemoveSelectedPoints();
}

BOOL CGenCompDialogBar::ArePrimaryPointsSelected()
{
   return m_pPrimaryGrid->ArePointsSelected();
}

void CGenCompDialogBar::AddSecondaryPoint()
{
   m_pSecondaryGrid->AddPoint();
}

void CGenCompDialogBar::RemoveSelectedSecondaryPoints()
{
   m_pSecondaryGrid->RemoveSelectedPoints();
}

BOOL CGenCompDialogBar::AreSecondaryPointsSelected()
{
   return m_pSecondaryGrid->ArePointsSelected();
}

void CGenCompDialogBar::OnUnitsModeChanged()
{
   m_pPrimaryGrid->OnUnitsModeChanged();
   m_pSecondaryGrid->OnUnitsModeChanged();
}

#if defined _DEBUG
void CGenCompDialogBar::AssertValid() const
{
   AFX_MANAGE_STATE(AfxGetAppModuleState());
   CDialogBar::AssertValid();
}
#endif
