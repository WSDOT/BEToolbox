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
#include "UnsymmetricBandedMatrix.h"

/////////////////////////////////////////
/// Computes the St. Venant torisional constant using the Prandtl membrane analogy
/// \f$ f(x) = a + b \f$
/// Some other text
/// @f[
/// \int_a^b f(x) dx = F(b) - F(a)
/// @f]
/////////////////////////////////////////
class PrandtlMembrane
{
public:
   PrandtlMembrane();
   ~PrandtlMembrane();

   /// Computes the torsional constant, J
   Float64 ComputeJ(const UniformFDMesh& mesh,Float64** ppValues=nullptr) const;

private:
   void BuildMatrix(const UniformFDMesh& mesh, mathUnsymmetricBandedMatrix& matrix) const; ///< Builds the finite difference system of equations
   static void BuildMatrixRow(IndexType startMeshRowIdx, IndexType endMeshRowIdx, const UniformFDMesh& mesh, mathUnsymmetricBandedMatrix& matrix); ///< Builds an individual row in the matrix, called from multiple threads
   static Float64 ComputeVolume(IndexType startElementIdx, IndexType endElementIdx, const UniformFDMesh& mesh, const Float64* meshValues); ///< Computes the volume under the membrane for a range of elements, called from multiple threads
};
