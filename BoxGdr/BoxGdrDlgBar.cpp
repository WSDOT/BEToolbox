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
#include "BoxGdrDlgBar.h"
#include <MFCTools\CustomDDX.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CBoxGdrDialogBar

IMPLEMENT_DYNAMIC(CBoxGdrDialogBar, CDialogBar)

CBoxGdrDialogBar::CBoxGdrDialogBar()
{
   m_pGrid = nullptr;
}

CBoxGdrDialogBar::~CBoxGdrDialogBar()
{
   delete m_pGrid;
}


BEGIN_MESSAGE_MAP(CBoxGdrDialogBar, CDialogBar)
END_MESSAGE_MAP()



// CBoxGdrDialogBar message handlers



void CBoxGdrDialogBar::DoDataExchange(CDataExchange* pDX)
{
   CDialogBar::DoDataExchange(pDX);

   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   int i = (pApp->GetUnitsMode() == eafTypes::umUS ? 0 : 1);
   DDX_Radio(pDX,IDC_US,i);

   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   DDX_MetaFileStatic(pDX, IDC_SCHEMATIC, m_Schematic, AfxGetResourceHandle(), _T("BoxGdr"), _T("Metafile") );

   m_pGrid->DoDataExchange(pDX);
}

BOOL CBoxGdrDialogBar::Create(CWnd* pParentWnd,UINT nIDTemplate,UINT nStyle,UINT nID)
{
   if ( CDialogBar::Create(pParentWnd,nIDTemplate,nStyle,nID) )
      InitGrid();

   return TRUE;
}

void CBoxGdrDialogBar::InitGrid()
{
   m_pGrid = new CBoxGdrDimensionGrid;

	m_pGrid->SubclassDlgItem(IDC_DIMENSIONS, this);
   m_pGrid->CustomInit();
}

void CBoxGdrDialogBar::AddProblem()
{
   m_pGrid->AddProblem();
}

void CBoxGdrDialogBar::RemoveSelectedProblems()
{
   m_pGrid->RemoveSelectedProblems();
}

BOOL CBoxGdrDialogBar::AreProblemsSelected()
{
   return m_pGrid->AreProblemsSelected();
}

void CBoxGdrDialogBar::OnUnitsModeChanged()
{
   m_pGrid->OnUnitsModeChanged();
}

#if defined _DEBUG
void CBoxGdrDialogBar::AssertValid() const
{
   AFX_MANAGE_STATE(AfxGetAppModuleState());
   CDialogBar::AssertValid();
}
#endif
