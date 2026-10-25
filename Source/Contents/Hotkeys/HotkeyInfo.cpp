#include "Contents/Hotkeys/HotkeyInfo.h"
#include "Script/LuaScript.h"

#include <cstdio>

// OFFSET: 0x009109E0
// FUNCTION: fn_12506
HotkeyInfo::HotkeyInfo() : key1(0), key2(0) {
}

// OFFSET: 0x005F7C30
// FUNCTION: fn_04053
HotkeyInfo::~HotkeyInfo() = default;

// OFFSET: 0x00E09790
// FUNCTION: fn_23783
HotkeyInfo GetHotkeyInfo(LuaScript& script, int tab, int index) {
  HotkeyInfo value;
  const char* text = nullptr;
  const char* description = nullptr;

  if (script.Call("GetHotKey", "dd>ddss", tab + 1, index, &value.key1, &value.key2, &text, &description)) {
    value.text = text;
    value.description = description;
  }

  return value;
}

// OFFSET: 0x00E1DFD0
// FUNCTION: fn_23890
bool SkillUsesPercentSp(int id) {
  return id == 2039 || id == 2464 || id == 6528;
}

// OFFSET: 0x008224C0
// OFFSET: 0x00822550
std::string SkillTooltip(const char* hotkey, const char* name, int level, int sp, int ap, bool percent) {
  char text[4096];
  const char* format;
  if (percent) {
    format = ap != 0 ? "%s %s Use Lv %d (Sp : %d%%, Ap : %d)" : "%s %s Use Lv %d (Sp : %d%%)";
  } else {
    format = ap != 0 ? "%s %s Use Lv %d (Sp : %d, Ap : %d)" : "%s %s Use Lv %d (Sp : %d)";
  }

  _snprintf_s(text, sizeof(text), _TRUNCATE, format, hotkey, name, level, sp, ap);
  return text;
}
