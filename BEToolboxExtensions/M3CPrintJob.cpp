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

#include "stdafx.h"
#include "M3CPrintJob.h"
#include "M3CTabView.h"
#include "M3CMainView.h"
#include "M3CSectionView.h"
#include "M3CGraphView.h"
#include <EAF\EAFUtilities.h>
#include <EAF\EAFDocument.h>
#include <MFCTools\WsdotCalculationSheet.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


CM3CPrintJob::CM3CPrintJob(CM3CChildFrame* pframe)
{
   m_pFrame = pframe;

   m_rcMarginMM = CRect(10, 10, 10, 10);

   m_csFtPrint = "Arial";
   m_iFtPrint = 120;
}

CM3CPrintJob::~CM3CPrintJob()
{
}

bool CM3CPrintJob::OnPreparePrinting(CPrintInfo* pInfo, bool bPrintPreview)
{
   pInfo->SetMinPage(1);
   pInfo->SetMaxPage(1);

   return CPrinterJob::OnPreparePrinting(pInfo, bPrintPreview);
}

void CM3CPrintJob::OnBeginPrinting(CDC * pDC, CPrintInfo * pInfo)
{
}

void CM3CPrintJob::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{

}

void CM3CPrintJob::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
   int obkm = pDC->SetBkMode(TRANSPARENT);

   CEAFDocument* pDoc = EAFGetDocument();

   // print calc sheet border
   WsdotCalculationSheet border;
   border.SetTitle(_T("M3C - Moment Curvature of Circular Columns"));

   CString path = pDoc->GetPathName();
   border.SetFileName(path);

   CRect rcPrint = border.Print(pDC, 1);
   if (rcPrint.IsRectEmpty())
   {
      CHECKX(0, _T("Can't print border - page too small?"));
      rcPrint = pInfo->m_rectDraw;
   }

   CRect tl(rcPrint.TopLeft(), CSize(rcPrint.Width() / 2, rcPrint.Height() / 2));
   CRect tr(CPoint(tl.right,tl.top), CSize(rcPrint.Width() / 2, rcPrint.Height() / 2));
   CRect bl(CPoint(tl.left,tl.bottom), CSize(rcPrint.Width() / 2, rcPrint.Height() / 2));
   CRect br(CPoint(bl.right,bl.top), CSize(rcPrint.Width() / 2, rcPrint.Height() / 2));

   // want to offset picture away from borders - get device units for 10mm
   int oldmode = pDC->SetMapMode(MM_LOMETRIC);
   POINT offset[2] = { { 0,0 },{ 100,-100 } };
   pDC->LPtoDP(offset, 2);
   pDC->SetMapMode(oldmode);

   int offsetX = offset[1].x - offset[0].x;
   int offsetY = offset[1].y - offset[0].y;
   CSize szOffset(offsetX, offsetY);

   tl.left += offsetX;
   tl.right -= offsetX / 2;
   tl.top += offsetY;
   tl.bottom -= offsetY / 2;
   
   tr.left += offsetX / 2;
   tr.right -= offsetX;
   tr.top += offsetY;
   tr.bottom -= offsetY / 2;

   bl.left += offsetX;
   bl.right -= offsetX / 2;
   bl.top += offsetY / 2;
   bl.bottom -= offsetY;

   br.left += offsetX / 2;
   br.right -= offsetX;
   br.top += offsetY / 2;
   br.bottom -= offsetY;

   CWnd* pWnd = m_pFrame->GetWindow(GW_CHILD);
   ASSERT_KINDOF(CM3CTabView, pWnd);
   CM3CTabView* pTabView = (CM3CTabView*)pWnd;

   CM3CMainView* pMainView = pTabView->GetMainView();

   CM3CSectionView* pwndTL = (CM3CSectionView*)pMainView->GetTopLeftView();
   CM3CGraphView* pwndTR = (CM3CGraphView*)pMainView->GetTopRightView();
   CM3CGraphView* pwndBL = (CM3CGraphView*)pMainView->GetBottomLeftView();
   CM3CGraphView* pwndBR = (CM3CGraphView*)pMainView->GetBottomRightView();

   pDC->SetMapMode(MM_TEXT);

   CRect orig_clip;
   pDC->GetClipBox(&orig_clip);

   pInfo->m_rectDraw = tl;
   pwndTL->DoPrint(pDC, pInfo);

   CRgn old_clip;
   old_clip.CreateRectRgnIndirect(&orig_clip);
   pDC->SelectClipRgn(&old_clip);

   pInfo->m_rectDraw = tr;
   pwndTR->DoPrint(pDC, pInfo);

   pInfo->m_rectDraw = bl;
   pwndBL->DoPrint(pDC, pInfo);

   pInfo->m_rectDraw = br;
   pwndBR->DoPrint(pDC, pInfo);
}


