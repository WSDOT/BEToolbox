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

// PGStablePointLoadGrid.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPGStablePointLoadGrid window

class CPGStablePointLoadGrid : public CGXGridWnd
{
	//GRID_DECLARE_REGISTER()
// Construction
public:
	CPGStablePointLoadGrid();

// Attributes
public:

// Operations
public:
   virtual void DoDataExchange(CDataExchange* pDX) override;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPGStablePointLoadGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPGStablePointLoadGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPGStablePointLoadGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

   void InsertLoad(Float64 X,Float64 P);

   // virtual overrides for grid
   virtual BOOL OnValidateCell(ROWCOL nRow, ROWCOL nCol) override;

   void UpdateColumnHeaders();

   // get a cell value whether is is selected or not
   CString GetCellValue(ROWCOL nRow, ROWCOL nCol);

public:
   // custom stuff for grid
   void CustomInit();
   void AddLoad();

   void GetLoad(ROWCOL row,Float64* pX,Float64* pP);
   void RemoveSelectedLoads();
   BOOL AreLoadsSelected();

   void FillGrid(const std::vector<std::pair<Float64,Float64>>& vLoads);
   void GetLoads(std::vector<std::pair<Float64,Float64>>& vLoads);



#if defined _DEBUG
   void AssertValid() const;
#endif
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.
