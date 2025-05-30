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

#include ".\Bearing\BearingDocTemplate.h"
#include ".\Bearing\BearingDoc.h"
#include ".\Bearing\BearingRptView.h"
#include ".\Bearing\BearingChildFrame.h"


class CBoxGdrTool : public CToolImpl<CBoxGdrDoc, CBoxGdrChildFrame, CBoxGdrRptView, CBoxGdrDocTemplate, IDR_BOXGDR>
{
public:
   virtual CString GetName() const override { return _T("BoxGdr"); }
};

class CGenCompTool : public CToolImpl<CGenCompDoc, CGenCompChildFrame, CGenCompRptView, CGenCompDocTemplate,IDR_GENCOMP>
{
public:
   virtual CString GetName() const override { return _T("GenComp"); }
};

class CGirCompTool : public CToolImpl<CGirCompDoc, CGirCompChildFrame, CGirCompRptView, CGirCompDocTemplate, IDR_GIRCOMP>
{
public:
   virtual CString GetName() const override { return _T("GirComp"); }
};

class CUltColTool : public CToolImpl<CUltColDoc, CUltColChildFrame, CUltColView, CUltColDocTemplate, IDR_ULTCOL>
{
public:
   virtual CString GetName() const override { return _T("UltCol"); }
};

class CCurvelTool : public CToolImpl<CCurvelDoc, CCurvelChildFrame, CCurvelRptView, CCurvelDocTemplate, IDR_CURVEL>
{
public:
   virtual CString GetName() const override { return _T("Curvel"); }
};

class CPGStableTool : public CToolImpl<CPGStableDoc, CPGStableChildFrame, CPGStableTabView, CPGStableDocTemplate, IDR_PGSTABLE>
{
public:
   virtual CString GetName() const override { return _T("PGStable"); }
};

class CSpectraTool : public CToolImpl<CSpectraDoc, CSpectraChildFrame, CSpectraRptView, CSpectraDocTemplate, IDR_SPECTRA>
{
public:
   virtual CString GetName() const override { return _T("Spectra"); }
};

class CBearingTool : public CToolImpl<CBearingDoc, CBearingChildFrame, CBearingRptView, CBearingDocTemplate, IDR_BEARING>
{
public:
   virtual CString GetName() const override { return _T("Bearing"); }
};
