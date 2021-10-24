#pragma once

#include <windows.h>
#include <string>

class WinMutexMutex final
{
  HANDLE __hMutex;

public:
  explicit WinMutexMutex();
  ~WinMutexMutex();

  void lock();
  void unlock();

  WinMutexMutex(WinMutexMutex&&) = delete;
  WinMutexMutex(const WinMutexMutex&) = delete;
  WinMutexMutex& operator=(WinMutexMutex&&) = delete;
  WinMutexMutex& operator=(const WinMutexMutex&) = delete;
};
