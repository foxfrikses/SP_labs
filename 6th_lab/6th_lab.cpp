#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <string>

#include "MovingLineWindowClass.h"
#include "IWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  auto w = IWindow(std::make_unique<MovingLineWindowClass>(hInstance), 
                   nCmdShow);
  return w.Run();
}
