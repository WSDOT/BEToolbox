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

// StationRangeGrid.cpp : implementation file
//

#include "stdafx.h"
#include "CurvelStationRangeGrid.h"
#include "..\BEToolboxUtilities.h"
#include <EAF\EAFUtilities.h>
#include <EAF\EAFApp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//GRID_IMPLEMENT_REGISTER(CCurvelStationRangeGrid, CS_DBLCLKS, 0, 0, 0);

/////////////////////////////////////////////////////////////////////////////
// CCurvelStationRangeGrid

CCurvelStationRangeGrid::CCurvelStationRangeGrid()
{
//   RegisterClass();
   m_objStation.CoCreateInstance(CLSID_Station);
}

CCurvelStationRangeGrid::~CCurvelStationRangeGrid()
{
}

BEGIN_MESSAGE_MAP(CCurvelStationRangeGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CCurvelStationRangeGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCurvelStationRangeGrid message handlers

void CCurvelStationRangeGrid::RemoveSelectedStations()
{
	CGXRangeList* pSelList = GetParam()->GetRangeList();
	if (pSelList->IsAnyCellFromCol(0) && pSelList->GetCount() == 1)
	{
		CGXRange range = pSelList->GetHead();
		range.ExpandRange(1, 0, GetRowCount(), 0);
		RemoveRows(range.top, range.bottom);
	}
}

BOOL CCurvelStationRangeGrid::AreStationsSelected()
{
	if (GetParam() == nullptr)
	{
      return FALSE;
	}

	CGXRangeList* pSelList = GetParam()->GetRangeList();
   return (pSelList->IsAnyCellFromCol(0) && pSelList->GetCount() == 1) ? TRUE : FALSE;
}

void CCurvelStationRangeGrid::CorrectForSuperelevation(bool bCorrect)
{
   HideCols(4,4,bCorrect ? FALSE : TRUE);
}

void CCurvelStationRangeGrid::CustomInit()
{
   // Initialize the grid. For CWnd based grids this call is // 
   // essential. For view based grids this initialization is done 
   // in OnInitialUpdate.
	Initialize( );

	GetParam( )->EnableUndo(FALSE);

   const ROWCOL num_rows = 0;
   const ROWCOL num_cols = 4;

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

   // make it so that text fits correctly in header row
	ResizeRowHeightsToFit(CGXRange(0,0,0,num_cols));
   ResizeColWidthsToFit(CGXRange(0,0,0,num_cols));

   // don't allow users to resize grids
   GetParam( )->EnableTrackColWidth(0); 
   GetParam( )->EnableTrackRowHeight(0); 

	SetFocus();

	GetParam( )->EnableUndo(TRUE);

   EnableIntelliMouse();
}

void CCurvelStationRangeGrid::AddStation()
{
   StationRange station;
   station.StartStation = 0;
   station.EndStation = 0;
   station.nSpaces = 0;
   station.Offset = 0;
   if ( 0 < GetRowCount() )
      GetStation(GetRowCount(),station);

   InsertStation(station);
}

void CCurvelStationRangeGrid::GetStation(ROWCOL row,StationRange& station)
{
   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   ROWCOL col = 1;

   CString strStation = GetCellValue(row,col++);
   HRESULT hr = m_objStation->FromString(CComBSTR(strStation),umUS);
   ATLASSERT(SUCCEEDED(hr));

   Float64 station_value;
   m_objStation->get_Value(&station_value);
   station_value = WBFL::Units::ConvertToSysUnits(station_value,pDispUnits->AlignmentLength.UnitOfMeasure);
   station.StartStation = station_value;

   strStation = GetCellValue(row,col++);
   hr = m_objStation->FromString(CComBSTR(strStation),umUS);
   ATLASSERT(SUCCEEDED(hr));

   m_objStation->get_Value(&station_value);
   station_value = WBFL::Units::ConvertToSysUnits(station_value,pDispUnits->AlignmentLength.UnitOfMeasure);
   station.EndStation = station_value;

   CString strValue;
   long nSpaces;
   strValue = GetCellValue(row,col++);
   sysTokenizer::ParseLong(strValue,&nSpaces);
   station.nSpaces = nSpaces;

   station.Offset = GetOffset(GetCellValue(row,col++),pDispUnits->AlignmentLength);
}

void CCurvelStationRangeGrid::InsertStation(const StationRange& station)
{
   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   GetParam()->EnableUndo(FALSE);
	ROWCOL nRow = GetRowCount()+1;

	InsertRows(nRow, 1);

   ROWCOL col = 1;

   CString strStation = FormatStation(pDispUnits->StationFormat,station.StartStation);
	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(strStation)
         );

   strStation = FormatStation(pDispUnits->StationFormat,station.EndStation);
	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(strStation)
         );

   SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue((long)station.nSpaces)
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(GetOffset(station.Offset,pDispUnits->AlignmentLength))
         );

   ResizeColWidthsToFit(CGXRange().SetCols(0,GetColCount()));

   SetCurrentCell(nRow, GetLeftCol(), GX_SCROLLINVIEW|GX_DISPLAYEDITWND);
	Invalidate();

   GetParam()->EnableUndo(TRUE);
}

CString CCurvelStationRangeGrid::GetCellValue(ROWCOL nRow, ROWCOL nCol)
{
   if (IsCurrentCell(nRow, nCol) && IsActiveCurrentCell())
   {
      CString s;
      CGXControl* pControl = GetControl(nRow, nCol);
      pControl->GetValue(s);
      return s;
   }

   return GetValueRowCol(nRow, nCol);
}

// validate input
BOOL CCurvelStationRangeGrid::OnValidateCell(ROWCOL nRow, ROWCOL nCol)
{
	CString s;
	CGXControl* pControl = GetControl(nRow, nCol);
	pControl->GetCurrentText(s);

   if ( s.IsEmpty() )
   {
      SetWarningText(_T("Value must be a number"));
      return FALSE;
   }

   if ( nCol == 1 || nCol == 2 )
   {
      // station
      CEAFApp* pApp = EAFGetApp();

      HRESULT hr = m_objStation->FromString( CComBSTR(s), pApp->GetUnitsMode() == eafTypes::umUS ? umUS : umSI);
      if ( FAILED(hr) )
      {
         if ( pApp->GetUnitsMode() == eafTypes::umUS )
            SetWarningText (_T("Invalid station value. Enter the station in the following format: xx+yy.zz"));
         else
            SetWarningText (_T("Invalid station value. Enter the station in the following format: xx+yyy.zz"));
         return FALSE;
      }
      else
      {
         return TRUE;
      }
   }

	return CGXGridWnd::OnValidateCell(nRow, nCol);
}
   
BOOL CCurvelStationRangeGrid::OnEndEditing(ROWCOL nRow,ROWCOL nCol)
{
   ResizeColWidthsToFit(CGXRange(0,0,GetRowCount(),GetColCount()));
   return CGXGridWnd::OnEndEditing(nRow,nCol);
}

void CCurvelStationRangeGrid::UpdateColumnHeaders()
{
   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   // set text along top row
   ROWCOL col = 1;

	SetStyleRange(CGXRange(0,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(_T("Start\nStation"))
		);

	SetStyleRange(CGXRange(0,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(_T("End\nStation"))
		);

	SetStyleRange(CGXRange(0,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
         .SetValue(_T("#\nSpaces"))
		);

   CString strDimension;
   strDimension.Format(_T("%s\n(%s)"),_T("Offset"),pDispUnits->AlignmentLength.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(strDimension)
		);
}

void CCurvelStationRangeGrid::SetStations(const std::vector<StationRange>& stations)
{
   if ( 0 < GetRowCount() )
      RemoveRows(1,GetRowCount());

   std::vector<StationRange>::const_iterator iter(stations.begin());
   std::vector<StationRange>::const_iterator end(stations.end());
   for ( ; iter != end; iter++ )
   {
      InsertStation(*iter);
   }
}

std::vector<StationRange> CCurvelStationRangeGrid::GetStations()
{
   std::vector<StationRange> stations;
   ROWCOL nRows = GetRowCount();
   for ( ROWCOL row = 0; row < nRows; row++ )
   {
      StationRange station;
      GetStation(row+1,station);
      stations.push_back(station);
   }
   return stations;
}

#if defined _DEBUG
void CCurvelStationRangeGrid::AssertValid() const
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CGXGridWnd::AssertValid();
}
#endif
