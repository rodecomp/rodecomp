#ifndef RAGNAROK_SOURCE_CONTENTS_HOTKEYS_HOTKEYINFO_H
#define RAGNAROK_SOURCE_CONTENTS_HOTKEYS_HOTKEYINFO_H

#include <string>

class LuaScript;

struct HotkeyInfo {
  int key1;
  int key2;
  std::string text;
  std::string description;
  HotkeyInfo();
  ~HotkeyInfo();
};

HotkeyInfo GetHotkeyInfo(LuaScript& script, int tab, int index);
std::string SkillTooltipPercent(const char* hotkey, const char* name, int level, int sp, int ap);
std::string SkillTooltipFlat(const char* hotkey, const char* name, int level, int sp, int ap);
std::string SkillTooltip(const char* hotkey, const char* name, int level, int sp, int ap, bool percent);
bool SkillUsesPercentSp(int id);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(HotkeyInfo) == 56);
#endif

#endif
