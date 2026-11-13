#include "Contents/Skills/SkillInfo.h"
#include "Client/Application/Registry.h"
#include "Client/Session/Session.h"
#include "Contents/Skills/SkillLevelRegistry.h"
#include "Script/LuaScript.h"

#include <cstdio>
#include <cstring>
#include <string>
#include <windows.h>

// GLOBAL: gl_25596
const char* g_skillUseLevelInfoKeySuffix = "\\SkillUseLevelInfo";

// OFFSET: 0x00A92370
// FUNCTION: fn_17177
void LoadSkillUseLevelRegistry() {
  char key[512];
  std::strcpy(key, g_subKey);
  std::strcat(key, g_skillUseLevelInfoKeySuffix);
  HKEY probe;

  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, key, 0, KEY_QUERY_VALUE, &probe) != ERROR_SUCCESS) {
    return;
  }

  RegCloseKey(probe);

  HKEY handle;

  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, key, 0, KEY_QUERY_VALUE, &handle) != ERROR_SUCCESS) {
    return;
  }

  const auto read = [&](int id, int& value) {
    DWORD type = REG_DWORD;
    DWORD size = sizeof(int);
    char name[100] = {};
    const int character = g_session.selectedCharacterSlot;
    const char* skill = CSkillInfo::IdName(id);
    std::sprintf(name, "%s%d", skill, character);

    return RegQueryValueExA(handle, name, nullptr, &type, reinterpret_cast<BYTE*>(&value), &size) == ERROR_SUCCESS && CSkillInfo::UsesLevel(id);
  };

  int value;
  for (int id = 1; id < 7999; ++id) {
    if (read(id, value)) {
      g_session.m_skillBundle.SetSkillValue(id, value);
    } else {
      g_session.m_skillBundle.SetSkillValue(id, 0);
    }
  }

  for (int id = 8000; id < 8061; ++id) {
    if (read(id, value)) {
      g_session.SetHomunSkillLevel(id, value);
    } else {
      g_session.SetHomunSkillLevel(id, 0);
    }
  }

  for (int id = 8200; id < 8458; ++id) {
    if (read(id, value)) {
      g_session.SetMercSkillLevel(id, value);
    } else {
      g_session.SetMercSkillLevel(id, 0);
    }
  }

  RegCloseKey(handle);
}
