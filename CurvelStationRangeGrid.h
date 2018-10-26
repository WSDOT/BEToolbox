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

#include <WBFLCogo.h>

// StationRangeGrid.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCurvelStationRangeGrid window

class CCurvelStationRangeGrid : public CGXGridWnd
{
	//GRID_DECLARE_REGISTER()
// Construction
public:
	CCurvelStationRangeGrid();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCurvelStationRangeGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCurvelStationRangeGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCurvelStationRangeGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

   void InsertStation(const StationRange& station);

   // virtual overrides for grid
   virtual BOOL OnValidateCell(ROWCOL nRow, ROWCOL nCol) override;
   virtual BOOL OnEndEditing(ROWCOL nRow,ROWCOL nCol) override;

   void UpdateColumnHeaders();

   CComPtr<IStation> m_objStation;

public:
   // custom stuff for grid
   void CustomInit();

   void CorrectForSuperelevation(bool bCorrect);

   void SetStations(const std::vector<StationRange>& stations);
   std::vector<StationRange> GetStations();

   void AddStation();
   void GetStation(ROWCOL row,StationRange& station);
   void RemoveSelectedStations();
   BOOL AreStationsSelected();

   // get a cell value whether is is selected or not
   CString GetCellValue(ROWCOL nRow, ROWCOL nCol);


#if defined _DEBUG
   void AssertValid() const;
#endif
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.
