#pragma once

#include <windows.h>

#include <atomic>
#include <memory>

#include "ModifiableBoard.h"

namespace figures_printer
{
  DWORD WINAPI ThreadFunction( LPVOID lpParam );

  struct ThreadData
  {
    std::shared_ptr<ModifiableBoard> board;
    std::atomic_flag stop;
  };
}
