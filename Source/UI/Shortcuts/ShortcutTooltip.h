#ifndef RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTTOOLTIP_H
#define RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTTOOLTIP_H

#include "Contents/Skills/ClientSkills.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Shortcuts/ShortcutGrid.h"

class UIWindow;
class LuaScript;
class ButtonServices;

struct ShortcutTooltip {
  ClientSkills& skills;
  const std::list<ItemInfo>& inventory;
  ItemNaming& naming;
  LuaScript& script;
  ButtonServices* services;
  const bool& itemSlotActive;
  unsigned char& currentRow;

  void Hover(UIWindow& window, ShortcutGrid& grid, int x, int y, const NormalItemSlot* const* slots, const NormalItemSlot* const* itemSlots);
};

#endif
