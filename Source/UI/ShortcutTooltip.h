#ifndef RAGNAROK_SOURCE_UI_SHORTCUTTOOLTIP_H
#define RAGNAROK_SOURCE_UI_SHORTCUTTOOLTIP_H

#include "Client/ClientOptions.h"
#include "Contents/ClientSkills.h"
#include "Contents/ItemInfo.h"
#include "Contents/ItemNaming.h"
#include "Contents/NormalItemSlots.h"
#include "UI/ShortcutGrid.h"

class UIWindow;
class LuaScript;
class ButtonServices;

struct ShortcutTooltip {
  ClientSkills& skills;
  const std::list<ItemInfo>& inventory;
  ItemNaming& naming;
  ClientOptions& options;
  LuaScript& script;
  ButtonServices* services;
  const bool& itemSlotActive;
  unsigned char& currentRow;

  void Hover(
    UIWindow& window,
    ShortcutGrid& grid,
    int x,
    int y,
    const NormalItemSlot* const* slots,
    const NormalItemSlot* const* itemSlots
  );
};

#endif
