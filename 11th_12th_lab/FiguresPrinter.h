#pragma once

#include "IModifiableBoard.h"
#include <memory>
#include <array>

#include "FiguresPrinterThreadFunction.h"

class FiguresPrinter final
{
public:
  explicit FiguresPrinter(std::shared_ptr<IModifiableBoard>);
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
  } __state = State::stopped;
 
  std::shared_ptr<IModifiableBoard> __board = nullptr;

  figures_printer::ThreadData* __threadDataArray = nullptr;
  std::array<DWORD, THREAD_COUNT>  __threadIdArray = {0};
  std::array<HANDLE, THREAD_COUNT> __threadArray = {0};
};
