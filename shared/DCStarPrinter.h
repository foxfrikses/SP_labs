#pragma once

#include "IDCPrinter.h"

#include <windows.h>

class DCStarPrinter final
  : public IDCPrinter
{
public:
  void Print(HDC hdc) override;
};
