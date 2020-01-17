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

#include "StdAfx.h"
#include "M3CAnalysisDetailsReportSpecification.h"

CM3CAnalysisDetailsReportSpecification::CM3CAnalysisDetailsReportSpecification(LPCTSTR strReportName) :
CReportSpecification(strReportName)
{
   m_Index = 1;
}
   
CM3CAnalysisDetailsReportSpecification::~CM3CAnalysisDetailsReportSpecification(void)
{
}

void CM3CAnalysisDetailsReportSpecification::SetResultsIndex(IndexType idx)
{
   m_Index = idx;
}

IndexType CM3CAnalysisDetailsReportSpecification::GetResultsIndex() const
{
   return m_Index;
}
