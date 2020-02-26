///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2020  Washington State Department of Transportation
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

// ExampleToolView.cpp : implementation file
//

#include "stdafx.h"
#include "ExampleToolView.h"
#include "ExampleToolDoc.h"

#include <EAF\EAFHints.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CExampleToolView

IMPLEMENT_DYNCREATE(CExampleToolView, CEAFView)

CExampleToolView::CExampleToolView()
{

}

CExampleToolView::~CExampleToolView()
{
}

BEGIN_MESSAGE_MAP(CExampleToolView, CEAFView)
   //ON_COMMAND(ID_FILE_PRINT_DIRECT,&CExampleToolView::OnFilePrint)
END_MESSAGE_MAP()

// CExampleToolView diagnostics

#ifdef _DEBUG
void CExampleToolView::AssertValid() const
{
	CEAFView::AssertValid();
}

#ifndef _WIN32_WCE
void CExampleToolView::Dump(CDumpContext& dc) const
{
	CEAFView::Dump(dc);
}
#endif
#endif //_DEBUG


// CExampleToolView message handlers

void CExampleToolView::OnInitialUpdate()
{
   CExampleToolDoc* pDoc = (CExampleToolDoc*)GetDocument();

   m_pFrame = (CExampleToolChildFrame*)GetParentFrame();

   CEAFView::OnInitialUpdate();
}

void CExampleToolView::OnUpdate(CView* pSender,LPARAM lHint,CObject* pHint)
{
   CEAFView::OnUpdate(pSender, lHint, pHint);
}

void CExampleToolView::OnDraw(CDC* pDC)
{
   pDC->TextOut(0, 0, _T("Example Tool"));
}
