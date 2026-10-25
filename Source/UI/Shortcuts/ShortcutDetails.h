#ifndef RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTDETAILS_H
#define RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTDETAILS_H

#include "Contents/Skills/ClientSkills.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Shortcuts/ShortcutGrid.h"

class UIWindow;
class WindowManagerWindows;
struct SkillDetailClose;

struct ShortcutDetails {
  WindowManagerWindows& windows;
  SkillDetailClose& skillClose;
  const std::uint32_t& describedSkill;
  ClientSkills& skills;
  const std::list<ItemInfo>& inventory;
  const bool& itemSlotActive;
  unsigned char& currentRow;
  const unsigned& screenWidth;
  const unsigned& screenHeight;

  void Open(UIWindow& window, ShortcutGrid& grid, int x, int y, const NormalItemSlot* const* slots, const NormalItemSlot* const* itemSlots);
};

#endif
