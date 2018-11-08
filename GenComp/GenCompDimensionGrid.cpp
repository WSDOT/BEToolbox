///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2018  Washington State Department of Transportation
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

// GenCompDiaphramGrid.cpp : implementation file
//

#include "stdafx.h"
#include "GenCompDimensionGrid.h"
#include <EAF\EAFUtilities.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//GRID_IMPLEMENT_REGISTER(CGenCompDimensionGrid, CS_DBLCLKS, 0, 0, 0);

/////////////////////////////////////////////////////////////////////////////
// CGenCompDimensionGrid

CGenCompDimensionGrid::CGenCompDimensionGrid()
{
//   RegisterClass();
}

CGenCompDimensionGrid::~CGenCompDimensionGrid()
{
}

BEGIN_MESSAGE_MAP(CGenCompDimensionGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CGenCompDimensionGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CGenCompDimensionGrid::DoDataExchange(CDataExchange* pDX)
{
   CGenCompDoc* pDoc = (CGenCompDoc*)EAFGetDocument();
   if ( pDX->m_bSaveAndValidate )
   {
      std::vector<std::pair<Float64,Float64>> points;
      ROWCOL nRows = GetRowCount();
      for ( ROWCOL row = 0; row < nRows; row++ )
      {
         Float64 x,y;
         GetPoint(row+1,x,y);
         points.emplace_back(x,y);
      }

      if ( m_bPrimaryPoints )
      {
         pDoc->ClearPrimaryPoints();
         pDoc->AddPrimaryPoints(points);
      }
      else
      {
         pDoc->ClearSecondaryPoints();
         pDoc->AddSecondaryPoints(points);
      }
   }
   else
   {
      if ( 0 < GetRowCount() )
         RemoveRows(1,GetRowCount());

      const std::vector<std::pair<Float64,Float64>>& points(m_bPrimaryPoints ? pDoc->GetPrimaryPoints() : pDoc->GetSecondaryPoints());
      std::vector<std::pair<Float64,Float64>>::const_iterator iter(points.begin());
      std::vector<std::pair<Float64,Float64>>::const_iterator end(points.end());
      for ( ; iter != end; iter++ )
      {
         InsertRow(iter->first,iter->second);
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
// CGenCompDimensionGrid message handlers

void CGenCompDimensionGrid::CustomInit(bool bPrimaryPoints)
{
   m_bPrimaryPoints = bPrimaryPoints;

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

   // make it so that text fits correctly in header row
	ResizeRowHeightsToFit(CGXRange(0,0,0,num_cols));

   // don't allow users to resize grids
   GetParam( )->EnableTrackColWidth(0); 
   GetParam( )->EnableTrackRowHeight(0); 

	SetFocus();

	GetParam( )->EnableUndo(TRUE);

//   EnableIntelliMouse();
}

void CGenCompDimensionGrid::AddPoint()
{
   InsertRow(0.0,0.0);

   UpdateData(TRUE);
   CGenCompDoc* pDoc = (CGenCompDoc*)EAFGetDocument();
   pDoc->UpdateAllViews(nullptr,0,nullptr);
}

BOOL CGenCompDimensionGrid::ArePointsSelected()
{
	if (GetParam() == nullptr)
	{
      return FALSE;
	}

	CGXRangeList* pSelList = GetParam()->GetRangeList();
   return (pSelList->IsAnyCellFromCol(0) && pSelList->GetCount() == 1) ? TRUE : FALSE;
}

void CGenCompDimensionGrid::RemoveSelectedPoints()
{
	CGXRangeList* pSelList = GetParam()->GetRangeList();
	if (pSelList->IsAnyCellFromCol(0) && pSelList->GetCount() == 1)
	{
		CGXRange range = pSelList->GetHead();
		range.ExpandRange(1, 0, GetRowCount(), 0);
		RemoveRows(range.top, range.bottom);
	}

   UpdateData(TRUE);
   CGenCompDoc* pDoc = (CGenCompDoc*)EAFGetDocument();
   pDoc->UpdateAllViews(nullptr,0,nullptr);
}

void CGenCompDimensionGrid::GetPoint(ROWCOL row,Float64& x,Float64 &y)
{
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   CString strValue;
   Float64 value;
   strValue = GetCellValue(row,1);
   sysTokenizer::ParseDouble(strValue, &value);
   x = ::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);

   strValue = GetCellValue(row,2);
   sysTokenizer::ParseDouble(strValue, &value);
   y = ::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);
}

void CGenCompDimensionGrid::InsertRow(Float64 x,Float64 y)
{
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   GetParam()->EnableUndo(FALSE);
	ROWCOL nRow = GetRowCount()+1;

	InsertRows(nRow, 1);

   ROWCOL col = 1;
	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetValue(::ConvertFromSysUnits(x,pDispUnits->ComponentDim.UnitOfMeasure))
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetValue(::ConvertFromSysUnits(y,pDispUnits->ComponentDim.UnitOfMeasure))
         );

   SetCurrentCell(nRow, GetLeftCol(), GX_SCROLLINVIEW|GX_DISPLAYEDITWND);
	Invalidate();

   GetParam()->EnableUndo(TRUE);
}

CString CGenCompDimensionGrid::GetCellValue(ROWCOL nRow, ROWCOL nCol)
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
BOOL CGenCompDimensionGrid::OnValidateCell(ROWCOL nRow, ROWCOL nCol)
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
   
void CGenCompDimensionGrid::OnModifyCell(ROWCOL nRow,ROWCOL nCol)
{
   CGXGridWnd::OnModifyCell(nRow,nCol);

	//CString s;
	//CGXControl* pControl = GetControl(nRow, nCol);
	//pControl->GetCurrentText(s);

 //  Float64 d;
 //  if ( sysTokenizer::ParseDouble(s,&d) )
 //  {
 //     CGenCompDoc* pDoc = (CGenCompDoc*)EAFGetDocument();
 //     pDoc->UpdateAllViews(nullptr,0,nullptr);
 //  }

 //  SetCurrentCell(nRow, nCol, GX_SCROLLINVIEW|GX_DISPLAYEDITWND);
}

void CGenCompDimensionGrid::OnUnitsModeChanged()
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

void CGenCompDimensionGrid::UpdateColumnHeaders()
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
			.SetValue(_T(""))
		);

   CString strDimension;
   strDimension.Format(_T("X (%s)"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(strDimension)
		);

   strDimension.Format(_T("Y (%s)"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(strDimension)
         );
}

#if defined _DEBUG
void CGenCompDimensionGrid::AssertValid() const
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CGXGridWnd::AssertValid();
}
#endif
