#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <string>

#include "MovingLineWindowClass.h"
#include "MainWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  auto wc = std::make_unique<MovingLineWindowClass>(hInstance);
  auto w = MainWindow(std::move(wc), nCmdShow);
  return w.Run();
}
