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

// GenCompExporter.cpp : Implementation of CGenCompExporter
#include "stdafx.h"
#include "BEToolbox_i.h"
#include "GenCompExporter.h"
#include <GenComp.h>

#include <IFace\Bridge.h>
#include <IFace\Intervals.h>
#include <EAF\EAFDisplayUnits.h>
#include <IFace\Selection.h>

#include <PgsExt\Prompts.h>

HRESULT CGenCompExporter::FinalConstruct()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   VERIFY(m_Bitmap.LoadBitmap(IDB_GENCOMP));
   return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// CGenCompExporter

STDMETHODIMP CGenCompExporter::Init(UINT nCmdID)
{
   return S_OK;
}

STDMETHODIMP CGenCompExporter::GetMenuText(BSTR*  bstrText)
{
   *bstrText = CComBSTR("BEToolbox:GenComp composite girder section model");
   return S_OK;
}

STDMETHODIMP CGenCompExporter::GetBitmapHandle(HBITMAP* phBmp)
{
   *phBmp = m_Bitmap;
   return S_OK;
}

STDMETHODIMP CGenCompExporter::GetCommandHintText(BSTR*  bstrText)
{
   *bstrText = CComBSTR("BEToolbox:GenComp model\nTool tip text");
   return S_OK;   
}

STDMETHODIMP CGenCompExporter::Export(IBroker* pBroker)
{
   pgsPointOfInterest poi;
   GET_IFACE2(pBroker,ISelection,pSelection);
   CSelection selection = pSelection->GetSelection();
   if (selection.Type == CSelection::Girder )
   {
      poi = pgsPointOfInterest(CSegmentKey(selection.GroupIdx,selection.GirderIdx,0),0.0);
   }
   else if ( selection.Type == CSelection::Segment )
   {
      poi = pgsPointOfInterest(CSegmentKey(selection.GroupIdx,selection.GirderIdx,selection.SegmentIdx),0.0);
   }
   else
   {
      poi = pgsPointOfInterest(CSegmentKey(0,0,0),0.0);
   }

   IntervalIndexType intervalIdx = 0;
   if ( pgsSelectPointOfInterest(poi,intervalIdx) != IDOK )
   {
      return S_FALSE;
   }

   // write some bridge data to a text file
	CFileDialog fileDlg(FALSE,_T("GenComp"),_T("PGSuperExport.GenComp"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("GenComp File (*.GenComp)|*.GenComp||"));
	if (fileDlg.DoModal() == IDOK)
	{
      GET_IFACE2(pBroker,IShapes,pShapes);
      CComPtr<IShape> shape;
      pShapes->GetSegmentShape(intervalIdx,poi,false,pgsTypes::scGirder,&shape);

      CComPtr<IPoint2dCollection> primaryShapePoints;
      CComPtr<IPoint2dCollection> secondaryShapePoints;
      CComQIPtr<ICompositeShape> compShape(shape);
      if ( compShape )
      {
         IndexType nShapes;
         compShape->get_Count(&nShapes);

         CComPtr<IShape> primaryShape;
         CComPtr<ICompositeShapeItem> item;
         compShape->get_Item(0,&item);
         item->get_Shape(&primaryShape);
         primaryShape->get_PolyPoints(&primaryShapePoints);

         if ( 1 < nShapes )
         {
            CComPtr<IShape> secondaryShape;
            item.Release();
            compShape->get_Item(1,&item);
            item->get_Shape(&secondaryShape);
            secondaryShape->get_PolyPoints(&secondaryShapePoints);
         }
      }
      else
      {
         shape->get_PolyPoints(&primaryShapePoints);
      }



      GET_IFACE2(pBroker,IMaterials,pMaterials);
      Float64 EcGdr;
      if ( poi.HasAttribute(POI_CLOSURE) )
      {
         EcGdr = pMaterials->GetClosureJointEc(poi.GetSegmentKey(),intervalIdx);
      }
      else
      {
         EcGdr = pMaterials->GetSegmentEc(poi.GetSegmentKey(),intervalIdx);
      }

      Float64 EcDeck = pMaterials->GetDeckEc(intervalIdx);

      std::auto_ptr<GenComp> genCompXML( CreateGenCompModel() );
      ShapeType& primaryShapeXML(genCompXML->PrimaryShape());
      ShapeType::Point_sequence& primaryShapePointsXML(primaryShapeXML.Point());
      ShapeType& secondaryShapeXML(genCompXML->SecondaryShape());
      ShapeType::Point_sequence& secondaryShapePointsXML(secondaryShapeXML.Point());

      GET_IFACE2(pBroker,IEAFDisplayUnits,pDisplayUnits);
      genCompXML->Units(pDisplayUnits->GetUnitMode() == eafTypes::umSI ? UnitMode::SI : UnitMode::US);

      Float64 n = EcDeck/EcGdr;
      n = ::RoundOff(n,0.001);
      genCompXML->ModularRatio(n);

      CollectionIndexType nPoints;
      if ( primaryShapePoints )
      {
         primaryShapePoints->get_Count(&nPoints);
         for ( CollectionIndexType i = 0; i < nPoints; i++ )
         {
            CComPtr<IPoint2d> pnt;
            primaryShapePoints->get_Item(i,&pnt);
            Float64 x,y;
            pnt->Location(&x,&y);

            x = IsZero(x) ? 0 : x;
            y = IsZero(y) ? 0 : y;

            primaryShapePointsXML.push_back(PointType(x,y));
         }
      }

      if ( secondaryShapePoints )
      {
         secondaryShapePoints->get_Count(&nPoints);
         for ( CollectionIndexType i = 0; i < nPoints; i++ )
         {
            CComPtr<IPoint2d> pnt;
            secondaryShapePoints->get_Item(i,&pnt);
            Float64 x,y;
            pnt->Location(&x,&y);

            x = IsZero(x) ? 0 : x;
            y = IsZero(y) ? 0 : y;

            secondaryShapePointsXML.push_back(PointType(x,y));
         }
      }

      CString strPathName = fileDlg.GetPathName();
      if (  SaveGenCompModel(strPathName,genCompXML.get()) )
      {
         AFX_MANAGE_STATE(AfxGetAppModuleState());
         AfxMessageBox(_T("Export complete"),MB_OK | MB_ICONEXCLAMATION);
         return S_OK;
      }
   }		

   return S_FALSE;
}
