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

// M3CView.cpp : implementation file
//

#include "stdafx.h"
#include "M3CDoc.h"
#include "M3CReportView.h"
#include "M3CChildFrame.h"
#include "M3CAnalysisDetailsReportSpecification.h"
#include <MFCTools\Format.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CM3CReportView

IMPLEMENT_DYNAMIC(CM3CReportView, CEAFReportView)
IMPLEMENT_DYNCREATE(CM3CAnalysisReportView, CM3CReportView)
IMPLEMENT_DYNCREATE(CM3CMaterialDetailsReportView, CM3CReportView)
IMPLEMENT_DYNCREATE(CM3CAnalysisDetailsReportView, CM3CReportView)

BEGIN_MESSAGE_MAP(CM3CAnalysisReportView, CM3CReportView)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CM3CMaterialDetailsReportView, CM3CReportView)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CM3CAnalysisDetailsReportView, CM3CReportView)
END_MESSAGE_MAP()

CM3CReportView::CM3CReportView()
{

}

CM3CReportView::~CM3CReportView()
{
}

BEGIN_MESSAGE_MAP(CM3CReportView, CEAFReportView)
   ON_COMMAND(ID_FILE_PRINT_DIRECT,&CM3CReportView::OnFilePrint)
END_MESSAGE_MAP()

// CM3CReportView diagnostics

#ifdef _DEBUG
void CM3CReportView::AssertValid() const
{
	CEAFReportView::AssertValid();
}

#ifndef _WIN32_WCE
void CM3CReportView::Dump(CDumpContext& dc) const
{
	CEAFReportView::Dump(dc);
}
#endif
#endif //_DEBUG


// CM3CReportView message handlers

void CM3CReportView::OnInitialUpdate()
{
   CM3CDoc* pDoc = (CM3CDoc*)GetDocument();

   auto names = pDoc->m_RptMgr.GetReportNames();
   std::_tstring strName = GetReportName();
   IndexType idx = 0;
   for (const auto& name : names)
   {
      if (strName == name)
      {
         break;
      }
      idx++;
   }

   CEAFReportViewCreationData data;
   data.m_RptIdx = idx;
   data.m_bPromptForSpec = false;
   data.m_pReportBuilderMgr = &pDoc->m_RptMgr;
   CEAFDocTemplate* pDocTemplate = (CEAFDocTemplate*)pDoc->GetDocTemplate();
   pDocTemplate->SetViewCreationData((void*)&data);

   CEAFReportView::OnInitialUpdate();
}

////////////////////////////////////////////////////////////
void CM3CAnalysisDetailsReportView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
   UpdateCurvatureList();

   CCurvatureList* pCB = (CCurvatureList*)m_pwndEdit;
   if (m_pwndEdit == nullptr)
      return;

   int curSel = pCB->GetCurSel();
   CurvatureChanged((IndexType)curSel);

   __super::OnUpdate(pSender, lHint, pHint);
}

void CM3CAnalysisDetailsReportView::UpdateCurvatureList()
{
   CCurvatureList* pCB = (CCurvatureList*)m_pwndEdit;
   if (m_pwndEdit == nullptr)
      return;

   int curSel = pCB->GetCurSel();

   pCB->ResetContent();

   CM3CDoc* pDoc = (CM3CDoc*)GetDocument();

   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDisplayUnits = pApp->GetDisplayUnits();

   CComPtr<IMomentCurvatureSolution> curvature_solution;
   pDoc->GetMomentCurvature(&curvature_solution);
   IndexType nPoints;
   curvature_solution->get_PointCount(&nPoints);

   for (IndexType pntIdx = 0; pntIdx < nPoints; pntIdx++)
   {
      Float64 k;
      curvature_solution->get_Curvature(pntIdx, &k);
      CString strCurvature;
      strCurvature.Format(_T("%s"), FormatDimension(k, pDisplayUnits->Curvature));

      pCB->AddString(strCurvature);
   }

   if (pCB->SetCurSel(curSel) == CB_ERR)
   {
      pCB->SetCurSel(0);
   }
}

CWnd* CM3CAnalysisDetailsReportView::CreateEditButton()
{
   CWnd* pWeb = m_pReportBrowser->GetBrowserWnd();

   CRect rect(0, 0, 200, 21);
   CCurvatureList* pCB = new CCurvatureList();
   pCB->Register(this);
   pCB->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, rect, pWeb, 1);

   UpdateCurvatureList();
   pCB->SetFont(&m_fnEdit);

   return pCB;
}

void CM3CAnalysisDetailsReportView::CurvatureChanged(IndexType idx)
{
   m_pReportSpec = m_pReportBrowser->GetReportSpecification();
   std::shared_ptr<CM3CAnalysisDetailsReportSpecification> pSpec = std::dynamic_pointer_cast<CM3CAnalysisDetailsReportSpecification>(m_pReportSpec);
   pSpec->SetResultsIndex(idx);
   RefreshReport();
}

///////////////////////////////
IMPLEMENT_DYNAMIC(CCurvatureList,CComboBox);
BEGIN_MESSAGE_MAP(CCurvatureList,CComboBox)
   ON_CONTROL_REFLECT(CBN_SELCHANGE,OnSelChange)
END_MESSAGE_MAP()

CCurvatureList::CCurvatureList()
{
   m_pListener = nullptr;
}

CCurvatureList::~CCurvatureList()
{
}

void CCurvatureList::Register(CM3CAnalysisDetailsReportView* pView)
{
   m_pListener = pView;
}

void CCurvatureList::OnSelChange()
{
   int curSel = GetCurSel();
   m_pListener->CurvatureChanged((IndexType)curSel);
};
