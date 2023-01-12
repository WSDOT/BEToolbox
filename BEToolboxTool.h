///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2023  Washington State Department of Transportation
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

class CEAFApp;
class CBEToolboxPlugin;
class CEAFDocTemplate;

// {4EB3C9C6-69ED-4A2D-B16A-D8AD4C1CDB53}
DEFINE_GUID(CATID_BEToolboxTool,
   0x4eb3c9c6, 0x69ed, 0x4a2d, 0xb1, 0x6a, 0xd8, 0xad, 0x4c, 0x1c, 0xdb, 0x53);


// {80A75731-DFD6-40C2-8FA0-20E8DADD9932}
DEFINE_GUID(IID_IBEToolboxTool,
   0x80a75731, 0xdfd6, 0x40c2, 0x8f, 0xa0, 0x20, 0xe8, 0xda, 0xdd, 0x99, 0x32);
struct __declspec(uuid("{80A75731-DFD6-40C2-8FA0-20E8DADD9932}")) IBEToolboxTool;// for __uuidof

interface IBEToolboxTool : IUnknown
{
   virtual BOOL Init(CEAFApp* pParent) = 0;
   virtual void Terminate() = 0;
   virtual CString GetName() const = 0;
   virtual CEAFDocTemplate* CreateDocumentTemplate() const = 0;
};
