///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2019  Washington State Department of Transportation
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
#include "FDMeshGenerator.h"
#include <WBFLGeometry.h>
#include <MathEx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

FDMeshGenerator::FDMeshGenerator()
{
}

FDMeshGenerator::FDMeshGenerator(Float64 dxMax, Float64 dyMax) :
   m_DxMax(dxMax), m_DyMax(dyMax)
{
}

void FDMeshGenerator::Initialize(Float64 dxMax, Float64 dyMax)
{
   m_DxMax = dxMax;
   m_DyMax = dyMax;
}

//////////////////////////////////////////
/// \param[in] pShape the shape to generate the mesh for
/// \param[in] mesh the mesh to be generated
/// \param[in] bIgnoreSymmetry if true, symmetry of the shape is ignored and a full mesh is generated
void FDMeshGenerator::GenerateMesh(IShape* pShape, UniformFDMesh& mesh, bool bIgnoreSymmetry) const
{
   CComPtr<IRect2d> bbox;
   pShape->get_BoundingBox(&bbox);
   Float64 width, height;
   bbox->get_Width(&width);
   bbox->get_Height(&height);

   CComPtr<IShapeProperties> props;
   pShape->get_ShapeProperties(&props);
   Float64 Ixy;
   props->get_Ixy(&Ixy);
   if (IsZero(Ixy) && !bIgnoreSymmetry)
   {
      // shape is symmetric (assume about the vertical axis)
      width /= 2;
      mesh.HasSymmetry(true);
   }

   IndexType Nx = IndexType(width / m_DxMax);
   IndexType Ny = IndexType(height / m_DyMax);

   if (mesh.HasSymmetry() && ::IsOdd(Nx))
   {
      // for a symmetric mesh the rows must have an even number of elements
      Nx++;
   }

   Float64 Dx = width / Nx;
   Float64 Dy = height / Ny;

   mesh.SetElementSize(Dx, Dy);

   CComPtr<IPoint2d> pntTopLeft;
   bbox->get_TopLeft(&pntTopLeft);
   Float64 tlx, tly;
   pntTopLeft->Location(&tlx, &tly);

   mesh.AllocateElementRows(Ny); // preallocate the rows so we can add them in any order

   // we could do this in multiple threads, however our COM implementation
   // doesn't support it
   for (IndexType row = 0; row < Ny; row++)
   {
      GenerateMeshRow(row, Nx, Dx, Dy, tlx, tly, pShape, mesh);
   }
}

void FDMeshGenerator::GenerateMeshRow(IndexType row, IndexType Nx, Float64 dx, Float64 dy, Float64 tlx, Float64 tly, IShape* pShape, UniformFDMesh& mesh)
{
   Float64 cy = tly - row*dy - dy / 2;

   CComPtr<IPoint2d> pnt;
   pnt.CoCreateInstance(CLSID_Point2d);
   IndexType startElementIdx = INVALID_INDEX;
   for (IndexType col = 0; col < Nx; col++)
   {
      Float64 cx = tlx + col*dx + dx / 2;
      pnt->Move(cx, cy);
      VARIANT_BOOL bContainsPoint;
      pShape->PointInShape(pnt, &bContainsPoint);
      if (bContainsPoint == VARIANT_TRUE)
      {
         // found the element where the row starts
         startElementIdx = col;
         break;
      }
   }

   if (startElementIdx == INVALID_INDEX)
   {
      // The row doesn't have any elements that meet the meshing criteria
      // however, the row can't be empty. add one element
      mesh.AddElements(row, Nx - 1 , 1);
      return;
   }

   for (IndexType col = Nx - 1; col >= startElementIdx; col--)
   {
      Float64 cx = tlx + col*dx + dx / 2;
      pnt->Move(cx, cy);
      VARIANT_BOOL bContainsPoint;
      pShape->PointInShape(pnt, &bContainsPoint);
      if (bContainsPoint == VARIANT_TRUE)
      {
         IndexType nElementsInRow = col - startElementIdx + 1;
         mesh.AddElements(row, startElementIdx, nElementsInRow);
         return;
      }
   }
}
