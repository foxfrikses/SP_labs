#pragma once

#include <windows.h>
#include <memory>

#include "ModifiableBoard.h"
#include "PaintTools.h"
#include "FiguresPrinter.h"

class FiguresPrintingLogic final
{
  std::shared_ptr<ModifiableBoard> __board;
  std::unique_ptr<FiguresPrinter> __printer;

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
  HWND CreateButton(const Position& pos, LPCWSTR name, int id) const;

  void OnRunClicked();
  void OnStopClicked();

private:
  HWND __hRunWnd = NULL; 
  HWND __hStopWnd = NULL; 

  HWND __hwnd;
  HINSTANCE __hInst;
};
