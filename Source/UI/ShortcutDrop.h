#ifndef RAGNAROK_SOURCE_UI_SHORTCUTDROP_H
#define RAGNAROK_SOURCE_UI_SHORTCUTDROP_H

#include "Contents/ItemInfo.h"
#include "UI/ShortcutBindings.h"
#include "UI/ShortcutGrid.h"

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
