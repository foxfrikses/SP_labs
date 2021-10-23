#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <string>

#include "IWindow.h"
#include "FiguresPrintingWindowClass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  auto w = IWindow(
    std::make_unique<FiguresPrintingWindowClass>(hInstance), 
    nCmdShow
  );
  return w.Run();
}