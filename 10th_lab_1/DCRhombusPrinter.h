#pragma once

#include "IDCPrinter.h"

#include <windows.h>

class DCRhombusPrinter final
  : public IDCPrinter
{
public:
  void Print(HDC hdc) override;
};
