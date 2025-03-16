///////////////////////////////////////////////////////////////////////
// PGSuper - Prestressed Girder SUPERstructure Design and Analysis
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
//
#include "PGStableLongitudinalRebarData.h"

/////////////////////////////////////////////////////////////////////////////
// CPGStableLongRebarGrid window

class CPGStableLongRebarGrid : public CGXGridWnd
{
	GRID_DECLARE_REGISTER()
// Construction
public:
	CPGStableLongRebarGrid();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPGStableLongRebarGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPGStableLongRebarGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPGStableLongRebarGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

   // virtual overrides for grid
   virtual int GetColWidth(ROWCOL nCol) override;
   virtual BOOL OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt) override;
   virtual BOOL OnValidateCell(ROWCOL nRow, ROWCOL nCol) override;
   virtual void OnModifyCell(ROWCOL nRow,ROWCOL nCol) override;
   afx_msg void OnKillFocus(CWnd* pNewWnd);

   virtual BOOL SetCurrentCell(ROWCOL nRow, ROWCOL nCol, UINT flags = GX_SCROLLINVIEW | GX_UPDATENOW) override;
   // Use value below to prevent validation messages from being shown multiple times when a cell is invalid
   int m_CurrCellFocuses = 0;

public:
   // custom stuff for grid
   void CustomInit();
   // insert a row above the currently selected cell or at the top if no selection
   void Insertrow(WBFL::Materials::Rebar::Type type, WBFL::Materials::Rebar::Grade grade);
   void Removerows();

   // fill grid with data
   void FillGrid(const CPGStableLongitudinalRebarData& rebarData);
   bool GetRebarData(CDataExchange* pDX, CPGStableLongitudinalRebarData* pRebarData);

   BOOL ValidateAllCells(bool showWarningText);
   bool m_DoSetWarningText = true;

private:
   // get a cell value whether is is selected or not
   CString GetCellValue(ROWCOL nRow, ROWCOL nCol);
   // get data for a row
   bool GetRowData(CDataExchange* pDX, ROWCOL nRow, CPGStableLongitudinalRebarData::RebarRow* plsi);

   // set up styles for interior rows
   void SetRowStyle(ROWCOL nRow, WBFL::Materials::Rebar::Type type, WBFL::Materials::Rebar::Grade grade);

   WBFL::Materials::Rebar::Size GetBarSize(ROWCOL row);
   void EnableCell(ROWCOL nRow, ROWCOL nCol, BOOL bEnable);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

