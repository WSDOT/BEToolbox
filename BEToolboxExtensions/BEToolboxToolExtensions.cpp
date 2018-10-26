// BEToolboxToolExtensions.cpp : Implementation of DLL Exports.


#include "stdafx.h"
#include "resource.h"

#include <initguid.h>

#include "dllmain.h"

#include <WBFLRCCapacity.h>
#include <WBFLRCCapacity_i.c>

#include <WBFLGeometry_i.c>
#include <WBFLUnitServer_i.c>

#include <DManip\DManip.h>

#include "Tools.h"

using namespace ATL;



HRESULT Register(bool bRegister)
{
   HRESULT hr = S_OK;

   hr = sysComCatMgr::RegWithCategory(CLSID_M3CTool, CATID_BEToolboxTool, bRegister);
   if (FAILED(hr))
   {
      return hr;
   }

   return S_OK;
}


// Used to determine whether the DLL can be unloaded by OLE.
STDAPI DllCanUnloadNow(void)
{
			AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (AfxDllCanUnloadNow()==S_OK && _AtlModule.GetLockCount()==0) ? S_OK : S_FALSE;
	}

// Returns a class factory to create an object of the requested type.
_Check_return_
STDAPI DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID* ppv)
{
		return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}

// DllRegisterServer - Adds entries to the system registry.
STDAPI DllRegisterServer(void)
{
	// registers object, typelib and all interfaces in typelib
	HRESULT hr = _AtlModule.DllRegisterServer(FALSE);
   Register(true);
		return hr;
}

// DllUnregisterServer - Removes entries from the system registry.
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer(FALSE);
   Register(false);
   return hr;
}

// DllInstall - Adds/Removes entries to the system registry per user per machine.
STDAPI DllInstall(BOOL bInstall, _In_opt_  LPCWSTR pszCmdLine)
{
	HRESULT hr = E_FAIL;
	static const wchar_t szUserSwitch[] = L"user";

	if (pszCmdLine != NULL)
	{
		if (_wcsnicmp(pszCmdLine, szUserSwitch, _countof(szUserSwitch)) == 0)
		{
			ATL::AtlSetPerUserRegistration(true);
		}
	}

	if (bInstall)
	{	
		hr = DllRegisterServer();
		if (FAILED(hr))
		{
			DllUnregisterServer();
		}
	}
	else
	{
		hr = DllUnregisterServer();
	}

	return hr;
}


