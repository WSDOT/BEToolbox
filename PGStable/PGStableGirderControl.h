///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2019  Washington State Department of Transportation
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

#pragma once
#include <GraphicsLib\Pointmapper.h>

class CPGStableGirderControlParent
{
public:
   virtual std::vector<std::pair<Float64,Float64>> GetGirderProfile() = 0;
   virtual void GetStrandProfiles(std::vector<std::pair<Float64,Float64>>* pvStraight,std::vector<std::pair<Float64,Float64>>* pvHarped,std::vector<std::pair<Float64,Float64>>* pvTemp) = 0;
};

// CPGStableGirderControl

class CPGStableGirderControl : public CWnd
{
	DECLARE_DYNAMIC(CPGStableGirderControl)

public:
	CPGStableGirderControl();
	virtual ~CPGStableGirderControl();

   void CustomInit();

   afx_msg BOOL OnEraseBkgnd(CDC* pDC);
   afx_msg void OnPaint();

protected:
	DECLARE_MESSAGE_MAP()
   void DrawProfile(CDC* pDC,grlibPointMapper& mapper,std::vector<std::pair<Float64,Float64>>& vProfile);
   void DrawStrands(CDC* pDC,grlibPointMapper& mapper);
   void Draw(CDC* pDC,grlibPointMapper& mapper,std::vector<std::pair<Float64,Float64>>& vProfile,BOOL bPolygon);
};


