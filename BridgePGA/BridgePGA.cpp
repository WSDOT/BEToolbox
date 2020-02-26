// BridgePGA.cpp : Defines the entry point for the console application.
//

// This is a "one time" utility application to update PGA values for WSDOT's bridge inventory
// The file BridgeLocations.csv contains the location of every bridge in our inventory (as of 1/3/2017).
// This file is read, the lat/lng is used to get the PGA from the USGS 2014 Seismic Hazard Map,
// and then bridge information, location, and PGA is stored into PGA.csv
// PGA.csv can then be converted to an Excel file and used to update the inventory information

#include "stdafx.h"
#include "..\SpectraBuilder\SpectraValues.h"
#include <System\Tokenizer.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// The one and only application object

CWinApp theApp;

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

   ::CoInitialize(0);

   // Load the spectral values in binary format
   SpectralValues* pValues = new SpectralValues[SPECTRAL_VALUE_COUNT];
   std::_tstring strResourceFile = _T("\\ARP\\BEToolbox\\USGS\\2014\\us_hazard_7pc75.2014.bin");
   std::ifstream resourceFile(strResourceFile.c_str(),std::ios::binary);
   if ( resourceFile.bad() || resourceFile.fail() )
   {
      std::_tcout << _T("Failed to load seismic hazard map data.");
      return -1;
   }
   resourceFile.read(reinterpret_cast<char*>(pValues),SPECTRAL_VALUE_COUNT*sizeof(SpectralValues));
   resourceFile.close();

   // Read bridge location information

   std::_tifstream ifile(_T("BridgeLocations.csv"));
   if ( ifile.bad() || ifile.fail() )
   {
      std::_tcout << _T("Failed to load bridge location data.");
      return -1;
   }

   std::_tofstream ofile(_T("PGA.csv"));
   ofile << _T("structure_id,bridge_no,latitude,longitude,pga") << std::endl;
   int i = 0;
   for ( std::_tstring line; std::getline(ifile,line); i++ )
   {
      if ( i == 0 )
         continue; // skipper the header row

      sysTokenizer tokenizer(_T(","));
      tokenizer.push_back(line.c_str());

      std::_tstring sid, bridge;
      Float64 lat, lng;

      std::_tstring token;

      token = tokenizer[0];
      sid = token.c_str();

      token = tokenizer[1];
      bridge = token.c_str();

      token = tokenizer[2];
      sysTokenizer::ParseDouble(token.c_str(),&lat);

      token = tokenizer[3];
      sysTokenizer::ParseDouble(token.c_str(),&lng);

      Float64 S1, Ss, PGA;
      GetSpectralValues(lat,-lng,pValues,&S1,&Ss,&PGA); // lng is given as + value, but we need it to be - because they are West values

      ofile << sid << _T(",") << bridge << _T(",") << lat << _T(",") << lng << _T(",") << PGA << std::endl;
   }

   ifile.close();
   ofile.close();

   // Clean up
   delete[] pValues;

   return 0;
}

