// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
#include "BEToolboxToolExample_i.h"
#include "dllmain.h"

CBEToolboxToolExampleModule _AtlModule;

class CBEToolboxToolExampleApp : public CWinApp
{
public:

// Overrides
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CBEToolboxToolExampleApp, CWinApp)
END_MESSAGE_MAP()

CBEToolboxToolExampleApp theApp;

BOOL CBEToolboxToolExampleApp::InitInstance()
{
	return CWinApp::InitInstance();
}

int CBEToolboxToolExampleApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
