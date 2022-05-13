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
#include "UltColChapterBuilder.h"
#include "..\BEToolboxColors.h"
#include <Reporter\Reporter.h>
#include <Graphing/GraphXY.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////
CUltColChapterBuilder::CUltColChapterBuilder(CUltColDoc* pDoc)
{
   m_pDoc = pDoc;
}

CUltColChapterBuilder::~CUltColChapterBuilder(void)
{
   for (const auto& file : m_TemporaryFiles)
   {
      ::DeleteFile( file.c_str() );
   }
}

LPCTSTR CUltColChapterBuilder::GetName() const
{
   return _T("UltCol");
}

Uint16 CUltColChapterBuilder::GetMaxLevel() const
{
   return 1;
}

rptChapter* CUltColChapterBuilder::Build(CReportSpecification* pRptSpec,Uint16 level) const
{
   rptChapter* pChapter = new rptChapter;
   rptParagraph* pPara;

   if ( m_pDoc->m_Column == nullptr )
   {
       pPara = new rptParagraph;
      (*pChapter) << pPara;
      *pPara << _T("Press the Update button to compute the column interaction") << rptNewLine;
      return pChapter;
   }

   pPara = new rptParagraph;
   (*pChapter) << pPara;

   rptRcTable* pLayoutTable = rptStyleManager::CreateLayoutTable(2);
   (*pPara) << pLayoutTable;

   //
   // Echo Input
   //
   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   INIT_UV_PROTOTYPE( rptLengthUnitValue,  length, pDispUnits->ComponentDim, true);
   INIT_UV_PROTOTYPE( rptAreaUnitValue,    area,   pDispUnits->Area,         true);
   INIT_UV_PROTOTYPE( rptStressUnitValue,  stress, pDispUnits->Stress,       true);
   INIT_UV_PROTOTYPE( rptStressUnitValue,  modE,   pDispUnits->ModE,         true);

   Float64 diameter, cover, As, fc, fy, Es, ecl, etl;
   m_pDoc->m_Column->get_Diameter(&diameter);
   m_pDoc->m_Column->get_Cover(&cover);
   m_pDoc->m_Column->get_As(&As);
   m_pDoc->m_Column->get_fc(&fc);
   m_pDoc->m_Column->get_fy(&fy);
   m_pDoc->m_Column->get_Es(&Es);
   ecl = m_pDoc->m_ecl;
   etl = m_pDoc->m_etl;

   (*pLayoutTable)(0,1) << _T("Diameter = ") << length.SetValue(diameter) << rptNewLine;
   (*pLayoutTable)(0,1) << Sub2(_T("f'"),_T("c")) << _T(" = ") << stress.SetValue(fc) << rptNewLine;
   (*pLayoutTable)(0,1) << _T("Cover = ") << length.SetValue(cover) << rptNewLine;
   (*pLayoutTable)(0,1) << Sub2(_T("A"),_T("s")) << _T(" = ") << area.SetValue(As) << rptNewLine;
   (*pLayoutTable)(0,1) << Sub2(_T("E"),_T("s")) << _T(" = ") << modE.SetValue(Es) << rptNewLine;
   (*pLayoutTable)(0,1) << Sub2(_T("f"),_T("y")) << _T(" = ") << stress.SetValue(fy) << rptNewLine;
   (*pLayoutTable)(0,1) << Sub2(symbol(epsilon),_T("cl")) << _T(" = ") << ecl << rptNewLine;
   (*pLayoutTable)(0,1) << Sub2(symbol(epsilon),_T("tl")) << _T(" = ") << etl << rptNewLine;
   (*pLayoutTable)(0,1) << _T("0.75 ") << symbol(LTE) << _T(" ") << symbol(phi) << _T(" = 0.75 + 0.15(") << Sub2(symbol(epsilon),_T("t")) << _T(" - ") << Sub2(symbol(epsilon),_T("cl")) << _T(")/(") << Sub2(symbol(epsilon),_T("tl")) << _T(" - ") << Sub2(symbol(epsilon),_T("cl"))<< _T(") ") << symbol(LTE) << _T(" 0.9") << rptNewLine;

   (*pLayoutTable)(0,1) << rptNewLine;

   //
   // DO THE COLUMN ANALYSIS
   //
   CComPtr<IPoint2dCollection> unfactored, factored;
   m_pDoc->m_Column->ComputeInteractionEx(35,ecl,etl,&unfactored,&factored);

   (*pLayoutTable)(0,1) << CreateImage(unfactored,factored);

   //
   // Create the table
   //
   INIT_UV_PROTOTYPE( rptForceUnitValue,  axial,  pDispUnits->GeneralForce, false);
   INIT_UV_PROTOTYPE( rptMomentUnitValue, moment, pDispUnits->Moment,  false);

   rptRcTable* pTable = rptStyleManager::CreateDefaultTable(5);
   (*pLayoutTable)(0,0) << pTable;

   (*pTable)(0,0) << COLHDR(Sub2(_T("M"),_T("n")), rptMomentUnitTag, pDispUnits->Moment);
   (*pTable)(0,1) << COLHDR(Sub2(_T("P"),_T("n")), rptForceUnitTag,  pDispUnits->GeneralForce);
   (*pTable)(0,2) << symbol(phi);
   (*pTable)(0,3) << COLHDR(symbol(phi) << Sub2(_T("M"),_T("n")), rptMomentUnitTag, pDispUnits->Moment);
   (*pTable)(0,4) << COLHDR(symbol(phi) << Sub2(_T("P"),_T("n")), rptForceUnitTag,  pDispUnits->GeneralForce);


   RowIndexType row = pTable->GetNumberOfHeaderRows();

   CollectionIndexType nPoints;
   unfactored->get_Count(&nPoints);
   for ( CollectionIndexType i = 0; i < nPoints; i++ )
   {
      CComPtr<IPoint2d> pn;
      unfactored->get_Item(i,&pn);

      CComPtr<IPoint2d> pr;
      factored->get_Item(i,&pr);

      Float64 Mn,Pn;
      pn->get_X(&Mn);
      pn->get_Y(&Pn);

      Float64 Mr,Pr;
      pr->get_X(&Mr);
      pr->get_Y(&Pr);

      Float64 phi = Pr/Pn;
      ATLASSERT( IsEqual(phi,Mr/Mn) );

      (*pTable)(row,0) << moment.SetValue(Mn);
      (*pTable)(row,1) << axial.SetValue(-Pn); // minus because we want tension to be < 0
      (*pTable)(row,2) << phi;
      (*pTable)(row,3) << moment.SetValue(Mr);
      (*pTable)(row,4) << axial.SetValue(-Pr); // minus because we want tension to be < 0

      row++;
   }


   return pChapter;
}

CChapterBuilder* CUltColChapterBuilder::Clone() const
{
   return new CUltColChapterBuilder(m_pDoc);
}

rptRcImage* CUltColChapterBuilder::CreateImage(IPoint2dCollection* unfactored,IPoint2dCollection* factored) const
{
   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   CImage image;
   image.Create(500,500,32);
   CRect rect(CPoint(0,0),CSize(image.GetWidth(),image.GetHeight()));

   CDC* pDC = CDC::FromHandle(image.GetDC());

   // fill the background of the image (otherwise it will be black)
   CBrush brush(GRAPH_BACKGROUND);
   CBrush* pOldBrush = pDC->SelectObject(&brush);
   pDC->Rectangle(rect);
   pDC->SelectObject(pOldBrush);

   WBFL::Units::MomentTool momentTool(pDispUnits->Moment);
   WBFL::Units::AxialTool  axialTool(pDispUnits->GeneralForce);
   WBFL::Graphing::GraphXY graph(momentTool,axialTool);

   graph.SetOutputRect(rect);
   graph.SetClientAreaColor(GRAPH_BACKGROUND);
   graph.SetGridPenStyle(GRAPH_GRID_PEN_STYLE, GRAPH_GRID_PEN_WEIGHT, GRAPH_GRID_COLOR);

   graph.SetTitle(_T("Interaction Diagram"));

   // Setup X-axis
   CString strMoment;
   strMoment.Format(_T("Moment (%s)"),pDispUnits->Moment.UnitOfMeasure.UnitTag().c_str());
   graph.SetXAxisTitle(strMoment.LockBuffer());
   strMoment.UnlockBuffer();
   graph.XAxisNiceRange(true);
   graph.SetXAxisNumberOfMinorTics(0);
   graph.SetXAxisNumberOfMajorTics(11);
   graph.SetXAxisLabelAngle(350); // 35 degrees

   // Setup Y-axis
   CString strAxial;
   strAxial.Format(_T("Axial (%s)"),pDispUnits->GeneralForce.UnitOfMeasure.UnitTag().c_str());
   graph.SetYAxisTitle(strAxial.LockBuffer());
   strAxial.UnlockBuffer();
   graph.YAxisNiceRange(true);
   graph.SetYAxisNumberOfMinorTics(5);
   graph.SetYAxisNumberOfMajorTics(21);

   IndexType series1 = graph.CreateDataSeries(_T(""),PS_SOLID,1,BLUE);
   IndexType series2 = graph.CreateDataSeries(_T(""),PS_SOLID,1,GREEN);

   CollectionIndexType nPoints;
   unfactored->get_Count(&nPoints);
   for ( CollectionIndexType i = 0; i < nPoints; i++ )
   {
      CComPtr<IPoint2d> pn;
      unfactored->get_Item(i,&pn);

      Float64 Mn,Pn;
      pn->get_X(&Mn);
      pn->get_Y(&Pn);

      Mn = WBFL::Units::ConvertFromSysUnits(Mn,pDispUnits->Moment.UnitOfMeasure);
      Pn = WBFL::Units::ConvertFromSysUnits(Pn,pDispUnits->GeneralForce.UnitOfMeasure);

      CComPtr<IPoint2d> pr;
      factored->get_Item(i,&pr);

      Float64 Mr,Pr;
      pr->get_X(&Mr);
      pr->get_Y(&Pr);

      Mr = WBFL::Units::ConvertFromSysUnits(Mr,pDispUnits->Moment.UnitOfMeasure);
      Pr = WBFL::Units::ConvertFromSysUnits(Pr,pDispUnits->GeneralForce.UnitOfMeasure);

      graph.AddPoint(series1, WBFL::Graphing::Point(Mn,-Pn));
      graph.AddPoint(series2, WBFL::Graphing::Point(Mr,-Pr));
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
   if ( ::GetTempFileName( temp_path, _T("ultcol_"), 0, temp_file ) == 0 )
   {
      // We could not get a temp name, so just use this default
      // (Use a tmp extension so it is in the same format as the one
      //  the OS would have created for us)
      _tcscpy_s( temp_file, _MAX_PATH, _T("ultcol.tmp") );
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
   CUltColChapterBuilder* pMe = const_cast<CUltColChapterBuilder*>(this);
   pMe->m_TemporaryFiles.push_back(strFilename);

   image.Save(strFilename.c_str(),Gdiplus::ImageFormatPNG);

   rptRcImage* pImage = new rptRcImage(strFilename.c_str(),rptRcImage::Baseline,rptRcImage::Right);
   return pImage;
}
