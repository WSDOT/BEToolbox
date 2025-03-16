///////////////////////////////////////////////////////////////////////
// PGSuper - Prestressed Girder SUPERstructure Design and Analysis
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
// SYSTEM INCLUDES
//
#include "..\BEToolboxLib.h"

//#include <WBFLCore.h>

#if !defined INCLUDED_MATHEX_H_
#include <MathEx.h>
#endif

// PROJECT INCLUDES
//

#include <StrData.h>
#include <Materials/Rebar.h>

// LOCAL INCLUDES
//

// FORWARD DECLARATIONS
//
class GirderLibraryEntry;

// MISCELLANEOUS
//

/*****************************************************************************
CLASS 
   CPGStableLongitudinalRebarData

   Utility class for longitudinal rebar description data.

DESCRIPTION
   Utility class for longitudinal rebar. This class encapsulates 
   the input data a row of rebar and implements the IStructuredLoad 
   and IStructuredSave persistence interfaces.

LOG
   rdp : 02.10.2025 : Created file
*****************************************************************************/

class  CPGStableLongitudinalRebarData
{
public:
   class RebarRow
   {
   public:

      pgsTypes::FaceType  Face;
      WBFL::Materials::Rebar::Size BarSize;
      IndexType NumberOfBars;
      Float64     Cover;
      Float64     BarSpacing;

      RebarRow():
         Face(pgsTypes::TopFace), BarSize(WBFL::Materials::Rebar::Size::bsNone), NumberOfBars(0), Cover(0), BarSpacing(0)
      {;}

      bool operator==(const RebarRow& other) const;

   };

   WBFL::Materials::Rebar::Type BarType;
   WBFL::Materials::Rebar::Grade BarGrade;
   std::vector<RebarRow> RebarRows;

   Float64 MaxCoverToUseHigherTensionStressLimit = 0;

   // GROUP: LIFECYCLE

   //------------------------------------------------------------------------
   // Constructor
   CPGStableLongitudinalRebarData();

   //------------------------------------------------------------------------
   // Copy constructor
   CPGStableLongitudinalRebarData(const CPGStableLongitudinalRebarData& rOther);

   //------------------------------------------------------------------------
   // Destructor
   ~CPGStableLongitudinalRebarData();

   // GROUP: OPERATORS
   //------------------------------------------------------------------------
   // Assignment operator
   CPGStableLongitudinalRebarData& operator = (const CPGStableLongitudinalRebarData& rOther);
   bool operator == (const CPGStableLongitudinalRebarData& rOther) const;
   bool operator != (const CPGStableLongitudinalRebarData& rOther) const;

   // GROUP: OPERATIONS

	HRESULT Load(IStructuredLoad* pStrLoad);
	HRESULT Save(IStructuredSave* pStrSave);

   // GROUP: ACCESS
   // GROUP: INQUIRY

#if defined _DEBUG
   void AssertValid();
#endif

protected:
   // GROUP: DATA MEMBERS
   // GROUP: LIFECYCLE
   // GROUP: OPERATORS
   // GROUP: OPERATIONS
   //------------------------------------------------------------------------
   void MakeCopy(const CPGStableLongitudinalRebarData& rOther);

   //------------------------------------------------------------------------
   void MakeAssignment(const CPGStableLongitudinalRebarData& rOther);

   // GROUP: ACCESS
   // GROUP: INQUIRY

private:
   // GROUP: DATA MEMBERS
   // GROUP: LIFECYCLE
   // GROUP: OPERATORS
   // GROUP: OPERATIONS
   // GROUP: ACCESS
   // GROUP: INQUIRY
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

