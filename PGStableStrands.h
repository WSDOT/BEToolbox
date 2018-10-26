///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2016  Washington State Department of Transportation
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

#define TOP 0
#define BOTTOM 1

#define DISTANCE 0
#define FRACTION 1

class CPGStableStrands
{
public:
   CPGStableStrands();
   bool operator==(const CPGStableStrands& other) const;
   bool operator!=(const CPGStableStrands& other) const;

   HRESULT Save(IStructuredSave* pStrSave);
   HRESULT Load(IStructuredLoad* pStrLoad);

   Float64 XferLength; // prestress transfer length... used only with constant Fpe

   Float64 Ys;
   int YsMeasure;

   Float64 Yt;
   int YtMeasure;

   Float64 Xh1,Yh1;
   int Xh1Measure,Yh1Measure;

   Float64 Xh2,Yh2;
   int Xh2Measure,Yh2Measure;

   Float64 Xh3,Yh3;
   int Xh3Measure,Yh3Measure;

   Float64 Xh4,Yh4;
   int Xh4Measure,Yh4Measure;
};
