#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <string>

#include "IWindow.h"
#include "FiguresPrintingWindowClass.h"
#include "FiguresPrintingWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  auto w = FiguresPrintingWindow(
    std::make_unique<FiguresPrintingWindowClass>(hInstance), 
    nCmdShow
  );
  return w.Run();
}