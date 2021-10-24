#pragma once

#include <windows.h>
#include <memory>

#include "IModifiableBoard.h"
#include "PaintTools.h"
#include "FiguresPrinter.h"

#include "WinCriticalSectionMutex.h"
#include "WinSemaphoreMutex.h"
#include "WinMutexMutex.h"

enum class MutexType
{
  stdMutex,
  winMutex,
  winSemaphore,
  winCriticalSection
};

class FiguresPrintingLogic final
{
  std::shared_ptr<IModifiableBoard> __board;
  std::unique_ptr<FiguresPrinter> __printer;

  MutexType __mutexType = MutexType::stdMutex;

public:
  explicit FiguresPrintingLogic(HWND hwnd, HINSTANCE hinst);
  ~FiguresPrintingLogic();
 
  FiguresPrintingLogic(const FiguresPrintingLogic&) = delete;
  FiguresPrintingLogic& operator =(const FiguresPrintingLogic&) = delete;
  FiguresPrintingLogic(FiguresPrintingLogic&&);
  FiguresPrintingLogic& operator =(FiguresPrintingLogic&&);

  void Command(WPARAM wparam, LPARAM lparam);
  void Timer(WPARAM wparam);
  void Resize(int width, int height);
  void Print();

private:
  void SetMutexType(MutexType type);

  void PrintPicture();

  HWND CreateButton(
    const Position& pos, 
    LPCWSTR name, 
    int id
  ) const;

  HWND CreateRadioButton(
    const Position &pos, 
    LPCWSTR name, 
    int id, 
    bool isFirstInTheGroup
  ) const;

  void OnRunClicked();
  void OnSuspendClicked();
  void OnStopClicked();

private:
  HWND __hRunWnd = NULL; 
  HWND __hStopWnd = NULL; 
  HWND __hSuspendWnd = NULL; 

  HWND __hStdWnd = NULL;
  HWND __hMutWnd = NULL;
  HWND __hSemWnd = NULL;
  HWND __hCrsWnd = NULL;

  HWND __hwnd;
  HINSTANCE __hInst;
};
