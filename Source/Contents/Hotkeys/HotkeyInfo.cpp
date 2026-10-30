#include "Contents/Hotkeys/HotkeyInfo.h"
#include "Script/LuaScript.h"

// OFFSET: 0x009109E0
// FUNCTION: fn_12506
HotkeyInfo::HotkeyInfo() : key1(0), key2(0) {
}

// OFFSET: 0x00910960
// FUNCTION: fn_12505
HotkeyInfo::HotkeyInfo(const HotkeyInfo& other) = default;

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
// FUNCTION: fn_10278
std::string SkillTooltipPercent(const char* hotkey, const char* name, int level, int sp, int ap) {
  char text[4096];
  if (ap != 0) {
    _snprintf_s(text, sizeof(text), _TRUNCATE, "%s %s Use Lv %d (Sp : %d%%, Ap : %d)", hotkey, name, level, sp, ap);
  } else {
    _snprintf_s(text, sizeof(text), _TRUNCATE, "%s %s Use Lv %d (Sp : %d%%)", hotkey, name, level, sp);
  }
  return text;
}

// OFFSET: 0x00822550
// FUNCTION: fn_10279
std::string SkillTooltipFlat(const char* hotkey, const char* name, int level, int sp, int ap) {
  char text[4096];
  if (ap != 0) {
    _snprintf_s(text, sizeof(text), _TRUNCATE, "%s %s Use Lv %d (Sp : %d, Ap : %d)", hotkey, name, level, sp, ap);
  } else {
    _snprintf_s(text, sizeof(text), _TRUNCATE, "%s %s Use Lv %d (Sp : %d)", hotkey, name, level, sp);
  }
  return text;
}

// INLINED
std::string SkillTooltip(const char* hotkey, const char* name, int level, int sp, int ap, bool percent) {
  if (percent) {
    return SkillTooltipPercent(hotkey, name, level, sp, ap);
  }
  return SkillTooltipFlat(hotkey, name, level, sp, ap);
}
