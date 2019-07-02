///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright   1999-2019  Washington State Department of Transportation
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
#include "PrandtlMembrane.h"
#include "Helpers.h"
#include <future>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


PrandtlMembrane::PrandtlMembrane()
{
}

PrandtlMembrane::~PrandtlMembrane()
{
}

///
/// /param[in] mesh a finite difference mesh
/// /param[out] ppValues the finite difference slout
/// /return returns J
///
Float64 PrandtlMembrane::ComputeJ(const UniformFDMesh& mesh,Float64** ppValues) const
{
   IndexType nInteriorNodes = mesh.GetInteriorNodeCount();
   IndexType bw = 2 * mesh.GetMaxIntriorNodesPerRow() + 1;
   mathUnsymmetricBandedMatrix matrix(nInteriorNodes, bw);
   BuildMatrix(mesh, matrix);

   Float64* meshValues = matrix.Solve();

   auto nElements = mesh.GetElementCount();

   IndexType nWorkerThreads, nElementsPerThread;
   GetThreadParameters(nElements, nWorkerThreads, nElementsPerThread);

   std::vector<std::future<Float64>> vFutures;
   IndexType startElementIdx = 0;
   for (IndexType i = 0; i < nWorkerThreads; i++)
   {
      IndexType endElementIdx = startElementIdx + nElementsPerThread - 1;
      vFutures.emplace_back(std::async(&PrandtlMembrane::ComputeVolume, startElementIdx, endElementIdx, std::ref(mesh), meshValues));
      startElementIdx = endElementIdx + 1;
   }

   Float64 V = ComputeVolume(startElementIdx, nElements - 1, mesh, meshValues);
   for (auto& result : vFutures)
   {
      V += result.get();
   }

   if (mesh.HasSymmetry())
   {
      V *= 2;
   }


   if (ppValues)
   {
      *ppValues = meshValues;
   }
   else
   {
      delete[] meshValues;
   }

   return V;
}

void PrandtlMembrane::BuildMatrix(const UniformFDMesh& mesh, mathUnsymmetricBandedMatrix& matrix) const
{
   auto nElements = mesh.GetElementRowCount(); // this is the number of rows of elements
                                               // we need number of rows of nodes which is one more than the number of elements
                                               // however the top and bottom rows are boundary rows so we don't generate FD equations for them
                                               // nElements + 1 - 2 = nElements - 1
   nElements--;

   IndexType nWorkerThreads, nElementsPerThread;
   GetThreadParameters(nElements, nWorkerThreads, nElementsPerThread);

   IndexType startMeshRowIdx = 0;
   std::vector<std::future<void>> vFutures;
   for (IndexType i = 0; i < nWorkerThreads; i++)
   {
      IndexType endMeshRowIdx = startMeshRowIdx + nElementsPerThread - 1;
      vFutures.emplace_back(std::async(&PrandtlMembrane::BuildMatrixRow, startMeshRowIdx, endMeshRowIdx, std::ref(mesh), std::ref(matrix)));
      startMeshRowIdx = endMeshRowIdx + 1;
   }

   BuildMatrixRow(startMeshRowIdx, nElements - 1, mesh, matrix);
   for (auto& f : vFutures)
   {
      f.get();
   }
}

void PrandtlMembrane::BuildMatrixRow(IndexType startMeshRowIdx, IndexType endMeshRowIdx, const UniformFDMesh& mesh, mathUnsymmetricBandedMatrix& matrix)
{
   Float64 Dx, Dy;
   mesh.GetElementSize(&Dx, &Dy);
   Float64 R2 = pow(Dy / Dx, 2);
   Float64 K0 = 0.5*(1 + R2);
   Float64 K13 = -0.25;
   Float64 K24 = -0.25*R2;
   Float64 K24_Sym = 2 * K24;
   Float64 Dy2 = Dy*Dy;

   bool bIsSymmetric = mesh.HasSymmetry();
   IndexType symmetryIdx = 0;
   if (bIsSymmetric)
   {
      IndexType Nx, Ny;
      mesh.GetGridSize(&Nx, &Ny);
      symmetryIdx = Nx - 1;
   }

   for (IndexType meshRowIdx = startMeshRowIdx; meshRowIdx <= endMeshRowIdx; meshRowIdx++)
   {
      IndexType gridRowStartIdx, startElementIdx, endElementIdx;
      mesh.GetElementRange(meshRowIdx, &gridRowStartIdx, &startElementIdx, &endElementIdx);
      if (bIsSymmetric)
      {
         // the loop below doesn't cover the last element in the row because, for full grids
         // the right hand side of the last elements in a row are boundary nodes.
         // for meshes with a vertical axis of symmetry, the right hand side of the last
         // elements aren't boundaries. the loop must cover these elements so add one
         // to the end element
         endElementIdx++;
      }


      for (IndexType elementIdx = startElementIdx; elementIdx < endElementIdx; elementIdx++, gridRowStartIdx++)
      {
         const auto* pElement = mesh.GetElement(elementIdx);

         if (pElement->Node[FDMeshElement::BottomRight] != INVALID_INDEX)
         {
            matrix(pElement->Node[FDMeshElement::BottomRight], pElement->Node[FDMeshElement::BottomRight]) = K0;

            const auto* pBelowElement = mesh.GetElementBelow(meshRowIdx, elementIdx - startElementIdx);
            if (pBelowElement && pBelowElement->Node[FDMeshElement::BottomRight] != INVALID_INDEX)
            {
               matrix(pElement->Node[FDMeshElement::BottomRight], pBelowElement->Node[FDMeshElement::BottomRight]) = K13;
            }

            if (pElement->Node[FDMeshElement::BottomLeft] != INVALID_INDEX)
            {
               if (bIsSymmetric && gridRowStartIdx == symmetryIdx)
               {
                  matrix(pElement->Node[FDMeshElement::BottomRight], pElement->Node[FDMeshElement::BottomLeft]) = K24_Sym;
               }
               else
               {
                  matrix(pElement->Node[FDMeshElement::BottomRight], pElement->Node[FDMeshElement::BottomLeft]) = K24;
               }
            }

            if (pElement->Node[FDMeshElement::TopRight] != INVALID_INDEX)
            {
               matrix(pElement->Node[FDMeshElement::BottomRight], pElement->Node[FDMeshElement::TopRight]) = K13;
            }

            if (!bIsSymmetric || gridRowStartIdx != symmetryIdx)
            {
               const auto* pNextElement = mesh.GetElement(elementIdx + 1);
               if (pNextElement->Node[FDMeshElement::BottomRight] != INVALID_INDEX)
               {
                  matrix(pElement->Node[FDMeshElement::BottomRight], pNextElement->Node[FDMeshElement::BottomRight]) = K24;
               }
            }

            matrix[pElement->Node[FDMeshElement::BottomRight]] = Dy2;
         }
      }
   }
}

Float64 PrandtlMembrane::ComputeVolume(IndexType startElementIdx, IndexType endElementIdx, const UniformFDMesh& mesh, Float64* meshValues)
{
   Float64 V = 0;
   Float64 area = mesh.GetElementArea();
   static std::array<Float64, 5> area_factor{ 0, 1. / 3., 0.5, 5. / 6., 1.0 };
   for (IndexType elementIdx = startElementIdx; elementIdx <= endElementIdx; elementIdx++)
   {
      const auto* pElement = mesh.GetElement(elementIdx);
      IndexType nInteriorNodes = 0;
      Float64 sum_values = 0;
      for (long i = 0; i < 4; i++)
      {
         if (pElement->Node[i] != INVALID_INDEX)
         {
            sum_values += meshValues[pElement->Node[i]];
            nInteriorNodes++;
         }
      }

      // if there are no interior nodes, all nodes are on the boundary and
      // there values are zero so no contribution to the volume under the membrane
      if (nInteriorNodes != 0)
      {
         Float64 avg_value = sum_values / nInteriorNodes;
         Float64 dV = area_factor[nInteriorNodes] * area * avg_value;
         V += dV;
      }
   }
   return V;
}
