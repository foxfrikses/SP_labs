#pragma once

#include "ModifiableBoard.h"
#include <memory>
#include <array>

class FiguresPrinter
{
public:
  FiguresPrinter(std::shared_ptr<ModifiableBoard>);
  ~FiguresPrinter();

  void Run();
  void Stop();

private:
  inline static const int THREAD_COUNT = 8;
 
  bool __isRunning = false;
  std::shared_ptr<ModifiableBoard> __board;

  struct ThreadData
  {
    std::shared_ptr<ModifiableBoard> board;
    std::atomic_flag stop;
  };
  ThreadData* __threadDataArray;
  std::array<DWORD, THREAD_COUNT>  __threadIdArray;
  std::array<HANDLE, THREAD_COUNT> __threadArray;
 
  friend DWORD WINAPI ThreadFunction( LPVOID lpParam );
;
};
