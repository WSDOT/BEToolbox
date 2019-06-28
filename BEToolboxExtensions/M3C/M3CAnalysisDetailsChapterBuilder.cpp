///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2019  Washington State Department of Transportation
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
#include "M3CAnalysisDetailsChapterBuilder.h"
#include "M3CAnalysisDetailsReportSpecification.h"
#include "BEToolboxColors.h"
#include <Reporter\Reporter.h>
#include <GraphicsLib\GraphicsLib.h>

#include <algorithm>
#include <cctype>

static const COLORREF BACKGROUND_COLOR = WHITE;
static const COLORREF VOID_COLOR = WHITE;
static const COLORREF COMPRESSION_COLOR = RED;
static const COLORREF TENSION_COLOR = BLUE;
static const COLORREF SEGMENT_FILL_COLOR = GREY70;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////
CM3CAnalysisDetailsChapterBuilder::CM3CAnalysisDetailsChapterBuilder(CM3CDoc* pDoc)
{
   m_pDoc = pDoc;
}

CM3CAnalysisDetailsChapterBuilder::~CM3CAnalysisDetailsChapterBuilder(void)
{
   for (const auto& file : m_TemporaryFiles)
   {
      ::DeleteFile( file.c_str() );
   }
}

LPCTSTR CM3CAnalysisDetailsChapterBuilder::GetName() const
{
   return _T("M3C Analysis Details");
}

Uint16 CM3CAnalysisDetailsChapterBuilder::GetMaxLevel() const
{
   return 1;
}

rptChapter* CM3CAnalysisDetailsChapterBuilder::Build(CReportSpecification* pRptSpec, Uint16 level) const
{
   CM3CAnalysisDetailsReportSpecification* pMySpec = dynamic_cast<CM3CAnalysisDetailsReportSpecification*>(pRptSpec);
   IndexType idx = pMySpec->GetResultsIndex();
   rptChapter* pChapter = new rptChapter;
   BuildAnalysisModel(pChapter, idx);
   return pChapter;
}

void CM3CAnalysisDetailsChapterBuilder::BuildAnalysisModel(rptChapter* pChapter, IndexType idx) const
{
   if (idx == INVALID_INDEX)
   {
      rptParagraph* pPara = new rptParagraph(rptStyleManager::GetHeadingStyle());
      (*pChapter) << pPara;
      (*pPara) << _T("Moment curvature could not be computed") << rptNewLine;
      return;
   }

   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDisplayUnits = pApp->GetDisplayUnits();

   CComPtr<IMomentCurvatureSolution> curvature_solution;
   m_pDoc->GetMomentCurvature(&curvature_solution);

   CComPtr<IMomentCapacitySolution> capacity_solution;
   curvature_solution->get_CapacitySolution(idx, &capacity_solution);

   CComPtr<IGeneralSectionSolution> general_solution;
   capacity_solution->get_GeneralSectionSolution(&general_solution);

   Float64 k;
   curvature_solution->get_Curvature(idx, &k);
   INIT_UV_PROTOTYPE(rptPerLengthUnitValue, curvature, pDisplayUnits->Curvature, true);

   rptParagraph* pPara = new rptParagraph(rptStyleManager::GetHeadingStyle());
   (*pChapter) << pPara;
   (*pPara) << _T("Curvature = ") << curvature.SetValue(k) << rptNewLine;

   pPara = new rptParagraph;
   (*pChapter) << pPara;

   rptRcTable* pLayoutTable = rptStyleManager::CreateLayoutTable(2);
   *pPara << pLayoutTable << rptNewLine;

   INIT_UV_PROTOTYPE(rptAreaUnitValue, area, pDisplayUnits->Area, false);
   INIT_UV_PROTOTYPE(rptLengthUnitValue, cg, pDisplayUnits->ComponentDim, false);
   INIT_UV_PROTOTYPE(rptStressUnitValue, stress, pDisplayUnits->Stress, false);
   INIT_UV_PROTOTYPE(rptMomentUnitValue, moment, pDisplayUnits->SmallMoment, true);
   INIT_UV_PROTOTYPE(rptForceUnitValue, force, pDisplayUnits->GeneralForce, true);
   INIT_UV_PROTOTYPE(rptLengthUnitValue, dist, pDisplayUnits->ComponentDim, true);


   rptRcImage* pImage = CreateImage(capacity_solution);
   (*pLayoutTable)(0,1) << pImage << rptNewLine << rptNewLine;

   pPara = &((*pLayoutTable)(0, 0));

   rptRcTable* pTable = rptStyleManager::CreateDefaultTable(7, _T(""));
   (*pPara) << pTable << rptNewLine;

   ColumnIndexType col = 0;
   (*pTable)(0, col++) << _T("Piece");
   (*pTable)(0, col++) << COLHDR(_T("Area"), rptAreaUnitTag, pDisplayUnits->Area);
   (*pTable)(0, col++) << COLHDR(Sub2(_T("Y"), _T("cg")), rptLengthUnitTag, pDisplayUnits->ComponentDim);
   (*pTable)(0, col++) << _T("Strain");
   (*pTable)(0, col++) << COLHDR(_T("Stress"), rptStressUnitTag, pDisplayUnits->Stress);
   (*pTable)(0, col++) << COLHDR(symbol(delta) << _T("F =") << rptNewLine << _T("(Area)(Stress)"), rptForceUnitTag, pDisplayUnits->GeneralForce);
   (*pTable)(0, col++) << COLHDR(symbol(delta) << _T("M =") << rptNewLine << _T("(") << symbol(delta) << _T("F") << _T(")(") << Sub2(_T("Y"), _T("cg")) << _T(")"), rptMomentUnitTag, pDisplayUnits->SmallMoment);
   force.ShowUnitTag(false);
   moment.ShowUnitTag(false);


   RowIndexType row = pTable->GetNumberOfHeaderRows();
   Float64 sum_force = 0;
   Float64 sum_moment = 0;
   CollectionIndexType nSlices;
   general_solution->get_SliceCount(&nSlices);
   for (CollectionIndexType sliceIdx = 0; sliceIdx < nSlices; sliceIdx++)
   {
      CComPtr<IGeneralSectionSlice> slice;
      general_solution->get_Slice(sliceIdx, &slice);

      Float64 slice_area;
      slice->get_Area(&slice_area);

      CComPtr<IPoint2d> pntCG;
      slice->get_CG(&pntCG);

      Float64 cgY;
      pntCG->get_Y(&cgY);

      Float64 strain;
      slice->get_Strain(&strain);

      Float64 fgStress, bgStress, netStress;
      slice->get_ForegroundStress(&fgStress);
      slice->get_BackgroundStress(&bgStress);

      netStress = fgStress - bgStress;

      Float64 F = slice_area * netStress;
      Float64 M = F*cgY;

      col = 0;
      (*pTable)(row, col++) << row;
      (*pTable)(row, col++) << area.SetValue(slice_area);
      (*pTable)(row, col++) << cg.SetValue(cgY);
      (*pTable)(row, col++) << strain;
      (*pTable)(row, col++) << stress.SetValue(netStress);
      (*pTable)(row, col++) << force.SetValue(F);
      (*pTable)(row, col++) << moment.SetValue(M);

      sum_force += F;
      sum_moment += M;

      row++;
   }

   force.ShowUnitTag(true);
   moment.ShowUnitTag(true);

   (*pPara) << _T("Resultant Force  = ") << symbol(SUM) << _T("(") << symbol(delta) << _T("F) = ") << force.SetValue(sum_force) << rptNewLine;
   (*pPara) << _T("Resultant Moment = ") << symbol(SUM) << _T("(-") << symbol(delta) << _T("M) = ") << moment.SetValue(-sum_moment) << rptNewLine;
   //(*pPara) << _T("Depth to neutral axis, c = ") << dist.SetValue(pmcd->c) << rptNewLine;
   //(*pPara) << _T("Compression Resultant, C = ") << force.SetValue(pmcd->C) << rptNewLine;
   //(*pPara) << _T("Depth to Compression Resultant, ") << Sub2(_T("d"), _T("c")) << _T(" = ") << dist.SetValue(pmcd->dc) << rptNewLine;
   //(*pPara) << _T("Tension Resultant, T = ") << force.SetValue(pmcd->T) << rptNewLine;
   //(*pPara) << _T("Depth to Tension Resultant, ") << Sub2(_T("d"), _T("e")) << _T(" = ") << dist.SetValue(pmcd->de) << rptNewLine;
}

CChapterBuilder* CM3CAnalysisDetailsChapterBuilder::Clone() const
{
   return new CM3CAnalysisDetailsChapterBuilder(m_pDoc);
}

rptRcImage* CM3CAnalysisDetailsChapterBuilder::CreateImage(IMomentCapacitySolution* pSolution) const
{
   CImage image;
   DrawSection(image, pSolution);

   // get a temporary file name for the image
   TCHAR temp_path[_MAX_PATH];
   TCHAR temp_file[_MAX_PATH];
   bool should_delete = true;

   if (::GetTempPath(_MAX_PATH, temp_path) == 0)
      _tcscpy_s(temp_path, _MAX_PATH, _T("\\")); // Couldn't establish a temp path, just use the root drive.

                                                 //
                                                 // Make sure the temp path actually exists
                                                 // We do this by looking for any file in the directory.  If nothing shows up, then the
                                                 // path doesn't exist. (Well, this isn't exactly true, but its the best I can come up
                                                 // with).
   CFileFind finder;
   BOOL bExist;
   CString path(temp_path);
   if (path[path.GetLength() - 1] != '\\')
   {
      path += _T("\\");
   }

   path += _T("*.*");
   bExist = finder.FindFile(path);
   if (!bExist)
   {
      _tcscpy_s(temp_path, _MAX_PATH, _T("\\"));
   }

   // This creates a file called "temp_file".TMP
   if (::GetTempFileName(temp_path, _T("m3c_section_"), 0, temp_file) == 0)
   {
      // We could not get a temp name, so just use this default
      // (Use a tmp extension so it is in the same format as the one
      //  the OS would have created for us)
      _tcscpy_s(temp_file, _MAX_PATH, _T("m3c_section.tmp"));
      should_delete = false;
   }

   // Replace the TMP extension with png
   std::_tstring strFilename;
   strFilename.assign(temp_file);
   strFilename.replace(strFilename.end() - 3, strFilename.end(), _T("png"));

   // We don't want the file Windows created for us
   if (should_delete)
      ::DeleteFile(temp_file);

   std::transform(std::begin(strFilename),std::end(strFilename),std::begin(strFilename),(int(*)(int))std::toupper);

   // this is a const function so we have to cast away const-ness to save
   // the file name
   CM3CAnalysisDetailsChapterBuilder* pMe = const_cast<CM3CAnalysisDetailsChapterBuilder*>(this);
   pMe->m_TemporaryFiles.push_back(strFilename);

   image.Save(strFilename.c_str(), Gdiplus::ImageFormatPNG);

   rptRcImage* pImage = new rptRcImage(strFilename.c_str(), rptRcImage::Baseline);
   return pImage;

}

void CM3CAnalysisDetailsChapterBuilder::DrawSection(CImage& image, IMomentCapacitySolution* pSolution) const
{
   CComPtr<IGeneralSectionSolution> general_solution;
   pSolution->get_GeneralSectionSolution(&general_solution);
   CollectionIndexType nSlices;
   general_solution->get_SliceCount(&nSlices);

   // determine the bounding box
   CComPtr<IRect2d> bbox;
   for (CollectionIndexType sliceIdx = 0; sliceIdx < nSlices; sliceIdx++)
   {
      CComPtr<IGeneralSectionSlice> slice;
      general_solution->get_Slice(sliceIdx, &slice);

      CComPtr<IShape> shape;
      slice->get_Shape(&shape);

      if (sliceIdx == 0)
      {
         shape->get_BoundingBox(&bbox);
      }
      else
      {
         CComPtr<IRect2d> box;
         shape->get_BoundingBox(&box);
         bbox->Union(box);
      }
   }

   // select image size
   Float64 wx, wy;
   bbox->get_Width(&wx);
   bbox->get_Height(&wy);
   Float64 aspect_ratio = wx / wy;

   int base_dimension = 400;
   Float64 width_scale = 1.5;
   if (aspect_ratio < 1)
   {
      // section is taller than it is wide
      image.Create(int(width_scale*aspect_ratio*base_dimension) + 15, base_dimension + 15, 32);
   }
   else
   {
      // section is wider than it is tall
      image.Create(int(width_scale*base_dimension) + 15, int(base_dimension / aspect_ratio) + 15, 32);
   }


   // set up coordinate mapping
   grlibPointMapper mapper;
   mapper.SetMappingMode(grlibPointMapper::Isotropic);
   mapper.SetWorldExt(width_scale*wx, wy);

   Float64 orgY;
   bbox->get_Top(&orgY);

   mapper.SetWorldOrg(0, orgY);

   mapper.SetDeviceExt(image.GetWidth(), image.GetHeight());
   mapper.SetDeviceOrg(int(image.GetWidth() / (width_scale * 2)) + 2, 2);

   // Fill image background (otherwise it will be black)
   CDC* pDC = CDC::FromHandle(image.GetDC());
   CRect rect(CPoint(0, 0), CSize(image.GetWidth(), image.GetHeight()));
   CBrush bgBrush(BACKGROUND_COLOR);
   CPen bgPen(PS_SOLID, 1, BACKGROUND_COLOR);
   CBrush* pOldBrush = pDC->SelectObject(&bgBrush);
   CPen* pOldPen = pDC->SelectObject(&bgPen);
   pDC->Rectangle(rect);

   // draw each slice
   CPen columnPen(PS_SOLID, 1, WHITESMOKE);
   CBrush columnBrush(SEGMENT_FILL_COLOR);
   CBrush voidBrush(VOID_COLOR);
   CBrush tensionBrush(TENSION_COLOR);
   CBrush compressionBrush(COMPRESSION_COLOR);

   // we don't want the larger pieces to cover over the smaller pieces
   // (e.g... we don't want column sections to cover over the bars and strands)
   // We will fill up a container with references to the pieces and their type (tension, compression, etc)
   // then sort by area so the bigger pieces are drawn first
   const int Neutral = 0;
   const int Void = 1;
   const int Tension = 2;
   const int Compression = 3;

   std::vector<std::pair<CComPtr<IGeneralSectionSlice>, int>> vPieces;
   vPieces.reserve(nSlices);

   for (CollectionIndexType sliceIdx = 0; sliceIdx < nSlices; sliceIdx++)
   {
      CComPtr<IGeneralSectionSlice> slice;
      general_solution->get_Slice(sliceIdx, &slice);

      Float64 fgStress, bgStress;
      slice->get_ForegroundStress(&fgStress);
      slice->get_BackgroundStress(&bgStress);

      Float64 stress = fgStress - bgStress;
      stress = IsZero(stress) ? 0 : stress;

      CComPtr<IStressStrain> fgMaterial, bgMaterial;
      slice->get_ForegroundMaterial(&fgMaterial);
      slice->get_BackgroundMaterial(&bgMaterial);

      if (fgMaterial == nullptr && bgMaterial != nullptr)
      {
         // this is a void
         // save the slice index and go to the next slice
         vPieces.push_back(std::make_pair(slice, Void));
         continue;
      }

      if (stress < 0)
      {
         vPieces.push_back(std::make_pair(slice, Compression));
      }
      else if (0 < stress)
      {
         vPieces.push_back(std::make_pair(slice, Tension));
      }
      else
      {
         vPieces.push_back(std::make_pair(slice, Neutral));
      }
   }

   auto sort_by_area = [](auto& pieceA, auto& pieceB)
   {
      Float64 areaA, areaB;
      pieceA.first->get_Area(&areaA);
      pieceB.first->get_Area(&areaB);
      return areaB < areaA;
   };

   std::sort(std::begin(vPieces), std::end(vPieces), sort_by_area);

   pDC->SelectObject(&columnPen);
   for (auto& pieceInfo : vPieces)
   {
      switch (pieceInfo.second)
      {
      case Neutral:
         pDC->SelectObject(&columnBrush);
         break;

      case Void:
         pDC->SelectObject(&voidBrush);
         break;

      case Compression:
         pDC->SelectObject(&compressionBrush);
         break;

      case Tension:
         pDC->SelectObject(&tensionBrush);
         break;
      }

      CComPtr<IGeneralSectionSlice> slice = pieceInfo.first;
      CComPtr<IShape> shape;
      slice->get_Shape(&shape);

      DrawSlice(shape, pDC, mapper);
   }

   // draw the strain profile
   CPen pen(PS_SOLID, 1, BLACK);
   pDC->SelectObject(&pen);

   Float64 top, bottom, left, right;
   bbox->get_Top(&top);
   bbox->get_Bottom(&bottom);
   bbox->get_Left(&left);
   bbox->get_Right(&right);

   CPoint p;

   CComPtr<IPlane3d> strain_plane;
   pSolution->get_StrainPlane(&strain_plane);

   Float64 eTop, eBottom; // strain top and bottom
   strain_plane->GetZ(0, top, &eTop);
   strain_plane->GetZ(0, bottom, &eBottom);

   // scale strains so that they plot with the same
   // aspect ratio as the section
   Float64 strain = Max(fabs(eBottom), fabs(eTop));
   Float64 scale = (wx / 4) / strain;
   eTop *= scale;
   eBottom *= scale;
   strain *= scale;

   mapper.SetDeviceOrg(3 * image.GetWidth() / 4 + 2, 2);

   // negate the mirror factor so the strain plane draws correctly
   mapper.GetWorldExt(&wx, &wy);
   mapper.SetWorldExt(wx, wy);

   mapper.WPtoDP(0, top, &p.x, &p.y);
   pDC->MoveTo(p);
   mapper.WPtoDP(eTop, top, &p.x, &p.y);
   pDC->LineTo(p);
   mapper.WPtoDP(eBottom, bottom, &p.x, &p.y);
   pDC->LineTo(p);
   mapper.WPtoDP(0, bottom, &p.x, &p.y);
   pDC->LineTo(p);
   mapper.WPtoDP(0, top, &p.x, &p.y);
   pDC->LineTo(p);

   // Draw the compression resultant
   CPen cPen(PS_SOLID, 5, COMPRESSION_COLOR);
   pDC->SelectObject(&cPen);

   CComPtr<IPoint2d> pntC;
   pSolution->get_CompressionResultantLocation(&pntC);
   Float64 y;
   pntC->get_Y(&y);

   mapper.WPtoDP(strain, y, &p.x, &p.y);
   pDC->MoveTo(p);
   mapper.WPtoDP(0, y, &p.x, &p.y);
   pDC->LineTo(p);

   // arrow head
   pDC->MoveTo(p.x + 5, p.y - 5);
   pDC->LineTo(p);
   pDC->MoveTo(p.x + 5, p.y + 5);
   pDC->LineTo(p);

   // Draw the tension resultant
   CPen tPen(PS_SOLID, 5, TENSION_COLOR);
   pDC->SelectObject(&tPen);

   CComPtr<IPoint2d> pntT;
   pSolution->get_TensionResultantLocation(&pntT);
   pntT->get_Y(&y);

   mapper.WPtoDP(0, y, &p.x, &p.y);
   pDC->MoveTo(p);
   mapper.WPtoDP(strain, y, &p.x, &p.y);
   pDC->LineTo(p);


   // arrow head
   pDC->MoveTo(p.x - 5, p.y - 5);
   pDC->LineTo(p);
   pDC->MoveTo(p.x - 5, p.y + 5);
   pDC->LineTo(p);

   pDC->SelectObject(pOldBrush);
   pDC->SelectObject(pOldPen);

   image.ReleaseDC();
}

void CM3CAnalysisDetailsChapterBuilder::DrawSlice(IShape* pShape, CDC* pDC, grlibPointMapper& mapper) const
{
   CComPtr<IPoint2dCollection> objPoints;
   pShape->get_PolyPoints(&objPoints);

   CollectionIndexType nPoints;
   objPoints->get_Count(&nPoints);

   if (nPoints < 3)
   {
      CComPtr<IPoint2d> pnt;
      objPoints->get_Item(0, &pnt);

      Float64 x, y;
      pnt->Location(&x, &y);

      LONG dx, dy;
      mapper.WPtoDP(x, y, &dx, &dy);

      CRect box(CPoint(dx, dy), CSize(0, 0));
      box.top -= 2;
      box.bottom += 2;
      box.left -= 2;
      box.right += 2;

      pDC->Rectangle(box);
   }
   else
   {
      CPoint* points = new CPoint[nPoints];
      for (CollectionIndexType pntIdx = 0; pntIdx < nPoints; pntIdx++)
      {
         CComPtr<IPoint2d> point;
         objPoints->get_Item(pntIdx, &point);
         Float64 x, y;
         point->get_X(&x);
         point->get_Y(&y);

         LONG dx, dy;
         mapper.WPtoDP(x, y, &dx, &dy);

         points[pntIdx] = CPoint(dx, dy);
      }

      pDC->Polygon(points, (int)nPoints);

      delete[] points;
   }
}