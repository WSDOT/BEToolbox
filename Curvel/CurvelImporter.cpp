///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2025  Washington State Department of Transportation
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

// CurvelImporter.cpp : Implementation of CCurvelImporter
#include "stdafx.h"
#include "..\BEToolbox_i.h"
#include "CurvelImporter.h"

#include <EAF\EAFAutoProgress.h>
#include <IFace\Project.h>

#include <Curvel.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


HRESULT CCurvelImporter::FinalConstruct()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   VERIFY(m_Bitmap.LoadBitmap(IDB_CURVEL));
   return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// CCurvelImporter
STDMETHODIMP CCurvelImporter::Init(UINT nCmdID)
{
   return S_OK;
}

STDMETHODIMP CCurvelImporter::GetMenuText(BSTR*  bstrText) const
{
   *bstrText = CComBSTR("BEToolbox:Curve vertical profile model");
   return S_OK;
}

STDMETHODIMP CCurvelImporter::GetBitmapHandle(HBITMAP* phBmp) const
{
   *phBmp = m_Bitmap;
   return S_OK;
}

STDMETHODIMP CCurvelImporter::GetCommandHintText(BSTR*  bstrText) const
{
   *bstrText = CComBSTR("Import BEToolbox:Curvel model\nTool tip text");
   return S_OK;   
}

STDMETHODIMP CCurvelImporter::Import(IBroker* pBroker)
{
	CFileDialog  fileDlg(TRUE,_T("Curvel"),_T(""),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Curvel File (*.Curvel)|*.Curvel||"));
	if (fileDlg.DoModal() == IDOK)
   {
      CString strFileName(fileDlg.GetPathName());
      // PGSuper is using the old C++ units management system... we need the new C++/COM version
      CComPtr<IUnitServer> unitServer;
      unitServer.CoCreateInstance(CLSID_UnitServer);
      unitServer->SetSystemUnits(CComBSTR(WBFL::Units::System::GetMassUnit().UnitTag().c_str()),
                               CComBSTR(WBFL::Units::System::GetLengthUnit().UnitTag().c_str()),
                               CComBSTR(WBFL::Units::System::GetTimeUnit().UnitTag().c_str()),
                               CComBSTR(WBFL::Units::System::GetTemperatureUnit().UnitTag().c_str()),
                               CComBSTR(WBFL::Units::System::GetAngleUnit().UnitTag().c_str()));
      std::unique_ptr<Curvel> curvelXML(CreateCurvelModel(strFileName,unitServer));
      if ( curvelXML.get() == nullptr )
      {
         return E_FAIL;
      }

      VerticalCurveDataType& vc = curvelXML->VerticalCurveData();

      ProfileData2 profileData;
      profileData.Station = vc.PVIStation();
      profileData.Elevation = vc.PVIElevation();
      profileData.Grade = vc.g1();

      VertCurveData vcd;
      vcd.PVIStation = profileData.Station;
      vcd.ExitGrade = vc.g2();
      vcd.L1 = vc.Length();
      vcd.L2 = 0;
      profileData.VertCurves.push_back(vcd);

      Curvel::SuperelevationData_optional& super = curvelXML->SuperelevationData();
      bool bCrossSectionData = super.present();
      RoadwaySectionData sectionData;
      if ( bCrossSectionData )
      {
         Float64 cpo = super->ProfileGradeOffset();

         if (IsZero(cpo))
         {
            sectionData.NumberOfSegmentsPerSection = 2;
            sectionData.AlignmentPointIdx = 1;
            sectionData.ProfileGradePointIdx = 1;
         }
         else if (cpo > 0.0) 
         {
            sectionData.NumberOfSegmentsPerSection = 3;
            sectionData.AlignmentPointIdx = 1;
            sectionData.ProfileGradePointIdx = 1;
         }
         else // (cpo < 0.0) 
         {
            sectionData.NumberOfSegmentsPerSection = 3;
            sectionData.AlignmentPointIdx = 2;
            sectionData.ProfileGradePointIdx = 2;
         }

         for ( int i = 0; i < 3; i++ )
         {
            CrownSlopeType& crownSlope = super->CrownSlope();
            CrownSlopeType::SuperelevationProfilePoint_sequence& superPP = crownSlope.SuperelevationProfilePoint();
            ATLASSERT(superPP.size() == 3);

            RoadwaySectionTemplate crown;
            crown.Station  = superPP[i].Station();
            crown.LeftSlope     = superPP[i].LeftSlope();
            crown.RightSlope    = superPP[i].RightSlope();

            // add extra segments for crown point offset
            if (!IsZero(cpo))
            {
               RoadwaySegmentData seg;
               if (0.0 < cpo)
               {
                  seg.Length = cpo;
                  seg.Slope = crown.LeftSlope;
               }
               else
               {
                  seg.Length = -cpo;
                  seg.Slope = crown.RightSlope;
               }

               crown.SegmentDataVec.push_back(seg);
            }

            sectionData.RoadwaySectionTemplates.push_back(crown);
         }
      }

      GET_IFACE2(pBroker,IEvents,pEvents);
      pEvents->HoldEvents();

      GET_IFACE2(pBroker,IRoadwayData,pRoadway);
      pRoadway->SetProfileData2(profileData);

      if ( bCrossSectionData )
      {
         pRoadway->SetRoadwaySectionData(sectionData);
      }

      pEvents->FirePendingEvents();

      CString strMsg;
      strMsg.Format(_T("Profile and Cross Section data was successfully imported from %s\n\nCurvel data does not include horizontal alignment information. Verify your alignment data is correct."),fileDlg.GetPathName());
      AfxMessageBox(strMsg,MB_OK | MB_ICONINFORMATION);
   }
   return S_OK;
}
