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
#pragma once

// CM3CMainView view
#include <afxsplitterwndex.h>

class CM3CMainView : public CView
{
	DECLARE_DYNCREATE(CM3CMainView)

protected:
	CM3CMainView();           // protected constructor used by dynamic creation
	virtual ~CM3CMainView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

   CSplitterWndEx m_SplitterWnd;
   DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnSize(UINT nType, int cx, int cy);
   virtual void OnInitialUpdate();


   CView* GetTopLeftView();
   CView* GetTopRightView();
   CView* GetBottomLeftView();
   CView* GetBottomRightView();
};


