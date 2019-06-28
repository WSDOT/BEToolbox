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
#include "SVTDlgBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static std::_tstring gs_Name[]{
   _T("Type I"),
   _T("Type II"),
   _T("Type III"),
   _T("Type IV"),
   _T("Type V"),
   _T("Type VI"),
   _T("BT54"),
   _T("BT63"),
   _T("BT72"),
   _T("W42G"),
   _T("W50G"),
   _T("W58G"),
   _T("W74G"),
   _T("WF36G"),
   _T("WF42G"),
   _T("WF50G"),
   _T("WF58G"),
   _T("WF66G"),
   _T("WF74G"),
   _T("WF83G"),
   _T("WF95G"),
   _T("WF100G"),
   _T("Tx28"),
   _T("Tx34"),
   _T("Tx40"),
   _T("Tx46"),
   _T("Tx54"),
   _T("Tx62"),
   _T("Tx70"),

   _T("U54G4"),
   _T("U54G5"),
   _T("U54G6"),
   _T("U66G4"),
   _T("U66G5"),
   _T("U66G6"),
   _T("U78G4"),
   _T("U78G5"),
   _T("U78G6"),
   _T("UF60G4"),
   _T("UF60G5"),
   _T("UF60G6"),
   _T("UF72G4"),
   _T("UF72G5"),
   _T("UF72G6"),
   _T("UF84G4"),
   _T("UF84G5"),
   _T("UF84G6"),
   _T("U40"),
   _T("U54"),
};

// CSVTDlgBar

IMPLEMENT_DYNAMIC(CSVTDlgBar, CDialogBar)

CSVTDlgBar::CSVTDlgBar()
{
}

CSVTDlgBar::~CSVTDlgBar()
{
}


BEGIN_MESSAGE_MAP(CSVTDlgBar, CDialogBar)
END_MESSAGE_MAP()


// CM3CDlgBar message handlers

BOOL CSVTDlgBar::Create(CWnd* pParentWnd, LPCTSTR lpszTemplateName, UINT nStyle, UINT nID)
{
   BOOL bResult = CDialogBar::Create(pParentWnd, lpszTemplateName, nStyle, nID);
   if (bResult)
   {
      FillGirderList();
   }

   return bResult;
}

BOOL CSVTDlgBar::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
   return CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
}

void CSVTDlgBar::FillGirderList()
{
   CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_GIRDERS);

   int n = sizeof(gs_Name) / sizeof(gs_Name[0]);
   for (int i = 0; i < n; i++)
   {
      pCB->AddString(gs_Name[i].c_str());
   }
}

void CSVTDlgBar::DoDataExchange(CDataExchange* pDX)
{
   CDialogBar::DoDataExchange(pDX);
}
