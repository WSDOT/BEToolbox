///////////////////////////////////////////////////////////////////////
// BEToolboxXML
// Copyright � 1999-2025  Washington State Department of Transportation
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
#include "GenComp.h"
#include "Helpers.h"
#include <WBFLUnitServer\OpenBridgeML.h>

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


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


XALAN_USING_XALAN(XalanCompiledStylesheet)
XALAN_USING_XALAN(XalanParsedSource)
XALAN_USING_XALAN(XalanTransformer)

std::unique_ptr<GenComp> CreateGenCompModel()
{
   std::unique_ptr<GenComp> genCompXML(std::make_unique<GenComp>(UnitMode::US,1.0,ShapeType(),ShapeType()));
   return genCompXML;
}

BOOL ConvertToBaseUnits(GenComp* pGenComp,IUnitServer* pDocUnitServer)
{
   // Walk the XML data structure and make sure all units of measure are in base units

   // The unit server will usually be an application level object, but
   // I am using a local one here just because this is a test/proof of concept.
   // This represents one technique for applications to deal with units:
   // 1) Load document as is
   // 2) Walk document and convert all units to Consistent Base Units
   // 3) Do all work in consistent base units
   // 4) Save document with consistent base units
   CComPtr<IUnitServer> xmlDocUnitServer;
   HRESULT hr = xmlDocUnitServer.CoCreateInstance(CLSID_UnitServer);
   ATLASSERT(SUCCEEDED(hr));

   GenComp::UnitsDeclaration_optional& unitsDeclaration(pGenComp->UnitsDeclaration());
   if ( unitsDeclaration.present())
   {
      if ( !InitializeWBFLUnitServer(&unitsDeclaration.get(),xmlDocUnitServer) )
      {
         return FALSE;
      }
   }

   // Primary Shape
   ShapeType& primaryShape(pGenComp->PrimaryShape());
   ShapeType::Point_iterator iter(primaryShape.Point().begin());
   ShapeType::Point_iterator end(primaryShape.Point().end());
   for ( ; iter != end; iter++ )
   {
      PointType& point(*iter);
      ConvertBetweenBaseUnits(point.X(), xmlDocUnitServer, pDocUnitServer);
      ConvertBetweenBaseUnits(point.Y(), xmlDocUnitServer, pDocUnitServer);
   }

   // Secondary Shape
   ShapeType& secondaryShape(pGenComp->SecondaryShape());
   iter = secondaryShape.Point().begin();
   end  = secondaryShape.Point().end();
   for ( ; iter != end; iter++ )
   {
      PointType& point(*iter);
      ConvertBetweenBaseUnits(point.X(), xmlDocUnitServer, pDocUnitServer);
      ConvertBetweenBaseUnits(point.Y(), xmlDocUnitServer, pDocUnitServer);
   }

   return TRUE;
}

std::unique_ptr<GenComp> CreateGenCompModel(LPCTSTR lpszFilePath,IUnitServer* pDocUnitServer)
{
   ATLASSERT(lpszFilePath != nullptr);
   ATLASSERT(pDocUnitServer != nullptr);

   // Creating is a two phase process.
   // Step 1 is to run the document through the Xalan XSLT engine,
   // applying XSL Transforms to bring the document model up to
   // the most current version.
   // Step 2 is to run the document through the C++\Tree binding
   // to populate the object model.
   //
   // Most of this code is taken from the TransformToXercesDOM example
   // from the Xalan distribution.
   std::unique_ptr<GenComp> genCompXML;
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
         strMsg.Format(_T("Error parsing source document: %s"),theTransformer.getLastError());
         AfxMessageBox(strMsg);
      }
      else
      {
           assert(theParsedSource != 0);

           const XalanCompiledStylesheet*  theCompiledStylesheet = 0;

           const char* lpstrXSLT = nullptr;
           DWORD size = 0;
#pragma Reminder("UPDATE: add error handling")
           if ( !LoadXMLResource(IDR_GENCOMP_10_TO_20_XSLT,XSLTFILE,size,lpstrXSLT) )
           {
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
              strMsg.Format(_T("Error compiling stylesheet: %s"),theTransformer.getLastError());
              AfxMessageBox(strMsg);
           }
           else
           {
                XALAN_USING_XERCES(DOMDocument)
                XALAN_USING_XERCES(DOMImplementation)
                XALAN_USING_XALAN(FormatterToXercesDOM)
                XALAN_USING_XALAN(XalanAutoPtr)

                // This is the document which we'll build...
                const XalanAutoPtr<DOMDocument>     theDocument(DOMImplementation::getImplementation()->createDocument());
                assert(theDocument.get() != nullptr);

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
                }
                else
                {
                   // Now that theDocument has been converted to the latest format
                   // create the C++ binding
               #pragma Reminder("UPDATE: try to make the schema be a resource that is compiled into this dll")
                     xml_schema::properties props;
                     props.no_namespace_schema_location(_T("GenComp_Version_2.0.xsd"));
                     genCompXML = GenComp_(*theDocument.get()/*lpszPathName*/,0,props);
                }
           }
      }
      }
      XalanTransformer::terminate();
      XMLPlatformUtils::Terminate();
   }
   catch(const xml_schema::exception& /*e*/)
   {
//#pragma Reminder("UPDATE: provide better error handling")
//      AfxMessageBox(_T("An error occured while loading the file"));
      return std::unique_ptr<GenComp>();
   }
   catch(...)
   {
//#pragma Reminder("UPDATE: provide better error handling")
//      AfxMessageBox(_T("An error occured while loading the file"));
      return std::unique_ptr<GenComp>();
   }

   if ( !ConvertToBaseUnits(genCompXML.get(),pDocUnitServer) )
   {
      // something went wrong in the unit conversion
#pragma Reminder("UPDATE: provide better error handling")
      AfxMessageBox(_T("An error occured while loading the file"));
      return std::unique_ptr<GenComp>();
   }

   return genCompXML;
}

BOOL SaveGenCompModel(LPCTSTR lpszPathName,GenComp* pGenCompXML)
{
   try
   {
      std::ofstream file(lpszPathName);
#pragma Reminder("UPDATE: error handle file opening")
      GenComp_(file,*pGenCompXML);
   }
   catch(const xml_schema::exception& /*e*/)
   {
#pragma Reminder("UPDATE: provide better error handling")
      AfxMessageBox(_T("An error occured while saving the file"));
      return FALSE;
   }
   return TRUE;
}