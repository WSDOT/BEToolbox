///////////////////////////////////////////////////////////////////////
// BEToolbox - Bridge Engineering Toolbox
// Copyright © 1999-2026  Washington State Department of Transportation
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
