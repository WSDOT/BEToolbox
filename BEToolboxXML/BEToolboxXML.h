///////////////////////////////////////////////////////////////////////
// BEToolboxXML
// Copyright © 1999-2018  Washington State Department of Transportation
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

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the BETOOLBOXXML_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// BETOOLBOXXML_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#pragma once

#if defined (BUILDBETOOLBOXXMLLIB) && !defined(BETOOLBOXXMLLIB)
#define BETOOLBOXXMLCLASS __declspec(dllexport)
#define BETOOLBOXXMLFUNC  __declspec(dllexport)
#define BETOOLBOXXMLTPL   template class BETOOLBOXXMLCLASS
#elif defined(BETOOLBOXXMLLIB)
#define BETOOLBOXXMLCLASS
#define BETOOLBOXXMLFUNC
#define BETOOLBOXXMLTPL
#else
#define BETOOLBOXXMLCLASS __declspec(dllimport)
#define BETOOLBOXXMLFUNC
#define BETOOLBOXXMLTPL   extern template class BETOOLBOXXMLCLASS
#endif

#include "AutoLib.h"
