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

#pragma once

#if defined _EAF_USING_MFC_FEATURE_PACK
#include <EAF\EAFPaneDialog.h>
#else
#define CEAFPaneDialog CDialogBar
#endif

#include <MfcTools\MetaFileStatic.h>
#include "GenCompDimensionGrid.h"

// CGenCompDialogBar
class CGenCompDialogBar : public CEAFPaneDialog
{
	DECLARE_DYNAMIC(CGenCompDialogBar)

public:
	CGenCompDialogBar();
	virtual ~CGenCompDialogBar();

   virtual BOOL Create(CWnd* pParentWnd,UINT nIDTemplate,UINT nStyle,UINT nID);

   void AddPrimaryPoint();
   void RemoveSelectedPrimaryPoints();
   BOOL ArePrimaryPointsSelected();

   void AddSecondaryPoint();
   void RemoveSelectedSecondaryPoints();
   BOOL AreSecondaryPointsSelected();

   void OnUnitsModeChanged();

   Float64 m_ModularRatio;

protected:
	DECLARE_MESSAGE_MAP()

#if defined _DEBUG
   void AssertValid() const;
#endif

   virtual void DoDataExchange(CDataExchange* pDX);
   void InitGrids();

   CGenCompDimensionGrid* m_pPrimaryGrid;
   CGenCompDimensionGrid* m_pSecondaryGrid;
};
