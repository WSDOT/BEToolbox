///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2013  Washington State Department of Transportation
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

#include "StdAfx.h"
#include "resource.h"
#include "CurvelReportSpecificationBuilder.h"
#include "CurvelReportSpecification.h"
#include "CurvelReportWizard.h"
#include "CurvelDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCurvelReportSpecificationBuilder::CCurvelReportSpecificationBuilder() :
CReportSpecificationBuilder()
{
}

CCurvelReportSpecificationBuilder::~CCurvelReportSpecificationBuilder(void)
{
}

boost::shared_ptr<CReportSpecification> CCurvelReportSpecificationBuilder::CreateReportSpec(const CReportDescription& rptDesc,boost::shared_ptr<CReportSpecification>& pRptSpec)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   CCurvelDoc* pDoc = (CCurvelDoc*)EAFGetDocument();

   boost::shared_ptr<CCurvelReportSpecification> pInitRptSpec( boost::shared_dynamic_cast<CCurvelReportSpecification>(pRptSpec) );
   CCurvelReportWizard wiz(pInitRptSpec);

   if ( wiz.DoModal() == ID_WIZFINISH )
   {
      CCurvelReportSpecification* pSpec = new CCurvelReportSpecification(rptDesc.GetReportName());
      boost::shared_ptr<CReportSpecification> pRptSpec(pSpec);

      wiz.UpdateReportSpecification(pSpec);

      rptDesc.ConfigureReportSpecification(pRptSpec);

      pDoc->SetModifiedFlag(TRUE);

      return pRptSpec;
   }
   else
   {
      // Wizard was canceled... if there was an initialization spec, return it
      if ( pInitRptSpec )
         return pInitRptSpec;

      // otherwise, return the default spec (which, for the case of CURVEL, is with the document object)
      return pDoc->GetDefaultReportSpecification();
   }

   return boost::shared_ptr<CReportSpecification>();
}
