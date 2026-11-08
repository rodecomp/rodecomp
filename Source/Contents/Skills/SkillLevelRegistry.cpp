#include "Contents/Skills/SkillInfo.h"
#include "Client/Registry.h"
#include "Client/Session.h"
#include "Contents/Skills/SkillLevelRegistry.h"
#include "Script/LuaScript.h"

#include <cstdio>
#include <cstring>
#include <string>
#include <windows.h>

// OFFSET: 0x00A92370
// FUNCTION: fn_17177
void LoadSkillUseLevelRegistry() {
  char key[512];
  std::strcpy(key, g_subKey);
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
    const int character = g_session.selectedCharacterSlot;
    const char* skill = CSkillInfo::IdName(*g_session.m_hotkeyScript, id);
    std::sprintf(name, "%s%d", skill, character);

    if (RegQueryValueExA(handle, name, nullptr, &type, reinterpret_cast<BYTE*>(&value), &size) != ERROR_SUCCESS || !CSkillInfo::UsesLevel(*g_session.m_hotkeyScript, id)) {
      return 0;
    }

    return value;
  };

  for (int id = 1; id < 7999; ++id) {
    g_session.m_skillBundle.SetUnknownValue(id, read(id));
  }

  for (int id = 8000; id < 8061; ++id) {
    g_session.SetHomunSkillLevel(id, read(id));
  }

  for (int id = 8200; id < 8458; ++id) {
    g_session.SetMercSkillLevel(id, read(id));
  }

  RegCloseKey(handle);
}
