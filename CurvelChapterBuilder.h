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

#pragma once
#include <ReportManager\ChapterBuilder.h>
#include "CurvelDoc.h"
#include <WBFLCogo.h>

class CCurvelReportSpecification;

class CCurvelChapterBuilder :
   public CChapterBuilder
{
public:
   CCurvelChapterBuilder(CCurvelDoc* pDoc);
   ~CCurvelChapterBuilder(void);

   virtual LPCTSTR GetName() const override;
   virtual Uint16 GetMaxLevel() const override;
   virtual rptChapter* Build(CReportSpecification* pRptSpec,Uint16 level) const override;
   virtual bool Select() const override { return true; }
   virtual CChapterBuilder* Clone() const override;

private:
   CCurvelDoc* m_pDoc;

   CComPtr<IAngle> m_SkewAngle;
   CComPtr<IAngleDisplayUnitFormatter> m_AngleFormatter;
   CComBSTR m_bstrAngleFormat;

   void Init(CCurvelReportSpecification* pRptSpec) const;
   void UpdateAlignment(const SkewLine& skewLine) const;
   mutable CComPtr<IAlignment> m_Alignment;
   mutable CComPtr<IProfile> m_Profile;
};
