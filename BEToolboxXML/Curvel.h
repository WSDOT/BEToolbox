///////////////////////////////////////////////////////////////////////
// BEToolboxXML
// Copyright © 1999-2017  Washington State Department of Transportation
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

#include <BEToolboxXML.h>
#include <Curvel_Version_2.0.hxx>
#include <WBFLUnitServer.h>

// Creates a Curvel data model. If lpszFileName is NULL, a default
// model is created, otherwise the model is created from the
// curvel dataset supplied in the specified file.
std::auto_ptr<Curvel> BETOOLBOXXMLFUNC CreateCurvelModel();
std::auto_ptr<Curvel> BETOOLBOXXMLFUNC CreateCurvelModel(LPCTSTR lpszFilePath,IUnitServer* pDocUnitServer);

// Saves a curvel model to the specified file.
BOOL BETOOLBOXXMLFUNC SaveCurvelModel(LPCTSTR lpszFilePath,Curvel* pCurvelXML);