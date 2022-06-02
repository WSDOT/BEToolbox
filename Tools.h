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

#include "resource.h"
#include "ToolImpl.h"

#include ".\BoxGdr\BoxGdrDocTemplate.h"
#include ".\BoxGdr\BoxGdrDoc.h"
#include ".\BoxGdr\BoxGdrRptView.h"
#include ".\BoxGdr\BoxGdrChildFrame.h"

#include ".\GenComp\GenCompDocTemplate.h"
#include ".\GenComp\GenCompDoc.h"
#include ".\GenComp\GenCompRptView.h"
#include ".\GenComp\GenCompChildFrame.h"

#include ".\GirComp\GirCompDocTemplate.h"
#include ".\GirComp\GirCompDoc.h"
#include ".\GirComp\GirCompRptView.h"
#include ".\GirComp\GirCompChildFrame.h"

#include ".\UltCol\UltColDocTemplate.h"
#include ".\UltCol\UltColDoc.h"
#include ".\UltCol\UltColView.h"
#include ".\UltCol\UltColChildFrame.h"

#include ".\Curvel\CurvelDocTemplate.h"
#include ".\Curvel\CurvelDoc.h"
#include ".\Curvel\CurvelRptView.h"
#include ".\Curvel\CurvelChildFrame.h"

#include ".\PGStable\PGStableDocTemplate.h"
#include ".\PGStable\PGStableDoc.h"
#include ".\PGStable\PGStableTabView.h"
#include ".\PGStable\PGStableChildFrame.h"

#include ".\Spectra\SpectraDocTemplate.h"
#include ".\Spectra\SpectraDoc.h"
#include ".\Spectra\SpectraRptView.h"
#include ".\Spectra\SpectraChildFrame.h"

// {4C9C743B-199E-43C8-AF13-F1D16669ACC9}
DEFINE_GUID(CLSID_BoxGdrTool,
   0x4c9c743b, 0x199e, 0x43c8, 0xaf, 0x13, 0xf1, 0xd1, 0x66, 0x69, 0xac, 0xc9);
class CBoxGdrTool : public CToolImpl<CBoxGdrTool, &CLSID_BoxGdrTool, IDR_BOXGDRTOOL, CBoxGdrDoc, CBoxGdrChildFrame, CBoxGdrRptView, CBoxGdrDocTemplate, IDR_BOXGDR>
{
public:
   virtual CString GetName() const override { return _T("BoxGdr"); }
};
OBJECT_ENTRY_AUTO(CLSID_BoxGdrTool, CBoxGdrTool)


// {D84CBB4B-97C9-47A9-A7D1-83702A99A9CE}
DEFINE_GUID(CLSID_GenCompTool, 
   0xd84cbb4b, 0x97c9, 0x47a9, 0xa7, 0xd1, 0x83, 0x70, 0x2a, 0x99, 0xa9, 0xce);
class CGenCompTool : public CToolImpl<CGenCompTool,&CLSID_GenCompTool, IDR_GENCOMPTOOL, CGenCompDoc, CGenCompChildFrame, CGenCompRptView, CGenCompDocTemplate,IDR_GENCOMP>
{
public:
   virtual CString GetName() const override { return _T("GenComp"); }
};
OBJECT_ENTRY_AUTO(CLSID_GenCompTool, CGenCompTool)


// {AFF95666-CCDE-45A3-85DA-6E10336D4A79}
DEFINE_GUID(CLSID_GirCompTool,
   0xaff95666, 0xccde, 0x45a3, 0x85, 0xda, 0x6e, 0x10, 0x33, 0x6d, 0x4a, 0x79);
class CGirCompTool : public CToolImpl<CGirCompTool, &CLSID_GirCompTool, IDR_GIRCOMPTOOL, CGirCompDoc, CGirCompChildFrame, CGirCompRptView, CGirCompDocTemplate, IDR_GIRCOMP>
{
public:
   virtual CString GetName() const override { return _T("GirComp"); }
};
OBJECT_ENTRY_AUTO(CLSID_GirCompTool, CGirCompTool)


// {174F1702-B615-4925-ADA4-9466606622F8}
DEFINE_GUID(CLSID_UltColTool,
   0x174f1702, 0xb615, 0x4925, 0xad, 0xa4, 0x94, 0x66, 0x60, 0x66, 0x22, 0xf8);
class CUltColTool : public CToolImpl<CUltColTool, &CLSID_UltColTool, IDR_ULTCOLTOOL, CUltColDoc, CUltColChildFrame, CUltColView, CUltColDocTemplate, IDR_ULTCOL>
{
public:
   virtual CString GetName() const override { return _T("UltCol"); }
};
OBJECT_ENTRY_AUTO(CLSID_UltColTool, CUltColTool)


// {90DA3F11-67D6-45D8-9469-CCA6278021AD}
DEFINE_GUID(CLSID_CurvelTool,
   0x90da3f11, 0x67d6, 0x45d8, 0x94, 0x69, 0xcc, 0xa6, 0x27, 0x80, 0x21, 0xad);
class CCurvelTool : public CToolImpl<CCurvelTool, &CLSID_CurvelTool, IDR_CURVELTOOL, CCurvelDoc, CCurvelChildFrame, CCurvelRptView, CCurvelDocTemplate, IDR_CURVEL>
{
public:
   virtual CString GetName() const override { return _T("Curvel"); }
};
OBJECT_ENTRY_AUTO(CLSID_CurvelTool, CCurvelTool)


// {876DBB2E-ECF9-4439-8F8F-E09A486018DF}
DEFINE_GUID(CLSID_PGStableTool,
   0x876dbb2e, 0xecf9, 0x4439, 0x8f, 0x8f, 0xe0, 0x9a, 0x48, 0x60, 0x18, 0xdf);
class CPGStableTool : public CToolImpl<CPGStableTool, &CLSID_PGStableTool, IDR_PGSTABLETOOL, CPGStableDoc, CPGStableChildFrame, CPGStableTabView, CPGStableDocTemplate, IDR_PGSTABLE>
{
public:
   virtual CString GetName() const override { return _T("PGStable"); }
};
OBJECT_ENTRY_AUTO(CLSID_PGStableTool, CPGStableTool)


// {91F9BC47-B245-4E2E-B642-9F926C5A5E14}
DEFINE_GUID(CLSID_SpectraTool,
   0x91f9bc47, 0xb245, 0x4e2e, 0xb6, 0x42, 0x9f, 0x92, 0x6c, 0x5a, 0x5e, 0x14);
class CSpectraTool : public CToolImpl<CSpectraTool, &CLSID_SpectraTool, IDR_SPECTRATOOL, CSpectraDoc, CSpectraChildFrame, CSpectraRptView, CSpectraDocTemplate, IDR_SPECTRA>
{
public:
   virtual CString GetName() const override { return _T("Spectra"); }
};
OBJECT_ENTRY_AUTO(CLSID_SpectraTool, CSpectraTool)
