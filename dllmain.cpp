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
#include <WBFLSections.h>
#include <WBFLSections_i.c>
#include <EAF\EAFComponentInfo.h>
#include <WBFLRCCapacity.h>
#include <WBFLRCCapacity_i.c>
#include <WBFLCogo.h>
#include <WBFLCogo_i.c>

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

   // Set up report styles
   rptFontStyleLibrary* pStyleLib = rptFontStyleLibrary::Instance();

   // Report Title
   rptRiStyle title;
   title.SetFontSize(16);
   title.SetBold( true );
   title.SetFontType(rptRiStyle::SWISS);
   title.SetAlignment(rptRiStyle::CENTER);
   title.SetMediaType(rptRiStyle::Print); // only shows up on the printer
   pStyleLib->AddNamedStyle(_T("ReportTitle"), title);

   // Report Title
   rptRiStyle subtitle;
   subtitle.SetFontSize(15);
   subtitle.SetItalic( true );
   subtitle.SetFontType(rptRiStyle::SWISS);
   subtitle.SetAlignment(rptRiStyle::CENTER);
   subtitle.SetMediaType(rptRiStyle::Print); // only shows up on the printer
   pStyleLib->AddNamedStyle(_T("ReportSubtitle"), subtitle);

   // Copyright
   rptRiStyle copyright;
   copyright.SetFontSize(8);
   copyright.SetItalic( false );
   copyright.SetFontType( rptRiStyle::SWISS );
   copyright.SetAlignment(rptRiStyle::CENTER);
   copyright.SetMediaType(rptRiStyle::Print); // only shows up on the printer
   pStyleLib->AddNamedStyle(_T("CopyrightText"), copyright);

   // column headings
   rptRiStyle tableTitle;
   tableTitle.SetFontSize(9);
   tableTitle.SetFontType(rptRiStyle::SWISS);
   tableTitle.SetAlignment(rptRiStyle::CENTER);
   tableTitle.SetVerticalAlignment( rptRiStyle::TOP );
   tableTitle.SetBold( true );
   pStyleLib->AddNamedStyle(_T("TableTitle"), tableTitle);

   rptRiStyle colheadings;
   colheadings.SetFontSize(9);
   colheadings.SetFontType(rptRiStyle::SWISS);
   colheadings.SetAlignment(rptRiStyle::CENTER);
   colheadings.SetVerticalAlignment( rptRiStyle::TOP );
   colheadings.SetBold( true );
   colheadings.SetBGColor( rptRiStyle::LightSteelBlue );
   pStyleLib->AddNamedStyle(_T("ColumnHeading"), colheadings);

   // cell style
   rptRiStyle cell;
   cell.SetFontSize(9);
   cell.SetFontType(rptRiStyle::SWISS);
   cell.SetAlignment(rptRiStyle::RIGHT);
   cell.SetTopBorder(rptRiStyle::HAIR_THICK);
   cell.SetVerticalAlignment( rptRiStyle::BASELINE );
   cell.SetBottomBorder(rptRiStyle::HAIR_THICK);
   cell.SetLeftBorder(rptRiStyle::HAIR_THICK);
   cell.SetRightBorder(rptRiStyle::HAIR_THICK);
   pStyleLib->AddNamedStyle(_T("NormalRow"),cell);

   cell.SetAlignment(rptRiStyle::LEFT);
   pStyleLib->AddNamedStyle(_T("NormalRowLJ"),cell);

   cell.SetAlignment(rptRiStyle::RIGHT);
   cell.SetBGColor( rptRiStyle::AliceBlue );
   pStyleLib->AddNamedStyle(_T("StripedRow"),cell);

   cell.SetAlignment(rptRiStyle::LEFT);
   pStyleLib->AddNamedStyle(_T("StripedRowLJ"),cell);


   rptRiStyle layoutcell;
   layoutcell.SetFontType(rptRiStyle::SWISS);
   layoutcell.SetAlignment(rptRiStyle::LEFT);
   layoutcell.SetTopBorder(rptRiStyle::HAIR_THICK);
   layoutcell.SetVerticalAlignment( rptRiStyle::TOP );
   layoutcell.SetBottomBorder(rptRiStyle::HAIR_THICK);
   layoutcell.SetLeftBorder(rptRiStyle::HAIR_THICK);
   layoutcell.SetRightBorder(rptRiStyle::HAIR_THICK);
   pStyleLib->AddNamedStyle(_T("LayoutCell"),layoutcell);

   // normal style
   rptRiStyle normal;
   normal.SetBold( false );
   normal.SetFontSize(9);
   normal.SetFontType(rptRiStyle::SWISS);
   normal.SetVerticalAlignment( rptRiStyle::TOP );
   normal.SetAlignment(rptRiStyle::LEFT);
   pStyleLib->AddNamedStyle(_T("Normal"), normal);

   rptRiStyle headings;
   headings.SetFontType(rptRiStyle::SWISS);
   headings.SetFontSize(11);
   headings.SetBold( true );
   pStyleLib->AddNamedStyle(_T("Headings"), headings);

   // Deal with help file name
   CString strHelpFile(m_pszHelpFilePath);
#if defined _DEBUG
#if defined _WIN64
   strHelpFile.Replace(_T("x64\\Debug\\"),_T(""));
#else
   strHelpFile.Replace(_T("Win32\\Debug\\"),_T(""));
#endif
#else
   // in a real release, the path doesn't contain RegFreeCOM\\Release, but that's
   // ok... the replace will fail and the string wont be altered.
#if defined _WIN64
   strHelpFile.Replace(_T("x64\\Release\\"),_T(""));
#else
   strHelpFile.Replace(_T("Win32\\Release\\"),_T(""));
#endif
#endif

   // rename the file itself
   int loc = strHelpFile.ReverseFind('\\');
   strHelpFile = strHelpFile.Left(loc+1);
   strHelpFile += _T("BEToolbox.chm");

   free((void*)m_pszHelpFilePath);
   m_pszHelpFilePath = _tcsdup(strHelpFile);

   // pretty sure this is required
   EnableHtmlHelp();

   return CWinApp::InitInstance();
}

int CBEToolboxApp::ExitInstance()
{
   ::GXTerminate();

	return CWinApp::ExitInstance();
}
