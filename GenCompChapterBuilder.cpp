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
#include "GenCompChapterBuilder.h"
#include "BEToolboxUtilities.h"
#include <Reporter\Reporter.h>
#include <Colors.h>
#include <GraphicsLib\GraphicsLib.h>

class arvPhysicalConverter: public sysINumericFormatToolBase
{
public:
   virtual Float64 Convert(Float64 value) const=0;
   virtual std::_tstring UnitTag() const =0;
};

// a template class for printing out physical values
template <class T>
class PhysicalFormatTool : public arvPhysicalConverter
{
public:
   // built to take a unitmgtIndirectMeasureDataT
   PhysicalFormatTool(const T& umd) :
      // these formats are for reports, let's graphs a bit less precision
      m_FormatTool(umd.Format, umd.Width-1, umd.Precision-1),
      m_rT(umd)
      {
         CHECK(umd.Width>0);     // Make sure these are positive. Otherwise subtraction
         CHECK(umd.Precision>0); // above will cause UINT's to roll over
      }

   std::_tstring AsString(Float64 val) const
   {
      if (fabs(val) > m_rT.Tol/10.)
         return m_FormatTool.AsString(val);
      else
         return m_FormatTool.AsString(0.0);
   }

   Float64 Convert(Float64 value) const
   {
      return ::ConvertFromSysUnits(value, m_rT.UnitOfMeasure);
   }

   std::_tstring UnitTag() const
   {
      return m_rT.UnitOfMeasure.UnitTag();
   }
private:
   sysNumericFormatTool m_FormatTool;
   const T&             m_rT;
};

typedef PhysicalFormatTool<unitmgtLengthData>  LengthTool;

static const COLORREF GRAPH_BACKGROUND = ALICEBLUE;
static const COLORREF GRID_COLOR       = LIGHTSTEELBLUE;

//////////////////////////////////
CGenCompChapterBuilder::CGenCompChapterBuilder(CGenCompDoc* pDoc)
{
   m_pDoc = pDoc;
}

CGenCompChapterBuilder::~CGenCompChapterBuilder(void)
{
   std::vector<std::_tstring>::iterator iter;
   for ( iter = m_TemporaryFiles.begin(); iter != m_TemporaryFiles.end(); iter++ )
   {
      std::_tstring file = *iter;
      ::DeleteFile( file.c_str() );
   }
}

LPCTSTR CGenCompChapterBuilder::GetName() const
{
   return _T("GenComp");
}

Uint16 CGenCompChapterBuilder::GetMaxLevel() const
{
   return 1;
}

rptChapter* CGenCompChapterBuilder::Build(CReportSpecification* pRptSpec,Uint16 level) const
{
   rptChapter* pChapter = new rptChapter;
   rptParagraph* pPara;

   pPara = new rptParagraph;
   (*pChapter) << pPara;

   rptRcTable* pLayoutTable = new rptRcTable(4,0.0);
   pLayoutTable->SetStyleName(_T("NormalRow"));
   pLayoutTable->SetOutsideBorderStyle(rptRiStyle::NOBORDER);
   pLayoutTable->SetInsideBorderStyle(rptRiStyle::NOBORDER);
   pLayoutTable->SetTableHeaderStyle(_T("NormalRow"));
   pLayoutTable->SetNumberOfHeaderRows(0);
   pLayoutTable->SetCellPad(0.125);

   pLayoutTable->SetColumnStyle(0,_T("LayoutCell"));
   pLayoutTable->SetColumnStyle(1,_T("LayoutCell"));
   pLayoutTable->SetColumnStyle(2,_T("LayoutCell"));
   pLayoutTable->SetColumnStyle(3,_T("LayoutCell"));

   (*pPara) << pLayoutTable;

   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   INIT_UV_PROTOTYPE( rptLengthUnitValue,  length, pDispUnits->ComponentDim, false);

   rptRcTable* pPrimaryPointsTable = new rptRcTable(2,1.5);
   pPrimaryPointsTable->TableCaption().SetStyleName(_T("TableTitle"));
   pPrimaryPointsTable->TableCaption() << _T("Basic Section");
   pPrimaryPointsTable->SetTableHeaderStyle( _T("ColumnHeading") );
   pPrimaryPointsTable->SetOutsideBorderStyle( rptRiStyle::HAIR_THICK );
   pPrimaryPointsTable->SetInsideBorderStyle( rptRiStyle::NOBORDER );
   pPrimaryPointsTable->SetCellPad( 0.03125 );

   pPrimaryPointsTable->EnableRowStriping(true);

   ColumnIndexType numColumns = pPrimaryPointsTable->GetNumberOfColumns();

   for ( ColumnIndexType i = 0; i < numColumns; i++ )
   {
      pPrimaryPointsTable->SetColumnStyle( i, _T("NormalRow") );
      pPrimaryPointsTable->SetStripeRowColumnStyle( i, _T("StripedRow") );
   }

   (*pLayoutTable)(0,0) << pPrimaryPointsTable;
   (*pPrimaryPointsTable)(0,0) << COLHDR(_T("X"),rptLengthUnitTag, pDispUnits->ComponentDim);
   (*pPrimaryPointsTable)(0,1) << COLHDR(_T("Y"),rptLengthUnitTag, pDispUnits->ComponentDim);


   rptRcTable* pSecondaryPointsTable = new rptRcTable(2,1.5);
   pSecondaryPointsTable->TableCaption().SetStyleName(_T("TableTitle"));
   pSecondaryPointsTable->TableCaption() << _T("Composite Piece");
   pSecondaryPointsTable->SetTableHeaderStyle( _T("ColumnHeading") );
   pSecondaryPointsTable->SetOutsideBorderStyle( rptRiStyle::HAIR_THICK );
   pSecondaryPointsTable->SetInsideBorderStyle( rptRiStyle::NOBORDER );
   pSecondaryPointsTable->SetCellPad( 0.03125 );

   pSecondaryPointsTable->EnableRowStriping(true);

   numColumns = pSecondaryPointsTable->GetNumberOfColumns();

   for ( ColumnIndexType i = 0; i < numColumns; i++ )
   {
      pSecondaryPointsTable->SetColumnStyle( i, _T("NormalRow") );
      pSecondaryPointsTable->SetStripeRowColumnStyle( i, _T("StripedRow") );
   }

   (*pLayoutTable)(0,1) << pSecondaryPointsTable;
   (*pSecondaryPointsTable)(0,0) << COLHDR(_T("X"),rptLengthUnitTag, pDispUnits->ComponentDim);
   (*pSecondaryPointsTable)(0,1) << COLHDR(_T("Y"),rptLengthUnitTag, pDispUnits->ComponentDim);

   //
   // Echo Input
   //
   RowIndexType row = pPrimaryPointsTable->GetNumberOfHeaderRows();
   const std::vector<std::pair<Float64,Float64>>& primaryPoints(m_pDoc->GetPrimaryPoints());
   std::vector<std::pair<Float64,Float64>>::const_iterator iter(primaryPoints.begin());
   std::vector<std::pair<Float64,Float64>>::const_iterator end(primaryPoints.end());
   for ( ; iter != end; iter++, row++ )
   {
      Float64 x = iter->first;
      Float64 y = iter->second;

      (*pPrimaryPointsTable)(row,0) << length.SetValue(x);
      (*pPrimaryPointsTable)(row,1) << length.SetValue(y);
   }

   row = pSecondaryPointsTable->GetNumberOfHeaderRows();
   const std::vector<std::pair<Float64,Float64>>& secondaryPoints(m_pDoc->GetSecondaryPoints());
   iter = secondaryPoints.begin();
   end  = secondaryPoints.end();
   for ( ; iter != end; iter++, row++ )
   {
      Float64 x = iter->first;
      Float64 y = iter->second;

      (*pSecondaryPointsTable)(row,0) << length.SetValue(x);
      (*pSecondaryPointsTable)(row,1) << length.SetValue(y);
   }

   // Properties
   CComPtr<IPolyShape> primaryShape(m_pDoc->GetPrimaryShape());
   CComQIPtr<IShape> shape(primaryShape);
   CComPtr<IShapeProperties> shapeProps;
   shape->get_ShapeProperties(&shapeProps);
   (*pLayoutTable)(0,2) << Bold(_T("Basic Section Properties")) << rptNewLine;
   WriteSectionProperties((*pLayoutTable)(0,2),shapeProps);

   CComPtr<ICompositeSection> compSection(m_pDoc->GetCompositeSection());
   CComQIPtr<ISection> section(compSection);
   CComQIPtr<IElasticProperties> elasticProperties;
   section->get_ElasticProperties(&elasticProperties);
   CComPtr<IShapeProperties> compositeProperties;
   elasticProperties->TransformProperties(1.0,&compositeProperties);

   (*pLayoutTable)(0,3) << Bold(_T("Composite Section Properties")) << rptNewLine;
   (*pLayoutTable)(0,3) << _T("Modular Ratio, n = ") << m_pDoc->GetModularRatio() << rptNewLine;
   WriteSectionProperties((*pLayoutTable)(0,3),compositeProperties);

   
   pPara = new rptParagraph;
   *pChapter << pPara;
   *pPara << rptNewPage;
   *pPara << CreateImage() << rptNewLine;


   return pChapter;
}

void CGenCompChapterBuilder::WriteSectionProperties(rptParagraph& para,IShapeProperties* pShapeProp) const
{
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   INIT_UV_PROTOTYPE( rptLengthUnitValue,  length, pDispUnits->ComponentDim, true);
   INIT_UV_PROTOTYPE( rptLength2UnitValue,  area, pDispUnits->Area, true);
   INIT_UV_PROTOTYPE( rptLength4UnitValue,  momentOfInertia, pDispUnits->MomentOfInertia, true);

   Float64 Area,I,yb;
   pShapeProp->get_Area(&Area);
   pShapeProp->get_Ixx(&I);
   pShapeProp->get_Ybottom(&yb);

   para << _T("Area = ") << area.SetValue(Area) << rptNewLine;
   para << _T("Bar Y = ") << length.SetValue(yb) << rptNewLine;
   para << _T("I = ") << momentOfInertia.SetValue(I) << rptNewLine;
}

CChapterBuilder* CGenCompChapterBuilder::Clone() const
{
   return new CGenCompChapterBuilder(m_pDoc);
}

rptRcImage* CGenCompChapterBuilder::CreateImage() const
{
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   CImage image;
   image.Create(500,500,32);
   CRect rect(CPoint(0,0),CSize(image.GetWidth(),image.GetHeight()));

   CDC* pDC = CDC::FromHandle(image.GetDC());

   // fill the background of the image (otherwise it will be black)
   CBrush brush(ALICEBLUE);
   CBrush* pOldBrush = pDC->SelectObject(&brush);
   pDC->Rectangle(rect);
   pDC->SelectObject(pOldBrush);

   LengthTool lengthTool(pDispUnits->ComponentDim);
   grGraphXY graph(lengthTool,lengthTool);

   graph.SetOutputRect(rect);
   graph.SetClientAreaColor(GRAPH_BACKGROUND);
   graph.SetGridPenStyle(PS_DOT, 1, GRID_COLOR);

#if 400 <= _WBFL_VERSION
   graph.SetIsotropicAxes(true);
#endif 

   graph.SetDoDrawGrid(true);
   graph.DrawLegend(false);

   //graph.SetTitle(_T("Interaction Diagram"));

   // Setup X-axis
   CString strXAxis;
   strXAxis.Format(_T("X (%s)"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
   graph.SetXAxisTitle(strXAxis.LockBuffer());
   strXAxis.UnlockBuffer();
   graph.SetXAxisNiceRange(true);
   graph.SetXAxisNumberOfMinorTics(0);
   graph.SetXAxisNumberOfMajorTics(11);
   graph.SetXAxisLabelAngle(350); // 35 degrees

   // Setup Y-axis
   CString strYAxis;
   strYAxis.Format(_T("Y (%s)"),pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
   graph.SetYAxisTitle(strYAxis.LockBuffer());
   strYAxis.UnlockBuffer();
   graph.SetYAxisNiceRange(true);
   graph.SetYAxisNumberOfMinorTics(0);
   graph.SetYAxisNumberOfMajorTics(11);

   IndexType primarySeries = graph.CreateDataSeries(_T(""),PS_SOLID,1,BLUE);
   IndexType secondarySeries = graph.CreateDataSeries(_T(""),PS_SOLID,1,BLUE);

   const std::vector<std::pair<Float64,Float64>>& primaryPoints(m_pDoc->GetPrimaryPoints());
   std::vector<std::pair<Float64,Float64>>::const_iterator iter(primaryPoints.begin());
   std::vector<std::pair<Float64,Float64>>::const_iterator end(primaryPoints.end());
   for (; iter != end; iter++)
   {
      gpPoint2d point(::ConvertFromSysUnits(iter->first,pDispUnits->ComponentDim.UnitOfMeasure),::ConvertFromSysUnits(iter->second,pDispUnits->ComponentDim.UnitOfMeasure));
      graph.AddPoint(primarySeries,point);
   }

   if ( 0 < primaryPoints.size() )
   {
      gpPoint2d point(::ConvertFromSysUnits(primaryPoints.front().first,pDispUnits->ComponentDim.UnitOfMeasure),::ConvertFromSysUnits(primaryPoints.front().second,pDispUnits->ComponentDim.UnitOfMeasure));
      graph.AddPoint(primarySeries,point);
   }

   const std::vector<std::pair<Float64,Float64>>& secondaryPoints(m_pDoc->GetSecondaryPoints());
   iter = secondaryPoints.begin();
   end = secondaryPoints.end();
   for (; iter != end; iter++)
   {
      gpPoint2d point(::ConvertFromSysUnits(iter->first,pDispUnits->ComponentDim.UnitOfMeasure),::ConvertFromSysUnits(iter->second,pDispUnits->ComponentDim.UnitOfMeasure));
      graph.AddPoint(secondarySeries,point);
   }

   if ( 0 < secondaryPoints.size() )
   {
      gpPoint2d point(::ConvertFromSysUnits(secondaryPoints.front().first,pDispUnits->ComponentDim.UnitOfMeasure),::ConvertFromSysUnits(secondaryPoints.front().second,pDispUnits->ComponentDim.UnitOfMeasure));
      graph.AddPoint(secondarySeries,point);
   }

   // Updates the graph metrics based on "nice" axis ranges
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
      path += _T("\\");
   path += _T("*.*");
   bExist = finder.FindFile(path);
   if ( !bExist )
      _tcscpy_s( temp_path,_MAX_PATH, _T("C:\\") );

   // This creates a file called _T("temp_file").TMP
   if ( ::GetTempFileName( temp_path, _T("gencomp_"), 0, temp_file ) == 0 )
   {
      // We could not get a temp name, so just use this default
      // (Use a tmp extension so it is in the same format as the one
      //  the OS would have created for us)
      _tcscpy_s( temp_file, _MAX_PATH, _T("gencomp.tmp") );
      should_delete = false;
   }

   // Replace the TMP extension with png
   std::_tstring strFilename;
   strFilename.assign( temp_file );
   strFilename.replace( strFilename.end() - 3, strFilename.end(), _T("png") );

   // We don't want the file Windows created for us
   if ( should_delete )
      ::DeleteFile( temp_file );

   make_upper( strFilename.begin(), strFilename.end() );

   // this is a const function so we have to cast away const-ness to save
   // the file name
   m_TemporaryFiles.push_back(strFilename);

   image.Save(strFilename.c_str(),Gdiplus::ImageFormatPNG);

   rptRcImage* pImage = new rptRcImage(strFilename.c_str(),rptRcImage::Baseline,rptRcImage::None);
   return pImage;
}
