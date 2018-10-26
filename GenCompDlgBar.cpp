///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2014  Washington State Department of Transportation
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
#include "resource.h"
#include <EAF\EAFApp.h>
#include "GenCompDlgBar.h"
#include <MFCTools\CustomDDX.h>

// CGenCompDialogBar

IMPLEMENT_DYNAMIC(CGenCompDialogBar, CEAFPaneDialog)

CGenCompDialogBar::CGenCompDialogBar()
{
}

CGenCompDialogBar::~CGenCompDialogBar()
{
}


BEGIN_MESSAGE_MAP(CGenCompDialogBar, CEAFPaneDialog)
END_MESSAGE_MAP()



// CGenCompDialogBar message handlers



void CGenCompDialogBar::DoDataExchange(CDataExchange* pDX)
{
   CEAFPaneDialog::DoDataExchange(pDX);

   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   int i = (pApp->GetUnitsMode() == eafTypes::umUS ? 0 : 1);
   DDX_Radio(pDX,IDC_US,i);

   CGenCompDoc* pDoc = (CGenCompDoc*)EAFGetDocument();
   Float64 modularRatio = pDoc->GetModularRatio();

   DDX_Text(pDX,IDC_N,modularRatio);

   pDoc->SetModularRatio(modularRatio);

   m_PrimaryGrid.DoDataExchange(pDX);
   m_SecondaryGrid.DoDataExchange(pDX);
}

BOOL CGenCompDialogBar::Create(CWnd* pParentWnd,UINT nIDTemplate,UINT nStyle,UINT nID)
{
   if ( CEAFPaneDialog::Create(pParentWnd,nIDTemplate,nStyle,nID) )
      InitGrids();

   return TRUE;
}

void CGenCompDialogBar::InitGrids()
{
	m_PrimaryGrid.SubclassDlgItem(IDC_PRIMARY_SHAPE_GRID, this);
   m_PrimaryGrid.CustomInit(true);

	m_SecondaryGrid.SubclassDlgItem(IDC_SECONDARY_SHAPE_GRID, this);
   m_SecondaryGrid.CustomInit(false);
}

void CGenCompDialogBar::AddPrimaryPoint()
{
   m_PrimaryGrid.AddPoint();
}

void CGenCompDialogBar::RemoveSelectedPrimaryPoints()
{
   m_PrimaryGrid.RemoveSelectedPoints();
}

BOOL CGenCompDialogBar::ArePrimaryPointsSelected()
{
   return m_PrimaryGrid.ArePointsSelected();
}

void CGenCompDialogBar::AddSecondaryPoint()
{
   m_SecondaryGrid.AddPoint();
}

void CGenCompDialogBar::RemoveSelectedSecondaryPoints()
{
   m_SecondaryGrid.RemoveSelectedPoints();
}

BOOL CGenCompDialogBar::AreSecondaryPointsSelected()
{
   return m_SecondaryGrid.ArePointsSelected();
}

void CGenCompDialogBar::OnUnitsModeChanged()
{
   m_PrimaryGrid.OnUnitsModeChanged();
   m_SecondaryGrid.OnUnitsModeChanged();
}

#if defined _DEBUG
void CGenCompDialogBar::AssertValid() const
{
   AFX_MANAGE_STATE(AfxGetAppModuleState());
   CEAFPaneDialog::AssertValid();
}
#endif
