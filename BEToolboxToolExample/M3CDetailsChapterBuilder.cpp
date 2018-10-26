///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2018  Washington State Department of Transportation
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
#include "M3CDetailsChapterBuilder.h"
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
CM3CDetailsChapterBuilder::CM3CDetailsChapterBuilder(CM3CDoc* pDoc)
{
   m_pDoc = pDoc;
}

CM3CDetailsChapterBuilder::~CM3CDetailsChapterBuilder(void)
{
   for (const auto& file : m_TemporaryFiles)
   {
      ::DeleteFile( file.c_str() );
   }
}

LPCTSTR CM3CDetailsChapterBuilder::GetName() const
{
   return _T("M3C Details");
}

Uint16 CM3CDetailsChapterBuilder::GetMaxLevel() const
{
   return 1;
}

rptChapter* CM3CDetailsChapterBuilder::Build(CReportSpecification* pRptSpec, Uint16 level) const
{
   rptChapter* pChapter = new rptChapter;
   BuildConcreteModel(pChapter);
   BuildRebarModel(pChapter);
   BuildAnalysisModel(pChapter, 50);
   return pChapter;
}

void CM3CDetailsChapterBuilder::BuildConcreteModel(rptChapter* pChapter) const
{
   rptParagraph* pPara = new rptParagraph;
   (*pChapter) << pPara;

   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDisplayUnits = pApp->GetDisplayUnits();

   rptRcTable* pLayoutTable = rptStyleManager::CreateLayoutTable(2);
   (*pPara) << pLayoutTable;

   INIT_UV_PROTOTYPE(rptStressUnitValue, stress, pDisplayUnits->Stress, true);
   INIT_UV_PROTOTYPE(rptLengthUnitValue, dim, pDisplayUnits->ComponentDim, true);
   INIT_UV_PROTOTYPE(rptAreaUnitValue, area, pDisplayUnits->Area, true);

   CComPtr<IStressStrain> ssConcrete;
   m_pDoc->BuildConcreteModel(&ssConcrete);

   CComQIPtr<IManderModel> mander_model(ssConcrete);
   Float64 fco, eco, R;
   mander_model->get_fco(&fco);
   mander_model->get_eco(&eco);
   mander_model->get_R(&R);

   (*pLayoutTable)(0, 0) << _T("f'co") << _T(" = ") << stress.SetValue(fco) << rptNewLine;
   (*pLayoutTable)(0, 0) << _T("eco") << _T(" = ") << eco << rptNewLine;
   (*pLayoutTable)(0, 0) << _T("R") << _T(" = ") << R << rptNewLine;

   CComPtr<IManderModelSection> mander_model_section;
   mander_model->get_Section(&mander_model_section);

   CComQIPtr<ICircularManderSection> circular_model(mander_model_section);
   Float64 Asp, As, db, S, D, cover;
   circular_model->get_Asp(&Asp);
   circular_model->get_As(&As);
   circular_model->get_db(&db);
   circular_model->get_S(&S);
   circular_model->get_Diameter(&D);
   circular_model->get_Cover(&cover);

   (*pLayoutTable)(0, 0) << _T("Column Diameter (D) ") << _T(" = ") << dim.SetValue(D) << rptNewLine;
   (*pLayoutTable)(0, 0) << _T("Cover ") << _T(" = ") << dim.SetValue(cover) << rptNewLine;
   (*pLayoutTable)(0, 0) << _T("As") << _T(" = ") << area.SetValue(As) << rptNewLine;
   (*pLayoutTable)(0, 0) << _T("Asp") << _T(" = ") << area.SetValue(Asp) << rptNewLine;
   (*pLayoutTable)(0, 0) << _T("db") << _T(" = ") << dim.SetValue(db) << rptNewLine;
   (*pLayoutTable)(0, 0) << _T("S") << _T(" = ") << dim.SetValue(S) << rptNewLine;


   (*pLayoutTable)(0, 0) << rptNewLine;

   Float64 fyh, ps, ke;
   mander_model_section->get_TransvYieldStrength(&fyh);
   mander_model_section->get_TransvReinforcementRatio(&ps);
   mander_model_section->get_ConfinementEffectivenessCoefficient(&ke);

   (*pLayoutTable)(0, 0) << _T("fyh") << _T(" = ") << stress.SetValue(fyh) << rptNewLine;
   (*pLayoutTable)(0, 0) << _T("ds = D - 2*Cover - db") << rptNewLine;
   (*pLayoutTable)(0, 0) << _T("ps = 4*Asp/(ds*S)") << _T(" = ") << ps << rptNewLine;
   (*pLayoutTable)(0, 0) << _T("s = S - db") << rptNewLine;
   (*pLayoutTable)(0, 0) << _T("Ac = PI*ds^2/4") << rptNewLine;
   (*pLayoutTable)(0, 0) << _T("pcc = As/Ac") << rptNewLine;
   (*pLayoutTable)(0, 0) << _T("ke = (1 - s/(2*ds))/(1-pcc)") << _T(" = ") << ke << rptNewLine;


   (*pLayoutTable)(0, 0) << rptNewLine;

   Float64 fr, fcc, ecc;
   mander_model->GetConcreteParameters(&fr, &fcc, &ecc);
   (*pLayoutTable)(0, 0) << _T("fr = 0.5*ke*ps*fyh") << _T(" = ") << stress.SetValue(fr) << rptNewLine;
   (*pLayoutTable)(0, 0) << _T("fcc = fco(-1.254 + 2.254sqrt(1+7.94(fr/fco)) - 2(fr/fco))") << _T(" = ") << stress.SetValue(fcc) << rptNewLine;
   (*pLayoutTable)(0, 0) << _T("ecc = eco(1 + R(fcc/fco - 1))") << _T(" = ") << ecc << rptNewLine;

   Float64 Ec;
   ssConcrete->get_ModulusOfElasticity(&Ec);
   (*pLayoutTable)(0, 0) << _T("Ec = 60000sqrt(fco) PSI") << _T(" = ") << stress.SetValue(Ec) << rptNewLine;
   (*pLayoutTable)(0, 0) << _T("Esec = fcc/ecc") << rptNewLine;
   (*pLayoutTable)(0, 0) << _T("r = Ec/(Ec - Ecc)") << rptNewLine;
   (*pLayoutTable)(0, 0) << _T("x = e/ecc") << rptNewLine;
   (*pLayoutTable)(0, 0) << _T("fc = fcc*x*r/(r-1-x^r)") << rptNewLine;

   // Build e-f graph
   Float64 minStrain, maxStrain;
   ssConcrete->StrainLimits(&minStrain, &maxStrain);

   int nSteps = 50;
   Float64 step = (fabs(minStrain) - minStrain) / (nSteps-1);


   rptRcTable* pTable = rptStyleManager::CreateDefaultTable(2);
   (*pLayoutTable)(0, 0) << pTable;
   (*pTable)(0, 0) << symbol(epsilon);
   (*pTable)(0, 1) << COLHDR(Sub2(_T("f'"),_T("c")), rptStressUnitTag, pDisplayUnits->Stress);

   stress.ShowUnitTag(false);
   RowIndexType row = pTable->GetNumberOfHeaderRows();
   for (int i = 0; i < nSteps; i++)
   {
      Float64 strain = minStrain + i*step;
      Float64 f;
      ssConcrete->ComputeStress(strain, &f);

      ColumnIndexType col = 0;
      (*pTable)(row, col++) << -strain;
      (*pTable)(row, col++) << stress.SetValue(-f);

      row++;
   }
}

void CM3CDetailsChapterBuilder::BuildRebarModel(rptChapter* pChapter) const
{
   rptParagraph* pPara = new rptParagraph;
   (*pChapter) << pPara;

   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDisplayUnits = pApp->GetDisplayUnits();

   rptRcTable* pLayoutTable = rptStyleManager::CreateLayoutTable(2);
   (*pPara) << pLayoutTable;

   INIT_UV_PROTOTYPE(rptStressUnitValue, stress, pDisplayUnits->Stress, true);
   INIT_UV_PROTOTYPE(rptLengthUnitValue, dim, pDisplayUnits->ComponentDim, true);
   INIT_UV_PROTOTYPE(rptAreaUnitValue, area, pDisplayUnits->Area, true);

   CComPtr<IStressStrain> ssRebar;
   m_pDoc->BuildRebarModel(&ssRebar);

   CComQIPtr<IStrainHardenedRebarModel> shRebar(ssRebar);

   Float64 fy, fu, Es, esh, efr;
   shRebar->get_fy(&fy);
   shRebar->get_fu(&fu);
   shRebar->get_Es(&Es);
   shRebar->get_esh(&esh);
   shRebar->get_efr(&efr);

   (*pLayoutTable)(0, 0) << _T("fy") << _T(" = ") << stress.SetValue(fy) << rptNewLine;
   (*pLayoutTable)(0, 0) << _T("fu") << _T(" = ") << stress.SetValue(fu) << rptNewLine;
   (*pLayoutTable)(0, 0) << _T("Es") << _T(" = ") << stress.SetValue(Es) << rptNewLine;
   (*pLayoutTable)(0, 0) << _T("esh") << _T(" = ") << esh << rptNewLine;
   (*pLayoutTable)(0, 0) << _T("efr") << _T(" = ") << efr << rptNewLine;

   (*pLayoutTable)(0, 0) << rptNewLine;

   (*pLayoutTable)(0, 0) << _T("fs = Es*e for 0 <= e <= fy/Es") << rptNewLine;
   (*pLayoutTable)(0, 0) << _T("fs = fy for fy/Es <= e <= esh") << rptNewLine;
   (*pLayoutTable)(0, 0) << _T("fs = fu - (fu - fy)(efr-e)^2/(esh-efr)^2 for esh <= e <= efr") << rptNewLine;

   (*pLayoutTable)(0, 0) << rptNewLine;

   // Build e-f graph
   Float64 minStrain, maxStrain;
   ssRebar->StrainLimits(&minStrain, &maxStrain);

   int nSteps = 50;
   Float64 step = (maxStrain - minStrain) / (nSteps - 1);

   rptRcTable* pTable = rptStyleManager::CreateDefaultTable(2);
   (*pLayoutTable)(0, 0) << pTable;
   (*pTable)(0, 0) << symbol(epsilon);
   (*pTable)(0, 1) << COLHDR(_T("fs"), rptStressUnitTag, pDisplayUnits->Stress);

   stress.ShowUnitTag(false);
   RowIndexType row = pTable->GetNumberOfHeaderRows();
   for (int i = 0; i < nSteps; i++)
   {
      Float64 strain = minStrain + i*step;
      Float64 f;
      ssRebar->ComputeStress(strain, &f);

      ColumnIndexType col = 0;
      (*pTable)(row, col++) << strain;
      (*pTable)(row, col++) << stress.SetValue(f);

      row++;
   }
}

void CM3CDetailsChapterBuilder::BuildAnalysisModel(rptChapter* pChapter, IndexType idx) const
{
   rptParagraph* pPara = new rptParagraph;
   (*pChapter) << pPara;

   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDisplayUnits = pApp->GetDisplayUnits();

   CComPtr<IMomentCurvatureSolution> curvature_solution;
   m_pDoc->GetMomentCurvature(&curvature_solution);

   CComPtr<IMomentCapacitySolution> capacity_solution;
   curvature_solution->get_CapacitySolution(idx, &capacity_solution);

   CComPtr<IGeneralSectionSolution> general_solution;
   capacity_solution->get_GeneralSectionSolution(&general_solution);

   INIT_UV_PROTOTYPE(rptAreaUnitValue, area, pDisplayUnits->Area, false);
   INIT_UV_PROTOTYPE(rptLengthUnitValue, cg, pDisplayUnits->ComponentDim, false);
   INIT_UV_PROTOTYPE(rptStressUnitValue, stress, pDisplayUnits->Stress, false);
   INIT_UV_PROTOTYPE(rptMomentUnitValue, moment, pDisplayUnits->SmallMoment, true);
   INIT_UV_PROTOTYPE(rptForceUnitValue, force, pDisplayUnits->GeneralForce, true);
   INIT_UV_PROTOTYPE(rptLengthUnitValue, dist, pDisplayUnits->ComponentDim, true);
//   INIT_UV_PROTOTYPE(rptPointOfInterest, location, pDisplayUnits->GetSpanLengthUnit(), true);

   // Results
   //(*pPara) << _T("Depth to neutral axis, c = ") << dist.SetValue(pmcd->c) << rptNewLine;
   //(*pPara) << _T("Compression Resultant, C = ") << force.SetValue(pmcd->C) << rptNewLine;
   //(*pPara) << _T("Depth to Compression Resultant, ") << Sub2(_T("d"), _T("c")) << _T(" = ") << dist.SetValue(pmcd->dc) << rptNewLine;
   //(*pPara) << _T("Tension Resultant, T = ") << force.SetValue(pmcd->T) << rptNewLine;
   //(*pPara) << _T("Depth to Tension Resultant, ") << Sub2(_T("d"), _T("e")) << _T(" = ") << dist.SetValue(pmcd->de) << rptNewLine;
   //(*pPara) << _T("Depth to Tension Resultant (for shear), ") << Sub2(_T("d"), _T("e")) << _T(" = ") << dist.SetValue(pmcd->de_shear) << rptNewLine;
   //(*pPara) << _T("Nominal Capacity, ") << Sub2(_T("M"), _T("n")) << _T(" = ") << moment.SetValue(pmcd->Mn) << rptNewLine;
   //(*pPara) << _T("Moment Arm = ") << Sub2(_T("d"), _T("e")) << _T(" - ") << Sub2(_T("d"), _T("c")) << _T(" = ") << Sub2(_T("M"), _T("n")) << _T("/T = ") << dist.SetValue(pmcd->MomentArm) << rptNewLine;


   pPara = new rptParagraph;
   (*pChapter) << pPara;
   rptRcImage* pImage = CreateImage(capacity_solution);
   (*pPara) << pImage << rptNewLine << rptNewLine;


   rptRcTable* pTable = rptStyleManager::CreateDefaultTable(9, _T(""));
   (*pPara) << pTable << rptNewLine;

   (*pTable)(0, 0) << _T("Slice");
   (*pTable)(0, 1) << COLHDR(_T("Area"), rptAreaUnitTag, pDisplayUnits->Area);
   (*pTable)(0, 2) << COLHDR(Sub2(_T("Y"), _T("cg")), rptLengthUnitTag, pDisplayUnits->ComponentDim);
   (*pTable)(0, 3) << _T("Strain");
   (*pTable)(0, 4) << COLHDR(_T("Foreground") << rptNewLine << _T("Stress"), rptStressUnitTag, pDisplayUnits->Stress);
   (*pTable)(0, 5) << COLHDR(_T("Background") << rptNewLine << _T("Stress"), rptStressUnitTag, pDisplayUnits->Stress);
   (*pTable)(0, 6) << COLHDR(_T("Stress") << rptNewLine << _T("(Fg - Bg)"), rptStressUnitTag, pDisplayUnits->Stress);
   (*pTable)(0, 7) << COLHDR(symbol(delta) << _T("F =") << rptNewLine << _T("(Area)(Stress)"), rptForceUnitTag, pDisplayUnits->GeneralForce);
   (*pTable)(0, 8) << COLHDR(symbol(delta) << _T("M =") << rptNewLine << _T("(") << symbol(delta) << _T("F") << _T(")(") << Sub2(_T("Y"), _T("cg")) << _T(")"), rptMomentUnitTag, pDisplayUnits->SmallMoment);
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

      (*pTable)(row, 0) << row;
      (*pTable)(row, 1) << area.SetValue(slice_area);
      (*pTable)(row, 2) << cg.SetValue(cgY);
      (*pTable)(row, 3) << strain;
      (*pTable)(row, 4) << stress.SetValue(fgStress);
      (*pTable)(row, 5) << stress.SetValue(bgStress);
      (*pTable)(row, 6) << stress.SetValue(netStress);

      (*pTable)(row, 7) << force.SetValue(F);
      (*pTable)(row, 8) << moment.SetValue(M);

      sum_force += F;
      sum_moment += M;

      row++;
   }

   force.ShowUnitTag(true);
   moment.ShowUnitTag(true);

   (*pPara) << _T("Resultant Force  = ") << symbol(SUM) << _T("(") << symbol(delta) << _T("F) = ") << force.SetValue(sum_force) << rptNewLine;
   (*pPara) << _T("Resultant Moment = ") << symbol(SUM) << _T("(") << symbol(delta) << _T("M) = ") << moment.SetValue(sum_moment) << rptNewLine;
}

CChapterBuilder* CM3CDetailsChapterBuilder::Clone() const
{
   return new CM3CDetailsChapterBuilder(m_pDoc);
}

rptRcImage* CM3CDetailsChapterBuilder::CreateImage(IMomentCapacitySolution* pSolution) const
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
   CM3CDetailsChapterBuilder* pMe = const_cast<CM3CDetailsChapterBuilder*>(this);
   pMe->m_TemporaryFiles.push_back(strFilename);

   image.Save(strFilename.c_str(), Gdiplus::ImageFormatPNG);

   rptRcImage* pImage = new rptRcImage(strFilename.c_str(), rptRcImage::Baseline);
   return pImage;

}

void CM3CDetailsChapterBuilder::DrawSection(CImage& image, IMomentCapacitySolution* pSolution) const
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
   pDC->SelectObject(pOldBrush);
   pDC->SelectObject(pOldPen);

   // draw each slice
   CPen girderPen(PS_SOLID, 1, WHITESMOKE);
   CBrush girderBrush(SEGMENT_FILL_COLOR);
   CBrush voidBrush(VOID_COLOR);
   CBrush tensionBrush(TENSION_COLOR);
   CBrush compressionBrush(COMPRESSION_COLOR);

   pOldPen = pDC->SelectObject(&girderPen);

   std::vector<CollectionIndexType> voidIndices;        // contains slice index for void slices
   std::vector<CollectionIndexType> neutralIndices;     // contains slice index for neutral slices
   std::vector<CollectionIndexType> tensionIndices;     // contains slice index for tension slices
   std::vector<CollectionIndexType> compressionIndices; // contains slice index for compression slices

   for (CollectionIndexType sliceIdx = 0; sliceIdx < nSlices; sliceIdx++)
   {
      CComPtr<IGeneralSectionSlice> slice;
      general_solution->get_Slice(sliceIdx, &slice);

      Float64 fgStress, bgStress;
      slice->get_ForegroundStress(&fgStress);
      slice->get_BackgroundStress(&bgStress);

      Float64 stress = fgStress - bgStress;

      CComPtr<IStressStrain> fgMaterial, bgMaterial;
      slice->get_ForegroundMaterial(&fgMaterial);
      slice->get_BackgroundMaterial(&bgMaterial);

      if (fgMaterial == nullptr && bgMaterial != nullptr)
      {
         // this is a void
         // save the slice index and go to the next slice
         voidIndices.push_back(sliceIdx);
         continue;
      }

      if (stress < 0)
      {
         compressionIndices.push_back(sliceIdx);
      }
      else if (0 < stress)
      {
         tensionIndices.push_back(sliceIdx);
      }
      else
      {
         neutralIndices.push_back(sliceIdx);
      }
   }

   std::vector<CollectionIndexType>::iterator iter;
   // draw neutral slices first
   pOldBrush = pDC->SelectObject(&girderBrush);
   for (iter = neutralIndices.begin(); iter != neutralIndices.end(); iter++)
   {
      CollectionIndexType sliceIdx = *iter;
      CComPtr<IGeneralSectionSlice> slice;
      general_solution->get_Slice(sliceIdx, &slice);

      CComPtr<IShape> shape;
      slice->get_Shape(&shape);

      DrawSlice(shape, pDC, mapper);
   }

   // draw compression slices
   pDC->SelectObject(&compressionBrush);
   for (iter = compressionIndices.begin(); iter != compressionIndices.end(); iter++)
   {
      CollectionIndexType sliceIdx = *iter;
      CComPtr<IGeneralSectionSlice> slice;
      general_solution->get_Slice(sliceIdx, &slice);

      CComPtr<IShape> shape;
      slice->get_Shape(&shape);

      DrawSlice(shape, pDC, mapper);
   }

   // draw the voids on top of the foreground shape
   pDC->SelectObject(&voidBrush);
   for (iter = voidIndices.begin(); iter != voidIndices.end(); iter++)
   {
      CollectionIndexType sliceIdx = *iter;
      CComPtr<IGeneralSectionSlice> slice;
      general_solution->get_Slice(sliceIdx, &slice);

      CComPtr<IShape> shape;
      slice->get_Shape(&shape);

      DrawSlice(shape, pDC, mapper);
   }

   // draw tension slices
   pDC->SelectObject(&tensionBrush);
   CPen tensionPen(PS_SOLID, 1, TENSION_COLOR);
   pDC->SelectObject(&tensionPen);
   for (iter = tensionIndices.begin(); iter != tensionIndices.end(); iter++)
   {
      CollectionIndexType sliceIdx = *iter;
      CComPtr<IGeneralSectionSlice> slice;
      general_solution->get_Slice(sliceIdx, &slice);

      CComPtr<IShape> shape;
      slice->get_Shape(&shape);

      DrawSlice(shape, pDC, mapper);
   }

   pDC->SelectObject(pOldBrush);
   pDC->SelectObject(pOldPen);

   // draw the strain profile
   CPen pen(PS_SOLID, 1, BLACK);
   pDC->SelectObject(&pen);

   Float64 top, bottom, left, right;
   bbox->get_Top(&top);
   bbox->get_Bottom(&bottom);
   bbox->get_Left(&left);
   bbox->get_Right(&right);

   //// Draw Y = 0 line
   CPoint p;
   //mapper.WPtoDP(left,0,&p.x,&p.y);
   //pDC->MoveTo(p);
   //mapper.WPtoDP(right,0,&p.x,&p.y);
   //pDC->LineTo(p);

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

   pDC->SelectObject(pOldPen);

   image.ReleaseDC();
}

void CM3CDetailsChapterBuilder::DrawSlice(IShape* pShape, CDC* pDC, grlibPointMapper& mapper) const
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