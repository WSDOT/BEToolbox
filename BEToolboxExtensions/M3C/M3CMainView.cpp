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
#include "M3CMainView.h"

#include "M3CSectionView.h"
#include "M3CGraphView.h"
#include "M3CView.h"

// CM3CMainView

IMPLEMENT_DYNCREATE(CM3CMainView, CView)

CM3CMainView::CM3CMainView()
{

}

CM3CMainView::~CM3CMainView()
{
}

BEGIN_MESSAGE_MAP(CM3CMainView, CView)
   ON_WM_CREATE()
   ON_WM_SIZE()
END_MESSAGE_MAP()

int CM3CMainView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CView::OnCreate(lpCreateStruct) == -1)
      return -1;


   return 0;
}

// CM3CMainView drawing

void CM3CMainView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CM3CMainView diagnostics

#ifdef _DEBUG
void CM3CMainView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CM3CMainView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG

// CM3CMainView message handlers


void CM3CMainView::OnSize(UINT nType, int cx, int cy)
{
   CView::OnSize(nType, cx, cy);

   // TODO: Add your message handler code here
   if (m_SplitterWnd.GetSafeHwnd())
   {
      // Maintain the relative size/shape of the windows

      // get the row and column information
      int col1, col1Min;
      int col2, col2Min;
      int row1, row1Min;
      int row2, row2Min;
      m_SplitterWnd.GetColumnInfo(0, col1, col1Min);
      m_SplitterWnd.GetColumnInfo(1, col2, col2Min);
      m_SplitterWnd.GetRowInfo(0, row1, row1Min);
      m_SplitterWnd.GetRowInfo(1, row2, row2Min);

      int col_total = col1 + col2;
      int row_total = row1 + row2;

      // resize the window
      m_SplitterWnd.SetWindowPos(this, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE);

      // compute the new sizes
      col1 = cx*col1 / col_total;
      col2 = cx*col2 / col_total;
      row1 = cy*row1 / row_total;
      row2 = cy*row2 / row_total;
      
      // set the new sizes
      m_SplitterWnd.SetColumnInfo(0, col1, col1Min);
      m_SplitterWnd.SetColumnInfo(1, col2, col2Min);
      m_SplitterWnd.SetRowInfo(0, row1, row1Min);
      m_SplitterWnd.SetRowInfo(1, row2, row2Min);

      // update the splitters
      m_SplitterWnd.RecalcLayout();
   }
}


void CM3CMainView::OnInitialUpdate()
{
   CView::OnInitialUpdate();

   // TODO: Add your specialized code here and/or call the base class

   if (!m_SplitterWnd.CreateStatic(this, 2, 2))
   {
      return;
   }

   CRect rect;
   GetClientRect(&rect);

   CCreateContext context;
   context.m_pNewViewClass = RUNTIME_CLASS(CM3CMainView);
   context.m_pCurrentDoc = GetDocument();
   context.m_pCurrentFrame = nullptr;
   context.m_pLastView = nullptr;
   context.m_pNewDocTemplate = nullptr;

   if (!m_SplitterWnd.CreateView(0, 0, RUNTIME_CLASS(CM3CSectionView), CSize(rect.Width() / 2, rect.Height()/ 2), &context))
   {
      return;
   }

   if (!m_SplitterWnd.CreateView(0, 1, RUNTIME_CLASS(CM3CConcreteGraphView), CSize(rect.Width() / 2, rect.Height() / 2), &context))
   {
      return;
   }

   if (!m_SplitterWnd.CreateView(1, 0, RUNTIME_CLASS(CM3CRebarGraphView), CSize(rect.Width() / 2, rect.Height() / 2), &context))
   {
      return;
   }

   if (!m_SplitterWnd.CreateView(1, 1, RUNTIME_CLASS(CM3CStrandGraphView), CSize(rect.Width() / 2, rect.Height() / 2), &context))
   {
      return;
   }
}

CView* CM3CMainView::GetTopLeftView()
{
   return (CView*)m_SplitterWnd.GetPane(0, 0);
}

CView* CM3CMainView::GetTopRightView()
{
   return (CView*)m_SplitterWnd.GetPane(0, 1);
}

CView* CM3CMainView::GetBottomLeftView()
{
   return (CView*)m_SplitterWnd.GetPane(1, 0);
}

CView* CM3CMainView::GetBottomRightView()
{
   return (CView*)m_SplitterWnd.GetPane(1, 1);
}
