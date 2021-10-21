#pragma once

#include <windows.h>
 
#include "PaintTools.h"

class SimplePicPrinter {
  enum {
    drawMode,
    clearMode 
  } __mode;

  HWND __hwnd;
  HINSTANCE __hInst;

  HWND __drawButtonHwnd;
  HWND __clearButtonHwnd;

public:
  explicit SimplePicPrinter(HWND hwnd, HINSTANCE hinst);

  void Command(WPARAM wparam, LPARAM lparam);
  void Resize(int width, int height);
  void DrawItem(LPARAM lparam);
  void Print();

private:
  HWND CreateButton(const Position& pos, LPCWSTR name, int id) const;
};
