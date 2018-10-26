///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2016  Washington State Department of Transportation
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
	if (GetParam() == NULL)
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

   // don't allow users to resize grids
   GetParam( )->EnableTrackColWidth(0); 
   GetParam( )->EnableTrackRowHeight(0); 

	SetFocus();

	GetParam( )->EnableUndo(TRUE);

//   EnableIntelliMouse();
}

void CPGStableFpeGrid::AddFpe()
{
   Float64 X,FpeStraight,FpeHarped,FpeTemp;
   GetFpe(GetRowCount(),&X,&FpeStraight,&FpeHarped,&FpeTemp);
   InsertFpe(X,FpeStraight,FpeHarped,FpeTemp);
}

void CPGStableFpeGrid::GetFpe(ROWCOL row,Float64* pX,Float64* pFpeStraight,Float64* pFpeHarped,Float64* pFpeTemp)
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
   *pFpeHarped = ::ConvertToSysUnits(value,pDispUnits->GeneralForce.UnitOfMeasure);

   strValue = GetCellValue(row,col++);
   sysTokenizer::ParseDouble(strValue, &value);
   *pFpeTemp = ::ConvertToSysUnits(value,pDispUnits->GeneralForce.UnitOfMeasure);
}

void CPGStableFpeGrid::InsertFpe(Float64 X,Float64 FpeStraight,Float64 FpeHarped,Float64 FpeTemp)
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
      .SetValue(::FormatDimension(X,pDispUnits->SpanLength,false))
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(FpeStraight,pDispUnits->GeneralForce,false))
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(FpeHarped,pDispUnits->GeneralForce,false))
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(FpeTemp,pDispUnits->GeneralForce,false))
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

   Float64 d;
   if ( !sysTokenizer::ParseDouble(s,&d) )
   {
      SetWarningText(_T("Value must be a number"));
      return FALSE;
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
   ROWCOL col = 1;
   CString strDimension;
   strDimension.Format(_T("%s (%s)"),_T("X"),pDispUnits->SpanLength.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)          // disables usage as current cell
		.SetValue(strDimension)
	);

   strDimension.Format(_T("Fpe\r\n%s (%s)"),_T("Straight"),pDispUnits->GeneralForce.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)          // disables usage as current cell
		.SetValue(strDimension)
	);

   strDimension.Format(_T("Fpe\r\n%s (%s)"),_T("Harped"),pDispUnits->GeneralForce.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)          // disables usage as current cell
		.SetValue(strDimension)
	);

   strDimension.Format(_T("Fpe\r\n%s (%s)"),_T("Temporary"),pDispUnits->GeneralForce.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)          // disables usage as current cell
		.SetValue(strDimension)
	);
}

void CPGStableFpeGrid::FillGrid(const std::set<stbFpe>& vFpe)
{
   ROWCOL nRows = GetRowCount();
   if ( 0 < nRows )
   {
      CGXRange range(1, 0, GetRowCount(), 0);
      RemoveRows(range.top, range.bottom);
   }

   BOOST_FOREACH(const stbFpe& fpe,vFpe)
   {
      InsertFpe(fpe.X,fpe.fpeStraight,fpe.fpeHarped,fpe.fpeTemporary);
   }
}

void CPGStableFpeGrid::GetFpe(std::set<stbFpe>& vFpe)
{
   vFpe.clear();
   
   ROWCOL nRows = GetRowCount();
   for ( ROWCOL row = 0; row < nRows; row++ )
   {
      stbFpe fpe;

      GetFpe(row+1,&fpe.X,&fpe.fpeStraight,&fpe.fpeHarped,&fpe.fpeTemporary);
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
