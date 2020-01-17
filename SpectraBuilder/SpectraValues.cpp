///////////////////////////////////////////////////////////////////////
// SpectraBuilder
// Copyright © 1999-2020  Washington State Department of Transportation
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
#include "SpectraValues.h"
#include <WBFLGeometry.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void GetSpectralValueIndies(Float64 lat,Float64 lng,long* i1,long* i2,long* i3,long* i4)
{
   long k = (long)floor((MAX_LATITUDE - lat)/INC_LATITUDE);
   long l = (long)floor((lng - MIN_LONGITUDE)/INC_LONGITUDE);

   *i1 = k*RANGE_LONGITUDE + l;
   *i2 = k*RANGE_LONGITUDE + l+1;
   *i3 = (k+1)*RANGE_LONGITUDE + l;
   *i4 = (k+1)*RANGE_LONGITUDE + l+1;
}

void GetSpectralValues(Float64 lat,Float64 lng,SpectralValues* pValues,Float64* pS1,Float64* pSs,Float64* pPGA)
{
   long i1,i2,i3,i4;
   GetSpectralValueIndies(lat,lng,&i1,&i2,&i3,&i4);

#if defined _DEBUG
   // make sure we've got the right values.
   ATLASSERT(pValues[i3].lat <= lat && lat <= pValues[i1].lat);
   ATLASSERT(pValues[i4].lat <= lat && lat <= pValues[i2].lat);
   ATLASSERT(pValues[i1].lng <= lng && lng <= pValues[i2].lng);
   ATLASSERT(pValues[i3].lng <= lng && lng <= pValues[i4].lng);
#endif

   // Don't assume that all four points are co-planar
   // Assume we have two triangles and we need to determine
   // which triangle forms the plane we want to evaluate
   //
   //  1          2
   //   +--------+ 
   //   |       /|
   //   |      / |
   //   |     /  |
   //   |    /   |
   //   |   /    |
   //   |  /     |
   //   | /      |
   //   |/       |
   //   +--------+
   //  3          4
   //

   CComPtr<IGeomUtil2d> geomUtil;
   geomUtil.CoCreateInstance(CLSID_GeomUtil);

   CComPtr<IPoint2d> pnt;
   pnt.CoCreateInstance(CLSID_Point2d);
   pnt->Move(lng,lat);
   
   CComPtr<IPoint2d> pnt1, pnt2, pnt3, pnt4;
   pnt1.CoCreateInstance(CLSID_Point2d);
   pnt2.CoCreateInstance(CLSID_Point2d);
   pnt3.CoCreateInstance(CLSID_Point2d);
   pnt4.CoCreateInstance(CLSID_Point2d);

   pnt1->Move(pValues[i1].lng,pValues[i1].lat);
   pnt2->Move(pValues[i2].lng,pValues[i2].lat);
   pnt3->Move(pValues[i3].lng,pValues[i3].lat);
   pnt4->Move(pValues[i4].lng,pValues[i4].lat);

   CComPtr<IPoint3d> pntA, pntB, pntC;
   pntA.CoCreateInstance(CLSID_Point3d);
   pntB.CoCreateInstance(CLSID_Point3d);
   pntC.CoCreateInstance(CLSID_Point3d);

   VARIANT_BOOL bPntInTriangle;
   geomUtil->PointInTriangle(pnt,pnt1,pnt2,pnt3,&bPntInTriangle);
   if ( bPntInTriangle == VARIANT_TRUE )
   {
      pntA->Move(pValues[i1].lng,pValues[i1].lat,pValues[i1].v1hz);
      pntB->Move(pValues[i2].lng,pValues[i2].lat,pValues[i2].v1hz);
      pntC->Move(pValues[i3].lng,pValues[i3].lat,pValues[i3].v1hz);
   }
   else
   {
#if defined _DEBUG
      VARIANT_BOOL bResult;
      geomUtil->PointInTriangle(pnt,pnt2,pnt3,pnt4,&bResult);
      ATLASSERT(bResult == VARIANT_TRUE);
#endif
      pntA->Move(pValues[i2].lng,pValues[i2].lat,pValues[i2].v1hz);
      pntB->Move(pValues[i3].lng,pValues[i3].lat,pValues[i3].v1hz);
      pntC->Move(pValues[i4].lng,pValues[i4].lat,pValues[i4].v1hz);
   }

   CComPtr<IPlane3d> plane;
   plane.CoCreateInstance(CLSID_Plane3d);

   plane->ThroughPoints(pntA,pntB,pntC);
   plane->GetZ(lng,lat,pS1);

   if ( bPntInTriangle == VARIANT_TRUE )
   {
      pntA->Move(pValues[i1].lng,pValues[i1].lat,pValues[i1].v5hz);
      pntB->Move(pValues[i2].lng,pValues[i2].lat,pValues[i2].v5hz);
      pntC->Move(pValues[i3].lng,pValues[i3].lat,pValues[i3].v5hz);
   }
   else
   {
      pntA->Move(pValues[i2].lng,pValues[i2].lat,pValues[i2].v5hz);
      pntB->Move(pValues[i3].lng,pValues[i3].lat,pValues[i3].v5hz);
      pntC->Move(pValues[i4].lng,pValues[i4].lat,pValues[i4].v5hz);
   }
   plane->ThroughPoints(pntA,pntB,pntC);
   plane->GetZ(lng,lat,pSs);

   if ( bPntInTriangle == VARIANT_TRUE )
   {
      pntA->Move(pValues[i1].lng,pValues[i1].lat,pValues[i1].vpga);
      pntB->Move(pValues[i2].lng,pValues[i2].lat,pValues[i2].vpga);
      pntC->Move(pValues[i3].lng,pValues[i3].lat,pValues[i3].vpga);
   }
   else
   {
      pntA->Move(pValues[i2].lng,pValues[i2].lat,pValues[i2].vpga);
      pntB->Move(pValues[i3].lng,pValues[i3].lat,pValues[i3].vpga);
      pntC->Move(pValues[i4].lng,pValues[i4].lat,pValues[i4].vpga);
   }
   plane->ThroughPoints(pntA,pntB,pntC);
   plane->GetZ(lng,lat,pPGA);
}
