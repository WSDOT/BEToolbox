///////////////////////////////////////////////////////////////////////
// SpectraBuilder
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

// SpectraBuilder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SpectraBuilder.h"
#include "SpectraValues.h"
#include <System\Tokenizer.h>
#include <MathEx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

typedef enum ValueType
{
   c1hz,
   c5hz,
   cpga
} ValueType;

void LoadValues(LPCTSTR fileName,ValueType valueType,SpectralValues* values)
{
   std::_tcout << _T("Reading spectral values from ") << fileName << std::endl;

   std::_tifstream ifile(fileName);
   const TCHAR comment = _T('#');
   int i = 0;
   for ( std::_tstring string; std::getline(ifile,string); )
   {
      if ( string[0] == comment )
         continue;

      sysTokenizer tokenizer(_T(" "));
      tokenizer.push_back(string.c_str());
      ATLASSERT(tokenizer.size() == 3);
      std::_tstring token;

      Float64 lng, lat, value;
      token = tokenizer[0];
      sysTokenizer::ParseDouble(token.c_str(),&lng);

      token = tokenizer[1];
      sysTokenizer::ParseDouble(token.c_str(),&lat);

      token = tokenizer[2];
      sysTokenizer::ParseDouble(token.c_str(),&value);

      if ( valueType == c1hz )
      {
         SpectralValues v;
         v.lng = lng;
         v.lat = lat;
         v.v1hz = value;
         values[i++] = v;
      }
      else if ( valueType == c5hz )
      {
         ATLASSERT(IsEqual(values[i].lng,lng));
         ATLASSERT(IsEqual(values[i].lat,lat));
         values[i++].v5hz = value;
      }
      else
      {
         ATLASSERT(valueType == cpga);
         ATLASSERT(IsEqual(values[i].lng,lng));
         ATLASSERT(IsEqual(values[i].lat,lat));
         values[i++].vpga = value;
      }
   }
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
	}

   Float64 minLat = MIN_LATITUDE;
   Float64 maxLat = MAX_LATITUDE;
   Float64 incLat = INC_LATITUDE;
   
   Float64 minLng = MIN_LONGITUDE;
   Float64 maxLng = MAX_LONGITUDE;
   Float64 incLng = INC_LONGITUDE;

   SpectralValues* values = new SpectralValues[SPECTRAL_VALUE_COUNT];

   LoadValues(_T("\\ARP\\BEToolbox\\USGS\\2014\\us_hazard.1hz.7pc75"),c1hz,values);
   LoadValues(_T("\\ARP\\BEToolbox\\USGS\\2014\\us_hazard.5hz.7pc75"),c5hz,values);
   LoadValues(_T("\\ARP\\BEToolbox\\USGS\\2014\\us_hazard.pga.7pc75"),cpga,values);

   std::_tcout << _T("Saving spectral values in binary format") << std::endl;

   std::ofstream ofile(_T("\\ARP\\BEToolbox\\USGS\\2014\\us_hazard_7pc75.2014.bin"),std::ios::binary);
   int size = SPECTRAL_VALUE_COUNT*sizeof(SpectralValues);
   ofile.write(reinterpret_cast<const char*>(values),size);
   ofile.close();

   delete[] values;

   std::_tcout << _T("Done!") << std::endl;

   return nRetCode;
}
