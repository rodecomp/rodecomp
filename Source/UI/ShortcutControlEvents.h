#ifndef RAGNAROK_SOURCE_UI_SHORTCUTCONTROLEVENTS_H
#define RAGNAROK_SOURCE_UI_SHORTCUTCONTROLEVENTS_H

#include "UI/ShortcutControls.h"
#include "UI/ShortcutGrid.h"

class UIWindow;
struct ShortcutVisibility;
struct ClientOptions;
struct ShortcutCooldowns;

struct ShortcutControlEvents {
  UIWindow& window;
  ShortcutGrid& grid;
  ShortcutControls& controls;
  ShortcutVisibility& visibility;
  ClientOptions& options;
  ShortcutCooldowns& cooldowns;
  bool& itemSlotActive;

  void Click(int id);
};

#endif
