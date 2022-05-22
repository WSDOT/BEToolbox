///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2022  Washington State Department of Transportation
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

// BEToolbox.cpp : Implementation of DLL Exports.


#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "BEToolbox_i.h"
#include "dllmain.h"
#include "BEToolboxTool.h"

#include <EAF\EAFAppPlugin.h>
#include <System\ComCatMgr.h>

#include <BridgeLinkCATID.h>
#include "PGSuperCatCom.h"
#include "PGSpliceCatCom.h"
#include <WBFLCore_i.c>
#include <WBFLTools_i.c>
#include <Plugins\PGSuperIEPlugin.h>
#include <Plugins\BeamFamilyCLSID.h>

#include <EAF\EAFDocument.h>
#include <EAF\EAFDisplayUnits.h>

#include <IFace\Intervals.h>
#include <IFace\Bridge.h>
#include <IFace\Project.h>
#include <IFace\GirderHandlingSpecCriteria.h>
#include <IFace\PrestressForce.h>
#include <IFace\AnalysisResults.h>
#include <IFace\DocumentType.h>
#include <IFace\Selection.h>

#include "Tools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Used to determine whether the DLL can be unloaded by OLE
STDAPI DllCanUnloadNow(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return (AfxDllCanUnloadNow()==S_OK && _AtlModule.GetLockCount()==0) ? S_OK : S_FALSE;
}


// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


HRESULT Register(bool bRegister)
{
   HRESULT hr = S_OK;

   // Register the BEToolbox component infor object with BridgeLink... this gives BridgeLink an object with
   // BEToolbox information for the About dialog
   hr = WBFL::System::ComCatMgr::RegWithCategory(CLSID_BEToolboxComponentInfo,CATID_BridgeLinkComponentInfo,bRegister);
   if ( FAILED(hr) )
      return hr;

   // Register BEToolbox as a BridgeLink application (this makes BEToolbox show up in the New Project dialog)
   hr = WBFL::System::ComCatMgr::RegWithCategory(CLSID_BEToolboxPlugin, CATID_BridgeLinkAppPlugin, bRegister);
   if ( FAILED(hr) )
      return hr;

   // Register the PGSuper/PGSplice Data Importers/Exporters this DLL implements
   hr = WBFL::System::ComCatMgr::RegWithCategory(CLSID_CurvelImporter,  CATID_PGSuperDataImporter,    bRegister);
   if ( FAILED(hr) )
   {
      return hr;
   }

   hr = WBFL::System::ComCatMgr::RegWithCategory(CLSID_CurvelImporter,  CATID_PGSpliceDataImporter,    bRegister);
   if ( FAILED(hr) )
   {
      return hr;
   }

   hr = WBFL::System::ComCatMgr::RegWithCategory(CLSID_CurvelExporter,  CATID_PGSuperDataExporter,    bRegister);
   if ( FAILED(hr) )
   {
      return hr;
   }

   hr = WBFL::System::ComCatMgr::RegWithCategory(CLSID_CurvelExporter,  CATID_PGSpliceDataExporter,    bRegister);
   if ( FAILED(hr) )
   {
      return hr;
   }

   hr = WBFL::System::ComCatMgr::RegWithCategory(CLSID_GenCompExporter,  CATID_PGSuperDataExporter,    bRegister);
   if ( FAILED(hr) )
   {
      return hr;
   }

   hr = WBFL::System::ComCatMgr::RegWithCategory(CLSID_GenCompExporter,  CATID_PGSpliceDataExporter,    bRegister);
   if ( FAILED(hr) )
   {
      return hr;
   }

   hr = WBFL::System::ComCatMgr::RegWithCategory(CLSID_PGStableExporter,  CATID_PGSuperDataExporter,    bRegister);
   if ( FAILED(hr) )
   {
      return hr;
   }

   hr = WBFL::System::ComCatMgr::RegWithCategory(CLSID_PGStableExporter,  CATID_PGSpliceDataExporter,    bRegister);
   if ( FAILED(hr) )
   {
      return hr;
   }

   // Register the standard BEToolbox Tools so they can be discovered at application startup
   hr = WBFL::System::ComCatMgr::RegWithCategory(CLSID_BoxGdrTool, CATID_BEToolboxTool, bRegister);
   if (FAILED(hr))
   {
      return hr;
   }

   hr = WBFL::System::ComCatMgr::RegWithCategory(CLSID_GenCompTool, CATID_BEToolboxTool, bRegister);
   if (FAILED(hr))
   {
      return hr;
   }

   hr = WBFL::System::ComCatMgr::RegWithCategory(CLSID_GirCompTool, CATID_BEToolboxTool, bRegister);
   if (FAILED(hr))
   {
      return hr;
   }

   hr = WBFL::System::ComCatMgr::RegWithCategory(CLSID_UltColTool, CATID_BEToolboxTool, bRegister);
   if (FAILED(hr))
   {
      return hr;
   }

   hr = WBFL::System::ComCatMgr::RegWithCategory(CLSID_CurvelTool, CATID_BEToolboxTool, bRegister);
   if (FAILED(hr))
   {
      return hr;
   }

   hr = WBFL::System::ComCatMgr::RegWithCategory(CLSID_PGStableTool, CATID_BEToolboxTool, bRegister);
   if (FAILED(hr))
   {
      return hr;
   }

   hr = WBFL::System::ComCatMgr::RegWithCategory(CLSID_SpectraTool, CATID_BEToolboxTool, bRegister);
   if (FAILED(hr))
   {
      return hr;
   }


   return S_OK;
}

// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib

   HRESULT hr = _AtlModule.DllRegisterServer();
   if ( FAILED(hr) )
      return hr;

   hr = Register(true);
   if ( FAILED(hr) )
      return hr;

   return S_OK;
}


// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
   HRESULT hr = Register(false);
   if ( FAILED(hr) )
      return hr;

   hr = _AtlModule.DllUnregisterServer();
	return hr;
}

// THIS FUNCTION WAS CAUSING SOME PROBLEMS.
//
// Must compile with UNICODE for this function, but this caused MFC problems
// because the other DLL's aren't compiled with UNICODE. If this function is
// put back into the mix, be sure to go to BEToolbox.def and put it
// back in the function exports as well

//// DllInstall - Adds/Removes entries to the system registry per user
////              per machine.	
//STDAPI DllInstall(BOOL bInstall, LPCWSTR pszCmdLine)
//{
//    HRESULT hr = E_FAIL;
//    static const wchar_t szUserSwitch[] = _T("user");
//
//    if (pszCmdLine != nullptr)
//    {
//    	if (_wcsnicmp(pszCmdLine, szUserSwitch, _countof(szUserSwitch)) == 0)
//    	{
//    		AtlSetPerUserRegistration(true);
//    	}
//    }
//
//    if (bInstall)
//    {	
//    	hr = DllRegisterServer();
//    	if (FAILED(hr))
//    	{	
//    		DllUnregisterServer();
//    	}
//    }
//    else
//    {
//    	hr = DllUnregisterServer();
//    }
//
//    return hr;
//}


