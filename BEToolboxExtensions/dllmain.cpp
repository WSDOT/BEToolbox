// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
#include "dllmain.h"

CBEToolboxExtensionsModule _AtlModule;

class CBEToolboxExtensionsApp : public CWinApp
{
public:

// Overrides
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CBEToolboxExtensionsApp, CWinApp)
END_MESSAGE_MAP()

CBEToolboxExtensionsApp theApp;

BOOL CBEToolboxExtensionsApp::InitInstance()
{
	return CWinApp::InitInstance();
}

int CBEToolboxExtensionsApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
