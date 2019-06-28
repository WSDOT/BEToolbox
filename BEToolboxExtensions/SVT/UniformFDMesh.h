#pragma once
#include <array>
#include <vector>
#include <memory>

typedef struct FDMeshElement
{
   // access array with Corner enum
   enum Corner { TopLeft, TopRight, BottomRight, BottomLeft };
   std::array<IndexType, 4> Node{ INVALID_INDEX,INVALID_INDEX,INVALID_INDEX,INVALID_INDEX };
} FDMeshElement;

class UniformFDMesh
{
public:
   UniformFDMesh();
   UniformFDMesh(Float64 dx, Float64 dy);
   virtual ~UniformFDMesh();

   void HasSymmetry(bool bSymmetric);
   bool HasSymmetry() const;

   void AllocateElementRows(IndexType nRows);
   void SetElementSize(Float64 dx, Float64 dy);
   void GetElementSize(Float64* pdx, Float64* pdy) const;
   Float64 GetElementArea() const;

   void AddElements(IndexType gridRowIdx, IndexType gridRowStartIdx, IndexType nElements);
   void AddElementRow(IndexType gridRowStartIdx, IndexType nElements);
   IndexType GetElementRowCount() const;
   void GetElementRange(IndexType elementRowIdx, IndexType* pGridRowStartIdx, IndexType* pFirstElementIdx, IndexType* pLastElementIdx) const;
   IndexType GetElementCount() const;
   IndexType GetInteriorNodeCount() const;
   IndexType GetMaxIntriorNodesPerRow() const;
   const FDMeshElement* GetElement(IndexType elementIdx) const;
   const FDMeshElement* GetElementAbove(IndexType gridRowIdx, IndexType elementIdx) const;
   const FDMeshElement* GetElementBelow(IndexType gridRowIdx, IndexType elementIdx) const;

   void GetGridSize(IndexType* pNx, IndexType* pNy) const;

   void Dump(std::ostream& os) const;

protected:
   Float64 m_Dx, m_Dy; // element dimensions
   bool m_bIsSymmetric;

   struct ElementRow
   {
      IndexType gridRowStartIdx; // index in the grid row where the first element is located
      IndexType nElements; // number of elements in this row
      IndexType firstElementIdx; // global index of the first element in this row

      inline ElementRow() : gridRowStartIdx(INVALID_INDEX), nElements(INVALID_INDEX), firstElementIdx(INVALID_INDEX) {};
      inline ElementRow(IndexType a, IndexType b, IndexType c) : gridRowStartIdx(a), nElements(b), firstElementIdx(c) {};
      inline IndexType GetNextRowFirstElementIndex() const { return firstElementIdx + nElements; }
   };
   mutable std::vector<ElementRow> m_vElementRows;
   mutable std::vector<std::unique_ptr<FDMeshElement>> m_vElements;
   mutable IndexType m_Nx; // overall number of grid squares in a row
   mutable IndexType m_nMaxElementsPerRow; // maximum number of elements in a row. this is the overall width of the grid and defines the axis of symmetry if the mesh is symmetric
   mutable IndexType m_nMaxInteriorNodesPerRow; // maximum number of interior nodes in a row. bandwidth is equal to 2(max nodes per row)+1
   mutable IndexType m_nInteriorNodes; // number of interior nodes (this is the number of degrees of freedom in the FD model)

   mutable bool m_bIsDirty;
   void Update() const;
   void Clear();
   IndexType GetFirstElementIndex(IndexType gridRowIdx);
};
