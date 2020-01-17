///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2020  Washington State Department of Transportation
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

#include "M3CDocTemplate.h"
#include "M3CDoc.h"
#include "M3CChildFrame.h"
#include "M3CTabView.h"

// {DACA33E6-D45C-4FBA-B04F-50F0A7A8E132}
DEFINE_GUID(CLSID_M3CTool,
   0xdaca33e6, 0xd45c, 0x4fba, 0xb0, 0x4f, 0x50, 0xf0, 0xa7, 0xa8, 0xe1, 0x32);
class CM3CTool : public CToolImpl<CM3CTool, &CLSID_M3CTool, IDR_M3CTOOL, CM3CDoc, CM3CChildFrame, CM3CTabView, CM3CDocTemplate, IDR_M3C>
{
public:
   virtual CString GetName() const override { return _T("M3C"); }
};
OBJECT_ENTRY_AUTO(CLSID_M3CTool, CM3CTool)
