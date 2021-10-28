#include "WinSemaphoreMutex.h"

#include <stdexcept>

WinSemaphoreMutex::WinSemaphoreMutex() 
  : __hSemaphore(CreateSemaphore(NULL, 1, 1, NULL))
{
  if (!__hSemaphore) {
    throw std::runtime_error(
      "WinSemaphoreMutex cannot be opened"
    );
  }
}

void WinSemaphoreMutex::lock()
{
  WaitForSingleObject(__hSemaphore, INFINITE);
}

void WinSemaphoreMutex::unlock()
{
  ReleaseSemaphore(__hSemaphore, 1, NULL);
}
