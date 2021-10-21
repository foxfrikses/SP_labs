#pragma once

#include <windows.h>

class PrintBoard final 
{
  HDC __targetDC;
  HDC __hdc;

  HBITMAP __hBmp = nullptr;
  int __width = 0;
  int __height = 0;

public:
  explicit PrintBoard(HDC targetDC, UINT width, UINT height);
  PrintBoard(const PrintBoard&) = delete;
  PrintBoard& operator =(const PrintBoard&) = delete;
  PrintBoard(PrintBoard&&);
  PrintBoard& operator =(PrintBoard&&);
  ~PrintBoard();

  PrintBoard& SetSize(UINT width, UINT height);

  operator HDC() const {return __hdc;}

  UINT Width() const {return __width;}
  UINT Height() const {return __height;}
};
