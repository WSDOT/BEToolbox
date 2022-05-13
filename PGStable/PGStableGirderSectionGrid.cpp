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

// PGStableGirderSectionGrid.cpp : implementation file
//

#include "stdafx.h"
#include "..\resource.h"
#include "PGStableGirderSectionGrid.h"
#include "PGStableNonprismaticGirder.h"
#include <EAF\EAFUtilities.h>
#include <EAF\EAFApp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//GRID_IMPLEMENT_REGISTER(CPGStableGirderSectionGrid, CS_DBLCLKS, 0, 0, 0);

/////////////////////////////////////////////////////////////////////////////
// CPGStableGirderSectionGrid

CPGStableGirderSectionGrid::CPGStableGirderSectionGrid()
{
//   RegisterClass();
}

CPGStableGirderSectionGrid::~CPGStableGirderSectionGrid()
{
}

BEGIN_MESSAGE_MAP(CPGStableGirderSectionGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CPGStableGirderSectionGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CPGStableGirderSectionGrid::DoDataExchange(CDataExchange* pDX)
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
// CPGStableGirderSectionGrid message handlers

void CPGStableGirderSectionGrid::RemoveSelectedGirderSections()
{
	CGXRangeList* pSelList = GetParam()->GetRangeList();
	if (pSelList->IsAnyCellFromCol(0) && pSelList->GetCount() == 1)
	{
		CGXRange range = pSelList->GetHead();
		range.ExpandRange(1, 0, GetRowCount(), 0);
		RemoveRows(range.top, range.bottom);
	}

   AFX_MANAGE_STATE(AfxGetAppModuleState());
   CPGStableNonprismaticGirder* pDlg = (CPGStableNonprismaticGirder*)GetParent();
   pDlg->SetGirderLength(GetGirderLength());
}

BOOL CPGStableGirderSectionGrid::AreGirderSectionsSelected()
{
	if (GetParam() == nullptr)
	{
      return FALSE;
	}

	CGXRangeList* pSelList = GetParam()->GetRangeList();
   return (pSelList->IsAnyCellFromCol(0) && pSelList->GetCount() == 1) ? TRUE : FALSE;
}

void CPGStableGirderSectionGrid::CustomInit()
{
   // Initialize the grid. For CWnd based grids this call is // 
   // essential. For view based grids this initialization is done 
   // in OnInitialUpdate.
	Initialize( );

	GetParam( )->EnableUndo(FALSE);

   const ROWCOL num_rows = 1;
   const ROWCOL num_cols = 18;

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

void CPGStableGirderSectionGrid::AddGirderSection()
{
   Float64 L,Ag,Ixx,Iyy,Ixy,Xleft,Ytop,Hg,Wtop,Wbot;
   WBFL::Stability::Point pntTL, pntTR, pntBL, pntBR;
   GetGirderSection(GetRowCount(),&L,&Ag,&Ixx,&Iyy,&Ixy,&Xleft,&Ytop,&Hg,&Wtop,&Wbot,&pntTL,&pntTR,&pntBL,&pntBR);

   InsertGirderSection(L,Ag,Ixx,Iyy,Ixy,Xleft,Ytop,Hg,Wtop,Wbot,pntTL,pntTR,pntBL,pntBR);
   InsertGirderSection(L,Ag,Ixx,Iyy,Ixy,Xleft,Ytop,Hg,Wtop,Wbot, pntTL, pntTR, pntBL, pntBR);

   AFX_MANAGE_STATE(AfxGetAppModuleState());
   CPGStableNonprismaticGirder* pDlg = (CPGStableNonprismaticGirder*)GetParent();
   pDlg->SetGirderLength(GetGirderLength());
}

void CPGStableGirderSectionGrid::GetGirderSection(ROWCOL row,Float64* pL,Float64* pAg,Float64* pIxx,Float64* pIyy,Float64* pIxy,Float64* pXcg,Float64* pYcg,Float64* pHg,Float64* pWtop,Float64* pWbot, WBFL::Stability::Point* pntTL, WBFL::Stability::Point* pntTR, WBFL::Stability::Point* pntBL, WBFL::Stability::Point* pntBR)
{
   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   ROWCOL col = 1;
   CString strValue;
   Float64 value;

   strValue = GetCellValue(row,col++);
   sysTokenizer::ParseDouble(strValue, &value);
   *pHg = WBFL::Units::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);

   strValue = GetCellValue(row,col++);
   sysTokenizer::ParseDouble(strValue, &value);
   *pWtop = WBFL::Units::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);

   strValue = GetCellValue(row,col++);
   sysTokenizer::ParseDouble(strValue, &value);
   *pWbot = WBFL::Units::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);

   strValue = GetCellValue(row,col++);
   sysTokenizer::ParseDouble(strValue, &value);
   *pAg = WBFL::Units::ConvertToSysUnits(value,pDispUnits->Area.UnitOfMeasure);

   strValue = GetCellValue(row,col++);
   sysTokenizer::ParseDouble(strValue, &value);
   *pIxx = WBFL::Units::ConvertToSysUnits(value,pDispUnits->MomentOfInertia.UnitOfMeasure);

   strValue = GetCellValue(row, col++);
   sysTokenizer::ParseDouble(strValue, &value);
   *pIyy = WBFL::Units::ConvertToSysUnits(value, pDispUnits->MomentOfInertia.UnitOfMeasure);

   strValue = GetCellValue(row, col++);
   sysTokenizer::ParseDouble(strValue, &value);
   *pIxy = WBFL::Units::ConvertToSysUnits(value, pDispUnits->MomentOfInertia.UnitOfMeasure);

   strValue = GetCellValue(row, col++);
   sysTokenizer::ParseDouble(strValue, &value);
   *pXcg = WBFL::Units::ConvertToSysUnits(value, pDispUnits->ComponentDim.UnitOfMeasure);

   strValue = GetCellValue(row,col++);
   sysTokenizer::ParseDouble(strValue, &value);
   *pYcg = -1 * WBFL::Units::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);

   strValue = GetCellValue(row,col++);
   sysTokenizer::ParseDouble(strValue, &value);
   *pL = WBFL::Units::ConvertToSysUnits(value,pDispUnits->SpanLength.UnitOfMeasure);

   strValue = GetCellValue(row, col++);
   sysTokenizer::ParseDouble(strValue, &value);
   pntTL->X() = WBFL::Units::ConvertToSysUnits(value, pDispUnits->ComponentDim.UnitOfMeasure);

   strValue = GetCellValue(row, col++);
   sysTokenizer::ParseDouble(strValue, &value);
   pntTL->Y() = WBFL::Units::ConvertToSysUnits(value, pDispUnits->ComponentDim.UnitOfMeasure);

   strValue = GetCellValue(row, col++);
   sysTokenizer::ParseDouble(strValue, &value);
   pntTR->X() = WBFL::Units::ConvertToSysUnits(value, pDispUnits->ComponentDim.UnitOfMeasure);

   strValue = GetCellValue(row, col++);
   sysTokenizer::ParseDouble(strValue, &value);
   pntTR->Y() = WBFL::Units::ConvertToSysUnits(value, pDispUnits->ComponentDim.UnitOfMeasure);

   strValue = GetCellValue(row, col++);
   sysTokenizer::ParseDouble(strValue, &value);
   pntBL->X() = WBFL::Units::ConvertToSysUnits(value, pDispUnits->ComponentDim.UnitOfMeasure);

   strValue = GetCellValue(row, col++);
   sysTokenizer::ParseDouble(strValue, &value);
   pntBL->Y() = WBFL::Units::ConvertToSysUnits(value, pDispUnits->ComponentDim.UnitOfMeasure);

   strValue = GetCellValue(row, col++);
   sysTokenizer::ParseDouble(strValue, &value);
   pntBR->X() = WBFL::Units::ConvertToSysUnits(value, pDispUnits->ComponentDim.UnitOfMeasure);

   strValue = GetCellValue(row, col++);
   sysTokenizer::ParseDouble(strValue, &value);
   pntBR->Y() = WBFL::Units::ConvertToSysUnits(value, pDispUnits->ComponentDim.UnitOfMeasure);
}

void CPGStableGirderSectionGrid::InsertGirderSection(Float64 Length,Float64 Ag,Float64 Ixx,Float64 Iyy,Float64 Ixy,Float64 Xleft,Float64 Ytop,Float64 Hg,Float64 Wtf,Float64 Wbf, const WBFL::Stability::Point& pntTL, const WBFL::Stability::Point& pntTR, const WBFL::Stability::Point& pntBL, const WBFL::Stability::Point& pntBR)
{
   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   GetParam()->EnableUndo(FALSE);
	ROWCOL nRow = GetRowCount()+1;

	InsertRows(nRow, 1);

   ROWCOL col = 0;
	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(nRow-1)
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(Hg,pDispUnits->ComponentDim,false))
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(Wtf,pDispUnits->ComponentDim,false))
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(Wbf,pDispUnits->ComponentDim,false))
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(Ag,pDispUnits->Area,false))
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(Ixx,pDispUnits->MomentOfInertia,false))
         );

   SetStyleRange(CGXRange(nRow, col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(Iyy, pDispUnits->MomentOfInertia, false))
   );

   SetStyleRange(CGXRange(nRow, col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(Ixy, pDispUnits->MomentOfInertia, false))
   );

   SetStyleRange(CGXRange(nRow, col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(Xleft, pDispUnits->ComponentDim, false))
   );

   SetStyleRange(CGXRange(nRow, col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(-Ytop, pDispUnits->ComponentDim, false))
   );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(Length,pDispUnits->SpanLength,false))
         );

   if ( nRow % 2 != 0 )
   {
      col--;
      SetStyleRange(CGXRange(nRow,col++), CGXStyle()
         .SetEnabled(FALSE)
         .SetReadOnly(TRUE)
         .SetInterior(::GetSysColor(COLOR_BTNFACE))
         .SetTextColor(::GetSysColor(COLOR_BTNFACE))
         );
   }

   SetStressPoints(nRow, pntTL, pntTR, pntBL, pntBR);

   SetCurrentCell(nRow, GetLeftCol(), GX_SCROLLINVIEW|GX_DISPLAYEDITWND);
	Invalidate();

   GetParam()->EnableUndo(TRUE);
}

void CPGStableGirderSectionGrid::SetStressPoints(ROWCOL row,const WBFL::Stability::Point& pntTL, const WBFL::Stability::Point& pntTR, const WBFL::Stability::Point& pntBL, const WBFL::Stability::Point& pntBR)
{
   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   GetParam()->EnableUndo(FALSE);

   ROWCOL col = 11;

   SetStyleRange(CGXRange(row, col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(pntTL.X(), pDispUnits->ComponentDim, false))
   );

   SetStyleRange(CGXRange(row, col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(pntTL.Y(), pDispUnits->ComponentDim, false))
   );

   SetStyleRange(CGXRange(row, col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(pntTR.X(), pDispUnits->ComponentDim, false))
   );

   SetStyleRange(CGXRange(row, col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(pntTR.Y(), pDispUnits->ComponentDim, false))
   );

   SetStyleRange(CGXRange(row, col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(pntBL.X(), pDispUnits->ComponentDim, false))
   );

   SetStyleRange(CGXRange(row, col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(pntBL.Y(), pDispUnits->ComponentDim, false))
   );

   SetStyleRange(CGXRange(row, col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(pntBR.X(), pDispUnits->ComponentDim, false))
   );

   SetStyleRange(CGXRange(row, col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(pntBR.Y(), pDispUnits->ComponentDim, false))
   );

   GetParam()->EnableUndo(TRUE);
}

CString CPGStableGirderSectionGrid::GetCellValue(ROWCOL nRow, ROWCOL nCol)
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

// validate input
BOOL CPGStableGirderSectionGrid::OnValidateCell(ROWCOL nRow, ROWCOL nCol)
{
	//CString s;
	//CGXControl* pControl = GetControl(nRow, nCol);
	//pControl->GetCurrentText(s);

 //  if ( s.IsEmpty() )
 //  {
 //     SetWarningText(_T("Value must be a number"));
 //     return FALSE;
 //  }

 //  Float64 d;
 //  if ( !sysTokenizer::ParseDouble(s,&d) )
 //  {
 //     SetWarningText(_T("Value must be a number"));
 //     return FALSE;
 //  }

	return CGXGridWnd::OnValidateCell(nRow, nCol);
}
   
void CPGStableGirderSectionGrid::OnModifyCell(ROWCOL nRow,ROWCOL nCol)
{
   CGXGridWnd::OnModifyCell(nRow,nCol);

   AFX_MANAGE_STATE(AfxGetAppModuleState());
   CPGStableNonprismaticGirder* pDlg = (CPGStableNonprismaticGirder*)GetParent();

   if ( nCol == 9 )
   {
      pDlg->SetGirderLength(GetGirderLength());
   }

   pDlg->OnChange();
}

void CPGStableGirderSectionGrid::UpdateColumnHeaders()
{
   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   SetMergeCellsMode(gxnMergeEvalOnDisplay); // we want to merge cells
   SetFrozenRows(1/*# frozen rows*/, 1/*# extra header rows*/);

   // set text along top row
   ROWCOL col = 0;

   SetStyleRange(CGXRange(0, col, 1, col++), CGXStyle()
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_TOP)
      .SetEnabled(FALSE)          // disables usage as current cell
      .SetMergeCell(GX_MERGE_VERTICAL | GX_MERGE_COMPVALUE)
      .SetValue(_T(" "))
   );


   CString strDimension;
   strDimension.Format(_T("%s\n(%s)"),_T("Height"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col,1,col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)          // disables usage as current cell
      .SetMergeCell(GX_MERGE_VERTICAL | GX_MERGE_COMPVALUE)
      .SetValue(strDimension)
	);

   strDimension.Format(_T("%s\n(%s)"),_T("Wtf"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
   SetStyleRange(CGXRange(0, col, 1, col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
      .SetEnabled(FALSE)          // disables usage as current cell
      .SetMergeCell(GX_MERGE_VERTICAL | GX_MERGE_COMPVALUE)
      .SetValue(strDimension)
   );

   strDimension.Format(_T("%s\n(%s)"),_T("Wbf"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
   SetStyleRange(CGXRange(0, col, 1, col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
      .SetEnabled(FALSE)          // disables usage as current cell
      .SetMergeCell(GX_MERGE_VERTICAL | GX_MERGE_COMPVALUE)
      .SetValue(strDimension)
   );

   strDimension.Format(_T("%s\n(%s)"),_T("Area"),pDispUnits->Area.UnitOfMeasure.UnitTag().c_str());
   SetStyleRange(CGXRange(0, col, 1, col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
      .SetEnabled(FALSE)          // disables usage as current cell
      .SetMergeCell(GX_MERGE_VERTICAL | GX_MERGE_COMPVALUE)
      .SetValue(strDimension)
   );

   strDimension.Format(_T("%s\n(%s)"),_T("Ixx"),pDispUnits->MomentOfInertia.UnitOfMeasure.UnitTag().c_str());
   SetStyleRange(CGXRange(0, col, 1, col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
      .SetEnabled(FALSE)          // disables usage as current cell
      .SetMergeCell(GX_MERGE_VERTICAL | GX_MERGE_COMPVALUE)
      .SetValue(strDimension)
   );

   strDimension.Format(_T("%s\n(%s)"), _T("Iyy"), pDispUnits->MomentOfInertia.UnitOfMeasure.UnitTag().c_str());
   SetStyleRange(CGXRange(0, col, 1, col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
      .SetEnabled(FALSE)          // disables usage as current cell
      .SetMergeCell(GX_MERGE_VERTICAL | GX_MERGE_COMPVALUE)
      .SetValue(strDimension)
   );

   strDimension.Format(_T("%s\n(%s)"), _T("Ixy"), pDispUnits->MomentOfInertia.UnitOfMeasure.UnitTag().c_str());
   SetStyleRange(CGXRange(0, col, 1, col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
      .SetEnabled(FALSE)          // disables usage as current cell
      .SetMergeCell(GX_MERGE_VERTICAL | GX_MERGE_COMPVALUE)
      .SetValue(strDimension)
   );

   strDimension.Format(_T("%s\n(%s)"), _T("Xleft"), pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
   SetStyleRange(CGXRange(0, col, 1, col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
      .SetEnabled(FALSE)          // disables usage as current cell
      .SetMergeCell(GX_MERGE_VERTICAL | GX_MERGE_COMPVALUE)
      .SetValue(strDimension)
   );

   strDimension.Format(_T("%s\n(%s)"), _T("Ytop"), pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
   SetStyleRange(CGXRange(0, col, 1, col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
      .SetEnabled(FALSE)          // disables usage as current cell
      .SetMergeCell(GX_MERGE_VERTICAL | GX_MERGE_COMPVALUE)
      .SetValue(strDimension)
   );

   strDimension.Format(_T("%s\n(%s)"),_T("L"),pDispUnits->SpanLength.UnitOfMeasure.UnitTag().c_str());
   SetStyleRange(CGXRange(0, col, 1, col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
      .SetEnabled(FALSE)          // disables usage as current cell
      .SetMergeCell(GX_MERGE_VERTICAL | GX_MERGE_COMPVALUE)
      .SetValue(strDimension)
   );

   SetStyleRange(CGXRange(0, col, 0, col + 1), CGXStyle()
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_TOP)
      .SetEnabled(FALSE)
      .SetValue(_T("Top Left"))
      .SetMergeCell(GX_MERGE_HORIZONTAL | GX_MERGE_COMPVALUE)
   );

   strDimension.Format(_T("%s (%s)"), _T("X"), pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
   SetStyleRange(CGXRange(1, col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
      .SetEnabled(FALSE)          // disables usage as current cell
      .SetValue(strDimension)
   );

   strDimension.Format(_T("%s (%s)"), _T("Y"), pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
   SetStyleRange(CGXRange(1, col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
      .SetEnabled(FALSE)          // disables usage as current cell
      .SetValue(strDimension)
   );


   SetStyleRange(CGXRange(0, col, 0, col + 1), CGXStyle()
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_TOP)
      .SetEnabled(FALSE)
      .SetValue(_T("Top Right"))
      .SetMergeCell(GX_MERGE_HORIZONTAL | GX_MERGE_COMPVALUE)
   );

   strDimension.Format(_T("%s (%s)"), _T("X"), pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
   SetStyleRange(CGXRange(1, col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
      .SetEnabled(FALSE)          // disables usage as current cell
      .SetValue(strDimension)
   );

   strDimension.Format(_T("%s (%s)"), _T("Y"), pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
   SetStyleRange(CGXRange(1, col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
      .SetEnabled(FALSE)          // disables usage as current cell
      .SetValue(strDimension)
   );


   SetStyleRange(CGXRange(0, col, 0, col + 1), CGXStyle()
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_TOP)
      .SetEnabled(FALSE)
      .SetValue(_T("Bottom Left"))
      .SetMergeCell(GX_MERGE_HORIZONTAL | GX_MERGE_COMPVALUE)
   );

   strDimension.Format(_T("%s (%s)"), _T("X"), pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
   SetStyleRange(CGXRange(1, col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
      .SetEnabled(FALSE)          // disables usage as current cell
      .SetValue(strDimension)
   );

   strDimension.Format(_T("%s (%s)"), _T("Y"), pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
   SetStyleRange(CGXRange(1, col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
      .SetEnabled(FALSE)          // disables usage as current cell
      .SetValue(strDimension)
   );


   SetStyleRange(CGXRange(0, col, 0, col + 1), CGXStyle()
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_TOP)
      .SetEnabled(FALSE)
      .SetValue(_T("Bottom Right"))
      .SetMergeCell(GX_MERGE_HORIZONTAL | GX_MERGE_COMPVALUE)
   );

   strDimension.Format(_T("%s (%s)"), _T("X"), pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
   SetStyleRange(CGXRange(1, col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
      .SetEnabled(FALSE)          // disables usage as current cell
      .SetValue(strDimension)
   );

   strDimension.Format(_T("%s (%s)"), _T("Y"), pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
   SetStyleRange(CGXRange(1, col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
      .SetEnabled(FALSE)          // disables usage as current cell
      .SetValue(strDimension)
   );

   ResizeColWidthsToFit(CGXRange(0, 0, GetRowCount(), GetColCount()));
}

void CPGStableGirderSectionGrid::FillGrid(const WBFL::Stability::Girder& girder)
{
   ROWCOL nRows = GetRowCount();
   if ( 1 < nRows )
   {
      CGXRange range(2, 0, GetRowCount(), 0);
      RemoveRows(range.top, range.bottom);
   }

   IndexType nSections = girder.GetSectionCount();
   for ( IndexType sectIdx = 0; sectIdx < nSections; sectIdx++ )
   {
      Float64 L = girder.GetSectionLength(sectIdx);
      Float64 Ag,Ixx,Iyy,Ixy,Xleft,Ytop,Hg,Wtf,Wbf;
      WBFL::Stability::Point pntTL, pntTR, pntBL, pntBR;
      for ( int s = 0; s < 2; s++ )
      {
         WBFL::Stability::Section section = (WBFL::Stability::Section)s;
         girder.GetSectionProperties(sectIdx,section,&Ag,&Ixx,&Iyy,&Ixy,&Xleft,&Ytop,&Hg,&Wtf,&Wbf);
         girder.GetStressPoints(sectIdx, section, &pntTL, &pntTR, &pntBL, &pntBR);
         InsertGirderSection(L, Ag, Ixx, Iyy, Ixy, Xleft, Ytop, Hg, Wtf, Wbf, pntTL, pntTR, pntBL, pntBR);
      }
   }

   //UpdateColumnHeaders();

   //HideCols(8, 8); // don't show Xleft column... we are only doing symmetric girders

   ResizeColWidthsToFit(CGXRange(0, 0, GetRowCount(), GetColCount()));

   AFX_MANAGE_STATE(AfxGetAppModuleState());
   CPGStableNonprismaticGirder* pDlg = (CPGStableNonprismaticGirder*)GetParent();
   pDlg->SetGirderLength(GetGirderLength());
}

void CPGStableGirderSectionGrid::GetGirderSections(WBFL::Stability::Girder& girder)
{
   girder.ClearSections();
   ROWCOL nRows = GetRowCount();
   for ( ROWCOL row = 1; row < nRows; row += 2 )
   {
      Float64 L[2],Ag[2],Ixx[2],Iyy[2],Ixy[2],Xleft[2],Ytop[2],Hg[2],Wtf[2],Wbf[2];
      WBFL::Stability::Point pntTL[2], pntTR[2], pntBL[2], pntBR[2];

      GetGirderSection(row+1,&L[WBFL::Stability::Start], 
                             &Ag[WBFL::Stability::Start], 
                             &Ixx[WBFL::Stability::Start], 
                             &Iyy[WBFL::Stability::Start], 
                             &Ixy[WBFL::Stability::Start],
                             &Xleft[WBFL::Stability::Start],
                             &Ytop[WBFL::Stability::Start],
                             &Hg[WBFL::Stability::Start],
                             &Wtf[WBFL::Stability::Start], 
                             &Wbf[WBFL::Stability::Start],
                             &pntTL[WBFL::Stability::Start],
                             &pntTR[WBFL::Stability::Start],
                             &pntBL[WBFL::Stability::Start],
                             &pntBR[WBFL::Stability::Start]
                             );

      GetGirderSection(row+2,&L[WBFL::Stability::End],
                             &Ag[WBFL::Stability::End],
                             &Ixx[WBFL::Stability::End],
                             &Iyy[WBFL::Stability::End],
                             &Ixy[WBFL::Stability::End],
                             &Xleft[WBFL::Stability::End],
                             &Ytop[WBFL::Stability::End],
                             &Hg[WBFL::Stability::End],
                             &Wtf[WBFL::Stability::End],
                             &Wbf[WBFL::Stability::End],
                             &pntTL[WBFL::Stability::End],
                             &pntTR[WBFL::Stability::End],
                             &pntBL[WBFL::Stability::End],
                             &pntBR[WBFL::Stability::End]
                             );

      // NOTE: L[WBFL::Stability::Start] is the valid length... the L cell for the second row is disabled and will be out of date if the length changed
      IndexType sectIdx = girder.AddSection(L[WBFL::Stability::Start],Ag[WBFL::Stability::Start], 
                                                    Ixx[WBFL::Stability::Start], 
                                                    Iyy[WBFL::Stability::Start],
                                                    Ixy[WBFL::Stability::Start],
                                                    Xleft[WBFL::Stability::Start],
                                                    Ytop[WBFL::Stability::Start], 
                                                    Hg[WBFL::Stability::Start], 
                                                    Wtf[WBFL::Stability::Start], 
                                                    Wbf[WBFL::Stability::Start],
                                                    Ag[WBFL::Stability::End],
                                                    Ixx[WBFL::Stability::End],
                                                    Iyy[WBFL::Stability::End],
                                                    Ixy[WBFL::Stability::End],
                                                    Xleft[WBFL::Stability::End],
                                                    Ytop[WBFL::Stability::End],
                                                    Hg[WBFL::Stability::End],
                                                    Wtf[WBFL::Stability::End],
                                                    Wbf[WBFL::Stability::End]);

      girder.SetStressPoints(sectIdx, pntTL[WBFL::Stability::Start], pntTR[WBFL::Stability::Start], pntBL[WBFL::Stability::Start], pntBR[WBFL::Stability::Start], pntTL[WBFL::Stability::End], pntTR[WBFL::Stability::End], pntBL[WBFL::Stability::End], pntBR[WBFL::Stability::End]);
   }
}

Float64 CPGStableGirderSectionGrid::GetGirderLength()
{
   Float64 Lg = 0;
   ROWCOL nRows = GetRowCount();
   for ( ROWCOL row = 1; row < nRows; row += 2 )
   {
      Float64 L,Ag,Ixx,Iyy,Ixy,Xleft,Ytop,Hg,Wtf,Wbf;
      WBFL::Stability::Point pntTL, pntTR, pntBL, pntBR;
      GetGirderSection(row+1,&L,&Ag,&Ixx,&Iyy,&Ixy,&Xleft,&Ytop,&Hg,&Wtf,&Wbf,&pntTL,&pntTR,&pntBL,&pntBR);
      Lg += L;
   }
   return Lg;
}

std::vector<std::pair<Float64,Float64>> CPGStableGirderSectionGrid::GetGirderProfile()
{
   std::vector<std::pair<Float64,Float64>> vProfile;
   ROWCOL nRows = GetRowCount();
   Float64 X = 0;
   for ( ROWCOL row = 1; row < nRows; row += 2 )
   {
      Float64 L,Ag,Ixx,Iyy,Ixy,Xleft,Ytop,Hg,Wtf,Wbf;
      WBFL::Stability::Point pntTL, pntTR, pntBL, pntBR;
      GetGirderSection(row+1,&L,&Ag,&Ixx,&Iyy,&Ixy,&Xleft,&Ytop,&Hg,&Wtf,&Wbf, &pntTL, &pntTR, &pntBL, &pntBR);
      vProfile.emplace_back(X,-Hg);

      X += L;

      GetGirderSection(row+2,&L,&Ag,&Ixx,&Iyy,&Ixy,&Xleft,&Ytop,&Hg,&Wtf,&Wbf, &pntTL, &pntTR, &pntBL, &pntBR);
      vProfile.emplace_back(X,-Hg);
   }
   return vProfile;
}

std::vector<StressPoints> CPGStableGirderSectionGrid::GetStressPoints()
{
   std::vector<StressPoints> vStressPoints;
   ROWCOL nRows = GetRowCount();
   for (ROWCOL row = 1; row < nRows; row += 2)
   {
      Float64 L, Ag, Ixx, Iyy, Ixy, Xleft, Ytop, Hg, Wtf, Wbf;

      StressPoints sp;
      GetGirderSection(row + 1, &L, &Ag, &Ixx, &Iyy, &Ixy, &Xleft, &Ytop, &Hg, &Wtf, &Wbf, &sp.pntTL[WBFL::Stability::Start], &sp.pntTR[WBFL::Stability::Start], &sp.pntBL[WBFL::Stability::Start], &sp.pntBR[WBFL::Stability::Start]);
      GetGirderSection(row + 2, &L, &Ag, &Ixx, &Iyy, &Ixy, &Xleft, &Ytop, &Hg, &Wtf, &Wbf, &sp.pntTL[WBFL::Stability::End], &sp.pntTR[WBFL::Stability::End], &sp.pntBL[WBFL::Stability::End], &sp.pntBR[WBFL::Stability::End]);

      vStressPoints.push_back(sp);
   }
   return vStressPoints;
}

void CPGStableGirderSectionGrid::SetStressPoints(const std::vector<StressPoints>& vStressPoints)
{
   ATLASSERT(vStressPoints.size() == (GetRowCount() - 1)/2);
   ROWCOL row = 2;
   for (const auto& sp : vStressPoints)
   {
      SetStressPoints(row,   sp.pntTL[WBFL::Stability::Start], sp.pntTR[WBFL::Stability::Start], sp.pntBL[WBFL::Stability::Start], sp.pntBR[WBFL::Stability::Start]);
      SetStressPoints(row+1, sp.pntTL[WBFL::Stability::End],   sp.pntTR[WBFL::Stability::End],   sp.pntBL[WBFL::Stability::End],   sp.pntBR[WBFL::Stability::End]);

      row += 2;
   }
}

#if defined _DEBUG
void CPGStableGirderSectionGrid::AssertValid() const
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CGXGridWnd::AssertValid();
}
#endif


BOOL CPGStableGirderSectionGrid::OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
{
   SelectRow(nRow);

   return TRUE;
}

void CPGStableGirderSectionGrid::SelectRow(ROWCOL nRow)
{
   ROWCOL nCols = GetColCount();

   // unselect currently selected rows
   CRowColArray awRows;
   GetSelectedRows(awRows);
   INT_PTR cnt = awRows.GetSize();
   for(INT_PTR idx = 0; idx < cnt; idx++)
   {
      ROWCOL row = awRows[idx];
      SelectRange(CGXRange(row,0,row,nCols), FALSE);
   }

   if (1 < nRow)
   {
      ROWCOL topRow,botRow;
      if ( ::IsOdd(nRow) )
      {
         topRow = nRow-1;
         botRow = nRow;
      }
      else
      {
         topRow = nRow;
         botRow = nRow+1;
      }

      SelectRange(CGXRange(topRow,0,botRow,nCols), TRUE);
   }
}

void CPGStableGirderSectionGrid::ComputeStressPoints()
{
   ROWCOL nRows = GetRowCount();
   for (ROWCOL row = 1; row < nRows; row += 2)
   {
      Float64 L, Ag[2], Ixx[2], Iyy[2], Ixy[2], Xleft[2], Ytop[2], Hg[2], Wtf[2], Wbf[2];

      StressPoints sp;
      GetGirderSection(row + 1, &L, &Ag[WBFL::Stability::Start], &Ixx[WBFL::Stability::Start], &Iyy[WBFL::Stability::Start], &Ixy[WBFL::Stability::Start], &Xleft[WBFL::Stability::Start], &Ytop[WBFL::Stability::Start], &Hg[WBFL::Stability::Start], &Wtf[WBFL::Stability::Start], &Wbf[WBFL::Stability::Start], &sp.pntTL[WBFL::Stability::Start], &sp.pntTR[WBFL::Stability::Start], &sp.pntBL[WBFL::Stability::Start], &sp.pntBR[WBFL::Stability::Start]);
      GetGirderSection(row + 2, &L, &Ag[WBFL::Stability::End],   &Ixx[WBFL::Stability::End],   &Iyy[WBFL::Stability::End],   &Ixy[WBFL::Stability::End],   &Xleft[WBFL::Stability::End],   &Ytop[WBFL::Stability::End],   &Hg[WBFL::Stability::End],   &Wtf[WBFL::Stability::End],   &Wbf[WBFL::Stability::End],   &sp.pntTL[WBFL::Stability::End],   &sp.pntTR[WBFL::Stability::End],   &sp.pntBL[WBFL::Stability::End],   &sp.pntBR[WBFL::Stability::End]);

      for (int i = 0; i < 2; i++)
      {
         WBFL::Stability::Section section = (WBFL::Stability::Section)i;
         sp.pntTL[section].X() = (Wbf[section] < Wtf[section] ? -Xleft[section] : Wbf[section]/2 - Xleft[section] - Wtf[section] / 2);
         sp.pntTL[section].Y() = -Ytop[section];

         sp.pntTR[section].X() = (Wbf[section] < Wtf[section] ? Wtf[section] - Xleft[section] : Wtf[section] / 2 - Xleft[section] + Wbf[section]/2);
         sp.pntTR[section].Y() = -Ytop[section];

         sp.pntBL[section].X() = (Wbf[section] < Wtf[section] ? Wtf[section]/2 - Xleft[section] - Wbf[section] / 2 : -Xleft[section]);
         sp.pntBL[section].Y() = -(Ytop[section] + Hg[section]);

         sp.pntBR[section].X() = (Wbf[section] < Wtf[section] ? Wbf[section] / 2 - Xleft[section] + Wtf[section]/2 : Wbf[section] - Xleft[section]);
         sp.pntBR[section].Y() = -(Ytop[section] + Hg[section]);

         SetStressPoints(row + i + 1, sp.pntTL[section], sp.pntTR[section], sp.pntBL[section], sp.pntBR[section]);
      }
   }
}

void CPGStableGirderSectionGrid::EnableStressPoints(BOOL bEnable)
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

   SetStyleRange(CGXRange(2, 11, GetRowCount(), GetColCount()), style);

   GetParam()->EnableUndo(TRUE);
   GetParam()->SetLockReadOnly(TRUE);
}

void CPGStableGirderSectionGrid::OnUnitsChanged()
{
   UpdateColumnHeaders();
}