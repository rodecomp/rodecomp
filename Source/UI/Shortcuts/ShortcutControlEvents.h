#ifndef RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTCONTROLEVENTS_H
#define RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTCONTROLEVENTS_H

#include "UI/Shortcuts/ShortcutControls.h"
#include "UI/Shortcuts/ShortcutGrid.h"

class UIWindow;
struct ShortcutVisibility;
struct ShortcutCooldowns;

struct ShortcutControlEvents {
  UIWindow& window;
  ShortcutGrid& grid;
  ShortcutControls& controls;
  ShortcutVisibility& visibility;
  ShortcutCooldowns& cooldowns;
  bool& itemSlotActive;

  void Click(int id);
};

#endif
