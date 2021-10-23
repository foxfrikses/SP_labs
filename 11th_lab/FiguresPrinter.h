#pragma once

#include "ModifiableBoard.h"
#include <memory>
#include <array>

#include "FiguresPrinterThreadFunction.h"

class FiguresPrinter
{
public:
  FiguresPrinter(std::shared_ptr<ModifiableBoard>);
  ~FiguresPrinter();

  void Run();
  void Suspend();
  void Stop();

private:
  inline static const int THREAD_COUNT = 8;

  enum class State
  {
    stopped,
    running,
    suspended
  } __state;
 
  std::shared_ptr<ModifiableBoard> __board;

  figures_printer::ThreadData* __threadDataArray;
  std::array<DWORD, THREAD_COUNT>  __threadIdArray;
  std::array<HANDLE, THREAD_COUNT> __threadArray;
};
