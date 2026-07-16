///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright � 1999-2026  Washington State Department of Transportation
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

// CurvelExporter.cpp : Implementation of CCurvelExporter
#include "stdafx.h"
#include "..\resource.h"
#include "CurvelExporter.h"

#include <EAF/AutoProgress.h>
#include <IFace\Project.h>
#include <EAF\EAFDisplayUnits.h>

#include <Curvel.h>
#include <MFCTools\Prompts.h>

class CSuperelevationValidator : public CMultiChoiceValidator
{
public:
   virtual BOOL IsValid(const std::vector<int>& options) override;
   virtual void DisplayValidationErrorMessage() override;

private:
   CString m_strMessage;
};

BOOL CSuperelevationValidator::IsValid(const std::vector<int>& options)
{
   if ( options.size() != 3 )
   {
      m_strMessage = _T("Please select three consecutive sections");
      return FALSE;
   }
   else
   {
      std::vector<int>::const_iterator iter(options.begin());
      std::vector<int>::const_iterator end(options.end());
      int last = *iter++;
      for ( ; iter != end; iter++ )
      {
         if ( *iter != last+1 )
         {
            m_strMessage = _T("Please select three consecutive sections");
            return FALSE;
         }

         last = *iter;
      }
   }

   return TRUE;
}

void CSuperelevationValidator::DisplayValidationErrorMessage()
{
   AfxMessageBox(m_strMessage,MB_OK | MB_ICONEXCLAMATION);
}




CCurvelExporter::CCurvelExporter()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   VERIFY(m_Bitmap.LoadBitmap(IDB_CURVEL));
}

HRESULT CCurvelExporter::Init(UINT nCmdID)
{
   return S_OK;
}

CString CCurvelExporter::GetMenuText() const
{
   return CString("BEToolbox:Curvel vertical profile model");
}

HBITMAP CCurvelExporter::GetBitmapHandle() const
{
   return m_Bitmap;
}

CString CCurvelExporter::GetCommandHintText() const
{
   return CString("Export BEToolbox:Curvel model\nTool tip text");
}

HRESULT CCurvelExporter::Export(std::shared_ptr<WBFL::EAF::Broker> pBroker)
{
   // write some bridge data to a text file
	CFileDialog fileDlg(FALSE,_T("Curvel"),_T("PGSuperExport.Curvel"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Curvel File (*.Curvel)|*.Curvel||"));
	if (fileDlg.DoModal() == IDOK)
	{
      GET_IFACE2(pBroker,IRoadwayData,pRoadway);
      std::unique_ptr<Curvel> curvelXML( CreateCurvelModel() ); // create a default curvel model

      ///////////////////////////////////////////////////////////////
      // Export vertical profile
      ///////////////////////////////////////////////////////////////
      const ProfileData2& profileData = pRoadway->GetProfileData2();
      VerticalCurveDataType& vCurveXML = curvelXML->VerticalCurveData();

      if ( profileData.VertCurves.size() == 0 )
      {
         // No vertical curves in PGSuper/PGSplice... create a zero length
         // curve Curvel
         vCurveXML.g1( profileData.Grade );
         vCurveXML.g2( profileData.Grade );
         vCurveXML.Length(0.0);
         vCurveXML.PVIStation(profileData.Station);
         vCurveXML.PVIElevation(profileData.Elevation);
      }
      else
      {
         int curveIdx = 0;

         // Curvel supports a single vertical curve, select which vertical curve to export to Curvel
         if ( 1 < profileData.VertCurves.size() )
         {
            GET_IFACE2(pBroker,IEAFDisplayUnits,pDisplayUnits);
            CString strList;
            auto iter(profileData.VertCurves.cbegin());
            auto end(profileData.VertCurves.cend());
            int idx = 1;
            for ( ; iter != end; iter++, idx++ )
            {
               const auto& vCurve(*iter);
               CString str;
               str.Format(_T("Curve %d: PVI Station %s"),idx,::FormatStation(pDisplayUnits->GetStationFormat(),vCurve.PVIStation));
               strList += str + _T("\n");

            }
            curveIdx = AfxChoose(_T("Multiple Vertical Curves"),_T("Curvel is limited to a single vertical curve. Select the vertical curve to export."),strList,0,TRUE);
            if ( curveIdx == -1 )
               return S_FALSE;
         }
         
         const auto& vCurve(profileData.VertCurves[curveIdx]);
         if ( !IsEqual(vCurve.L1,vCurve.L2) && !IsZero(vCurve.L2) )
         {
            AfxMessageBox(_T("Cannot export profile information. Curvel does not support unsymmetric vertical curves"),MB_OK | MB_ICONSTOP);
            return FALSE;
         }

         vCurveXML.g1( profileData.Grade );
         vCurveXML.g2( vCurve.ExitGrade );
         vCurveXML.Length(vCurve.L1 + vCurve.L2);

         if ( IsEqual(profileData.Station,vCurve.PVIStation) )
         {
            vCurveXML.PVIStation(profileData.Station);
            vCurveXML.PVIElevation(profileData.Elevation);
         }
         else
         {
            // Reference station and PVI of vertical curve are at different locations... compute
            // the PVI elevation
            Float64 pviElevation = profileData.Elevation + profileData.Grade*(vCurve.PVIStation - profileData.Station);
            vCurveXML.PVIStation(vCurve.PVIStation);
            vCurveXML.PVIElevation(pviElevation);
         }
      }

      ///////////////////////////////////////////////////////////////
      // Export crown slopes
      ///////////////////////////////////////////////////////////////
      CrownSlopeType crownXML;
      CrownSlopeType::SuperelevationProfilePoint_sequence& superelevationPointsXML(crownXML.SuperelevationProfilePoint());

      const RoadwaySectionData& sectionData = pRoadway->GetRoadwaySectionData();
      if ( sectionData.NumberOfSegmentsPerSection > 2 )
      {
         CString strMsg;
         strMsg.Format(_T("Curvel is limited to a single crown point. Only the two exterior crown slopes will be exported. In addtion, crown point offsets are not considered."));
         AfxMessageBox(strMsg,MB_OK | MB_ICONINFORMATION);
      }

      std::size_t nSections = sectionData.RoadwaySectionTemplates.size();
      if ( nSections <= 3 )
      {
         Float64 crownPointOffset = 0.0;
         for ( std::size_t i = 0; i < nSections; i++ )
         {
            const auto& crown = sectionData.RoadwaySectionTemplates[i];

            superelevationPointsXML.push_back(SuperelevationProfilePointType(crown.Station,crown.LeftSlope,crown.RightSlope));
         }

         if ( nSections < 3 )
         {
            std::size_t nTempSections = 3 - nSections;
            for ( std::size_t i = 0; i < nTempSections; i++ )
            {
               SuperelevationProfilePointType pp = superelevationPointsXML.back();
               pp.Station(pp.Station() + WBFL::Units::ConvertToSysUnits(50,WBFL::Units::Measure::Feet));
               superelevationPointsXML.push_back(pp);
            }
         }
         SuperelevationDataType superelevationXML(crownPointOffset,crownXML);
         curvelXML->SuperelevationData(superelevationXML);
      }
      else
      {
         CString strOptions;
         GET_IFACE2(pBroker,IEAFDisplayUnits,pDisplayUnits);
         for ( std::size_t i = 0; i < nSections; i++ )
         {
            const auto& crown = sectionData.RoadwaySectionTemplates[i];
            CString str;
            str.Format(_T("Section %d: Station %s, Left Slope: %s, Right Slope %s"), i+1,
                       ::FormatStation(pDisplayUnits->GetStationFormat(),crown.Station),
                       ::FormatScalar(crown.LeftSlope,pDisplayUnits->GetScalarFormat()),
                       ::FormatScalar(crown.RightSlope,pDisplayUnits->GetScalarFormat()));
            strOptions += str + _T("\n");
         }
         std::vector<int> choices;
         choices.push_back(0);
         choices.push_back(1);
         choices.push_back(2);
         CSuperelevationValidator validator;
         choices = AfxMultiChoice(_T("Select Cross Sections"),_T("Select three consecutive cross sections to export to Curvel."),strOptions,&validator,choices,TRUE);
         if ( choices.size() == 0 )
         {
            return S_FALSE;
         }

         ATLASSERT(choices.size()==3);
         std::vector<int>::iterator iter(choices.begin());
         std::vector<int>::iterator end(choices.end());
         Float64 crownPointOffset=0.0;
         bool bNotifyCrownPointOffset = false;
         int i = 0;
         for ( ; iter != end; iter++, i++ )
         {
            const auto& crown = sectionData.RoadwaySectionTemplates[*iter];
            superelevationPointsXML.push_back(SuperelevationProfilePointType(crown.Station,crown.LeftSlope,crown.RightSlope));
            SuperelevationDataType superelevationXML(crownPointOffset,crownXML);
            curvelXML->SuperelevationData(superelevationXML);
         }
      }

      ///////////////////////////////////////////////////////////////
      // Export horizontal curves
      ///////////////////////////////////////////////////////////////

      // Curvel does not model horizontal curves the same way PGSuper does
      // Create a skew line reporting point for each horizontal curve
      // This is the best way to export the horizontal curve information
      const AlignmentData2& alignmentData = pRoadway->GetAlignmentData2();
      auto iter(alignmentData.CompoundCurves.cbegin());
      auto end(alignmentData.CompoundCurves.cend());
      bool bSpiral = false;
      bool bCurves = false;
      SkewLinesType skewLines;
      for ( ; iter != end; iter++ )
      {
         bCurves = true;
         const auto& hCurve(*iter);
         SkewLineType skewLine(hCurve.PIStation,OffsetType::RadialFromCrownLine,0.0,_T("0.0 L"),hCurve.Radius,0.0);
         if ( !IsZero(hCurve.EntrySpiral) || !IsZero(hCurve.ExitSpiral) )
         {
            bSpiral = true;
         }

         skewLines.SkewLine().push_back(skewLine);
      }

      if ( bCurves )
      {
         curvelXML->SkewLines().set(skewLines);

         CString strMsg(_T("Horizontal curves have been modeled as Skew Line input in Curvel."));
         if ( bSpiral )
         {
            strMsg += _T("\nEntry and Exit Spirals are not modeled in Curvel.");
         }

         AfxMessageBox(strMsg,MB_OK | MB_ICONINFORMATION);
      }

      ///////////////////////////////////////////////////////////////
      // Done creating CurveXML model... save it
      ///////////////////////////////////////////////////////////////
		CString strPathName = fileDlg.GetPathName();
      if ( SaveCurvelModel(strPathName,curvelXML.get()) )
      {
         AfxMessageBox(_T("Export complete"),MB_OK | MB_ICONEXCLAMATION);
         return S_OK;
      }
	}

   return S_FALSE;
}
