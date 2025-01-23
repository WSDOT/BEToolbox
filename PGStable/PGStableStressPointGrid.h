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

// PGStableStressPointGrid.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPGStableStressPointGrid window

class CPGStableStressPointGrid : public CGXGridWnd
{
	//GRID_DECLARE_REGISTER()
// Construction
public:
   CPGStableStressPointGrid();

// Attributes
public:

// Operations
public:
   void DoDataExchange(CDataExchange* pDX);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPGStableStressPointGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPGStableStressPointGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPGStableStressPointGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

   void SetStressPoint(ROWCOL row, Float64 x, Float64 y);
   void GetStressPoint(ROWCOL row, Float64* pX, Float64* pY);
   Float64 GetCellValue(ROWCOL nRow, ROWCOL nCol);

   void UpdateColumnHeaders();

public:
   // custom stuff for grid
   void CustomInit();

   void Enable(BOOL bEnable);

   void SetTopLeft(Float64 x, Float64 y);
   void GetTopLeft(Float64* pX, Float64* pY);

   void SetTopRight(Float64 x, Float64 y);
   void GetTopRight(Float64* pX, Float64* pY);

   void SetBottomLeft(Float64 x, Float64 y);
   void GetBottomLeft(Float64* pX, Float64* pY);

   void SetBottomRight(Float64 x, Float64 y);
   void GetBottomRight(Float64* pX, Float64* pY);

   void OnUnitsChanged();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.
