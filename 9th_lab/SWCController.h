#pragma once

#include <windows.h>

#include <array>
 
#include "../shared/common.h"

class SWCController final {
  HWND __hwnd;
  HINSTANCE __hInst;

  std::array<HBITMAP, 4> __btmps;
  size_t __curBtmp = 0;
  const int __btmpWidth = 40;
  const int __btmpHeight = 40;

  const int __D = 5;
  const int __timerPeriod = 16; // ms

  struct FloatPoint{double x, y;};
  FloatPoint __aimPoint;
  FloatPoint __curPoint;

public:
  explicit SWCController(HWND hwnd, HINSTANCE hinst);
  ~SWCController();

  void Timer(WPARAM wparam, LPARAM lparam);
  void Resize(int width, int height);
  void Print();
  void SetAimPoint(int x, int y);

private:
  void Move();
};
