///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2022  Washington State Department of Transportation
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

#include "stdafx.h"
#include "SpectraChapterBuilder.h"
#include <Reporter\Reporter.h>
#include <Graphing/GraphXY.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////
CSpectraChapterBuilder::CSpectraChapterBuilder(CSpectraDoc* pDoc)
{
   m_pDoc = pDoc;
}

CSpectraChapterBuilder::~CSpectraChapterBuilder(void)
{
   for (const auto& file : m_TemporaryFiles)
   {
      ::DeleteFile( file.c_str() );
   }
}

LPCTSTR CSpectraChapterBuilder::GetName() const
{
   return _T("Spectra");
}

Uint16 CSpectraChapterBuilder::GetMaxLevel() const
{
   return 1;
}

rptChapter* CSpectraChapterBuilder::Build(CReportSpecification* pRptSpec,Uint16 level) const
{
   Float64 lat,lng;
   m_pDoc->GetLocation(&lat,&lng);
   SiteClass siteClass = m_pDoc->GetSiteClass();
   SpecificationType specType = m_pDoc->GetSpecification();

   rptRcScalar scalar;
   scalar.SetFormat(WBFL::System::NumericFormatTool::Format::Fixed);
   scalar.SetWidth(6);
   scalar.SetPrecision(3);

   rptRcScalar table_value;
   table_value.SetFormat(WBFL::System::NumericFormatTool::Format::Fixed);
   table_value.SetWidth(4);
   table_value.SetPrecision(1);

   std::_tstring strSiteClass[] = {_T("A"), _T("B"), _T("C"), _T("D"), _T("E"), _T("F")};

   rptChapter* pChapter = new rptChapter;
   rptParagraph* pPara;

   pPara = new rptParagraph;
   (*pChapter) << pPara;

   CResponseSpectra spectra;
   Uint32 result = m_pDoc->GetResponseSpectra(specType,lat,lng,siteClass,&spectra);
   if ( result != SPECTRA_OK )
   {
      (*pPara) << _T("Response spectra could not be computed. ") << m_pDoc->GetSpectraResultExplaination(result) << rptNewLine;
      return pChapter;
   }

   (*pPara) << m_pDoc->GetSpecificationDescription(specType) << rptNewLine;
   (*pPara) << _T("2014 Seismic Hazard Map, 7% probability of exceedance in 75 years") << rptNewLine;
   (*pPara) << rptNewLine;
   (*pPara) << _T("Site Coordinates (Latitude,Longitude): ") << lat << _T("° N, ") << -lng << _T("° W") << rptNewLine;
   (*pPara) << _T("Site Soil Classification: ") << _T("Site Class ") << strSiteClass[siteClass] << _T(" - ") << m_pDoc->GetSiteClassDescription(siteClass) << rptNewLine;
   (*pPara) << rptNewLine;

   if (specType == WSDOT_BDM)
   {
      (*pPara) << _T("Seismic hazard maps are for sites at the boundary of Site Classes B and C, which is ") << Sub2(Overline(_T("v")),_T("s")) << _T(" = 2500 ft/s (760 m/s). Adjustments for other Site Classes are made as needed.") << rptNewLine;
   }
   else
   {
      (*pPara) << _T("Seismic hazard maps are for Site Class B. Adjustments for other Site Classes are made as needed.") << rptNewLine;
   }

   rptRcTable* pTable = rptStyleManager::CreateDefaultTable(3);
   (*pPara) << pTable << rptNewLine;
   (*pTable)(0,0) << _T("Period") << rptNewLine << _T("(sec)");
   (*pTable)(0,1) << _T("Sa") << rptNewLine << _T("(g)");
   (*pTable)(0,2) << _T("");
   
   RowIndexType row = pTable->GetNumberOfHeaderRows();
   (*pTable)(row,0) << _T("0.0");
   (*pTable)(row,1) << scalar.SetValue(spectra.GetPGA());
   (*pTable)(row,2) << _T("PGA - Site Class B");
   if (specType == WSDOT_BDM)
   {
      (*pTable)(row, 2) << _T("/C Boundary");
   }
   row++;

   (*pTable)(row,0) << _T("0.2");
   (*pTable)(row,1) << scalar.SetValue(spectra.GetSs());
   (*pTable)(row,2) << Sub2(_T("S"),_T("s")) << _T(" - Site Class B");
   if (specType == WSDOT_BDM)
   {
      (*pTable)(row, 2) << _T("/C Boundary");
   }
   row++;

   (*pTable)(row,0) << _T("1.0");
   (*pTable)(row,1) << scalar.SetValue(spectra.GetS1());
   (*pTable)(row,2) << Sub2(_T("S"),_T("1")) << _T(" - Site Class B");
   if (specType == WSDOT_BDM)
   {
      (*pTable)(row, 2) << _T("/C Boundary");
   }
   row++;


   ColumnIndexType nColumns = (specType == WSDOT_BDM ? 6 : 5) + 1;

   (*pPara) << _T("Values of Site Coefficient, ") << Sub2(_T("F"), _T("pga")) << _T(", for Peak Ground Acceleration") << rptNewLine;
   rptRcTable* pZeroPeriodTable = rptStyleManager::CreateDefaultTable(nColumns);
   (*pPara) << pZeroPeriodTable << rptNewLine;
   (*pPara) << _T("For Site Class ") << strSiteClass[siteClass];
   if (specType != WSDOT_BDM)
   {
      (*pPara) << _T(" and PGA = ") << scalar.SetValue(spectra.GetPGA());
   }
   (*pPara) << _T(", ") << Sub2(_T("F"), _T("pga")) << _T(" = ") << scalar.SetValue(spectra.GetFpga()) << rptNewLine;

   (*pPara) << rptNewLine;

   for ( ColumnIndexType colIdx = 0; colIdx < nColumns; colIdx++ )
   {
      pZeroPeriodTable->SetColumnStyle(colIdx,rptStyleManager::GetTableCellStyle(CJ_CENTER));
      pZeroPeriodTable->SetStripeRowColumnStyle(colIdx,rptStyleManager::GetTableStripeRowCellStyle(CJ_CENTER));
   }

   pZeroPeriodTable->SetNumberOfHeaderRows(2);
   pZeroPeriodTable->SetRowSpan(0,0,2);

   ColumnIndexType col = 0;

   (*pZeroPeriodTable)(0,col++) << _T("Site Class");

   pZeroPeriodTable->SetColumnSpan(0,col,nColumns-1);
   (*pZeroPeriodTable)(0, col) << _T("Mapped Peak Ground Acceleration Coefficient (PGA)");
   (*pZeroPeriodTable)(1, col++) << _T("PGA") << symbol(LTE) << _T(" 0.10");
   (*pZeroPeriodTable)(1, col++) << _T("PGA") << _T("=")     << _T(" 0.20");
   (*pZeroPeriodTable)(1, col++) << _T("PGA") << _T("=")     << _T(" 0.30");
   (*pZeroPeriodTable)(1, col++) << _T("PGA") << _T("=")     << _T(" 0.40");
   if (specType == WSDOT_BDM)
   {
      (*pZeroPeriodTable)(1, col++) << _T("PGA") << _T("=") << _T(" 0.50");
      (*pZeroPeriodTable)(1, col++) << _T("PGA") << symbol(GTE) << _T(" 0.60");
   }
   else
   {
      (*pZeroPeriodTable)(1, col++) << _T("PGA") << symbol(GTE) << _T(" 0.50");
   }


   (*pPara) << _T("Values for Site Coefficient, ") << Sub2(_T("F"), _T("a")) << _T(", for 0.2 sec Period Spectral Acceleration") << rptNewLine;
   rptRcTable* pShortPeriodTable = rptStyleManager::CreateDefaultTable(nColumns);
   (*pPara) << pShortPeriodTable << rptNewLine;
   (*pPara) << _T("For Site Class ") << strSiteClass[siteClass];
   if (specType != WSDOT_BDM)
   {
      (*pPara) << _T(" and ") << Sub2(_T("S"), _T("s")) << _T(" = ") << scalar.SetValue(spectra.GetSs());
   }
   (*pPara) << _T(", ") << Sub2(_T("F"), _T("a")) << _T(" = ") << scalar.SetValue(spectra.GetFa()) << rptNewLine;
   (*pPara) << rptNewLine;

   for (ColumnIndexType colIdx = 0; colIdx < nColumns; colIdx++)
   {
      pShortPeriodTable->SetColumnStyle(colIdx, rptStyleManager::GetTableCellStyle(CJ_CENTER));
      pShortPeriodTable->SetStripeRowColumnStyle(colIdx, rptStyleManager::GetTableStripeRowCellStyle(CJ_CENTER));
   }
   pShortPeriodTable->SetNumberOfHeaderRows(2);
   pShortPeriodTable->SetRowSpan(0, 0, 2);
   col = 0;
   (*pShortPeriodTable)(0, col++) << _T("Site Class");

   pShortPeriodTable->SetColumnSpan(0, col, nColumns-1);
   (*pShortPeriodTable)(0, col) << _T("Mapped Spectral Acceleration Coefficient at Period 0.2 sec (") << Sub2(_T("S"), _T("s")) << _T(")");

   (*pShortPeriodTable)(1, col++) << Sub2(_T("S"), _T("s")) << symbol(LTE) << _T(" 0.25");
   (*pShortPeriodTable)(1, col++) << Sub2(_T("S"), _T("s")) << _T("=") << _T(" 0.50");
   (*pShortPeriodTable)(1, col++) << Sub2(_T("S"), _T("s")) << _T("=") << _T(" 0.75");
   (*pShortPeriodTable)(1, col++) << Sub2(_T("S"), _T("s")) << _T("=") << _T(" 1.00");
   if (specType == WSDOT_BDM)
   {
      (*pShortPeriodTable)(1, col++) << Sub2(_T("S"), _T("s")) << _T("=") << _T(" 1.25");
      (*pShortPeriodTable)(1, col++) << Sub2(_T("S"), _T("s")) << symbol(GTE) << _T(" 1.50");
   }
   else
   {
      (*pShortPeriodTable)(1, col++) << Sub2(_T("S"), _T("s")) << symbol(GTE) << _T(" 1.25");
   }

   (*pPara) << _T("Values of Site Coefficient, ") << Sub2(_T("F"), _T("v")) << _T(", for 1.0 sec Period Spectral Acceleration") << rptNewLine;
   rptRcTable* pLongPeriodTable = rptStyleManager::CreateDefaultTable(nColumns);
   (*pPara) << pLongPeriodTable << rptNewLine;
   (*pPara) << _T("For Site Class ") << strSiteClass[siteClass];
   if (specType != WSDOT_BDM)
   {
      (*pPara) << _T(" and ") << Sub2(_T("S"), _T("1")) << _T(" = ") << scalar.SetValue(spectra.GetS1());
   }
   (*pPara) << _T(", ") << Sub2(_T("F"), _T("v")) << _T(" = ") << scalar.SetValue(spectra.GetFv()) << rptNewLine;
   (*pPara) << rptNewLine;

   for (ColumnIndexType colIdx = 0; colIdx < nColumns; colIdx++)
   {
      pLongPeriodTable->SetColumnStyle(colIdx, rptStyleManager::GetTableCellStyle(CJ_CENTER));
      pLongPeriodTable->SetStripeRowColumnStyle(colIdx, rptStyleManager::GetTableStripeRowCellStyle(CJ_CENTER));
   }
   pLongPeriodTable->SetNumberOfHeaderRows(2);
   pLongPeriodTable->SetRowSpan(0, 0, 2);
   col = 0;
   (*pLongPeriodTable)(0, col++) << _T("Site Class");

   pLongPeriodTable->SetColumnSpan(0, col, nColumns-1);
   (*pLongPeriodTable)(0, col) << _T("Mapped Spectral Acceleration Coefficient at Period 1.0 sec (") << Sub2(_T("S"), _T("1")) << _T(")");

   (*pLongPeriodTable)(1, col++) << Sub2(_T("S"), _T("1")) << symbol(LTE) << _T(" 0.1");
   (*pLongPeriodTable)(1, col++) << Sub2(_T("S"), _T("1")) << _T("=") << _T(" 0.2");
   (*pLongPeriodTable)(1, col++) << Sub2(_T("S"), _T("1")) << _T("=") << _T(" 0.3");
   (*pLongPeriodTable)(1, col++) << Sub2(_T("S"), _T("1")) << _T("=") << _T(" 0.4");
   if (specType == WSDOT_BDM)
   {
      (*pLongPeriodTable)(1, col++) << Sub2(_T("S"), _T("1")) << _T("=") << _T(" 0.5");
      (*pLongPeriodTable)(1, col++) << Sub2(_T("S"), _T("1")) << symbol(GTE) << _T(" 0.6");
   }
   else
   {
      (*pLongPeriodTable)(1, col++) << Sub2(_T("S"), _T("1")) << symbol(GTE) << _T(" 0.5");
   }

   row = pZeroPeriodTable->GetNumberOfHeaderRows();
   for (int i = 0; i < (int)(nSiteClasses-1); i++, row++)
   {
      SiteClass thisSiteClass = (SiteClass)i;

      col = 0;
      const mathPwLinearFunction2dUsingPoints* pZeroPeriodSiteFactors = m_pDoc->GetZeroPeriodSiteFactors(specType, thisSiteClass);
      (*pZeroPeriodTable)(row, col++) << strSiteClass[thisSiteClass];
      (*pZeroPeriodTable)(row, col++) << table_value.SetValue(pZeroPeriodSiteFactors->Evaluate(0.1));
      (*pZeroPeriodTable)(row, col++) << table_value.SetValue(pZeroPeriodSiteFactors->Evaluate(0.2));
      (*pZeroPeriodTable)(row, col++) << table_value.SetValue(pZeroPeriodSiteFactors->Evaluate(0.3));
      (*pZeroPeriodTable)(row, col++) << table_value.SetValue(pZeroPeriodSiteFactors->Evaluate(0.4));
      (*pZeroPeriodTable)(row, col++) << table_value.SetValue(pZeroPeriodSiteFactors->Evaluate(0.5));
      if (specType == WSDOT_BDM)
      {
         (*pZeroPeriodTable)(row, col++) << table_value.SetValue(pZeroPeriodSiteFactors->Evaluate(0.6));
      }

      col = 0;
      const mathPwLinearFunction2dUsingPoints* pShortPeriodSiteFactors = m_pDoc->GetShortPeriodSiteFactors(specType, thisSiteClass);
      (*pShortPeriodTable)(row, col++) << strSiteClass[thisSiteClass];
      (*pShortPeriodTable)(row, col++) << table_value.SetValue(pShortPeriodSiteFactors->Evaluate(0.25));
      (*pShortPeriodTable)(row, col++) << table_value.SetValue(pShortPeriodSiteFactors->Evaluate(0.50));
      (*pShortPeriodTable)(row, col++) << table_value.SetValue(pShortPeriodSiteFactors->Evaluate(0.75));
      (*pShortPeriodTable)(row, col++) << table_value.SetValue(pShortPeriodSiteFactors->Evaluate(1.00));
      (*pShortPeriodTable)(row, col++) << table_value.SetValue(pShortPeriodSiteFactors->Evaluate(1.25));
      if (specType == WSDOT_BDM)
      {
         (*pShortPeriodTable)(row, col++) << table_value.SetValue(pShortPeriodSiteFactors->Evaluate(1.50));
      }

      col = 0;
      const mathPwLinearFunction2dUsingPoints* pLongPeriodSiteFactors = m_pDoc->GetLongPeriodSiteFactors(specType, thisSiteClass);
      (*pLongPeriodTable)(row, col++) << strSiteClass[thisSiteClass];
      (*pLongPeriodTable)(row, col++) << table_value.SetValue(pLongPeriodSiteFactors->Evaluate(0.1));
      (*pLongPeriodTable)(row, col++) << table_value.SetValue(pLongPeriodSiteFactors->Evaluate(0.2));
      (*pLongPeriodTable)(row, col++) << table_value.SetValue(pLongPeriodSiteFactors->Evaluate(0.3));
      (*pLongPeriodTable)(row, col++) << table_value.SetValue(pLongPeriodSiteFactors->Evaluate(0.4));
      (*pLongPeriodTable)(row, col++) << table_value.SetValue(pLongPeriodSiteFactors->Evaluate(0.5));
      if (specType == WSDOT_BDM)
      {
         (*pLongPeriodTable)(row, col++) << table_value.SetValue(pLongPeriodSiteFactors->Evaluate(0.6));
      }
   }
   
   (*pPara) << Sub2(_T("A"),_T("s")) << _T(" = ") << Sub2(_T("F"),_T("pga")) << _T(" PGA = (") << scalar.SetValue(spectra.GetFpga()) << _T(")(") << scalar.SetValue(spectra.GetPGA()) << _T("g) = ") << scalar.SetValue(spectra.GetAs()) << _T("g") << rptNewLine;
   (*pPara) << Sub2(_T("S"),_T("DS")) << _T(" = ") << Sub2(_T("F"),_T("a")) << _T(" ") << Sub2(_T("S"),_T("s")) << _T(" = (") << scalar.SetValue(spectra.GetFa()) << _T(")(") << scalar.SetValue(spectra.GetSs()) << _T("g) = ") << scalar.SetValue(spectra.GetSds()) << _T("g") << rptNewLine;
   (*pPara) << Sub2(_T("S"),_T("D1")) << _T(" = ") << Sub2(_T("F"),_T("v")) << _T(" ") << Sub2(_T("S"),_T("1")) << _T(" = (") << scalar.SetValue(spectra.GetFv()) << _T(")(") << scalar.SetValue(spectra.GetS1()) << _T("g) = ") << scalar.SetValue(spectra.GetSd1()) << _T("g") << rptNewLine;
   (*pPara) << Sub2(_T("T"),_T("o")) << _T(" = ") << _T("0.2") << Sub2(_T("T"),_T("s")) << _T(" = ") << _T("(0.2)(") << scalar.SetValue(spectra.GetTs()) << _T(") = ") << scalar.SetValue(spectra.GetTo()) << _T(" sec") << rptNewLine;
   (*pPara) << Sub2(_T("T"),_T("s")) << _T(" = ") << Sub2(_T("S"),_T("D1")) << _T("/") << Sub2(_T("S"),_T("DS")) << _T(" = (") << scalar.SetValue(spectra.GetSd1()) << _T(")/(") << scalar.SetValue(spectra.GetSds()) << _T(") = ") << scalar.SetValue(spectra.GetTs()) << _T(" sec") << rptNewLine;

   (*pPara) << rptNewPage;
   pPara = new rptParagraph; // create a new paragraph after a page break.
   (*pChapter) << pPara;

   if (specType == AASHTO_LRFD)
   {
      (*pPara) << _T("Partitions for Seismic Performance Zones 1, 2, 3, and 4") << rptNewLine;
   }
   else
   {
      (*pPara) << _T("Partitions for Seismic Design Categories A, B, C, and D") << rptNewLine;
   }
   pTable = rptStyleManager::CreateDefaultTable(2);
   pTable->SetColumnStyle(1,rptStyleManager::GetTableCellStyle(CJ_CENTER));
   pTable->SetStripeRowColumnStyle(1,rptStyleManager::GetTableStripeRowCellStyle(CJ_CENTER));

   (*pPara) << pTable << rptNewLine;
   (*pTable)(0,0) << Sub2(_T("S"),_T("D1"));
   if (specType == AASHTO_LRFD)
   {
      (*pTable)(0, 1) << _T("Zone");
   }
   else
   {
      (*pTable)(0, 1) << _T("SDC");
   }

   (*pTable)(1,0) << Sub2(_T("S"),_T("D1")) << _T(" < 0.15");
   if (specType == AASHTO_LRFD)
   {
      (*pTable)(1, 1) << _T("1");
   }
   else
   {
      (*pTable)(1, 1) << _T("A");
   }


   (*pTable)(2,0) << _T("0.15 ") << symbol(LTE) << _T(" ") << Sub2(_T("S"),_T("D1")) << _T(" < 0.30");
   if (specType == AASHTO_LRFD)
   {
      (*pTable)(2, 1) << _T("2");
   }
   else
   {
      (*pTable)(2, 1) << _T("B");
   }

   (*pTable)(3,0) << _T("0.30 ") << symbol(LTE) << _T(" ") << Sub2(_T("S"),_T("D1")) << _T(" < 0.50");
   if (specType == AASHTO_LRFD)
   {
      (*pTable)(3, 1) << _T("3");
   }
   else
   {
      (*pTable)(3, 1) << _T("C");
   }

   (*pTable)(4,0) << _T("0.50 ") << symbol(LTE) << _T(" ") << Sub2(_T("S"),_T("D1"));
   if (specType == AASHTO_LRFD)
   {
      (*pTable)(4, 1) << _T("4");
   }
   else
   {
      (*pTable)(4, 1) << _T("D");
   }

   if (specType == AASHTO_LRFD)
   {
      (*pPara) << _T("Seismic Performance Zone = ") << spectra.GetSDC(specType) << rptNewLine;
   }
   else
   {
      (*pPara) << _T("Seismic Design Category (SDC) = ") << spectra.GetSDC(specType) << rptNewLine;
   }

   (*pPara) << rptNewLine;

   rptRcTable* pLayoutTable = rptStyleManager::CreateLayoutTable(2);
   (*pPara) << pLayoutTable << rptNewLine;

   pPara = &(*pLayoutTable)(0,0);
   pTable = rptStyleManager::CreateDefaultTable(3);
   *pPara << pTable << rptNewLine;
   (*pTable)(0,0) << _T("Period, T") << rptNewLine << _T("(sec)");
   (*pTable)(0,1) << _T("Sa") << rptNewLine << _T("(g)");
   (*pTable)(0,2) << _T("");

   Float64 To = spectra.GetTo();
   Float64 Ts = spectra.GetTs();

   std::vector<std::pair<Float64,Float64>> values(spectra.GetSpectraValues(SPECTRUM_MAX_PERIOD,SPECTRUM_STEP_SIZE));
   row = pTable->GetNumberOfHeaderRows();
   std::vector<std::pair<Float64,Float64>>::iterator iter(values.begin());
   std::vector<std::pair<Float64,Float64>>::iterator end(values.end());
   for ( ; iter != end; iter++ )
   {
      Float64 t = iter->first;
      Float64 Sa = iter->second;
      (*pTable)(row,0) << scalar.SetValue(t);
      (*pTable)(row,1) << scalar.SetValue(Sa);
      if ( t == To )
      {
         (*pTable)(row,2) << Sub2(_T("T"),_T("o"));
      }
      else if (t == Ts )
      {
         (*pTable)(row,2) << Sub2(_T("T"),_T("s"));
      }
      else
      {
         (*pTable)(row,2) << _T("");
      }
      row++;
   }

   pPara = &(*pLayoutTable)(0,1);
   (*pPara) << CreateImage(values);

   return pChapter;
}

CChapterBuilder* CSpectraChapterBuilder::Clone() const
{
   return new CSpectraChapterBuilder(m_pDoc);
}

rptRcImage* CSpectraChapterBuilder::CreateImage(const std::vector<std::pair<Float64,Float64>>& values) const
{
   CImage image;
   image.Create(500,500,32);
   CRect rect(CPoint(0,0),CSize(image.GetWidth(),image.GetHeight()));

   CDC* pDC = CDC::FromHandle(image.GetDC());

   // fill the background of the image (otherwise it will be black)
   CBrush brush(GRAPH_BACKGROUND);
   CBrush* pOldBrush = pDC->SelectObject(&brush);
   pDC->Rectangle(rect);
   pDC->SelectObject(pOldBrush);

   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();
   WBFL::Units::TimeTool   TimeTool(pDispUnits->Time3);
   WBFL::Units::ScalarTool SaTool(pDispUnits->Scalar);
   WBFL::Graphing::GraphXY graph(TimeTool,SaTool);

   graph.SetOutputRect(rect);
   graph.SetClientAreaColor(GRAPH_BACKGROUND);
   graph.SetGridPenStyle(GRAPH_GRID_PEN_STYLE, GRAPH_GRID_PEN_WEIGHT, GRAPH_GRID_COLOR);

   graph.SetTitle(_T("Design Response Spectrum"));

   // Setup X-axis
   CString strPeriod(_T("Period, T (sec)"));
   graph.SetXAxisTitle(strPeriod.LockBuffer());
   strPeriod.UnlockBuffer();
   graph.XAxisNiceRange(true);
   graph.SetXAxisNumberOfMinorTics(0);
   graph.SetXAxisNumberOfMajorTics(11);

   // Setup Y-axis
   CString strCsm;
   strCsm.Format(_T("Response Spectral Acceleration, Sa (g)"));
   graph.SetYAxisTitle(strCsm.LockBuffer());
   strCsm.UnlockBuffer();
   graph.YAxisNiceRange(true);

   IndexType series = graph.CreateDataSeries(_T(""),PS_SOLID,1,BLUE);

   std::vector<std::pair<Float64,Float64>>::const_iterator iter(values.begin());
   std::vector<std::pair<Float64,Float64>>::const_iterator end(values.end());
   for ( ; iter != end; iter++ )
   {
      Float64 t = iter->first;
      Float64 csm = iter->second;
      graph.AddPoint(series, WBFL::Graphing::Point(t,csm));
   }

   graph.UpdateGraphMetrics(pDC->GetSafeHdc());
   graph.Draw(pDC->GetSafeHdc());

   image.ReleaseDC();


   // get a temporary file name for the image
   TCHAR temp_path[ _MAX_PATH ];
   TCHAR temp_file[ _MAX_PATH ];
   bool should_delete = true;

   if ( ::GetTempPath( _MAX_PATH, temp_path ) == 0 )
      _tcscpy_s(temp_path,_MAX_PATH,_T("C:\\")); // Couldn't establish a temp path, just use the root drive.

   //
   // Make sure the temp path actually exists
   // We do this by looking for any file in the directory.  If nothing shows up, then the
   // path doesn't exist. (Well, this isn't exactly true, but its the best I can come up
   // with).
   CFileFind finder;
   BOOL bExist;
   CString path(temp_path);
   if ( path[path.GetLength()-1] != '\\' )
   {
      path += _T("\\");
   }
   path += _T("*.*");
   bExist = finder.FindFile(path);
   if ( !bExist )
   {
      _tcscpy_s( temp_path,_MAX_PATH, _T("C:\\") );
   }

   // This creates a file called _T("temp_file").TMP
   if ( ::GetTempFileName( temp_path, _T("spectra_"), 0, temp_file ) == 0 )
   {
      // We could not get a temp name, so just use this default
      // (Use a tmp extension so it is in the same format as the one
      //  the OS would have created for us)
      _tcscpy_s( temp_file, _MAX_PATH, _T("spectra.tmp") );
      should_delete = false;
   }

   // Replace the TMP extension with png
   std::_tstring strFilename;
   strFilename.assign( temp_file );
   strFilename.replace( strFilename.end() - 3, strFilename.end(), _T("png") );

   // We don't want the file Windows created for us
   if ( should_delete )
   {
      ::DeleteFile( temp_file );
   }

   std::transform(strFilename.cbegin(),strFilename.cend(),strFilename.begin(),(int(*)(int))std::toupper);

   // this is a const function so we have to cast away const-ness to save
   // the file name
   CSpectraChapterBuilder* pMe = const_cast<CSpectraChapterBuilder*>(this);
   pMe->m_TemporaryFiles.push_back(strFilename);

   image.Save(strFilename.c_str(),Gdiplus::ImageFormatPNG);

   rptRcImage* pImage = new rptRcImage(strFilename.c_str(),rptRcImage::Baseline,rptRcImage::Left);
   return pImage;
}