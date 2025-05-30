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

// BEToolboxPlugin.cpp : Implementation of CBEToolboxPlugin

#include "stdafx.h"
#include "BEToolboxPluginApp.h"
#include "BEToolboxCLSID.h"
#include ".\PGStable\PGStablePluginCATID.h"
#include <EAF\EAFApp.h>

#include <MFCTools\VersionInfo.h>

#define ID_MANAGE_PLUGINS 37000
#define ID_MANAGE_PGSTABLE_PLUGINS 37001

BEGIN_MESSAGE_MAP(CMyCmdTarget, CCmdTarget)
   ON_COMMAND(ID_MANAGE_PLUGINS, OnManagePlugins)
   ON_COMMAND(ID_MANAGE_PGSTABLE_PLUGINS, OnManagePGStablePlugins)
END_MESSAGE_MAP()

void CMyCmdTarget::OnManagePlugins()
{
   m_pMyAppPlugin->ManagePlugins();
}

void CMyCmdTarget::OnManagePGStablePlugins()
{
   m_pMyAppPlugin->ManagePGStablePlugins();
}

// CBEToolboxPlugin
BOOL CBEToolboxPlugin::Init(CEAFApp* pParent)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   auto plugin = std::dynamic_pointer_cast<WBFL::EAF::IPluginApp>(shared_from_this());
   m_DocumentationImpl.Init(plugin);
   m_Tools.SetParent(pParent);
   m_Tools.SetCATID(CATID_BEToolboxTool);

   if (m_Tools.LoadPlugins())
   {
      WBFL::System::Logger::Info(_T("Initializing BEToolbox Tools"));
      m_Tools.InitPlugins();
   }

   return TRUE;
}

void CBEToolboxPlugin::Terminate()
{
   m_Tools.UnloadPlugins();
   //m_Tools2.UnloadPlugins();
}

void CBEToolboxPlugin::IntegrateWithUI(BOOL bIntegrate)
{
   CEAFMainFrame* pFrame = EAFGetMainFrame();
   auto pMainMenu = pFrame->GetMainMenu();

   UINT filePos = pMainMenu->FindMenuItem(_T("&File"));
   auto pFileMenu = pMainMenu->GetSubMenu(filePos);

   UINT managePos = pFileMenu->FindMenuItem(_T("Manage"));
   auto pManageMenu = pFileMenu->GetSubMenu(managePos);

   auto callback = std::dynamic_pointer_cast<WBFL::EAF::ICommandCallback>(shared_from_this());
   if (bIntegrate)
   {
      pManageMenu->AppendMenu(ID_MANAGE_PLUGINS, _T("BEToolbox Tools..."), callback);
      pManageMenu->AppendMenu(ID_MANAGE_PGSTABLE_PLUGINS, _T("PGStable plugins..."), callback);
   }
   else
   {
      pManageMenu->RemoveMenu(ID_MANAGE_PGSTABLE_PLUGINS, MF_BYCOMMAND, callback);
      pManageMenu->RemoveMenu(ID_MANAGE_PLUGINS, MF_BYCOMMAND, callback);
   }
}

std::vector<CEAFDocTemplate*> CBEToolboxPlugin::CreateDocTemplates()
{
   return m_Tools.CreateDocumentTemplates();
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

std::pair<WBFL::EAF::HelpResult,CString> CBEToolboxPlugin::GetDocumentLocation(LPCTSTR lpszDocSetName,UINT nID)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return m_DocumentationImpl.GetDocumentLocation(lpszDocSetName,nID);
}

//////////////////////////
// ICommandCallback
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
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CEAFApp* pApp = (CEAFApp*)AfxGetApp();

   std::vector<CEAFPluginState> pluginStates = EAFManageApplicationPlugins(_T("Manage BEToolbox Tools"), _T("Select the BEToolbox Tools that you want to be available"), CATID_BEToolboxTool, EAFGetMainFrame(), pApp);

   if (pluginStates.size() == 0)
   {
      return;
   }

   // write the plugin states into the registry
   for(auto& state : pluginStates)
   {
      pApp->WriteProfileString(_T("Plugins"), state.GetCLSIDString(), state.IsEnabled() ? _T("Enabled") : _T("Disabled"));
   }
}

void CBEToolboxPlugin::ManagePGStablePlugins()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CEAFApp* pApp = (CEAFApp*)AfxGetApp();

   std::vector<CEAFPluginState> pluginStates = EAFManageApplicationPlugins(_T("Manage PGStable plugins"), _T("Select the PGStable plugins that you want to be available"), CATID_PGStablePlugin, EAFGetMainFrame(), pApp);

   if (pluginStates.size() == 0)
   {
      return;
   }

   // write the plugin states into the registry
   for (auto& state : pluginStates)
   {
      pApp->WriteProfileString(_T("Plugins"), state.GetCLSIDString(), state.IsEnabled() ? _T("Enabled") : _T("Disabled"));
   }
}
