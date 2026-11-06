#include "Utility/RegistryKey.h"

// OFFSET: 0x00AED000
// FUNCTION: fn_18150
bool RegistryKey::Create(HKEY parent, const char* subKey) {
  DWORD disposition;
  return RegCreateKeyExA(parent, subKey, 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, nullptr, &this->handle, &disposition) == ERROR_SUCCESS;
}

// OFFSET: 0x00AED030
// FUNCTION: fn_18151
bool RegistryKey::Open(HKEY parent, const char* subKey) {
  return RegOpenKeyExA(parent, subKey, 0, KEY_ALL_ACCESS, &this->handle) == ERROR_SUCCESS;
}

// OFFSET: 0x00AED050
// FUNCTION: fn_18152
bool RegistryKey::Query(const char* name, DWORD expectedType, BYTE* data, DWORD size) {
  DWORD type;
  if (this->handle == nullptr || RegQueryValueExA(this->handle, name, nullptr, &type, data, &size) != ERROR_SUCCESS) {
    return false;
  }
  return type == expectedType;
}

// OFFSET: 0x00AED0C0
// FUNCTION: fn_18153
bool RegistryKey::Set(const char* name, DWORD type, const BYTE* data, DWORD size) {
  return this->handle != nullptr && RegSetValueExA(this->handle, name, 0, type, data, size) == ERROR_SUCCESS;
}
