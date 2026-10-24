#include "Contents/SkillInfo.h"
#include "Contents/SkillLevelRegistry.h"
#include "Script/LuaScript.h"

#include <cstdio>
#include <cstring>
#include <string>
#include <windows.h>

// OFFSET: 0x00A92370
// FUNCTION: fn_17177
void SkillLevelRegistry::Load() {
  char key[512];
  std::strcpy(key, this->baseKey);
  std::strcat(key, "\\SkillUseLevelInfo");
  HKEY handle;

  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, key, 0, KEY_QUERY_VALUE, &handle) != ERROR_SUCCESS) {
    return;
  }

  RegCloseKey(handle);

  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, key, 0, KEY_QUERY_VALUE, &handle) != ERROR_SUCCESS) {
    return;
  }

  const auto read = [&](int id) {
    DWORD type = REG_DWORD;
    DWORD size = sizeof(int);
    int value;
    char name[100] = {};
    const int character = this->character;
    const char* skill = CSkillInfo::IdName(this->script, id);
    std::sprintf(name, "%s%d", skill, character);

    if (
      RegQueryValueExA(handle, name, nullptr, &type, reinterpret_cast<BYTE*>(&value), &size) != ERROR_SUCCESS ||
      !CSkillInfo::UsesLevel(this->script, id)
    ) {
      return 0;
    }

    return value;
  };

  for (int id = 1; id < 7999; ++id) {
    this->useLevels[id] = read(id);
  }

  for (int id = 8000; id < 8061; ++id) {
    this->homunculusLevels[id - 8000] = read(id);
  }

  for (int id = 8200; id < 8458; ++id) {
    this->otherLevels[id - 8200] = read(id);
  }

  RegCloseKey(handle);
}
