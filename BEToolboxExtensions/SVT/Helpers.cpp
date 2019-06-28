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
