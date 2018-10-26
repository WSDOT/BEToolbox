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

#include "StdAfx.h"
#include "SpectraChapterBuilder.h"
#include <Reporter\Reporter.h>

#include <GraphicsLib\GraphicsLib.h>

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

   rptRcScalar scalar;
   scalar.SetFormat( sysNumericFormatTool::Fixed );
   scalar.SetWidth(6);
   scalar.SetPrecision(3);

   std::_tstring strSiteClass[] = {_T("A"), _T("B"), _T("C"), _T("D"), _T("E")};

   rptChapter* pChapter = new rptChapter;
   rptParagraph* pPara;

   pPara = new rptParagraph;
   (*pChapter) << pPara;

   CResponseSpectra spectra;
   Uint32 result = m_pDoc->GetResponseSpectra(lat,lng,siteClass,&spectra);
   if ( result != SPECTRA_OK )
   {
      (*pPara) << _T("Response spectra could not be computed. ") << m_pDoc->GetSpectraResultExplaination(result) << rptNewLine;
      return pChapter;
   }

   (*pPara) << _T("2014 Seismic Hazard Map, 7% probability of exceedance in 75 years") << rptNewLine;
   (*pPara) << rptNewLine;
   (*pPara) << _T("Site Coordinates (Latitude,Longitude): ") << lat << _T("° N, ") << -lng << _T("° W") << rptNewLine;
   (*pPara) << _T("Site Soil Classification: ") << _T("Site Class ") << strSiteClass[siteClass] << _T(" - ") << m_pDoc->GetSiteClassDescription(siteClass) << rptNewLine;
   (*pPara) << rptNewLine;
   (*pPara) << _T("Seismic hazard maps are for Site Class B. Adjustments for other Site Classes are made as needed.") << rptNewLine;

   rptRcTable* pTable = rptStyleManager::CreateDefaultTable(3);
   (*pPara) << pTable << rptNewLine;
   (*pTable)(0,0) << _T("Period") << rptNewLine << _T("(sec)");
   (*pTable)(0,1) << _T("Sa") << rptNewLine << _T("(g)");
   (*pTable)(0,2) << _T("");
   
   RowIndexType row = pTable->GetNumberOfHeaderRows();
   (*pTable)(row,0) << _T("0.0");
   (*pTable)(row,1) << scalar.SetValue(spectra.GetPGA());
   (*pTable)(row,2) << _T("PGA - Site Class B");
   row++;

   (*pTable)(row,0) << _T("0.2");
   (*pTable)(row,1) << scalar.SetValue(spectra.GetSs());
   (*pTable)(row,2) << Sub2(_T("S"),_T("s")) << _T(" - Site Class B");
   row++;

   (*pTable)(row,0) << _T("1.0");
   (*pTable)(row,1) << scalar.SetValue(spectra.GetS1());
   (*pTable)(row,2) << Sub2(_T("S"),_T("1")) << _T(" - Site Class B");
   row++;

   (*pPara) << _T("Values of Site Coefficient, ") << Sub2(_T("F"),_T("pga")) << _T(", for Peak Ground Acceleration") << rptNewLine;
   ColumnIndexType nColumns = 7;
   pTable = rptStyleManager::CreateDefaultTable(nColumns);
   for ( ColumnIndexType colIdx = 0; colIdx < nColumns; colIdx++ )
   {
      pTable->SetColumnStyle(colIdx,rptStyleManager::GetTableCellStyle(CJ_CENTER));
      pTable->SetStripeRowColumnStyle(colIdx,rptStyleManager::GetTableStripeRowCellStyle(CJ_CENTER));
   }

   (*pPara) << pTable << rptNewLine;
   pTable->SetNumberOfHeaderRows(2);
   pTable->SetRowSpan(0,0,2);
   pTable->SetRowSpan(1,0,SKIP_CELL);
   (*pTable)(0,0) << _T("Site Class");

   pTable->SetColumnSpan(0,1,6);
   pTable->SetColumnSpan(0,2,SKIP_CELL);
   pTable->SetColumnSpan(0,3,SKIP_CELL);
   pTable->SetColumnSpan(0,4,SKIP_CELL);
   pTable->SetColumnSpan(0,5,SKIP_CELL);
   pTable->SetColumnSpan(0,6,SKIP_CELL);
   (*pTable)(0,1) << _T("Mapped Peak Ground Acceleration Coefficient (PGA)");

   (*pTable)(1,1) << _T("PGA") << symbol(LTE) << _T(" 0.10");
   (*pTable)(1,2) << _T("PGA") << _T("=")     << _T(" 0.20");
   (*pTable)(1,3) << _T("PGA") << _T("=")     << _T(" 0.30");
   (*pTable)(1,4) << _T("PGA") << _T("=")     << _T(" 0.40");
   (*pTable)(1,5) << _T("PGA") << _T("=")     << _T(" 0.50");
   (*pTable)(1,6) << _T("PGA") << symbol(GTE) << _T(" 0.60");

   (*pTable)(2,0) << _T("A");
   (*pTable)(2,1) << _T("0.8");
   (*pTable)(2,2) << _T("0.8");
   (*pTable)(2,3) << _T("0.8");
   (*pTable)(2,4) << _T("0.8");
   (*pTable)(2,5) << _T("0.8");
   (*pTable)(2,6) << _T("0.8");

   (*pTable)(3,0) << _T("B");
   (*pTable)(3,1) << _T("0.9");
   (*pTable)(3,2) << _T("0.9");
   (*pTable)(3,3) << _T("0.9");
   (*pTable)(3,4) << _T("0.9");
   (*pTable)(3,5) << _T("0.9");
   (*pTable)(3,6) << _T("0.9");

   (*pTable)(4,0) << _T("C");
   (*pTable)(4,1) << _T("1.3");
   (*pTable)(4,2) << _T("1.2");
   (*pTable)(4,3) << _T("1.2");
   (*pTable)(4,4) << _T("1.2");
   (*pTable)(4,5) << _T("1.2");
   (*pTable)(4,6) << _T("1.2");

   (*pTable)(5,0) << _T("D");
   (*pTable)(5,1) << _T("1.6");
   (*pTable)(5,2) << _T("1.4");
   (*pTable)(5,3) << _T("1.3");
   (*pTable)(5,4) << _T("1.2");
   (*pTable)(5,5) << _T("1.1");
   (*pTable)(5,6) << _T("1.1");

   (*pTable)(6,0) << _T("E");
   (*pTable)(6,1) << _T("2.4");
   (*pTable)(6,2) << _T("1.9");
   (*pTable)(6,3) << _T("1.6");
   (*pTable)(6,4) << _T("1.4");
   (*pTable)(6,5) << _T("1.2");
   (*pTable)(6,6) << _T("1.1");

   (*pPara) << _T("For Site Class ") << strSiteClass[siteClass] << _T(" and PGA = ") << scalar.SetValue(spectra.GetPGA()) << _T(", ") << Sub2(_T("F"),_T("pga")) << _T(" = ") << scalar.SetValue(spectra.GetFpga()) << rptNewLine;
   (*pPara) << rptNewLine;

   (*pPara) << _T("Values for Site Coefficient, ") << Sub2(_T("F"),_T("a")) << _T(", for 0.2 sec Period Spectral Acceleration") << rptNewLine;
   nColumns = 7;
   pTable = rptStyleManager::CreateDefaultTable(nColumns);
   for ( ColumnIndexType colIdx = 0; colIdx < nColumns; colIdx++ )
   {
      pTable->SetColumnStyle(colIdx,rptStyleManager::GetTableCellStyle(CJ_CENTER));
      pTable->SetStripeRowColumnStyle(colIdx,rptStyleManager::GetTableStripeRowCellStyle(CJ_CENTER));
   }
   (*pPara) << pTable << rptNewLine;
   pTable->SetNumberOfHeaderRows(2);
   pTable->SetRowSpan(0,0,2);
   pTable->SetRowSpan(1,0,SKIP_CELL);
   (*pTable)(0,0) << _T("Site Class");

   pTable->SetColumnSpan(0,1,6);
   pTable->SetColumnSpan(0,2,SKIP_CELL);
   pTable->SetColumnSpan(0,3,SKIP_CELL);
   pTable->SetColumnSpan(0,4,SKIP_CELL);
   pTable->SetColumnSpan(0,5,SKIP_CELL);
   pTable->SetColumnSpan(0,6,SKIP_CELL);
   (*pTable)(0,1) << _T("Mapped Spectral Acceleration Coefficient at Period 0.2 sec (") << Sub2(_T("S"),_T("s")) << _T(")");

   (*pTable)(1,1) << Sub2(_T("S"),_T("s")) << symbol(LTE) << _T(" 0.25");
   (*pTable)(1,2) << Sub2(_T("S"),_T("s")) << _T("=")     << _T(" 0.50");
   (*pTable)(1,3) << Sub2(_T("S"),_T("s")) << _T("=")     << _T(" 0.75");
   (*pTable)(1,4) << Sub2(_T("S"),_T("s")) << _T("=")     << _T(" 1.00");
   (*pTable)(1,5) << Sub2(_T("S"),_T("s")) << _T("=")     << _T(" 1.25");
   (*pTable)(1,6) << Sub2(_T("S"),_T("s")) << symbol(GTE) << _T(" 1.50");

   (*pTable)(2,0) << _T("A");
   (*pTable)(2,1) << _T("0.8");
   (*pTable)(2,2) << _T("0.8");
   (*pTable)(2,3) << _T("0.8");
   (*pTable)(2,4) << _T("0.8");
   (*pTable)(2,5) << _T("0.8");
   (*pTable)(2,6) << _T("0.8");

   (*pTable)(3,0) << _T("B");
   (*pTable)(3,1) << _T("0.9");
   (*pTable)(3,2) << _T("0.9");
   (*pTable)(3,3) << _T("0.9");
   (*pTable)(3,4) << _T("0.9");
   (*pTable)(3,5) << _T("0.9");
   (*pTable)(3,6) << _T("0.9");

   (*pTable)(4,0) << _T("C");
   (*pTable)(4,1) << _T("1.3");
   (*pTable)(4,2) << _T("1.3");
   (*pTable)(4,3) << _T("1.2");
   (*pTable)(4,4) << _T("1.2");
   (*pTable)(4,5) << _T("1.2");
   (*pTable)(4,6) << _T("1.2");

   (*pTable)(5,0) << _T("D");
   (*pTable)(5,1) << _T("1.6");
   (*pTable)(5,2) << _T("1.4");
   (*pTable)(5,3) << _T("1.2");
   (*pTable)(5,4) << _T("1.1");
   (*pTable)(5,5) << _T("1.0");
   (*pTable)(5,6) << _T("1.0");

   (*pTable)(6,0) << _T("E");
   (*pTable)(6,1) << _T("2.4");
   (*pTable)(6,2) << _T("1.7");
   (*pTable)(6,3) << _T("1.3");
   (*pTable)(6,4) << _T("*");
   (*pTable)(6,5) << _T("*");
   (*pTable)(6,6) << _T("*");

   (*pPara) << _T("For Site Class ") << strSiteClass[siteClass] << _T(" and ") << Sub2(_T("S"),_T("s")) << _T(" = ") << scalar.SetValue(spectra.GetSs()) << _T(", ") << Sub2(_T("F"),_T("a")) << _T(" = ") << scalar.SetValue(spectra.GetFa()) << rptNewLine;
   (*pPara) << rptNewLine;

   (*pPara) << _T("Values of Site Coefficient, ") << Sub2(_T("F"),_T("v")) << _T(", for 1.0 sec Period Spectral Acceleration") << rptNewLine;
   nColumns = 7;
   pTable = rptStyleManager::CreateDefaultTable(nColumns);
   for ( ColumnIndexType colIdx = 0; colIdx < nColumns; colIdx++ )
   {
      pTable->SetColumnStyle(colIdx,rptStyleManager::GetTableCellStyle(CJ_CENTER));
      pTable->SetStripeRowColumnStyle(colIdx,rptStyleManager::GetTableStripeRowCellStyle(CJ_CENTER));
   }
   (*pPara) << pTable << rptNewLine;
   pTable->SetNumberOfHeaderRows(2);
   pTable->SetRowSpan(0,0,2);
   pTable->SetRowSpan(1,0,SKIP_CELL);
   (*pTable)(0,0) << _T("Site Class");

   pTable->SetColumnSpan(0,1,6);
   pTable->SetColumnSpan(0,2,SKIP_CELL);
   pTable->SetColumnSpan(0,3,SKIP_CELL);
   pTable->SetColumnSpan(0,4,SKIP_CELL);
   pTable->SetColumnSpan(0,5,SKIP_CELL);
   pTable->SetColumnSpan(0,6,SKIP_CELL);
   (*pTable)(0,1) << _T("Mapped Spectral Acceleration Coefficient at Period 1.0 sec (") << Sub2(_T("S"),_T("1")) << _T(")");

   (*pTable)(1,1) << Sub2(_T("S"),_T("1")) << symbol(LTE) << _T(" 0.1");
   (*pTable)(1,2) << Sub2(_T("S"),_T("1")) << _T("=")     << _T(" 0.2");
   (*pTable)(1,3) << Sub2(_T("S"),_T("1")) << _T("=")     << _T(" 0.3");
   (*pTable)(1,4) << Sub2(_T("S"),_T("1")) << _T("=")     << _T(" 0.4");
   (*pTable)(1,5) << Sub2(_T("S"),_T("1")) << _T("=")     << _T(" 0.5");
   (*pTable)(1,6) << Sub2(_T("S"),_T("1")) << symbol(GTE) << _T(" 0.6");


   (*pTable)(2,0) << _T("A");
   (*pTable)(2,1) << _T("0.8");
   (*pTable)(2,2) << _T("0.8");
   (*pTable)(2,3) << _T("0.8");
   (*pTable)(2,4) << _T("0.8");
   (*pTable)(2,5) << _T("0.8");
   (*pTable)(2,6) << _T("0.8");

   (*pTable)(3,0) << _T("B");
   (*pTable)(3,1) << _T("0.8");
   (*pTable)(3,2) << _T("0.8");
   (*pTable)(3,3) << _T("0.8");
   (*pTable)(3,4) << _T("0.8");
   (*pTable)(3,5) << _T("0.8");
   (*pTable)(3,6) << _T("0.8");

   (*pTable)(4,0) << _T("C");
   (*pTable)(4,1) << _T("1.5");
   (*pTable)(4,2) << _T("1.5");
   (*pTable)(4,3) << _T("1.5");
   (*pTable)(4,4) << _T("1.5");
   (*pTable)(4,5) << _T("1.5");
   (*pTable)(4,6) << _T("1.4");

   (*pTable)(5,0) << _T("D");
   (*pTable)(5,1) << _T("2.4");
   (*pTable)(5,2) << _T("2.2");
   (*pTable)(5,3) << _T("2.0");
   (*pTable)(5,4) << _T("1.9");
   (*pTable)(5,5) << _T("1.8");
   (*pTable)(5,6) << _T("1.7");

   (*pTable)(6,0) << _T("E");
   (*pTable)(6,1) << _T("4.2");
   (*pTable)(6,2) << _T("3.3");
   (*pTable)(6,3) << _T("2.8");
   (*pTable)(6,4) << _T("2.4");
   (*pTable)(6,5) << _T("2.2");
   (*pTable)(6,6) << _T("2.0");

   (*pPara) << _T("For Site Class ") << strSiteClass[siteClass] << _T(" and ") << Sub2(_T("S"),_T("1")) << _T(" = ") << scalar.SetValue(spectra.GetS1()) << _T(", ") << Sub2(_T("F"),_T("v")) << _T(" = ") << scalar.SetValue(spectra.GetFv()) << rptNewLine;
   (*pPara) << rptNewLine;

   (*pPara) << Sub2(_T("A"),_T("s")) << _T(" = ") << Sub2(_T("F"),_T("pga")) << _T(" PGA = (") << scalar.SetValue(spectra.GetFpga()) << _T(")(") << scalar.SetValue(spectra.GetPGA()) << _T("g) = ") << scalar.SetValue(spectra.GetAs()) << _T("g") << rptNewLine;
   (*pPara) << Sub2(_T("S"),_T("DS")) << _T(" = ") << Sub2(_T("F"),_T("a")) << _T(" ") << Sub2(_T("S"),_T("s")) << _T(" = (") << scalar.SetValue(spectra.GetFa()) << _T(")(") << scalar.SetValue(spectra.GetSs()) << _T("g) = ") << scalar.SetValue(spectra.GetSds()) << _T("g") << rptNewLine;
   (*pPara) << Sub2(_T("S"),_T("D1")) << _T(" = ") << Sub2(_T("F"),_T("v")) << _T(" ") << Sub2(_T("S"),_T("1")) << _T(" = (") << scalar.SetValue(spectra.GetFv()) << _T(")(") << scalar.SetValue(spectra.GetS1()) << _T("g) = ") << scalar.SetValue(spectra.GetSd1()) << _T("g") << rptNewLine;
   (*pPara) << Sub2(_T("T"),_T("o")) << _T(" = ") << _T("0.2") << Sub2(_T("T"),_T("s")) << _T(" = ") << _T("(0.2)(") << scalar.SetValue(spectra.GetTs()) << _T(") = ") << scalar.SetValue(spectra.GetTo()) << _T(" sec") << rptNewLine;
   (*pPara) << Sub2(_T("T"),_T("s")) << _T(" = ") << Sub2(_T("S"),_T("D1")) << _T("/") << Sub2(_T("S"),_T("DS")) << _T(" = (") << scalar.SetValue(spectra.GetSd1()) << _T(")/(") << scalar.SetValue(spectra.GetSds()) << _T(") = ") << scalar.SetValue(spectra.GetTs()) << _T(" sec") << rptNewLine;

   (*pPara) << rptNewPage;
   pPara = new rptParagraph; // create a new paragraph after a page break.
   (*pChapter) << pPara;

   (*pPara) << _T("Partitions for Seismic Design Categories A, B, C, and D") << rptNewLine;
   pTable = rptStyleManager::CreateDefaultTable(2);
   pTable->SetColumnStyle(1,rptStyleManager::GetTableCellStyle(CJ_CENTER));
   pTable->SetStripeRowColumnStyle(1,rptStyleManager::GetTableStripeRowCellStyle(CJ_CENTER));

   (*pPara) << pTable << rptNewLine;
   (*pTable)(0,0) << Sub2(_T("S"),_T("D1"));
   (*pTable)(0,1) << _T("SDC");

   (*pTable)(1,0) << Sub2(_T("S"),_T("D1")) << _T(" < 0.15");
   (*pTable)(1,1) << _T("A");

   (*pTable)(2,0) << _T("0.15 ") << symbol(LTE) << _T(" ") << Sub2(_T("S"),_T("D1")) << _T(" < 0.30");
   (*pTable)(2,1) << _T("B");

   (*pTable)(3,0) << _T("0.30 ") << symbol(LTE) << _T(" ") << Sub2(_T("S"),_T("D1")) << _T(" < 0.50");
   (*pTable)(3,1) << _T("C");

   (*pTable)(4,0) << _T("0.50 ") << symbol(LTE) << _T(" ") << Sub2(_T("S"),_T("D1"));
   (*pTable)(4,1) << _T("D");

   (*pPara) << _T("Seismic Design Category (SDC) = ") << spectra.GetSDC() << rptNewLine;

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
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();
   TimeTool   TimeTool(pDispUnits->Time3);
   ScalarTool SaTool(pDispUnits->Scalar);
   grGraphXY graph(TimeTool,SaTool);

   graph.SetOutputRect(rect);
   graph.SetClientAreaColor(GRAPH_BACKGROUND);
   graph.SetGridPenStyle(GRAPH_GRID_PEN_STYLE, GRAPH_GRID_PEN_WEIGHT, GRAPH_GRID_COLOR);

   graph.SetTitle(_T("Design Response Spectrum"));

   // Setup X-axis
   CString strPeriod(_T("Period, T (sec)"));
   graph.SetXAxisTitle(strPeriod.LockBuffer());
   strPeriod.UnlockBuffer();
   graph.SetXAxisNiceRange(true);
   graph.SetXAxisNumberOfMinorTics(0);
   graph.SetXAxisNumberOfMajorTics(11);

   // Setup Y-axis
   CString strCsm;
   strCsm.Format(_T("Response Spectral Acceleration, Sa (g)"));
   graph.SetYAxisTitle(strCsm.LockBuffer());
   strCsm.UnlockBuffer();
   graph.SetYAxisNiceRange(true);

   IndexType series = graph.CreateDataSeries(_T(""),PS_SOLID,1,BLUE);

   std::vector<std::pair<Float64,Float64>>::const_iterator iter(values.begin());
   std::vector<std::pair<Float64,Float64>>::const_iterator end(values.end());
   for ( ; iter != end; iter++ )
   {
      Float64 t = iter->first;
      Float64 csm = iter->second;
      graph.AddPoint(series,gpPoint2d(t,csm));      
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

   std::transform(strFilename.begin(),strFilename.end(),strFilename.begin(),(int(*)(int))std::toupper);

   // this is a const function so we have to cast away const-ness to save
   // the file name
   CSpectraChapterBuilder* pMe = const_cast<CSpectraChapterBuilder*>(this);
   pMe->m_TemporaryFiles.push_back(strFilename);

   image.Save(strFilename.c_str(),Gdiplus::ImageFormatPNG);

   rptRcImage* pImage = new rptRcImage(strFilename.c_str(),rptRcImage::Baseline,rptRcImage::Left);
   return pImage;
}