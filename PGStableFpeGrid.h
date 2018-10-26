///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2017  Washington State Department of Transportation
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

#include "PGStableStrands.h"

// PGStableFpeGrid.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPGStableFpeGrid window

class CPGStableFpeGrid : public CGXGridWnd
{
	//GRID_DECLARE_REGISTER()
// Construction
public:
	CPGStableFpeGrid();

// Attributes
public:

// Operations
public:
   void DoDataExchange(CDataExchange* pDX);

   void OnUnitsModeChanged();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPGStableFpeGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPGStableFpeGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPGStablePointLoadGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

   void InsertFpe(Float64 X,Float64 FpeStraight,Float64 YpsStraight,int YpsStraightMeasure,Float64 FpeHarped,Float64 YpsHarped,int YpsHarpedMeasure,Float64 FpeTemp,Float64 YpsTemp,int YpsTempMeasure);

   // virtual overrides for grid
   virtual BOOL OnValidateCell(ROWCOL nRow, ROWCOL nCol) override;

   void UpdateColumnHeaders();

   // get a cell value whether is is selected or not
   CString GetCellValue(ROWCOL nRow, ROWCOL nCol);

public:
   // custom stuff for grid
   void CustomInit();
   void AddFpe();

   void GetFpe(ROWCOL row,Float64* pX,Float64* pFpeStraight,Float64* pYpsStraight,int* pYpsStraightMeasure,Float64* pFpeHarped,Float64* pYpsHarped,int* pYpsHarpedMeasure,Float64* pFpeTemp,Float64* pYpsTemp,int* pYpsTempMeasure);
   void RemoveSelected();
   BOOL AreSelected();

   void FillGrid(const std::set<CPGStableFpe>& vFpe);
   void GetFpe(std::set<CPGStableFpe>& vFpe);



#if defined _DEBUG
   void AssertValid() const;
#endif
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.
