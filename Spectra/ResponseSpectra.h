///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2023  Washington State Department of Transportation
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

#pragma once

#define SPECTRA_OK               0
#define SPECTRA_INVALID_LOCATION 1
#define SPECTRA_SITE_SPECIFIC    2

class CResponseSpectra
{
public:
   CResponseSpectra();
   CResponseSpectra(const CResponseSpectra& other);
   CResponseSpectra& operator=(const CResponseSpectra& other);

   void Init(Float64 PGA,Float64 Ss,Float64 S1,Float64 Fpga,Float64 Fa,Float64 Fv);

   Float64 GetPGA() const;
   Float64 GetSs() const;
   Float64 GetS1() const;
   Float64 GetFpga() const;
   Float64 GetFa() const;
   Float64 GetFv() const;
   Float64 GetTo() const;
   Float64 GetTs() const;
   Float64 GetAs() const;
   Float64 GetSds() const;
   Float64 GetSd1() const;
   LPCTSTR GetSDC(SpecificationType specType) const;
   Float64 GetSa(Float64 time) const;
   std::vector<std::pair<Float64,Float64>> GetSpectraValues(Float64 Tmax,Float64 tStep);

protected:
   Float64 m_PGA, m_Ss, m_S1;
   Float64 m_Fpga, m_Fa, m_Fv;
};
