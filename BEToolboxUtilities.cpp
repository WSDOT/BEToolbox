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

#include "stdafx.h"
#include "BEToolboxUtilities.h"
#include <EAF\EAFApp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


Float64 GetOffset(CString strOffset,const WBFL::Units::LengthData& displayUnit)
{
   strOffset.MakeUpper();
   strOffset.Trim();
   TCHAR cDir = (strOffset.GetLength() == 0 ? 'R' : strOffset.GetAt(strOffset.GetLength()-1));
   Float64 sign = (cDir == 'L' ? -1 : 1);
   Float64 value = _tstof(strOffset);
   value *= sign;
   value = WBFL::Units::ConvertToSysUnits(value,displayUnit.UnitOfMeasure);
   return value;
}

CString GetOffset(Float64 value,const WBFL::Units::LengthData& displayUnit)
{
   CString strOffset = FormatOffset(value,displayUnit.UnitOfMeasure,false);
   return strOffset;
}