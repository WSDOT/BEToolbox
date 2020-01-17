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
#include "M3CChapterBuilder.h"
#include "BEToolboxColors.h"
#include <Reporter\Reporter.h>
#include <GraphicsLib\GraphicsLib.h>

#include <algorithm>
#include <cctype>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////
CM3CChapterBuilder::CM3CChapterBuilder(CM3CDoc* pDoc)
{
   m_pDoc = pDoc;
}

CM3CChapterBuilder::~CM3CChapterBuilder(void)
{
   for (const auto& file : m_TemporaryFiles)
   {
      ::DeleteFile( file.c_str() );
   }
}

LPCTSTR CM3CChapterBuilder::GetName() const
{
   return _T("M3C");
}

Uint16 CM3CChapterBuilder::GetMaxLevel() const
{
   return 1;
}

rptChapter* CM3CChapterBuilder::Build(CReportSpecification* pRptSpec, Uint16 level) const
{
   rptChapter* pChapter = new rptChapter;
   rptParagraph* pPara;

   pPara = new rptParagraph;
   (*pChapter) << pPara;

   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDisplayUnits = pApp->GetDisplayUnits();

   
   rptRcTable* pLayoutTable = rptStyleManager::CreateLayoutTable(2);
   (*pPara) << pLayoutTable;

   INIT_UV_PROTOTYPE(rptMomentUnitValue, moment, pDisplayUnits->SmallMoment, false);
   INIT_UV_PROTOTYPE(rptPerLengthUnitValue, curvature, pDisplayUnits->Curvature, false);

   CComPtr<IMomentCurvatureSolution> solution;
   m_pDoc->GetMomentCurvature(&solution);

   (*pLayoutTable)(0, 1) << CreateImage(solution);

   const CM3CProblemParameters& problemParams = m_pDoc->GetProblemParameters();
   Float64 radius = problemParams.D / 2;

   rptRcTable* pTable = rptStyleManager::CreateDefaultTable(6);
   (*pLayoutTable)(0, 0) << pTable;

   ColumnIndexType col = 0;
   (*pTable)(0, col++) << COLHDR(_T("Curvature"), rptPerLengthUnitTag, pDisplayUnits->Curvature);
   (*pTable)(0, col++) << COLHDR(_T("Moment"), rptMomentUnitTag, pDisplayUnits->SmallMoment);
   (*pTable)(0, col++) << COLHDR(_T("Curvature"), rptPerLengthUnitTag, pDisplayUnits->Curvature);
   (*pTable)(0, col++) << COLHDR(_T("Moment"), rptMomentUnitTag, pDisplayUnits->SmallMoment);
   (*pTable)(0, col++) << COLHDR(_T("Curvature"), rptPerLengthUnitTag, pDisplayUnits->Curvature);
   (*pTable)(0, col++) << COLHDR(_T("Moment"), rptMomentUnitTag, pDisplayUnits->SmallMoment);

   IndexType nSolutionPoints;
   solution->get_PointCount(&nSolutionPoints);

   RowIndexType row = pTable->GetNumberOfHeaderRows();
   IndexType nRows = nSolutionPoints / 3;
   for (IndexType r = 0; r < nRows; r++, row++)
   {
      col = 0;
      for (IndexType i = 0; i < 3; i++)
      {
         IndexType idx = row + i*nRows;
         if (idx < nSolutionPoints)
         {
            Float64 m, k;
            solution->get_Moment(idx, &m);
            solution->get_Curvature(idx, &k);
            (*pTable)(row, col++) << curvature.SetValue(k);
            (*pTable)(row, col++) << moment.SetValue(m);
         }
         else
         {
            (*pTable)(row, col++) << _T("");
            (*pTable)(row, col++) << _T("");
         }
      }
   }

   //for (IndexType i = 0; i < nSolutionPoints; i++)
   //{
   //   Float64 m, k;
   //   solution->get_Moment(i, &m);
   //   solution->get_Curvature(i, &k);

   //   ColumnIndexType col = 0;
   //   (*pTable)(row, col++) << curvature.SetValue(k);
   //   (*pTable)(row, col++) << moment.SetValue(m);
   //   (*pTable)(row, col++) << etl;
   //   (*pTable)(row, col++) << etr;
   //   (*pTable)(row, col++) << ebl;
   //   (*pTable)(row, col++) << ebr;

   //   row++;
   //}


   return pChapter;
}

CChapterBuilder* CM3CChapterBuilder::Clone() const
{
   return new CM3CChapterBuilder(m_pDoc);
}

rptRcImage* CM3CChapterBuilder::CreateImage(IMomentCurvatureSolution* pSolution) const
{
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   CImage image;
   image.Create(500,500,32);
   CRect rect(CPoint(0,0),CSize(image.GetWidth(),image.GetHeight()));

   CDC* pDC = CDC::FromHandle(image.GetDC());

   // fill the background of the image (otherwise it will be black)
   CBrush brush(GRAPH_BACKGROUND);
   CBrush* pOldBrush = pDC->SelectObject(&brush);
   pDC->Rectangle(rect);
   pDC->SelectObject(pOldBrush);

   MomentTool momentTool(pDispUnits->SmallMoment);
   CurvatureTool  curvatureTool(pDispUnits->Curvature);
   grGraphXY graph(curvatureTool,momentTool);

   graph.SetOutputRect(rect);
   graph.SetClientAreaColor(GRAPH_BACKGROUND);
   graph.SetGridPenStyle(GRAPH_GRID_PEN_STYLE, GRAPH_GRID_PEN_WEIGHT, GRAPH_GRID_COLOR);

   graph.SetTitle(_T("Moment-Curvature"));

   // Setup X-axis
   CString strCurvature;
   strCurvature.Format(_T("Curvature (%s)"), pDispUnits->Curvature.UnitOfMeasure.UnitTag().c_str());
   graph.SetXAxisTitle(strCurvature.LockBuffer());
   strCurvature.UnlockBuffer();
   graph.SetXAxisNiceRange(true);
   graph.SetXAxisNumberOfMinorTics(5);
   //graph.SetXAxisNumberOfMajorTics(21);
   graph.SetXAxisLabelAngle(350); // 35 degrees

   // Setup Y-axis
   CString strMoment;
   strMoment.Format(_T("Moment (%s)"),pDispUnits->SmallMoment.UnitOfMeasure.UnitTag().c_str());
   graph.SetYAxisTitle(strMoment.LockBuffer());
   strMoment.UnlockBuffer();
   graph.SetYAxisNiceRange(true);
   graph.SetYAxisNumberOfMinorTics(0);
   graph.SetYAxisNumberOfMajorTics(11);

   IndexType series1 = graph.CreateDataSeries(_T(""),PS_SOLID,1,BLUE);

   Float64 xMin = DBL_MAX;
   Float64 xMax = -DBL_MAX;
   Float64 yMin = DBL_MAX;
   Float64 yMax = -DBL_MAX;
   IndexType nSolutionPoints;
   pSolution->get_PointCount(&nSolutionPoints);
   for (IndexType i = 0; i < nSolutionPoints; i++)
   {
      Float64 m, k;
      pSolution->get_Moment(i, &m);
      pSolution->get_Curvature(i, &k);

      m = ::ConvertFromSysUnits(m, pDispUnits->SmallMoment.UnitOfMeasure);
      k = ::ConvertFromSysUnits(k, pDispUnits->Curvature.UnitOfMeasure);

      xMin = Min(xMin, k);
      xMax = Max(xMax, k);
      yMin = Min(yMin, m);
      yMax = Max(yMax, m);

      graph.AddPoint(series1,gpPoint2d(k,m));      
   }

   graph.SetMinimumSize(xMin, xMax, yMin, yMax);


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
   if ( ::GetTempFileName( temp_path, _T("m3c_"), 0, temp_file ) == 0 )
   {
      // We could not get a temp name, so just use this default
      // (Use a tmp extension so it is in the same format as the one
      //  the OS would have created for us)
      _tcscpy_s( temp_file, _MAX_PATH, _T("m3c.tmp") );
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
   CM3CChapterBuilder* pMe = const_cast<CM3CChapterBuilder*>(this);
   pMe->m_TemporaryFiles.push_back(strFilename);

   image.Save(strFilename.c_str(),Gdiplus::ImageFormatPNG);

   rptRcImage* pImage = new rptRcImage(strFilename.c_str(),rptRcImage::Baseline,rptRcImage::Right);
   return pImage;
}
