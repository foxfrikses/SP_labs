#pragma once

#include <windows.h>
#include <string>
#include "common.h"

class WinMutexMutex final
{
  Handle __hMutex;

public:
  WinMutexMutex();

  void lock();
  void unlock();
};
