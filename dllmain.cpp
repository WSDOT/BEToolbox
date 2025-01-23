///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2025  Washington State Department of Transportation
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

// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
#include "BEToolboxApp.h"
#include "BEToolbox_i.h"
#include "dllmain.h"

#include <Reporter\Reporter.h>

#include <objbase.h>
#include <initguid.h>
#include <WBFLTools.h>
#include <WBFLUnitServer.h>
#include <WBFLGeometry.h>
#include <WBFLGeometry_i.c>
#include <EAF\EAFComponentInfo.h>
#include <WBFLCogo.h>
#include <WBFLCogo_i.c>

#include <WBFLUnitServer_i.c>

#include ".\PGStable\PGStablePluginCATID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBEToolboxModule _AtlModule;

BEGIN_MESSAGE_MAP(CBEToolboxApp, CWinApp)
END_MESSAGE_MAP()

CBEToolboxApp theApp;

BOOL CBEToolboxApp::InitInstance()
{
   GXInit();

   // Store registry entries in HKCU\Software\Washington State Department of Transportation\BEToolbox
   SetRegistryKey( _T("Washington State Department of Transportation") );
   free((void*)m_pszProfileName);
   m_pszProfileName = _tcsdup(_T("BEToolbox"));

   rptStyleManager::InitStyles(); // initialize the default report styles

   
   // add an addition style called "Normal"
   rptFontStyleLibrary* pStyleLib = rptFontStyleLibrary::Instance();

   rptRiStyle normal;
   normal.SetBold( false );
   normal.SetFontSize(9);
   normal.SetFontType(rptRiStyle::SWISS);
   normal.SetVerticalAlignment( rptRiStyle::TOP );
   normal.SetAlignment(rptRiStyle::LEFT);
   pStyleLib->AddNamedStyle(_T("Normal"), normal);

   // We don't want the title pages to show up, so we will make them Print only in style
   rptRiStyle reportTitle = pStyleLib->GetNamedStyle(rptStyleManager::GetReportTitleStyle());
   reportTitle.SetMediaType(rptRiStyle::Print);
   pStyleLib->AddNamedStyle(_T("BEToolboxReportTitle"),reportTitle);

   rptRiStyle reportSubtitle = pStyleLib->GetNamedStyle(rptStyleManager::GetReportSubtitleStyle());
   reportSubtitle.SetMediaType(rptRiStyle::Print);
   pStyleLib->AddNamedStyle(_T("BEToolboxReportSubtitle"),reportSubtitle);

   rptRiStyle copyright = pStyleLib->GetNamedStyle(rptStyleManager::GetCopyrightStyle());
   copyright.SetMediaType(rptRiStyle::Print);
   pStyleLib->AddNamedStyle(_T("BEToolboxCopyright"),copyright);

   rptRiStyle printOnly;
   printOnly.SetBold( false );
   printOnly.SetFontSize(9);
   printOnly.SetFontType(rptRiStyle::SWISS);
   printOnly.SetVerticalAlignment( rptRiStyle::TOP );
   printOnly.SetAlignment(rptRiStyle::LEFT);
   printOnly.SetMediaType(rptRiStyle::Print);
   pStyleLib->AddNamedStyle(_T("BEToolboxPrintOnly"), printOnly);

   WBFL::System::ComCatMgr::CreateCategory(CComBSTR("PGStable Plugins"), CATID_PGStablePlugin);

   return CWinApp::InitInstance();
}

int CBEToolboxApp::ExitInstance()
{
   GXForceTerminate();
	return CWinApp::ExitInstance();
}
