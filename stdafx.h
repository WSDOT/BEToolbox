///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2013  Washington State Department of Transportation
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

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#include <WBFLAll.h>

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <atlbase.h>
#include <atlcom.h>
#include <atlimage.h> // for CImage used in the chapter builder code

#include <grid\gxall.h>
#include <System\System.h>
#include <MathEx.h>
#include <ReportManager\ReportManager.h>
#include <Reporter\Reporter.h>
#include <MFCTools\MFCTools.h>
#include <UnitMgt\UnitMgt.h>
#include <EAF\EAFUtilities.h>

#include <WBFLRCCapacity.h>
#include <WBFLTools.h>
#include <WBFLUnitServer.h>
#include <WBFLSections.h>
#include <WBFLCogo.h>

#include "BEToolboxTypes.h"

// prior to WBFL Version 3.3.0 the _WBFL_VERSION macro did not exist
// we need it for conditional compilation so if it isn't defined
// define it here
#ifndef _WBFL_VERSION
#define _WBFL_VERSION 320
#endif
