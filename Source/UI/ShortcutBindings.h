#ifndef RAGNAROK_SOURCE_UI_SHORTCUTBINDINGS_H
#define RAGNAROK_SOURCE_UI_SHORTCUTBINDINGS_H

#include "Contents/ClientSkills.h"
#include "Contents/NormalItemSlots.h"
#include "UI/ShortcutCooldowns.h"

class LuaScript;
class ModeEvents;
// Original binding banks and client rotation/use-level fields are borrowed.
struct ShortcutBindings {
  NormalItemSlot* banks[2];
  NormalItemSlot* items;
  const int& mode;
  const int& rotation0;
  const int& rotation1;
  ClientSkills& skills;
  ShortcutCooldowns& cooldowns;
  // Same live saved-use-level owner used by incoming skill updates.
  const SkillUseLevels& levels;

  int Rotate(int index, int bank) const;
  void Refresh(NormalItemSlot** slots, NormalItemSlot** itemSlots);
  void Assign(
    unsigned char type,
    std::uint32_t id,
    int level,
    int slot,
    int bank,
    const unsigned char& currentRow,
    LuaScript& script,
    ModeEvents& modeEvents
  );
  void AssignItem(std::uint32_t id, int slot);
};

#endif
