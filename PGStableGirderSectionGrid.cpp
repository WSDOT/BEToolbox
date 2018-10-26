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

// PGStableGirderSectionGrid.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
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
	if (GetParam() == NULL)
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

   const ROWCOL num_rows = 0;
   const ROWCOL num_cols = 8;

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
   Float64 L,Ag,Ix,Iy,Yt,Hg,Wtop,Wbot;
   GetGirderSection(GetRowCount(),&L,&Ag,&Ix,&Iy,&Yt,&Hg,&Wtop,&Wbot);
   InsertGirderSection(L,Ag,Ix,Iy,Yt,Hg,Wtop,Wbot);
   InsertGirderSection(L,Ag,Ix,Iy,Yt,Hg,Wtop,Wbot);

   AFX_MANAGE_STATE(AfxGetAppModuleState());
   CPGStableNonprismaticGirder* pDlg = (CPGStableNonprismaticGirder*)GetParent();
   pDlg->SetGirderLength(GetGirderLength());
}

void CPGStableGirderSectionGrid::GetGirderSection(ROWCOL row,Float64* pL,Float64* pAg,Float64* pIx,Float64* pIy,Float64* pYt,Float64* pHg,Float64* pWtop,Float64* pWbot)
{
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   ROWCOL col = 1;
   CString strValue;
   Float64 value;

   strValue = GetCellValue(row,col++);
   sysTokenizer::ParseDouble(strValue, &value);
   *pHg = ::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);

   strValue = GetCellValue(row,col++);
   sysTokenizer::ParseDouble(strValue, &value);
   *pWtop = ::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);

   strValue = GetCellValue(row,col++);
   sysTokenizer::ParseDouble(strValue, &value);
   *pWbot = ::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);

   strValue = GetCellValue(row,col++);
   sysTokenizer::ParseDouble(strValue, &value);
   *pAg = ::ConvertToSysUnits(value,pDispUnits->Area.UnitOfMeasure);

   strValue = GetCellValue(row,col++);
   sysTokenizer::ParseDouble(strValue, &value);
   *pIx = ::ConvertToSysUnits(value,pDispUnits->MomentOfInertia.UnitOfMeasure);

   strValue = GetCellValue(row,col++);
   sysTokenizer::ParseDouble(strValue, &value);
   *pIy = ::ConvertToSysUnits(value,pDispUnits->MomentOfInertia.UnitOfMeasure);

   strValue = GetCellValue(row,col++);
   sysTokenizer::ParseDouble(strValue, &value);
   *pYt = -1 * ::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);

   strValue = GetCellValue(row,col++);
   sysTokenizer::ParseDouble(strValue, &value);
   *pL = ::ConvertToSysUnits(value,pDispUnits->SpanLength.UnitOfMeasure);
}

void CPGStableGirderSectionGrid::InsertGirderSection(Float64 Length,Float64 Ag,Float64 Ix,Float64 Iy,Float64 Yt,Float64 Hg,Float64 Wtf,Float64 Wbf)
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
      .SetValue(::FormatDimension(Ix,pDispUnits->MomentOfInertia,false))
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(Iy,pDispUnits->MomentOfInertia,false))
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(-Yt,pDispUnits->ComponentDim,false))
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::FormatDimension(Length,pDispUnits->SpanLength,false))
         );

   if ( nRow % 2 == 0 )
   {
      col--;
      SetStyleRange(CGXRange(nRow,col++), CGXStyle()
         .SetEnabled(FALSE)
         .SetReadOnly(TRUE)
         .SetInterior(::GetSysColor(COLOR_BTNFACE))
         .SetTextColor(::GetSysColor(COLOR_BTNFACE))
         );
   }

   SetCurrentCell(nRow, GetLeftCol(), GX_SCROLLINVIEW|GX_DISPLAYEDITWND);
	Invalidate();

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
        return GetValueRowCol(nRow, nCol);
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

   if ( nCol == 8 )
   {
      pDlg->SetGirderLength(GetGirderLength());
   }

   pDlg->OnChange();
}

void CPGStableGirderSectionGrid::UpdateColumnHeaders()
{
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   // set text along top row
   ROWCOL col = 1;
   CString strDimension;
   strDimension.Format(_T("%s (%s)"),_T("Height"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)          // disables usage as current cell
		.SetValue(strDimension)
	);

   strDimension.Format(_T("%s (%s)"),_T("Wtf"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)          // disables usage as current cell
		.SetValue(strDimension)
	);

   strDimension.Format(_T("%s (%s)"),_T("Wbf"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)          // disables usage as current cell
		.SetValue(strDimension)
	);

   strDimension.Format(_T("%s (%s)"),_T("Area"),pDispUnits->Area.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)          // disables usage as current cell
		.SetValue(strDimension)
	);

   strDimension.Format(_T("%s (%s)"),_T("Ix"),pDispUnits->MomentOfInertia.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)          // disables usage as current cell
		.SetValue(strDimension)
	);

   strDimension.Format(_T("%s (%s)"),_T("Iy"),pDispUnits->MomentOfInertia.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)          // disables usage as current cell
		.SetValue(strDimension)
	);

   strDimension.Format(_T("%s (%s)"),_T("Yt"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)          // disables usage as current cell
		.SetValue(strDimension)
	);

   strDimension.Format(_T("%s (%s)"),_T("L"),pDispUnits->SpanLength.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col++), CGXStyle()
      .SetWrapText(TRUE)
      .SetHorizontalAlignment(DT_CENTER)
      .SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)          // disables usage as current cell
		.SetValue(strDimension)
	);
}

void CPGStableGirderSectionGrid::FillGrid(const stbGirder& girder)
{
   ROWCOL nRows = GetRowCount();
   if ( 0 < nRows )
   {
      CGXRange range(1, 0, GetRowCount(), 0);
      RemoveRows(range.top, range.bottom);
   }

   IndexType nSections = girder.GetSectionCount();
   for ( IndexType sectIdx = 0; sectIdx < nSections; sectIdx++ )
   {
      Float64 L = girder.GetSectionLength(sectIdx);
      Float64 Ag[2],Ix[2],Iy[2],Yt[2],Hg[2],Wtf[2],Wbf[2];
      for ( int s = 0; s < 2; s++ )
      {
         stbTypes::Section section = (stbTypes::Section)s;
         girder.GetSectionProperties(sectIdx,section,&Ag[section],&Ix[section],&Iy[section],&Yt[section],&Hg[section],&Wtf[section],&Wbf[section]);
         InsertGirderSection(L,Ag[section],Ix[section],Iy[section],Yt[section],Hg[section],Wtf[section],Wbf[section]);
      }
   }

   UpdateColumnHeaders();

   AFX_MANAGE_STATE(AfxGetAppModuleState());
   CPGStableNonprismaticGirder* pDlg = (CPGStableNonprismaticGirder*)GetParent();
   pDlg->SetGirderLength(GetGirderLength());
}

void CPGStableGirderSectionGrid::GetGirderSections(stbGirder& girder)
{
   girder.ClearSections();
   ROWCOL nRows = GetRowCount();
   for ( ROWCOL row = 0; row < nRows; row += 2 )
   {
      Float64 L[2],Ag[2],Ix[2],Iy[2],Yt[2],Hg[2],Wtf[2],Wbf[2];
      GetGirderSection(row+1,&L[stbTypes::Start], 
                             &Ag[stbTypes::Start], 
                             &Ix[stbTypes::Start], 
                             &Iy[stbTypes::Start], 
                             &Yt[stbTypes::Start], 
                             &Hg[stbTypes::Start], 
                             &Wtf[stbTypes::Start], 
                             &Wbf[stbTypes::Start]);

      GetGirderSection(row+2,&L[stbTypes::End],
                             &Ag[stbTypes::End],
                             &Ix[stbTypes::End],
                             &Iy[stbTypes::End],
                             &Yt[stbTypes::End],
                             &Hg[stbTypes::End],
                             &Wtf[stbTypes::End],
                             &Wbf[stbTypes::End]);

      // NOTE: L[stbTypes::Start] is the valid length... the L cell for the second row is disabled and will be out of date if the length changed
      girder.AddSection(L[stbTypes::Start],Ag[stbTypes::Start], 
                                                    Ix[stbTypes::Start], 
                                                    Iy[stbTypes::Start], 
                                                    Yt[stbTypes::Start], 
                                                    Hg[stbTypes::Start], 
                                                    Wtf[stbTypes::Start], 
                                                    Wbf[stbTypes::Start],
                                                    Ag[stbTypes::End],
                                                    Ix[stbTypes::End],
                                                    Iy[stbTypes::End],
                                                    Yt[stbTypes::End],
                                                    Hg[stbTypes::End],
                                                    Wtf[stbTypes::End],
                                                    Wbf[stbTypes::End]);
   }
}

Float64 CPGStableGirderSectionGrid::GetGirderLength()
{
   Float64 Lg = 0;
   ROWCOL nRows = GetRowCount();
   for ( ROWCOL row = 0; row < nRows; row += 2 )
   {
      Float64 L,Ag,Ix,Iy,Yt,Hg,Wtf,Wbf;
      GetGirderSection(row+1,&L,&Ag,&Ix,&Iy,&Yt,&Hg,&Wtf,&Wbf);
      Lg += L;
   }
   return Lg;
}

std::vector<std::pair<Float64,Float64>> CPGStableGirderSectionGrid::GetGirderProfile()
{
   std::vector<std::pair<Float64,Float64>> vProfile;
   ROWCOL nRows = GetRowCount();
   Float64 X = 0;
   for ( ROWCOL row = 0; row < nRows; row += 2 )
   {
      Float64 L,Ag,Ix,Iy,Yt,Hg,Wtf,Wbf;
      GetGirderSection(row+1,&L,&Ag,&Ix,&Iy,&Yt,&Hg,&Wtf,&Wbf);
      vProfile.push_back(std::make_pair(X,-Hg));

      X += L;

      GetGirderSection(row+2,&L,&Ag,&Ix,&Iy,&Yt,&Hg,&Wtf,&Wbf);
      vProfile.push_back(std::make_pair(X,-Hg));
   }
   return vProfile;
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

   if (0 < nRow)
   {
      ROWCOL topRow,botRow;
      if ( ::IsEven(nRow) )
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
