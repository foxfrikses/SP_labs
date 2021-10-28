#pragma once

#include <windows.h>
#include <memory>
#include <string>

#include "WinEvent.h"
 
enum class RegKeyPredefined {
  classesRoot,
  currentConfig,
  currentUser,
  localMachine,
  users
};

class RegKey;
using RegKeyPtr = std::shared_ptr<RegKey>;
using EventPtr = std::shared_ptr<WinEvent>;

class RegKey final
{
public:
  static RegKey CreateKey(RegKeyPtr parent, const std::wstring& name);
  static RegKey OpenKey(RegKeyPtr parent, const std::wstring& name);
  static RegKey GetPredefinedKey(RegKeyPredefined, EventPtr);

  operator HKEY() const;

  EventPtr GetChangedEvent() const;

  bool CanCreateSubkey() const;
  bool CanEnumSubkey() const;
  bool CanNotify() const;
  bool CanQueryValue() const;
  bool CanSetValue() const;

  std::wstring FullName() const;
  const std::wstring& Name() const;

  RegKey(RegKey&&) noexcept;
  RegKey& operator=(RegKey&&) noexcept;

  ~RegKey();
  RegKey(const RegKey&) = delete;
  RegKey& operator=(const RegKey&) = delete;

private:
  HKEY __hKey = NULL;
  EventPtr __changedEvent;
  std::wstring __name;
  RegKeyPtr __parent;
  DWORD __accessRights = 0l;

  explicit RegKey(RegKeyPtr parent, const std::wstring& name);

  void Open();
  void Create();
};
