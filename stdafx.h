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

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#include <cctype>

#include <WBFLAll.h>

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <atlbase.h>
#include <atlcom.h>
#include <atlimage.h> // for CImage used in the chapter builder code
#include <afxext.h>
#include <afxdisp.h>
#include <afxole.h>

#if defined _NOGRID
#include <NoGrid.h>
#else
#include <grid\gxall.h>
#endif

#include <System\System.h>
#include <MathEx.h>
#include <ReportManager\ReportManager.h>
#include <Reporter\Reporter.h>
#include <MFCTools\MFCTools.h>
#include <Units\Units.h>
#include <EAF\EAFUtilities.h>
#include <EAF\EAFHelp.h>
#include <EAF\EAFResources.h>
#include <EAF\EAFHints.h>
#include <EAF\EAFCustSiteVars.h>

#include <WBFLSTL.h>
#include <WBFLRCCapacity.h>
#include <WBFLTools.h>
#include <WBFLUnitServer.h>
#include <WBFLCogo.h>
#include <WBFLAtlExt.h>

#include "BEToolboxTypes.h"
#include "BEToolboxColors.h"
#include "BEToolboxVersion.h"

#include <afxwin.h>

#include <BEToolbox.hh>
#include <afxext.h>

#include <IFace\Tools.h>

#include <PGSuperTypes.h>
#include <PgsExt\LongitudinalRebarData.h>

#include <Units\Units.h>
#include <Stability\Stability.h>

#include "SpectraBuilder\SpectraValues.h"

#include <WBFLUnitServer.hxx>
#include <BEToolboxXML.h>
