// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
#include "dllmain.h"

#include <EAF\ComponentModule.h>
#include "ExampleTool/ExampleTool.h"
#include "CLSID.h"

WBFL::EAF::ComponentModule Module_;

EAF_BEGIN_OBJECT_MAP(ObjectMap)
   EAF_OBJECT_ENTRY(CLSID_ExampleTool, CExampleTool)
EAF_END_OBJECT_MAP()

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
   Module_.Init(ObjectMap);
	return CWinApp::InitInstance();
}

int CBEToolboxToolExampleApp::ExitInstance()
{
   Module_.Term();
	return CWinApp::ExitInstance();
}
