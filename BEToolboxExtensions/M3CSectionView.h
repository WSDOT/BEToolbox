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
#pragma once

// CM3CSectionView view

#include <DManip\DManip.h>

class CM3CSectionView : public CDisplayView
{
	DECLARE_DYNCREATE(CM3CSectionView)

protected:
   CM3CSectionView();           // protected constructor used by dynamic creation
	virtual ~CM3CSectionView();

   void BuildDisplayLists();
   void UpdateDrawingScale();
   void UpdateDisplayObjects();

public:
   virtual void OnInitialUpdate() override;
   virtual void OnUpdate(CView* pView, LPARAM lHint, CObject* pHint) override;
   void DoPrint(CDC* pDC, CPrintInfo* pInfo);

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
   DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnSize(UINT nType, int cx, int cy);
};


