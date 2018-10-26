///////////////////////////////////////////////////////////////////////
// BEToolboxXML
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

#include "stdafx.h"
#include "resource.h"
#include "Curvel.h"
#include <WBFLUnitServer\UnitServerUtils.h>
#include "Helpers.h"

#include <fstream>

#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xalanc/Include/XalanAutoPtr.hpp>
#include <xalanc/XalanDOM/XalanDocument.hpp>
#include <xalanc/XalanDOM/XalanElement.hpp>
#include <xalanc/PlatformSupport/XalanOutputStreamPrintWriter.hpp>
#include <xalanc/PlatformSupport/XalanStdOutputStream.hpp>
#include <xalanc/XMLSupport/FormatterToXML.hpp>
#include <xalanc/XercesParserLiaison/FormatterToXercesDOM.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMFormatterWalker.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>

//XALAN_USING_XERCES(DOMDocument)

XALAN_USING_XALAN(XalanCompiledStylesheet)
XALAN_USING_XALAN(XalanParsedSource)
XALAN_USING_XALAN(XalanTransformer)

std::auto_ptr<Curvel> CreateCurvelModel()
{
   std::auto_ptr<Curvel> curvelXML(new Curvel(VerticalCurveDataType(0,0,0,0,0)));

   return curvelXML;
}


BOOL ConvertToBaseUnits(Curvel* pCurvel,IUnitConvert* pDocUnitConvert)
{
   // Walk the XML data structure and make sure all units of measure are in base units

   // The unit server will usually be an application level object, but
   // I am using a local one here just because this is a test/proof of concept.
   // This represents one technique for applications to deal with units:
   // 1) Load document as is
   // 2) Walk document and convert all units to Consistent Base Units
   // 3) Do all work in consistent base units
   // 4) Save document with consistent base units
   CComPtr<IUnitServer> unitServer;
   HRESULT hr = unitServer.CoCreateInstance(CLSID_UnitServer);
   ATLASSERT(SUCCEEDED(hr));
   CComQIPtr<IUnitConvert> convert(unitServer);

   Curvel::UnitsDeclaration_optional& unitsDeclaration(pCurvel->UnitsDeclaration());
   if ( unitsDeclaration.present())
   {
      if ( !InitializeWBFLUnitServer(&unitsDeclaration.get(),unitServer) )
      {
         return FALSE;
      }
   }

   CComBSTR bstrLengthUnit;
   unitServer->get_Length(&bstrLengthUnit);
   Float64 value;


   // Convert Vertical Curve Parameters
   VerticalCurveDataType& vc(pCurvel->VerticalCurveData());

   if ( vc.Length().unit().present() )
   {
      // The unit of measure for PVI Station is specified

      // Get the unit of measure
      OpenBridgeML::Units::LengthValueType::unit_type unit = vc.Length().unit().get();

      // Convert the value from the specified unit of measure to the
      // consistent unit of measure defined in <UnitsDeclaration>
      hr = convert->ConvertToBaseUnits(vc.Length(),A2BSTR(unit.c_str()),&value);
      ATLASSERT(SUCCEEDED(hr)); // if this fires, then the unit of measure was not valid

      // Put the value back into the data model and stop using the locally
      // defined unit of measure
      vc.Length(value);
      vc.Length().unit().reset(); // no longer using the specified unit of measure
   }
   // convert from whatever base units were defined in the instance document
   // to the base units used by this application
   value = vc.Length();
   pDocUnitConvert->ConvertToBaseUnits(value,bstrLengthUnit,&value);
   vc.Length(value);
   

   if ( vc.PVIStation().unit().present() )
   {
      // The unit of measure for PVI Station is specified... get it and convert to base units
      OpenBridgeML::Units::LengthValueType::unit_type unit = vc.PVIStation().unit().get();
      hr = convert->ConvertToBaseUnits(vc.PVIStation(),A2BSTR(unit.c_str()),&value);
      ATLASSERT(SUCCEEDED(hr)); // if this fires, then the unit of measure was not valid
      vc.PVIStation(value);
      vc.PVIStation().unit().reset(); // no longer using the specified unit of measure
   }
   value = vc.PVIStation();
   pDocUnitConvert->ConvertToBaseUnits(value,bstrLengthUnit,&value);
   vc.PVIStation(value);

   if ( vc.PVIElevation().unit().present() )
   {
      // The unit of measure for PVI Elevation is specified... get it and convert to base units
      OpenBridgeML::Units::LengthValueType::unit_type unit = vc.PVIElevation().unit().get();
      hr = convert->ConvertToBaseUnits(vc.PVIElevation(),A2BSTR(unit.c_str()),&value);
      ATLASSERT(SUCCEEDED(hr)); // if this fires, then the unit of measure was not valid
      vc.PVIElevation(value);
      vc.PVIElevation().unit().reset(); // no longer using the specified unit of measure
   }
   value = vc.PVIElevation();
   pDocUnitConvert->ConvertToBaseUnits(value,bstrLengthUnit,&value);
   vc.PVIElevation(value);

   // Superelevation Data
   Curvel::SuperelevationData_optional& seData(pCurvel->SuperelevationData());
   if ( seData.present() )
   {
      if ( seData->ProfileGradeOffset().unit().present() )
      {
         OpenBridgeML::Units::LengthValueType::unit_type unit = seData->ProfileGradeOffset().unit().get();
         hr = convert->ConvertToBaseUnits(seData->ProfileGradeOffset(),A2BSTR(unit.c_str()),&value);
         ATLASSERT(SUCCEEDED(hr)); // if this fires, then the unit of measure was not valid
         seData->ProfileGradeOffset(value);
         seData->ProfileGradeOffset().unit().reset(); // no longer using the specified unit of measure
      }
      value = seData->ProfileGradeOffset();
      pDocUnitConvert->ConvertToBaseUnits(value,bstrLengthUnit,&value);
      seData->ProfileGradeOffset(value);


      CrownSlopeType& crownSlope( seData->CrownSlope() );
      CrownSlopeType::SuperelevationProfilePoint_sequence& superPP(crownSlope.SuperelevationProfilePoint());

      ATLASSERT(superPP.size() == 3);
      for ( int i = 0; i < 3; i++ )
      {
         if ( superPP[i].Station().unit().present() )
         {
            OpenBridgeML::Units::LengthValueType::unit_type unit = superPP[i].Station().unit().get();
            hr = convert->ConvertToBaseUnits(superPP[i].Station(),A2BSTR(unit.c_str()),&value);
            ATLASSERT(SUCCEEDED(hr)); // if this fires, then the unit of measure was not valid
            superPP[i].Station(value);
            superPP[i].Station().unit().reset();
         }
         value = superPP[i].Station();
         pDocUnitConvert->ConvertToBaseUnits(value,bstrLengthUnit,&value);
         superPP[i].Station(value);
      }
   }
   
   // Individual Stations
   Curvel::IndividualStations_optional& individualStationsOptional(pCurvel->IndividualStations());
   if ( individualStationsOptional.present() )
   {
      IndividualStationsType::IndividualStation_sequence& individualStations(individualStationsOptional->IndividualStation());
      IndividualStationsType::IndividualStation_iterator iter(individualStations.begin());
      IndividualStationsType::IndividualStation_iterator end(individualStations.end());
      for ( ; iter != end; iter++ )
      {
         IndividualStationType& individualStation(*iter);

         if ( individualStation.Station().unit().present() )
         {
            OpenBridgeML::Units::LengthValueType::unit_type unit = individualStation.Station().unit().get();
            hr = convert->ConvertToBaseUnits(individualStation.Station(),A2BSTR(unit.c_str()),&value);
            ATLASSERT(SUCCEEDED(hr)); // if this fires, then the unit of measure was not valid
            individualStation.Station(value);
            individualStation.Station().unit().reset();
         }
         value = individualStation.Station();
         pDocUnitConvert->ConvertToBaseUnits(value,bstrLengthUnit,&value);
         individualStation.Station(value);


         if ( individualStation.Offset().unit().present() )
         {
            OpenBridgeML::Units::LengthValueType::unit_type unit = individualStation.Offset().unit().get();
            hr = convert->ConvertToBaseUnits(individualStation.Offset(),A2BSTR(unit.c_str()),&value);
            ATLASSERT(SUCCEEDED(hr)); // if this fires, then the unit of measure was not valid
            individualStation.Offset(value);
            individualStation.Offset().unit().reset();
         }
         value = individualStation.Offset();
         pDocUnitConvert->ConvertToBaseUnits(value,bstrLengthUnit,&value);
         individualStation.Offset(value);
      }
   }

   // Station Ranges
   Curvel::StationRanges_optional& stationRangesOptional(pCurvel->StationRanges());
   if ( stationRangesOptional.present() )
   {
      StationRangesType::StationRange_sequence& stationRanges(stationRangesOptional->StationRange());
      StationRangesType::StationRange_iterator iter(stationRanges.begin());
      StationRangesType::StationRange_iterator end(stationRanges.end());
      for ( ; iter != end; iter++ )
      {
         StationRangeType& stationRange(*iter);

         if ( stationRange.StartStation().unit().present() )
         {
            OpenBridgeML::Units::LengthValueType::unit_type unit = stationRange.StartStation().unit().get();
            hr = convert->ConvertToBaseUnits(stationRange.StartStation(),A2BSTR(unit.c_str()),&value);
            ATLASSERT(SUCCEEDED(hr)); // if this fires, then the unit of measure was not valid
            stationRange.StartStation(value);
            stationRange.StartStation().unit().reset();
         }
         value = stationRange.StartStation();
         pDocUnitConvert->ConvertToBaseUnits(value,bstrLengthUnit,&value);
         stationRange.StartStation(value);

         if ( stationRange.EndStation().unit().present() )
         {
            OpenBridgeML::Units::LengthValueType::unit_type unit = stationRange.EndStation().unit().get();
            hr = convert->ConvertToBaseUnits(stationRange.EndStation(),A2BSTR(unit.c_str()),&value);
            ATLASSERT(SUCCEEDED(hr)); // if this fires, then the unit of measure was not valid
            stationRange.EndStation(value);
            stationRange.EndStation().unit().reset();
         }
         value = stationRange.EndStation();
         pDocUnitConvert->ConvertToBaseUnits(value,bstrLengthUnit,&value);
         stationRange.EndStation(value);

         if ( stationRange.Offset().unit().present() )
         {
            OpenBridgeML::Units::LengthValueType::unit_type unit = stationRange.Offset().unit().get();
            hr = convert->ConvertToBaseUnits(stationRange.Offset(),A2BSTR(unit.c_str()),&value);
            ATLASSERT(SUCCEEDED(hr)); // if this fires, then the unit of measure was not valid
            stationRange.Offset(value);
            stationRange.Offset().unit().reset();
         }
         value = stationRange.Offset();
         pDocUnitConvert->ConvertToBaseUnits(value,bstrLengthUnit,&value);
         stationRange.Offset(value);
      }
   }

   // Skew Lines
   Curvel::SkewLines_optional& skewLinesOptional(pCurvel->SkewLines());
   if ( skewLinesOptional.present() )
   {
      SkewLinesType::SkewLine_sequence& skewLines(skewLinesOptional->SkewLine());
      SkewLinesType::SkewLine_iterator iter(skewLines.begin());
      SkewLinesType::SkewLine_iterator end(skewLines.end());
      for ( ; iter != end; iter++ )
      {
         SkewLineType& skewLine(*iter);

         if ( skewLine.Station().unit().present() )
         {
            OpenBridgeML::Units::LengthValueType::unit_type unit = skewLine.Station().unit().get();
            hr = convert->ConvertToBaseUnits(skewLine.Station(),A2BSTR(unit.c_str()),&value);
            ATLASSERT(SUCCEEDED(hr)); // if this fires, then the unit of measure was not valid
            skewLine.Station(value);
            skewLine.Station().unit().reset();
         }
         value = skewLine.Station();
         pDocUnitConvert->ConvertToBaseUnits(value,bstrLengthUnit,&value);
         skewLine.Station(value);

         if ( skewLine.Offset().unit().present() )
         {
            OpenBridgeML::Units::LengthValueType::unit_type unit = skewLine.Offset().unit().get();
            hr = convert->ConvertToBaseUnits(skewLine.Offset(),A2BSTR(unit.c_str()),&value);
            ATLASSERT(SUCCEEDED(hr)); // if this fires, then the unit of measure was not valid
            skewLine.Offset(value);
            skewLine.Offset().unit().reset();
         }
         value = skewLine.Offset();
         pDocUnitConvert->ConvertToBaseUnits(value,bstrLengthUnit,&value);
         skewLine.Offset(value);

         if ( skewLine.Radius().unit().present() )
         {
            OpenBridgeML::Units::LengthValueType::unit_type unit = skewLine.Radius().unit().get();
            hr = convert->ConvertToBaseUnits(skewLine.Radius(),A2BSTR(unit.c_str()),&value);
            ATLASSERT(SUCCEEDED(hr)); // if this fires, then the unit of measure was not valid
            skewLine.Radius(value);
            skewLine.Radius().unit().reset();
         }
         value = skewLine.Radius();
         pDocUnitConvert->ConvertToBaseUnits(value,bstrLengthUnit,&value);
         skewLine.Radius(value);

         if ( skewLine.CrownOffset().unit().present() )
         {
            OpenBridgeML::Units::LengthValueType::unit_type unit = skewLine.CrownOffset().unit().get();
            hr = convert->ConvertToBaseUnits(skewLine.CrownOffset(),A2BSTR(unit.c_str()),&value);
            ATLASSERT(SUCCEEDED(hr)); // if this fires, then the unit of measure was not valid
            skewLine.CrownOffset(value);
            skewLine.CrownOffset().unit().reset();
         }
         value = skewLine.CrownOffset();
         pDocUnitConvert->ConvertToBaseUnits(value,bstrLengthUnit,&value);
         skewLine.CrownOffset(value);
      }
   }

   return TRUE;
}

std::auto_ptr<Curvel> CreateCurvelModel(LPCTSTR lpszFilePath,IUnitConvert* pDocUnitConvert)
{
   ATLASSERT(lpszFilePath != NULL);
   ATLASSERT(pDocUnitConvert != NULL);

   USES_CONVERSION;

   // Creating is a two phase process.
   // Step 1 is to run the document through the Xalan XSLT engine,
   // applying XSL Transforms to bring the document model up to
   // the most current version.
   // Step 2 is to run the document through the C++\Tree binding
   // to populate the object model.
   //
   // Most of this code is taken from the TransformToXercesDOM example
   // from the Xalan distribution.
   std::auto_ptr<Curvel> curvelXML;
   try
   {
      XALAN_USING_XERCES(XMLPlatformUtils)
      XMLPlatformUtils::Initialize();
      XalanTransformer::initialize();

      {
      XalanTransformer  theTransformer;
      const XalanParsedSource* theParsedSource = 0;

      int theResult = theTransformer.parseSource(lpszFilePath, theParsedSource);

      if (theResult != 0)
      {
         CString strMsg;
         strMsg.Format(_T("Error parsing source document: %s"),A2T(theTransformer.getLastError()));
         AfxMessageBox(strMsg);
         return std::auto_ptr<Curvel>();
      }
      else
      {
           assert(theParsedSource != 0);

           const XalanCompiledStylesheet*  theCompiledStylesheet = 0;

           const char* lpstrXSLT = NULL;
           DWORD size = 0;
#pragma Reminder("UPDATE: add error handling")
           if ( !LoadXMLResource(IDR_CURVEL_10_TO_20_XSLT,XSLTFILE,size,lpstrXSLT) )
           {
              AfxMessageBox(_T("Error loading XSL Transform resource"));
              return std::auto_ptr<Curvel>();
           }

           std::string strXSLT(lpstrXSLT,size);
           std::istringstream xsltStream(strXSLT);

           theResult = theTransformer.compileStylesheet(xsltStream, theCompiledStylesheet);
           // Note: create multiple XalanTransformer objects, one for each transformation
           // Version 1.0 -> Version 2.0
           // Version 2.0 -> Version 2.1
           // etc

           if (theResult != 0)
           {
              CString strMsg;
              strMsg.Format(_T("Error compiling stylesheet: %s"),A2T(theTransformer.getLastError()));
              AfxMessageBox(strMsg);
              return std::auto_ptr<Curvel>();
           }
           else
           {
                XALAN_USING_XERCES(DOMDocument)
                XALAN_USING_XERCES(DOMImplementation)
                XALAN_USING_XALAN(FormatterToXercesDOM)
                XALAN_USING_XALAN(XalanAutoPtr)

                // This is the document which we'll build...
                const XalanAutoPtr<DOMDocument>     theDocument(DOMImplementation::getImplementation()->createDocument());
                assert(theDocument.get() != 0);

                // This is a class derived from FormatterListener, which
                // we'll hook up to Xalan's output stage...
                FormatterToXercesDOM    theFormatter(theDocument.get(), 0);

                // Do the transformation...
                int     theResult =
                    theTransformer.transform(
                        *theParsedSource,
                        theCompiledStylesheet,
                        theFormatter);
                // NOTE: do the transformation for each transformer

                if (theResult != 0)
                {
                    CString strMsg;
                    strMsg.Format(_T("Error transforming: %s"),theTransformer.getLastError());
                    AfxMessageBox(strMsg);
                    return std::auto_ptr<Curvel>();
                }
                else
                {
                   // Now that theDocument has been converted to the latest format
                   // create the C++ binding
#pragma Reminder("UPDATE: try to make the schema be a resource that is compiled into this dll")
                     xml_schema::properties props;
                     props.no_namespace_schema_location("Curvel_Version_2.0.xsd");
                     curvelXML = Curvel_(*theDocument.get()/*lpszPathName*/,0,props);
                }
           }
      }
      }
      XalanTransformer::terminate();
      XMLPlatformUtils::Terminate();
   }
   catch(const xml_schema::exception& e)
   {
#pragma Reminder("UPDATE: provide better error handling")
      AfxMessageBox(_T("An error occured while loading the file"));
      return std::auto_ptr<Curvel>();
   }
   catch(...)
   {
#pragma Reminder("UPDATE: provide better error handling")
      AfxMessageBox(_T("An error occured while loading the file"));
      return std::auto_ptr<Curvel>();
   }

   if ( !ConvertToBaseUnits(curvelXML.get(),pDocUnitConvert) )
   {
      // something went wrong in the unit conversion
#pragma Reminder("UPDATE: provide better error handling")
      AfxMessageBox(_T("An error occured while loading the file"));
      return std::auto_ptr<Curvel>();
   }

   return curvelXML;
}

BOOL SaveCurvelModel(LPCTSTR lpszPathName,Curvel* pCurvelXML)
{
   try
   {
      std::ofstream file(lpszPathName);
#pragma Reminder("UPDATE: error handle file opening")
      Curvel_(file,*pCurvelXML);
   }
   catch(const xml_schema::exception& e)
   {
#pragma Reminder("UPDATE: provide better error handling")
      AfxMessageBox(_T("An error occured while saving the file"));
      return FALSE;
   }
   return TRUE;
}