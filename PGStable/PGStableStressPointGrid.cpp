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

// PGStableStressPointGrid.cpp : implementation file
//

#include "stdafx.h"
#include "..\resource.h"
#include "PGStableStressPointGrid.h"
#include <EAF\EAFUtilities.h>
#include <EAF\EAFApp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//GRID_IMPLEMENT_REGISTER(CPGStableStressPointGrid, CS_DBLCLKS, 0, 0, 0);

/////////////////////////////////////////////////////////////////////////////
// CPGStableStressPointGrid

CPGStableStressPointGrid::CPGStableStressPointGrid()
{
//   RegisterClass();
}

CPGStableStressPointGrid::~CPGStableStressPointGrid()
{
}

BEGIN_MESSAGE_MAP(CPGStableStressPointGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CPGStableStressPointGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CPGStableStressPointGrid::DoDataExchange(CDataExchange* pDX)
{
   //CBoxGdrDoc* pDoc = (CBoxGdrDoc*)EAFGetDocument();
   //if ( pDX->m_bSaveAndValidate )
   //{
   //   std::vector<BOXGDRDIMENSIONS> problems;
   //   ROWCOL nRows = GetRowCount();
   //   for ( ROWCOL row = 0; row < nRows; row++ )
   //   {
   //      BOXGDRDIMENSIONS dimensions;
   //      GetProblemData(row+1,dimensions);
   //      problems.push_back(dimensions);
   //   }

   //   pDoc->ClearProblems();
   //   pDoc->AddProblems(problems);
   //}
   //else
   //{
   //   if ( 0 < GetRowCount() )
   //      RemoveRows(1,GetRowCount());

   //   IndexType nProblems = pDoc->GetProblemCount();
   //   for ( IndexType idx = 0; idx < nProblems; idx++ )
   //   {
   //      const BOXGDRDIMENSIONS& problem = pDoc->GetProblem(idx);
   //      InsertRow(problem);
   //   }
   //}
}

/////////////////////////////////////////////////////////////////////////////
// CPGStableStressPointGrid message handlers

void CPGStableStressPointGrid::CustomInit()
{
   // Initialize the grid. For CWnd based grids this call is // 
   // essential. For view based grids this initialization is done 
   // in OnInitialUpdate.
	Initialize( );

	GetParam( )->EnableUndo(FALSE);

   const ROWCOL num_rows = 0;
   const ROWCOL num_cols = 2;

	SetRowCount(num_rows);
	SetColCount(num_cols);

		// Turn off selecting whole columns when clicking on a column header
	GetParam()->EnableSelection((WORD) (GX_SELFULL & ~GX_SELCOL & ~GX_SELTABLE));

   // disable left side
	SetStyleRange(CGXRange(0,0,num_rows,0), CGXStyle()
			.SetControl(GX_IDS_CTRL_HEADER)
			.SetEnabled(FALSE)          // disables usage as current cell
		);


   UpdateColumnHeaders();


   InsertRows(1, 4);
   SetStyleRange(CGXRange(1, 0), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_LEFT)
      .SetVerticalAlignment(DT_VCENTER)
      .SetEnabled(FALSE)          // disables usage as current cell
      .SetValue(_T("Top Left"))
   );

   SetStyleRange(CGXRange(2, 0), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_LEFT)
      .SetVerticalAlignment(DT_VCENTER)
      .SetEnabled(FALSE)          // disables usage as current cell
      .SetValue(_T("Top Right"))
   );

   SetStyleRange(CGXRange(3, 0), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_LEFT)
      .SetVerticalAlignment(DT_VCENTER)
      .SetEnabled(FALSE)          // disables usage as current cell
      .SetValue(_T("Bottom Left"))
   );

   SetStyleRange(CGXRange(4, 0), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_LEFT)
      .SetVerticalAlignment(DT_VCENTER)
      .SetEnabled(FALSE)          // disables usage as current cell
      .SetValue(_T("Bottom Right"))
   );

   // make it so that text fits correctly in header row
   ResizeColWidthsToFit(CGXRange(0, 0, 4, GetColCount()));
   ResizeRowHeightsToFit(CGXRange(0, 0, 0, GetColCount()));

   // don't allow users to resize grids
   GetParam( )->EnableTrackColWidth(0); 
   GetParam( )->EnableTrackRowHeight(0); 

	SetFocus();

	GetParam( )->EnableUndo(TRUE);

//   EnableIntelliMouse();
}

void CPGStableStressPointGrid::UpdateColumnHeaders()
{
   GetParam()->EnableUndo(FALSE);

   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   // set text along top row
   ROWCOL col = 1;
   CString strDimension;
   strDimension.Format(_T("%s (%s)"), _T("X"), pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
   SetStyleRange(CGXRange(0, col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
      .SetEnabled(FALSE)          // disables usage as current cell
      .SetValue(strDimension)
   );

   strDimension.Format(_T("%s (%s)"), _T("Y"), pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
   SetStyleRange(CGXRange(0, col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
      .SetEnabled(FALSE)          // disables usage as current cell
      .SetValue(strDimension)
   );

   ResizeColWidthsToFit(CGXRange(0, 0, GetRowCount(), GetColCount()));

   GetParam()->EnableUndo(TRUE);
}

void CPGStableStressPointGrid::Enable(BOOL bEnable)
{
   GetParam()->EnableUndo(FALSE);
   GetParam()->SetLockReadOnly(FALSE);

   CGXStyle style;
   if (bEnable)
   {
      style.SetReadOnly(FALSE).SetEnabled(TRUE).SetInterior(::GetSysColor(COLOR_WINDOW)).SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
   }
   else
   {
      style.SetReadOnly(TRUE).SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)).SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
   }

   SetStyleRange(CGXRange(1, 1, 4, 2), style);

   GetParam()->EnableUndo(TRUE);
   GetParam()->SetLockReadOnly(TRUE);
}

void CPGStableStressPointGrid::SetTopLeft(Float64 x, Float64 y)
{
   SetStressPoint(1, x, y);
}

void CPGStableStressPointGrid::GetTopLeft(Float64* pX, Float64* pY)
{
   GetStressPoint(1, pX, pY);
}

void CPGStableStressPointGrid::SetTopRight(Float64 x, Float64 y)
{
   SetStressPoint(2, x, y);
}

void CPGStableStressPointGrid::GetTopRight(Float64* pX, Float64* pY)
{
   GetStressPoint(2, pX, pY);
}

void CPGStableStressPointGrid::SetBottomLeft(Float64 x, Float64 y)
{
   SetStressPoint(3, x, y);
}

void CPGStableStressPointGrid::GetBottomLeft(Float64* pX, Float64* pY)
{
   GetStressPoint(3, pX, pY);
}

void CPGStableStressPointGrid::SetBottomRight(Float64 x, Float64 y)
{
   SetStressPoint(4, x, y);
}

void CPGStableStressPointGrid::GetBottomRight(Float64* pX, Float64* pY)
{
   GetStressPoint(4, pX, pY);
}

void CPGStableStressPointGrid::OnUnitsChanged()
{
   UpdateColumnHeaders();
}

void CPGStableStressPointGrid::SetStressPoint(ROWCOL row, Float64 x, Float64 y)
{
   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   GetParam()->EnableUndo(FALSE);

   SetStyleRange(CGXRange(row, 1), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(x, pDispUnits->ComponentDim, false))
   );

   SetStyleRange(CGXRange(row, 2), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(y, pDispUnits->ComponentDim, false))
   );

   ResizeColWidthsToFit(CGXRange(0,0,GetRowCount(),GetColCount()));

   GetParam()->EnableUndo(TRUE);
}

void CPGStableStressPointGrid::GetStressPoint(ROWCOL row, Float64* pX, Float64* pY)
{
   *pX = GetCellValue(row, 1);
   *pY = GetCellValue(row, 2);
}

Float64 CPGStableStressPointGrid::GetCellValue(ROWCOL nRow, ROWCOL nCol)
{
   CString s;
   if (IsCurrentCell(nRow, nCol) && IsActiveCurrentCell())
   {
      CString s;
      CGXControl* pControl = GetControl(nRow, nCol);
      pControl->GetValue(s);
   }
   else
   {
      s = GetValueRowCol(nRow, nCol);
   }

   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   Float64 value;
   sysTokenizer::ParseDouble(s, &value);
   return WBFL::Units::ConvertToSysUnits(value, pDispUnits->ComponentDim.UnitOfMeasure);
}
