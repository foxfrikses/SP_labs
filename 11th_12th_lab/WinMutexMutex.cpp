#include "WinMutexMutex.h"

#include <stdexcept>

WinMutexMutex::WinMutexMutex()
  : __hMutex(CreateMutex(NULL, FALSE, NULL))
{
  if (!__hMutex) {
    throw std::runtime_error(
      "WinMutexMutex cannot be opened"
    );
  }
}

void WinMutexMutex::lock()
{
  WaitForSingleObject(__hMutex, INFINITE);
}

void WinMutexMutex::unlock() 
{
  ReleaseMutex(__hMutex);
}
