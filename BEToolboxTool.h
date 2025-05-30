///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2025  Washington State Department of Transportation
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

#include <EAF\ComponentObject.h>

/// Interface for all BEToolbox tools
class IBEToolboxTool
{
public:
   /// Called from the framework to initialize the tool.
   virtual BOOL Init(
      CEAFApp* pParent ///< pointer to the main application object
   ) = 0;

   /// Called by the framework when the tool is being shutdown and destroyed
   virtual void Terminate() = 0;

   /// Returns the name of the tool
   virtual CString GetName() const = 0;

   /// Factory method to create a CEAFDocTemplate for the tool
   virtual CEAFDocTemplate* CreateDocumentTemplate() const = 0;
};
