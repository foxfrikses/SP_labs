#include "FiguresPrinter.h"

using namespace figures_printer;

FiguresPrinter::FiguresPrinter(
  std::shared_ptr<IModifiableBoard> board
)
  : __board(std::move(board))
{}
FiguresPrinter::~FiguresPrinter()
{
  Stop();
}

void FiguresPrinter::Run()
{
  switch (__state) {
    case FiguresPrinter::State::stopped:
      {
        __threadDataArray = new ThreadData[THREAD_COUNT];
        for (int i = 0; i < THREAD_COUNT; ++i) {
          __threadDataArray[i].board = __board;
        }

        for (int i = 0; i < THREAD_COUNT; ++i)
        {
          auto& handler = __threadArray[i];
          auto& data = __threadDataArray[i];
          auto& id = __threadIdArray[i];

          handler = CreateThread( 
            NULL,
            0,
            ThreadFunction,
            &data,
            0,
            &id);

          if (handler == NULL) {
            ExitProcess(3);
          }
        } 

        __state = State::running;
      }
      break;
    case FiguresPrinter::State::suspended:
      {
        for (auto& handle : __threadArray) {
          ResumeThread(handle);
        }
        __state = State::running;
      }
      break;
    case FiguresPrinter::State::running:
      break; 
  }
}

void FiguresPrinter::Suspend()
{
  switch (__state) {
    case FiguresPrinter::State::running:
      {
        for (auto& handle : __threadArray) {
          SuspendThread(handle);
        }
        __state = State::suspended;
      }
      break;
    case FiguresPrinter::State::stopped:
      break;
    case FiguresPrinter::State::suspended:
      break;
  }
}

void FiguresPrinter::Stop()
{
  if (__state == FiguresPrinter::State::stopped) {
    return;
  }

  if (__state == FiguresPrinter::State::running ||
      __state == FiguresPrinter::State::suspended) {

    for (int i = 0; i < THREAD_COUNT; ++i) {
      __threadDataArray[i].stop.test_and_set();
    }

    if (__state == FiguresPrinter::State::suspended) {
      for (auto& handle : __threadArray) {
        ResumeThread(handle);
      }
    }

    WaitForMultipleObjects(__threadArray.size(), 
                           __threadArray.data(), 
                           TRUE, 
                           INFINITE);
    for (auto& handle : __threadArray) {
      CloseHandle(handle);
    }

    delete[] __threadDataArray;

    __state = State::stopped; 
  }
}

