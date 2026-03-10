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

// PGStableLongRebarGrid.cpp : implementation file
//

#include "stdafx.h"
#include "..\resource.h"
#include <EAF\EAFDisplayUnits.h>
#include "PGStableLongRebarGrid.h"
#include <system\tokenizer.h>
#include <LRFD\RebarPool.h>
#include <IFace\Bridge.h>

#include "PGStableDoc.h"
#include "PGStablePrismaticGirder.h"

GRID_IMPLEMENT_REGISTER(CPGStableLongRebarGrid, CS_DBLCLKS, 0, 0, 0);

/////////////////////////////////////////////////////////////////////////////
// CPGStableLongRebarGrid

CPGStableLongRebarGrid::CPGStableLongRebarGrid()
{
//   RegisterClass();
}

CPGStableLongRebarGrid::~CPGStableLongRebarGrid()
{
}

BEGIN_MESSAGE_MAP(CPGStableLongRebarGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CPGStableLongRebarGrid)
   ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPGStableLongRebarGrid message handlers
int CPGStableLongRebarGrid::GetColWidth(ROWCOL nCol)
{
	CRect rect = GetGridRect( );

   return rect.Width() / 5;
}

BOOL CPGStableLongRebarGrid::OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
{
   CPGStablePrismaticGirder* pdlg = (CPGStablePrismaticGirder*)GetParent();
   ASSERT (pdlg);

   if (nCol==0 && nRow!=0)
   {
      pdlg->OnEnableDeleteRebarRow(true);
   }
   else
   {
      pdlg->OnEnableDeleteRebarRow(false);
   }

   return FALSE;
}

void CPGStableLongRebarGrid::Insertrow(WBFL::Materials::Rebar::Type type, WBFL::Materials::Rebar::Grade grade)
{
	ROWCOL nRow = 0;

	// if there are no cells selected,
	// copy the current cell's coordinates
	CGXRangeList selList;
	if (CopyRangeList(selList, TRUE))
   {
		nRow = selList.GetHead()->top;
   }
	else
   {
		nRow = GetRowCount()+1;
   }

	nRow = Max((ROWCOL)1, nRow);

	InsertRows(nRow, 1);
   SetRowStyle(nRow,type,grade);
   SetCurrentCell(nRow, GetLeftCol(), GX_SCROLLINVIEW|GX_DISPLAYEDITWND);
	Invalidate();
}

void CPGStableLongRebarGrid::Removerows()
{
	CGXRangeList* pSelList = GetParam()->GetRangeList();
	if (pSelList->IsAnyCellFromCol(0) && pSelList->GetCount() == 1)
	{
		CGXRange range = pSelList->GetHead();
		range.ExpandRange(1, 0, GetRowCount(), 0);
		RemoveRows(range.top, range.bottom);
	}
}

void CPGStableLongRebarGrid::CustomInit()
{
   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

// Initialize the grid. For CWnd based grids this call is // 
// essential. For view based grids this initialization is done 
// in OnInitialUpdate.
	Initialize( );

	GetParam( )->EnableUndo(FALSE);

   const int num_rows=0;
   const int num_cols=4;

	SetRowCount(num_rows);
	SetColCount(num_cols);

		// Turn off selecting whole columns when clicking on a column header
	GetParam()->EnableSelection((WORD) (GX_SELFULL & ~GX_SELCOL & ~GX_SELTABLE));

   // disable left side
	SetStyleRange(CGXRange(0,0,num_rows,0), CGXStyle()
			.SetControl(GX_IDS_CTRL_HEADER)
			.SetEnabled(FALSE)          // disables usage as current cell
		);

   ROWCOL row = 0;
   ROWCOL col = 0;

// set text along top row
	SetStyleRange(CGXRange(row,col++), CGXStyle()
         .SetWrapText(TRUE)
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetEnabled(FALSE)          // disables usage as current cell
			.SetValue(_T("Row\n#"))
		);

   CString cv;
   cv.Format(_T("Cover\n(%s)"), pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(row, col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(cv)
		);

	SetStyleRange(CGXRange(row, col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(_T("Bar\nSize"))
		);

	SetStyleRange(CGXRange(row, col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(_T("# of\nBars"))
		);

   cv.Format(_T("Spacing\n(%s)"), pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(row, col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(cv)
		);

   // make it so that text fits correctly in header row
   ResizeColWidthsToFit(CGXRange(0, 0, 0, num_cols));
   ResizeRowHeightsToFit(CGXRange(0,0,0,num_cols));

   // don't allow users to resize grids
   GetParam( )->EnableTrackColWidth(0); 
   GetParam( )->EnableTrackRowHeight(0); 

	EnableIntelliMouse();
	SetFocus();

	GetParam( )->EnableUndo(TRUE);
}

CString CPGStableLongRebarGrid::GetCellValue(ROWCOL nRow, ROWCOL nCol)
{
    if (IsCurrentCell(nRow, nCol) && IsActiveCurrentCell())
    {
        CString s;
        CGXControl* pControl = GetControl(nRow, nCol);
        pControl->GetValue(s);
        return s;
    }
    else
    {
        return GetValueRowCol(nRow, nCol);
    }
}

void CPGStableLongRebarGrid::OnModifyCell(ROWCOL nRow,ROWCOL nCol)
{
   __super::OnModifyCell(nRow, nCol);
}

WBFL::Materials::Rebar::Size CPGStableLongRebarGrid::GetBarSize(ROWCOL row)
{
   CString s = GetCellValue(row, 2);
   s.TrimLeft();
   int l = s.GetLength();
   CString s2 = s.Right(l-1);
   int i = _tstoi(s2);
   if (s.IsEmpty() || (i==0))
   {
      return WBFL::Materials::Rebar::Size::bsNone;
   }

   switch(i)
   {
   case 3:  return WBFL::Materials::Rebar::Size::bs3;
   case 4:  return WBFL::Materials::Rebar::Size::bs4;
   case 5:  return WBFL::Materials::Rebar::Size::bs5;
   case 6:  return WBFL::Materials::Rebar::Size::bs6;
   case 7:  return WBFL::Materials::Rebar::Size::bs7;
   case 8:  return WBFL::Materials::Rebar::Size::bs8;
   case 9:  return WBFL::Materials::Rebar::Size::bs9;
   case 10: return WBFL::Materials::Rebar::Size::bs10;
   case 11: return WBFL::Materials::Rebar::Size::bs11;
   case 14: return WBFL::Materials::Rebar::Size::bs14;
   case 18: return WBFL::Materials::Rebar::Size::bs18;
   default: ATLASSERT(false);
   }

   return WBFL::Materials::Rebar::Size::bsNone;
}

bool CPGStableLongRebarGrid::GetRowData(CDataExchange* pDX, ROWCOL nRow, CPGStableLongitudinalRebarData::RebarRow* pBarRow)
{
   Float64 d;
   int i;
   ROWCOL col = 1;

   // Cover
   CString s = GetCellValue(nRow, col++);
   if (s.IsEmpty())
   {
      d=0.0;
   }
   else
   {
      d = _tstof(s);
      if (d == 0.0 && s[0] != _T('0') || d <= 0.0)
      {
         d = 0.0;
         ATLASSERT(0); // UI should prevent
      }
   }

   pBarRow->Cover = d;

   pBarRow->BarSize = GetBarSize(nRow);
   col++;

   // number of bars
   s = GetCellValue(nRow, col++);
   i = _tstoi(s);
   if (s.IsEmpty())
   {
      i=0;
   }
   else if (i==0 && s[0]!=_T('0') || i<=0)
   {
      i = 0;
      ATLASSERT(0);
   }

   pBarRow->NumberOfBars = i;

   // Spacing
   s = GetCellValue(nRow, col++);
   d = _tstof(s);
   if (s.IsEmpty())
   {
      d=0;
   }
   else if (d==0.0 && s[0]!=_T('0') || d<0.0)
   {
      ATLASSERT(0); //UI should prevent
      d = 0;
   }

   pBarRow->BarSpacing = d;

   return true;
}

void CPGStableLongRebarGrid::FillGrid(const CPGStableLongitudinalRebarData& rebarData)
{
   GetParam()->SetLockReadOnly(FALSE);

   ROWCOL rows = GetRowCount();
   if (1 <= rows)
   {
	   RemoveRows(1, rows);
   }

   IndexType size = rebarData.RebarRows.size();
   if (0 < size)
   {
      // size grid
      for (IndexType i=0; i<size; i++)
      {
	      Insertrow(rebarData.BarType,rebarData.BarGrade);
      }

      CEAFApp* pApp = EAFGetApp();
      const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

      // fill grid
      ROWCOL nRow=1;
      std::vector<CPGStableLongitudinalRebarData::RebarRow>::const_iterator iter(rebarData.RebarRows.begin());
      std::vector<CPGStableLongitudinalRebarData::RebarRow>::const_iterator end(rebarData.RebarRows.end());
      for ( ; iter != end; iter++ )
      {
         ROWCOL col = 1;
         const CPGStableLongitudinalRebarData::RebarRow& rebar = (*iter);

         Float64 cover = WBFL::Units::ConvertFromSysUnits(rebar.Cover, pDispUnits->ComponentDim.UnitOfMeasure);
         VERIFY(SetValueRange(CGXRange(nRow, col++), cover));

         CString tmp;
         tmp.Format(_T("%s"), WBFL::LRFD::RebarPool::GetBarSize(rebar.BarSize).c_str());
         VERIFY(SetValueRange(CGXRange(nRow, col++), tmp));

         VERIFY(SetValueRange(CGXRange(nRow, col++), (LONG)rebar.NumberOfBars));

         Float64 barSpacing = WBFL::Units::ConvertFromSysUnits(rebar.BarSpacing, pDispUnits->ComponentDim.UnitOfMeasure);
         VERIFY(SetValueRange(CGXRange(nRow, col++), barSpacing));

         nRow++;
      }
   }

   ResizeColWidthsToFit(CGXRange(0, 0, GetRowCount(), GetColCount()));

   GetParam()->SetLockReadOnly(TRUE);
}

bool CPGStableLongRebarGrid::GetRebarData(CDataExchange* pDX, CPGStableLongitudinalRebarData* pRebarData)
{
   if (FALSE == this->ValidateAllCells(FALSE))
   {
      return false;
   }
   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   pRebarData->RebarRows.clear();
   ROWCOL nRows = GetRowCount();
   for (ROWCOL row = 1; row <= nRows; row++)
   {
      CPGStableLongitudinalRebarData::RebarRow rebarRow;
      if (GetRowData(pDX,row,&rebarRow))
      {
         // values are in display units - must convert to system
         rebarRow.Cover       = WBFL::Units::ConvertToSysUnits(rebarRow.Cover, pDispUnits->ComponentDim.UnitOfMeasure);
         rebarRow.BarSpacing  = WBFL::Units::ConvertToSysUnits(rebarRow.BarSpacing, pDispUnits->ComponentDim.UnitOfMeasure);
         pRebarData->RebarRows.push_back(rebarRow);
      }
      else
      {
         return false;
      }
   }
   return true;
}

void CPGStableLongRebarGrid::EnableCell(ROWCOL nRow, ROWCOL nCol, BOOL bEnable)
{
   if (bEnable)
   {
      SetStyleRange(CGXRange(nRow,nCol), CGXStyle()
         .SetEnabled(TRUE)
         .SetFormat(GX_FMT_GEN)
         .SetReadOnly(FALSE)
         .SetInterior(::GetSysColor(COLOR_WINDOW))
         );
   }
   else
   {
      SetStyleRange(CGXRange(nRow,nCol), CGXStyle()
         .SetEnabled(FALSE)
         .SetFormat(GX_FMT_HIDDEN)
         .SetInterior(::GetSysColor(COLOR_BTNFACE))
         );
   }
}

void CPGStableLongRebarGrid::SetRowStyle(ROWCOL nRow, WBFL::Materials::Rebar::Type type, WBFL::Materials::Rebar::Grade grade)
{
   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   GetParam()->EnableUndo(FALSE);

   ROWCOL nCol = 1;

   // Cover
   Float64 defCover = WBFL::Units::ConvertToSysUnits(2.0, WBFL::Units::Measure::Inch);
   defCover = WBFL::Units::ConvertFromSysUnits(defCover, pDispUnits->ComponentDim.UnitOfMeasure);

   SetStyleRange(CGXRange(nRow, nCol++), CGXStyle()
      //.SetUserAttribute(GX_IDS_UA_VALID_MIN, _T("0"))
      //.SetUserAttribute(GX_IDS_UA_VALID_MAX, _T("1.0e99"))
      //.SetUserAttribute(GX_IDS_UA_VALID_MSG, _T("Please enter zero or greater"))
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(defCover)
   );

   // Bar size
   CString strBarSizeChoiceList;
   WBFL::LRFD::RebarIter rebarIter(type, grade);
   for (rebarIter.Begin(); rebarIter; rebarIter.Next())
   {
      const auto* pRebar = rebarIter.GetCurrentRebar();
      strBarSizeChoiceList += pRebar->GetName().c_str();
      strBarSizeChoiceList += _T("\n");
   }

   SetStyleRange(CGXRange(nRow, nCol++), CGXStyle()
      .SetEnabled(TRUE)
      .SetReadOnly(FALSE)
      .SetControl(GX_IDS_CTRL_CBS_DROPDOWNLIST)
      .SetChoiceList(strBarSizeChoiceList)
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(WBFL::LRFD::RebarPool::GetBarSize(WBFL::Materials::Rebar::Size::bs4).c_str())
   );

   // Number of bars
   SetStyleRange(CGXRange(nRow, nCol++), CGXStyle()
      //.SetUserAttribute(GX_IDS_UA_VALID_MIN, _T("1"))
      //.SetUserAttribute(GX_IDS_UA_VALID_MAX, _T("1.0e99"))
      //.SetUserAttribute(GX_IDS_UA_VALID_MSG, _T("You must have at least one bar in a row"))
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue((long)5)
   );

   // Spacing
   Float64 defSpacing = WBFL::Units::ConvertToSysUnits(4.0, WBFL::Units::Measure::Inch);
   defSpacing = WBFL::Units::ConvertFromSysUnits(defSpacing, pDispUnits->ComponentDim.UnitOfMeasure);

   SetStyleRange(CGXRange(nRow, nCol++), CGXStyle()
      //.SetUserAttribute(GX_IDS_UA_VALID_MIN, _T("0"))
      //.SetUserAttribute(GX_IDS_UA_VALID_MAX, _T("1.0e99"))
      //.SetUserAttribute(GX_IDS_UA_VALID_MSG, _T("Please enter zero or greater"))
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(defSpacing)
   );

   GetParam()->EnableUndo(TRUE);
}

void CPGStableLongRebarGrid::OnKillFocus(CWnd* pNewWnd)
{
   if (!m_bIgnoreFocus)
   {
      // Trap cases where focus is leaving grid. Grid will take care of its own controls
      if (pNewWnd != nullptr && m_pGridWnd != pNewWnd && !pNewWnd->IsChild(this))
      {
         if (!ValidateAllCells(TRUE))
         {
            // Don't let grid lose focus if data is invalid
            OnActivateGrid(TRUE);
            SetFocus();
            return;
         }
      }
   }

   __super::OnKillFocus(pNewWnd);
}

BOOL CPGStableLongRebarGrid::SetCurrentCell(ROWCOL nRow, ROWCOL nCol, UINT flags)
{
   m_CurrCellFocuses = 1;
   BOOL st = __super::SetCurrentCell(nRow, nCol, flags);
   m_CurrCellFocuses = 0;

   return st;
}

BOOL CPGStableLongRebarGrid::ValidateAllCells(bool showWarningText)
{
   m_DoSetWarningText = showWarningText;

   ROWCOL nrows = GetRowCount();
   ROWCOL ncols = GetColCount();
   for (ROWCOL row = 1; row <= nrows; row++)
   {
      for (ROWCOL col = 1; col <= ncols; col++)
      {
         if (!OnValidateCell(row, col))
         {
            if (showWarningText) 
            {
               this->DisplayWarningText();
            }

            return FALSE;
         }
      }
   }

   m_DoSetWarningText = true;
   return TRUE;
}

// validate input
BOOL CPGStableLongRebarGrid::OnValidateCell(ROWCOL nRow, ROWCOL nCol)
{
   if (m_CurrCellFocuses == 0 || m_CurrCellFocuses++ == 1)
   {

      CString s = GetCellValue(nRow, nCol);

      if (nCol == 1)
      {
         Float64 d;
         if (s.IsEmpty() || !WBFL::System::Tokenizer::ParseDouble(s, &d))
         {
            if (m_DoSetWarningText)
            {
               SetWarningText(_T("Cover value must be a positive number"));
            }
            return FALSE;
         }
         else if (d <= 0)
         {
            if (m_DoSetWarningText)
            {
               SetWarningText(_T("Cover value must be a positive number"));
            }
            return FALSE;
         }
         return TRUE;
      }
      else if (nCol == 3)
      {
         long l;
         if (s.IsEmpty() || !WBFL::System::Tokenizer::ParseLong(s, &l))
         {
            if (m_DoSetWarningText)
            {
               SetWarningText(_T("Number of bars must be an integer"));
            }
            return FALSE;
         }
         else if (l <= 0)
         {
            if (m_DoSetWarningText)
            {
               SetWarningText(_T("Number of bars must be 1 or more"));
            }
            return FALSE;
         }

         return TRUE;
      }
      else if (nCol == 4)
      {
         CString nbarstxt = GetCellValue(nRow, 3);
         long nbars;
         if (WBFL::System::Tokenizer::ParseLong(nbarstxt, &nbars))
         {
            // Ignore spacing if bars < 2
            if (nbars > 1)
            {
               Float64 d;
               if (s.IsEmpty() || !WBFL::System::Tokenizer::ParseDouble(s, &d))
               {
                  if (m_DoSetWarningText)
                  {
                     SetWarningText(_T("Spacing must be a positive number"));
                  }
                  return FALSE;
               }
               else if (d <= 0)
               {
                  if (m_DoSetWarningText)
                  {
                     SetWarningText(_T("Spacing must be a positive number"));
                  }
                  return FALSE;
               }
            }
         }
         else
         {
            ATLASSERT(0); // this should not happen
            return FALSE;
         }
         return TRUE;
      }
   }
   return CGXGridWnd::OnValidateCell(nRow, nCol);
}
