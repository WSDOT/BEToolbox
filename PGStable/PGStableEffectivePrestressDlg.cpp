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

// PGStableEffectivePrestressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\resource.h"
#include "PGStableEffectivePrestressDlg.h"
#include <EAF\EAFUtilities.h>
#include "PGStableDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



// CPGStableEffectivePrestressDlg dialog

IMPLEMENT_DYNAMIC(CPGStableEffectivePrestressDlg, CDialog)

CPGStableEffectivePrestressDlg::CPGStableEffectivePrestressDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(CPGStableEffectivePrestressDlg::IDD, pParent)
{

}

CPGStableEffectivePrestressDlg::~CPGStableEffectivePrestressDlg()
{
}

void CPGStableEffectivePrestressDlg::DoDataExchange(CDataExchange* pDX)
{
   CPGStableDoc* pDoc = (CPGStableDoc*)EAFGetDocument();

	CDialog::DoDataExchange(pDX);

   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   if ( pDX->m_bSaveAndValidate )
   {
      m_Grid.GetFpe(m_Strands.m_vFpe);
   }
   else
   {
      m_Grid.FillGrid(m_Strands.m_vFpe);
   }
}


BEGIN_MESSAGE_MAP(CPGStableEffectivePrestressDlg, CDialog)
   ON_BN_CLICKED(IDC_ADD, &CPGStableEffectivePrestressDlg::OnAdd)
   ON_BN_CLICKED(IDC_REMOVE, &CPGStableEffectivePrestressDlg::OnRemove)
END_MESSAGE_MAP()

// CPGStableEffectivePrestressDlg message handlers

BOOL CPGStableEffectivePrestressDlg::OnInitDialog()
{
   m_Grid.SubclassDlgItem(IDC_FPE_GRID,this);
   m_Grid.CustomInit();

   CDialog::OnInitDialog();

   // TODO:  Add extra initialization here

   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}

void CPGStableEffectivePrestressDlg::OnAdd()
{
   m_Grid.AddFpe();
}

void CPGStableEffectivePrestressDlg::OnRemove()
{
   m_Grid.RemoveSelected();
}
