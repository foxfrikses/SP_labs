#include "RegKey.h"

#include <stdexcept>
#include <accctrl.h>
#include <aclapi.h>
#include <string>
#include <format>

inline static TRUSTEE CurrentUserTrustee = [] {
  static const std::wstring cu = L"CURRENT_USER";

  TRUSTEE t;
  t.pMultipleTrustee = nullptr;
  t.MultipleTrusteeOperation = NO_MULTIPLE_TRUSTEE;
  t.TrusteeForm = TRUSTEE_IS_NAME;
  t.TrusteeType = TRUSTEE_IS_USER;
  t.ptstrName = (LPWCH)cu.c_str();
  return t;
}();

DWORD GetRights(PACL pacl)
{
  DWORD rights;
  auto res = GetEffectiveRightsFromAcl(pacl, 
                                       &CurrentUserTrustee, 
                                       &rights);
  if (ERROR_SUCCESS == res) {
    return rights;
  }

  throw std::runtime_error(
    "Error while getting access rights: " +
    std::to_string(res)
  );
}

DWORD GetRegKeyRights(const std::wstring& name)
{
  PACL dacl; // dont need to free (it's inside of descr)
  PSECURITY_DESCRIPTOR descriptor; // need to free

  auto res = GetNamedSecurityInfo(
    name.c_str(),
    SE_REGISTRY_KEY,
    DACL_SECURITY_INFORMATION,
    NULL,
    NULL,
    &dacl,
    NULL,
    &descriptor
  );

  if (res != ERROR_SUCCESS) {
    throw std::runtime_error(
      "Error while getting security descriptor: " +
      std::to_string(res)
    );
  }

  DWORD rights = (dacl != NULL) ? GetRights(dacl) : 0;

  LocalFree(descriptor);

  return rights;
}

RegKey::RegKey(RegKeyPtr parent, const std::wstring& name)
  : __name(name)
  , __parent(parent)
{}

void RegKey::Open()
{
  if (__hKey) {
    return;
  }
  __accessRights = GetRegKeyRights(__name);

  auto status = RegOpenKeyEx(
    *__parent,
    __name.c_str(),
    0,
    __accessRights,
    &__hKey
  );

  if (status != ERROR_SUCCESS) {
    throw std::runtime_error(
      "Error while opening the registry key" +
      std::to_string(status)
    );
    // FORMAT_MESSAGE_FROM_SYSTEM 
  }
}

void RegKey::Create()
{
  if (__hKey) {
    return;
  }
  __accessRights = KEY_ALL_ACCESS;

  auto status = RegCreateKeyEx(
    *__parent,
    __name.c_str(),
    0,
    NULL,
    REG_OPTION_NON_VOLATILE,
    __accessRights,
    NULL,
    &__hKey,
    NULL // 
  );

  if (status != ERROR_SUCCESS) {
    throw std::runtime_error(
      "Error while creating the registry key" +
      std::to_string(status)
    );
    // FORMAT_MESSAGE_FROM_SYSTEM 
  }
}

RegKey RegKey::CreateKey(RegKeyPtr parent, const std::wstring& name)
{
  RegKey rk(parent, name);
  rk.Create();
  return rk;
}

RegKey RegKey::OpenKey(RegKeyPtr parent, const std::wstring& name)
{
  RegKey rk(parent, name);
  rk.Open();
  return rk;
}


const std::wstring& RegKey::Name() const
{
  return __name;
}

std::wstring RegKey::FullName() const
{
  std::wstring res; 
  if (__parent) {
    res = __parent->FullName();
    res.reserve(res.size() + 1 + __name.size() + 1);
    res += L'/';
    res += __name;
  }
  else {
    res = __name;
  }
  return res;
}

RegKey::operator HKEY() const
{
  return __hKey;
}

bool RegKey::CanCreateSubkey() const
{
  return __hKey != NULL && 
    (__accessRights & KEY_CREATE_SUB_KEY);
}

bool RegKey::CanEnumSubkey() const
{
  return __hKey != NULL && 
    (__accessRights & KEY_ENUMERATE_SUB_KEYS);
}

bool RegKey::CanNotify() const
{
  return __hKey != NULL && 
    (__accessRights & KEY_NOTIFY);
}

bool RegKey::CanQueryValue() const
{
  return __hKey != NULL && 
    (__accessRights & KEY_QUERY_VALUE);
}

bool RegKey::CanSetValue() const
{
  return __hKey != NULL && 
    (__accessRights & KEY_SET_VALUE);
}

EventPtr RegKey::GetChangedEvent() const
{
  if (CanNotify()) {
    return __changedEvent;
  }
  else {
    return nullptr;
  }
}

RegKey RegKey::GetPredefinedKey(RegKeyPredefined predef, EventPtr eventPtr)
{
  switch (predef) {
    case RegKeyPredefined::classesRoot:
      {
        auto rk = RegKey(nullptr, L"CLASSES_ROOT");
        rk.__hKey = HKEY_CLASSES_ROOT;
        rk.__changedEvent = eventPtr;
        return rk;
      }
      break;

    case RegKeyPredefined::currentConfig:
      {
        auto rk = RegKey(nullptr, L"CURRENT_CONFIG");
        rk.__hKey = HKEY_CURRENT_CONFIG;
        rk.__changedEvent = eventPtr;
        return rk;
      }
      break;

    case RegKeyPredefined::currentUser:
      {
        auto rk = RegKey(nullptr, L"CURRENT_USER");
        rk.__hKey = HKEY_CURRENT_USER;
        rk.__changedEvent = eventPtr;
        return rk;
      }
      break;

    case RegKeyPredefined::localMachine:
      {
        auto rk = RegKey(nullptr, L"LOCAL_MACHINE");
        rk.__hKey = HKEY_LOCAL_MACHINE;
        rk.__changedEvent = eventPtr;
        return rk;
      }
      break;

    case RegKeyPredefined::users:
      {
        auto rk = RegKey(nullptr, L"USERS");
        rk.__hKey = HKEY_USERS;
        rk.__changedEvent = eventPtr;
        return rk;
      }
      break;

    default:
      throw std::runtime_error("unexpected value of RegKeyPredefined");
  }
}

RegKey::~RegKey()
{
  if ((__hKey == HKEY_CLASSES_ROOT) ||
      (__hKey == HKEY_CURRENT_CONFIG) ||
      (__hKey == HKEY_CURRENT_USER) ||
      (__hKey == HKEY_LOCAL_MACHINE) ||
      (__hKey == HKEY_USERS)) {
    return;
  }

  RegCloseKey(__hKey);
}

RegKey::RegKey(RegKey&& newKey) noexcept
{
  *this = std::move(newKey);
}

RegKey& RegKey::operator=(RegKey&& newKey) noexcept
{
  std::swap(__hKey, newKey.__hKey);
  std::swap(__changedEvent, newKey.__changedEvent);
  std::swap(__name, newKey.__name);
  std::swap(__parent, newKey.__parent);
  std::swap(__accessRights, newKey.__accessRights);

  return *this;
}

