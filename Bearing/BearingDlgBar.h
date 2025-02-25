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

#pragma once
#include <EngTools\Bearing.h>
#include <EngTools\BearingLoads.h>
#include <EngTools\BearingCalculator.h>
#include "BearingDoc.h"
#include <MfcTools\MetaFileStatic.h>





// CBearingDialogBar
class CBearingDialogBar : public CDialogBar
{
	DECLARE_DYNAMIC(CBearingDialogBar)

public:
	CBearingDialogBar();
	virtual ~CBearingDialogBar();

	void SetBearingParameters(CBearingDialogBar&,
		const WBFL::EngTools::Bearing&,
		const WBFL::EngTools::BearingLoads&,
		const WBFL::EngTools::BearingCalculator&);

	void SetBearing(
		WBFL::EngTools::Bearing& brg,
		WBFL::EngTools::BearingLoads& brg_loads,
		WBFL::EngTools::BearingCalculator& brg_calc);

private:

	int m_method;
	Float64 m_length;
	Float64 m_width;
	Float64 m_cover;
	Float64 m_layer;
	Float64 m_shim;
	IndexType m_n_layers;
	Float64 m_Gmin;
	Float64 m_Gmax;
	Float64 m_Fy;
	Float64 m_Fth;
	Float64 m_DL;
	Float64 m_LL;
	Float64 m_rot_st;
	Float64 m_rot_cy;
	Float64 m_rot_x;
	Float64 m_rot_y;
	Float64 m_shear_def;
	bool m_fixed_x;
	bool m_fixed_y;
	bool m_ext_plates;
	WBFL::LRFD::BDSManager::Edition m_specification{WBFL::LRFD::BDSManager::Edition::LastEdition};



protected:
	DECLARE_MESSAGE_MAP()

#if defined _DEBUG
   void AssertValid() const;
#endif
   
   virtual void DoDataExchange(CDataExchange* pDX) override;

public:
   virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID) override;

};
