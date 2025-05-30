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

#include <EAF\ComponentObject.h>
#include "BEToolboxTool.h"

template<class Doc,class Frame, class View,class Template, int ID>
class CToolImpl : public WBFL::EAF::ComponentObject,
   public IBEToolboxTool
{
   // IBEToolboxTool
public:
   virtual BOOL Init(CEAFApp* pParent) override
   {  return TRUE; }

   virtual void Terminate() override {}

   virtual CEAFDocTemplate* CreateDocumentTemplate() const override 
   { 
      AFX_MANAGE_STATE(AfxGetStaticModuleState());

      std::unique_ptr<Template> pTemplate = std::make_unique<Template>(
         ID,
         nullptr,
         RUNTIME_CLASS(Doc),
         RUNTIME_CLASS(Frame),
         RUNTIME_CLASS(View),
         nullptr, 1);

      return pTemplate.release();
   }
};
