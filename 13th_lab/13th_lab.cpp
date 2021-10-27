#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <string>

#include "IWindow.h"
#include "PriorityManagerWindow.h"
#include "PriorityManagerWindowClass.h"

int WINAPI WinMain(
  HINSTANCE hInstance, 
  HINSTANCE hPrevInstance, 
  LPSTR lpCmdLine, 
  int nCmdShow
) 
{
  auto w = PriorityManagerWindow(
    std::make_unique<PriorityManagerWindowClass>(
      hInstance
    ), 
    nCmdShow
  );
  return w.Run();
}