#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <string>

#include "ListBoxesWindowClass.h"
#include "../shared/IWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  auto w = IWindow(std::make_unique<ListBoxesWindowClass>(hInstance), 
                   nCmdShow);
  return w.Run();
}
