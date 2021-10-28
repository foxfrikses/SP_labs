#pragma once

#include <windows.h>
#include <memory>
#include <vector>
#include <string>
#include <optional>

class RegistryEditorLogic final
{
public:
  explicit RegistryEditorLogic(HWND hwnd, HINSTANCE hinst);

  RegistryEditorLogic(const RegistryEditorLogic&) = delete;
  RegistryEditorLogic& operator =(const RegistryEditorLogic&) = delete;
  RegistryEditorLogic(RegistryEditorLogic&&) = default;
  RegistryEditorLogic& operator =(RegistryEditorLogic&&) = default;

  void Command(WPARAM wparam, LPARAM lparam);
  void ContextMenu(HWND hwnd, int x, int y);
  void Resize(int width, int height);
  void Print();

private:
  HWND __hwnd;
  HINSTANCE __hInst;
};