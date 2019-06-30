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

//#define ROW_STORAGE

class mathUnsymmetricBandedMatrix
{
public:
   mathUnsymmetricBandedMatrix();
   mathUnsymmetricBandedMatrix(IndexType N, IndexType BW);
   mathUnsymmetricBandedMatrix(IndexType N, IndexType BW, Float64** ba, Float64* b);
   ~mathUnsymmetricBandedMatrix();
   void Initialize(IndexType N, IndexType BW);
   IndexType GetSize() const;
   IndexType GetBandwidth() const;
   void SetCoefficient(IndexType i, IndexType j, Float64 aij);
   Float64 GetCoefficient(IndexType i, IndexType j);
   void SetB(IndexType i, Float64 bi);
   Float64 GetB(IndexType i) const;
   Float64* Solve();

   Float64& operator()(IndexType i, IndexType j);
   Float64& operator[](IndexType i);

   void Dump(std::ostream& os, bool bFull) const;

private:
   IndexType N;
   IndexType BW;
   IndexType half_band_width;
   Float64** ba;
   Float64* b;

#if defined ROW_STORAGE
   inline void Full2Condensed(IndexType i, IndexType j, IndexType half_band_width, IndexType& m, IndexType& n) const
   {
      m = j - i + half_band_width;
      n = i;
   }
#else
   inline void Full2Condensed(IndexType i, IndexType j, IndexType half_band_width, IndexType& m, IndexType& n) const
   {
      m = i;
      n = j - i + half_band_width;
   }
#endif

   void ReduceRow(Float64 c, IndexType i, IndexType j, IndexType kStart, IndexType kEnd);

   void Clear();
   void DumpBanded(std::ostream& os) const;
   void DumpFull(std::ostream& os) const;
};
