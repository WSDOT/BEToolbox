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

#pragma once
#include <ReportManager\ChapterBuilder.h>
#include "SpectraDoc.h"

class CSpectraChapterBuilder :
   public CChapterBuilder
{
public:
   CSpectraChapterBuilder(CSpectraDoc* pDoc);
   ~CSpectraChapterBuilder(void);

   virtual LPCTSTR GetName() const;
   virtual Uint16 GetMaxLevel() const;
   virtual rptChapter* Build(CReportSpecification* pRptSpec,Uint16 level) const;
   virtual bool Select() const { return true; }
   virtual CChapterBuilder* Clone() const;

private:
   CSpectraDoc* m_pDoc;

   // This is a list of temporary files that were created on the fly
   // Delete them in the destructor
   std::vector<std::_tstring> m_TemporaryFiles;
   rptRcImage* CreateImage(const std::vector<std::pair<Float64,Float64>>& values) const;
};
