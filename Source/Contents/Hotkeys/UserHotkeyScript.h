#ifndef RAGNAROK_SOURCE_CONTENTS_HOTKEYS_USERHOTKEYSCRIPT_H
#define RAGNAROK_SOURCE_CONTENTS_HOTKEYS_USERHOTKEYSCRIPT_H

#include "Script/LuaScript.h"

#include <json/value.h>
#include <string>

void ClearUserHotkeys();
void SaveUserHotkeys();
void __stdcall ChangeUserHotkey(int tab, int index, int key1, int key2, const std::string& description);
void __stdcall LoadUserHotkeyTab(Json::Value& values, int tab);

inline void ClearUserHotkeys(LuaScript&) {
  ClearUserHotkeys();
}

inline void SaveUserHotkeys(LuaScript&) {
  SaveUserHotkeys();
}

inline void ChangeUserHotkey(LuaScript&, int tab, int index, int key1, int key2, const std::string& description) {
  ChangeUserHotkey(tab, index, key1, key2, description);
}

inline void LoadUserHotkeyTab(LuaScript&, Json::Value& values, int tab) {
  LoadUserHotkeyTab(values, tab);
}

#endif
