///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2025  Washington State Department of Transportation
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

// PGStableGirderSectionGrid.h : header file
//

struct StressPoints
{
   WBFL::Stability::Point pntTL[2];
   WBFL::Stability::Point pntTR[2];
   WBFL::Stability::Point pntBL[2];
   WBFL::Stability::Point pntBR[2];
};

/////////////////////////////////////////////////////////////////////////////
// CPGStableGirderSectionGrid window

class CPGStableGirderSectionGrid : public CGXGridWnd
{
	//GRID_DECLARE_REGISTER()
// Construction
public:
	CPGStableGirderSectionGrid();

// Attributes
public:

// Operations
public:
   void DoDataExchange(CDataExchange* pDX);

   std::vector<std::pair<Float64,Float64>> GetGirderProfile();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPGStableGirderSectionGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPGStableGirderSectionGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPGStableGirderSectionGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

   void InsertGirderSection(Float64 Length,Float64 Ag,Float64 Ixx,Float64 Iyy,Float64 Ixy,Float64 Xleft,Float64 Ytop,Float64 Hg,Float64 Wtf,Float64 Wbf,const WBFL::Stability::Point& pntTL,const WBFL::Stability::Point& pntTR,const WBFL::Stability::Point& pntBL,const WBFL::Stability::Point& pntBR);
   void SetStressPoints(ROWCOL row, const WBFL::Stability::Point& pntTL, const WBFL::Stability::Point& pntTR, const WBFL::Stability::Point& pntBL, const WBFL::Stability::Point& pntBR);

   // virtual overrides for grid
   virtual BOOL OnValidateCell(ROWCOL nRow, ROWCOL nCol) override;
   virtual void OnModifyCell(ROWCOL nRow,ROWCOL nCol) override;
   virtual BOOL OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt) override;

   void UpdateColumnHeaders();

   // get a cell value whether is is selected or not
   CString GetCellValue(ROWCOL nRow, ROWCOL nCol);

   void SelectRow(ROWCOL nRow);

public:
   // custom stuff for grid
   void CustomInit();
   void AddGirderSection();

   void GetGirderSection(ROWCOL row,Float64* pL,Float64* pAg,Float64* pIxx,Float64* pIyy,Float64* pIxy,Float64* pXcg,Float64* pYcg,Float64* pHg,Float64* pWtop,Float64* pWbot, WBFL::Stability::Point* pntTL, WBFL::Stability::Point* pntTR, WBFL::Stability::Point* pntBL, WBFL::Stability::Point* pntBR);
   void RemoveSelectedGirderSections();
   BOOL AreGirderSectionsSelected();

   void FillGrid(const WBFL::Stability::Girder& girder);
   void GetGirderSections(WBFL::Stability::Girder& girder);

   Float64 GetGirderLength();

   std::vector<StressPoints> GetStressPoints();
   void SetStressPoints(const std::vector<StressPoints>& vStressPoints);

   void EnableStressPoints(BOOL bEnable);
   void ComputeStressPoints();

   void OnUnitsChanged();

#if defined _DEBUG
   void AssertValid() const;
#endif
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.
