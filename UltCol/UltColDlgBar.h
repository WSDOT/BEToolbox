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

#pragma once

// CUltColDlgBar
class CUltColDlgBar : public CDialogBar
{
	DECLARE_DYNAMIC(CUltColDlgBar)

public:
	CUltColDlgBar();
	virtual ~CUltColDlgBar();

   Float64 m_Diameter;
   Float64 m_Cover;
   Float64 m_As;
   Float64 m_Es;
   Float64 m_Fy;
   Float64 m_Fc;
   Float64 m_ecl;
   Float64 m_etl;

protected:
	DECLARE_MESSAGE_MAP()
   virtual void DoDataExchange(CDataExchange* pDX) override;
};
