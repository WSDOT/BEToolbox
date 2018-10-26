///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2016  Washington State Department of Transportation
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
#include "GirCompDlgBar.h"
#include <MFCTools\CustomDDX.h>

// CGirCompDialogTopBar

IMPLEMENT_DYNAMIC(CGirCompDialogTopBar, CDialogBar)

CGirCompDialogTopBar::CGirCompDialogTopBar()
{
   m_pGrid = NULL;
}

CGirCompDialogTopBar::~CGirCompDialogTopBar()
{
   delete m_pGrid;
}


BEGIN_MESSAGE_MAP(CGirCompDialogTopBar, CDialogBar)
END_MESSAGE_MAP()



// CGirCompDialogTopBar message handlers



void CGirCompDialogTopBar::DoDataExchange(CDataExchange* pDX)
{
   CDialogBar::DoDataExchange(pDX);

   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   int i = (pApp->GetUnitsMode() == eafTypes::umUS ? 0 : 1);
   DDX_Radio(pDX,IDC_US,i);

   m_pGrid->DoDataExchange(pDX);
}

BOOL CGirCompDialogTopBar::Create(CWnd* pParentWnd,UINT nIDTemplate,UINT nStyle,UINT nID)
{
   if ( CDialogBar::Create(pParentWnd,nIDTemplate,nStyle,nID) )
      InitGrid();

   return TRUE;
}

void CGirCompDialogTopBar::InitGrid()
{
   m_pGrid = new CGirCompDimensionGrid;

	m_pGrid->SubclassDlgItem(IDC_DIMENSIONS, this);
   m_pGrid->CustomInit();
}

void CGirCompDialogTopBar::AddProblem()
{
   m_pGrid->AddProblem();
}

void CGirCompDialogTopBar::RemoveSelectedProblems()
{
   m_pGrid->RemoveSelectedProblems();
}

BOOL CGirCompDialogTopBar::AreProblemsSelected()
{
   return m_pGrid->AreProblemsSelected();
}

void CGirCompDialogTopBar::OnUnitsModeChanged()
{
   m_pGrid->OnUnitsModeChanged();
}

#if defined _DEBUG
void CGirCompDialogTopBar::AssertValid() const
{
   AFX_MANAGE_STATE(AfxGetAppModuleState());
   CDialogBar::AssertValid();
}
#endif

// CGirCompDialogLeftBar

IMPLEMENT_DYNAMIC(CGirCompDialogLeftBar, CDialogBar)

CGirCompDialogLeftBar::CGirCompDialogLeftBar()
{
}

CGirCompDialogLeftBar::~CGirCompDialogLeftBar()
{
}


BEGIN_MESSAGE_MAP(CGirCompDialogLeftBar, CDialogBar)
END_MESSAGE_MAP()



// CGirCompDialogLeftBar message handlers



void CGirCompDialogLeftBar::DoDataExchange(CDataExchange* pDX)
{
   CDialogBar::DoDataExchange(pDX);

   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   DDX_MetaFileStatic(pDX, IDC_SCHEMATIC, m_Schematic, AfxGetResourceHandle(), _T("GirComp"), _T("Metafile") );
}

#if defined _DEBUG
void CGirCompDialogLeftBar::AssertValid() const
{
   AFX_MANAGE_STATE(AfxGetAppModuleState());
   CDialogBar::AssertValid();
}
#endif
