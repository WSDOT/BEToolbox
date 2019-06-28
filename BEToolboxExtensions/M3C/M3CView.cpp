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
// M3CMainView.cpp : implementation file
//

#include "stdafx.h"
#include "M3CView.h"

// CM3CView

IMPLEMENT_DYNCREATE(CM3CView, CView)

CM3CView::CM3CView()
{

}

CM3CView::~CM3CView()
{
}

BEGIN_MESSAGE_MAP(CM3CView, CView)
END_MESSAGE_MAP()

// CM3CMainView drawing

void CM3CView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CM3CMainView diagnostics

#ifdef _DEBUG
void CM3CView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CM3CView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG

// CM3CView message handlers
