#ifndef RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTDROP_H
#define RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTDROP_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Shortcuts/ShortcutBindings.h"
#include "UI/Shortcuts/ShortcutGrid.h"

class UIWindow;
struct ModeDragState;

struct ShortcutDrop {
  ShortcutBindings& bindings;
  LuaScript& script;
  ModeEvents*& mode;
  const std::list<ItemInfo>& inventory;
  const bool& itemSlotActive;
  unsigned char& currentRow;

  void Drop(UIWindow& window, ShortcutGrid& grid, int x, int y, const ModeDragState& value);
};

#endif
