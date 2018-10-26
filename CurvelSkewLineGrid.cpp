///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2013  Washington State Department of Transportation
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

// CurvelSkewLineGrid.cpp : implementation file
//

#include "stdafx.h"
#include "CurvelSkewLineGrid.h"
#include "BEToolboxUtilities.h"
#include <EAF\EAFUtilities.h>
#include <EAF\EAFApp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//GRID_IMPLEMENT_REGISTER(CCurvelSkewLineGrid, CS_DBLCLKS, 0, 0, 0);

/////////////////////////////////////////////////////////////////////////////
// CCurvelSkewLineGrid

CCurvelSkewLineGrid::CCurvelSkewLineGrid()
{
//   RegisterClass();
   m_objStation.CoCreateInstance(CLSID_Station);
   m_objAngle.CoCreateInstance(CLSID_Angle);
}

CCurvelSkewLineGrid::~CCurvelSkewLineGrid()
{
}

BEGIN_MESSAGE_MAP(CCurvelSkewLineGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CCurvelSkewLineGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCurvelSkewLineGrid message handlers

void CCurvelSkewLineGrid::RemoveSelectedSkewLines()
{
	CGXRangeList* pSelList = GetParam()->GetRangeList();
	if (pSelList->IsAnyCellFromCol(0) && pSelList->GetCount() == 1)
	{
		CGXRange range = pSelList->GetHead();
		range.ExpandRange(1, 0, GetRowCount(), 0);
		RemoveRows(range.top, range.bottom);
	}
}

BOOL CCurvelSkewLineGrid::AreSkewLinesSelected()
{
	if (GetParam() == NULL)
	{
      return FALSE;
	}

	CGXRangeList* pSelList = GetParam()->GetRangeList();
   return (pSelList->IsAnyCellFromCol(0) && pSelList->GetCount() == 1) ? TRUE : FALSE;
}

void CCurvelSkewLineGrid::CorrectForSuperelevation(bool bCorrect)
{
   // No corrections
}

void CCurvelSkewLineGrid::CustomInit()
{
   // Initialize the grid. For CWnd based grids this call is // 
   // essential. For view based grids this initialization is done 
   // in OnInitialUpdate.
	Initialize( );

	GetParam( )->EnableUndo(FALSE);

   const ROWCOL num_rows = 0;
   const ROWCOL num_cols = 6;

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

void CCurvelSkewLineGrid::AddSkewLine()
{
   SkewLine skewLine;
   skewLine.Station = 0;
   skewLine.Offset = 0;
   skewLine.OffsetType = SkewLine::RadialFromCrownLine;
   skewLine.Radius = 0;
   skewLine.CrownOffset = 0;
   skewLine.strSkewAngle = _T("00 00 00");

   if ( 0 < GetRowCount() )
      GetSkewLine(GetRowCount(),skewLine);

   InsertSkewLine(skewLine);
}

void CCurvelSkewLineGrid::GetSkewLine(ROWCOL row,SkewLine& skewLine)
{
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   ROWCOL col = 1;
   CString strStation = GetCellValue(row,col++);
   HRESULT hr = m_objStation->FromString(CComBSTR(strStation),umUS);
   ATLASSERT(SUCCEEDED(hr));

   Float64 station_value;
   m_objStation->get_Value(&station_value);
   station_value = ::ConvertToSysUnits(station_value,pDispUnits->AlignmentLength.UnitOfMeasure);
   skewLine.Station = station_value;

   skewLine.OffsetType = (SkewLine::Type)_tstoi(GetCellValue(row,col++));

   skewLine.Offset = GetOffset(GetCellValue(row,col++),pDispUnits->AlignmentLength);

   skewLine.strSkewAngle = GetCellValue(row,col++);

   skewLine.Radius = GetOffset(GetCellValue(row,col++),pDispUnits->AlignmentLength);

   skewLine.CrownOffset = GetOffset(GetCellValue(row,col++),pDispUnits->AlignmentLength);
}

void CCurvelSkewLineGrid::InsertSkewLine(const SkewLine& skewLine)
{
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   GetParam()->EnableUndo(FALSE);
	ROWCOL nRow = GetRowCount()+1;

	InsertRows(nRow, 1);

   ROWCOL col = 1;
   CString strStation = FormatStation(pDispUnits->StationFormat,skewLine.Station);
	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(strStation)
         );

   SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetControl(GX_IDS_CTRL_ZEROBASED_EX)
   	.SetChoiceList(_T("Normal from Crown Line\nAlong Skew from L-Line"))
		.SetValue((long)skewLine.OffsetType)
      );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(GetOffset(skewLine.Offset,pDispUnits->AlignmentLength))
         );

   m_objAngle->FromString(CComBSTR(skewLine.strSkewAngle.c_str()));

   SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(FormatAngle(m_objAngle))
      );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(GetOffset(skewLine.Radius,pDispUnits->AlignmentLength))
         );

   SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(GetOffset(skewLine.CrownOffset,pDispUnits->AlignmentLength))
         );

   ResizeColWidthsToFit(CGXRange().SetCols(0,GetColCount()));

   SetCurrentCell(nRow, GetLeftCol(), GX_SCROLLINVIEW|GX_DISPLAYEDITWND);
	Invalidate();

   GetParam()->EnableUndo(TRUE);
}

CString CCurvelSkewLineGrid::GetCellValue(ROWCOL nRow, ROWCOL nCol)
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
BOOL CCurvelSkewLineGrid::OnValidateCell(ROWCOL nRow, ROWCOL nCol)
{
	CString s;
	CGXControl* pControl = GetControl(nRow, nCol);
	pControl->GetCurrentText(s);

   if ( s.IsEmpty() )
   {
      SetWarningText(_T("Value must be a number"));
      return FALSE;
   }

   if ( nCol == 1 )
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

   if ( nCol == 4 )
   {
      // angle
      HRESULT hr = m_objAngle->FromString(CComBSTR(s));
      if ( FAILED(hr) )
      {
         SetWarningText(_T("Invalid skew angle value. Enter skew angle in one of the following formats:\n(+|-)dd.ddd\ndd.ddd L|R\n(+|-)dd mm ss.ss\ndd mm ss.ss L|R"));
         return FALSE;
      }
      else
      {
         return TRUE;
      }
   }

	return CGXGridWnd::OnValidateCell(nRow, nCol);
}
   
void CCurvelSkewLineGrid::OnModifyCell(ROWCOL nRow,ROWCOL nCol)
{
   CGXGridWnd::OnModifyCell(nRow,nCol);
   ResizeColWidthsToFit(CGXRange().SetCols(0,GetColCount()));
}

void CCurvelSkewLineGrid::UpdateColumnHeaders()
{
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   // set text along top row
   ROWCOL col = 1;

	SetStyleRange(CGXRange(0,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(_T("Station"))
		);

	SetStyleRange(CGXRange(0,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(_T("Offset Measure"))
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

	SetStyleRange(CGXRange(0,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(_T("Skew Angle"))
		);

   strDimension.Format(_T("%s\n(%s)"),_T("L-Line\nRadius"),pDispUnits->AlignmentLength.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(strDimension)
		);
   strDimension.Format(_T("%s\n(%s)"),_T("Crown Offset"),pDispUnits->AlignmentLength.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(strDimension)
		);
}

void CCurvelSkewLineGrid::SetSkewLines(const std::vector<SkewLine>& skewLines)
{
   if ( 0 < GetRowCount() )
      RemoveRows(1,GetRowCount());

   std::vector<SkewLine>::const_iterator iter(skewLines.begin());
   std::vector<SkewLine>::const_iterator end(skewLines.end());
   for ( ; iter != end; iter++ )
   {
      InsertSkewLine(*iter);
   }
}

std::vector<SkewLine> CCurvelSkewLineGrid::GetSkewLines()
{
   std::vector<SkewLine> skewLines;
   ROWCOL nRows = GetRowCount();
   for ( ROWCOL row = 0; row < nRows; row++ )
   {
      SkewLine skewLine;
      GetSkewLine(row+1,skewLine);
      skewLines.push_back(skewLine);
   }
   return skewLines;
}

#if defined _DEBUG
void CCurvelSkewLineGrid::AssertValid() const
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CGXGridWnd::AssertValid();
}
#endif
