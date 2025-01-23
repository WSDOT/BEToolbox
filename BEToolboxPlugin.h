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
#include "resource.h"       // main symbols

#include <EAF\EAFAppPlugin.h>
#include <EAF\EAFAppPluginDocumentationImpl.h>
#include <EAF\EAFUIIntegration.h>
#include <EAF\EAFPluginManagerBase.h>
#include "BEToolboxTool.h"
#include "BEToolbox_i.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

class CBEToolboxToolManager : public CEAFPluginManagerBase<IBEToolboxTool, CEAFApp>
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

class ATL_NO_VTABLE CBEToolboxPlugin :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CBEToolboxPlugin, &CLSID_BEToolboxPlugin>,
	public IEAFAppPlugin,
   public IEAFCommandCallback
{
public:
	CBEToolboxPlugin()
	{
      m_MyCmdTarget.m_pMyAppPlugin = this;
   }

DECLARE_REGISTRY_RESOURCEID(IDR_BETOOLBOXPLUGIN)


BEGIN_COM_MAP(CBEToolboxPlugin)
	COM_INTERFACE_ENTRY(IEAFAppPlugin)
   COM_INTERFACE_ENTRY(IEAFCommandCallback)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
      return S_OK;
	}

	void FinalRelease()
	{
	}

public:
   CMyCmdTarget m_MyCmdTarget;

   void ManagePlugins();
   void ManagePGStablePlugins();


// IEAFAppPlugin
public:
   virtual BOOL Init(CEAFApp* pParent) override;
   virtual void Terminate() override;
   virtual void IntegrateWithUI(BOOL bIntegrate) override;
   virtual std::vector<CEAFDocTemplate*> CreateDocTemplates() override;
   virtual HMENU GetSharedMenuHandle() override;
   virtual CString GetName() override;
   virtual CString GetDocumentationSetName() override;
   virtual CString GetDocumentationURL() override;
   virtual CString GetDocumentationMapFile() override;
   virtual void LoadDocumentationMap() override;
   virtual eafTypes::HelpResult GetDocumentLocation(LPCTSTR lpszDocSetName,UINT nID,CString& strURL) override;

   // IEAFCommandCallback
public:
   virtual BOOL OnCommandMessage(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
   virtual BOOL GetStatusBarMessageString(UINT nID, CString& rMessage) const;
   virtual BOOL GetToolTipMessageString(UINT nID, CString& rMessage) const;

private:
   CEAFAppPluginDocumentationImpl m_DocumentationImpl;
   CBEToolboxToolManager m_Tools;
};

OBJECT_ENTRY_AUTO(__uuidof(BEToolboxPlugin), CBEToolboxPlugin)
