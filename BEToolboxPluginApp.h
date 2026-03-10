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

// BEToolboxPlugin.h : Declaration of the CBEToolboxPlugin

#pragma once

#include <EAF\ComponentObject.h>
#include <EAF\PluginApp.h>
#include <EAF\PluginAppDocumentationImpl.h>
#include <EAF\EAFUIIntegration.h>
#include <EAF\PluginManagerBase.h>
#include "BEToolboxTool.h"


class CBEToolboxToolManager : public WBFL::EAF::PluginManagerBase<IBEToolboxTool, CEAFApp>
{
public:
   std::vector<CEAFDocTemplate*> CreateDocumentTemplates()
   {
      std::vector<CEAFDocTemplate*> vTemplates;
      vTemplates.reserve(m_Plugins.size());
      for (auto& item : m_Plugins)
      {
         auto plugin = item.second;
         vTemplates.push_back(plugin->CreateDocumentTemplate());
      }
      return vTemplates;
   }
};


// CBEToolboxPlugin

class CBEToolboxPlugin;

class CMyCmdTarget : public CCmdTarget
{
public:
   CMyCmdTarget() {};

   afx_msg void OnManagePlugins();
   afx_msg void OnManagePGStablePlugins();

   CBEToolboxPlugin* m_pMyAppPlugin;

   DECLARE_MESSAGE_MAP()
};

class CBEToolboxPlugin : public WBFL::EAF::ComponentObject,
   public WBFL::EAF::IPluginApp,
   public WBFL::EAF::ICommandCallback
{
public:
	CBEToolboxPlugin()
	{
      m_MyCmdTarget.m_pMyAppPlugin = this;
   }

public:
   CMyCmdTarget m_MyCmdTarget;

   void ManagePlugins();
   void ManagePGStablePlugins();


// IPluginApp
public:
   BOOL Init(CEAFApp* pParent) override;
   void Terminate() override;
   void IntegrateWithUI(BOOL bIntegrate) override;
   std::vector<CEAFDocTemplate*> CreateDocTemplates() override;
   HMENU GetSharedMenuHandle() override;
   CString GetName() override;
   CString GetDocumentationSetName() override;
   CString GetDocumentationURL() override;
   CString GetDocumentationMapFile() override;
   void LoadDocumentationMap() override;
   std::pair<WBFL::EAF::HelpResult,CString> GetDocumentLocation(LPCTSTR lpszDocSetName,UINT nID) override;

// ICommandCallback
public:
   BOOL OnCommandMessage(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) override;
   BOOL GetStatusBarMessageString(UINT nID, CString& rMessage) const override;
   BOOL GetToolTipMessageString(UINT nID, CString& rMessage) const override;

private:
   WBFL::EAF::PluginAppDocumentationImpl m_DocumentationImpl;

   CBEToolboxToolManager m_Tools;
};
