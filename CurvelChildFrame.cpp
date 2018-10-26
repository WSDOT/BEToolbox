///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2015  Washington State Department of Transportation
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

#include "StdAfx.h"
#include "resource.h"
#include "CurvelChildFrame.h"
#include "CurvelDoc.h"

IMPLEMENT_DYNCREATE(CCurvelChildFrame, CEAFChildFrame)

CCurvelChildFrame::CCurvelChildFrame(void)
{
}

CCurvelChildFrame::~CCurvelChildFrame(void)
{
}

BEGIN_MESSAGE_MAP(CCurvelChildFrame, CEAFChildFrame)
   ON_WM_CREATE()
END_MESSAGE_MAP()

BOOL CCurvelChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
   // force this window to be maximized (not sure why WS_VISIBLE is required)
   cs.style |= WS_MAXIMIZE | WS_VISIBLE;

   return CEAFChildFrame::PreCreateWindow(cs);
}

int CCurvelChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CEAFChildFrame::OnCreate(lpCreateStruct) == -1)
      return -1;

   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   EnableToolTips();

   SetIcon(AfxGetApp()->LoadIcon(IDR_CURVEL),TRUE);

   return 0;
}

void CCurvelChildFrame::OnUpdate()
{
   CCurvelDoc* pDoc = (CCurvelDoc*)GetActiveDocument();
   if ( pDoc )
   {
      pDoc->SetModifiedFlag();
      pDoc->UpdateAllViews(NULL);
   }
}

#if defined _DEBUG
void CCurvelChildFrame::AssertValid() const
{
   CEAFChildFrame::AssertValid();
}
#endif

BOOL CCurvelChildFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle , CWnd* pParentWnd , CCreateContext* pContext)
{
   // CEAFChildFrame::LoadFrame is going to load resources based on nIDResource
   // We have to set the module context to this module so the load happens
   // correctly
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   if ( !CEAFChildFrame::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext) )
      return FALSE;

   return TRUE;
}
