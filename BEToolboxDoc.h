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

// CBEToolboxDoc document
#include "BEToolboxLib.h"
#include <EAF\EAFDocument.h>

class BETCLASS CBEToolboxDoc : public CEAFDocument
{
	DECLARE_DYNAMIC(CBEToolboxDoc)

public:
	CBEToolboxDoc();
	virtual ~CBEToolboxDoc();

   virtual void DoIntegrateWithUI(BOOL bIntegrate) override;
   virtual BOOL GetStatusBarMessageString(UINT nID,CString& rMessage) const override;
   virtual BOOL GetToolTipMessageString(UINT nID, CString& rMessage) const override;

#ifdef _DEBUG
	virtual void AssertValid() const override;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const override;
#endif
#endif

protected:
   /// called when a document is created (New or Open)
   virtual BOOL Init(); 

   virtual void LoadToolbarState() override;
   virtual void SaveToolbarState() override;

   afx_msg void OnAbout();

	DECLARE_MESSAGE_MAP()
public:
   virtual BOOL OnOpenDocument(LPCTSTR lpszPathName) override;
   virtual void OnCloseDocument() override;
   virtual CString GetRootNodeName() override;
   virtual Float64 GetRootNodeVersion() override;

protected:
   CEAFToolBar* m_pMyToolBar;

   virtual void OnOldFormat(LPCTSTR lpszPathName);

   virtual UINT GetToolbarID();
   virtual void LoadToolbarResource();
   virtual HINSTANCE GetResourceInstance() override;

   HICON m_hMainFrameBigIcon;
   HICON m_hMainFrameSmallIcon;
};
