#pragma once

#include <windows.h>
#include "common.h"

class WinEvent
{
  Handle __hEvent;

public:
  explicit WinEvent();
 
  bool Wait();
  bool Signal();
};
