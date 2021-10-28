#pragma once

#include <windows.h>
#include <string>
#include "common.h"

class WinSemaphoreMutex final
{
  Handle __hSemaphore;

public:
  WinSemaphoreMutex();

  void lock();
  void unlock();
};
