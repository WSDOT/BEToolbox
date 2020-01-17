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

#include <MFCTools\PrinterJob.h>
#include "M3CChildFrame.h"

class CM3CPrintJob : public CPrinterJob
{
public:
   CM3CPrintJob(CM3CChildFrame* pFrame);
   virtual ~CM3CPrintJob();

   // virtual overridden from base class; same meaning as CView's 
   void OnBeginPrinting(CDC * pDC, CPrintInfo * pInfo);
   void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
   bool OnPreparePrinting(CPrintInfo* pInfo, bool bPrintPreview = false);
   void OnPrint(CDC* pDC, CPrintInfo* pInfo);

private:
   CM3CPrintJob(); // no default construction

                          // data members
   CRect m_rcMarginMM;	// contain the margins in millimeters

   CString	m_csFtPrint;// font type name
   int      m_iFtPrint;	// font size

   CM3CChildFrame* m_pFrame;
};

