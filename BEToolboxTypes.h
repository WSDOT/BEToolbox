///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2014  Washington State Department of Transportation
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

struct IndividualStation
{
   Float64 Station;
   Float64 Offset;
};

struct StationRange
{
   Float64 StartStation;
   Float64 EndStation;
   IndexType nSpaces;
   Float64 Offset;
};

struct SuperelevationProfilePoint
{
   Float64 Station;
   Float64 LeftSlope;
   Float64 RightSlope;
};

struct SkewLine
{
   enum Type { RadialFromCrownLine, // 992 - all offsets are taken normal to and measured from the crown line
               AlongSkewFromLLine // 993 - all offsets, except crown line to L Line, are taken as being along the skew line measured from the L Line
   };
   Float64 Station;
   Type OffsetType;
   Float64 Offset; // Depends on Type
   std::_tstring strSkewAngle;
   Float64 Radius; 
   Float64 CrownOffset; // Always normal/radial to L Line
};
