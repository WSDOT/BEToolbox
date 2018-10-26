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

// PGStableGraphControl.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "PGStableGraphControl.h"


// CPGStableGraphControl

// create a dummy unit conversion tool to pacify the graph constructor
static unitmgtLengthData DUMMY(unitMeasure::Meter);
static LengthTool        DUMMY_TOOL(DUMMY);


IMPLEMENT_DYNAMIC(CPGStableGraphControl, CWnd)

CPGStableGraphControl::CPGStableGraphControl() :
m_Graph(DUMMY_TOOL,DUMMY_TOOL),
m_pXFormat(0),
m_pYFormat(0)
{
   m_Graph.SetXAxisNumberOfMinorTics(0);
   m_Graph.SetXAxisNiceRange(false); // want X axis to be exactly the same length as the girder
   m_Graph.SetXAxisNumberOfMajorTics(11); // want tick marks at 10th points
}

CPGStableGraphControl::~CPGStableGraphControl()
{
   if ( m_pXFormat != NULL )
   {
      delete m_pXFormat;
      m_pXFormat = NULL;
   }

   if ( m_pYFormat != NULL )
   {
      delete m_pYFormat;
      m_pYFormat = NULL;
   }
}


BEGIN_MESSAGE_MAP(CPGStableGraphControl, CWnd)
   ON_WM_PAINT()
   ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CPGStableGraphControl message handlers
void CPGStableGraphControl::CustomInit()
{
}

grGraphXY& CPGStableGraphControl::GetGraph()
{
   return m_Graph;
}

void CPGStableGraphControl::SetAxisFormatters(arvPhysicalConverter* pXFormat,arvPhysicalConverter* pYFormat)
{
   if ( m_pXFormat != NULL )
   {
      delete m_pXFormat;
   }
   m_pXFormat = pXFormat;
   m_Graph.SetXAxisValueFormat(*m_pXFormat);

   if ( m_pYFormat != NULL )
   {
      delete m_pYFormat;
   }
   m_pYFormat = pYFormat;
   m_Graph.SetYAxisValueFormat(*m_pYFormat);
}

BOOL CPGStableGraphControl::OnEraseBkgnd(CDC* pDC)
{
   CBrush brush(::GetSysColor(COLOR_BTNFACE));
   CPen pen(PS_SOLID,1,::GetSysColor(COLOR_BTNFACE));

   CBrush* pOldBrush = pDC->SelectObject(&brush);
   CPen* pOldPen = pDC->SelectObject(&pen);

   CRect rect;
   GetClientRect(&rect);
   pDC->Rectangle(rect);

   pDC->SelectObject(pOldBrush);
   pDC->SelectObject(pOldPen);

   return TRUE;

   // default isn't working so we have to do our own erasing
   //return CWnd::OnEraseBkgnd(pDC);
}

void CPGStableGraphControl::OnPaint()
{
   CPaintDC dc(this); // device context for painting

   OnEraseBkgnd(&dc);

   CRect rect;
   GetClientRect(&rect);
   m_Graph.SetOutputRect(rect);

   m_Graph.Draw(dc.GetSafeHdc());
}
