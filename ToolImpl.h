///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2022  Washington State Department of Transportation
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

#include "BEToolboxTool.h"

template<class C,const CLSID* pclsid,int RID,class Doc,class Frame, class View,class Template, int ID>
class ATL_NO_VTABLE CToolImpl : 
   public CComObjectRootEx<CComSingleThreadModel>,
   public CComCoClass<C, pclsid>,
   public IBEToolboxTool
{
public:
   HRESULT FinalConstruct() { return S_OK; }
   void FinalRelease() {}

public:
   DECLARE_REGISTRY_RESOURCEID(RID)
   DECLARE_PROTECT_FINAL_CONSTRUCT()

   BEGIN_COM_MAP(C)
      COM_INTERFACE_ENTRY(IBEToolboxTool)
   END_COM_MAP()

   // IBEToolboxTool
public:
   virtual BOOL Init(CEAFApp* pParent) override
   {  return TRUE; }
   virtual void Terminate() override {}
   //virtual CString GetName() const override { return pstrName; }
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

