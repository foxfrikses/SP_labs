#include "PrintBoard.h"

#include <stdexcept>

PrintBoard::PrintBoard(HDC targetDC, UINT width, UINT height)
  : __hdc(CreateCompatibleDC(targetDC))
  , __targetDC(targetDC)
{
  if (!__hdc) {
    throw std::runtime_error("PrintBoard DC not initialized");
  }
  SetSize(width, height);
}

PrintBoard::PrintBoard(PrintBoard&& nb)
  : __hdc(NULL)
  , __hBmp(NULL)
  , __width(0)
  , __height(0)
  , __targetDC(NULL)
{
  *this = std::move(nb);
}

PrintBoard& PrintBoard::operator =(PrintBoard&& nb)
{
  std::swap(__targetDC, nb.__targetDC);
  std::swap(__hdc, nb.__hdc);
  std::swap(__hBmp, nb.__hBmp);
  std::swap(__width, nb.__width);
  std::swap(__height, nb.__height);

  return *this;
}

PrintBoard::~PrintBoard() 
{
  if (__hdc) {
    DeleteDC(__hdc);
  }
  if (__hBmp) {
    DeleteObject(__hBmp);
  }
}

PrintBoard& PrintBoard::SetSize(UINT width, UINT height) {

  if (width == __width && height == __height) {
    return *this;
  }

  auto hNewBmp = CreateCompatibleBitmap(__targetDC, width, height);

  if (!hNewBmp) {
    throw std::runtime_error("PrintBoard Bitmap not initialized");
  }

  if (__hBmp) {
    auto hTempDC = CreateCompatibleDC(__targetDC);
    if (!hTempDC) {
      throw std::runtime_error("PrintBoard TempDC not initialized");
    }
    SelectObject(hTempDC, hNewBmp);
    BitBlt(hTempDC, 0, 0, __width, __height, __hdc, 0, 0, SRCCOPY);

    DeleteDC(__hdc);
    DeleteObject(__hBmp);

    __hdc = hTempDC;
    __hBmp = hNewBmp;
  }
  else {
    __hBmp = hNewBmp;
    DeleteObject(SelectObject(__hdc, __hBmp));
  }

  __width = width;
  __height = height;
}


