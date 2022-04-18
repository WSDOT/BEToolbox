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

// PGStableExporter.cpp : Implementation of CPGStableExporter
#include "stdafx.h"
#include "..\BEToolbox_i.h"

#include "PGStableExporter.h"
#include "PGStableDoc.h"

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
#include <IFace\BeamFactory.h>

#include <WBFLCore.h>

#include <PgsExt\Prompts.h>
#include <PgsExt\SplicedGirderData.h>
#include <PgsExt\PrecastSegmentData.h>
#include <PgsExt\GirderLabel.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


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

STDMETHODIMP CPGStableExporter::GetMenuText(BSTR*  bstrText) const
{
   *bstrText = CComBSTR("BEToolbox:PGStable model");
   return S_OK;
}

STDMETHODIMP CPGStableExporter::GetBitmapHandle(HBITMAP* phBmp) const
{
   *phBmp = m_Bitmap;
   return S_OK;
}

STDMETHODIMP CPGStableExporter::GetCommandHintText(BSTR*  bstrText) const
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

   GET_IFACE2(pBroker, IMaterials, pMaterials);
   if (pMaterials->GetSegmentConcreteType(segmentKey) == pgsTypes::PCI_UHPC)
   {
      AfxMessageBox(_T("Cannot export. PGStable does not support PCI UHPC concrete."));
      return S_FALSE;
   }

   GET_IFACE2(pBroker,IEAFDocument,pDoc);
   CString strExtension(_T("PGStable"));
   CString strDefaultFileName;
   if ( pDocType->IsPGSuperDocument() )
   {
      strDefaultFileName.Format(_T("%s%s_Span_%s_Girder_%s.%s"),
         pDoc->GetFileRoot(), // path to file
         pDoc->GetFileTitle(), // the file name without path or extension
         LABEL_SPAN(segmentKey.groupIndex),
         LABEL_GIRDER(segmentKey.girderIndex),
         strExtension);
   }
   else
   {
      strDefaultFileName.Format(_T("%s%s_Group_%s_Girder_%s_Segment_%d.%s"),
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

      HRESULT hr = ::CoCreateInstance( CLSID_StructuredSave, nullptr,  CLSCTX_INPROC_SERVER, IID_IStructuredSave, (void**)&pStrSave );
      ATLASSERT(SUCCEEDED(hr));
      CString strFile = fileDlg.GetPathName();
      hr = pStrSave->Open( strFile );

      pStrSave->BeginUnit(_T("BEToolbox"),1.0);

      GET_IFACE2(pBroker,IProjectProperties,pProjProps);
      pStrSave->BeginUnit(_T("ProjectProperties"),1.0);
      pStrSave->put_Property(_T("Engineer"),CComVariant(pProjProps->GetEngineer()));
      pStrSave->put_Property(_T("Company"),CComVariant(pProjProps->GetCompany()));
      pStrSave->put_Property(_T("Job"),CComVariant(pProjProps->GetJobNumber()));
      pStrSave->put_Property(_T("Comments"),CComVariant(_T("")));
      pStrSave->EndUnit(); // ProjectProperties

      GET_IFACE2(pBroker,ISpecification,pSpec);
      GET_IFACE2(pBroker,IBridgeDescription,pBridgeDesc);
      CString strSpec = pSpec->GetSpecification().c_str();
      CString strTruck = pBridgeDesc->GetGirder(segmentKey)->GetSegment(segmentKey.segmentIndex)->HandlingData.HaulTruckName.c_str();

      CString strGirder = gs_strGirder;

      const GirderLibraryEntry* pGirderEntry = pBridgeDesc->GetGirder(segmentKey)->GetGirderLibraryEntry();

      CComPtr<IBeamFactory> factory;
      pGirderEntry->GetBeamFactory(&factory);

      CComQIPtr<ISplicedBeamFactory> splicedFactory(factory); // using only PGSuper prismatic beams... want splicedFactory to be nullptr
      if ( splicedFactory == nullptr && factory->IsPrismatic(segmentKey) )
      {
         strGirder = pBridgeDesc->GetGirder(segmentKey)->GetGirderName();
      }

      pStrSave->BeginUnit(_T("LibraryReferences"),1.0);
      pStrSave->put_Property(_T("ProjectCriteria"),CComVariant(strSpec));
      pStrSave->put_Property(_T("Girder"),CComVariant(strGirder));
      pStrSave->put_Property(_T("HaulTruck"),CComVariant(strTruck));
      pStrSave->EndUnit(); // LibraryReferences

      model.Save(pStrSave);

      pStrSave->EndUnit(); // BEToolbox

      hr = pStrSave->Close();

      AfxMessageBox(_T("Export Complete"));
   }
   return S_OK;
}

bool CPGStableExporter::ConfigureModel(IBroker* pBroker,const CSegmentKey& segmentKey,CPGStableModel& model)
{
   GET_IFACE2(pBroker, ISegmentTendonGeometry, pSegmentTendonGeometry);
   DuctIndexType nSegmentDucts = pSegmentTendonGeometry->GetDuctCount(segmentKey);
   if (0 < nSegmentDucts)
   {
      if (AfxMessageBox(_T("Plant installed segment post-tensioning cannot be exported into the PGStable model. Would you like to proceed?"), MB_ICONINFORMATION | MB_YESNO) == IDNO)
      {
         return false;
      }
   }

   GET_IFACE2(pBroker,IProgress,pProgress);
   CEAFAutoProgress ap(pProgress);
   pProgress->UpdateMessage(_T("Exporting PGStable model"));

   GET_IFACE2(pBroker,IIntervals,pIntervals);
   IntervalIndexType releaseIntervalIdx = pIntervals->GetPrestressReleaseInterval(segmentKey);
   IntervalIndexType liftingIntervalIdx = pIntervals->GetLiftSegmentInterval(segmentKey);
   IntervalIndexType haulingIntervalIdx = pIntervals->GetHaulSegmentInterval(segmentKey);

   GET_IFACE2(pBroker,IGirder,pGirder);
   bool bIsPrismatic = pGirder->IsPrismatic(liftingIntervalIdx,segmentKey);
   ATLASSERT(bIsPrismatic == pGirder->IsPrismatic(haulingIntervalIdx,segmentKey));

   GET_IFACE2(pBroker,IStrandGeometry,pStrandGeom);
   bool bHasDebonding = pStrandGeom->HasDebonding(segmentKey);

   model.SetGirderType((bIsPrismatic ? PRISMATIC : NONPRISMATIC));
   model.SetGirder(model.GetGirderType(),*pGirder->GetSegmentLiftingStabilityModel(segmentKey));
   model.SetStressPointType(bIsPrismatic ? COMPUTE_STRESS_POINTS : DEFINE_STRESS_POINTS);

   GET_IFACE2(pBroker,IBridge,pBridge);
   Float64 Lg = pBridge->GetSegmentLength(segmentKey);

   GET_IFACE2(pBroker,ISectionProperties,pSectProps);
   GET_IFACE2(pBroker,IPointOfInterest,pPoi);
   GET_IFACE2(pBroker,IPretensionForce,pPSForce);
   
   PoiList vPoi;
   pPoi->GetPointsOfInterest(segmentKey, &vPoi);

   CPGStableStrands liftingStrands = model.GetStrands(model.GetGirderType(),LIFTING);
   CPGStableStrands haulingStrands = model.GetStrands(model.GetGirderType(),HAULING);
   liftingStrands.strandMethod = CPGStableStrands::Detailed;
   haulingStrands.strandMethod = CPGStableStrands::Detailed;
   liftingStrands.m_vFpe.clear();
   haulingStrands.m_vFpe.clear();

   for (const pgsPointOfInterest& poi : vPoi)
   {
      Float64 X = poi.GetDistFromStart();
      if ( X < 0 || Lg < X )
      {
         continue;
      }

      Float64 Ytop = pSectProps->GetY(releaseIntervalIdx, poi, pgsTypes::TopGirder);
      Float64 Xleft = pSectProps->GetXleft(releaseIntervalIdx, poi);

      gpPoint2d es = pStrandGeom->GetEccentricity(releaseIntervalIdx, poi, pgsTypes::Straight);
      Float64 Xs = Xleft - es.X();
      Float64 Ys = Ytop + es.Y();

      gpPoint2d eh = pStrandGeom->GetEccentricity(releaseIntervalIdx,poi,pgsTypes::Harped);
      Float64 Xh = Xleft - eh.X();
      Float64 Yh = Ytop + eh.Y();

      gpPoint2d et = pStrandGeom->GetEccentricity(releaseIntervalIdx,poi,pgsTypes::Temporary);
      Float64 Xt = Xleft - et.X();
      Float64 Yt = Ytop + et.Y();

      if ( pStrandGeom->GetStrandCount(segmentKey,pgsTypes::Temporary) == 0 )
      {
         // if there aren't any temporary strands, Yt is zero. This value doesn't
         // work well in PGStable so make it 2"
         Yt = ::ConvertToSysUnits(2.0,unitMeasure::Inch);
      }

      Float64 Ps = pPSForce->GetPrestressForce(poi,pgsTypes::Straight, liftingIntervalIdx,pgsTypes::Start);
      Float64 Ph = pPSForce->GetPrestressForce(poi,pgsTypes::Harped,   liftingIntervalIdx,pgsTypes::Start);
      Float64 Pt = pPSForce->GetPrestressForce(poi,pgsTypes::Temporary,liftingIntervalIdx,pgsTypes::Start);

      liftingStrands.m_vFpe.insert(CPGStableFpe(X,Ps,Xs,Ys,TOP,Ph,Xh,Yh,TOP,Pt,Xt,Yt,TOP));

      Ps = pPSForce->GetPrestressForce(poi,pgsTypes::Straight, haulingIntervalIdx,pgsTypes::Start);
      Ph = pPSForce->GetPrestressForce(poi,pgsTypes::Harped,   haulingIntervalIdx,pgsTypes::Start);
      Pt = pPSForce->GetPrestressForce(poi,pgsTypes::Temporary,haulingIntervalIdx,pgsTypes::Start);

      haulingStrands.m_vFpe.insert(CPGStableFpe(X,Ps,Xs,Ys,TOP,Ph,Xh,Yh,TOP,Pt,Xt,Yt,TOP));
   }

   model.SetStrands(model.GetGirderType(),LIFTING,liftingStrands);
   model.SetStrands(model.GetGirderType(),HAULING,haulingStrands);

   model.SetLiftingStabilityProblem( *pGirder->GetSegmentLiftingStabilityProblem(segmentKey));
   model.SetHaulingStabilityProblem( *pGirder->GetSegmentHaulingStabilityProblem(segmentKey));

   GET_IFACE2(pBroker, IMaterials, pMaterial);
   model.SetK1(pMaterial->GetSegmentEccK1(segmentKey));
   model.SetK2(pMaterial->GetSegmentEccK2(segmentKey));

   GET_IFACE2(pBroker,ISegmentLiftingSpecCriteria,pSegmentLiftingSpecCriteria);
   GET_IFACE2(pBroker,ISegmentHaulingSpecCriteria,pSegmentHaulingSpecCriteria);

   CPGStableLiftingCriteria liftingCriteria;
   liftingCriteria = pSegmentLiftingSpecCriteria->GetLiftingStabilityCriteria(segmentKey); 
   model.SetLiftingCriteria(liftingCriteria);


   CPGStableHaulingCriteria haulingCriteria;
   haulingCriteria = pSegmentHaulingSpecCriteria->GetHaulingStabilityCriteria(segmentKey);
   model.SetHaulingCriteria(haulingCriteria);

   return true;
}