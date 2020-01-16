///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2020  Washington State Department of Transportation
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

// BoxGdrDiaphramGrid.cpp : implementation file
//

#include "stdafx.h"
#include "BoxGdrDimensionGrid.h"
#include <EAF\EAFUtilities.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//GRID_IMPLEMENT_REGISTER(CBoxGdrDimensionGrid, CS_DBLCLKS, 0, 0, 0);

/////////////////////////////////////////////////////////////////////////////
// CBoxGdrDimensionGrid

CBoxGdrDimensionGrid::CBoxGdrDimensionGrid()
{
//   RegisterClass();
}

CBoxGdrDimensionGrid::~CBoxGdrDimensionGrid()
{
}

BEGIN_MESSAGE_MAP(CBoxGdrDimensionGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CBoxGdrDimensionGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CBoxGdrDimensionGrid::DoDataExchange(CDataExchange* pDX)
{
   CBoxGdrDoc* pDoc = (CBoxGdrDoc*)EAFGetDocument();
   if ( pDX->m_bSaveAndValidate )
   {
      std::vector<BOXGDRDIMENSIONS> problems;
      ROWCOL nRows = GetRowCount();
      for ( ROWCOL row = 0; row < nRows; row++ )
      {
         BOXGDRDIMENSIONS dimensions;
         GetProblemData(row+1,dimensions);
         problems.push_back(dimensions);
      }

      pDoc->ClearProblems();
      pDoc->AddProblems(problems);
   }
   else
   {
      if ( 0 < GetRowCount() )
         RemoveRows(1,GetRowCount());

      IndexType nProblems = pDoc->GetProblemCount();
      for ( IndexType idx = 0; idx < nProblems; idx++ )
      {
         const BOXGDRDIMENSIONS& problem = pDoc->GetProblem(idx);
         InsertRow(problem);
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
// CBoxGdrDimensionGrid message handlers

void CBoxGdrDimensionGrid::RemoveSelectedProblems()
{
	CGXRangeList* pSelList = GetParam()->GetRangeList();
	if (pSelList->IsAnyCellFromCol(0) && pSelList->GetCount() == 1)
	{
		CGXRange range = pSelList->GetHead();
		range.ExpandRange(1, 0, GetRowCount(), 0);
		RemoveRows(range.top, range.bottom);
	}

   UpdateData(TRUE);
   CBoxGdrDoc* pDoc = (CBoxGdrDoc*)EAFGetDocument();
   pDoc->UpdateAllViews(nullptr,0,nullptr);
}

BOOL CBoxGdrDimensionGrid::AreProblemsSelected()
{
	if (GetParam() == nullptr)
	{
      return FALSE;
	}

	CGXRangeList* pSelList = GetParam()->GetRangeList();
   return (pSelList->IsAnyCellFromCol(0) && pSelList->GetCount() == 1) ? TRUE : FALSE;
}

void CBoxGdrDimensionGrid::CustomInit()
{
   // Initialize the grid. For CWnd based grids this call is // 
   // essential. For view based grids this initialization is done 
   // in OnInitialUpdate.
	Initialize( );

	GetParam( )->EnableUndo(FALSE);

   const ROWCOL num_rows = 0;
   const ROWCOL num_cols = 14;

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

   // don't allow users to resize grids
   GetParam( )->EnableTrackColWidth(0); 
   GetParam( )->EnableTrackRowHeight(0); 

	SetFocus();

	GetParam( )->EnableUndo(TRUE);

//   EnableIntelliMouse();
}

void CBoxGdrDimensionGrid::AddProblem()
{
   BOXGDRDIMENSIONS dimensions;
   GetProblemData(GetRowCount(),dimensions);
   InsertRow(dimensions);

   UpdateData(TRUE);
   CBoxGdrDoc* pDoc = (CBoxGdrDoc*)EAFGetDocument();
   pDoc->UpdateAllViews(nullptr,0,nullptr);
}

void CBoxGdrDimensionGrid::GetProblemData(ROWCOL row,BOXGDRDIMENSIONS& dimensions)
{
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   CString strValue;
   Float64 value;
   strValue = GetCellValue(row,1);
   sysTokenizer::ParseDouble(strValue, &value);
   dimensions.D = ::ConvertToSysUnits(value,pDispUnits->SpanLength.UnitOfMeasure);

   strValue = GetCellValue(row,2);
   sysTokenizer::ParseDouble(strValue, &value);
   dimensions.T = ::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);

   strValue = GetCellValue(row,3);
   long iValue;
   sysTokenizer::ParseLong(strValue, &iValue);
   dimensions.N = iValue;

   strValue = GetCellValue(row,4);
   sysTokenizer::ParseDouble(strValue, &value);
   dimensions.W = ::ConvertToSysUnits(value,pDispUnits->SpanLength.UnitOfMeasure);

   strValue = GetCellValue(row,5);
   sysTokenizer::ParseDouble(strValue, &value);
   dimensions.ST = ::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);

   strValue = GetCellValue(row,6);
   sysTokenizer::ParseDouble(strValue, &value);
   dimensions.SB = ::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);

   strValue = GetCellValue(row,7);
   sysTokenizer::ParseDouble(strValue, &value);
   dimensions.FT = ::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);

   strValue = GetCellValue(row,8);
   sysTokenizer::ParseDouble(strValue, &value);
   dimensions.FB = ::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);

   strValue = GetCellValue(row,9);
   sysTokenizer::ParseDouble(strValue, &value);
   dimensions.EL = ::ConvertToSysUnits(value,pDispUnits->SpanLength.UnitOfMeasure);

   strValue = GetCellValue(row,10);
   sysTokenizer::ParseDouble(strValue, &value);
   dimensions.CL = ::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);

   strValue = GetCellValue(row,11);
   sysTokenizer::ParseDouble(strValue, &value);
   dimensions.BL = ::ConvertToSysUnits(value,pDispUnits->SpanLength.UnitOfMeasure);

   strValue = GetCellValue(row,12);
   sysTokenizer::ParseDouble(strValue, &value);
   dimensions.ER = ::ConvertToSysUnits(value,pDispUnits->SpanLength.UnitOfMeasure);

   strValue = GetCellValue(row,13);
   sysTokenizer::ParseDouble(strValue, &value);
   dimensions.CR = ::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);

   strValue = GetCellValue(row,14);
   sysTokenizer::ParseDouble(strValue, &value);
   dimensions.BR = ::ConvertToSysUnits(value,pDispUnits->SpanLength.UnitOfMeasure);
}

void CBoxGdrDimensionGrid::InsertRow(const BOXGDRDIMENSIONS& dimensions)
{
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   GetParam()->EnableUndo(FALSE);
	ROWCOL nRow = GetRowCount()+1;

	InsertRows(nRow, 1);

   ROWCOL col = 0;
	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::ConvertFromSysUnits(dimensions.D,pDispUnits->SpanLength.UnitOfMeasure))
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::ConvertFromSysUnits(dimensions.T,pDispUnits->ComponentDim.UnitOfMeasure))
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue((UINT)dimensions.N)
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::ConvertFromSysUnits(dimensions.W,pDispUnits->SpanLength.UnitOfMeasure))
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::ConvertFromSysUnits(dimensions.ST,pDispUnits->ComponentDim.UnitOfMeasure))
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::ConvertFromSysUnits(dimensions.SB,pDispUnits->ComponentDim.UnitOfMeasure))
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::ConvertFromSysUnits(dimensions.FT,pDispUnits->ComponentDim.UnitOfMeasure))
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::ConvertFromSysUnits(dimensions.FB,pDispUnits->ComponentDim.UnitOfMeasure))
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::ConvertFromSysUnits(dimensions.EL,pDispUnits->SpanLength.UnitOfMeasure))
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::ConvertFromSysUnits(dimensions.CL,pDispUnits->ComponentDim.UnitOfMeasure))
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::ConvertFromSysUnits(dimensions.BL,pDispUnits->SpanLength.UnitOfMeasure))
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::ConvertFromSysUnits(dimensions.ER,pDispUnits->SpanLength.UnitOfMeasure))
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::ConvertFromSysUnits(dimensions.CR,pDispUnits->ComponentDim.UnitOfMeasure))
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::ConvertFromSysUnits(dimensions.BR,pDispUnits->SpanLength.UnitOfMeasure))
         );

   SetCurrentCell(nRow, GetLeftCol(), GX_SCROLLINVIEW|GX_DISPLAYEDITWND);
	Invalidate();

   GetParam()->EnableUndo(TRUE);
}

CString CBoxGdrDimensionGrid::GetCellValue(ROWCOL nRow, ROWCOL nCol)
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
BOOL CBoxGdrDimensionGrid::OnValidateCell(ROWCOL nRow, ROWCOL nCol)
{
	CString s;
	CGXControl* pControl = GetControl(nRow, nCol);
	pControl->GetCurrentText(s);

   if ( s.IsEmpty() )
   {
      SetWarningText(_T("Value must be a number"));
      return FALSE;
   }

   if ( nCol == 2 )
   {
      long l;
      if ( !sysTokenizer::ParseLong(s,&l) )
      {
         SetWarningText(_T("Value must be a number"));
         return FALSE;
      }
   }
   else
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
   
void CBoxGdrDimensionGrid::OnModifyCell(ROWCOL nRow,ROWCOL nCol)
{
   CGXGridWnd::OnModifyCell(nRow,nCol);

   CBoxGdrDoc* pDoc = (CBoxGdrDoc*)EAFGetDocument();
   pDoc->UpdateAllViews(nullptr,0,nullptr);
}

void CBoxGdrDimensionGrid::OnUnitsModeChanged()
{
   ROWCOL nRows = GetRowCount();
   if ( 0 < nRows )
   {
      CGXRange range(1, 0, GetRowCount(), 0);
      RemoveRows(range.top, range.bottom);
   }

   UpdateColumnHeaders();
   UpdateData(FALSE);
}

void CBoxGdrDimensionGrid::UpdateColumnHeaders()
{
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   // set text along top row
   ROWCOL col = 0;
	SetStyleRange(CGXRange(0,col++), CGXStyle()
         .SetWrapText(TRUE)
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetEnabled(FALSE)          // disables usage as current cell
			.SetValue(_T("Prob"))
		);

   CString strDimension;
   strDimension.Format(_T("%s (%s)"),_T("D"),pDispUnits->SpanLength.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(strDimension)
		);

   strDimension.Format(_T("%s (%s)"),_T("T"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
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
         .SetValue(_T("No."))
		);

   strDimension.Format(_T("%s (%s)"),_T("W"),pDispUnits->SpanLength.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(strDimension)
		);

   strDimension.Format(_T("%s (%s)"),_T("ST"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(strDimension)
		);

   strDimension.Format(_T("%s (%s)"),_T("SB"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(strDimension)
		);

   strDimension.Format(_T("%s (%s)"),_T("FT"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(strDimension)
		);

   strDimension.Format(_T("%s (%s)"),_T("FB"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(strDimension)
		);

   strDimension.Format(_T("%s (%s)"),_T("EL"),pDispUnits->SpanLength.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(strDimension)
		);

   strDimension.Format(_T("%s (%s)"),_T("CL"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(strDimension)
		);

   strDimension.Format(_T("%s (%s)"),_T("BL"),pDispUnits->SpanLength.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(strDimension)
		);

   strDimension.Format(_T("%s (%s)"),_T("ER"),pDispUnits->SpanLength.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(strDimension)
		);

   strDimension.Format(_T("%s (%s)"),_T("CR"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(strDimension)
		);

   strDimension.Format(_T("%s (%s)"),_T("BR"),pDispUnits->SpanLength.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(strDimension)
		);
}

#if defined _DEBUG
void CBoxGdrDimensionGrid::AssertValid() const
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CGXGridWnd::AssertValid();
}
#endif
