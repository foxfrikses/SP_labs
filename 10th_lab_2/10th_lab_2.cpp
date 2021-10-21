#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <string>

#include "ToolSelectorWindowClass.h"
#include "ToolSelectorWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  auto w = ToolSelectorWindow(
    std::make_unique<ToolSelectorWindowClass>(hInstance), 
    nCmdShow
  );
  return w.Run();
}
