///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright � 1999-2024  Washington State Department of Transportation
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

#include <MfcTools\MetaFileStatic.h>
#include "BoxGdrDimensionGrid.h"

// CBoxGdrDialogBar
class CBoxGdrDialogBar : public CDialogBar
{
	DECLARE_DYNAMIC(CBoxGdrDialogBar)

public:
	CBoxGdrDialogBar();
	virtual ~CBoxGdrDialogBar();

   virtual BOOL Create(CWnd* pParentWnd,UINT nIDTemplate,UINT nStyle,UINT nID) override;

   void AddProblem();
   void RemoveSelectedProblems();
   BOOL AreProblemsSelected();

   void OnUnitsModeChanged();

protected:
	DECLARE_MESSAGE_MAP()

#if defined _DEBUG
   void AssertValid() const;
#endif

   virtual void DoDataExchange(CDataExchange* pDX) override;
   void InitGrid();

   CBoxGdrDimensionGrid* m_pGrid;
   CMetaFileStatic m_Schematic;
};
