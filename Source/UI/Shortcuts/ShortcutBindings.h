#ifndef RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTBINDINGS_H
#define RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTBINDINGS_H

#include "Contents/Skills/ClientSkills.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Shortcuts/ShortcutCooldowns.h"

class LuaScript;
class ModeEvents;

struct ShortcutBindings {
  NormalItemSlot* banks[2];
  NormalItemSlot* items;
  const int& mode;
  const int& rotation0;
  const int& rotation1;
  ClientSkills& skills;
  ShortcutCooldowns& cooldowns;

  const SkillUseLevels& levels;

  int Rotate(int index, int bank) const;
  void Refresh(NormalItemSlot** slots, NormalItemSlot** itemSlots);
  void Assign(unsigned char type, std::uint32_t id, int level, int slot, int bank, const unsigned char& currentRow, LuaScript& script, ModeEvents& modeEvents);
  void AssignItem(std::uint32_t id, int slot);
  void ClearAdditionalSkills(const unsigned char& currentRow, LuaScript& script, ModeEvents& modeEvents);
};

#endif
