///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2018  Washington State Department of Transportation
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

#pragma once

#include "resource.h"
#include <ToolImpl.h>

#include "ExampleToolDocTemplate.h"
#include "ExampleToolDoc.h"
#include "ExampleToolView.h"
#include "ExampleToolChildFrame.h"

// {23CFD70B-F121-4122-821B-806F9685BE1A}
DEFINE_GUID(CLSID_ExampleTool,
   0x23cfd70b, 0xf121, 0x4122, 0x82, 0x1b, 0x80, 0x6f, 0x96, 0x85, 0xbe, 0x1a);
class CExampleTool : public CToolImpl<CExampleTool, &CLSID_ExampleTool, IDR_EXAMPLETOOL, CExampleToolDoc, CExampleToolChildFrame, CExampleToolView, CExampleToolDocTemplate, IDR_EXAMPLE>
{
public:
   virtual CString GetName() const override { return _T("ExampleTool"); }
};
OBJECT_ENTRY_AUTO(CLSID_ExampleTool, CExampleTool)
