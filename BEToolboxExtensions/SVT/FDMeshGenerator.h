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
