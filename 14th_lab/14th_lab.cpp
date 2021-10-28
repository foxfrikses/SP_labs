#include "RegistryEditorWindow.h"
#include "RegistryEditorWindowClass.h"

#include <memory>

int WINAPI WinMain(
  HINSTANCE hInstance, 
  HINSTANCE hPrevInstance, 
  LPSTR lpCmdLine, 
  int nCmdShow
) 
{
  auto w = RegistryEditorWindow(
    std::make_unique<RegistryEditorWindowClass>(
      hInstance
    ), 
    nCmdShow
  );
  return w.Run();
}
