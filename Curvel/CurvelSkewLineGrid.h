///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2022  Washington State Department of Transportation
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

#include <WBFLCogo.h>

// CurvelSkewLineGrid.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCurvelSkewLineGrid window

class CCurvelSkewLineGrid : public CGXGridWnd
{
	//GRID_DECLARE_REGISTER()
// Construction
public:
	CCurvelSkewLineGrid();

// Attributes
public:

// Operations
public:
   
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCurvelSkewLineGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCurvelSkewLineGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCurvelIndividualStationGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

   void InsertSkewLine(const SkewLine& skewLine);

   // virtual overrides for grid
   virtual BOOL OnValidateCell(ROWCOL nRow, ROWCOL nCol) override;
   virtual BOOL OnEndEditing(ROWCOL nRow,ROWCOL nCol) override;

   void UpdateColumnHeaders();

   CComPtr<IStation> m_objStation;
   CComPtr<IAngle> m_objAngle;

public:
   // custom stuff for grid
   void CustomInit();

   void CorrectForSuperelevation(bool bCorrect);

   void SetSkewLines(const std::vector<SkewLine>& skewLine);
   std::vector<SkewLine> GetSkewLines();

   void AddSkewLine();
   void GetSkewLine(ROWCOL row,SkewLine& skewLine);
   void RemoveSelectedSkewLines();
   BOOL AreSkewLinesSelected();

   // get a cell value whether is is selected or not
   CString GetCellValue(ROWCOL nRow, ROWCOL nCol);


#if defined _DEBUG
   void AssertValid() const;
#endif
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.
