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

// GirCompDiaphramGrid.cpp : implementation file
//

#include "stdafx.h"
#include "GirCompDimensionGrid.h"
#include <EAF\EAFUtilities.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//GRID_IMPLEMENT_REGISTER(CGirCompDimensionGrid, CS_DBLCLKS, 0, 0, 0);

/////////////////////////////////////////////////////////////////////////////
// CGirCompDimensionGrid

CGirCompDimensionGrid::CGirCompDimensionGrid()
{
//   RegisterClass();
}

CGirCompDimensionGrid::~CGirCompDimensionGrid()
{
}

BEGIN_MESSAGE_MAP(CGirCompDimensionGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CGirCompDimensionGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CGirCompDimensionGrid::DoDataExchange(CDataExchange* pDX)
{
   CGirCompDoc* pDoc = (CGirCompDoc*)EAFGetDocument();
   if ( pDX->m_bSaveAndValidate )
   {
      std::vector<GIRCOMPDIMENSIONS> problems;
      ROWCOL nRows = GetRowCount();
      for ( ROWCOL row = 1; row < nRows; row++ )
      {
         GIRCOMPDIMENSIONS dimensions;
         GetProblemData(row+1,dimensions);
         problems.push_back(dimensions);
      }

      pDoc->ClearProblems();
      pDoc->AddProblems(problems);
   }
   else
   {
      if ( 1 < GetRowCount() )
         RemoveRows(2,GetRowCount());

      IndexType nProblems = pDoc->GetProblemCount();
      for ( IndexType idx = 0; idx < nProblems; idx++ )
      {
         const GIRCOMPDIMENSIONS& problem = pDoc->GetProblem(idx);
         InsertRow(problem);
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
// CGirCompDimensionGrid message handlers

void CGirCompDimensionGrid::RemoveSelectedProblems()
{
	CGXRangeList* pSelList = GetParam()->GetRangeList();
	if (pSelList->IsAnyCellFromCol(0) && pSelList->GetCount() == 1)
	{
		CGXRange range = pSelList->GetHead();
		range.ExpandRange(1, 0, GetRowCount(), 0);
		RemoveRows(range.top, range.bottom);
	}

   UpdateData(TRUE);
   CGirCompDoc* pDoc = (CGirCompDoc*)EAFGetDocument();
   pDoc->UpdateAllViews(nullptr,0,nullptr);
}

BOOL CGirCompDimensionGrid::AreProblemsSelected()
{
	if (GetParam() == nullptr)
	{
      return FALSE;
	}

	CGXRangeList* pSelList = GetParam()->GetRangeList();
   return (pSelList->IsAnyCellFromCol(0) && pSelList->GetCount() == 1) ? TRUE : FALSE;
}

void CGirCompDimensionGrid::CustomInit()
{
   // Initialize the grid. For CWnd based grids this call is // 
   // essential. For view based grids this initialization is done 
   // in OnInitialUpdate.
	Initialize( );

	GetParam( )->EnableUndo(FALSE);

   const ROWCOL num_rows = 1;
   const ROWCOL num_cols = 17;

	SetRowCount(num_rows);
	SetColCount(num_cols);


   SetFrozenRows(1,1); // use one extra row as column headers

   // Turn off selecting whole columns when clicking on a column header
	GetParam()->EnableSelection((WORD) (GX_SELFULL & ~GX_SELCOL & ~GX_SELTABLE));

   // disable left side
	SetStyleRange(CGXRange(0,0,num_rows,0), CGXStyle()
			.SetControl(GX_IDS_CTRL_HEADER)
			.SetEnabled(FALSE)          // disables usage as current cell
		);

   UpdateColumnHeaders();

   // make it so that text fits correctly in header row
	ResizeRowHeightsToFit(CGXRange(0,0,num_rows,num_cols));

   // don't allow users to resize grids
   GetParam( )->EnableTrackColWidth(0); 
   GetParam( )->EnableTrackRowHeight(0); 

	SetFocus();

	GetParam( )->EnableUndo(TRUE);

   ATLASSERT(num_rows == GetRowCount());

//   EnableIntelliMouse();
}

void CGirCompDimensionGrid::AddProblem()
{
   GIRCOMPDIMENSIONS dimensions;
   GetProblemData(GetRowCount(),dimensions);
   InsertRow(dimensions);

   UpdateData(TRUE);
   CGirCompDoc* pDoc = (CGirCompDoc*)EAFGetDocument();
   pDoc->UpdateAllViews(nullptr,0,nullptr);
}

void CGirCompDimensionGrid::GetProblemData(ROWCOL row,GIRCOMPDIMENSIONS& dimensions)
{
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   ROWCOL col = 1;
   dimensions.Type = (GIRCOMPDIMENSIONS::BeamType)_tstoi(GetCellValue(row,col++));

   IndexType idx = _tstoi(GetCellValue(row,col++));
   CGirCompDoc* pDoc = (CGirCompDoc*)EAFGetDocument();
   const std::vector<ROLLEDSECTION>& rolledSections = pDoc->GetRolledSections();
   dimensions.RolledSectionName = rolledSections[idx].Name;

   Float64 value;
   sysTokenizer::ParseDouble(GetCellValue(row,col++), &value);
   dimensions.Area = ::ConvertToSysUnits(value,pDispUnits->Area.UnitOfMeasure);

   if ( dimensions.Type == GIRCOMPDIMENSIONS::General || dimensions.Type == GIRCOMPDIMENSIONS::Rolled )
   {
      sysTokenizer::ParseDouble(GetCellValue(row,col++), &value);
      dimensions.D = ::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);
   }
   else
   {
      col++;
   }

   sysTokenizer::ParseDouble(GetCellValue(row,col++), &value);
   dimensions.MomentOfInertia = ::ConvertToSysUnits(value,pDispUnits->MomentOfInertia.UnitOfMeasure);

   if ( dimensions.Type == GIRCOMPDIMENSIONS::BuiltUp )
   {
      sysTokenizer::ParseDouble(GetCellValue(row,col++), &value);
      dimensions.D = ::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);
   }
   else
   {
      col++;
   }

   sysTokenizer::ParseDouble(GetCellValue(row,col++), &value);
   dimensions.tWeb = ::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);

   sysTokenizer::ParseDouble(GetCellValue(row,col++), &value);
   dimensions.wTopFlange = ::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);

   sysTokenizer::ParseDouble(GetCellValue(row,col++), &value);
   dimensions.tTopFlange = ::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);

   sysTokenizer::ParseDouble(GetCellValue(row,col++), &value);
   dimensions.wBotFlange = ::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);

   sysTokenizer::ParseDouble(GetCellValue(row,col++), &value);
   dimensions.tBotFlange = ::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);

   sysTokenizer::ParseDouble(GetCellValue(row,col++), &value);
   dimensions.wSlab = ::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);

   sysTokenizer::ParseDouble(GetCellValue(row,col++), &value);
   dimensions.tSlab = ::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);

   sysTokenizer::ParseDouble(GetCellValue(row,col++), &value);
   dimensions.G = ::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);

   sysTokenizer::ParseDouble(GetCellValue(row,col++), &value);
   dimensions.N1 = value;

   sysTokenizer::ParseDouble(GetCellValue(row,col++), &value);
   dimensions.N2 = value;

   sysTokenizer::ParseDouble(GetCellValue(row,col++), &value);
   dimensions.N3 = value;
}

void CGirCompDimensionGrid::SetProblemData(ROWCOL row,const GIRCOMPDIMENSIONS& dimensions)
{
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   ROWCOL col = 0;
   SetStyleRange(CGXRange(row,col++), CGXStyle()
      .SetValue(row-1)
      );

   SetStyleRange(CGXRange(row,col++), CGXStyle()
      .SetControl(GX_IDS_CTRL_ZEROBASED_EX)
   	.SetChoiceList(_T("Rolled\nGeneral\nBuilt-up"))
		.SetValue((long)dimensions.Type) // Built-up
      );

   long idx = -1;
   CString strRolledSections = GetRolledSections(dimensions,idx);
   if ( idx < 0 )
      idx = 0;

   SetStyleRange(CGXRange(row,col++), CGXStyle()
      .SetControl(GX_IDS_CTRL_ZEROBASED_EX)
      .SetChoiceList(strRolledSections)
      .SetValue(idx)
      );

   if ( dimensions.Type == GIRCOMPDIMENSIONS::General || dimensions.Type == GIRCOMPDIMENSIONS::Rolled )
   {
	   SetStyleRange(CGXRange(row,col++), CGXStyle()
         .SetHorizontalAlignment(DT_RIGHT)
         .SetValue(::ConvertFromSysUnits(dimensions.Area,pDispUnits->Area.UnitOfMeasure))
            );

	   SetStyleRange(CGXRange(row,col++), CGXStyle()
         .SetHorizontalAlignment(DT_RIGHT)
         .SetValue(::ConvertFromSysUnits(dimensions.D,pDispUnits->ComponentDim.UnitOfMeasure))
            );

	   SetStyleRange(CGXRange(row,col++), CGXStyle()
         .SetHorizontalAlignment(DT_RIGHT)
         .SetValue(::ConvertFromSysUnits(dimensions.MomentOfInertia,pDispUnits->MomentOfInertia.UnitOfMeasure))
            );

      col++; // D
      col++; // tWeb

	   SetStyleRange(CGXRange(row,col++), CGXStyle()
         .SetHorizontalAlignment(DT_RIGHT)
         .SetValue(::ConvertFromSysUnits(dimensions.wTopFlange,pDispUnits->ComponentDim.UnitOfMeasure))
            );

      col++; // ttf
      col++; // wbf
      col++; // tbf
   }
   else if ( dimensions.Type == GIRCOMPDIMENSIONS::BuiltUp )
   {
      col++; // Ax
      col++; // D
      col++; // Ix

	   SetStyleRange(CGXRange(row,col++), CGXStyle()
         .SetHorizontalAlignment(DT_RIGHT)
         .SetValue(::ConvertFromSysUnits(dimensions.D,pDispUnits->ComponentDim.UnitOfMeasure))
            );

	   SetStyleRange(CGXRange(row,col++), CGXStyle()
         .SetHorizontalAlignment(DT_RIGHT)
         .SetValue(::ConvertFromSysUnits(dimensions.tWeb,pDispUnits->ComponentDim.UnitOfMeasure))
            );

	   SetStyleRange(CGXRange(row,col++), CGXStyle()
         .SetHorizontalAlignment(DT_RIGHT)
         .SetValue(::ConvertFromSysUnits(dimensions.wTopFlange,pDispUnits->ComponentDim.UnitOfMeasure))
            );

	   SetStyleRange(CGXRange(row,col++), CGXStyle()
         .SetHorizontalAlignment(DT_RIGHT)
         .SetValue(::ConvertFromSysUnits(dimensions.tTopFlange,pDispUnits->ComponentDim.UnitOfMeasure))
            );

	   SetStyleRange(CGXRange(row,col++), CGXStyle()
         .SetHorizontalAlignment(DT_RIGHT)
         .SetValue(::ConvertFromSysUnits(dimensions.wBotFlange,pDispUnits->ComponentDim.UnitOfMeasure))
            );

	   SetStyleRange(CGXRange(row,col++), CGXStyle()
         .SetHorizontalAlignment(DT_RIGHT)
         .SetValue(::ConvertFromSysUnits(dimensions.tBotFlange,pDispUnits->ComponentDim.UnitOfMeasure))
            );
   }

   // Slab
   SetStyleRange(CGXRange(row,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::ConvertFromSysUnits(dimensions.wSlab,pDispUnits->ComponentDim.UnitOfMeasure))
         );

   SetStyleRange(CGXRange(row,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::ConvertFromSysUnits(dimensions.tSlab,pDispUnits->ComponentDim.UnitOfMeasure))
         );

   SetStyleRange(CGXRange(row,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::ConvertFromSysUnits(dimensions.G,pDispUnits->ComponentDim.UnitOfMeasure))
         );

   SetStyleRange(CGXRange(row,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(dimensions.N1)
         );

   SetStyleRange(CGXRange(row,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(dimensions.N2)
         );

   SetStyleRange(CGXRange(row,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(dimensions.N3)
         );
}

CString CGirCompDimensionGrid::GetRolledSections(const GIRCOMPDIMENSIONS& dimensions,long& idx)
{
   CGirCompDoc* pDoc = (CGirCompDoc*)EAFGetDocument();
   const std::vector<ROLLEDSECTION>& rolledSections = pDoc->GetRolledSections();
   CString str;
   long index = 0;
   std::vector<ROLLEDSECTION>::const_iterator iter(rolledSections.begin());
   std::vector<ROLLEDSECTION>::const_iterator end(rolledSections.end());
   for ( ; iter != end; iter++, index++ )
   {
      const ROLLEDSECTION& rolledSection = *iter;
      if ( rolledSection.Name == dimensions.RolledSectionName )
         idx = index;

      if ( index == 0 )
         str = rolledSection.Name.c_str();
      else
         str += _T("\n") + CString(rolledSection.Name.c_str());
   }

   return str;
}

void CGirCompDimensionGrid::InsertRow(const GIRCOMPDIMENSIONS& dimensions)
{
   GetParam()->EnableUndo(FALSE);
	ROWCOL nRow = GetRowCount()+1;

	InsertRows(nRow, 1);

   SetProblemData(nRow,dimensions);

   OnUpdateCells(nRow);

   SetCurrentCell(nRow, GetLeftCol(), GX_SCROLLINVIEW|GX_DISPLAYEDITWND);
	Invalidate();

   GetParam()->EnableUndo(TRUE);
}

CString CGirCompDimensionGrid::GetCellValue(ROWCOL nRow, ROWCOL nCol)
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
BOOL CGirCompDimensionGrid::OnValidateCell(ROWCOL nRow, ROWCOL nCol)
{
	return CGXGridWnd::OnValidateCell(nRow, nCol);
}
   
void CGirCompDimensionGrid::OnModifyCell(ROWCOL nRow,ROWCOL nCol)
{
   CGXGridWnd::OnModifyCell(nRow,nCol);

   GIRCOMPDIMENSIONS dimensions;
   GetProblemData(nRow,dimensions);

   if ( (nCol == 1 || nCol == 2) && dimensions.Type == GIRCOMPDIMENSIONS::Rolled )
   {
      CGirCompDoc* pDoc = (CGirCompDoc*)EAFGetDocument();
      const std::vector<ROLLEDSECTION>& rolledSections = pDoc->GetRolledSections();

      IndexType idx = _tstoi(GetCellValue(nRow,2));
      const ROLLEDSECTION& rolledSection = rolledSections[idx];
      dimensions.RolledSectionName = rolledSection.Name;
      dimensions.Area              = rolledSection.Area;
      dimensions.D                 = rolledSection.D;
      dimensions.MomentOfInertia   = rolledSection.I;
      dimensions.wTopFlange        = rolledSection.wFlange;

      SetProblemData(nRow,dimensions);
   }

   OnUpdateCells(nRow);
}

void CGirCompDimensionGrid::OnUpdateCells(ROWCOL row)
{
   GetParam()->EnableUndo(FALSE);

   ROWCOL col = 1;
   GIRCOMPDIMENSIONS::BeamType type = (GIRCOMPDIMENSIONS::BeamType)_tstoi(GetCellValue(row,col++));

   COLORREF disabled = ::GetSysColor(COLOR_BTNFACE);
   COLORREF enabled  = ::GetSysColor(COLOR_WINDOW);

   if ( type == GIRCOMPDIMENSIONS::Rolled )
   {
      EnableCell(row,col++,true); // Shape
      EnableCell(row,col++,false,false); // Ax
      EnableCell(row,col++,false,false); // D
      EnableCell(row,col++,false,false); // Ix
      EnableCell(row,col++,false); // D
      EnableCell(row,col++,false); // t web
      EnableCell(row,col++,false,false); // w top flange
      EnableCell(row,col++,false); // t top flange
      EnableCell(row,col++,false); // w bottom flange
      EnableCell(row,col++,false); // t bottom flange
   }
   else if ( type == GIRCOMPDIMENSIONS::General )
   {
      EnableCell(row,col++,false); // Shape
      EnableCell(row,col++,true); // Ax
      EnableCell(row,col++,true); // D
      EnableCell(row,col++,true); // Ix
      EnableCell(row,col++,false); // D
      EnableCell(row,col++,false); // t web
      EnableCell(row,col++,true); // w top flange
      EnableCell(row,col++,false); // t top flange
      EnableCell(row,col++,false); // w bottom flange
      EnableCell(row,col++,false); // t bottom flange
   }
   else if ( type == GIRCOMPDIMENSIONS::BuiltUp )
   {
      EnableCell(row,col++,false); // Shape
      EnableCell(row,col++,false); // Ax
      EnableCell(row,col++,false); // D
      EnableCell(row,col++,false); // Ix
      EnableCell(row,col++,true); // D
      EnableCell(row,col++,true); // t web
      EnableCell(row,col++,true); // w top flange
      EnableCell(row,col++,true); // t top flange
      EnableCell(row,col++,true); // w bottom flange
      EnableCell(row,col++,true); // t bottom flange
   }
   else
   {
      ATLASSERT(false); // is there a new type?
   }

   // These never change
   //EnableCell(row,col++,true); // w slab
   //EnableCell(row,col++,true); // t slab
   //EnableCell(row,col++,true); // G
   //EnableCell(row,col++,true); // N1
   //EnableCell(row,col++,true); // N2
   //EnableCell(row,col++,true); // N3

   ResizeColWidthsToFit(CGXRange(0,0,GetRowCount(),GetColCount()));

   GetParam()->EnableUndo(TRUE);
}

void CGirCompDimensionGrid::EnableCell(ROWCOL nRow, ROWCOL nCol, BOOL bEnable,BOOL bHideValue)
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
         .SetFormat(bHideValue ? GX_FMT_HIDDEN : GX_FMT_GEN)
         .SetInterior(::GetSysColor(COLOR_BTNFACE))
         );
   }
}

void CGirCompDimensionGrid::OnUnitsModeChanged()
{
   ROWCOL nRows = GetRowCount();
   if ( 1 < nRows )
   {
      CGXRange range(2, 0, GetRowCount(), 0);
      RemoveRows(range.top, range.bottom);
   }

   UpdateColumnHeaders();
   UpdateData(FALSE);
}

void CGirCompDimensionGrid::UpdateColumnHeaders()
{
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   // set text along top row
   ROWCOL col = 0;
   SetCoveredCellsRowCol(0,col,1,col);
   SetStyleRange(CGXRange(0,col++), CGXStyle()
         .SetWrapText(TRUE)
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
		   .SetEnabled(FALSE)          // disables usage as current cell
		   .SetValue(_T("Prob"))
	   );

   SetCoveredCellsRowCol(0,col,1,col);
   SetStyleRange(CGXRange(0,col++), CGXStyle()
         .SetWrapText(TRUE)
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
		   .SetEnabled(FALSE)          // disables usage as current cell
		   .SetValue(_T("Type"))
	   );

   SetCoveredCellsRowCol(0,col,1,col);
   SetStyleRange(CGXRange(0,col++), CGXStyle()
         .SetWrapText(TRUE)
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
		   .SetEnabled(FALSE)          // disables usage as current cell
		   .SetValue(_T("Shape"))
	   );

   SetCoveredCellsRowCol(0,col,0,col+2);
   SetStyleRange(CGXRange(0,col), CGXStyle()
      .SetValue(_T("General/Rolled"))
      );

   CString strDimension;
   strDimension.Format(_T("%s (%s)"),_T("Ax"),pDispUnits->Area.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(1,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(strDimension)
		);

   strDimension.Format(_T("%s (%s)"),_T("D"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(1,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(strDimension)
         );

   strDimension.Format(_T("%s (%s)"),_T("Ix"),pDispUnits->MomentOfInertia.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(1,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(strDimension)
         );

   SetCoveredCellsRowCol(0,col,0,col+1);
   SetStyleRange(CGXRange(0,col), CGXStyle()
      .SetValue(_T("Web"))
      );

   strDimension.Format(_T("%s (%s)"),_T("D"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(1,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(strDimension)
		);

   strDimension.Format(_T("%s (%s)"),_T("t web"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(1,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(strDimension)
		);

   SetCoveredCellsRowCol(0,col,0,col+1);
   SetStyleRange(CGXRange(0,col), CGXStyle()
      .SetValue(_T("Top Flange"))
      );

   strDimension.Format(_T("%s (%s)"),_T("width"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(1,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(strDimension)
		);

   strDimension.Format(_T("%s (%s)"),_T("thick"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(1,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(strDimension)
		);


   SetCoveredCellsRowCol(0,col,0,col+1);
   SetStyleRange(CGXRange(0,col), CGXStyle()
      .SetValue(_T("Bottom Flange"))
      );

   strDimension.Format(_T("%s (%s)"),_T("width"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(1,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(strDimension)
		);

   strDimension.Format(_T("%s (%s)"),_T("thick"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(1,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(strDimension)
		);

   SetCoveredCellsRowCol(0,col,0,col+2);
   SetStyleRange(CGXRange(0,col), CGXStyle()
      .SetValue(_T("Slab"))
      );

   strDimension.Format(_T("%s (%s)"),_T("width"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(1,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(strDimension)
		);

   strDimension.Format(_T("%s (%s)"),_T("thick"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(1,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(strDimension)
		);

   strDimension.Format(_T("%s\n(%s)"),_T("G"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
   SetStyleRange(CGXRange(1,col++), CGXStyle()
         .SetWrapText(TRUE)
		   .SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
		   .SetValue(strDimension)
	   );


   SetCoveredCellsRowCol(0,col,0,col+2);
   SetStyleRange(CGXRange(0,col), CGXStyle()
      .SetValue(_T("Modular Ratio"))
      );
   SetStyleRange(CGXRange(1,col++), CGXStyle()
         .SetWrapText(TRUE)
		   .SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
		   .SetValue(_T("N1"))
	   );

   SetStyleRange(CGXRange(1,col++), CGXStyle()
         .SetWrapText(TRUE)
		   .SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
		   .SetValue(_T("N2"))
	   );

   SetStyleRange(CGXRange(1,col++), CGXStyle()
         .SetWrapText(TRUE)
		   .SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
		   .SetValue(_T("N3"))
	   );
}

#if defined _DEBUG
void CGirCompDimensionGrid::AssertValid() const
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CGXGridWnd::AssertValid();
}
#endif
