// QConLE.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f QConLEps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>


#include "QConLE.h"
#include "QConLE_i.c"
#include "QConLEEngine.h"
#include "QConLESpan.h"
#include "QConLESpans.h"

#include <LBAM_i.c>
#include <LBAMAnalysis_i.c>
#include <LBAMLiveLoader_i.c>
#include <LBAMLoadCombiner_i.c>
#include <LBAMUtility_i.c>
#include <LBAMAnalysisUtility_i.c>
#include <WBFLUnitServer_i.c>
#include <StrStorage2_i.c>
#include <WBFLTools_i.c>

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_QConLEEngine, CQConLEEngine)
OBJECT_ENTRY(CLSID_QConLESpan, CQConLESpan)
OBJECT_ENTRY(CLSID_QConLESpans, CQConLESpans)
END_OBJECT_MAP()

class CQConLEApp : public CWinApp
{
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQConLEApp)
	public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CQConLEApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CQConLEApp, CWinApp)
	//{{AFX_MSG_MAP(CQConLEApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CQConLEApp theApp;

BOOL CQConLEApp::InitInstance()
{
    _Module.Init(ObjectMap, m_hInstance, &LIBID_QCONLELib);
    return CWinApp::InitInstance();
}

int CQConLEApp::ExitInstance()
{
    _Module.Term();
    return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
    return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
    return _Module.UnregisterServer(TRUE);
}


