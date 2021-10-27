#pragma once

#include <windows.h>

class Handle final
{
  HANDLE __handle;

public:
  explicit Handle(HANDLE handle);
  ~Handle();

  operator HANDLE() const;
 
  Handle(Handle&&) noexcept;
  Handle& operator=(Handle&&) noexcept;

  Handle(const Handle&) = delete;
  Handle& operator=(const Handle&) = delete;
};

class Menu final
{
  HMENU __menu;

public:
  explicit Menu(HMENU menu);
  ~Menu();

  operator HMENU() const;
 
  Menu(Menu&&) noexcept;
  Menu& operator=(Menu&&) noexcept;

  Menu(const Menu&) = delete;
  Menu& operator=(const Menu&) = delete;
};

