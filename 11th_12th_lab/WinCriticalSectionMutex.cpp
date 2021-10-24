#include "WinCriticalSectionMutex.h"

WinCriticalSectionMutex::WinCriticalSectionMutex()
{
  InitializeCriticalSection(&__cs);
}

WinCriticalSectionMutex::~WinCriticalSectionMutex()
{
  DeleteCriticalSection(&__cs);
}

void WinCriticalSectionMutex::lock()
{
  EnterCriticalSection(&__cs);
}

void WinCriticalSectionMutex::unlock()
{
  LeaveCriticalSection(&__cs);
}
