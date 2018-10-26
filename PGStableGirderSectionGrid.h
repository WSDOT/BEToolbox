///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2018  Washington State Department of Transportation
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

   void InsertGirderSection(Float64 Length,Float64 Ag,Float64 Ix,Float64 Iy,Float64 Yt,Float64 Hg,Float64 Wtf,Float64 Wbf);

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

   void GetGirderSection(ROWCOL row,Float64* pL,Float64* pAg,Float64* pIx,Float64* pIy,Float64* pYt,Float64* pHg,Float64* pWtop,Float64* pWbot);
   void RemoveSelectedGirderSections();
   BOOL AreGirderSectionsSelected();

   void FillGrid(const stbGirder& girder);
   void GetGirderSections(stbGirder& girder);

   Float64 GetGirderLength();



#if defined _DEBUG
   void AssertValid() const;
#endif
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.
