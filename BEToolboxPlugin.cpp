///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2018  Washington State Department of Transportation
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

// BEToolboxPlugin.cpp : Implementation of CBEToolboxPlugin

#include "stdafx.h"
#include "BEToolboxPlugin.h"
#include "BoxGdrDocTemplate.h"
#include "BoxGdrDoc.h"
#include "BoxGdrRptView.h"
#include "BoxGdrChildFrame.h"

#include "GenCompDocTemplate.h"
#include "GenCompDoc.h"
#include "GenCompRptView.h"
#include "GenCompChildFrame.h"

#include "GirCompDocTemplate.h"
#include "GirCompDoc.h"
#include "GirCompRptView.h"
#include "GirCompChildFrame.h"

#include "UltColDocTemplate.h"
#include "UltColDoc.h"
#include "UltColView.h"
#include "UltColChildFrame.h"

#include "CurvelDocTemplate.h"
#include "CurvelDoc.h"
#include "CurvelRptView.h"
#include "CurvelChildFrame.h"

#include "PGStableDocTemplate.h"
#include "PGStableDoc.h"
#include "PGStableTabView.h"
#include "PGStableChildFrame.h"

#include "SpectraDocTemplate.h"
#include "SpectraDoc.h"
#include "SpectraRptView.h"
#include "SpectraChildFrame.h"

#include <MFCTools\VersionInfo.h>

#define ID_MANAGE_PLUGINS 37000

BEGIN_MESSAGE_MAP(CMyCmdTarget, CCmdTarget)
   ON_COMMAND(ID_MANAGE_PLUGINS, OnManagePlugins)
END_MESSAGE_MAP()

void CMyCmdTarget::OnManagePlugins()
{
   m_pMyAppPlugin->ManagePlugins();
}

// CBEToolboxPlugin
BOOL CBEToolboxPlugin::Init(CEAFApp* pParent)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   m_DocumentationImpl.Init(this);
   return TRUE;
}

void CBEToolboxPlugin::Terminate()
{
}

void CBEToolboxPlugin::IntegrateWithUI(BOOL bIntegrate)
{
   CEAFMainFrame* pFrame = EAFGetMainFrame();
   CEAFMenu* pMainMenu = pFrame->GetMainMenu();

   UINT filePos = pMainMenu->FindMenuItem(_T("&File"));
   CEAFMenu* pFileMenu = pMainMenu->GetSubMenu(filePos);

   UINT managePos = pFileMenu->FindMenuItem(_T("Manage"));
   CEAFMenu* pManageMenu = pFileMenu->GetSubMenu(managePos);

   if (bIntegrate)
   {
      pManageMenu->AppendMenu(ID_MANAGE_PLUGINS, _T("PGStable Plugins..."), this);
   }
   else
   {
      pManageMenu->RemoveMenu(ID_MANAGE_PLUGINS, MF_BYCOMMAND, this);
   }
}

std::vector<CEAFDocTemplate*> CBEToolboxPlugin::CreateDocTemplates()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   std::vector<CEAFDocTemplate*> vDocTemplates;

   CBoxGdrDocTemplate* pBoxGdrDocTemplate;
	pBoxGdrDocTemplate = new CBoxGdrDocTemplate(
      IDR_BOXGDR,
      nullptr,
		RUNTIME_CLASS(CBoxGdrDoc),
		RUNTIME_CLASS(CBoxGdrChildFrame),
		RUNTIME_CLASS(CBoxGdrRptView),
      nullptr,1);

   vDocTemplates.push_back(pBoxGdrDocTemplate);


   CGenCompDocTemplate* pGenCompDocTemplate;
	pGenCompDocTemplate = new CGenCompDocTemplate(
      IDR_GENCOMP,
      nullptr,
		RUNTIME_CLASS(CGenCompDoc),
		RUNTIME_CLASS(CGenCompChildFrame),
		RUNTIME_CLASS(CGenCompRptView),
      nullptr,1);

   vDocTemplates.push_back(pGenCompDocTemplate);


   CGirCompDocTemplate* pGirCompDocTemplate;
	pGirCompDocTemplate = new CGirCompDocTemplate(
      IDR_GIRCOMP,
      nullptr,
		RUNTIME_CLASS(CGirCompDoc),
		RUNTIME_CLASS(CGirCompChildFrame),
		RUNTIME_CLASS(CGirCompRptView),
      nullptr,1);

   vDocTemplates.push_back(pGirCompDocTemplate);


   CUltColDocTemplate* pUltColDocTemplate;
	pUltColDocTemplate = new CUltColDocTemplate(
      IDR_ULTCOL,
      nullptr,
		RUNTIME_CLASS(CUltColDoc),
		RUNTIME_CLASS(CUltColChildFrame),
		RUNTIME_CLASS(CUltColView),
      nullptr,1);

   vDocTemplates.push_back(pUltColDocTemplate);


   CCurvelDocTemplate* pCurvelDocTemplate;
	pCurvelDocTemplate = new CCurvelDocTemplate(
      IDR_CURVEL,
      nullptr,
		RUNTIME_CLASS(CCurvelDoc),
		RUNTIME_CLASS(CCurvelChildFrame),
		RUNTIME_CLASS(CCurvelRptView),
      nullptr,1);

   vDocTemplates.push_back(pCurvelDocTemplate);



   CPGStableDocTemplate* pPGStableDocTemplate;
	pPGStableDocTemplate = new CPGStableDocTemplate(
      IDR_PGSTABLE,
      nullptr,
		RUNTIME_CLASS(CPGStableDoc),
		RUNTIME_CLASS(CPGStableChildFrame),
		RUNTIME_CLASS(CPGStableTabView),
      nullptr,1);

   vDocTemplates.push_back(pPGStableDocTemplate);


   CSpectraDocTemplate* pSpectraDocTemplate;
   pSpectraDocTemplate = new CSpectraDocTemplate(
      IDR_SPECTRA,
      nullptr,
      RUNTIME_CLASS(CSpectraDoc),
      RUNTIME_CLASS(CSpectraChildFrame),
      RUNTIME_CLASS(CSpectraRptView),
      nullptr,1);

   vDocTemplates.push_back(pSpectraDocTemplate);

   return vDocTemplates;
}

HMENU CBEToolboxPlugin::GetSharedMenuHandle()
{
   return nullptr;
}

CString CBEToolboxPlugin::GetName()
{
   return CString("BEToolbox");
}

CString CBEToolboxPlugin::GetDocumentationSetName()
{
   return GetName();
}

CString CBEToolboxPlugin::GetDocumentationURL()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return m_DocumentationImpl.GetDocumentationURL();
}

CString CBEToolboxPlugin::GetDocumentationMapFile()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return m_DocumentationImpl.GetDocumentationMapFile();
}

void CBEToolboxPlugin::LoadDocumentationMap()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return m_DocumentationImpl.LoadDocumentationMap();
}

eafTypes::HelpResult CBEToolboxPlugin::GetDocumentLocation(LPCTSTR lpszDocSetName,UINT nID,CString& strURL)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return m_DocumentationImpl.GetDocumentLocation(lpszDocSetName,nID,strURL);
}

//////////////////////////
// IEAFCommandCallback
BOOL CBEToolboxPlugin::OnCommandMessage(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
   return m_MyCmdTarget.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CBEToolboxPlugin::GetStatusBarMessageString(UINT nID, CString& rMessage) const
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   // load appropriate string
   if (rMessage.LoadString(nID))
   {
      // first newline terminates actual string
      rMessage.Replace(_T('\n'), _T('\0'));
   }
   else
   {
      // not found
      TRACE1("Warning: no message line prompt for ID %d.\n", nID);
   }

   return TRUE;
}

BOOL CBEToolboxPlugin::GetToolTipMessageString(UINT nID, CString& rMessage) const
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CString string;
   // load appropriate string
   if (string.LoadString(nID))
   {
      // tip is after first newline 
      int pos = string.Find('\n');
      if (0 < pos)
         rMessage = string.Mid(pos + 1);
   }
   else
   {
      // not found
      TRACE1("Warning: no tool tip for ID %d.\n", nID);
   }

   return TRUE;
}


void CBEToolboxPlugin::ManagePlugins()
{
   std::vector<CEAFPluginState> pluginStates = EAFManageApplicationPlugins(_T("Manage PGStable Plugins"), _T("Select the PGStable plugins that you want to be available"), CATID_PGStablePlugin, EAFGetMainFrame());

   if (pluginStates.size() == 0)
      return;

   // Find our document template
   CEAFApp* pApp = EAFGetApp();

   // write the plugin states into the registry 
   std::vector<CEAFPluginState>::iterator iter;
   for (iter = pluginStates.begin(); iter != pluginStates.end(); iter++)
   {
      CEAFPluginState& state = *iter;
#if !defined _WBFL_VERSION
#error _WBFL_VERSION must be defined... add #include <WBFLAll.h>
#endif

#if _WBFL_VERSION < 330
      // Prior to WBFL version 3.3, there is a bug in the state.IsEnabled function
      // This code works around the bug
      bool bIsEnabled = false;
      if ((state.InitiallyEnabled() && !state.StateChanged()) || (!state.InitiallyEnabled() && state.StateChanged()))
         bIsEnabled = true;

      pApp->WriteProfileString(_T("Plugins"), state.GetCLSIDString(), bIsEnabled ? _T("Enabled") : _T("Disabled"));
#else
      pApp->WriteProfileString(_T("Plugins"), state.GetCLSIDString(), state.IsEnabled() ? _T("Enabled") : _T("Disabled"));
#endif
   }
}
