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

#pragma once

#include "UniformFDMesh.h"

interface IShape;

class FDMeshGenerator
{
public:
   FDMeshGenerator();
   FDMeshGenerator(Float64 dxMax, Float64 dyMax);
   void Initialize(Float64 dxMax, Float64 dyMax);
   void GenerateMesh(IShape* pShape, UniformFDMesh& mesh, bool bIgnoreSymmetry = false) const;

private:
   Float64 m_DxMax, m_DyMax;
   static void GenerateMeshRow(IndexType rowIdx, IndexType Nx, Float64 dx, Float64 dy, Float64 tlx, Float64 tly, IShape* pShape, UniformFDMesh& mesh);
};
