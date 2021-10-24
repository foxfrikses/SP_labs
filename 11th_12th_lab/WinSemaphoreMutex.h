#pragma once

#include <windows.h>
#include <string>

class WinSemaphoreMutex final
{
  HANDLE __hSemaphore;

public:
  explicit WinSemaphoreMutex();
  ~WinSemaphoreMutex();

  void lock();
  void unlock();

  WinSemaphoreMutex(WinSemaphoreMutex&&) = delete;
  WinSemaphoreMutex(const WinSemaphoreMutex&) = delete;
  WinSemaphoreMutex& operator=(WinSemaphoreMutex&&) = delete;
  WinSemaphoreMutex& operator=(const WinSemaphoreMutex&) = delete;
};
