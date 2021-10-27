#include "common.h"

#include <utility>

Handle::Handle(HANDLE handle)
  : __handle(handle)
{}

Handle::~Handle()
{
  if (__handle)
    CloseHandle(__handle);
}

Handle::operator HANDLE() const
{
  return __handle;
}
 
Handle::Handle(Handle&& newHandle) noexcept
  : __handle(NULL)
{
  *this = std::move(newHandle);
}

Handle& Handle::operator=(Handle&& newHandle) noexcept
{
  std::swap(__handle, newHandle.__handle);
  return *this;
}

Menu::Menu(HMENU menu)
  : __menu(menu)
{}

Menu::~Menu()
{
  if (__menu)
    DestroyMenu(__menu);
}

Menu::operator HMENU() const
{
  return __menu;
}
 
Menu::Menu(Menu&& newMenu) noexcept
  : __menu(NULL)
{
  *this = std::move(newMenu);
}

Menu& Menu::operator=(Menu&& newMenu) noexcept
{
  std::swap(__menu, newMenu.__menu);
  return *this;
}
