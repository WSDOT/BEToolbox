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

// SpectraDoc.cpp : implementation file
//

#include "stdafx.h"
#include "..\resource.h"
#include "SpectraDoc.h"
#include "SpectraTitlePageBuilder.h"
#include "SpectraChapterBuilder.h"
#include "SpectraChildFrame.h"
#include "..\BEToolboxStatusBar.h"

#include <EAF\EAFUtilities.h>
#include <EAF\EAFApp.h>

#include <WBFLGeometry.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CSpectraDoc

IMPLEMENT_DYNCREATE(CSpectraDoc, CBEToolboxDoc)

CSpectraDoc::CSpectraDoc()
{
   std::unique_ptr<CReportBuilder> pRptBuilder(std::make_unique<CReportBuilder>(_T("Spectra")));

   std::shared_ptr<CTitlePageBuilder> pTitlePageBuilder(std::make_shared<CSpectraTitlePageBuilder>());
   pRptBuilder->AddTitlePageBuilder( pTitlePageBuilder );

   std::shared_ptr<CChapterBuilder> pChBuilder(std::make_shared<CSpectraChapterBuilder>(this) );
   pRptBuilder->AddChapterBuilder(pChBuilder);

   m_RptMgr.AddReportBuilder(pRptBuilder.release());

   UIHints(FALSE); // not using UIHints feature


   // Lat/Lng of the Bridge Office, 7345 Linderson Way SW, Tumwater, WA 98501
   m_SpecType = WSDOT_BDM;
   m_Lat = 46.981394;
   m_Lng = -122.91888;
   m_SiteClass = scB;

   m_pValues = nullptr;
}

CSpectraDoc::~CSpectraDoc()
{
}

CString CSpectraDoc::GetToolbarSectionName()
{
   return _T("Spectra");
}

BEGIN_MESSAGE_MAP(CSpectraDoc, CBEToolboxDoc)
   ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
END_MESSAGE_MAP()

void CSpectraDoc::OnHelpFinder()
{
   EAFHelp(EAFGetDocument()->GetDocumentationSetName(),IDH_SPECTRA);
}


// CSpectraDoc diagnostics

#ifdef _DEBUG
void CSpectraDoc::AssertValid() const
{
	CBEToolboxDoc::AssertValid();
}

#ifndef _WIN32_WCE
void CSpectraDoc::Dump(CDumpContext& dc) const
{
	CBEToolboxDoc::Dump(dc);
}
#endif
#endif //_DEBUG

BOOL CSpectraDoc::Init()
{
   if ( !CBEToolboxDoc::Init() )
      return FALSE;

   if ( !LoadSpectralData() )
      return FALSE;

   InitWSDOT();
   InitLRFD();
   InitGuideSpec();

   return TRUE;
}

void CSpectraDoc::InitWSDOT()
{
   // Zero Period Site Factors (Fpga) - BDM Table 3.4.2.3-1A
   {
      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteA(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteA->AddPoint(0.1, 0.8);
      // all points are the same so we don't actually need to add them
      //siteA->AddPoint(0.2,0.8);
      //siteA->AddPoint(0.3,0.8);
      //siteA->AddPoint(0.4,0.8);
      //siteA->AddPoint(0.5,0.8);
      //siteA->AddPoint(0.6,0.8);
      m_ZeroPeriodSiteFactors[WSDOT_BDM].push_back(siteA);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteB(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteB->AddPoint(0.1, 0.9);
      // all points are the same so we don't actually need to add them
      //siteB->AddPoint(0.2,0.9);
      //siteB->AddPoint(0.3,0.9);
      //siteB->AddPoint(0.4,0.9);
      //siteB->AddPoint(0.5,0.9);
      //siteB->AddPoint(0.6,0.9);
      m_ZeroPeriodSiteFactors[WSDOT_BDM].push_back(siteB);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteC(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteC->AddPoint(0.1, 1.3);
      siteC->AddPoint(0.2, 1.2);
      siteC->AddPoint(0.3, 1.2);
      siteC->AddPoint(0.4, 1.2);
      siteC->AddPoint(0.5, 1.2);
      siteC->AddPoint(0.6, 1.2);
      m_ZeroPeriodSiteFactors[WSDOT_BDM].push_back(siteC);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteD(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteD->AddPoint(0.1, 1.6);
      siteD->AddPoint(0.2, 1.4);
      siteD->AddPoint(0.3, 1.3);
      siteD->AddPoint(0.4, 1.2);
      siteD->AddPoint(0.5, 1.1);
      siteD->AddPoint(0.6, 1.1);
      m_ZeroPeriodSiteFactors[WSDOT_BDM].push_back(siteD);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteE(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteE->AddPoint(0.1, 2.4);
      siteE->AddPoint(0.2, 1.9);
      siteE->AddPoint(0.3, 1.6);
      siteE->AddPoint(0.4, 1.4);
      siteE->AddPoint(0.5, 1.2);
      siteE->AddPoint(0.6, 1.1);
      m_ZeroPeriodSiteFactors[WSDOT_BDM].push_back(siteE);
   }

   {
      // Short Period Site Factors (Fa for 0.2 sec period spectral acceleration) - BDM Table 3.4.2.3-1B
      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteA(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteA->AddPoint(0.25, 0.8);
      // all points are the same so we don't actually need to add them
      //siteA->AddPoint(0.50,0.8);
      //siteA->AddPoint(0.75,0.8);
      //siteA->AddPoint(1.00,0.8);
      //siteA->AddPoint(1.25,0.8);
      //siteA->AddPoint(1.50,0.8);
      m_ShortPeriodSiteFactors[WSDOT_BDM].push_back(siteA);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteB(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteB->AddPoint(0.25, 0.9);
      // all points are the same so we don't actually need to add them
      //siteB->AddPoint(0.50,0.9);
      //siteB->AddPoint(0.75,0.9);
      //siteB->AddPoint(0.90,0.9);
      //siteB->AddPoint(1.25,0.9);
      //siteB->AddPoint(1.50,0.9);
      m_ShortPeriodSiteFactors[WSDOT_BDM].push_back(siteB);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteC(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteC->AddPoint(0.25, 1.3);
      siteC->AddPoint(0.50, 1.3);
      siteC->AddPoint(0.75, 1.2);
      siteC->AddPoint(1.00, 1.2);
      siteC->AddPoint(1.25, 1.2);
      siteC->AddPoint(1.50, 1.2);
      m_ShortPeriodSiteFactors[WSDOT_BDM].push_back(siteC);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteD(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteD->AddPoint(0.25, 1.6);
      siteD->AddPoint(0.50, 1.4);
      siteD->AddPoint(0.75, 1.2);
      siteD->AddPoint(1.00, 1.1);
      siteD->AddPoint(1.25, 1.0);
      siteD->AddPoint(1.50, 1.0);
      m_ShortPeriodSiteFactors[WSDOT_BDM].push_back(siteD);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteE(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteE->AddPoint(0.25, 2.4);
      siteE->AddPoint(0.50, 1.7);
      siteE->AddPoint(0.75, 1.3);
      siteE->AddPoint(1.00, 1.0);
      siteE->AddPoint(1.25, 0.9);
      siteE->AddPoint(1.50, 0.9);
      m_ShortPeriodSiteFactors[WSDOT_BDM].push_back(siteE);
   }

   {
      // Long Period Site Factors (Fv for 1.0 sec period spectral acceleration) - BDM Table 3.4.2.3.2
      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteA(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteA->AddPoint(0.1, 0.8);
      // all points are the same so we don't actually need to add them
      //siteA->AddPoint(0.2,0.8);
      //siteA->AddPoint(0.3,0.8);
      //siteA->AddPoint(0.4,0.8);
      //siteA->AddPoint(0.5,0.8);
      //siteA->AddPoint(0.6,0.8);
      m_LongPeriodSiteFactors[WSDOT_BDM].push_back(siteA);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteB(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteB->AddPoint(0.1, 0.8);
      // all points are the same so we don't actually need to add them
      //siteB->AddPoint(0.2,0.8);
      //siteB->AddPoint(0.3,0.8);
      //siteB->AddPoint(0.4,0.8);
      //siteB->AddPoint(0.5,0.8);
      //siteB->AddPoint(0.6,0.8);
      m_LongPeriodSiteFactors[WSDOT_BDM].push_back(siteB);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteC(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteC->AddPoint(0.1, 1.5);
      //siteC->AddPoint(0.2,1.5);
      //siteC->AddPoint(0.3,1.5);
      //siteC->AddPoint(0.4,1.5);
      siteC->AddPoint(0.5, 1.5);
      siteC->AddPoint(0.6, 1.4);
      m_LongPeriodSiteFactors[WSDOT_BDM].push_back(siteC);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteD(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteD->AddPoint(0.1, 2.4);
      siteD->AddPoint(0.2, 2.2);
      siteD->AddPoint(0.3, 2.0);
      siteD->AddPoint(0.4, 1.9);
      siteD->AddPoint(0.5, 1.8);
      siteD->AddPoint(0.6, 1.7);
      m_LongPeriodSiteFactors[WSDOT_BDM].push_back(siteD);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteE(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteE->AddPoint(0.1, 4.2);
      siteE->AddPoint(0.2, 3.3);
      siteE->AddPoint(0.3, 2.8);
      siteE->AddPoint(0.4, 2.4);
      siteE->AddPoint(0.5, 2.2);
      siteE->AddPoint(0.6, 2.0);
      m_LongPeriodSiteFactors[WSDOT_BDM].push_back(siteE);
   }
}

void CSpectraDoc::InitLRFD()
{
   // Zero Period Site Factors (Fpga) - Table 3.4.2.3-1
   {
      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteA(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteA->AddPoint(0.1, 0.8);
      // all points are the same so we don't actually need to add them
      //siteA->AddPoint(0.2,0.8);
      //siteA->AddPoint(0.3,0.8);
      //siteA->AddPoint(0.4,0.8);
      //siteA->AddPoint(0.5,0.8);
      m_ZeroPeriodSiteFactors[AASHTO_LRFD].push_back(siteA);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteB(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteB->AddPoint(0.1, 1.0);
      // all points are the same so we don't actually need to add them
      //siteB->AddPoint(0.2,1.0);
      //siteB->AddPoint(0.3,1.0);
      //siteB->AddPoint(0.4,1.0);
      //siteB->AddPoint(0.5,1.0);
      m_ZeroPeriodSiteFactors[AASHTO_LRFD].push_back(siteB);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteC(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteC->AddPoint(0.1, 1.2);
      siteC->AddPoint(0.2, 1.2);
      siteC->AddPoint(0.3, 1.1);
      siteC->AddPoint(0.4, 1.0);
      siteC->AddPoint(0.5, 1.0);
      m_ZeroPeriodSiteFactors[AASHTO_LRFD].push_back(siteC);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteD(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteD->AddPoint(0.1, 1.6);
      siteD->AddPoint(0.2, 1.4);
      siteD->AddPoint(0.3, 1.2);
      siteD->AddPoint(0.4, 1.1);
      siteD->AddPoint(0.5, 1.0);
      m_ZeroPeriodSiteFactors[AASHTO_LRFD].push_back(siteD);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteE(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteE->AddPoint(0.1, 2.5);
      siteE->AddPoint(0.2, 1.7);
      siteE->AddPoint(0.3, 1.2);
      siteE->AddPoint(0.4, 0.9);
      siteE->AddPoint(0.5, 0.9);
      m_ZeroPeriodSiteFactors[AASHTO_LRFD].push_back(siteE);
   }

   {
      // Short Period Site Factors (Fa for 0.2 sec period spectral acceleration) - Table 3.4.2.3-1
      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteA(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteA->AddPoint(0.25, 0.8);
      // all points are the same so we don't actually need to add them
      //siteA->AddPoint(0.50,0.8);
      //siteA->AddPoint(0.75,0.8);
      //siteA->AddPoint(1.00,0.8);
      //siteA->AddPoint(1.25,0.8);
      m_ShortPeriodSiteFactors[AASHTO_LRFD].push_back(siteA);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteB(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteB->AddPoint(0.25, 1.0);
      // all points are the same so we don't actually need to add them
      //siteB->AddPoint(0.50,1.0);
      //siteB->AddPoint(0.75,1.0);
      //siteB->AddPoint(0.90,1.0);
      //siteB->AddPoint(1.25,1.0);
      m_ShortPeriodSiteFactors[AASHTO_LRFD].push_back(siteB);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteC(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteC->AddPoint(0.25, 1.2);
      siteC->AddPoint(0.50, 1.2);
      siteC->AddPoint(0.75, 1.1);
      siteC->AddPoint(1.00, 1.0);
      siteC->AddPoint(1.25, 1.0);
      m_ShortPeriodSiteFactors[AASHTO_LRFD].push_back(siteC);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteD(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteD->AddPoint(0.25, 1.6);
      siteD->AddPoint(0.50, 1.4);
      siteD->AddPoint(0.75, 1.2);
      siteD->AddPoint(1.00, 1.1);
      siteD->AddPoint(1.25, 1.0);
      m_ShortPeriodSiteFactors[AASHTO_LRFD].push_back(siteD);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteE(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteE->AddPoint(0.25, 2.5);
      siteE->AddPoint(0.50, 1.7);
      siteE->AddPoint(0.75, 1.2);
      siteE->AddPoint(1.00, 0.9);
      siteE->AddPoint(1.25, 0.9);
      m_ShortPeriodSiteFactors[AASHTO_LRFD].push_back(siteE);
   }

   {
      // Long Period Site Factors (Fv for 1.0 sec period spectral acceleration) - Table 3.10.3.2-3
      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteA(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteA->AddPoint(0.1, 0.8);
      // all points are the same so we don't actually need to add them
      //siteA->AddPoint(0.2,0.8);
      //siteA->AddPoint(0.3,0.8);
      //siteA->AddPoint(0.4,0.8);
      //siteA->AddPoint(0.5,0.8);
      m_LongPeriodSiteFactors[AASHTO_LRFD].push_back(siteA);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteB(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteB->AddPoint(0.1, 1.0);
      // all points are the same so we don't actually need to add them
      //siteB->AddPoint(0.2,1.0;
      //siteB->AddPoint(0.3,1.0);
      //siteB->AddPoint(0.4,1.0);
      //siteB->AddPoint(0.5,1.0);
      m_LongPeriodSiteFactors[AASHTO_LRFD].push_back(siteB);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteC(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteC->AddPoint(0.1,1.7);
      siteC->AddPoint(0.2,1.6);
      siteC->AddPoint(0.3,1.5);
      siteC->AddPoint(0.4,1.4);
      siteC->AddPoint(0.5,1.3);
      m_LongPeriodSiteFactors[AASHTO_LRFD].push_back(siteC);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteD(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteD->AddPoint(0.1, 2.4);
      siteD->AddPoint(0.2, 2.0);
      siteD->AddPoint(0.3, 1.8);
      siteD->AddPoint(0.4, 1.6);
      siteD->AddPoint(0.5, 1.5);
      m_LongPeriodSiteFactors[AASHTO_LRFD].push_back(siteD);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteE(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteE->AddPoint(0.1, 3.5);
      siteE->AddPoint(0.2, 3.2);
      siteE->AddPoint(0.3, 2.8);
      siteE->AddPoint(0.4, 2.4);
      siteE->AddPoint(0.5, 2.4);
      m_LongPeriodSiteFactors[AASHTO_LRFD].push_back(siteE);
   }
}

void CSpectraDoc::InitGuideSpec()
{
   // Zero Period Site Factors (Fpga) - Table 3.4.2.3-1
   {
      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteA(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteA->AddPoint(0.1, 0.8);
      // all points are the same so we don't actually need to add them
      //siteA->AddPoint(0.2,0.8);
      //siteA->AddPoint(0.3,0.8);
      //siteA->AddPoint(0.4,0.8);
      //siteA->AddPoint(0.5,0.8);
      m_ZeroPeriodSiteFactors[AASHTO_SEISMIC].push_back(siteA);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteB(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteB->AddPoint(0.1, 1.0);
      // all points are the same so we don't actually need to add them
      //siteB->AddPoint(0.2,1.0);
      //siteB->AddPoint(0.3,1.0);
      //siteB->AddPoint(0.4,1.0);
      //siteB->AddPoint(0.5,1.0);
      m_ZeroPeriodSiteFactors[AASHTO_SEISMIC].push_back(siteB);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteC(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteC->AddPoint(0.1, 1.2);
      siteC->AddPoint(0.2, 1.2);
      siteC->AddPoint(0.3, 1.1);
      siteC->AddPoint(0.4, 1.0);
      siteC->AddPoint(0.5, 1.0);
      m_ZeroPeriodSiteFactors[AASHTO_SEISMIC].push_back(siteC);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteD(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteD->AddPoint(0.1, 1.6);
      siteD->AddPoint(0.2, 1.4);
      siteD->AddPoint(0.3, 1.2);
      siteD->AddPoint(0.4, 1.1);
      siteD->AddPoint(0.5, 1.0);
      m_ZeroPeriodSiteFactors[AASHTO_SEISMIC].push_back(siteD);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteE(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteE->AddPoint(0.1, 2.5);
      siteE->AddPoint(0.2, 1.7);
      siteE->AddPoint(0.3, 1.2);
      siteE->AddPoint(0.4, 0.9);
      siteE->AddPoint(0.5, 0.9);
      m_ZeroPeriodSiteFactors[AASHTO_SEISMIC].push_back(siteE);
   }

   {
      // Short Period Site Factors (Fa for 0.2 sec period spectral acceleration) - Table 3.4.2.3-1
      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteA(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteA->AddPoint(0.25, 0.8);
      // all points are the same so we don't actually need to add them
      //siteA->AddPoint(0.50,0.8);
      //siteA->AddPoint(0.75,0.8);
      //siteA->AddPoint(1.00,0.8);
      //siteA->AddPoint(1.25,0.8);
      m_ShortPeriodSiteFactors[AASHTO_SEISMIC].push_back(siteA);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteB(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteB->AddPoint(0.25, 1.0);
      // all points are the same so we don't actually need to add them
      //siteB->AddPoint(0.50,1.0);
      //siteB->AddPoint(0.75,1.0);
      //siteB->AddPoint(0.90,1.0);
      //siteB->AddPoint(1.25,1.0);
      m_ShortPeriodSiteFactors[AASHTO_SEISMIC].push_back(siteB);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteC(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteC->AddPoint(0.25, 1.2);
      siteC->AddPoint(0.50, 1.2);
      siteC->AddPoint(0.75, 1.1);
      siteC->AddPoint(1.00, 1.0);
      siteC->AddPoint(1.25, 1.0);
      m_ShortPeriodSiteFactors[AASHTO_SEISMIC].push_back(siteC);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteD(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteD->AddPoint(0.25, 1.6);
      siteD->AddPoint(0.50, 1.4);
      siteD->AddPoint(0.75, 1.2);
      siteD->AddPoint(1.00, 1.1);
      siteD->AddPoint(1.25, 1.0);
      m_ShortPeriodSiteFactors[AASHTO_SEISMIC].push_back(siteD);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteE(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteE->AddPoint(0.25, 2.5);
      siteE->AddPoint(0.50, 1.7);
      siteE->AddPoint(0.75, 1.2);
      siteE->AddPoint(1.00, 0.9);
      siteE->AddPoint(1.25, 0.9);
      m_ShortPeriodSiteFactors[AASHTO_SEISMIC].push_back(siteE);
   }

   {
      // Long Period Site Factors (Fv for 1.0 sec period spectral acceleration) - Table 3.4.2.3-2
      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteA(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteA->AddPoint(0.1, 0.8);
      // all points are the same so we don't actually need to add them
      //siteA->AddPoint(0.2,0.8);
      //siteA->AddPoint(0.3,0.8);
      //siteA->AddPoint(0.4,0.8);
      //siteA->AddPoint(0.5,0.8);
      m_LongPeriodSiteFactors[AASHTO_SEISMIC].push_back(siteA);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteB(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteB->AddPoint(0.1, 1.0);
      // all points are the same so we don't actually need to add them
      //siteB->AddPoint(0.2,1.0;
      //siteB->AddPoint(0.3,1.0);
      //siteB->AddPoint(0.4,1.0);
      //siteB->AddPoint(0.5,1.0);
      m_LongPeriodSiteFactors[AASHTO_SEISMIC].push_back(siteB);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteC(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteC->AddPoint(0.1, 1.7);
      siteC->AddPoint(0.2, 1.6);
      siteC->AddPoint(0.3, 1.5);
      siteC->AddPoint(0.4, 1.4);
      siteC->AddPoint(0.5, 1.3);
      m_LongPeriodSiteFactors[AASHTO_SEISMIC].push_back(siteC);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteD(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteD->AddPoint(0.1, 2.4);
      siteD->AddPoint(0.2, 2.0);
      siteD->AddPoint(0.3, 1.8);
      siteD->AddPoint(0.4, 1.6);
      siteD->AddPoint(0.5, 1.5);
      m_LongPeriodSiteFactors[AASHTO_SEISMIC].push_back(siteD);

      std::shared_ptr<mathPwLinearFunction2dUsingPoints> siteE(std::make_shared<mathPwLinearFunction2dUsingPoints>());
      siteE->AddPoint(0.1, 3.5);
      siteE->AddPoint(0.2, 3.2);
      siteE->AddPoint(0.3, 2.8);
      siteE->AddPoint(0.4, 2.4);
      siteE->AddPoint(0.5, 2.4);
      m_LongPeriodSiteFactors[AASHTO_SEISMIC].push_back(siteE);
   }
}

void CSpectraDoc::OnCloseDocument()
{
   EAFGetApp()->SetUnitsMode(eafTypes::umUS);
   
   delete[] m_pValues;
   m_pValues = nullptr;

   CBEToolboxDoc::OnCloseDocument();
}

HRESULT CSpectraDoc::WriteTheDocument(IStructuredSave* pStrSave)
{
   HRESULT hr = pStrSave->BeginUnit(_T("Spectra"),2.0);
   if ( FAILED(hr) )
      return hr;

   Float64 lat,lng;
   GetLocation(&lat,&lng);
   SiteClass siteClass = GetSiteClass();
   SpecificationType specType = GetSpecification();

   // added in version 2
   hr = pStrSave->put_Property(_T("Specification"), CComVariant(specType));
   if (FAILED(hr))
      return hr;

   hr = pStrSave->put_Property(_T("Latitude"),CComVariant(lat));
   if ( FAILED(hr) )
      return hr;

   hr = pStrSave->put_Property(_T("Longitude"),CComVariant(lng));
   if ( FAILED(hr) )
      return hr;

   hr = pStrSave->put_Property(_T("SiteClass"),CComVariant(siteClass));
   if ( FAILED(hr) )
      return hr;

   hr = pStrSave->EndUnit(); // Spectra
   if ( FAILED(hr) )
      return hr;

   return S_OK;
}

HRESULT CSpectraDoc::LoadTheDocument(IStructuredLoad* pStrLoad)
{
   HRESULT hr = pStrLoad->BeginUnit(_T("Spectra"));
   if ( FAILED(hr) )
      return hr;

   Float64 version;
   pStrLoad->get_Version(&version);

   CComVariant var;

   if (1 < version)
   {
      // added in version 2
      var.vt = VT_I4;
      hr = pStrLoad->get_Property(_T("Specification"), &var);
      if (FAILED(hr))
         return hr;
      m_SpecType = (SpecificationType)(var.lVal);
   }

   var.vt = VT_R8;
   hr = pStrLoad->get_Property(_T("Latitude"),&var);
   if ( FAILED(hr) )
      return hr;
   m_Lat = var.dblVal;

   hr = pStrLoad->get_Property(_T("Longitude"),&var);
   if ( FAILED(hr) )
      return hr;
   m_Lng = var.dblVal;

   var.vt = VT_UI4;
   hr = pStrLoad->get_Property(_T("SiteClass"),&var);
   if ( FAILED(hr) )
      return hr;

   m_SiteClass = (SiteClass)(var.uiVal);

   hr = pStrLoad->EndUnit(); // Spectra
   if ( FAILED(hr) )
      return hr;

   return S_OK;
}

void CSpectraDoc::LoadDocumentSettings()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   __super::LoadDocumentSettings();
}

void CSpectraDoc::SaveDocumentSettings()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   __super::SaveDocumentSettings();
}

CString CSpectraDoc::GetDocumentationRootLocation()
{
   CEAFApp* pApp = EAFGetApp();
   return pApp->GetDocumentationRootLocation();
}

CString CSpectraDoc::GetResourcePath()
{
   TCHAR szBuff[_MAX_PATH];
   ::GetModuleFileName(::GetModuleHandle(nullptr), szBuff, _MAX_PATH);
   CString filename(szBuff);
   filename.MakeUpper();

   CEAFApp* pApp = EAFGetApp();

   // find first Occurrence of the application name
   CString strAppName(pApp->m_pszAppName);
   strAppName.MakeUpper();
   int loc = filename.Find(strAppName + _T(".EXE"));
   if ( loc < 0 )
   {
      // something is wrong... that find should have succeeded
      // hard code the default install location so that there is a remote chance of success
      TCHAR szNativeProgramFilesFolder[MAX_PATH];
      ExpandEnvironmentStrings(_T("%ProgramW6432%"),szNativeProgramFilesFolder,ARRAYSIZE(szNativeProgramFilesFolder));
      filename.Format(_T("\\%s\\WSDOT\\%s"),szNativeProgramFilesFolder,strAppName);
      loc = filename.GetLength();
   }

   filename.Truncate(loc);
   return filename;
}

bool CSpectraDoc::LoadSpectralData()
{
   m_pValues = new SpectralValues[SPECTRAL_VALUE_COUNT];
   std::_tstring strResourceFile = GetResourcePath() + _T("us_hazard_7pc75.2014.bin");
   std::ifstream ifile(strResourceFile.c_str(),std::ios::binary);
   if ( ifile.bad() || ifile.fail() )
   {
      AFX_MANAGE_STATE(AfxGetStaticModuleState());
      AfxMessageBox(_T("Failed to load seismic hazard map data."),MB_OK | MB_ICONSTOP);
      return false;
   }
   ifile.read(reinterpret_cast<char*>(m_pValues),SPECTRAL_VALUE_COUNT*sizeof(SpectralValues));
   ifile.close();
   return true;
}

void CSpectraDoc::SetSpecification(SpecificationType specType)
{
   if (m_SpecType != specType)
   {
      m_SpecType = specType;
      UpdateAllViews(nullptr);
      SetModifiedFlag();
   }
}

SpecificationType CSpectraDoc::GetSpecification() const
{
   return m_SpecType;
}

void CSpectraDoc::SetLocation(Float64 lat,Float64 lng)
{
   if ( m_Lat != lat || m_Lng != lng )
   {
      m_Lat = lat;
      m_Lng = lng;
      UpdateAllViews(nullptr);
      SetModifiedFlag();
   }
}

void CSpectraDoc::GetLocation(Float64* pLat,Float64* pLng) const
{
   *pLat = m_Lat;
   *pLng = m_Lng;
}

void CSpectraDoc::SetSiteClass(SiteClass siteClass)
{
   if ( m_SiteClass != siteClass )
   {
      m_SiteClass = siteClass;
      UpdateAllViews(nullptr);
      SetModifiedFlag();
   }
}

SiteClass CSpectraDoc::GetSiteClass() const
{
   return m_SiteClass;
}

LPCTSTR CSpectraDoc::GetSpecificationDescription(SpecificationType specType) const
{
   switch (specType)
   {
   case WSDOT_BDM:
      return _T("WSDOT Bridge Design Manual");
   case AASHTO_LRFD:
      return _T("AASHTO LRFD Bridge Design Specifications, 8th Edition");
   case AASHTO_SEISMIC:
      return _T("AASHTO Guide Specifications for LRFD Seismic Bridge Design, 2nd Edition");
   default:
      ATLASSERT(false); // should never get here
      return _T("Unknown");
   }
}

LPCTSTR CSpectraDoc::GetSiteClassDescription(SiteClass siteClass) const
{
   switch(siteClass)
   {
   case scA:
      return _T("Hard Rock");

   case scB:
      return _T("Rock");

   case scC:
      return _T("Very Dense Soil and Soft Rock");

   case scD:
      return _T("Stiff Soil");

   case scE:
      return _T("Soft Clay Soil");

   case scF:
      return _T("Site Specific");

   default:
      return _T("Soft Clay Soil");
   }
}

LPCTSTR CSpectraDoc::GetSpectraResultExplaination(Uint32 result) const
{
   if ( result == SPECTRA_INVALID_LOCATION )
      return _T("Site Coordinates are invalid.");
   else if ( result == SPECTRA_SITE_SPECIFIC )
      return _T("Site Specific analysis is required.");
   else
      return _T("OK");
}

Uint32 CSpectraDoc::GetResponseSpectra(SpecificationType specType,Float64 lat,Float64 lng,SiteClass siteClass,CResponseSpectra* pSpectra) const
{
   if ( (lat < MIN_LATITUDE || MAX_LATITUDE < lat) || (lng < MIN_LONGITUDE || MAX_LONGITUDE < lng) )
      return SPECTRA_INVALID_LOCATION;

   Float64 S1, Ss, PGA;
   GetSpectralValues(lat,lng,&S1,&Ss,&PGA);

   Float64 Fa,Fv,Fpga;
   Uint32 result = GetSiteFactors(specType,S1,Ss,PGA,siteClass,&Fa,&Fv,&Fpga);
   if ( result != SPECTRA_OK )
      return result;

   pSpectra->Init(PGA,Ss,S1,Fpga,Fa,Fv);

   return SPECTRA_OK;
}

void CSpectraDoc::GetSpectralValues(Float64 lat,Float64 lng,Float64* pS1,Float64* pSs,Float64* pPGA) const
{
   ::GetSpectralValues(lat,lng,m_pValues,pS1,pSs,pPGA);
}

Uint32 CSpectraDoc::GetSiteFactors(SpecificationType specType, Float64 S1,Float64 Ss,Float64 PGA,SiteClass siteClass,Float64* pFa,Float64* pFv,Float64* pFpga) const
{
   if ( siteClass == scF && 0.75 < Ss )
      return SPECTRA_SITE_SPECIFIC;

   *pFpga = m_ZeroPeriodSiteFactors[specType][siteClass]->Evaluate(PGA);
   *pFa   = m_ShortPeriodSiteFactors[specType][siteClass]->Evaluate(Ss);
   *pFv   = m_LongPeriodSiteFactors[specType][siteClass]->Evaluate(S1);

   return SPECTRA_OK;
}

const mathPwLinearFunction2dUsingPoints* CSpectraDoc::GetZeroPeriodSiteFactors(SpecificationType specType, SiteClass siteClass) const
{
   return m_ZeroPeriodSiteFactors[specType][siteClass].get();
}

const mathPwLinearFunction2dUsingPoints* CSpectraDoc::GetShortPeriodSiteFactors(SpecificationType specType, SiteClass siteClass) const
{
   return m_ShortPeriodSiteFactors[specType][siteClass].get();
}

const mathPwLinearFunction2dUsingPoints* CSpectraDoc::GetLongPeriodSiteFactors(SpecificationType specType, SiteClass siteClass) const
{
   return m_LongPeriodSiteFactors[specType][siteClass].get();
}
