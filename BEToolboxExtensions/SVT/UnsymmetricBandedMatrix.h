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
