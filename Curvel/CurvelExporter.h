///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright � 1999-2025  Washington State Department of Transportation
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

// CurvelExporter.h : Declaration of the CCurvelExporter

#pragma once

#include <Plugins\PGSuperIEPlugin.h>
#include "..\resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCurvelExporter
class ATL_NO_VTABLE CCurvelExporter : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCurvelExporter, &CLSID_CurvelExporter>,
   public IPGSDataExporter
{
public:
	CCurvelExporter()
	{
	}

   HRESULT FinalConstruct();

   CBitmap m_Bitmap;

DECLARE_REGISTRY_RESOURCEID(IDR_CURVELEXPORTER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CCurvelExporter)
   COM_INTERFACE_ENTRY(IPGSDataExporter)
END_COM_MAP()

// IPGSDataExporter
public:
   STDMETHOD(Init)(UINT nCmdID);
   STDMETHOD(GetMenuText)(/*[out,retval]*/BSTR*  bstrText) const;
   STDMETHOD(GetBitmapHandle)(/*[out]*/HBITMAP* phBmp) const;
   STDMETHOD(GetCommandHintText)(BSTR*  bstrText) const;
   STDMETHOD(Export)(/*[in]*/IBroker* pBroker);
};

OBJECT_ENTRY_AUTO(__uuidof(CurvelExporter), CCurvelExporter)
