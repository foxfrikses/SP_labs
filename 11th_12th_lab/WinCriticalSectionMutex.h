#pragma once

#include <windows.h>

class WinCriticalSectionMutex final
{
  CRITICAL_SECTION __cs;

public:
  WinCriticalSectionMutex();
  ~WinCriticalSectionMutex();

  void lock();
  void unlock();

  WinCriticalSectionMutex(WinCriticalSectionMutex&&) = delete;
  WinCriticalSectionMutex(const WinCriticalSectionMutex&) = delete;
  WinCriticalSectionMutex& operator=(WinCriticalSectionMutex&&) = delete;
  WinCriticalSectionMutex& operator=(const WinCriticalSectionMutex&) = delete;
};
