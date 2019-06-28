#pragma once

#include "UniformFDMesh.h"
#include "UnsymmetricBandedMatrix.h"

class PrandtlMembrane
{
public:
   PrandtlMembrane();
   ~PrandtlMembrane();
   Float64 ComputeJ(const UniformFDMesh& mesh) const;

private:
   void GetThreadParameters(IndexType nElements, IndexType& nWorkerThreads, IndexType& nElementsPerThread) const;
   void BuildMatrix(const UniformFDMesh& mesh, mathUnsymmetricBandedMatrix& matrix) const;
   static void BuildMatrixRow(IndexType startMeshRowIdx, IndexType endMeshRowIdx, const UniformFDMesh& mesh, mathUnsymmetricBandedMatrix& matrix);
   static Float64 ComputeVolume(IndexType startElementIdx, IndexType endElementIdx, const UniformFDMesh& mesh, Float64* meshValues);
};
