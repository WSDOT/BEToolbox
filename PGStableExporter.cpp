///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2016  Washington State Department of Transportation
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

// PGStableExporter.cpp : Implementation of CPGStableExporter
#include "stdafx.h"
#include "BEToolbox_i.h"

#include "PGStableExporter.h"

#include <EAF\EAFAutoProgress.h>
#include <EAF\EAFDocument.h>

#include <IFace\Intervals.h>
#include <IFace\Bridge.h>
#include <IFace\Project.h>
#include <IFace\GirderHandlingSpecCriteria.h>
#include <IFace\PrestressForce.h>
#include <IFace\AnalysisResults.h>
#include <IFace\DocumentType.h>
#include <IFace\Selection.h>

#include <WBFLCore.h>

#include <PgsExt\Prompts.h>
#include <PgsExt\PrecastSegmentData.h>
#include <PgsExt\GirderLabel.h>

HRESULT CPGStableExporter::FinalConstruct()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   VERIFY(m_Bitmap.LoadBitmap(IDB_PGSTABLE));
   return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// CCurvelExporter

STDMETHODIMP CPGStableExporter::Init(UINT nCmdID)
{
   return S_OK;
}

STDMETHODIMP CPGStableExporter::GetMenuText(BSTR*  bstrText)
{
   *bstrText = CComBSTR("BEToolbox:PGStable model");
   return S_OK;
}

STDMETHODIMP CPGStableExporter::GetBitmapHandle(HBITMAP* phBmp)
{
   *phBmp = m_Bitmap;
   return S_OK;
}

STDMETHODIMP CPGStableExporter::GetCommandHintText(BSTR*  bstrText)
{
   *bstrText = CComBSTR("Export BEToolbox:PGStable model\nTool tip text");
   return S_OK;   
}

STDMETHODIMP CPGStableExporter::Export(IBroker* pBroker)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   CSegmentKey segmentKey(0,0,0);
   GET_IFACE2(pBroker,ISelection,pSelection);
   CSelection selection = pSelection->GetSelection();
   if ( selection.Type == CSelection::Girder )
   {
      segmentKey.groupIndex  = selection.GroupIdx;
      segmentKey.girderIndex = selection.GirderIdx;
   }
   else if ( selection.Type == CSelection::Segment )
   {
      segmentKey.groupIndex   = selection.GroupIdx;
      segmentKey.girderIndex  = selection.GirderIdx;
      segmentKey.segmentIndex = selection.SegmentIdx;
   }

   GET_IFACE2(pBroker,IDocumentType,pDocType);
   if ( pDocType->IsPGSuperDocument())
   {
      CGirderKey girderKey(segmentKey);
      if ( pgsSelectGirder(girderKey) == IDCANCEL )
      {
         return S_FALSE;
      }

      segmentKey = CSegmentKey(girderKey,0);
   }
   else
   {
      if ( pgsSelectSegment(segmentKey) == IDCANCEL )
      {
         return S_FALSE;
      }
   }

   GET_IFACE2(pBroker,IEAFDocument,pDoc);
   CString strExtension(_T("PGStable"));
   CString strDefaultFileName;
   if ( pDocType->IsPGSuperDocument() )
   {
      strDefaultFileName.Format(_T("%s%s_Span_%d_Girder_%s.%s"),
         pDoc->GetFileRoot(), // path to file
         pDoc->GetFileTitle(), // the file name without path or extension
         LABEL_SPAN(segmentKey.groupIndex),
         LABEL_GIRDER(segmentKey.girderIndex),
         strExtension);
   }
   else
   {
      strDefaultFileName.Format(_T("%s%s_Group_%d_Girder_%s_Segment_%d.%s"),
         pDoc->GetFileRoot(), // path to file
         pDoc->GetFileTitle(), // the file name without path or extension
         LABEL_SPAN(segmentKey.groupIndex),
         LABEL_GIRDER(segmentKey.girderIndex),
         LABEL_SEGMENT(segmentKey.segmentIndex),
         strExtension);
   }

	CFileDialog fileDlg(FALSE,strExtension,strDefaultFileName,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("PGStable File (*.PGStable)|*.PGStable||"));
	if (fileDlg.DoModal() == IDOK)
	{
      CComPtr<IStructuredSave> pStrSave;

      CPGStableModel model;
      if ( !ConfigureModel(pBroker,segmentKey,model) )
      {
         return S_FALSE;
      }

      HRESULT hr = ::CoCreateInstance( CLSID_StructuredSave, NULL,  CLSCTX_INPROC_SERVER, IID_IStructuredSave, (void**)&pStrSave );
      ATLASSERT(SUCCEEDED(hr));
      CString strFile = fileDlg.GetPathName();
      hr = pStrSave->Open( strFile );

      pStrSave->BeginUnit(_T("BEToolbox"),1.0);

      model.Save(pStrSave);

      pStrSave->EndUnit(); // BEToolbox

      hr = pStrSave->Close();

      AfxMessageBox(_T("Export Complete"));
   }
   return S_OK;
}

bool CPGStableExporter::ConfigureModel(IBroker* pBroker,const CSegmentKey& segmentKey,CPGStableModel& model)
{
   GET_IFACE2(pBroker,IProgress,pProgress);
   CEAFAutoProgress ap(pProgress);
   pProgress->UpdateMessage(_T("Exporting PGStable model"));

   GET_IFACE2(pBroker,IIntervals,pIntervals);
   IntervalIndexType releaseIntervalIdx = pIntervals->GetPrestressReleaseInterval(segmentKey);
   IntervalIndexType liftIntervalIdx = pIntervals->GetLiftSegmentInterval(segmentKey);
   IntervalIndexType haulIntervalIdx = pIntervals->GetHaulSegmentInterval(segmentKey);

   GET_IFACE2(pBroker,IGirder,pGirder);
   bool bIsPrismatic = pGirder->IsPrismatic(liftIntervalIdx,segmentKey);
   ATLASSERT(bIsPrismatic == pGirder->IsPrismatic(haulIntervalIdx,segmentKey));

   // Girder Section Properties
   GET_IFACE2(pBroker,IBridge,pBridge);
   GET_IFACE2(pBroker,IPointOfInterest,pPoi);
   GET_IFACE2(pBroker,ISectionProperties,pSectProps);
   Float64 L = pBridge->GetSegmentLength(segmentKey);
   if ( bIsPrismatic )
   {
      model.m_GirderType = PRISMATIC;
      model.m_Girder[model.m_GirderType].ClearSections();
      std::vector<pgsPointOfInterest> vPoi = pPoi->GetPointsOfInterest(segmentKey,POI_0L | POI_RELEASED_SEGMENT);
      ATLASSERT(vPoi.size() == 1);
      pgsPointOfInterest poi = vPoi.front();
      Float64 Ag = pSectProps->GetAg(liftIntervalIdx,poi);
      Float64 Ix = pSectProps->GetIx(liftIntervalIdx,poi);
      Float64 Iy = pSectProps->GetIy(liftIntervalIdx,poi);
      Float64 Yt = -pSectProps->GetY(liftIntervalIdx,poi,pgsTypes::TopGirder);
      Float64 Hg = pSectProps->GetHg(liftIntervalIdx,poi);
      Float64 Wtop = pGirder->GetTopWidth(poi);
      Float64 Wbot = pGirder->GetBottomWidth(poi);
      model.m_Girder[model.m_GirderType].AddSection(L,Ag,Ix,Iy,Yt,Hg,Wtop,Wbot);
   }
   else
   {
      model.m_GirderType = NONPRISMATIC;
      model.m_Girder[model.m_GirderType].ClearSections();
      std::vector<pgsPointOfInterest> vPoi = pPoi->GetPointsOfInterest(segmentKey,POI_SECTCHANGE);
      std::vector<pgsPointOfInterest>::iterator iter1(vPoi.begin());
      std::vector<pgsPointOfInterest>::iterator iter2(iter1+1);
      std::vector<pgsPointOfInterest>::iterator end(vPoi.end());
      for ( ; iter2 != end; iter1++, iter2++ )
      {
         pgsPointOfInterest poi1 = *iter1;
         pgsPointOfInterest poi2 = *iter2;
         Float64 Ls = poi2.GetDistFromStart() - poi1.GetDistFromStart();
         Float64 Ag1 = pSectProps->GetAg(liftIntervalIdx,poi1);
         Float64 Ix1 = pSectProps->GetIx(liftIntervalIdx,poi1);
         Float64 Iy1 = pSectProps->GetIy(liftIntervalIdx,poi1);
         Float64 Yt1 = -pSectProps->GetY(liftIntervalIdx,poi1,pgsTypes::TopGirder);
         Float64 Hg1 = pSectProps->GetHg(liftIntervalIdx,poi1);
         Float64 Wtop1 = pGirder->GetTopWidth(poi1);
         Float64 Wbot1 = pGirder->GetBottomWidth(poi1);

         Float64 Ag2 = pSectProps->GetAg(liftIntervalIdx,poi2);
         Float64 Ix2 = pSectProps->GetIx(liftIntervalIdx,poi2);
         Float64 Iy2 = pSectProps->GetIy(liftIntervalIdx,poi2);
         Float64 Yt2 = -pSectProps->GetY(liftIntervalIdx,poi2,pgsTypes::TopGirder);
         Float64 Hg2 = pSectProps->GetHg(liftIntervalIdx,poi2);
         Float64 Wtop2 = pGirder->GetTopWidth(poi2);
         Float64 Wbot2 = pGirder->GetBottomWidth(poi2);
         model.m_Girder[model.m_GirderType].AddSection(Ls,Ag1,Ix1,Iy1,Yt1,Hg1,Wtop1,Wbot1,Ag2,Ix2,Iy2,Yt2,Hg2,Wtop2,Wbot2);
      }
   }

   GET_IFACE2(pBroker,IProductLoads,pProductLoads);
   std::vector<DiaphragmLoad> vDiaLoads;
   pProductLoads->GetPrecastDiaphragmLoads(segmentKey,&vDiaLoads);
   BOOST_FOREACH(DiaphragmLoad& diaLoad,vDiaLoads)
   {
      model.m_Girder[model.m_GirderType].AddPointLoad(diaLoad.Loc,-diaLoad.Load);
   }

   // Strand Geometry
   GET_IFACE2(pBroker,IBridgeDescription,pIBridgeDesc);
   const CPrecastSegmentData* pSegment = pIBridgeDesc->GetPrecastSegmentData(segmentKey);

   if ( pSegment->Strands.GetStrandDefinitionType() == CStrandData::sdtDirectInput )
   {
#pragma Reminder("WORKING HERE - need to deal with the direct input case")
      // this is the case that corresponds to cantilevered girders with harped strands
      // that straighten out on the top of the girder.... 
      // 
      // don't really have a good way to handle this case since there could be multiple harp points
      // as well as straight strands that aren't parallel to girder faces
      if ( AfxMessageBox(_T("This girder uses direct input strands. These strands cannot be added to the PGStable model"),MB_OKCANCEL | MB_ICONINFORMATION) == IDCANCEL )
      {
         return false;
      }

      //{
      //   ATLASSERT(vhpPoi.size() == 3 || vhpPoi.size() == 4);
      //   std::vector<pgsPointOfInterest>::iterator iter(vhpPoi.begin());
      //   pgsPointOfInterest poi(*iter++);
      //   model.m_Strands[model.m_GirderType].Xh1 = poi.GetDistFromStart();
      //   model.m_Strands[model.m_GirderType].Xh1Measure = DISTANCE;
      //   model.m_Strands[model.m_GirderType].Yh1 = -pStrandGeom->GetStrandLocation(poi,pgsTypes::Harped,liftIntervalIdx);
      //   model.m_Strands[model.m_GirderType].Yh1Measure = TOP;
      //   
      //   poi = *iter++;
      //   Hg = pSectProps->GetHg(liftIntervalIdx,poi);
      //   model.m_Strands[model.m_GirderType].Xh2 = poi.GetDistFromStart();
      //   model.m_Strands[model.m_GirderType].Xh2Measure = DISTANCE;
      //   model.m_Strands[model.m_GirderType].Yh2 = Hg + pStrandGeom->GetStrandLocation(poi,pgsTypes::Harped,liftIntervalIdx);
      //   model.m_Strands[model.m_GirderType].Yh2Measure = BOTTOM;

      //   poi = *iter++;
      //   Hg = pSectProps->GetHg(liftIntervalIdx,poi);
      //   model.m_Strands[model.m_GirderType].Xh3 = poi.GetDistFromStart();
      //   model.m_Strands[model.m_GirderType].Xh3Measure = DISTANCE;
      //   model.m_Strands[model.m_GirderType].Yh3 = Hg + pStrandGeom->GetStrandLocation(poi,pgsTypes::Harped,liftIntervalIdx);
      //   model.m_Strands[model.m_GirderType].Yh3Measure = BOTTOM;

      //   poi = *iter++;
      //   model.m_Strands[model.m_GirderType].Xh4 = poi.GetDistFromStart();
      //   model.m_Strands[model.m_GirderType].Xh4Measure = DISTANCE;
      //   model.m_Strands[model.m_GirderType].Yh4 = -pStrandGeom->GetStrandLocation(poi,pgsTypes::Harped,liftIntervalIdx);
      //   model.m_Strands[model.m_GirderType].Yh4Measure = TOP;

      //   // the 4 harp point model can only be used with nonprismatic girders (that is the way the PGStable UI works)
      //   // make the girder nonprismatic
      //   if ( bIsPrismatic )
      //   {
      //      model.m_Girder[NONPRISMATIC]  = model.m_Girder[PRISMATIC];
      //      model.m_Strands[NONPRISMATIC] = model.m_Strands[PRISMATIC];
      //      model.m_GirderType = NONPRISMATIC;
      //      bIsPrismatic = false;
      //   }
      //}
   }
   else
   {
      GET_IFACE2(pBroker,IStrandGeometry,pStrandGeom);
      std::vector<pgsPointOfInterest> vPoi = pPoi->GetPointsOfInterest(segmentKey,POI_0L | POI_10L | POI_RELEASED_SEGMENT);
      ATLASSERT(vPoi.size() == 2);
      pgsPointOfInterest poi0 = vPoi.front();
      pgsPointOfInterest poi10 = vPoi.back();

      Float64 Hg = pSectProps->GetHg(liftIntervalIdx,poi0);
      
      model.m_Strands[model.m_GirderType].Ys = Hg + pStrandGeom->GetStrandLocation(poi0,pgsTypes::Straight,liftIntervalIdx);
      model.m_Strands[model.m_GirderType].YsMeasure = BOTTOM;
      model.m_Strands[model.m_GirderType].Yt = -pStrandGeom->GetStrandLocation(poi0,pgsTypes::Temporary,liftIntervalIdx);
      model.m_Strands[model.m_GirderType].YtMeasure = TOP;

      std::vector<pgsPointOfInterest> vhpPoi = pPoi->GetPointsOfInterest(segmentKey,POI_HARPINGPOINT);
      if ( vhpPoi.size() == 0 )
      {
         // no POI's so use dummy values
         model.m_Strands[model.m_GirderType].Xh1 = 0.0;
         model.m_Strands[model.m_GirderType].Xh1Measure = FRACTION;
         model.m_Strands[model.m_GirderType].Yh1 = -pStrandGeom->GetStrandLocation(poi0,pgsTypes::Harped,liftIntervalIdx);
         model.m_Strands[model.m_GirderType].Yh1Measure = TOP;

         model.m_Strands[model.m_GirderType].Xh2 = 0.4;
         model.m_Strands[model.m_GirderType].Xh2Measure = FRACTION;
         model.m_Strands[model.m_GirderType].Yh2 = Hg + pStrandGeom->GetStrandLocation(poi0,pgsTypes::Harped,liftIntervalIdx);
         model.m_Strands[model.m_GirderType].Yh2Measure = BOTTOM;

         model.m_Strands[model.m_GirderType].Xh3 = 0.6;
         model.m_Strands[model.m_GirderType].Xh3Measure = FRACTION;
         model.m_Strands[model.m_GirderType].Yh3 = Hg + pStrandGeom->GetStrandLocation(poi0,pgsTypes::Harped,liftIntervalIdx);
         model.m_Strands[model.m_GirderType].Yh3Measure = BOTTOM;

         model.m_Strands[model.m_GirderType].Xh4 = 1.0;
         model.m_Strands[model.m_GirderType].Xh4Measure = FRACTION;
         model.m_Strands[model.m_GirderType].Yh4 = -pStrandGeom->GetStrandLocation(poi10,pgsTypes::Harped,liftIntervalIdx);
         model.m_Strands[model.m_GirderType].Yh4Measure = TOP;
      }
      else if ( vhpPoi.size() == 1 )
      {
         pgsPointOfInterest hpPoi = vhpPoi.front();
         model.m_Strands[model.m_GirderType].Xh1 = 0.0;
         model.m_Strands[model.m_GirderType].Xh1Measure = FRACTION;
         model.m_Strands[model.m_GirderType].Yh1 = -pStrandGeom->GetStrandLocation(poi0,pgsTypes::Harped,liftIntervalIdx);
         model.m_Strands[model.m_GirderType].Yh1Measure = TOP;

         Hg = pSectProps->GetHg(liftIntervalIdx,hpPoi);

         model.m_Strands[model.m_GirderType].Xh2 = hpPoi.GetDistFromStart()/L;
         model.m_Strands[model.m_GirderType].Xh2Measure = FRACTION;
         model.m_Strands[model.m_GirderType].Yh2 = Hg + pStrandGeom->GetStrandLocation(hpPoi,pgsTypes::Harped,liftIntervalIdx);
         model.m_Strands[model.m_GirderType].Yh2Measure = BOTTOM;

         model.m_Strands[model.m_GirderType].Xh3 = hpPoi.GetDistFromStart()/L;
         model.m_Strands[model.m_GirderType].Xh3Measure = FRACTION;
         model.m_Strands[model.m_GirderType].Yh3 = Hg + pStrandGeom->GetStrandLocation(hpPoi,pgsTypes::Harped,liftIntervalIdx);
         model.m_Strands[model.m_GirderType].Yh3Measure = BOTTOM;

         model.m_Strands[model.m_GirderType].Xh4 = 1.0;
         model.m_Strands[model.m_GirderType].Xh4Measure = FRACTION;
         model.m_Strands[model.m_GirderType].Yh4 = -pStrandGeom->GetStrandLocation(poi10,pgsTypes::Harped,liftIntervalIdx);
         model.m_Strands[model.m_GirderType].Yh4Measure = TOP;
      }
      else
      {
         ATLASSERT(vhpPoi.size() == 2);
         pgsPointOfInterest hp1Poi = vhpPoi.front();
         pgsPointOfInterest hp2Poi = vhpPoi.back();
         model.m_Strands[model.m_GirderType].Xh1 = 0.0;
         model.m_Strands[model.m_GirderType].Xh1Measure = FRACTION;
         model.m_Strands[model.m_GirderType].Yh1 = -pStrandGeom->GetStrandLocation(poi0,pgsTypes::Harped,liftIntervalIdx);
         model.m_Strands[model.m_GirderType].Yh1Measure = TOP;

         Hg = pSectProps->GetHg(liftIntervalIdx,hp1Poi);

         model.m_Strands[model.m_GirderType].Xh2 = hp1Poi.GetDistFromStart()/L;
         model.m_Strands[model.m_GirderType].Xh2Measure = FRACTION;
         model.m_Strands[model.m_GirderType].Yh2 = Hg + pStrandGeom->GetStrandLocation(hp1Poi,pgsTypes::Harped,liftIntervalIdx);
         model.m_Strands[model.m_GirderType].Yh2Measure = BOTTOM;

         Hg = pSectProps->GetHg(liftIntervalIdx,hp2Poi);

         model.m_Strands[model.m_GirderType].Xh3 = hp2Poi.GetDistFromStart()/L;
         model.m_Strands[model.m_GirderType].Xh3Measure = FRACTION;
         model.m_Strands[model.m_GirderType].Yh3 = Hg + pStrandGeom->GetStrandLocation(hp2Poi,pgsTypes::Harped,liftIntervalIdx);
         model.m_Strands[model.m_GirderType].Yh3Measure = BOTTOM;

         model.m_Strands[model.m_GirderType].Xh4 = 1.0;
         model.m_Strands[model.m_GirderType].Xh4Measure = FRACTION;
         model.m_Strands[model.m_GirderType].Yh4 = -pStrandGeom->GetStrandLocation(poi0,pgsTypes::Harped,liftIntervalIdx);
         model.m_Strands[model.m_GirderType].Yh4Measure = TOP;
      }
   }

   // Materials
   GET_IFACE2(pBroker,IMaterials,pMaterials);
   model.m_Girder[model.m_GirderType].SetDensity(pMaterials->GetSegmentWeightDensity(segmentKey,liftIntervalIdx)); // density for computing dead load
   model.m_Density = pMaterials->GetSegmentStrengthDensity(segmentKey); // density for computed Ec

   model.m_Fci = pMaterials->GetSegmentFc(segmentKey,liftIntervalIdx);
   model.m_Fc  = pMaterials->GetSegmentFc(segmentKey,haulIntervalIdx);

   model.m_bComputeEci = !pSegment->Material.Concrete.bUserEci;
   model.m_bComputeEc  = !pSegment->Material.Concrete.bUserEc;
   
   model.m_LiftingStabilityProblem.SetEc(pMaterials->GetSegmentEc(segmentKey,liftIntervalIdx));
   model.m_HaulingStabilityProblem.SetEc(pMaterials->GetSegmentEc(segmentKey,haulIntervalIdx));

   GET_IFACE2(pBroker,ISegmentLiftingSpecCriteria,pSegmentLiftingSpecCriteria);
   model.m_LiftingFrCoefficient = pSegmentLiftingSpecCriteria->GetLiftingModulusOfRuptureFactor(pSegment->Material.Concrete.Type);

   GET_IFACE2(pBroker,ISegmentHaulingSpecCriteria,pSegmentHaulingSpecCriteria);
   model.m_HaulingFrCoefficient = pSegmentHaulingSpecCriteria->GetHaulingModulusOfRuptureFactor(pSegment->Material.Concrete.Type);

   // Effective Prestress Force
   GET_IFACE2(pBroker,IPretensionForce,pPSForce);
   model.m_LiftingFpeType = VARYING_FPE;
   model.m_HaulingFpeType = VARYING_FPE;
   model.m_LiftingStabilityProblem.ClearFpe();
   model.m_HaulingStabilityProblem.ClearFpe();
   std::vector<pgsPointOfInterest> vPoi = pPoi->GetPointsOfInterest(segmentKey);
   BOOST_FOREACH(pgsPointOfInterest& poi,vPoi)
   {
      Float64 X = poi.GetDistFromStart();
      Float64 Fse = pPSForce->GetPrestressForce(poi,pgsTypes::Straight, liftIntervalIdx,pgsTypes::Start);
      Float64 Fhe = pPSForce->GetPrestressForce(poi,pgsTypes::Harped,   liftIntervalIdx,pgsTypes::Start);
      Float64 Fte = pPSForce->GetPrestressForce(poi,pgsTypes::Temporary,liftIntervalIdx,pgsTypes::Start);
      model.m_LiftingStabilityProblem.AddFpe(X,Fse,Fhe,Fte);

      Fse = pPSForce->GetPrestressForce(poi,pgsTypes::Straight, haulIntervalIdx,pgsTypes::Start);
      Fhe = pPSForce->GetPrestressForce(poi,pgsTypes::Harped,   haulIntervalIdx,pgsTypes::Start);
      Fte = pPSForce->GetPrestressForce(poi,pgsTypes::Temporary,haulIntervalIdx,pgsTypes::Start);
      model.m_HaulingStabilityProblem.AddFpe(X,Fse,Fhe,Fte);
   }

   // Lifting Analysis Parameters
   vPoi = pPoi->GetPointsOfInterest(segmentKey,POI_5L | POI_RELEASED_SEGMENT);
   pgsPointOfInterest msPoi(vPoi.front());
   GET_IFACE2(pBroker,IProductForces,pProductForces);
   GET_IFACE2(pBroker,ICamber,pCamber);
   Float64 gms  = pProductForces->GetDeflection(releaseIntervalIdx,pgsTypes::pftGirder,msPoi,pgsTypes::SimpleSpan,rtCumulative,false);
   Float64 gend = pProductForces->GetDeflection(releaseIntervalIdx,pgsTypes::pftGirder,pgsPointOfInterest(segmentKey,0.0),pgsTypes::SimpleSpan,rtCumulative,false);
   Float64 g = gms - gend; // girder dead load deflection relative to ends of girder
   Float64 psp = pCamber->GetPrestressDeflection(msPoi,pgsTypes::pddRelease);
   Float64 pst = pCamber->GetInitialTempPrestressDeflection(msPoi,pgsTypes::pddRelease);
   Float64 camber = g + psp + pst; // we want camber relative to the ends of the girder
   model.m_LiftingStabilityProblem.SetCamber(true,camber);
   model.m_LiftingStabilityProblem.SetSupportLocations(pSegment->HandlingData.LeftLiftPoint,pSegment->HandlingData.RightLiftPoint);
   model.m_LiftingStabilityProblem.SetYRollAxis(pSegmentLiftingSpecCriteria->GetHeightOfPickPointAboveGirderTop());
   model.m_LiftingStabilityProblem.SetSweepTolerance(pSegmentLiftingSpecCriteria->GetLiftingSweepTolerance());
   model.m_LiftingStabilityProblem.SetSupportPlacementTolerance(pSegmentLiftingSpecCriteria->GetLiftingLoopPlacementTolerance());
   Float64 impactUp,impactDown;
   pSegmentLiftingSpecCriteria->GetLiftingImpact(&impactDown,&impactUp);
   model.m_LiftingStabilityProblem.SetImpact(impactUp,impactDown);
   model.m_LiftingStabilityProblem.ApplyImpactToTiltedGirder(true);
   model.m_LiftingStabilityProblem.SetWindPressure(0);

   // Hauling Analysis Parameters
   model.m_HaulingStabilityProblem.SetCamber(false,pSegmentHaulingSpecCriteria->GetIncreaseInCgForCamber());
   model.m_HaulingStabilityProblem.SetSupportLocations(pSegment->HandlingData.TrailingSupportPoint,pSegment->HandlingData.LeadingSupportPoint);
   model.m_HaulingStabilityProblem.SetSweepTolerance(pSegmentHaulingSpecCriteria->GetHaulingSweepTolerance());
   model.m_HaulingStabilityProblem.SetSupportPlacementTolerance(pSegmentHaulingSpecCriteria->GetHaulingSupportPlacementTolerance());
   pSegmentHaulingSpecCriteria->GetHaulingImpact(&impactDown,&impactUp);
   model.m_HaulingStabilityProblem.SetImpact(impactUp,impactDown);
   model.m_HaulingStabilityProblem.ApplyImpactToTiltedGirder(false);
   model.m_HaulingStabilityProblem.SetWindPressure(0);
   model.m_HaulingStabilityProblem.SetWheelLineSpacing(pSegmentHaulingSpecCriteria->GetAxleWidth());
   model.m_HaulingStabilityProblem.SetHeightOfRollAxisAboveRoadway(pSegmentHaulingSpecCriteria->GetHeightOfTruckRollCenterAboveRoadway());
   model.m_HaulingStabilityProblem.SetSuperelevation(pSegmentHaulingSpecCriteria->GetMaxSuperelevation());
   model.m_HaulingStabilityProblem.SetVelocity(0);
   //model.m_HaulingStabilityProblem.SetTurningRadius(Float64 r); // don't have a value for this... use the default
   model.m_Hgb = pSegmentHaulingSpecCriteria->GetHeightOfGirderBottomAboveRoadway();

#pragma Reminder("WORKING HERE - this is based on current methodology... will need to convert this to use new methodology once implemented")
   if ( pSegmentHaulingSpecCriteria->GetRollStiffnessMethod() == ISegmentHaulingSpecCriteria::LumpSum )
   {
      model.m_HaulingStabilityProblem.SetTruckRotationalStiffness(pSegmentHaulingSpecCriteria->GetLumpSumRollStiffness());
   }
   else
   {
      Float64 Wa = pSegmentHaulingSpecCriteria->GetAxleWeightLimit();
      IndexType nAxles = int(model.m_Girder[model.m_GirderType].GetGirderWeight()/Wa) + 1;
      Float64 Ktheta = nAxles*pSegmentHaulingSpecCriteria->GetAxleStiffness();
      Ktheta = Max(Ktheta,pSegmentHaulingSpecCriteria->GetMinimumRollStiffness());
      model.m_HaulingStabilityProblem.SetTruckRotationalStiffness(Ktheta);
   }

   return true;
}