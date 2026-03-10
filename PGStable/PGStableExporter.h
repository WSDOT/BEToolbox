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

// PGStableExporter.h : Declaration of the CPGStableExporter

#pragma once

#include <Plugins\PGSuperIEPlugin.h>
#include <EAF\ComponentObject.h>
#include "..\BEToolboxCLSID.h"

class CPGStableModel;

/////////////////////////////////////////////////////////////////////////////
// CPGStableExporter
class CPGStableExporter : public WBFL::EAF::ComponentObject,
   public PGS::IDataExporter
{
public:
   CPGStableExporter();

   CBitmap m_Bitmap;

// IDataExporter
public:
   HRESULT Init(UINT nCmdID) override;
   CString GetMenuText() const override;
   HBITMAP GetBitmapHandle() const override;
   CString GetCommandHintText() const override;
   HRESULT Export(std::shared_ptr<WBFL::EAF::Broker> pBroker) override;

protected:
   bool ConfigureModel(std::shared_ptr<WBFL::EAF::Broker> pBroker,const CSegmentKey& segmentKey,CPGStableModel& model);
};
