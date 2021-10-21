#pragma once

#include "IDCPrinter.h"

#include <windows.h>

class DCEllipsePrinter final
  : public IDCPrinter
{
public:
  void Print(HDC hdc) override;
};
