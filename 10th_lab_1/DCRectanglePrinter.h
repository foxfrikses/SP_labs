#pragma once

#include "IDCPrinter.h"

#include <windows.h>

class DCRectanglePrinter final
  : public IDCPrinter
{
public:
  void Print(HDC hdc) override;
};
