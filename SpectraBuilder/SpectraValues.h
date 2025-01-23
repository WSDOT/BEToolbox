///////////////////////////////////////////////////////////////////////
// SpectraBuilder
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

#include <math.h>

#define MIN_LATITUDE 24.6
#define MAX_LATITUDE 50.0
#define INC_LATITUDE 0.05

#define MIN_LONGITUDE -125.0
#define MAX_LONGITUDE -65.0
#define INC_LONGITUDE 0.05

#define RANGE_LATITUDE 509 //((long)ceil(MAX_LATITUDE-MIN_LATITUDE)/INC_LATITUDE)+1)
#define RANGE_LONGITUDE 1201 // ((long)ceil(MAX_LONGITUDE-MIN_LONGITUDE)/INC_LONGITUDE)+1)

#define SPECTRAL_VALUE_COUNT 611309 //RANGE_LATITUDE*RANGE_LONGITUDE

#define SPECTRUM_MAX_PERIOD 4.0 // report spectrum duration
#define SPECTRUM_STEP_SIZE  0.2 // report spectrum data step size


typedef enum SpecificationType
{
   WSDOT_BDM = 0, // WSDOT Bridge Design Manual
   AASHTO_LRFD = 1, // AASHTO LRFD Bridge Design Specifications, 8th Edition
   AASHTO_SEISMIC = 2, // AASHTO Guide Specifications for LRFD Seismic Bridge Design, 2nd Edition, 2011
   nSpecTypes = 3  // this is always last
} SpecificationType;

typedef enum SiteClass
{
   scA,
   scB,
   scC,
   scD,
   scE,
   scF,
   nSiteClasses
} SiteClass;

typedef struct SpectralValues
{
   Float64 lng;
   Float64 lat;
   Float64 v1hz;
   Float64 v5hz;
   Float64 vpga;
} SpectralValues;

// returns the four indices into the spectral value vector that represent
// the four corners of the grid that surround lat,lng
//
// +-----------+
// | *(lat,lng)|
// |           |
// |           |
// |           |
// +-----------+
//
// Returns the incies of the four corners of the box that surround the point * defined by lat,lng
void GetSpectralValueIndies(Float64 lat,Float64 lng,long* i1,long* i2,long* i3,long* i4);

// returns the spectra values for a specified lat/lng
void GetSpectralValues(Float64 lat,Float64 lng,SpectralValues* pValues,Float64* pS1,Float64* pSs,Float64* pPGA);

