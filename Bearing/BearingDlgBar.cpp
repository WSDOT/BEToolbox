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
