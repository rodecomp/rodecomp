#ifndef RAGNAROK_SOURCE_UI_SHORTCUTDETAILS_H
#define RAGNAROK_SOURCE_UI_SHORTCUTDETAILS_H

#include "Contents/ClientSkills.h"
#include "Contents/ItemInfo.h"
#include "Contents/NormalItemSlots.h"
#include "UI/ShortcutGrid.h"

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

  void Open(
    UIWindow& window,
    ShortcutGrid& grid,
    int x,
    int y,
    const NormalItemSlot* const* slots,
    const NormalItemSlot* const* itemSlots
  );
};

#endif
