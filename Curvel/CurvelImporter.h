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

// CurvelImporter.h : Declaration of the CCurvelImporter

#pragma once

#include <Plugins\PGSuperIEPlugin.h>
#include "..\resource.h"       // main symbols


/////////////////////////////////////////////////////////////////////////////
// CCurvelImporter
class ATL_NO_VTABLE CCurvelImporter : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCurvelImporter, &CLSID_CurvelImporter>,
   public IPGSDataImporter
{
public:
	CCurvelImporter()
	{
	}

   HRESULT FinalConstruct();
   CBitmap m_Bitmap;

DECLARE_REGISTRY_RESOURCEID(IDR_CURVELIMPORTER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CCurvelImporter)
	COM_INTERFACE_ENTRY(IPGSDataImporter)
END_COM_MAP()

// IPGSDataImporter
public:
   STDMETHOD(Init)(UINT nCmdID);
   STDMETHOD(GetMenuText)(/*[out,retval]*/BSTR*  bstrText) const;
   STDMETHOD(GetBitmapHandle)(/*[out]*/HBITMAP* phBmp) const;
   STDMETHOD(GetCommandHintText)(BSTR*  bstrText) const;
   STDMETHOD(Import)(/*[in]*/IBroker* pBroker);
};

OBJECT_ENTRY_AUTO(__uuidof(CurvelImporter), CCurvelImporter)
