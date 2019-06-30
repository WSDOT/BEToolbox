///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2019  Washington State Department of Transportation
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

#include "stdafx.h"
#include "Helpers.h"
#include <thread>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void GetThreadParameters(IndexType nElements, IndexType& nWorkerThreads, IndexType& nElementsPerThread)
{
   IndexType min_per_thread = 25;
   IndexType max_threads = (nElements + min_per_thread - 1) / min_per_thread;
   IndexType hardware_threads = std::thread::hardware_concurrency();
   IndexType nThreads = min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
   nWorkerThreads = nThreads - 1;
   nElementsPerThread = nElements / nThreads;

   // this makes everything run in the main thread
   nWorkerThreads = 0;
   nElementsPerThread = nElements;
}
