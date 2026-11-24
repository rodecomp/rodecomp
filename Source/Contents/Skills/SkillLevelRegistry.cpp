#include "Contents/Skills/SkillInfo.h"
#include "Client/Application/Registry.h"
#include "Client/Session/Session.h"
#include "UI/Window/UIWindowMgr.h"
#include "Script/LuaScript.h"

#include <cstdio>
#include <cstring>
#include <string>
#include <windows.h>

// GLOBAL: gl_25596
const char* g_skillUseLevelInfoKeySuffix = "\\SkillUseLevelInfo";

// OFFSET: 0x00A92370
// FUNCTION: fn_17177
void UIWindowMgr::LoadSkillUseLevelRegistry() {
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

  DWORD type;
  DWORD size;
  int value;
  const auto read = [&](int id) {
    type = REG_DWORD;
    size = sizeof(value);
    char name[100] = {};
    const int character = g_session.selectedCharacterSlot;
    const char* skill = CSkillInfo::IdName(id);
    std::sprintf(name, "%s%d", skill, character);

    return RegQueryValueExA(handle, name, nullptr, &type, reinterpret_cast<BYTE*>(&value), &size) == ERROR_SUCCESS;
  };

  for (int id = 1; id < 7999; ++id) {
    if (read(id)) {
      if (CSkillInfo::UsesLevel(id)) {
        g_session.m_skillBundle.SetSkillValue(id, value);
      } else {
        g_session.m_skillBundle.SetSkillValue(id, 0);
      }
    } else {
      g_session.m_skillBundle.SetSkillValue(id, 0);
    }
  }

  for (int id = 8000; id < 8061; ++id) {
    if (read(id)) {
      if (CSkillInfo::UsesLevel(id)) {
        g_session.SetHomunSkillLevel(id, value);
      } else {
        g_session.SetHomunSkillLevel(id, 0);
      }
    } else {
      g_session.SetHomunSkillLevel(id, 0);
    }
  }

  for (int id = 8200; id < 8458; ++id) {
    if (read(id)) {
      if (CSkillInfo::UsesLevel(id)) {
        g_session.SetMercSkillLevel(id, value);
      } else {
        g_session.SetMercSkillLevel(id, 0);
      }
    } else {
      g_session.SetMercSkillLevel(id, 0);
    }
  }

  RegCloseKey(handle);
}
