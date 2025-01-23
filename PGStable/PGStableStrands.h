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
#include "..\BEToolboxLib.h"

#define TOP 0
#define BOTTOM 1

#define DISTANCE 0
#define FRACTION 1

class BETCLASS CPGStableFpe
{
public:
   CPGStableFpe();
   CPGStableFpe(Float64 X,Float64 FpeStraight,Float64 XpsStraight,Float64 YpsStraight,int YpsStraightMeasure,Float64 FpeHarped,Float64 XpsHarped,Float64 YpsHarped,int YpsHarpedMeasure,Float64 FpeTemp,Float64 XpsTemp,Float64 YpsTemp,int YpsTempMeasure);

   bool operator<(const CPGStableFpe& other) const;
   bool operator==(const CPGStableFpe& other) const;

   Float64 X;
   Float64 FpeStraight;
   Float64 XpsStraight;
   Float64 YpsStraight;
   int     YpsStraightMeasure;

   Float64 FpeHarped;
   Float64 XpsHarped;
   Float64 YpsHarped;
   int     YpsHarpedMeasure;

   Float64 FpeTemp;
   Float64 XpsTemp;
   Float64 YpsTemp;
   int     YpsTempMeasure;
};

class BETCLASS CPGStableStrands
{
public:
   typedef enum StrandMethod
   {
      Simplified, // location of strands are defined using the simplified demensions below
      Detailed // the exact location of the prestress force is give in the Fpe definition
   } StrandMethod;

   CPGStableStrands();
   bool operator==(const CPGStableStrands& other) const;
   bool operator!=(const CPGStableStrands& other) const;

   HRESULT Save(IStructuredSave* pStrSave);
   HRESULT Load(IStructuredLoad* pStrLoad);

   StrandMethod strandMethod;

   std::set<CPGStableFpe> m_vFpe; // used if strandMethod == Exact, otherwise all the parameters below are used

   // Parameters for simplified strand definition

   Float64 XferLength; // prestress transfer length... used only with constant Fpe

   Float64 ex; // lateral eccentricity of the prestressing with respect to the vertical centriodal axis (< 0 is the left of the CG in girder section coordinates)

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

   Float64 FpeStraight, FpeHarped, FpeTemp;
};
