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

// PGStableGirderControl.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "PGStableGirderControl.h"

#include <GeometricPrimitives\Primitives.h>

#include <Colors.h>
#define SEGMENT_BORDER_COLOR GREY50
#define SEGMENT_FILL_COLOR   GREY70

// CPGStableGirderControl

IMPLEMENT_DYNAMIC(CPGStableGirderControl, CWnd)

CPGStableGirderControl::CPGStableGirderControl()
{
}

CPGStableGirderControl::~CPGStableGirderControl()
{
}


BEGIN_MESSAGE_MAP(CPGStableGirderControl, CWnd)
   ON_WM_PAINT()
   ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CPGStableGirderControl message handlers
void CPGStableGirderControl::CustomInit()
{
}

BOOL CPGStableGirderControl::OnEraseBkgnd(CDC* pDC)
{
   CBrush brush(::GetSysColor(COLOR_WINDOW));
   brush.UnrealizeObject();

   CPen pen(PS_SOLID,1,::GetSysColor(COLOR_WINDOW));
   pen.UnrealizeObject();

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

void CPGStableGirderControl::OnPaint()
{
   CPaintDC dc(this); // device context for painting
   // TODO: Add your message handler code here
   // Do not call CWnd::OnPaint() for painting messages

   AFX_MANAGE_STATE(AfxGetAppModuleState());
   CWnd* parent = GetParent();
   CPGStableGirderControlParent* pParent = dynamic_cast<CPGStableGirderControlParent*>(parent);


   //
   // Set up coordinate mapping
   //
   std::vector<std::pair<Float64,Float64>> vProfile = pParent->GetGirderProfile();
   gpRect2d box(DBL_MAX,DBL_MAX,-DBL_MAX,-DBL_MAX);
   std::vector<std::pair<Float64,Float64>>::iterator iter(vProfile.begin());
   std::vector<std::pair<Float64,Float64>>::iterator end(vProfile.end());
   for ( ; iter != end; iter++ )
   {
      box.Left()  = Min(box.Left(), iter->first);
      box.Right() = Max(box.Right(),iter->first);

      box.Top()    = Max(box.Top(),   iter->second);
      box.Bottom() = Min(box.Bottom(),iter->second);
   }
   box.Top() = Max(box.Top(),0.0);


   CRect rClient;
   GetClientRect(&rClient);

   grlibPointMapper mapper;
   mapper.SetMappingMode(grlibPointMapper::Anisotropic);
   mapper.SetWorldExt(box.Size());
   mapper.SetWorldOrg(box.TopLeft());
   mapper.SetDeviceExt(rClient.Size().cx,rClient.Size().cy);
   mapper.SetDeviceOrg(rClient.left,rClient.top);

   CPen pen(PS_SOLID,1,SEGMENT_BORDER_COLOR);
   CBrush brush(SEGMENT_FILL_COLOR);

   CPen* pOldPen = dc.SelectObject(&pen);
   CBrush* pOldBrush = dc.SelectObject(&brush);

   DrawProfile(&dc,mapper,vProfile);
   DrawStrands(&dc,mapper);

   // Clean up
   dc.SelectObject(pOldPen);
   dc.SelectObject(pOldBrush);
}

void CPGStableGirderControl::DrawProfile(CDC* pDC,grlibPointMapper& mapper,std::vector<std::pair<Float64,Float64>>& vProfile)
{
   Draw(pDC,mapper,vProfile,TRUE);
}

void CPGStableGirderControl::DrawStrands(CDC* pDC,grlibPointMapper& mapper)
{
   AFX_MANAGE_STATE(AfxGetAppModuleState());
   CWnd* parent = GetParent();
   CPGStableGirderControlParent* pParent = dynamic_cast<CPGStableGirderControlParent*>(parent);

   std::vector<std::pair<Float64,Float64>> vStraight,vHarped,vTemp;
   pParent->GetStrandProfiles(&vStraight,&vHarped,&vTemp);
   Draw(pDC,mapper,vStraight,FALSE);
   Draw(pDC,mapper,vHarped,FALSE);
   Draw(pDC,mapper,vTemp,FALSE);
}

void CPGStableGirderControl::Draw(CDC* pDC,grlibPointMapper& mapper,std::vector<std::pair<Float64,Float64>>& vProfile,BOOL bPolygon)
{
   IndexType nPoints = vProfile.size();
   if ( nPoints == 0 )
   {
      return;
   }

   if ( bPolygon )
   {
      nPoints += 2;
   }

   CPoint* points = new CPoint[nPoints];
   LONG dx,dy;
   mapper.WPtoDP(vProfile.front().first,0,&dx,&dy);
   IndexType idx = 0;
   if ( bPolygon )
   {
      points[idx++] = CPoint(dx,dy);
   }
   std::vector<std::pair<Float64,Float64>>::iterator iter(vProfile.begin());
   std::vector<std::pair<Float64,Float64>>::iterator end(vProfile.end());
   for ( ; iter != end; iter++ )
   {
      mapper.WPtoDP(iter->first,iter->second,&dx,&dy);
      points[idx++] = CPoint(dx,dy);
   }
   
   if ( bPolygon )
   {
      mapper.WPtoDP(vProfile.back().first,0,&dx,&dy);
      points[idx++] = CPoint(dx,dy);
      pDC->Polygon(points,(int)nPoints);
   }
   else
   {
      pDC->Polyline(points,(int)nPoints);
   }


   delete[] points;
}
