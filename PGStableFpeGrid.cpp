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

// PGStableFpeGrid.cpp : implementation file
//

#include "stdafx.h"
#include "PGStableFpeGrid.h"
#include <EAF\EAFUtilities.h>
#include <EAF\EAFApp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//GRID_IMPLEMENT_REGISTER(CPGStableFpeGrid, CS_DBLCLKS, 0, 0, 0);

/////////////////////////////////////////////////////////////////////////////
// CPGStableFpeGrid

CPGStableFpeGrid::CPGStableFpeGrid()
{
//   RegisterClass();
}

CPGStableFpeGrid::~CPGStableFpeGrid()
{
}

BEGIN_MESSAGE_MAP(CPGStableFpeGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CPGStableFpeGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CPGStableFpeGrid::DoDataExchange(CDataExchange* pDX)
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
// CPGStableFpeGrid message handlers

void CPGStableFpeGrid::RemoveSelected()
{
	CGXRangeList* pSelList = GetParam()->GetRangeList();
	if (pSelList->IsAnyCellFromCol(0) && pSelList->GetCount() == 1)
	{
		CGXRange range = pSelList->GetHead();
		range.ExpandRange(1, 0, GetRowCount(), 0);
		RemoveRows(range.top, range.bottom);
	}
}

BOOL CPGStableFpeGrid::AreSelected()
{
	if (GetParam() == nullptr)
	{
      return FALSE;
	}

	CGXRangeList* pSelList = GetParam()->GetRangeList();
   return (pSelList->IsAnyCellFromCol(0) && pSelList->GetCount() == 1) ? TRUE : FALSE;
}

void CPGStableFpeGrid::CustomInit()
{
   // Initialize the grid. For CWnd based grids this call is // 
   // essential. For view based grids this initialization is done 
   // in OnInitialUpdate.
	Initialize( );

   // we want to merge cells
   SetMergeCellsMode(gxnMergeEvalOnDisplay);

	GetParam( )->EnableUndo(FALSE);

   const ROWCOL num_rows = 1;
   const ROWCOL num_cols = 10;

	SetRowCount(num_rows);
	SetColCount(num_cols);

   // Turn off selecting whole columns when clicking on a column header
	GetParam()->EnableSelection((WORD) (GX_SELFULL & ~GX_SELCOL & ~GX_SELTABLE));

   // no row moving
	GetParam()->EnableMoveRows(FALSE);

   SetFrozenRows(1/*# frozen rows*/,1/*# extra header rows*/);

   // disable left side
	SetStyleRange(CGXRange(0,0,num_rows,0), CGXStyle()
			.SetControl(GX_IDS_CTRL_HEADER)
			.SetEnabled(FALSE)          // disables usage as current cell
		);

   UpdateColumnHeaders();

   // make it so that text fits correctly in header row
	ResizeRowHeightsToFit(CGXRange(0,0,0,num_cols));

   // don't allow users to resize grids
   GetParam( )->EnableTrackColWidth(0); 
   GetParam( )->EnableTrackRowHeight(0); 

	SetFocus();

	GetParam( )->EnableUndo(TRUE);

//   EnableIntelliMouse();
}

void CPGStableFpeGrid::AddFpe()
{
   Float64 X,FpeStraight,YpsStraight,FpeHarped,YpsHarped,FpeTemp,YpsTemp;
   int YpsStraightMeasure,YpsHarpedMeasure,YpsTempMeasure;
   GetFpe(GetRowCount(),&X,&FpeStraight,&YpsStraight,&YpsStraightMeasure,&FpeHarped,&YpsHarped,&YpsHarpedMeasure,&FpeTemp,&YpsTemp,&YpsTempMeasure);
   InsertFpe(X,FpeStraight,YpsStraight,YpsStraightMeasure,FpeHarped,YpsHarped,YpsHarpedMeasure,FpeTemp,YpsTemp,YpsTempMeasure);
}

void CPGStableFpeGrid::GetFpe(ROWCOL row,Float64* pX,Float64* pFpeStraight,Float64* pYpsStraight,int* pYpsStraightMeasure,Float64* pFpeHarped,Float64* pYpsHarped,int* pYpsHarpedMeasure,Float64* pFpeTemp,Float64* pYpsTemp,int* pYpsTempMeasure)
{
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   ROWCOL col = 1;
   CString strValue;
   Float64 value;
   strValue = GetCellValue(row,col++);
   sysTokenizer::ParseDouble(strValue, &value);
   *pX = ::ConvertToSysUnits(value,pDispUnits->SpanLength.UnitOfMeasure);

   strValue = GetCellValue(row,col++);
   sysTokenizer::ParseDouble(strValue, &value);
   *pFpeStraight = ::ConvertToSysUnits(value,pDispUnits->GeneralForce.UnitOfMeasure);

   strValue = GetCellValue(row,col++);
   sysTokenizer::ParseDouble(strValue, &value);
   *pYpsStraight = ::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);

   strValue = GetCellValue(row,col++);
   *pYpsStraightMeasure = (strValue == _T("Top") ? TOP : BOTTOM);

   strValue = GetCellValue(row,col++);
   sysTokenizer::ParseDouble(strValue, &value);
   *pFpeHarped = ::ConvertToSysUnits(value,pDispUnits->GeneralForce.UnitOfMeasure);

   strValue = GetCellValue(row,col++);
   sysTokenizer::ParseDouble(strValue, &value);
   *pYpsHarped = ::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);

   strValue = GetCellValue(row,col++);
   *pYpsHarpedMeasure = (strValue == _T("Top") ? TOP : BOTTOM);

   strValue = GetCellValue(row,col++);
   sysTokenizer::ParseDouble(strValue, &value);
   *pFpeTemp = ::ConvertToSysUnits(value,pDispUnits->GeneralForce.UnitOfMeasure);

   strValue = GetCellValue(row,col++);
   sysTokenizer::ParseDouble(strValue, &value);
   *pYpsTemp = ::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);

   strValue = GetCellValue(row,col++);
   *pYpsTempMeasure = (strValue == _T("Top") ? TOP : BOTTOM);
}

void CPGStableFpeGrid::InsertFpe(Float64 X,Float64 FpeStraight,Float64 YpsStraight,int YpsStraightMeasure,Float64 FpeHarped,Float64 YpsHarped,int YpsHarpedMeasure,Float64 FpeTemp,Float64 YpsTemp,int YpsTempMeasure)
{
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   CString strChoiceList(_T("Top\nBottom"));

   GetParam()->EnableUndo(FALSE);
	ROWCOL nRow = GetRowCount()+1;

	InsertRows(nRow, 1);

   ROWCOL col = 0;

   // Row number
   SetStyleRange(CGXRange(nRow,col++),CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(nRow-1)
      );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(X,pDispUnits->SpanLength,false))
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(FpeStraight,pDispUnits->GeneralForce,false))
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(YpsStraight,pDispUnits->ComponentDim,false))
         );

   SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetControl(GX_IDS_CTRL_CBS_DROPDOWNLIST)
      .SetChoiceList(strChoiceList)
      .SetValue(YpsStraightMeasure == TOP ? _T("Top") : _T("Bottom"))
      );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(FpeHarped,pDispUnits->GeneralForce,false))
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(YpsHarped,pDispUnits->ComponentDim,false))
         );

   SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetControl(GX_IDS_CTRL_CBS_DROPDOWNLIST)
      .SetChoiceList(strChoiceList)
      .SetValue(YpsHarpedMeasure == TOP ? _T("Top") : _T("Bottom"))
      );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(FpeTemp,pDispUnits->GeneralForce,false))
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(YpsTemp,pDispUnits->ComponentDim,false))
         );

   SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetControl(GX_IDS_CTRL_CBS_DROPDOWNLIST)
      .SetChoiceList(strChoiceList)
      .SetValue(YpsTempMeasure == TOP ? _T("Top") : _T("Bottom"))
      );

   SetCurrentCell(nRow, GetLeftCol(), GX_SCROLLINVIEW|GX_DISPLAYEDITWND);
	Invalidate();

   GetParam()->EnableUndo(TRUE);
}

CString CPGStableFpeGrid::GetCellValue(ROWCOL nRow, ROWCOL nCol)
{
    if (IsCurrentCell(nRow, nCol) && IsActiveCurrentCell())
    {
        CString s;
        CGXControl* pControl = GetControl(nRow, nCol);
        pControl->GetValue(s);
        return s;
  }
    else
        return GetValueRowCol(nRow, nCol);
}

// validate input
BOOL CPGStableFpeGrid::OnValidateCell(ROWCOL nRow, ROWCOL nCol)
{
	CString s;
	CGXControl* pControl = GetControl(nRow, nCol);
	pControl->GetCurrentText(s);

   if ( s.IsEmpty() )
   {
      SetWarningText(_T("Value must be a number"));
      return FALSE;
   }

   if ( nCol != 4 && nCol != 7 && nCol != 10 ) // these are the Top/Bottom combo box columns
   {
      Float64 d;
      if ( !sysTokenizer::ParseDouble(s,&d) )
      {
         SetWarningText(_T("Value must be a number"));
         return FALSE;
      }
   }

	return CGXGridWnd::OnValidateCell(nRow, nCol);
}

void CPGStableFpeGrid::OnUnitsModeChanged()
{
   //ROWCOL nRows = GetRowCount();
   //if ( 0 < nRows )
   //{
   //   CGXRange range(1, 0, GetRowCount(), 0);
   //   RemoveRows(range.top, range.bottom);
   //}

   //UpdateColumnHeaders();
   //UpdateData(FALSE);
}

void CPGStableFpeGrid::UpdateColumnHeaders()
{
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   // set text along top row
   ROWCOL col = 0;
	SetStyleRange(CGXRange(0,col,1,col++), CGXStyle()
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetMergeCell(GX_MERGE_VERTICAL | GX_MERGE_COMPVALUE)
         .SetValue(_T(" "))
		);

   CString strDimension;
   strDimension.Format(_T("%s (%s)"),_T("X"),pDispUnits->SpanLength.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col,1,col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)          // disables usage as current cell
      .SetMergeCell(GX_MERGE_VERTICAL | GX_MERGE_COMPVALUE)
		.SetValue(strDimension)
	);

	SetStyleRange(CGXRange(0,col,0,col+2), CGXStyle()
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_TOP)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetMergeCell(GX_MERGE_HORIZONTAL | GX_MERGE_COMPVALUE)
			.SetValue(_T("Straight"))
		);

   strDimension.Format(_T("Fpe (%s)"),pDispUnits->GeneralForce.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(1,col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)          // disables usage as current cell
		.SetValue(strDimension)
	);

   strDimension.Format(_T("Yps (%s)"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(1,col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)          // disables usage as current cell
		.SetValue(strDimension)
	);

	SetStyleRange(CGXRange(1,col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)          // disables usage as current cell
		.SetValue(_T("Face"))
	);

	SetStyleRange(CGXRange(0,col,0,col+2), CGXStyle()
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_TOP)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetMergeCell(GX_MERGE_HORIZONTAL | GX_MERGE_COMPVALUE)
			.SetValue(_T("Harped"))
		);

   strDimension.Format(_T("Fpe (%s)"),pDispUnits->GeneralForce.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(1,col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)          // disables usage as current cell
		.SetValue(strDimension)
	);

   strDimension.Format(_T("Yps (%s)"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(1,col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)          // disables usage as current cell
		.SetValue(strDimension)
	);

	SetStyleRange(CGXRange(1,col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)          // disables usage as current cell
		.SetValue(_T("Face"))
	);

	SetStyleRange(CGXRange(0,col,0,col+2), CGXStyle()
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_TOP)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetMergeCell(GX_MERGE_HORIZONTAL | GX_MERGE_COMPVALUE)
			.SetValue(_T("Temporary"))
		);

   strDimension.Format(_T("Fpe (%s)"),pDispUnits->GeneralForce.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(1,col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)          // disables usage as current cell
		.SetValue(strDimension)
	);

   strDimension.Format(_T("Yps (%s)"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(1,col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)          // disables usage as current cell
		.SetValue(strDimension)
	);

	SetStyleRange(CGXRange(1,col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)          // disables usage as current cell
		.SetValue(_T("Face"))
	);
}

void CPGStableFpeGrid::FillGrid(const std::set<CPGStableFpe>& vFpe)
{
   ROWCOL nRows = GetRowCount();
   if ( 2 <= nRows )
   {
      RemoveRows(2, nRows);
   }

   for (const auto& fpe : vFpe)
   {
      InsertFpe(fpe.X,fpe.FpeStraight,fpe.YpsStraight,fpe.YpsStraightMeasure,fpe.FpeHarped,fpe.YpsHarped,fpe.YpsHarpedMeasure,fpe.FpeTemp,fpe.YpsTemp,fpe.YpsTempMeasure);
   }
}

void CPGStableFpeGrid::GetFpe(std::set<CPGStableFpe>& vFpe)
{
   vFpe.clear();
   
   ROWCOL nRows = GetRowCount();
   for ( ROWCOL row = 2; row <= nRows; row++ )
   {
      CPGStableFpe fpe;

      GetFpe(row,&fpe.X,&fpe.FpeStraight,&fpe.YpsStraight,&fpe.YpsStraightMeasure,&fpe.FpeHarped,&fpe.YpsHarped,&fpe.YpsHarpedMeasure,&fpe.FpeTemp,&fpe.YpsTemp,&fpe.YpsTempMeasure);
      vFpe.insert(fpe);
   }
}

#if defined _DEBUG
void CPGStableFpeGrid::AssertValid() const
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CGXGridWnd::AssertValid();
}
#endif
