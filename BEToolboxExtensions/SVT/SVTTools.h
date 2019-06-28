///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2019  Washington State Department of Transportation
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

#include "SVTToolDocTemplate.h"
#include "SVTToolDoc.h"
#include "SVTToolView.h"
#include "SVTToolChildFrame.h"

// {888B471B-44EA-45C4-B793-C601FD282069}
DEFINE_GUID(CLSID_SVTTool, 
0x888b471b, 0x44ea, 0x45c4, 0xb7, 0x93, 0xc6, 0x1, 0xfd, 0x28, 0x20, 0x69);
class CSVTTool : public CToolImpl<CSVTTool, &CLSID_SVTTool, IDR_SVTTOOL, CSVTToolDoc, CSVTToolChildFrame, CSVTToolView, CSVTToolDocTemplate, IDR_SVT>
{
public:
   virtual CString GetName() const override { return _T("St. Venant Torsional Constant"); }
};
OBJECT_ENTRY_AUTO(CLSID_SVTTool, CSVTTool)
